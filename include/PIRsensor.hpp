# ifndef PIRSENSOR_HPP
# define PIRSENSOR_HPP

#include "support.hpp"

class PIRSensor{
private:
    int pir_pin;
    int last_movement_time; 
public:
    PIRSensor(int pir_pin):
    pir_pin(pir_pin){
        pinMode(pir_pin, INPUT);
    }

    void PIRTask();

    int getLastMovement();
};

#endif //PIRSENSOR_HPP