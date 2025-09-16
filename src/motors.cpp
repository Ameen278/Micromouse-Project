#include "motor.h"

// PID متغيرات
float Kp = 1.0, Ki = 0.0, Kd = 0.0;
float prevError = 0, integral = 0;

// تهيئة الموتور
void initMotors() {
  // تهيئة PWM لقناتين
  ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMA, 0);

  ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMB, 1);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

// دالة PID
int computePID(float target, float current) {
  float error = target - current;
  integral += error;
  float derivative = error - prevError;
  prevError = error;

  float output = Kp * error + Ki * integral + Kd * derivative;

  // قيد القيمة حسب الريزوليوشن
  output = constrain(output, -4095, 4095);

  return (int)output;
}

// دالة عامة لتشغيل موتور واحد
void setMotorSpeed(int motor, int speed) {
  bool dir = (speed >= 0);
  int pwmVal = abs(speed);

  if (motor == 0) {  // موتور A
    digitalWrite(AIN1, dir);
    digitalWrite(AIN2, !dir);
    ledcWrite(0, pwmVal);
  } else {           // موتور B
    digitalWrite(BIN1, dir);
    digitalWrite(BIN2, !dir);
    ledcWrite(1, pwmVal);
  }
}

// إيقاف الموتور
void stopMotors() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}

// حركة للأمام
void moveForward(int speed) {
  setMotorSpeed(0, speed);
  setMotorSpeed(1, speed);
}

// حركة للخلف
void moveBackward(int speed) {
  setMotorSpeed(0, -speed);
  setMotorSpeed(1, -speed);
}

// لف يمين (الموتور A قدام – B ورا)
void turnRight(int speed) {
  setMotorSpeed(0, speed);
  setMotorSpeed(1, -speed);
}

// لف شمال (الموتور B قدام – A ورا)
void turnLeft(int speed) {
  setMotorSpeed(0, -speed);
  setMotorSpeed(1, speed);
}
