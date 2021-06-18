// ==========================================================================
// internalBLESensor.hpp
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

# ifndef INTERNALBLESENSORS_HPP
# define INTERNALBLESENSORS_HPP

#include "support.hpp"

/**
 * @brief InternalBLESensors class containing code for internal arduino nano 33 ble sense sensors: humidity, temperature, baro.
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