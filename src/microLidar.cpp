#include "microLidar.hpp"

void MicroLidar::setup(){
    if (!lidar.begin()) {
        if (!lidar.begin(address, true)){
            Serial.println(F("Failed to boot VL53L0X"));
            while(1);
        }
    }
};

int MicroLidar::getDistandeMM(){
    lidar.rangingTest(&distance, false);
    Serial.println(distance.RangeStatus);
    if( distance.RangeStatus != 4){
        return distance.RangeMilliMeter;
    } else{
        return -1;
    }
}