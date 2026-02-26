#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "config.h"

class MotorController {
  public:
    void init() {
      // Левый передний
      pinMode(EN_LF, OUTPUT);
      pinMode(IN_LF1, OUTPUT);
      pinMode(IN_LF2, OUTPUT);
      
      // Левый задний
      pinMode(EN_LB, OUTPUT);
      pinMode(IN_LB1, OUTPUT);
      pinMode(IN_LB2, OUTPUT);
      
      // Правый передний
      pinMode(EN_RF, OUTPUT);
      pinMode(IN_RF1, OUTPUT);
      pinMode(IN_RF2, OUTPUT);
      
      // Правый задний
      pinMode(EN_RB, OUTPUT);
      pinMode(IN_RB1, OUTPUT);
      pinMode(IN_RB2, OUTPUT);
      
      stopAll();
    }
    
    void stopAll() {
      // Левые
      digitalWrite(IN_LF1, LOW);
      digitalWrite(IN_LF2, LOW);
      digitalWrite(IN_LB1, LOW);
      digitalWrite(IN_LB2, LOW);
      
      // Правые
      digitalWrite(IN_RF1, LOW);
      digitalWrite(IN_RF2, LOW);
      digitalWrite(IN_RB1, LOW);
      digitalWrite(IN_RB2, LOW);
      
      // ШИМ в 0
      analogWrite(EN_LF, 0);
      analogWrite(EN_LB, 0);
      analogWrite(EN_RF, 0);
      analogWrite(EN_RB, 0);
    }
    
    // Установка скорости для одного мотора
    void setMotorSpeed(int enPin, int in1Pin, int in2Pin, int speed) {
      if (speed >= 0) {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
        analogWrite(enPin, constrain(speed, 0, MAX_PWM));
      } else {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
        analogWrite(enPin, constrain(-speed, 0, MAX_PWM));
      }
    }
    
    // Установка скоростей всех моторов (массив из 4 значений)
    void setAllSpeeds(int speeds[4]) {
      // speeds: [LF, LB, RF, RB]
      setMotorSpeed(EN_LF, IN_LF1, IN_LF2, speeds[0]);
      setMotorSpeed(EN_LB, IN_LB1, IN_LB2, speeds[1]);
      setMotorSpeed(EN_RF, IN_RF1, IN_RF2, speeds[2]);
      setMotorSpeed(EN_RB, IN_RB1, IN_RB2, speeds[3]);
    }
    
    // Преобразование скорости (м/с) в ШИМ
    int velocityToPWM(float velocity) {
      int pwm = (int)((velocity / MAX_VELOCITY) * MAX_PWM);
      return constrain(pwm, -MAX_PWM, MAX_PWM);
    }
    
    // Установка скоростей из массива скоростей (м/с)
    void setVelocities(float velocities[4]) {
      int speeds[4];
      for (int i = 0; i < 4; i++) {
        speeds[i] = velocityToPWM(velocities[i]);
      }
      setAllSpeeds(speeds);
    }
};

#endif
