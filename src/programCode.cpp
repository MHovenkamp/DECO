#include <Arduino.h>


struct code{
    String program_code = R"(
    SETUP:
        weatherstation PERIOD = 10 SECOND
        STATE = IDLE 
    LOOP:
        WAIT 5 SECOND
    EOF:

    )";
};