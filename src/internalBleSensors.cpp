#include "../include/internalBleSensors.hpp"

void InternalBLESensors::setup(){
    if (!HTS.begin() || !BARO.begin() ) {
        Serial.println("Failed to initialize humidity temperature sensor!");
        while (1);
    }
}

void InternalBLESensors::updateSensors(){
    temperature = HTS.readTemperature();
    humidity = HTS.readHumidity();
    baro_pressure = BARO.readPressure();
}

float InternalBLESensors::getTemperature(){
    return temperature;
}

float InternalBLESensors::getHumidity(){
    return humidity;
}

float InternalBLESensors::getBarometric(){
    return baro_pressure;
}