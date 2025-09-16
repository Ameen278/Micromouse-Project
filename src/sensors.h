// sensors.h
// ================================

#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

// تعريف المنافذ للـ Ultrasonic sensors (يمين ويسار فقط)
const int ULTRASONIC_LEFT_TRIG  = 2;
const int ULTRASONIC_LEFT_ECHO  = 3;
const int ULTRASONIC_RIGHT_TRIG = 4;
const int ULTRASONIC_RIGHT_ECHO = 5;

// تعريف الدوال
void initSensors();
bool hasWallFront();
bool hasWallLeft();
bool hasWallRight();
int readUltrasonicDistance(int trigPin, int echoPin);
void printSensorReadings();
int getLeftWallDistance();
int getRightWallDistance();
int getFrontWallDistance();
bool isRobotCentered();
bool canMoveForward();

#endif
