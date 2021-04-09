#include "../include/robot.hpp"

extern ROBOT_STATES global_state;

void Robot::setup(){
    Serial.println("start setup robot");
    head_servo.setup();
    neck_servo.setup();
    internal_sensors.setup();
    face_screen.setup();
    lidar.setup();
    Songs songs;
    playSound<songs.start_up.size()>(songs.start_up);

    for(unsigned int i = 0; i<10; i++){
        for(unsigned int j = 0; j < 3; j++){
            surroundings_map[i][j] = -1;
        }
    }
    for(unsigned int x = 0; x < 10; x++){
        Serial.print("\n");
        for(unsigned int y = 0; y < 3; y++){
            Serial.print(surroundings_map[x][y]);
            Serial.print(" ");
        }
    }
    Serial.println("finished setup robot");
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
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    head_servo.turnToDegree(120);
    head_servo.turnToDegree(60);
    rtos::ThisThread::sleep_for(MS(2000));
    returnToStartPos();
    long unsigned int start_time = millis();
    while( (millis() - start_time)/SECOND <= break_time_duration/SECOND){
        rtos::ThisThread::sleep_for(MS(500));
        face_screen.showAnimation<4>(animations.big_break);
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
        face_screen.showAnimation<3>(animations.walk);
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
        face_screen.showAnimation<3>(animations.water);
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
    face_screen.showText<3>(message, line_lengths, 3);

}

void Robot::interactiveMode(){
    Serial.println("start interactive mode");
    scanSurroundings();
    findClosestObject();
    Serial.println("move to, x:y" + String(found_object_x*map_steps_neck)+":"+String(90 + (found_object_y*map_steps_head)));
    neck_servo.turnToDegree(found_object_x*map_steps_neck);
    head_servo.turnToDegree(90 + (found_object_y*map_steps_head));
    rtos::ThisThread::sleep_for(MS(3000));
    bool succesfull_object_detect = followClosestObject();
    Serial.println(succesfull_object_detect);
    while(succesfull_object_detect){
        Serial.println("in while");
        succesfull_object_detect = followClosestObject();
    }
}

void Robot::scanSurroundings(){
    int wait_after_movement_ms = 300;
    int current_distance;
    unsigned int first_index = 0;
    unsigned int second_index;
    bool movement_up = true;
    for(unsigned int i = 0; i <=180; i+=map_steps_neck){
        neck_servo.turnToDegree(i);
        second_index = 0;
        if(movement_up){
            for(unsigned int j = 90-map_steps_head; j<=90+map_steps_head; j+=map_steps_head){
                head_servo.turnToDegree(j);

                rtos::ThisThread::sleep_for(MS(wait_after_movement_ms));
                current_distance = lidar.getDistandeMM();

                Serial.println(String(first_index) + ", " + String(second_index) + ": "+ String(current_distance));
                Serial.println("current distance " + String(current_distance));

                surroundings_map[first_index][second_index] = current_distance;
                second_index ++;
            }
            movement_up = false;
        }else{
            for(unsigned int j = 90+map_steps_head; j>=90-map_steps_head; j-=map_steps_head){
                head_servo.turnToDegree(j);

                rtos::ThisThread::sleep_for(MS(wait_after_movement_ms));
                current_distance = lidar.getDistandeMM();
                
                Serial.println(String(first_index) + ", " + String(second_index) + ": "+ String(current_distance));
                Serial.println("current distance " + String(current_distance));

                surroundings_map[first_index][second_index] = current_distance;
                second_index ++;
            }
            movement_up = true;
        }
        first_index++;
        
    }
}

void Robot::findClosestObject(){
    int shortest_distance = infinity();
    for(unsigned int i = 0; i < 10; i++){
        for(unsigned int j = 0; j < 3; j++){
            if( surroundings_map[i][j] <= shortest_distance && surroundings_map[i][j] != -1&& surroundings_map[i][j] != 8191){
                shortest_distance = surroundings_map[i][j];
                found_object_x = i;
                found_object_y = j;
                distance_found_object = surroundings_map[i][j];
            }
        }
    }
    Serial.println("found x and y: " + String(found_object_x) + ":" + String(found_object_y));
}

bool Robot::followClosestObject(){
    Serial.println("start folow object");
    bool found_objects;
    int lowest_distance = distance_found_object + 100;

    int sensor_data = lidar.getDistandeMM();
    if( sensor_data != -1 ){
        if( abs(sensor_data - distance_found_object) <= 100 ){
            found_objects = true;
            Serial.println("object still close");
        } else{
            found_objects = false;
        }
    } else {
        found_objects = false;
    }
    if( found_objects == false ){
        Serial.println("start search for object");
        int number_of_loops = 2;
        int radius = 40; 
        int prev_x = 0, prev_y = 0;
        for (float t = 0.0; t <= 1.0; t += 1.0/(360.0*number_of_loops)) {
            int X=radius*t*sin(t*2*PI*number_of_loops);
            int Y=radius*t*cos(t*2*PI*number_of_loops);
            if(X != prev_x && Y != prev_y){
                Serial.println("x,y " + String(found_object_x+X) + ":" + String(found_object_y+Y));
                neck_servo.turnToDegree(found_object_x+X);
                head_servo.turnToDegree(found_object_y+Y);
                prev_x = X;
                prev_y = Y;
                sensor_data = lidar.getDistandeMM();
                if( sensor_data != -1 && sensor_data != 8191 ){
                    if( sensor_data < lowest_distance ){
                        lowest_distance = sensor_data;
                        found_object_x = 10 * X /180;
                        found_object_y = 3 * Y / 180;
                        break;
                    }
                }
            }
            rtos::ThisThread::sleep_for(MS(20));
        }
        // old linear movement
        // for(unsigned int x = x_lower; x <= x_higher; x+=5 ){
        //     neck_servo.turnToDegree(x);
        //     for(unsigned int y = y_lower; y <= y_higher; y+=5){
        //         head_servo.turnToDegree(y);
        //         rtos::ThisThread::sleep_for(MS(wait_after_movement_ms));
        //         sensor_data = lidar.getDistandeMM();
        //         rtos::ThisThread::sleep_for(MS(100));
        //         Serial.println("x,y " + String(x) + ":" + String(y) );
        //         Serial.println(sensor_data);
        //         if( sensor_data != -1 && sensor_data != 8191 ){
        //             if( sensor_data < lowest_distance ){
        //                 lowest_distance = sensor_data;
        //                 found_object_x = 10 * x /180;
        //                 found_object_y = 3 * y / 180;
        //                 Serial.println("new indexes, x:y " + String(found_object_x) + ":"+String(found_object_y));
        //                 stop = true;
        //                 break;
        //             }
        //         }
        //     }
        //     if(stop){
        //         break;
        //     }
        // }
        found_objects = true;
        difference_map_x = found_object_x;
        difference_map_y = found_object_y;
    }
    return found_objects;
}