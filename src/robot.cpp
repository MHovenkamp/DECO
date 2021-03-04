#include "../include/robot.hpp"

// TODO alle dingen die taak zijn global maken voor MBED OS

rtos::Thread head_servo_task;
rtos::Thread neck_servo_task;
rtos::Thread Pir_task;

void Robot::setup(){
    internal_sensors.setup();
    face_screen.setup();
    head_servo_task.start(headServoTask);
    neck_servo_task.start(neckServoTask);
    Pir_task.start(PIRTask);
}

void Robot::robotTask(){
    setup();
    while(true){
        idleState();
    }
}

void Robot::headServoTask(){
    while(true){
        head_servo.servoTask();
    }
}

void Robot::neckServoTask(){
    while(true){
        neck_servo.servoTask();
    }
}

void Robot::PIRTask(){
    while(true){
        pir_sensor.PIRTask();
    }
}

void Robot::idleState(){
    unsigned int amount_of_idle_face_rotations = 5;
    // play the idle animation
    for(unsigned int i = 0; i < amount_of_idle_face_rotations; i++){
        face_screen.showAnimation<animations.face_idle.size()>(animations.face_idle);
    }
    face_screen.showAnimation<animations.face_blink.size()>(animations.face_blink)
}

void Robot::rngMovementState(){
    // robot looks around
    srand( (unsigned)time(NULL) );
    unsigned int neck_servo_direction = (rand() % 180) + 1;
    unsigned int head_servo_direction = (rand() % 180) + 1;
    neck_servo.turnToDegree(neck_servo_direction);
    head_servo.turnToDegree(head_servo_direction);
}

void Robot::reminderBreak(){
    int minutes_of_break_time = 15;
    head_servo.turnToDegree(120);
    delay(500);
    head_servo.turnToDegree(60);
    int start_time = millis();
    while( (millis() - start_time)/1000 >= minutes_of_break_time*60 ){
        face_screen.showAnimation<animations.big_break.size()>(animations.big_break);
    }
}

void Robot::reminderWalk(){
    int minutes_of_break_time = 5;
    head_servo.turnToDegree(120);
    delay(500);
    head_servo.turnToDegree(60);
    int start_time = millis();
    while( (millis() - start_time)/1000 >= minutes_of_break_time*60 ){
        face_screen.showAnimation<animations.walk.size()>(animations.walk);
    }
}

void Robot::reminderWalk(){
    unsigned int amount_of_idle_face_rotations = 5;
    head_servo.turnToDegree(120);
    delay(500);
    head_servo.turnToDegree(60);
    int start_time = millis();
    for(unsigned int i = 0; i < amount_of_idle_face_rotations; i++){
        face_screen.showAnimation<animations.water.size()>(animations.water);
    }
}

