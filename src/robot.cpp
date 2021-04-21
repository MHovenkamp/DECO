#include "../include/robot.hpp"

extern ROBOT_STATES global_state;

void Robot::setup(){
    Serial.println("start setup robot");
    head_servo.setup();
    neck_servo.setup();
    rtos::ThisThread::sleep_for(MS(1000));
    internal_sensors.setup();
    face_screen.setup();
    lidar.setup();
    Songs songs;
    playSound<songs.start_up.size()>(songs.start_up);
    start_time_timer = millis();
    Serial.println("finished setup robot");
};

void Robot::run(){
    if( pir_sensor.getLastMovement() >= 1200){
        setState(ROBOT_STATES::OFF);
    }
    if( current_state != ROBOT_STATES::OFF){
      current_time_difference = (millis() - start_time_timer)/SECOND;
      int walk_time_seconds = getWalkTime()/SECOND;
      int water_time_seconds = getWaterTime()/SECOND;
      int break_time_seconds = getBreakTime()/SECOND;
      if( current_time_difference != 0){
        if(current_time_difference % walk_time_seconds == 0){
            setState(ROBOT_STATES::REMINDER_WALK);
        } else if(current_time_difference % water_time_seconds == 0){
            setState(ROBOT_STATES::REMINDER_WATER);
        } else if(current_time_difference % break_time_seconds == 0){
            setState(ROBOT_STATES::REMINDER_BREAK);
        } 
      }
    } else {
      start_time_timer = millis();
    }
    internal_sensors.updateSensors();
    switch(current_state){
        case ROBOT_STATES::INTERACTIVE_MODE:
            if(tmp_x_coordinate <= 90 && tmp_y_coordinate >= 90){ // upper left
                face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_LEFT_UP);
            } else if(tmp_x_coordinate >= 90 && tmp_y_coordinate >= 90){ // upper right
                face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_RIGHT_UP);
            } else if(tmp_x_coordinate <= 90 && tmp_y_coordinate <= 90){ // lower left
                face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_LEFT_DOWN);
            } else if(tmp_x_coordinate >= 90 && tmp_y_coordinate <= 90){ // lower right
                face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_RIGHT_DOWN);
            }
            face_screen.showAnimation();
            break;
        case ROBOT_STATES::IDLE:
            Serial.println("IDLE");
            idleState();
            break;
        case ROBOT_STATES::REMINDER_BREAK:
            if(break_time_active){
                Serial.println("REMINDER_BREAK");
                reminderBreak();
            } else {
                setState(ROBOT_STATES::IDLE);
            }
            break;
        case ROBOT_STATES::REMINDER_WATER:
            if(water_time_active){
                Serial.println("REMINDER_WATER");
                reminderWater();
            } else {
                setState(ROBOT_STATES::IDLE);
            }
            break;
        case ROBOT_STATES::REMINDER_WALK:
            if(walk_time_active){
                Serial.println("REMINDER_WALK");
                reminderWalk();
            } else {
                setState(ROBOT_STATES::IDLE);
            }
            break;
        case ROBOT_STATES::WEATHER_STATION:
            Serial.println("WEATHER_STATION");
            showWeatherStation();
            break;
        case ROBOT_STATES::OFF:
            Serial.println("OFF");
            shutDown();
            break;
        default:
            break;
    }
    rtos::ThisThread::sleep_for(MS(500));
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

void Robot::setBreakTime(unsigned long time, bool active){
    break_time = time;
    break_time_active = active;
};
void Robot::setWalkTime(unsigned long time, bool active){
    break_time = time;
    walk_time_active = active;
};
void Robot::setWaterTime(unsigned long time, bool active){
    break_time = time;
    water_time_active = active;
};

void Robot::setShutdownAfter(unsigned long time){
    shutdown_after = time;
}

void Robot::setBreakTimeDuration(unsigned long time){
    break_time_duration = time;
}

void Robot::setWalkTimeDuration(unsigned long time){
    walk_time_duration = time;
}

void Robot::setWaterTimeDuration(unsigned long time){
    water_time_duration = time;
}

void Robot::setInteractiveModeDuration(unsigned long time){
    interactive_mode_duration = time;
}


void Robot::setState(ROBOT_STATES state){
    current_state = state;
};
ROBOT_STATES Robot::getState(){
    return current_state;
};

void Robot::moveHead( int pos ){
    if( pos >= 140){
        pos = 140;
    }
    if( pos <= 40){
        pos = 40;
    }
    head_servo.turnToDegree(pos);
};
void Robot::moveNeck( int pos ){
    neck_servo.turnToDegree(pos);
};

void Robot::shutDown(){
    returnToStartPos();
    global_state = ROBOT_STATES::OFF;
    face_screen.clearScreen();
    while(pir_sensor.getLastMovement() >= shutdown_after){
        rtos::ThisThread::sleep_for(MS(500));
        Serial.println("shutdown acitve");

    }
    Serial.println("restart");
    global_state = ROBOT_STATES::IDLE;
    setState(ROBOT_STATES::IDLE);
};

void Robot::idleState(){
    unsigned int amount_of_idle_face_rotations = 5;
    // play the idle animation
    for(unsigned int i = 0; i < amount_of_idle_face_rotations; i++){
        face_screen.setAnimation(ROBOT_FRAMES::FACE_IDLE);
        face_screen.showAnimation();
    }
    face_screen.setAnimation(ROBOT_FRAMES::FACE_BLINK);
    face_screen.showAnimation();
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
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    rtos::ThisThread::sleep_for(MS(2000));
    returnToStartPos();
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= break_time_duration/SECOND){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.setAnimation(ROBOT_FRAMES::BREAK_REMINDER);
        face_screen.showAnimation();
    }
    setState(ROBOT_STATES::IDLE);
};

void Robot::reminderWalk(){
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    rtos::ThisThread::sleep_for(MS(2000));
    returnToStartPos();
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= walk_time_duration/SECOND ){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.setAnimation(ROBOT_FRAMES::WALK_REMINDER);
        face_screen.showAnimation();
    }
    setState(ROBOT_STATES::IDLE);
};

void Robot::reminderWater(){
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    rtos::ThisThread::sleep_for(MS(2000));
    returnToStartPos();
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= water_time_duration/SECOND ){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.setAnimation(ROBOT_FRAMES::WATER_REMINDER);
        face_screen.showAnimation();
    }
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
    face_screen.setDisplayText<3>(message, line_lengths, 3);

}

void Robot::interactiveMode(){
    Songs songs;
    int start_time = millis();
    Serial.println("starttime set: " + start_time);
    if( current_state == ROBOT_STATES::REMINDER_BREAK ||
        current_state == ROBOT_STATES::REMINDER_WALK ||
        current_state == ROBOT_STATES::REMINDER_WATER ||
        current_state == ROBOT_STATES::WEATHER_STATION){
            rtos::ThisThread::sleep_for(MS(500));
    } else {
        found_object_x = neck_servo.getCurrentDegree();
        found_object_y = head_servo.getCurrentDegree();
        bool succesfull_object_detect = followClosestObject();
        Serial.println("interactive mode " + String(succesfull_object_detect));
        if(succesfull_object_detect){
            playSound<songs.notification.size()>(songs.notification);
        }
        setState(ROBOT_STATES::INTERACTIVE_MODE);
        while(succesfull_object_detect){
            if((millis() - start_time)/SECOND >= interactive_mode_duration/SECOND ){
                break;
            }
            face_screen.setAnimation(ROBOT_FRAMES::FACE_IDLE);
            face_screen.showAnimation();
            succesfull_object_detect = followClosestObject();
            neck_servo.turnToDegree(found_object_x);
            head_servo.turnToDegree(found_object_y);
            face_screen.setAnimation(ROBOT_FRAMES::FACE_BLINK);
            face_screen.showAnimation();
            rtos::ThisThread::sleep_for(MS(500));
        }
        setState(ROBOT_STATES::IDLE);
    }
}

bool Robot::followClosestObject(){
    int max_range = 300;
    bool found_objects;
    int sensor_data = lidar.getDistanceMM();
    Serial.println("follow closest object " + String(sensor_data));
    if( sensor_data != 8191 && sensor_data != -1 && sensor_data <= max_range){
        found_objects = true;
    } else {
        found_objects = false;
    }
    if( found_objects == false ){
        int number_of_loops = 2;
        int radius = 40; 
        int prev_x = 0, prev_y = 0;
        bool done = false;
        for (float t = 0.0; t <= 1.0; t += 1.0/(100.0*number_of_loops)) {
            if(!done){
                int X=radius*t*sin(t*2*PI*number_of_loops);
                int Y=radius*t*cos(t*2*PI*number_of_loops);
                if(X != prev_x && Y != prev_y){
                    tmp_x_coordinate = found_object_x+X;
                    tmp_y_coordinate = found_object_y+Y;
                    neck_servo.turnToDegree(tmp_x_coordinate);
                    head_servo.turnToDegree(tmp_y_coordinate);
                    prev_x = X;
                    prev_y = Y;
                    sensor_data = lidar.getDistanceMM();
                    if( sensor_data != 8191 && sensor_data != -1 && sensor_data <= max_range){
                        found_object_x = neck_servo.getCurrentDegree();
                        found_object_y = head_servo.getCurrentDegree();
                        done = true;;
                    }
                }
                rtos::ThisThread::sleep_for(MS(80));
            } else{
                break;
            }
        }
        found_objects = true;
    }
    return found_objects;
}