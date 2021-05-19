#include <Arduino.h>


struct code{
    String program_code = R"(
    SETUP:
        shut_down_after = 10 MINUTE
        STATE = IDLE
        
    LOOP:
        move_neck 45
        WAIT 5 SECOND
        move_neck 135
        WAIT 5 SECOND
    EOF:

    )";
};