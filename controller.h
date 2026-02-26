#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "config.h"
#include "kinematics.h"  // ВАЖНО: добавляем include

class RobotController {
  public:
    // Целевые скорости робота
    float targetVx = 0.0;    // (+) вперед
    float targetVy = 0.0;    // (+) влево
    float targetOmega = 0.0; // (+) поворот ПРОТИВ часовой
    
    // Текущие скорости робота (по энкодерам)
    float currentVx = 0.0;
    float currentVy = 0.0;
    float currentOmega = 0.0;
    
    // Целевые скорости колес
    float targetWheelSpeeds[4] = {0, 0, 0, 0};
    
    // Добавляем объект кинематики как член класса
    Kinematics kinematics;
    
    void updateCurrentVelocity(float wheelSpeeds[4]) {
      // Используем член класса
      kinematics.forwardKinematics(wheelSpeeds, currentVx, currentVy, currentOmega);
    }
    
    void computeTargetWheelSpeeds() {
      // Используем член класса
      kinematics.inverseKinematics(targetVx, targetVy, targetOmega, targetWheelSpeeds);
      
      // Отладка: выводим скорости колес
      Serial.print("Wheel speeds: ");
      Serial.print(targetWheelSpeeds[0]); Serial.print(", ");
      Serial.print(targetWheelSpeeds[1]); Serial.print(", ");
      Serial.print(targetWheelSpeeds[2]); Serial.print(", ");
      Serial.println(targetWheelSpeeds[3]);
    }
    
    void processSerialCommands() {
      if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        parseCommand(command);
      }
    }
    
    void parseCommand(String cmd) {
      // Формат: "vx,vy,omega"
      int firstComma = cmd.indexOf(',');
      int secondComma = cmd.indexOf(',', firstComma + 1);
      
      if (firstComma > 0 && secondComma > 0) {
        targetVx = constrain(cmd.substring(0, firstComma).toFloat(), 
                             -MAX_VELOCITY, MAX_VELOCITY);
        targetVy = constrain(cmd.substring(firstComma + 1, secondComma).toFloat(), 
                             -MAX_VELOCITY, MAX_VELOCITY);
        targetOmega = constrain(cmd.substring(secondComma + 1).toFloat(), 
                               -1.0, 1.0);  // ограничение угловой скорости
        
        Serial.print("CMD: Vx=");
        Serial.print(targetVx);
        Serial.print(" Vy=");
        Serial.print(targetVy);
        Serial.print(" W=");
        Serial.println(targetOmega);
      }
    }
    
    void printDebugInfo(float wheelVels[4], long encPos[4]) {
      static unsigned long lastPrint = 0;
      
      if (millis() - lastPrint > 500) {
        Serial.println("\n=== ROBOT STATUS ===");
        Serial.print("Target: Vx=");
        Serial.print(targetVx, 2);
        Serial.print(" Vy=");
        Serial.print(targetVy, 2);
        Serial.print(" W=");
        Serial.println(targetOmega, 2);
        
        Serial.print("Current: Vx=");
        Serial.print(currentVx, 2);
        Serial.print(" Vy=");
        Serial.print(currentVy, 2);
        Serial.print(" W=");
        Serial.println(currentOmega, 2);
        
        lastPrint = millis();
      }
    }
};

#endif
