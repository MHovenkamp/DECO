#include "robot.hpp"

// TODO mutex for multiple i2c connections

rtos::Thread servo_task;
// rtos::Thread servo_head_task;
rtos::Thread pir_task;
rtos::Thread robot_task;
rtos::Thread robot_control_task;
rtos::Thread timer_robot_task;
rtos::Thread i2c_task;

Servo neck_servo(3);
Servo head_servo(4);
PIRSensor pir_sensor(2);
MicroLidar lidar;
OledScreen screen;
Robot robot_test(head_servo,neck_servo,pir_sensor,lidar,screen,5);
ServoPositions servo_positions;

ROBOT_STATES global_state;

/**
 * @brief i2c task, responsible for the oled and lidar i2c communication. 
 * 
 */
void i2cTask(){
  Serial.println("i2c task started");
  screen.setAnimation(ROBOT_FRAMES::FACE_IDLE);
  while(true){
    screen.flashOled();
    lidar.lidarTask();
    rtos::ThisThread::sleep_for(MS(500));
  }
}

/**
 * @brief servoTask, continues loop supporting the head and neck servo of the robot.
 * 
 */
void servoTask(){
  Serial.println("servo head task started");
  while(true){
    if( global_state != ROBOT_STATES::OFF ){
      neck_servo.servoTask();
      head_servo.servoTask();
      rtos::ThisThread::sleep_for(MS(20)); // 20 ms standard for sg932r TowerPro, change when changing servo
    }
  }
}


/**
 * @brief PIRTask, continues loop that polls the PIR sensor.
 * 
 */
void pirTask(){
  Serial.println("PIR task started");
  while(true){ 
    pir_sensor.PIRTask();
    rtos::ThisThread::sleep_for(MS(1000));
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
    rtos::ThisThread::sleep_for(MS(1000));
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
  delay(5000);
  while (true){
    if(lidar.getDistanceMM() != 8191 && lidar.getDistanceMM() != -1){
      robot_test.interactiveMode();
      rtos::ThisThread::sleep_for(MS(5000));
    }
    rtos::ThisThread::sleep_for(MS(500));
  }
}

void setup() {
  Serial.begin(9600);
  delay(2000);
  robot_test.setup();
  delay(2000);

  osStatus error;
  error = pir_task.start(pirTask);
  if(error){Serial.println(error);}
  error = servo_task.start(servoTask);
  if(error){Serial.println(error);}

  error = timer_robot_task.start(timerRobotTask);
  if(error){Serial.println(error);}
  error = robot_control_task.start(robotControlTask);
  if(error){Serial.println(error);}
  error = robot_task.start(robotTask);
  if(error){Serial.println(error);}
  error = i2c_task.start(i2cTask);
  if(error){Serial.println(error);}
};


void loop(){}
