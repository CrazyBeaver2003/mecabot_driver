#ifndef CONFIG_H
#define CONFIG_H

// ===== ROBOT GEOMETRY =====
#define WHEEL_RADIUS 0.097
#define BASE_LENGTH 0.2
#define BASE_WIDTH 0.31

// ===== ENCODER =====

#define PPR_MOTOR 11
#define GEAR_RATIO 56
#define PULSES_PER_REV (PPR_MOTOR * GEAR_RATIO)

// ===== CONTROL =====
#define CONTROL_DT 0.02
#define MAX_VELOCITY 0.5
#define MAX_PWM 255

// ===== PIN DEFINITIONS =====
// Энкодеры (C1 и C2 для каждого мотора)
#define ENC_LF_A 18  // Левый передний (C1)
#define ENC_LF_B 22  // Левый передний (C2)

#define ENC_RF_A 19  // Правый передний (C1)
#define ENC_RF_B 23  // Правый передний (C2)

#define ENC_LB_A 20  // Левый задний (C1)
#define ENC_LB_B 24  // Левый задний (C2)

#define ENC_RB_A 21  // Правый задний (C1)
#define ENC_RB_B 25  // Правый задний (C2)

// Драйвер L298N 
#define EN_LF 2      // PWM левый передний
#define IN_LF1 35
#define IN_LF2 33

#define EN_RF 3      // PWM правый передний
#define IN_RF1 32
#define IN_RF2 34

#define EN_LB 4      // PWM левый задний
#define IN_LB1 39
#define IN_LB2 37

#define EN_RB 5      // PWM правый задний
#define IN_RB1 36
#define IN_RB2 38

#endif
