
// main.cpp أو الـArduino Sketch الرئيسي لـMicromouse Robot
// هذا الكود يجمع كل الأجزاء: حساسات، رسم الخريطة، البحث عن المسار، والمحركات.
// افتراضيات: MAZE_SIZE=16, PWM_FREQ=1000, PWM_RESOLUTION=12 (من motor.h).
// استخدم ESP32 مع حساسات VL53L0X (TOF أمامي) وUltrasonic (يسار/يمين).
// الروبوت يبدأ في (0,0) مواجهاً شمال (dir=0).
// يتنقل حتى يصل إلى المركز، مع تحديث الخريطة والمسار.

#include <Arduino.h>  // لـsetup وloop في Arduino/ESP32
#include "sensors.h"  // حساسات المسافة
#include "motor.h"    // التحكم في المحركات
#include "mapping.h"  // تحديث الخريطة (updateWall)
#include "pathfinding.h"  // Flood Fill وgetNextDirection

// ثوابت إضافية (يمكن تعديلها)
#define MAZE_SIZE 16  // حجم المتاهة (16x16)
#define BASE_SPEED 2000  // سرعة أساسية للمحركات (0-4095 لـ12-bit PWM)
#define TURN_SPEED 1500  // سرعة الدوران
#define CELL_DISTANCE_CM 15  // مسافة خلية واحدة (حسب حجم المتاهة)
#define MOVE_DELAY_MS 500  // تأخير للحركة في خلية واحدة (تعديل حسب السرعة)

// متغيرات حالة الروبوت
int currentX = 0;  // إحداثي X الحالي
int currentY = 0;  // إحداثي Y الحالي
int currentDirection = 0;  // 0: شمال, 1: شرق, 2: جنوب, 3: غرب

// تهيئة في setup
void setup() {
  Serial.begin(115200);  // للتصحيح عبر USB
  delay(1000);  // تأخير للاستقرار
  
  initSensors();  // تهيئة الحساسات
  initMotors();   // تهيئة المحركات
  initFloodFill();  // تهيئة Flood Fill (قيم 255، مركز=0)
  
  Serial.println("Micromouse Robot Initialized. Starting at (0,0) facing North.");
}

// الدورة الرئيسية
void loop() {
  // طباعة القراءات الحالية للتصحيح
  printSensorReadings();
  
  // التحقق إذا وصلنا المركز
  if (isAtCenter(currentX, currentY)) {
    stopMotors();
    Serial.println("Reached Center! Stopping.");
    while (true) {}  // وقف الروبوت (يمكن تعديل للعودة)
  }
  
  // قراءة الحساسات وتحديث الخريطة بناءً على الاتجاه الحالي
  updateMazeBasedOnSensors();
  
  // إعادة حساب Flood Fill بعد التحديث
  floodFill(currentX, currentY);
  
  // طباعة Flood Array للتصحيح (اختياري، يمكن تعطيله للكفاءة)
  printFloodArray();
  
  // الحصول على الاتجاه التالي الأفضل
  int nextDir = getNextDirection(currentX, currentY);
  
  // حساب الدوران اللازم (فرق الاتجاهات)
  int turnAmount = (nextDir - currentDirection + 4) % 4;  // 0: مباشر, 1: يمين, 2: 180, 3: شمال
  
  // تنفيذ الدوران
  switch (turnAmount) {
    case 1:  // يمين
      turnRight(TURN_SPEED);
      delay(300);  // تأخير للدوران 90 درجة (تعديل حسب الروبوت)
      stopMotors();
      break;
    case 3:  // شمال (أو يسار)
      turnLeft(TURN_SPEED);
      delay(300);
      stopMotors();
      break;
    case 2:  // 180 درجة (يمين مرتين أو خلف)
      turnRight(TURN_SPEED);
      delay(600);  // تأخير لـ180
      stopMotors();
      break;
    default:  // 0: لا دوران
      break;
  }
  
  // تحديث الاتجاه الحالي
  currentDirection = nextDir;
  
  // التحقق من التمركز قبل الحركة (استخدام PID إذا لزم)
  adjustCentering();
  
  // الحركة للأمام إلى الخلية التالية
  if (canMoveForward()) {
    moveForward(BASE_SPEED);
    delay(MOVE_DELAY_MS);  // تأخير لعبور الخلية (حسب المسافة)
    stopMotors();
    
    // تحديث الموقع
    updatePosition(currentX, currentY, currentDirection);
    
    Serial.print("Moved to (");
    Serial.print(currentX);
    Serial.print(", ");
    Serial.print(currentY);
    Serial.println(")");
  } else {
    Serial.println("Cannot move forward! Wall detected.");
  }
  
  delay(100);  // تأخير صغير بين الدورات
}

// دالة لتحديث الخريطة بناءً على الحساسات والاتجاه الحالي
void updateMazeBasedOnSensors() {
  // تحويل الاتجاهات النسبية إلى مطلقة بناءً على currentDirection
  bool wallFront = hasWallFront();
  bool wallLeft = hasWallLeft();
  bool wallRight = hasWallRight();
  
  // الاتجاهات المطلقة (بناءً على الاتجاه الحالي)
  int absFront = currentDirection;
  int absLeft = (currentDirection + 3) % 4;  // -90 درجة
  int absRight = (currentDirection + 1) % 4;  // +90 درجة
  
  // تحديث الجدران
  updateWall(currentX, currentY, absFront, wallFront);
  updateWall(currentX, currentY, absLeft, wallLeft);
  updateWall(currentX, currentY, absRight, wallRight);
}

// دالة لضبط التمركز باستخدام PID (إذا كان غير مركز)
void adjustCentering() {
  if (!isRobotCentered()) {
    int leftDist = getLeftWallDistance();
    int rightDist = getRightWallDistance();
    if (leftDist > 0 && rightDist > 0) {
      int error = leftDist - rightDist;  // إذا إيجابي، يسار أبعد → يميل يمين
      int correction = computePID(0, error);  // هدف error=0
      
      // تعديل السرعة
      int leftSpeed = BASE_SPEED + correction;
      int rightSpeed = BASE_SPEED - correction;
      
      setMotorSpeed(0, constrain(leftSpeed, 0, 4095));  // موتور A (افتراضي يسار)
      setMotorSpeed(1, constrain(rightSpeed, 0, 4095));  // موتور B (يمين)
      
      delay(100);  // تأخير صغير للتصحيح
      stopMotors();
    }
  }
}