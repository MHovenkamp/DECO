#include <robot_interpreter.hpp>

extern ROBOT_STATES global_state;

void Interpreter::run(){
    // check for repl mode or full file mode.
    if(mode == INTERPRETER_MODES::IDLE){
        String choice = "";
        Serial.println("Enter mode to which to program robot with: ");
        Serial.println("1. REPL, Enter a single command and instantly see the result");
        Serial.println("2. FILE, upload a file with your written code");
        Serial.println("3. EXIT, exit current program");
        global_state = ROBOT_STATES::PAUSE;
        while(choice != "3"){
            Serial.println("waiting for input");
            while(choice == ""){
                choice = Serial.readStringUntil('\n');
            }
            Serial.println("chosen option: " + choice);
            if( choice == "2"){ 
                Serial.println("FILE chosen"); 
                mode = INTERPRETER_MODES::FILE; 
                choice = "";
                break; 
            }
            else if( choice == "1"){
                Serial.println("REPL chosen"); 
                mode = INTERPRETER_MODES::REPL; 
                choice = "";
                break; 
            };
        }
        global_state = ROBOT_STATES::IDLE;
    }
    if(mode == INTERPRETER_MODES::REPL){
        repl();
        Serial.print("hewwo");
    } else if(mode == INTERPRETER_MODES::FILE){
        file();
    }
};

void Interpreter::file(){
    Serial.println("file");
}

void Interpreter::repl(){
    String command = "";
    while(command != "3"){
        Serial.println("Enter command. Or 3. EXIT to quit");
        global_state = ROBOT_STATES::PAUSE;
        while(command == ""){
            command = Serial.readStringUntil('\n');
        }
        global_state = ROBOT_STATES::IDLE;
        Serial.println("given input: " + command);
        std::unique_ptr<Node> node_ptr = parseCommand(command); 
        node_ptr->execute(robot);
        node_ptr.reset();
        command = "";
        rtos::ThisThread::sleep_for(MS(1000));
    }
}

std::unique_ptr<Node> Interpreter::parseCommand(String command){
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
            string_array[current_array_index] = command.substring(start_index,i);
            current_array_index++;
            start_index = i+1;
        }
        if( i == command.length()-1){
            string_array[current_array_index] = command.substring(start_index,i+1);
        }
    }

    switch (amount_of_spaces+1)
    {
    case 1:
        if(string_array[0] == "interactive_mode" || string_array[0] == "shutdown"){
            return std::unique_ptr<Node> (new CommandNode(command, string_array[0], 0, false));
        } 
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
        break;
    case 2:
        char letter;
        for(unsigned int i =0 ; i < string_array[1].length()-1; i++){
            letter = string_array[1][i];
            if(!isDigit(letter)){
                return std::unique_ptr<Node> (new ErrorNode(command, "value not a digit"));
                break;
            }
        }
        return std::unique_ptr<Node> (new CommandNode(command, string_array[0], string_array[1].toInt(), true));
        break;
    case 3:
        if(string_array[1] == "="){
            // STATE = IDLE
            if( string_array[0] == "STATE" && string_array[1] == "="){
                if(string_array[2] == "IDLE"){
                    return std::unique_ptr<Node> (new SetStateNode(command, PARSE_WORDS::IDLE));
                } else if(string_array[2] == "REMINDER_BREAK"){
                    return std::unique_ptr<Node> (new SetStateNode(command, PARSE_WORDS::REMINDER_BREAK));
                } else if(string_array[2] == "REMINDER_WATER"){
                    return std::unique_ptr<Node> (new SetStateNode(command, PARSE_WORDS::REMINDER_WATER));
                } else if(string_array[2] == "REMINDER_WALK"){
                    return std::unique_ptr<Node> (new SetStateNode(command, PARSE_WORDS::REMINDER_WALK));
                } else if(string_array[2] == "WEATHER_STATION"){
                    return std::unique_ptr<Node> (new SetStateNode(command, PARSE_WORDS::WEATHER_STATION));
                } else if(string_array[2] == "INTERACTIVE_MODE"){
                    return std::unique_ptr<Node> (new SetStateNode(command, PARSE_WORDS::INTERACTIVE_MODE));
                } else {
                    return std::unique_ptr<Node> (new ErrorNode(command, "unknown state"));
                }
            }
            // weatherstation = ACTIVE
            if(string_array[2] == "ACTIVE"){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0],PARSE_WORDS::ACTIVE, 0 , PARSE_WORDS::MILLI_SECOND_ ));
            } else if(string_array[2] == "NON_ACTIVE"){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0],PARSE_WORDS::NON_ACTIVE, 0 , PARSE_WORDS::MILLI_SECOND_ ));
            }
        } else { 
            // WAIT 10 MINUTES
            if(string_array[0] == "WAIT"){
                char letter;
                for(unsigned int i =0 ; i < string_array[1].length()-1; i++){
                    letter = string_array[1][i];
                    if(!isDigit(letter)){
                        return std::unique_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                        break;
                    }
                }
                if(string_array[2] == "MINUTE"){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), PARSE_WORDS::MINUTE_));
                } else if(string_array[2] == "HOUR"){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), PARSE_WORDS::HOUR_));
                } else if (string_array[2] == "SECOND"){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), PARSE_WORDS::SECOND_));
                } else if (string_array[2] == "MIllI_SECOND"){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), PARSE_WORDS::MILLI_SECOND_));
                } else {
                    return std::unique_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
                }
            }
        }
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
    case 4:
        //shut_down_after = 10 MINUTE
        if(string_array[0] == "shut_down_after" && string_array[1] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[2].length()-1; i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    return std::unique_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                    break;
                }
            }
            if(string_array[3] == "MINUTE"){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::MINUTE_));
                } else if(string_array[3] == "HOUR"){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::HOUR_));
                } else if (string_array[3] == "SECOND"){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::SECOND_));
                } else if (string_array[3] == "MIllI_SECOND"){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , PARSE_WORDS::shut_down_after, string_array[2].toInt(), PARSE_WORDS::MILLI_SECOND_));
                } else {
                    return std::unique_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
                }
        }
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
    case 5:
        // weatherstation PERIOD = 10 MINUTE
        if((string_array[1] == "PERIOD" || string_array[1] == "DURATION") && string_array[2] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[3].length()-1; i++){
                letter = string_array[3][i];
                if(!isDigit(letter)){
                    return std::unique_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                    break;
                }
            }
            PARSE_WORDS temp_setter_type;
            if(string_array[1] == "PERIOD"){ temp_setter_type = PARSE_WORDS::PERIOD;}
            else { temp_setter_type = PARSE_WORDS::DURATION;}
            if(string_array[3] == "MINUTE"){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::MINUTE_));
            } else if(string_array[3] == "HOUR"){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::HOUR_));
            } else if (string_array[3] == "SECOND"){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::SECOND_));
            } else if (string_array[3] == "MIllI_SECOND"){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , temp_setter_type, string_array[3].toInt(), PARSE_WORDS::MILLI_SECOND_));
            } else {
                return std::unique_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
            }
        }
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
    default:
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));   
        Serial.println("Unknown command: " + command);
        break;
    }
    return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
}

void Node::execute(Robot & robot){
    Serial.println(original_string);
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
            Serial.println("Weather station activity set to : TRUE");
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setWeatherStationActive(false);
            Serial.println("Weather station activity set to : FALSE");
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setWeatherStationDuration(getTime());
            Serial.println("Weather station duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setWeatherStationTime(getTime());
            Serial.println("Weather station loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == "walk_reminder"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setWalkTimeActive(true);
            Serial.println("Walk reminder activity set to : TRUE");
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setWalkTimeActive(false);
            Serial.println("Walk reminder activity set to : FALSE");
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setWalkTimeDuration(getTime());
            Serial.println("Walk reminder duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setWalkTime(getTime());
            Serial.println("Walk reminder loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == "water_reminder"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setWaterTimeActive(true);
            Serial.println("Water reminder activity set to : TRUE");
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setWaterTimeActive(false);
            Serial.println("Water reminder activity set to : FALSE");
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setWaterTimeDuration(getTime());
            Serial.println("Water reminder duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setWaterTime(getTime());
            Serial.println("Water reminder loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == "break_reminder"){
        if(setter_type == PARSE_WORDS::ACTIVE){
            robot.setBreakTimeActive(true);
            Serial.println("Break reminder activity set to : TRUE");
        } else if(setter_type == PARSE_WORDS::NON_ACTIVE){
            robot.setBreakTimeActive(false);
            Serial.println("Break reminder activity set to : FALSE");
        } else if(setter_type == PARSE_WORDS::DURATION){
            robot.setBreakTimeDuration(getTime());
            Serial.println("Break reminder duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == PARSE_WORDS::PERIOD){
            robot.setBreakTime(getTime());
            Serial.println("Break reminder loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == "shut_down_after"){
        robot.setShutdownAfter(getTime());
        Serial.println("shut down timer set to: " + String(getTime()) + "milliseconds");
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
        Serial.println("robot state set to : IDLE");
    } else if(state == PARSE_WORDS::REMINDER_BREAK){
        robot.setState(ROBOT_STATES::REMINDER_BREAK);
        Serial.println("robot state set to : REMINDER_BREAK");
    } else if(state == PARSE_WORDS::REMINDER_WATER){
        robot.setState(ROBOT_STATES::REMINDER_WATER);
        Serial.println("robot state set to : REMINDER_WATER");
    } else if(state == PARSE_WORDS::REMINDER_WALK){
        robot.setState(ROBOT_STATES::REMINDER_WALK);
        Serial.println("robot state set to : REMINDER_WALK");
    } else if(state == PARSE_WORDS::WEATHER_STATION){
        robot.setState(ROBOT_STATES::WEATHER_STATION);
        Serial.println("robot state set to : WEATHER_STATION");
    } else if(state == PARSE_WORDS::INTERACTIVE_MODE){
        robot.setState(ROBOT_STATES::INTERACTIVE_MODE);
        Serial.println("robot state set to : INTERACTIVE_MODE");
    } else {
        Serial.println("Unknown state " + original_string);
    }
}

void CommandNode::execute(Robot & robot){
    if(command == "shutdown"){
        robot.shutDown();
    } else if(command == "interactive_mode"){
        while(robot.interactiveMode()){};
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