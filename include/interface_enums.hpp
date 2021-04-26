# ifndef INTERFACE_ENUMS_HPP
# define INTERFACE_ENUMS_HPP

enum class PARSE_WORDS{
    weatherstation,
    walk_reminder,
    water_reminder,
    break_reminder,
    shut_down_after,
    STATE,
    IDLE,
    REMINDER_BREAK,
    REMINDER_WATER,
    REMINDER_WALK,
    WEATHER_STATION,
    INTERACTIVE_MODE,
    OFF,
    ACTIVE,
    NON_ACTIVE,
    PERIOD,
    DURATION,
    MINUTE_,
    HOUR_,
    SECOND_,
    MILLI_SECOND_,
    WAIT,
    interactive_mode,
    shutdown,
    move_head,
    move_neck
}

enum class NODE_TYPES{
    STATE_SETTER
    SETTER,
    COMMAND,
    WAIT,
    ERROR
}


#endif