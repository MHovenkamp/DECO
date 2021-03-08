#include "robot.hpp"

rtos::Thread servo_task;
rtos::Thread pir_task;
rtos::Thread robot_task;
rtos::Thread robot_control_task;
rtos::Thread timer_robot_task;

Servo neck_servo(4);
Servo head_servo(3);
PIRSensor pir_sensor(2);
Robot robot_test(head_servo,neck_servo,pir_sensor);
ServoPositions servo_positions;

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
    if( current_time_difference == 0){
      continue;
    }
    if(current_time_difference % robot_test.getWalkTime() == 0){
      robot_test.setState(ROBOT_STATES::REMINDER_WALK);
    } else if(current_time_difference % robot_test.getWaterTime() == 0){
      robot_test.setState(ROBOT_STATES::REMINDER_WATER);
    } else if(current_time_difference % robot_test.getBreakTime() == 0){
      robot_test.setState(ROBOT_STATES::REMINDER_BREAK);
    }
    rtos::ThisThread::sleep_for(MS(500));
  }
}

void robotTask(){
    while(true){
      robot_test.run();
    }
}

void robotControlTask(){
  while (true){
    robot_test.moveNeck(servo_positions.left_full);
    rtos::ThisThread::sleep_for(MS(5000));
    robot_test.moveNeck(servo_positions.right_full);
    rtos::ThisThread::sleep_for(MS(5000));
  }
}

void setup() {
  Serial.begin(9600);
  delay(2000);
  robot_test.setup();

  servo_task.start(servoTask);
  robot_task.start(robotTask);
  robot_control_task.start(robotControlTask);
  pir_task.start(PIRTask);
  timer_robot_task.start(timerRobotTask);
};


void loop(){}
