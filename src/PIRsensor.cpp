#include <../include/PIRsensor.hpp>

void PIRSensor::PIRTask(){
    if( digitalRead(pir_pin) == HIGH){
        last_movement_time = millis();
    }
    rtos::ThisThread::sleep_for(MS(50));
    rtos::ThisThread::yield();
};

int PIRSensor::getLastMovement(){
    int time = millis() - last_movement_time;
    int seconds = time/1000;
    return seconds;
};