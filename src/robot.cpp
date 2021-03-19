#include "../include/robot.hpp"

void Robot::setup(){
    head_servo.setup();
    neck_servo.setup();
    internal_sensors.setup();
    face_screen.setup();
};

void Robot::run(){
    internal_sensors.updateSensors();
    rtos::ThisThread::sleep_for(MS(1000));
    switch(current_state){
        case ROBOT_STATES::IDLE:
            Serial.println("IDLE");
            idleState();
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
        case ROBOT_STATES::WEATHER_STATION:
            Serial.println("WEATHER_STATION");
            showWeatherStation();
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

void Robot::setBreakTimeDuration(unsigned long time){
    break_time_duration = time;
}

void Robot::setWalkTimeDuration(unsigned long time){
    walk_time_duration = time;
}

void Robot::setWaterTimeDuration(unsigned long time){
    water_time_duration = time;
}


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

void Robot::rngMovement(){
    // robot looks around
    srand( (unsigned)time(NULL) );
    unsigned int neck_servo_direction = (rand() % 30) + 1;
    unsigned int head_servo_direction = (rand() % 30) + 1;

    unsigned int neck_servo_direction_plus_or_minus = rand() > (RAND_MAX / 2);
    unsigned int head_servo_direction_plus_or_minus = rand() > (RAND_MAX / 2);
    
    if(neck_servo_direction_plus_or_minus){
        neck_servo_direction = neck_servo.getCurrentDegree() - neck_servo_direction;
    } else{
        neck_servo_direction = neck_servo.getCurrentDegree() + neck_servo_direction;
    }

    if(head_servo_direction_plus_or_minus){
        head_servo_direction = head_servo.getCurrentDegree() - head_servo_direction;
    } else{
        head_servo_direction = head_servo.getCurrentDegree() + head_servo_direction;
    }
    if(head_servo_direction < 50){ head_servo_direction = 50; };
    if(head_servo_direction > 130){ head_servo_direction = 130; };

    neck_servo.turnToDegree(neck_servo_direction);
    head_servo.turnToDegree(head_servo_direction);
};

void Robot::returnToStartPos(){
    neck_servo.turnToDegree(90);
    head_servo.turnToDegree(90);
}

void Robot::reminderBreak(){
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= break_time_duration/SECOND){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.showAnimation<4>(animations.big_break);
    }
    returnToStartPos();
    setState(ROBOT_STATES::IDLE);
};

void Robot::reminderWalk(){
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= walk_time_duration/SECOND ){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.showAnimation<3>(animations.walk);
    }
    returnToStartPos();
    setState(ROBOT_STATES::IDLE);
};

void Robot::reminderWater(){
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= water_time_duration/SECOND ){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.showAnimation<3>(animations.water);
    }
    returnToStartPos();
    setState(ROBOT_STATES::IDLE);
};

void Robot::showWeatherStation(){
    String weather_message_temp = String(internal_sensors.getTemperature()) + " C\n";
    String weather_message_hum = String(internal_sensors.getHumidity()) + " %\n";
    String weather_message_baro = String(int(internal_sensors.getBarometric())) + " hPa\n";
    String weather_message = weather_message_temp +weather_message_hum + weather_message_baro;
    int str_len = weather_message.length() + 1;
    char message[str_len];
    weather_message.toCharArray(message, str_len);
    std::array<unsigned int, 3> line_lengths = {(weather_message_temp.length()), (weather_message_hum.length()), (weather_message_baro.length())};
    face_screen.showText<3>(message, line_lengths, 3);

}