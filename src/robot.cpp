#include "../include/robot.hpp"

extern ROBOT_STATES global_state;

void Robot::setup(){
    Serial.println("---start setup robot---");
    // setup al peripherals
    internal_sensors.setup();
    face_screen.setup();
    lidar.setup();
    rtos::ThisThread::sleep_for(MS(1000)); // give time for i2C to get ready
    head_servo.setup();
    neck_servo.setup();
    // returnToStartPos();
    rtos::ThisThread::sleep_for(MS(2000)); // give time for servo's to move to strat position
    // play startup sound
    Songs songs;
    playSound<songs.start_up.size()>(songs.start_up);
    // seperate timers for each timed event
    start_time_timer_water = millis();
    start_time_timer_walk = millis();
    start_time_timer_break = millis();
    start_time_timer_weather = millis();
    // set states to IDLE to start robot
    current_state = ROBOT_STATES::IDLE;
    prev_state = ROBOT_STATES::IDLE;
    temp_state = ROBOT_STATES::IDLE;
    Serial.println("---finished setup robot---");
};

void Robot::run(){ // TODO robot keeps starting up in walkreminder mode
    if( pir_sensor.getLastMovement() >= 1200){ //after 20 minutes of no activity shut down.
        setState(ROBOT_STATES::OFF);
    }
    if( current_state != ROBOT_STATES::OFF){ // Check timers for timed actions
        if( millis() - start_time_timer_water >=  getWaterTime()){
            setState(ROBOT_STATES::REMINDER_WATER);
            start_time_timer_water = millis();
        } else if( millis() - start_time_timer_walk >=  getWalkTime()){
            setState(ROBOT_STATES::REMINDER_WALK);
            start_time_timer_walk = millis();
        } else if( millis() - start_time_timer_break >=  getBreakTime()){
            setState(ROBOT_STATES::REMINDER_BREAK);
            start_time_timer_break = millis();
        } else if( millis() - start_time_timer_weather >=  getWeatherStationTime()){
            setState(ROBOT_STATES::WEATHER_STATION);
            start_time_timer_weather = millis();
        }
    } else {
        start_time_timer_water = millis();
        start_time_timer_walk = millis();
        start_time_timer_break = millis();
        start_time_timer_weather = millis();
    }
    switch(current_state){
        case ROBOT_STATES::INTERACTIVE_MODE:
            interactiveMode();
            break;
        case ROBOT_STATES::IDLE:
            idleState();
            break;
        case ROBOT_STATES::REMINDER_BREAK:
            if(break_time_active){
                reminderBreak();
            } else {
                setState(prev_state);
            }
            break;
        case ROBOT_STATES::REMINDER_WATER:
            if(water_time_active){
                reminderWater();
            } else {
                setState(prev_state);
            }
            break;
        case ROBOT_STATES::REMINDER_WALK:
            if(walk_time_active){
                reminderWalk();
            } else { 
                setState(prev_state);
            }
            break;
        case ROBOT_STATES::WEATHER_STATION:
            if(weather_time_active){
                showWeatherStation();
            } else {
                setState(prev_state);
            }
            break;
        case ROBOT_STATES::OFF:
            shutDown();
            break;
        default:
            break;
    }
    rtos::ThisThread::sleep_for(MS(100));
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
unsigned int Robot::getWeatherStationTime(){
    return weather_station_time;
}

void Robot::setBreakTimeActive(bool active){
    break_time_active = active;
}
void Robot::setWalkTimeActive(bool active){
    walk_time_active = active;
}
void Robot::setWaterTimeActive(bool active){
    water_time_active = active;
}
void Robot::setWeatherStationActive(bool active){
    weather_time_active = active;
}

void Robot::setBreakTime(unsigned long time){
    break_time = time;
}
void Robot::setWalkTime(unsigned long time){
    break_time = time;
}
void Robot::setWaterTime(unsigned long time){
    break_time = time;
}
void Robot::setShutdownAfter(unsigned long time){
    shutdown_after = time;
}
void Robot::setWeatherStationTime(unsigned long time){
    weather_station_time = time;
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
void Robot::setWeatherStationDuration(unsigned long time){
    weather_station_duration = time;
}


void Robot::setState(ROBOT_STATES state){ // prev = IDLE temp = WEATHER current = WEATHER
    temp_state = current_state;
    current_state = state;
    if( temp_state != current_state){ // state change
        prev_state = temp_state;
    }
};
ROBOT_STATES Robot::getState(){
    return current_state;
};
void Robot::printState(ROBOT_STATES state_to_print){
    switch (state_to_print){
        case ROBOT_STATES::IDLE:
            Serial.println("IDLE");
            break;
        case ROBOT_STATES::INTERACTIVE_MODE:
            Serial.println("INTERACTIVE_MODE");
            break;
        case ROBOT_STATES::OFF:
            Serial.println("OFF");
            break;
        case ROBOT_STATES::REMINDER_BREAK:
            Serial.println("REMINDER_BREAK");
            break;
        case ROBOT_STATES::REMINDER_WALK:
            Serial.println("REMINDER_WALK");
            break;
        case ROBOT_STATES::REMINDER_WATER:
            Serial.println("REMINDER_WATER");
            break;
        case ROBOT_STATES::WEATHER_STATION:
            Serial.println("WEATHER_STATION");
            break;
        default:
            break;
        }
}

void Robot::moveHead( int pos ){
    int max_turn_up = 140;
    int max_turn_down = 40;
    if( pos >= max_turn_up){
        pos = max_turn_up;
    }
    if( pos <= max_turn_down){
        pos = max_turn_down;
    }
    head_servo.turnToDegree(pos);
};
void Robot::moveNeck( int pos ){
    neck_servo.turnToDegree(pos);
};

int Robot::getHeadPos(){
    return head_servo.getCurrentDegree();
}
int Robot::getNeckPos(){
    return neck_servo.getCurrentDegree();
}

int Robot::getLastMovementDetected(){
    return pir_sensor.getLastMovement();
}

void Robot::shutDown(){
    returnToStartPos();
    global_state = ROBOT_STATES::OFF;
    face_screen.clearScreen();
    Serial.println("shutdown acitve");
    while(pir_sensor.getLastMovement() >= shutdown_after){
        rtos::ThisThread::sleep_for(MS(500));
    }
    Serial.println("restart");
    global_state = ROBOT_STATES::IDLE;
    setState(ROBOT_STATES::IDLE);
};

void Robot::idleState(){
    // play the idle animation
    face_screen.setAnimation(ROBOT_FRAMES::FACE_IDLE);
    rtos::ThisThread::sleep_for(MS(2000));
    face_screen.setAnimation(ROBOT_FRAMES::FACE_BLINK);
};

void Robot::rngMovement(){
    // robot turns to random position, to simulate life likeness.
    srand( (unsigned)time(NULL) );
    unsigned int neck_servo_direction = (rand() % 30) + 1;
    unsigned int head_servo_direction = (rand() % 30) + 1;

    //decide if the robot moves left/up or right/down;
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
    moveNeck(neck_servo_direction);
    moveHead(head_servo_direction);
};

void Robot::returnToStartPos(){
    moveHead(90);
    moveNeck(90);
}

void Robot::reminderBreak(){
    Serial.println("Break reminder active, duration of: " + String(break_time_duration) + "MS");
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    returnToStartPos();
    rtos::ThisThread::sleep_for(MS(200));
    long unsigned int start_time = millis();
    face_screen.setAnimation(ROBOT_FRAMES::BREAK_REMINDER);
    while( (millis() - start_time) <= break_time_duration && current_state == ROBOT_STATES::REMINDER_BREAK){
        if(current_state!=ROBOT_STATES::REMINDER_BREAK){
            break;
        }
        rtos::ThisThread::sleep_for(MS(500));
    }
    if( current_state == ROBOT_STATES::REMINDER_BREAK){
        setState(prev_state);
    }
};

void Robot::reminderWalk(){
    Serial.println("Walk reminder active, duration of: " + String(walk_time_duration) + "MS");
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    returnToStartPos();
    rtos::ThisThread::sleep_for(MS(200));
    long unsigned int start_time = millis();
    face_screen.setAnimation(ROBOT_FRAMES::WALK_REMINDER);
    while( (millis() - start_time) <= walk_time_duration && current_state == ROBOT_STATES::REMINDER_WALK){
        if(current_state!=ROBOT_STATES::REMINDER_WALK){
            break;
        }
        rtos::ThisThread::sleep_for(MS(500));
    }
    if( current_state == ROBOT_STATES::REMINDER_WALK){
        setState(prev_state);
    }
};

void Robot::reminderWater(){
    Serial.println("Water reminder active, duration of: " + String(water_time_duration) + "MS");
    Songs songs;
    playSound<songs.notification.size()>(songs.notification);
    returnToStartPos();
    rtos::ThisThread::sleep_for(MS(200));
    long unsigned int start_time = millis();
    face_screen.setAnimation(ROBOT_FRAMES::WATER_REMINDER);
    while( (millis() - start_time) <= water_time_duration && current_state == ROBOT_STATES::REMINDER_WATER ){
        if(current_state!=ROBOT_STATES::REMINDER_WATER){
            break;
        }
        rtos::ThisThread::sleep_for(MS(500));
    }
    if( current_state == ROBOT_STATES::REMINDER_WATER){
        setState(prev_state);
    }
};

void Robot::showWeatherStation(){
    setState(ROBOT_STATES::WEATHER_STATION);
    Serial.println("showing weather station, duration of: " + String(weather_station_duration) + "MS");
    String weather_message_temp, weather_message_hum, weather_message_baro, weather_message;
    long unsigned int start_time = millis();
    while( (millis() - start_time) <= weather_station_duration && current_state == ROBOT_STATES::WEATHER_STATION ){
        internal_sensors.updateSensors();
        if(current_state!=ROBOT_STATES::WEATHER_STATION){
            break;
        }
        weather_message_temp = String(internal_sensors.getTemperature()) + " C\n";
        weather_message_hum = String(internal_sensors.getHumidity()) + " %\n";
        weather_message_baro = String(int(internal_sensors.getBarometric())) + " hPa\n";

        weather_message = weather_message_temp +weather_message_hum + weather_message_baro;
        char message[255];
        weather_message.toCharArray(message, 255);
        std::array<unsigned int, 3> line_lengths = {(weather_message_temp.length()), (weather_message_hum.length()), (weather_message_baro.length())};
        
        face_screen.setDisplayText<3>(message, line_lengths, 3);
        face_screen.flashTextDisplay();
        rtos::ThisThread::sleep_for(MS(500));
    }
    if( current_state == ROBOT_STATES::WEATHER_STATION ){
        setState(prev_state);
    }
}

bool Robot::interactiveMode(){
    Songs songs;
    int start_time = millis();
    if( current_state == ROBOT_STATES::REMINDER_BREAK ||
        current_state == ROBOT_STATES::REMINDER_WALK  ||
        current_state == ROBOT_STATES::REMINDER_WATER ||
        current_state == ROBOT_STATES::WEATHER_STATION){
            return false;
    } else {
        //set current point as start point
        found_object_x = neck_servo.getCurrentDegree(); 
        found_object_y = head_servo.getCurrentDegree();

        // check if an object within set range can be found.
        bool succesfull_object_detect = followClosestObject();
        Serial.println("interactive mode succesfull: " + String(succesfull_object_detect));
        if(succesfull_object_detect){
            playSound<songs.notification.size()>(songs.notification);
        }
        while(succesfull_object_detect && (millis() - start_time) <= interactive_mode_duration){
            // continue following object
            succesfull_object_detect = followClosestObject();
            moveHead(found_object_y);
            moveNeck(found_object_x);
            rtos::ThisThread::sleep_for(MS(100));
        }
        returnToStartPos();
        rtos::ThisThread::sleep_for(MS(100));
        setState(ROBOT_STATES::IDLE);
        Serial.println("interactive loop finished");
        returnToStartPos();
        return succesfull_object_detect;
    }
    setState(ROBOT_STATES::IDLE);
    Serial.println("interactive loop finished");
    return false;
}

bool Robot::followClosestObject(){
    int min_range = 50;
    int max_range = 300; // max distance in millimeters
    bool found_objects;
    int sensor_data = lidar.getDistanceMM();
    if( sensor_data > 0 && sensor_data <= max_range){ // -1 default result when error occured in reading data
        Serial.println("follow closest object " + String(sensor_data));
        found_objects = true;
    } else {
        found_objects = false;
    }
    if( !found_objects ){
        int number_of_loops = 2;
        int radius = 40; 
        int prev_x = 0, prev_y = 0;
        bool done = false;
        //start spiral motion, calculations return values of spiral with center point of (0,0). Currnet amount of points = 100
        for (float t = 0.0; t <= 1.0; t += 1.0/(100.0*number_of_loops)) {
            if(!done){
                int X=radius*t*sin(t*2*PI*number_of_loops);
                int Y=radius*t*cos(t*2*PI*number_of_loops);
                if(X != prev_x && Y != prev_y){
                    //because loop starts from center neck and ead coordinates have to be calculated using the middle and the spiral results.
                    tmp_x_coordinate = found_object_x+X;
                    tmp_y_coordinate = found_object_y+Y;
                    moveNeck(tmp_x_coordinate);
                    moveHead(tmp_y_coordinate);

                    // set searching for animations depending on looking directions
                    if((prev_x >= X)&&(prev_y >= Y)){
                        face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_LEFT_DOWN);
                    } else if((prev_x >= X)&& (prev_y <= Y)){
                        face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_LEFT_UP);
                    } else if((prev_x <= X) && (prev_y >= Y)){
                        face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_RIGHT_DOWN);
                    } else if((prev_x <= X) && (prev_y <= Y)){
                        face_screen.setAnimation(ROBOT_FRAMES::SEARCHING_RIGHT_UP);
                    }

                    //The spiral takes very short steps in the beginning so we speed up the timing when small steps are taken.
                    if(abs(prev_x - X) > 5 || abs(prev_y - Y) > 5){
                        rtos::ThisThread::sleep_for(MS(120));
                    } else{
                        rtos::ThisThread::sleep_for(MS(100));
                    }
                    sensor_data = lidar.getDistanceMM();
                    if( sensor_data > 0 && sensor_data <= max_range && sensor_data >= min_range){
                        Serial.println("Found object at distance " + String(sensor_data) );
                        found_object_x = neck_servo.getCurrentDegree();
                        found_object_y = head_servo.getCurrentDegree();
                        done = true;
                        found_objects = true;
                    }
                    prev_x = X;
                    prev_y = Y;
                }
            } else{
                break;
            }
        }
    }
    return found_objects;
}