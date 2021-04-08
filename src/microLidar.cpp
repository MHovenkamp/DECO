#include "microLidar.hpp"

void MicroLidar::setup(){
    if (!lidar.begin()) {
        if (!lidar.begin(address, true)){
            Serial.println(F("Failed to boot VL53L0X"));
            while(1);
        }
    }
    lidar.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);
};

int MicroLidar::getDistandeMM(){
    lidar.rangingTest(&distance, false);
    if( distance.RangeStatus != 4){
        return distance.RangeMilliMeter;
    } else{
        return -1;
    }
}