#ifndef ENCODERS_H
#define ENCODERS_H
#include <Arduino.h>
#include "config.h"

class EncoderHandler {
  private:
    // Счетчики для 4 энкодеров
    volatile long countLF = 0; // Левый передний
    volatile long countLB = 0;  // Левый задний
    volatile long countRF = 0;  // Правый передний
    volatile long countRB = 0;  // Правый задний
    
    // Предыдущие значения для расчета скорости
    long lastLF = 0;
    long lastLB = 0;
    long lastRF = 0;
    long lastRB = 0;
    
  public:
    void init(){
      // Левая сторона
      pinMode(ENC_LF_A, INPUT_PULLUP);
      pinMode(ENC_LF_B, INPUT_PULLUP);
      pinMode(ENC_LB_A, INPUT_PULLUP);
      pinMode(ENC_LB_B, INPUT_PULLUP);
      
      // Правая сторона
      pinMode(ENC_RF_A, INPUT_PULLUP);
      pinMode(ENC_RF_B, INPUT_PULLUP);
      pinMode(ENC_RB_A, INPUT_PULLUP);
      pinMode(ENC_RB_B, INPUT_PULLUP);
    }

    // ISR для левого переднего
    void isrLF(){
      if (digitalRead(ENC_LF_A) == digitalRead(ENC_LF_B)){
        countLF++;
      }
      else {
        countLF--;    
      }
    }

    // ISR для левого заднего
    void isrLB() {
      if (digitalRead(ENC_LB_A) == digitalRead(ENC_LB_B)) {
        countLB++;
      } else {
        countLB--;
      }
    }
    
    // ISR для правого переднего
    void isrRF() {
      if (digitalRead(ENC_RF_A) == digitalRead(ENC_RF_B)) {
        countRF--;
      } else {
        countRF++;
      }
    }
    
    // ISR для правого заднего
    void isrRB() {
      if (digitalRead(ENC_RB_A) == digitalRead(ENC_RB_B)) {
        countRB--;
      } else {
        countRB++;
      }
    }

    // Получить скорость колеса (м/с)
    float getVelocityLF(float dt){
      long delta = countLF - lastLF;
      float wheelSpeed = delta / (float)PULSES_PER_REV / dt;
      lastLF = countLF;
      return wheelSpeed * 2 * PI * WHEEL_RADIUS;
    }
    float getVelocityLB(float dt) {
      long delta = countLB - lastLB;
      float wheelSpeed = delta / (float)PULSES_PER_REV / dt;
      lastLB = countLB;
      return wheelSpeed * 2 * PI * WHEEL_RADIUS;
    }
    
    float getVelocityRF(float dt) {
      long delta = countRF - lastRF;
      float wheelSpeed = delta / (float)PULSES_PER_REV / dt;
      lastRF = countRF;
      return wheelSpeed * 2 * PI * WHEEL_RADIUS;
    }
    
    float getVelocityRB(float dt) {
      long delta = countRB - lastRB;
      float wheelSpeed = delta / (float)PULSES_PER_REV / dt;
      lastRB = countRB;
      return wheelSpeed * 2 * PI * WHEEL_RADIUS;
    }

    // Получить все скорости сразу (массив)
    void getAllVelocities(float dt, float* velocities) {
      velocities[0] = getVelocityLF(dt);
      velocities[1] = getVelocityLB(dt);
      velocities[2] = getVelocityRF(dt);
      velocities[3] = getVelocityRB(dt);
    }

    // Получить позиции энкодеров
    void getPositions(long* positions) {
      noInterrupts();
      positions[0] = countLF;
      positions[1] = countLB;
      positions[2] = countRF;
      positions[3] = countRB;
      interrupts();
    }

    // Сброс счетчиков
    void reset() {
      noInterrupts();
      countLF = countLB = countRF = countRB = 0;
      lastLF = lastLB = lastRF = lastRB = 0;
      interrupts();
    }
};

#endif
