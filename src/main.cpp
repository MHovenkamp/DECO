#include "support.hpp"
#include "servo.hpp"
#include "PIRsensor.hpp"

PIRSensor pir_sensor(2);
Servo neck_servo(3);
Servo head_servo(4);

rtos::Thread neck_servo_thread;
rtos::Thread head_servo_thread;
rtos::Thread pir_sensor_thread;
rtos::Thread main_thread;

void neck_servo_loop(){
  while(true){
    Serial.println("neck");
    neck_servo.servoTask();
  }
};

void head_servo_loop(){
  while(true){
    Serial.println("head");
    head_servo.servoTask();}
};

void pir_sensor_loop(){
  while(true){
    Serial.println("pir");
    pir_sensor.PIRTask();
  }
}

void main_loop(){
  while(true){
      Serial.println("main");
    // create head servo, neck servo and PIR sensor, this is the robot loop
  for( unsigned int i = 0; i < 180; i++){
    neck_servo.turnToDegree(i);
    head_servo.turnToDegree(i);
    rtos::ThisThread::sleep_for(MS(15));
    };
  for( unsigned int i = 180; i > 0; i--){
    neck_servo.turnToDegree(i);
    head_servo.turnToDegree(i);
    rtos::ThisThread::sleep_for(MS(15));
  };
  rtos::ThisThread::sleep_for(MS(1000));
  rtos::ThisThread::yield();
  }
}

void setup() {
  Serial.begin(9600);
  neck_servo_thread.start(neck_servo_loop);
  head_servo_thread.start(head_servo_loop);
  pir_sensor_thread.start(pir_sensor_loop);
  main_thread.start(main_loop);
};

void loop(){}
