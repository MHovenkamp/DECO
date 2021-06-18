#include <Arduino.h>


struct code{
    String program_code = R"(
SETUP:
    interactive_mode DURATION = 20 SECOND
    weatherstation = NON_ACTIVE
    water_reminder = NON_ACTIVE
    break_reminder = NON_ACTIVE
    return_to_start_pos
LOOP:
    rng_movement
    WAIT 2 SECOND
    rng_movement
    WAIT 2 SECOND
    IF(getDistance < 150){
        interactive_mode
        WAIT 20 SECOND
    }
EOF:
    )";
};