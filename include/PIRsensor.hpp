// ==========================================================================
// PIRsensor.hpp
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