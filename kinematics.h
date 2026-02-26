#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <Arduino.h>
#include "config.h"

class Kinematics {
  public:
    // Прямая кинематика: скорости колес (рад/с) -> скорость робота
    void forwardKinematics(float wheelRPM[4], float &vx, float &vy, float &omega) {
      // Переводим RPM в линейную скорость колеса (м/с)
      float wheelSpeeds[4];
      for (int i = 0; i < 4; i++) {
        wheelSpeeds[i] = wheelRPM[i] * WHEEL_RADIUS;  // ω * R = v
      }

      float L = BASE_LENGTH;
      float W = BASE_WIDTH;

      // Матричное преобразование
      // vx = (wLF + wLB + wRF + wRB) / 4
      vx = (wheelSpeeds[0] + wheelSpeeds[1] + wheelSpeeds[2] + wheelSpeeds[3]) / 4.0;

      // vy = (-wLF + wLB + wRF - wRB) / 4
      vy = (-wheelSpeeds[0] + wheelSpeeds[1] - wheelSpeeds[2] + wheelSpeeds[3]) / 4.0;

      // ω = (-wLF + wLB - wRF + wRB) / (4 * (L+W))
      omega = (-wheelSpeeds[0] + wheelSpeeds[1] - wheelSpeeds[2] + wheelSpeeds[3]) / (4.0 * (L + W));
    }

    // Обратная кинематика: скорость робота -> скорости колес (м/с)
    void inverseKinematics(float vx, float vy, float omega, float wheelSpeeds[4]) {
      float L = BASE_LENGTH;
      float W = BASE_WIDTH;

      // Матричное преобразование
      wheelSpeeds[0] = vx - vy - (L + W) * omega;  // LF
      wheelSpeeds[1] = vx + vy - (L + W) * omega;  // LB
      wheelSpeeds[2] = vx + vy + (L + W) * omega;  // RF
      wheelSpeeds[3] = vx - vy + (L + W) * omega;  // RB

      // Эти скорости уже в м/с
    }

    // Для отладки - выводим матрицу преобразования
    void printKinematicsMatrix() {
      Serial.println("=== Матрица прямой кинематики ===");
      Serial.println("[vx]    [ 1   1   1   1] [wLF]");
      Serial.println("[vy] =  [-1   1   1  -1] [wLB] * 1/4");
      Serial.println("[ω ]    [-1   1  -1   1] [wRF]");
      Serial.println("                       [wRB]");
      Serial.println();

      Serial.print("L = ");
      Serial.print(BASE_LENGTH);
      Serial.print(" м, W = ");
      Serial.print(BASE_WIDTH);
      Serial.print(" м, R = ");
      Serial.println(WHEEL_RADIUS);
      Serial.println("L+W = " + String(BASE_LENGTH + BASE_WIDTH));
    }
};

#endif
