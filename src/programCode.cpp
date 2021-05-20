#include <Arduino.h>


struct code{
    String program_code = R"(
    SETUP:
        break_reminder PERIOD = 10 SECOND
        STATE = IDLE
        
    LOOP:
        move_neck 45
        WAIT 5 SECOND
        move_neck 135
        WAIT 5 SECOND
    EOF:

    )";
};