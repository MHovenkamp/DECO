#include <../include/PIRsensor.hpp>

void PIRSensor::PIRTask(){
    if( digitalRead(pir_pin) == HIGH){
        last_movement_time = millis();
    }
    delay(50);
    yield();
};

int PIRSensor::getLastMovement(){
    int time = millis() - last_movement_time;
    int seconds = time/1000;
    Serial.println(seconds);
    return seconds;
};