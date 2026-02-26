#include "config.h"
#include "encoders.h"
#include "motors.h"
#include "kinematics.h"
#include "controller.h"

// Объекты классов
EncoderHandler encoders;
MotorController motors;
Kinematics kinematics;
RobotController robot;

// Для измерения времени
unsigned long lastTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("4WD Mecanum Robot Initializing...");
  
  // Инициализация
  encoders.init();
  motors.init();
  
  // Подключение прерываний для всех энкодеров
  attachInterrupt(digitalPinToInterrupt(ENC_LF_A), isrLF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_LB_A), isrLB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_RF_A), isrRF, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_RB_A), isrRB, CHANGE);
  
  lastTime = micros();
  
  Serial.println("Ready! Send commands: vx,vy,omega");
  Serial.println("Example: 0.2,0,0  (forward)");
  Serial.println("         0,0.2,0  (left)");
  Serial.println("         0,0,0.5  (rotate)");
}

void loop() {
  // Обработка команд
  robot.processSerialCommands();
  
  // Основной цикл управления
  unsigned long currentTime = micros();
  float dt = (currentTime - lastTime) / 1000000.0;
  
  if (dt >= CONTROL_DT) {
    // Получаем текущие скорости колес
    float wheelVels[4];
    encoders.getAllVelocities(dt, wheelVels);
    
    // Обновляем текущую скорость робота
    robot.updateCurrentVelocity(wheelVels);
    
    // Вычисляем целевые скорости колес
    robot.computeTargetWheelSpeeds();
    
    // Применяем управление
    motors.setVelocities(robot.targetWheelSpeeds);
    
    // Отладка
    long encPos[4];
    encoders.getPositions(encPos);
    robot.printDebugInfo(wheelVels, encPos);
    
    lastTime = currentTime;
  }
//    float test[4] = {100, 0, 0, 0};
//    motors.setAllSpeeds(test);
}

// Обработчики прерываний
void isrLF() { encoders.isrLF(); }
void isrLB() { encoders.isrLB(); }
void isrRF() { encoders.isrRF(); }
void isrRB() { encoders.isrRB(); }
