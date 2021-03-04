# ifndef INTERNALBLESENSORS_HPP
# define INTERNALBLESENSORS_HPP

#include "support.hpp"

/**
 * @brief inInternalBLESensors class cotnaining code for internal arduino nano 33 ble sense sensors: humidity, temperature, baro.
 * 
 */
class InternalBLESensors{
private:
    float temperature;
    float humidity;
    float baro_pressure;
public:
    /**
     * @brief Construct a new Internal B L E Sensors object
     * 
     */
    InternalBLESensors(){};

    /**
     * @brief setup the internal sensors
     * 
     */
    void setup();

    /**
     * @brief update sensor data, call every once in a while for accurate data
     * 
     */
    void updateSensors();

    /**
     * @brief Get the Temperature 
     * 
     * @return float 
     */
    float getTemperature();

    /**
     * @brief Get the Humidity 
     * 
     * @return float 
     */
    float getHumidity();

    /**
     * @brief Get the Barometric 
     * 
     * @return float 
     */
    float getBarometric();
};

#endif