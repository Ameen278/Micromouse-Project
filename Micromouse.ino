/*
v [1]
 */

#include <PID_v1.h>
#include "src/sensors.h"
#include "src/motors.h"
#include "src/mapping.h"
#include "src/pathfinding.h"

// المتغيرات الأساسية
const int MAZE_SIZE = 16;
int currentX = 0, currentY = 0;
int currentDirection = 0; // 0=شمال، 1=شرق، 2=جنوب، 3=غرب

void setup() {
  Serial.begin(9600);
  Serial.println("=== Micromouse Initialized ===");
  
  // تهيئة المكونات
  initSensors();
  initMotors();
  initMaze();
  
  Serial.println("All systems ready!");
  delay(1000);
}

void loop() {
  // الحلقة الرئيسية - سيتم تطويرها لاحقاً
  Serial.println("Main loop running...");
  delay(2000);
}