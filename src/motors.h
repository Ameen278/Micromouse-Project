#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// تعريف البنات (غيّرهم حسب التوصيل عندك)
// القناة A
#define AIN1 25
#define AIN2 26
#define PWMA 27   // PWM Channel A

// القناة B
#define BIN1 32
#define BIN2 33
#define PWMB 14   // PWM Channel B

// إعدادات PWM
#define PWM_FREQ 20000      // تردد PWM (20kHz)
#define PWM_RESOLUTION 12   // Resolution = 12 bit → 0–4095

// دوال التحكم
void initMotors();
void setMotorSpeed(int motor, int speed);   // motor = 0 → A , 1 → B
void stopMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);

// PID
int computePID(float target, float current);

#endif
