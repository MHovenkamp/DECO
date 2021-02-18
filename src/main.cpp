#include "support.hpp"
#include "servo.hpp"
#include "PIRsensor.hpp"

PIRSensor pir_sensor(10);
Servo neck_servo(11);
Servo head_servo(12);

void neck_servo_loop(){
  neck_servo.servoTask();
};

void head_servo_loop(){
  head_servo.servoTask();
};

void pir_sensor_loop(){
  pir_sensor.PIRTask();
}

void setup() {
  Serial.begin(9600);
  Scheduler.startLoop(neck_servo_loop);
  Scheduler.startLoop(head_servo_loop);
  Scheduler.startLoop(pir_sensor_loop);
};


void loop() {
  // create head servo, neck servo and PIR sensor, this is the robot loop
  for( unsigned int i = 0; i < 180; i++){
    neck_servo.turnToDegree(i);
    head_servo.turnToDegree(i);
    delay(15);
  };
  for( unsigned int i = 180; i > 0; i--){
    neck_servo.turnToDegree(i);
    head_servo.turnToDegree(i);
    delay(15);
  };
  pir_sensor.getLastMovement();
  delay(100);
  yield();

}