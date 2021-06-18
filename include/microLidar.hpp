// ==========================================================================
// microLidar.hpp
// Copyright (C) 2021  Maaike Hovenkamp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// ==========================================================================

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
    int getDistanceMM();
    
    /**
     * @brief lidar task, call from continues loop
     * 
     */
    void lidarTask();

};