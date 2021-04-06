#include <../include/PIRsensor.hpp>

void PIRSensor::PIRTask(){
    if( digitalRead(pir_pin) == HIGH){
        last_movement_time = millis();
    }
    rtos::ThisThread::sleep_for(MS(500));
    Serial.println("task active");
};

unsigned int PIRSensor::getLastMovement(){
    int time = millis() - last_movement_time;
    int seconds = time/SECOND;
    return seconds;
};