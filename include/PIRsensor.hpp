# ifndef PIRSENSOR_HPP
# define PIRSENSOR_HPP

#include "support.hpp"

/**
 * @brief control class for PIRSensor
 * 
 */
class PIRSensor{
private:
    const int pir_pin;
    int last_movement_time; 
public:
    /**
     * @brief Construct a new PIRSensor object
     * 
     * @param pir_pin, int digital pin
     */
    PIRSensor(int pir_pin):
    pir_pin(pir_pin){
        pinMode(pir_pin, INPUT);
    }

    /**
     * @brief continiues polling of the PIR sensor
     * 
     */
    void PIRTask();

    /**
     * @brief Get timedifference between last sensor detection and current time in seconds.
     * 
     * @return int 
     */
    unsigned int getLastMovement();
};

#endif //PIRSENSOR_HPP