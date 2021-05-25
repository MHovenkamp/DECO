#include <Arduino.h>


struct code{
    String program_code = R"(
    SETUP:
        STATE = IDLE 
    LOOP:
        WAIT 5 SECOND
    EOF:

    )";
};