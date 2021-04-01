from enum import Enum

class LexerTokens(Enum):

    SETUP = "SETUP:"
    LOOP = "LOOP:"

    WEATHERSTATION = "weatherstation"
    WALKREMINDER = "walk_reminder"
    WEATHERREMINDER = "weather_reminder"
    BREAKREMINDER= "break_reminder"
    SCARNSURROUNDINGS = "scan_surroundings"
    FOLLOWOBJECT = "follow_object"

    ACTIVE = "ACTIVE"
    OFF = "OFF"
    DURATION = "DURATION"

    IS = "="

    MILLISECOND = "MILLISECOND"
    SECOND = "SECOND"
    MINUTE = "MINUTE"
    HOUR = "HOUR"

    IDLE = "IDLE"
    REMINDER_BREAK = "REMINDER_BREAK"
    REMINDER_WATER = "REMINDER_WATER"
    REMINDER_WALK = "REMINDER_WALK"
    WEATHER_STATION = "WEATHER_STATION"
    SHUTDOWNAFTER = "shut_down_after"
    OFF = "OFF"

    NEWLINE = "\n"

class ParserTokens(Enum):
    BASENODE = "BASE"
    TIMENODE = "TIMENODE"
    STARTSECTIONNODE = "STARTSECTION"
    ACTIVATENODE = "ACTIVATENODE"
    PERIODNODE = "PERIODNODE"
    DURATIONNODE = "DURATIONNODE"
    SETSTATENODE = "SETSTATE"
    FUNTIONCALLNODE = "FUNCTIONCALLNODE"



class Token(object):
    def __init__(self, token_value : str, token_type : LexerTokens):
        self.token_value = token_value
        self.token_type = token_type

    def __str__(self) -> str:
        return 'Token({value}, {token_type})'.format(
            value=self.token_value,
            token_type=self.token_type
        )

    def __repr__(self) ->str:
        return self.__str__()