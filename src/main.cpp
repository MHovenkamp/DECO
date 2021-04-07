#include "robot.hpp"

rtos::Thread servo_neck_task;
rtos::Thread servo_head_task;
rtos::Thread pir_task;
rtos::Thread robot_task;
rtos::Thread robot_control_task;
rtos::Thread timer_robot_task;

Servo neck_servo(3);
Servo head_servo(4);
PIRSensor pir_sensor(2);
Robot robot_test(head_servo,neck_servo,pir_sensor,5);
ServoPositions servo_positions;

ROBOT_STATES global_state;

/**
 * @brief servoHeadTask, continues loop supporting the head servo of the robot.
 * 
 */
void servoHeadTask(){
  Serial.println("servo head task started");
  while(true){
    if( global_state != ROBOT_STATES::OFF ){
      head_servo.servoTask();
    }
  }
}

/**
 * @brief servoNeckTask, continues loop supporting the neck servo of the robot.
 * 
 */
void servoNeckTask(){
  Serial.println("servo neck task started");
  while(true){
    if( global_state != ROBOT_STATES::OFF ){
      neck_servo.servoTask();
    }
  }
}

/**
 * @brief PIRTask, continues loop that polls the PIR sensor.
 * 
 */
void PIRTask(){
  Serial.println("PIR task started");
  while(true){ 
    pir_sensor.PIRTask();
  }
}

/**
 * @brief timerRobotTask, continues loop polling time to trigger timed looped events.
 * 
 */
void timerRobotTask(){
  Serial.println("timed tasks started");
  unsigned long start_time = millis();
  unsigned long current_time_difference;
  while(true){
    rtos::ThisThread::sleep_for(MS(500));
    if( global_state != ROBOT_STATES::OFF ){
      current_time_difference = (millis() - start_time)/SECOND;
      int walk_time_seconds = robot_test.getWalkTime()/SECOND;
      int water_time_seconds = robot_test.getWaterTime()/SECOND;
      int break_time_seconds = robot_test.getBreakTime()/SECOND;
      if( current_time_difference == 0){
        continue;
      }
      if(current_time_difference % walk_time_seconds == 0){
        robot_test.setState(ROBOT_STATES::REMINDER_WALK);
      } else if(current_time_difference % water_time_seconds == 0){
        robot_test.setState(ROBOT_STATES::REMINDER_WATER);
      } else if(current_time_difference % break_time_seconds == 0){
        robot_test.setState(ROBOT_STATES::REMINDER_BREAK);
      }
    } else {
      start_time = millis();
    }
  }
}

/**
 * @brief robotTask, continues loop runnning the robot.
 * 
 */
void robotTask(){
  Serial.println("main robot task started");
  while(true){
    robot_test.run();
  }
}

/**
 * @brief robotControlTask, continues loop running the control of the user over the robot.
 * 
 */
void robotControlTask(){
  Serial.println("robot control task started");
  while (true){
    robot_test.interactiveMode();
    rtos::ThisThread::sleep_for(MS(500));
  }
}

void setup() {
  Serial.begin(9600);
  delay(2000);
  robot_test.setup();
  delay(2000);

  osStatus error;
  error = pir_task.start(PIRTask);
  if(error){Serial.println(error);}
  error = servo_neck_task.start(servoNeckTask);
  if(error){Serial.println(error);}
  error = servo_head_task.start(servoHeadTask);
  if(error){Serial.println(error);}

  error = timer_robot_task.start(timerRobotTask);
  if(error){Serial.println(error);}
  error = robot_control_task.start(robotControlTask);
  if(error){Serial.println(error);}
  error = robot_task.start(robotTask);
  if(error){Serial.println(error);}
};


void loop(){}
