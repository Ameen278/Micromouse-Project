
#include "motors.h"

// متغيرات PID
double Setpoint = 0, Input = 0, Output = 0;
PID myPID(&Input, &Output, &Setpoint, 2.0, 5.0, 1.0, DIRECT);

// متغيرات التحكم
int baseSpeed = 150;        // السرعة الأساسية
int turnSpeed = 100;        // سرعة الدوران
bool motorsInitialized = false;

void initMotors() {
  // تهيئة منافذ المحركات
  pinMode(MOTOR_LEFT_PWM, OUTPUT);
  pinMode(MOTOR_RIGHT_PWM, OUTPUT);
  pinMode(MOTOR_LEFT_DIR, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR, OUTPUT);
  
  // إيقاف المحركات في البداية
  stopMotors();
  
  // تهيئة PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);
  
  motorsInitialized = true;
  Serial.println("Motors initialized");
}

void moveForward() {
  if (!motorsInitialized) {
    Serial.println("Motors not initialized!");
    return;
  }
  
  // تشغيل المحركات للأمام
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  
  analogWrite(MOTOR_LEFT_PWM, baseSpeed);
  analogWrite(MOTOR_RIGHT_PWM, baseSpeed);
  
  Serial.println("Moving forward");
}

void turnLeft() {
  if (!motorsInitialized) {
    Serial.println("Motors not initialized!");
    return;
  }
  
  // المحرك الأيسر للخلف، الأيمن للأمام
  digitalWrite(MOTOR_LEFT_DIR, LOW);
  digitalWrite(MOTOR_RIGHT_DIR, HIGH);
  
  analogWrite(MOTOR_LEFT_PWM, turnSpeed);
  analogWrite(MOTOR_RIGHT_PWM, turnSpeed);
  
  Serial.println("Turning left");
  delay(500);  // وقت الدوران (يحتاج ضبط حسب الروبوت)
  stopMotors();
  delay(100);
}

void turnRight() {
  if (!motorsInitialized) {
    Serial.println("Motors not initialized!");
    return;
  }
  
  // المحرك الأيسر للأمام، الأيمن للخلف
  digitalWrite(MOTOR_LEFT_DIR, HIGH);
  digitalWrite(MOTOR_RIGHT_DIR, LOW);
  
  analogWrite(MOTOR_LEFT_PWM, turnSpeed);
  analogWrite(MOTOR_RIGHT_PWM, turnSpeed);
  
  Serial.println("Turning right");
  delay(500);  // وقت الدوران (يحتاج ضبط حسب الروبوت)
  stopMotors();
  delay(100);
}

void stopMotors() {
  analogWrite(MOTOR_LEFT_PWM, 0);
  analogWrite(MOTOR_RIGHT_PWM, 0);
  Serial.println("Motors stopped");
}

void setSpeed(int speed) {
  baseSpeed = constrain(speed, 0, 255);
  Serial.print("Speed set to: ");
  Serial.println(baseSpeed);
}