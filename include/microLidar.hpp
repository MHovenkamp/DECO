#include "support.hpp"

/**
 * @brief MicroLidar class for the VL53LOX chop
 * 
 */
class MicroLidar{
private:
    VL53L0X_RangingMeasurementData_t distance;
    const int address = 0x29;
    Adafruit_VL53L0X lidar = Adafruit_VL53L0X();
public:
    /**
     * @brief Construct a new Micro Lidar object
     * 
     */
    MicroLidar(){};

    /**
     * @brief Setup the MicroLidar
     * 
     */
    void setup();

    /**
     * @brief Get the Distande in milimeters.
     * 
     * @return int 
     */
    int getDistandeMM();
    
    /**
     * @brief lidar task, call from continues loop
     * 
     */
    void lidarTask();

};