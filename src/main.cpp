#include "robot.hpp"

rtos::Thread servo_task;
rtos::Thread pir_task;
rtos::Thread timer_robot_task;
rtos::Thread robot_task;

Servo neck_servo(3);
Servo head_servo(4);
PIRSensor pir_sensor(2);
Robot robot_test(head_servo,neck_servo,pir_sensor);

void servoTask(){
    while(true){
      head_servo.servoTask();
      neck_servo.servoTask();
    }
}

void PIRTask(){
    while(true){ pir_sensor.PIRTask();}
}

void timerRobotTask(){
  unsigned long start_time = millis();
  unsigned long current_time_difference;
  while(true){
    current_time_difference = millis() - start_time;
    if(current_time_difference % robot_test.getWalkTime() == 0){
      robot_test.setState(ROBOT_STATES::REMINDER_WALK);
    } else if(current_time_difference % robot_test.getWaterTime() == 0){
      robot_test.setState(ROBOT_STATES::REMINDER_WATER);
    } else if(current_time_difference % robot_test.getBreakTime() == 0){
      robot_test.setState(ROBOT_STATES::REMINDER_BREAK);
    }
  }
}

void robotTask(){
    robot_test.setup();
    while(true){
      robot_test.run();
    }
}

void setup() {
  Serial.begin(9600);
  delay(2000);

  robot_task.start(robotTask);
  servo_task.start(servoTask);
  pir_task.start(PIRTask);
  timer_robot_task.start(timerRobotTask);
};


void loop(){}
