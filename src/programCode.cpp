#include <Arduino.h>


struct code{
    String program_code = R"(
SETUP:
    weatherstation = NON_ACTIVE
    walk_reminder = NON_ACTIVE
    weatherstation = NON_ACTIVE
    break_reminder = NON_ACTIVE

    weatherstation = ACTIVE
    walk_reminder = ACTIVE
    weatherstation = ACTIVE
    break_reminder = ACTIVE
 
    STATE = IDLE
    STATE = REMINDER_BREAK
    STATE = REMINDER_WATER
    STATE = REMINDER_WALK
    STATE = WEATHER_STATION
    STATE = INTERACTIVE_MODE
    STATE = OFF  

    STATE = IDLE

    shut_down_after = 10 MILLI_SECOND
    shut_down_after = 10 SECOND
    shut_down_after = 1 MINUTE
    shut_down_after = 1 HOUR

    weatherstation DURATION = 1 MINUTE
    walk_reminder DURATION = 1 MINUTE
    weatherstation DURATION = 1 MINUTE
    break_reminder DURATION = 1 MINUTE

    weatherstation PERIOD = 1 MINUTE
    walk_reminder PERIOD = 1 MINUTE
    weatherstation PERIOD = 1 MINUTE
    break_reminder PERIOD = 1 MINUTE

    interactive_mode DURATION = 2 MINUTE
LOOP:
EOF:
    )";
};