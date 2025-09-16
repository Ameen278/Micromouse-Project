#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <PID_v1.h>

// تعريف المنافذ
const int MOTOR_LEFT_PWM = 5;
const int MOTOR_RIGHT_PWM = 6;
const int MOTOR_LEFT_DIR = 7;
const int MOTOR_RIGHT_DIR = 8;

// متغيرات PID
extern double Setpoint, Input, Output;
extern PID myPID;

// تعريف الدوال
void initMotors();
void moveForward();
void turnLeft();
void turnRight();
void stopMotors();

#endif