// ================================
// sensors.cpp
// ================================

#include "sensors.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X tofFront;

// متغيرات المعايرة للـ Ultrasonic
const int WALL_THRESHOLD_CM = 15;    // عتبة وجود الجدار بالسنتيمتر
const int FRONT_THRESHOLD_CM = 10;   // عتبة الجدار الأمامي (أقرب)
const int MAX_DISTANCE = 200;        // أقصى مسافة قياس
const int SAMPLES = 3;               // عدد العينات للتصفية
const int SAMPLE_DELAY = 10;         // تأخير بين العينات

void initSensors() {
  // تهيئة منافذ Ultrasonic (يمين ويسار فقط)
  pinMode(ULTRASONIC_LEFT_TRIG, OUTPUT);
  pinMode(ULTRASONIC_LEFT_ECHO, INPUT);
  pinMode(ULTRASONIC_RIGHT_TRIG, OUTPUT);
  pinMode(ULTRASONIC_RIGHT_ECHO, INPUT);

  Serial.println("Sensors initialized");
  Serial.println("- Ultrasonic (Left/Right)");
  Serial.println("- TOF (Front)");

  // تهيئة I2C
  Wire.begin();
  if (!tofFront.init()) {
    Serial.println("Failed to detect TOF front sensor!");
    while (1) {} // وقف الكود لو الحساس مش متوصل
  }
  tofFront.setTimeout(500);
  tofFront.startContinuous();

  // اختبار أولي
  delay(100);
  Serial.print("Initial readings - Left: ");
  Serial.print(readUltrasonicDistance(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO));
  Serial.print("cm, Right: ");
  Serial.print(readUltrasonicDistance(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO));
  Serial.print("cm, Front(TOF): ");
  Serial.print(getFrontWallDistance());
  Serial.println("cm");
}


bool hasWallFront() {
  int frontDistance = getFrontWallDistance();
  return (frontDistance > 0 && frontDistance <= FRONT_THRESHOLD_CM);
}

bool hasWallLeft() {
  int distance = readUltrasonicDistance(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO);
  return (distance > 0 && distance <= WALL_THRESHOLD_CM);
}

bool hasWallRight() {
  int distance = readUltrasonicDistance(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO);
  return (distance > 0 && distance <= WALL_THRESHOLD_CM);
}

int readUltrasonicDistance(int trigPin, int echoPin) {
  long duration;
  int distance;
  long totalDistance = 0;
  int validReadings = 0;
  
  // أخذ عدة عينات للتصفية
  for (int i = 0; i < SAMPLES; i++) {
    // إرسال نبضة
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // قراءة المدة
    duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
    
    if (duration > 0) {
      distance = duration * 0.034 / 2; // تحويل إلى سنتيمتر
      
      // فلترة القراءات غير المعقولة
      if (distance > 2 && distance < MAX_DISTANCE) {
        totalDistance += distance;
        validReadings++;
      }
    }
    
    if (i < SAMPLES - 1) delay(SAMPLE_DELAY);
  }
  
  // إرجاع المتوسط أو -1 في حالة عدم وجود قراءات صحيحة
  return (validReadings > 0) ? (totalDistance / validReadings) : -1;
}

void printSensorReadings() {
  Serial.print("Sensors - L:");
  int leftDist = readUltrasonicDistance(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO);
  Serial.print(leftDist);
  Serial.print("cm R:");
  int rightDist = readUltrasonicDistance(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO);
  Serial.print(rightDist);
  Serial.print("cm F(TOF):");
  int frontDist = getFrontWallDistance();
  Serial.print(frontDist);
  Serial.print("cm | Walls - F:");
  Serial.print(hasWallFront() ? "Y" : "N");
  Serial.print(" L:");
  Serial.print(hasWallLeft() ? "Y" : "N");
  Serial.print(" R:");
  Serial.println(hasWallRight() ? "Y" : "N");
}


int getLeftWallDistance() {
  return readUltrasonicDistance(ULTRASONIC_LEFT_TRIG, ULTRASONIC_LEFT_ECHO);
}

int getRightWallDistance() {
  return readUltrasonicDistance(ULTRASONIC_RIGHT_TRIG, ULTRASONIC_RIGHT_ECHO);
}


int getFrontWallDistance() {
  int d = tofFront.readRangeContinuousMillimeters() / 10; // mm → cm
  if (tofFront.timeoutOccurred()) return -1;
  return d;
}


bool isRobotCentered() {
  int leftDist = getLeftWallDistance();
  int rightDist = getRightWallDistance();
  
  // إذا كان هناك جدارين على الجانبين
  if (leftDist > 0 && rightDist > 0 && 
      leftDist <= WALL_THRESHOLD_CM && rightDist <= WALL_THRESHOLD_CM) {
    
    // فرق المسافة يجب أن يكون أقل من 2 سم للاعتبار مركزياً
    return abs(leftDist - rightDist) < 2;
  }
  
  return true; // افتراض أنه مركزي إذا لم يكن هناك جدارين
}

bool canMoveForward() {
  int frontDist = getFrontWallDistance();
  return (frontDist < 0 || frontDist > FRONT_THRESHOLD_CM);
}