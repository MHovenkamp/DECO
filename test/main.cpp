#include "support.hpp"
#include "servo.hpp"

Servo neck_servo(3);
Servo head_servo(4);

rtos::Thread neck_servo_thread;
rtos::Thread head_servo_thread;

void neck_servo_loop(){
  while(true){
    neck_servo.servoTask();
  }
};

void head_servo_loop(){
  while(true){
    head_servo.servoTask();}
};

void setup() {
  Serial.begin(9600);
  neck_servo.turnToDegree(90);
  head_servo.turnToDegree(90);
  neck_servo_thread.start(neck_servo_loop);
  head_servo_thread.start(head_servo_loop);
};

void loop(){}
