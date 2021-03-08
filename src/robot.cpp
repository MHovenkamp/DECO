#include "../include/robot.hpp"

void Robot::setup(){
    head_servo.setup();
    neck_servo.setup();
    internal_sensors.setup();
    face_screen.setup();
};

void Robot::run(){
    internal_sensors.updateSensors();
    rtos::ThisThread::sleep_for(MS(500));
    switch(current_state){
        case ROBOT_STATES::IDLE:
            Serial.println("IDLE");
            idleState();
            break;
        case ROBOT_STATES::RNG_MOVEMENT:
            Serial.println("RNG_MOVEMENT");
            rngMovementState();
            break;
        case ROBOT_STATES::REMINDER_BREAK:
            Serial.println("REMINDER_BREAK");
            reminderBreak();
            break;
        case ROBOT_STATES::REMINDER_WATER:
            Serial.println("REMINDER_WATER");
            reminderWater();
            break;
        case ROBOT_STATES::REMINDER_WALK:
            Serial.println("REMINDER_WALK");
            reminderWalk();
            break;
        default:
            break;
    }
};

unsigned int Robot::getBreakTime(){
    return break_time;
};
unsigned int Robot::getWalkTime(){
    return walk_time;
};
unsigned int Robot::getWaterTime(){
    return water_time;
};

void Robot::setBreakTime(unsigned long time){
    break_time = time;
};
void Robot::setWalkTime(unsigned long time){
    break_time = time;
};
void Robot::setWaterTime(unsigned long time){
    break_time = time;
};

void Robot::setState(ROBOT_STATES state){
    current_state = state;
};
ROBOT_STATES Robot::getState(){
    return current_state;
};

void Robot::moveHead( int pos ){
    head_servo.turnToDegree(pos);
};
void Robot::moveNeck( int pos ){
    neck_servo.turnToDegree(pos);
};

void Robot::idleState(){
    unsigned int amount_of_idle_face_rotations = 5;
    // play the idle animation
    for(unsigned int i = 0; i < amount_of_idle_face_rotations; i++){
        face_screen.showAnimation<2>(animations.face_idle);
    }
    face_screen.showAnimation<5>(animations.face_blink);
};

void Robot::rngMovementState(){
    // robot looks around
    srand( (unsigned)time(NULL) );
    unsigned int neck_servo_direction = (rand() % 180) + 1;
    unsigned int head_servo_direction = (rand() % 180) + 1;
    neck_servo.turnToDegree(neck_servo_direction);
    head_servo.turnToDegree(head_servo_direction);
};

void Robot::reminderBreak(){
    long unsigned int minutes_of_break_time = 15;
    head_servo.turnToDegree(120);
    rtos::ThisThread::sleep_for(MS(500));
    head_servo.turnToDegree(60);
    long unsigned int start_time = millis();
    while( (millis() - start_time)/1000 >= minutes_of_break_time*60 ){
        face_screen.showAnimation<4>(animations.big_break);
    }
};

void Robot::reminderWalk(){
    long unsigned int minutes_of_break_time = 5;
    head_servo.turnToDegree(120);
    rtos::ThisThread::sleep_for(MS(500));
    head_servo.turnToDegree(60);
    long unsigned int start_time = millis();
    while( (millis() - start_time)/1000 >= minutes_of_break_time*60 ){
        face_screen.showAnimation<3>(animations.walk);
    }
};

void Robot::reminderWater(){
    unsigned int amount_of_idle_face_rotations = 5;
    head_servo.turnToDegree(120);
    rtos::ThisThread::sleep_for(MS(500));
    head_servo.turnToDegree(60);
    for(unsigned int i = 0; i < amount_of_idle_face_rotations; i++){
        face_screen.showAnimation<3>(animations.water);
    }
};

