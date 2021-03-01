# ifndef SUPPORT_HPP
# define SUPPORT_HPP

#include <Arduino.h>
#include "mbed.h"
#include "rtos.h"
#include <array>

//Oled screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MS(x) std::chrono::milliseconds(x)
#define US(x) std::chrono::microseconds(x)


#endif //SUPPORT_HPP