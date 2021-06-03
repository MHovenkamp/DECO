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
        IF(getState = IDLE){
            move_head 100
            WAIT 10 SECOND
            move_head 10
            WAIT 10 SECOND
        }
        interactive_mode
    EOF:

    )";
};