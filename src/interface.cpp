#include <interface.hpp>

void Interface::run(){
    // check for repl mode or full file mode.
    if(mode == INTERFACE_MODES::IDLE){
        String choice = "";
        Serial.println("Enter mode to which to program robot with: ");
        Serial.println("1. REPL, Enter a single command and instantly see the result");
        Serial.println("2. FILE, upload a file with your written code");
        if(Serial.available()){
            choice = Serial.readString();
        }
        if( choice == "FILE"){ mode = INTERFACE_MODES::FILE; }
        else if( choice == "REPL"){mode = INTERFACE_MODES::REPL; };
    }
    switch (mode)
    {
    case INTERFACE_MODES::REPL :
        repl();
        break;
    case INTERFACE_MODES::FILE :
        file();
        break;
    default:
        break;
    }
};

void Interface::repl(){
    Serial.println("Enter command.");
    String command = "";
    if(Serial.available()){
        command = Serial.readString();
    }
    Node parsed_command = parseCommand(command); 
    parsed_command.execute();
}

Node Interface::parseCommand(String command){
    int amount_of_spaces = 0;
    for(unsigned int i = 0; i < command.length(); i++){
        if( command[i] == ' '){
            amount_of_spaces++;
        }
    }
    String string_array[amount_of_spaces+1] = {};
    int current_array_index = 0;
    int start_index = 0;
    for(unsigned int i = 0; i < command.length(); i++){
        if( command[i] == ' '){
            string_array[current_array_index] = command.substring(start_index,i-1);
            current_array_index++;
            start_index = i;
        }
    }

    switch (amount_of_spaces+1)
    {
    case 1:
        if(string_array[0] == "interactive_mode" || string_array[0] == "shutdown"){
            return CommandNode(command, string_array[0], 0, false);
        } 
        return ErrorNode(command, "unknown command");
        break;
    case 2:
        char letter;
        for(unsigned int i =0 ; i < string_array[1].length()-1; i++){
            letter = string_array[1][i];
            if(!isDigit(letter)){
                return ErrorNode(command, "time value not a digit");
                break;
            }
        }
        return CommandNode(command, string_array[0], string_array[1].toInt(), true);
        break;
    case 3:
        if(string_array[1] == "="){
            // STATE = IDLE
            if( string_array[0] == "STATE" && string_array[1] == "="){
                if(string_array[2] == "IDLE"){
                    return SetStateNode(command, PARSE_WORDS::IDLE);
                } else if(string_array[2] == "REMINDER_BREAK"){
                    return SetStateNode(command, PARSE_WORDS::REMINDER_BREAK);
                } else if(string_array[2] == "REMINDER_WATER"){
                    return SetStateNode(command, PARSE_WORDS::REMINDER_WATER);
                } else if(string_array[2] == "REMINDER_WALK"){
                    return SetStateNode(command, PARSE_WORDS::REMINDER_WALK);
                } else if(string_array[2] == "WEATHER_STATION"){
                    return SetStateNode(command, PARSE_WORDS::WEATHER_STATION);
                } else if(string_array[2] == "INTERACTIVE_MODE"){
                    return SetStateNode(command, PARSE_WORDS::INTERACTIVE_MODE);
                } else {
                    return ErrorNode(command, "unknown state");
                }
            }
            // weatherstation = ACTIVE
            if(string_array[-1] == "ACTIVE"){
                return SetterNode(command, string_array[0],PARSE_WORDS::ACTIVE, 0 , PARSE_WORDS::MILLI_SECOND_ );
            } else if(string_array[-1] == "NON_ACTIVE"){
                return SetterNode(command, string_array[0],PARSE_WORDS::NON_ACTIVE, 0 , PARSE_WORDS::MILLI_SECOND_ );
            }
        } else { 
            // WAIT 10 MINUTES
            if(string_array[0] == "WAIT"){
                char letter;
                for(unsigned int i =0 ; i < string_array[1].length()-1; i++){
                    letter = string_array[1][i];
                    if(!isDigit(letter)){
                        return ErrorNode(command, "time value not a digit");
                        break;
                    }
                }
                if(string_array[2] == "MINUTE"){
                    return WaitNode(command, string_array[1].toInt(), PARSE_WORDS::MINUTE_);
                } else if(string_array[2] == "HOUR"){
                    return WaitNode(command, string_array[1].toInt(), PARSE_WORDS::HOUR_);
                } else if (string_array[2] == "SECOND"){
                    return WaitNode(command, string_array[1].toInt(), PARSE_WORDS::SECOND_);
                } else if (string_array[2] == "MIllI_SECOND"){
                    return WaitNode(command, string_array[1].toInt(), PARSE_WORDS::MILLI_SECOND_);
                } else {
                    return ErrorNode(command, "Unknown time measurement type");
                }
            }
        }
        return ErrorNode(command, "unknown command");
    case 4:
        //shut_down_after = 10 MINUTE
        if(string_array[0] == "shut_down_after" && string_array[1] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[2].length()-1; i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    return ErrorNode(command, "time value not a digit");
                    break;
                }
            }
            if(string_array[3] == "MINUTE"){
                    return SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::MINUTE_);
                } else if(string_array[3] == "HOUR"){
                    return SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::HOUR_);
                } else if (string_array[3] == "SECOND"){
                    return SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::SECOND_);
                } else if (string_array[3] == "MIllI_SECOND"){
                    return SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::MILLI_SECOND_);
                } else {
                    return ErrorNode(command, "Unknown time measurement type");
                }
        }
        return ErrorNode(command, "unknown command");
    case 5:
        // weatherstation PERIOD = 10 MINUTE
        if((string_array[1] == "PERIOD" || string_array[1] == "DURATION") && string_array[2] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[3].length()-1; i++){
                letter = string_array[3][i];
                if(!isDigit(letter)){
                    return ErrorNode(command, "time value not a digit");
                    break;
                }
            }
            PARSE_WORDS temp_setter_type;
            if(string_array[1] == "PERIOD"){ temp_setter_type = PARSE_WORDS::PERIOD;}
            else if(string_array[1] == "DURATION"){ temp_setter_type = PARSE_WORDS::DURATION;}
            if(string_array[3] == "MINUTE"){
                return SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::MINUTE_);
            } else if(string_array[3] == "HOUR"){
                return SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::HOUR_);
            } else if (string_array[3] == "SECOND"){
                return SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::SECOND_);
            } else if (string_array[3] == "MIllI_SECOND"){
                return SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::MILLI_SECOND_);
            } else {
                return ErrorNode(command, "Unknown time measurement type");
            }
        }
        return ErrorNode(command, "unknown command");
    default:
        Serial.println("Unknown command: " + command);
        break;
    }
}

unsigned int SetterNode::getTime(){
    if(time_measurements == PARSE_WORDS::MINUTE_){
        return time_period * MINUTE;
    } else if(time_measurements == PARSE_WORDS::HOUR_){
        return time_period * HOUR;
    } else if(time_measurements == PARSE_WORDS::SECOND_){
        return time_period * SECOND;
    } else if(time_measurements == PARSE_WORDS::MILLI_SECOND_){
        return time_period * MIllI_SECOND;
    } else {
        return 0;
    }
}

void SetterNode::execute(Robot & robot){
    if(to_set == "weatherstation"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setWeatherStationActive(true);
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setWeatherStationActive(false);
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setWeatherStationDuration(getTime());
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setWeatherStationTime(getTime());
        }
    } else if(to_set == "walk_reminder"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setWalkTimeActive(true);
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setWalkTimeActive(false);
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setWalkTimeDuration(getTime());
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setWalkTime(getTime());
        }
    } else if(to_set == "water_reminder"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setWaterTimeActive(true);
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setWaterTimeActive(false);
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setWaterTimeDuration(getTime());
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setWaterTime(getTime());
        }
    } else if(to_set == "break_reminder"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setBreakTimeActive(true);
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setBreakTimeActive(false);
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setBreakTimeDuration(getTime());
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setBreakTime(getTime());
        }
    } else if(to_set == "shut_down_after"){
        robot.setShutdownAfter(getTime());
    } else if(to_set == "interactive_mode"){
        if(setter_type == PARSE_WORDS::DURATION){
            robot.setInteractiveModeDuration(getTime());
        }
    } else {
        Serial.println("Unknown setter: " + original_string);
    }
}

void SetStateNode::execute(Robot & robot){
    if(state == PARSE_WORDS::IDLE){
        robot.setState(ROBOT_STATES::IDLE);
    } else if(state == PARSE_WORDS::REMINDER_BREAK){
        robot.setState(ROBOT_STATES::REMINDER_BREAK);
    } else if(state == PARSE_WORDS::REMINDER_WATER){
        robot.setState(ROBOT_STATES::REMINDER_WATER);
    } else if(state == PARSE_WORDS::REMINDER_WALK){
        robot.setState(ROBOT_STATES::REMINDER_WALK);
    } else if(state == PARSE_WORDS::WEATHER_STATION){
        robot.setState(ROBOT_STATES::WEATHER_STATION);
    } else if(state == PARSE_WORDS::INTERACTIVE_MODE){
        robot.setState(ROBOT_STATES::INTERACTIVE_MODE);
    } else {
        Serial.println("Unknown state " + original_string);
    }
}

void CommandNode::execute(Robot & robot){
    if(command == "shutdown"){
        robot.shutDown();
    } else if(command == "interactive_mode"){
        robot.interactiveMode();
    } else if(command == "move_head"){
        robot.moveHead(param);
    } else if(command == "move_neck"){
        robot.moveNeck(param);
    } else {
        Serial.println("Unknown command " + original_string);
    }
}

void WaitNode::execute(Robot & robot){
    if(time_measurements == PARSE_WORDS::MINUTE_){
        rtos::ThisThread::sleep_for(MS( time_period * MINUTE ));
    } else if(time_measurements == PARSE_WORDS::HOUR_){
        rtos::ThisThread::sleep_for(MS( time_period * HOUR ));
    } else if(time_measurements == PARSE_WORDS::SECOND_){
        rtos::ThisThread::sleep_for(MS( time_period * SECOND ));
    } else if(time_measurements == PARSE_WORDS::MILLI_SECOND_){
        rtos::ThisThread::sleep_for(MS( time_period * MIllI_SECOND ));
    } else {
        rtos::ThisThread::sleep_for(MS(0));
    }
}

void ErrorNode::execute(Robot & robot){
    Serial.println("ERROR: " + original_string + " -> " + error_message);
}