#include <robot_interpreter.hpp>

extern String program_code;

void Interpreter::run(){
    // check for repl mode or full file mode.
    if(mode == INTERPRETER_MODES::IDLE){
        if(Serial){
            String choice = "";
            Serial.println("------------------------------------------------------------");
            Serial.println("Enter mode to which to program robot with: ");
            Serial.println("1. REPL, Enter a single command and instantly see the result");
            Serial.println("2. FILE, upload a file with your written code");
            Serial.println("3. PREPROGRAMMED FILE, Load in the preprogrammed file");
            Serial.println("4. EXIT, exit current program");
            Serial.println("------------------------------------------------------------");
            while(choice != "4"){
                Serial.println("waiting for input");
                while(choice == ""){
                    choice = Serial.readStringUntil('\n');
                }
                choice.trim();
                Serial.println("chosen option: " + choice);
                if( choice == "2"){ 
                    while(Serial.available()>0){
                        char temp = Serial.read();
                        temp+=1; //against unused var warning
                    }
                    mode = INTERPRETER_MODES::FILE; 
                    choice = "";
                    break; 
                }
                else if( choice == "1"){
                    while(Serial.available()>0){
                        char temp = Serial.read();
                        temp+=1; //against unused var warning
                    }
                    mode = INTERPRETER_MODES::REPL; 
                    choice = "";
                    break; 
                } else if(choice == "3"){
                    while(Serial.available()>0){
                        char temp = Serial.read();
                        temp+=1; //against unused var warning
                    }
                    mode = INTERPRETER_MODES::PRECOMPILED_FILE;
                    choice = "";
                    break;
                } else{
                    Serial.println("------------------------------------------------------------");
                    Serial.println("Enter mode to which to program robot with: ");
                    Serial.println("1. REPL, Enter a single command and instantly see the result");
                    Serial.println("2. FILE, upload a file with your written code");
                    Serial.println("3. PREPROGRAMMED FILE, Load in the preprogrammed file");
                    Serial.println("4. EXIT, exit current program");
                    Serial.println("------------------------------------------------------------");
                    choice = "";
                }
            }
        } else{
        mode = INTERPRETER_MODES::PRECOMPILED_FILE;
        }
    }
    if(mode == INTERPRETER_MODES::REPL){
        repl();
        mode = INTERPRETER_MODES::IDLE;
    } else if(mode == INTERPRETER_MODES::FILE){
        file(readFileFromSerial());
        mode = INTERPRETER_MODES::IDLE;
    } else if( mode == INTERPRETER_MODES::PRECOMPILED_FILE){
        code code_struct;
        file(code_struct.program_code);
        mode = INTERPRETER_MODES::IDLE;
    }
};

String Interpreter::readFileFromSerial(){
    String allowed_symbols = "_=: (){}";
    char temp_char;
    String file_text = "";
    Serial.println("Enter file text.");
    while(Serial.available()<=0){;}
    while(file_text.indexOf("EOF:") == -1 && Serial.available() != 0){
        temp_char = Serial.read();
        if( allowed_symbols.indexOf(temp_char) != -1 || 
            isAlphaNumeric(temp_char) ||
            temp_char == ' ' ||
            temp_char == '\n'){
            file_text += temp_char;
        }
    }
    return file_text;
    }

void Interpreter::file(String file_text){
    DoubleLinkedList<Node> file_loop_list;
    DoubleLinkedList<Node> file_setup_list;
    String line = "";
    String current_word="";
    String setup = "";
    String loop = "";
    bool setup_trigger = false;
    bool loop_trigger = false;
    for(unsigned int i = 0; i < file_text.length(); i++ ){
        if(file_text[i] == ' ' || file_text[i] == '\n'){
            current_word.trim();
            if(current_word == "SETUP:"){
                Serial.println("SETUP: found");
                setup_trigger = true;
            } else if(current_word == "LOOP:"){
                Serial.println("LOOP: found");
                setup = setup.substring(0,setup.length()-5);
                setup_trigger = false;
                loop_trigger = true;
            } else if(current_word == "EOF:"){
                Serial.println("EOF: found");
                setup_trigger = false;
                loop_trigger = false;
            }
            current_word = "";
        }
        if(setup_trigger){
            setup += file_text[i];
        }
        if(loop_trigger){
            loop += file_text[i];
        }
        current_word += file_text[i];
    }

    setup.trim();
    loop.trim();

    // Process setup
    if(setup.length() > 0){
        file_setup_list = CreateCommandList(setup);
        if(file_setup_list.getLength() > 0){
            file_setup_list.setToStart();
            std::shared_ptr<Node> temp = file_setup_list.getCurrentNode();
            temp->execute(robot);
            temp.reset();
            while(file_setup_list.gotToNextNode()){
                temp = file_setup_list.getCurrentNode();
                temp->execute(robot);
                temp.reset();
            }
        }
    }
    // Process loop
    if(loop.length() > 0){
        file_loop_list = CreateCommandList(loop);
    }
    // Keep processing loop until serial input is reached
    if(file_loop_list.getLength() > 0){
        file_loop_list.setToStart();
        std::shared_ptr<Node> temp = file_loop_list.getCurrentNode();
        temp = file_loop_list.getCurrentNode();
        temp->execute(robot);
        temp.reset();
        while(Serial.available()>0){
            char temp = Serial.read();
            temp+=1; //against unused var warning
        }
        while(Serial.available() == 0){
            if(!file_loop_list.gotToNextNode()){
                file_loop_list.setToStart();
            }
            temp = file_loop_list.getCurrentNode();
            temp->execute(robot);
            rtos::ThisThread::sleep_for(MS(1000));
            temp.reset();
        }
    }
}

DoubleLinkedList<Node> Interpreter::CreateCommandList(String text){
    std::shared_ptr<IfNode> possible_if_node;
    DoubleLinkedList<Node> linked_list;
    bool in_if_node = false;
    String line = "";
    for(unsigned int i = 0; i < text.length(); i++ ){
        line += text[i];
        if(text[i] == '\n' || i == text.length()-1){
            line.trim();
            if(in_if_node){
                if(line.indexOf('}') == -1){
                    possible_if_node->addCommand(parseCommand(line));
                } else if (line.indexOf('}') != -1){ // ifnode finished
                    linked_list.append(possible_if_node);
                    in_if_node = false;
                }
            } else{
                possible_if_node = createPossibleIf(line);
                if(possible_if_node->getType() == NODE_TYPES::ERROR){
                    std::shared_ptr<Node> node_ptr = parseCommand(line);
                    linked_list.append(node_ptr);
                    node_ptr.reset();
                } else{
                    in_if_node = true;
                }
            }
            line = "";
        }
    }
    return linked_list;
}

//TODO repl no longer functional, probably null character in serial read. also head servo prob broken, test tomorrow.
void Interpreter::repl(){
    String command = "";
    char character_temp = ' ';
    while(command != "4"){
        Serial.println("---Enter command. Or 4 to quit---");
        while(Serial.available()<=0){
            rtos::ThisThread::sleep_for(MS(1000));
        }
        while(character_temp != '\n' && Serial.available() != 0){
            character_temp = Serial.read();
            command += character_temp;
        }
        command.trim();
        if(command == "4"){
            while(Serial.available()>0){
                char temp = Serial.read();
                temp+=1; //against unused var warning
            }
            Serial.println(command +" "+ String(command.length()));
            break;
        }
        Serial.println("given input: " + command);
        std::shared_ptr<Node> node_ptr = parseCommand(command);
        node_ptr->execute(robot);
        node_ptr.reset();
        command = "";
        character_temp = ' ';
    }
}

std::shared_ptr<Node> Interpreter::parseCommand(String command){
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
        return std::shared_ptr<Node> (new CommandNode(command, string_array[0], "", false));
        break;
    case 2:
        if(string_array[0] != parse_words.EOF_){
            return std::shared_ptr<Node> (new CommandNode(command, string_array[0], string_array[1], true));
        }
        break;
    case 3:
        if(string_array[1] == "="){
            // STATE = IDLE
            if( string_array[0] == parse_words.STATE && string_array[1] == "="){
                if(string_array[2] == parse_words.IDLE){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.IDLE));
                } else if(string_array[2] == parse_words.REMINDER_BREAK){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_BREAK));
                } else if(string_array[2] == parse_words.REMINDER_WATER){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_WATER));
                } else if(string_array[2] == parse_words.REMINDER_WALK){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_WALK));
                } else if(string_array[2] == parse_words.WEATHER_STATION){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.WEATHER_STATION));
                } else if(string_array[2] == parse_words.INTERACTIVE_MODE){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.INTERACTIVE_MODE));
                } else {
                    return std::shared_ptr<Node> (new ErrorNode(command, "unknown state"));
                }
            }
            // weatherstation = ACTIVE
            if(string_array[2] == parse_words.ACTIVE){
                return std::shared_ptr<Node> (new SetterNode(command, string_array[0],parse_words.ACTIVE, 0 ,parse_words.MILLI_SECOND_ ));
            } else if(string_array[2] == parse_words.NON_ACTIVE){
                return std::shared_ptr<Node> (new SetterNode(command, string_array[0],parse_words.NON_ACTIVE, 0 , parse_words.MILLI_SECOND_ ));
            }
        } else { 
            // WAIT 10 MINUTES
            if(string_array[0] == parse_words.WAIT){
                char letter;
                for(unsigned int i =0 ; i < string_array[1].length()-1; i++){
                    letter = string_array[1][i];
                    if(!isDigit(letter)){
                        return std::shared_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                        break;
                    }
                }
                if(string_array[2] == parse_words.MINUTE_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.MINUTE_));
                } else if(string_array[2] == parse_words.HOUR_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.HOUR_));
                } else if (string_array[2] == parse_words.SECOND_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.SECOND_));
                } else if (string_array[2] == parse_words.MILLI_SECOND_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.MILLI_SECOND_));
                } else {
                    return std::shared_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
                }
            }
        }
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command"));
    case 4:
        //shut_down_after = 10 MINUTE
        if(string_array[0] == parse_words.shut_down_after && string_array[1] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[2].length()-1; i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    return std::shared_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                    break;
                }
            }
            if(string_array[3] == parse_words.MINUTE_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.MINUTE_));
                } else if(string_array[3] == parse_words.HOUR_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.HOUR_));
                } else if (string_array[3] == parse_words.SECOND_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.SECOND_));
                } else if (string_array[3] == parse_words.MILLI_SECOND_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.MILLI_SECOND_));
                } else {
                    return std::shared_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
                }
        }
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command"));
    case 5:
        // weatherstation PERIOD = 10 MINUTE
        if((string_array[1] == parse_words.PERIOD || string_array[1] == parse_words.DURATION) && string_array[2] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[3].length()-1; i++){
                letter = string_array[3][i];
                if(!isDigit(letter)){
                    return std::shared_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                    break;
                }
            }
            return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , string_array[1], string_array[3].toInt(), string_array[4]));
        }
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command"));
    default:
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command"));   
        Serial.println("Unknown command: " + command);
        break;
    }
    return std::shared_ptr<Node> (new ErrorNode(command, "unknown command"));
}

std::shared_ptr<IfNode> Interpreter::createPossibleIf(String command){
    command.trim();
    const int array_size = 265;
    char char_array[array_size];
    String condition_line = "";     
    command.toCharArray(char_array, array_size);
    int index = 3;
    if(char_array[0] == 'I' && char_array[1] == 'F'){
        if(char_array[2] == '('){
            char temp_char = ' ';
            while( temp_char != ')'){
                temp_char = char_array[index];
                condition_line += temp_char;
                index++;
            }
        }
    }
    if(char_array[index] == '{'){
        return std::shared_ptr<IfNode>( new IfNode(command, condition_line));        
    }
    // If no if statement is found an IfNode is returned with the node type set to error
    return std::shared_ptr<IfNode>( new IfNode("NON_VIABLE", condition_line, NODE_TYPES::ERROR));        
}

void Node::execute(Robot & robot){
    Serial.println(original_string);
}

NODE_TYPES Node::getType(){
    return type;
}

unsigned int SetterNode::getTime(){
    if(time_measurements == parse_words.MINUTE_){
        return time_period * MINUTE;
    } else if(time_measurements == parse_words.HOUR_){
        return time_period * HOUR;
    } else if(time_measurements == parse_words.SECOND_){
        return time_period * SECOND;
    } else if(time_measurements == parse_words.MILLI_SECOND_){
        return time_period * MIllI_SECOND;
    } else {
        return 0;
    }
}

void SetterNode::execute(Robot & robot){
    if(to_set == parse_words.weatherstation){
        if(setter_type == parse_words.ACTIVE){
            robot.setWeatherStationActive(true);
            Serial.println("Weather station activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setWeatherStationActive(false);
            Serial.println("Weather station activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setWeatherStationDuration(getTime());
            Serial.println("Weather station duration set to: " + String(getTime()/SECOND) + "seconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setWeatherStationTime(getTime());
            Serial.println("Weather station loop time set to: " + String(getTime()/SECOND) + "seconds");
        }
    } else if(to_set == parse_words.walk_reminder){
        if(setter_type == parse_words.ACTIVE){
            robot.setWalkTimeActive(true);
            Serial.println("Walk reminder activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setWalkTimeActive(false);
            Serial.println("Walk reminder activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setWalkTimeDuration(getTime());
            Serial.println("Walk reminder duration set to: " + String(getTime()/SECOND) + "seconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setWalkTime(getTime());
            Serial.println("Walk reminder loop time set to: " + String(getTime()/SECOND) + "seconds");
        }
    } else if(to_set == parse_words.water_reminder){
        if(setter_type == parse_words.ACTIVE){
            robot.setWaterTimeActive(true);
            Serial.println("Water reminder activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setWaterTimeActive(false);
            Serial.println("Water reminder activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setWaterTimeDuration(getTime());
            Serial.println("Water reminder duration set to: " + String(getTime()/SECOND) + "seconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setWaterTime(getTime());
            Serial.println("Water reminder loop time set to: " + String(getTime()/SECOND) + "seconds");
        }
    } else if(to_set == parse_words.break_reminder){
        if(setter_type == parse_words.ACTIVE){
            robot.setBreakTimeActive(true);
            Serial.println("Break reminder activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setBreakTimeActive(false);
            Serial.println("Break reminder activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setBreakTimeDuration(getTime());
            Serial.println("Break reminder duration set to: " + String(getTime()/SECOND) + "seconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setBreakTime(getTime());
            Serial.println("Break reminder loop time set to: " + String(getTime()/SECOND) + "seconds");
        }
    } else if(to_set == parse_words.shut_down_after){
        robot.setShutdownAfter(getTime());
        Serial.println("shut down timer set to: " + String(getTime()/SECOND) + "seconds");
    } else if(to_set == parse_words.interactive_mode){
        if(setter_type == parse_words.DURATION){
            robot.setInteractiveModeDuration(getTime());
            Serial.println("Interactive mode duration time set to: " + String(getTime()/SECOND) + "seconds");
        }
    } else {
        Serial.println("Unknown setter: " + original_string);
    }
}

void SetStateNode::execute(Robot & robot){
    if(state == parse_words.IDLE){
        robot.setState(ROBOT_STATES::IDLE);
        Serial.println("robot state set to : IDLE");
    } else if(state == parse_words.REMINDER_BREAK){
        robot.setState(ROBOT_STATES::REMINDER_BREAK);
        Serial.println("robot state set to : REMINDER_BREAK");
    } else if(state == parse_words.REMINDER_WATER){
        robot.setState(ROBOT_STATES::REMINDER_WATER);
        Serial.println("robot state set to : REMINDER_WATER");
    } else if(state == parse_words.REMINDER_WALK){
        robot.setState(ROBOT_STATES::REMINDER_WALK);
        Serial.println("robot state set to : REMINDER_WALK");
    } else if(state == parse_words.WEATHER_STATION){
        robot.setState(ROBOT_STATES::WEATHER_STATION);
        Serial.println("robot state set to : WEATHER_STATION");
    } else if(state == parse_words.INTERACTIVE_MODE){
        robot.setState(ROBOT_STATES::INTERACTIVE_MODE);
        Serial.println("robot state set to : INTERACTIVE_MODE");
    } else if(state == parse_words.OFF){
        robot.setState(ROBOT_STATES::OFF);
        Serial.println("robot state set to : OFF");
    }else {
        Serial.println("Unknown state " + original_string);
    }
}

void CommandNode::execute(Robot & robot){
    char letter;
    bool is_digit = true;
    for(unsigned int i =0 ; i < param.length()-1; i++){
        letter = param[i];
        if(!isDigit(letter)){
            is_digit = false;
            break;
        }
    }
    if(command == parse_words.shutdown){
        robot.shutDown();
    } else if(command == parse_words.interactive_mode){
        robot.setState(ROBOT_STATES::INTERACTIVE_MODE);
    } else if(command == parse_words.break_reminder){
        robot.setState(ROBOT_STATES::REMINDER_BREAK);
    } else if(command == parse_words.walk_reminder){
        robot.setState(ROBOT_STATES::REMINDER_WALK);
    } else if(command == parse_words.water_reminder){
        robot.setState(ROBOT_STATES::REMINDER_WATER);
    }else if(command == parse_words.showWeatherStation){
        robot.setState(ROBOT_STATES::WEATHER_STATION);
    } else if(command == parse_words.getBreakTime){
        Serial.println("Break reminder period " + String(robot.getBreakTime())+" MS");
    } else if(command == parse_words.getWalkTime){
        Serial.println("Walk reminder period " + String(robot.getWalkTime())+" MS");
    } else if(command == parse_words.getWaterTime){
        Serial.println("Water reminder period " + String(robot.getWaterTime())+" MS");
    } else if(command == parse_words.getWeatherStationTime){
        Serial.println("weather station period " + String(robot.getWeatherStationTime())+" MS");
    } else if(command == parse_words.getState){
        ROBOT_STATES state = robot.getState();
        switch (state){
        case ROBOT_STATES::IDLE:
            Serial.println("Current state: IDLE");
            break;
        case ROBOT_STATES::INTERACTIVE_MODE:
            Serial.println("Current state: INTERACTIVE_MODE");
            break;
        case ROBOT_STATES::OFF:
            Serial.println("Current state: OFF");
            break;
        case ROBOT_STATES::REMINDER_BREAK:
            Serial.println("Current state: REMINDER_BREAK");
            break;
        case ROBOT_STATES::REMINDER_WALK:
            Serial.println("Current state: REMINDER_WALK");
            break;
        case ROBOT_STATES::REMINDER_WATER:
            Serial.println("Current state: REMINDER_WATER");
            break;
        case ROBOT_STATES::WEATHER_STATION:
            Serial.println("Current state: WEATHER_STATION");
            break;
        default:
            break;
        }
    } else if(command == parse_words.rngMovement){
        robot.rngMovement();
    } else if(command == parse_words.returnToStartPos){
        robot.returnToStartPos();  
    } else if(command == parse_words.getHeadPos){
        Serial.println("Head at: " + String(robot.getHeadPos()) + " degrees");
    } else if(command == parse_words.getNeckPos){
        Serial.println("Neck at: " + String(robot.getNeckPos()) + " degrees");
    } else if(command == parse_words.getLastMovementDetected){
        Serial.println("Movement detected " + String(robot.getLastMovementDetected()) + " seconds ago");
    }
    else if(is_digit){
        if(command == parse_words.move_head){
            robot.moveHead(param.toInt());
        } else if(command == parse_words.move_neck){
            robot.moveNeck(param.toInt());
        } 
    } else if(!is_digit){
        if(command == parse_words.playSound){
            Songs songs;
            if(param == parse_words.start_up){
                robot.playSound<songs.start_up.size()>(songs.start_up);
            } else if(param == parse_words.shut_down){
                robot.playSound<songs.shut_down.size()>(songs.shut_down);
            } else if(param == parse_words.notification){
                robot.playSound<songs.notification.size()>(songs.notification);
            }
        }
    } else{
        if(command == parse_words.EOF_){return;}
        ErrorNode error("Unknown command: ", original_string);
        error.execute(robot);
    }
}

void WaitNode::execute(Robot & robot){
    if(time_measurements == parse_words.MINUTE_){
        rtos::ThisThread::sleep_for(MS( time_period * MINUTE ));
    } else if(time_measurements == parse_words.HOUR_){
        rtos::ThisThread::sleep_for(MS( time_period * HOUR ));
    } else if(time_measurements == parse_words.SECOND_){
        Serial.println("started wait "+ String(time_period * SECOND));
        rtos::ThisThread::sleep_for(MS( time_period * SECOND ));
        Serial.println("ended wait");
    } else if(time_measurements == parse_words.MILLI_SECOND_){
        rtos::ThisThread::sleep_for(MS( time_period * MIllI_SECOND ));
    } else {
        rtos::ThisThread::sleep_for(MS(0));
    }
}

void IfNode::execute(Robot & robot){
    Serial.println("===========IN EXECUTE===========");
    if(CheckIfConditionTrue()){
        body.setToStart();
        std::shared_ptr<Node> body_command = body.getCurrentNode();
        body_command->print();
        // body_command->execute(robot);
        body_command.reset();
        while(body.gotToNextNode()){
            body_command = body.getCurrentNode();
            // body_command->execute(robot);
            body_command->print();
            body_command.reset();
        }
        Serial.println("==========END EXECUTE===========");
    }
}

bool IfNode::CheckIfConditionTrue(){
    // check if condition is true or false
    return true;
}

void IfNode::addCommand(std::shared_ptr<Node> command){
    body.append(command);
}

void ErrorNode::execute(Robot & robot){
    Serial.println("ERROR: " + original_string + " -> " + error_message);
}

void Node::print(){
    Serial.println("Node -> " + original_string);
}

void SetterNode::print(){
    Serial.println("SetterNode -> " +  to_set + " " + setter_type + " " + String(time_period) + " " + time_measurements);
}

void SetStateNode::print(){
    Serial.println("SetStateNode -> set state to " + state);
}

void CommandNode::print(){
    Serial.println("CommandNode -> "+ command + "(" + param + ")");
}

void WaitNode::print(){
    Serial.println("WaitNode -> wait for "+ String(time_period) + " " + time_measurements);
}

void ErrorNode::print(){
    Serial.println("ErrorNode -> " + error_message);
}

void IfNode::print(){
    Serial.println("IfNode - > IF(" + condition + "){" + body.getLength() + "}");
}