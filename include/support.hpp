// ==========================================================================
// support.hpp
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

# ifndef SUPPORT_HPP
# define SUPPORT_HPP

#include <Arduino.h>
#include "mbed.h"
#include "rtos.h"
#include <array>
#include <time.h>
#include <memory>

//file reading
#include <fstream>

//Oled screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>

//Micro LIDAR
#include "Adafruit_VL53L0X.h"


//Internal humidity and temperature sensor
#include <Arduino_HTS221.h>
//internal baro sensor
#include <Arduino_LPS22HB.h>

// used in thread delays.
#define MS(x) std::chrono::milliseconds(x)

#define HOUR 3600000
#define MINUTE 60000
#define SECOND 1000
#define MIllI_SECOND 1

#endif //SUPPORT_HPP
