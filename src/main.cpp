#include "robot_interpreter.hpp"

rtos::Thread servo_task;
rtos::Thread pir_task;
rtos::Thread robot_task;
rtos::Thread robot_control_task;
rtos::Thread i2c_task;
rtos::Thread animation_task;

Servo neck_servo(3);
Servo head_servo(4);
PIRSensor pir_sensor(2);
MicroLidar lidar;
OledScreen screen;
Robot robot_test(head_servo,neck_servo,pir_sensor,lidar,screen,5);
ServoPositions servo_positions;
Interpreter interpreter(robot_test);

ROBOT_STATES global_state;

/**
 * @brief i2c task, responsible for the oled and lidar i2c communication. 
 * 
 */
void i2cTask(){
  Serial.println("i2c task started");
  while(true){
    if( global_state != ROBOT_STATES::OFF ){
      screen.flashOled();
      lidar.lidarTask();
    }
    rtos::ThisThread::sleep_for(MS(150));
  }
}

void animationTask(){
  Serial.println("animation task started");
  while(true){
    screen.showAnimation();
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
    interpreter.run();
  }
}

/**
 * @brief setup function containing the strat of the threads. 
 * 
 */
void setup(){
  Serial.begin(9600);
  delay(2000);
  robot_test.setup();

  osStatus error;
  error = pir_task.start(pirTask);
  if(error){Serial.println(error);}

  error = robot_control_task.start(robotControlTask);
  if(error){Serial.println(error);}
  error = robot_task.start(robotTask);
  if(error){Serial.println(error);}
  error = i2c_task.start(i2cTask);
  if(error){Serial.println(error);}
  error = animation_task.start(animationTask);
  if(error){Serial.println(error);}
  
  //wait till servos are at 90 degrees before starting the task
  while(neck_servo.getCurrentDegree() != 90 && head_servo.getCurrentDegree() != 90){}
  error = servo_task.start(servoTask);
  if(error){Serial.println(error);}
};


/**
 * @brief loop function not in use due to use of MBED OS.
 * 
 */
void loop(){}
