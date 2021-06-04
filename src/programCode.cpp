#include <Arduino.h>


struct code{
    String program_code = R"(
    SETUP:
        STATE = IDLE
        rng_movement
        WAIT 3 SECOND
        return_to_start_pos
        playSound notification
    LOOP:
        WHILE(getState = IDLE){
            rng_movement
            WAIT 3 SECOND
            return_to_start_pos
            WAIT 10 SECOND
            rng_movement
        }
        IF(getState = WEATHER_STATION){
            playSound notification
        }
        interactive_mode
    EOF:
    )";
};