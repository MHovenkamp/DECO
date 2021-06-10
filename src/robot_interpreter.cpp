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
    String allowed_symbols = "_=: (){}<>+-/*";
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
    // Find and split the code block into the setup and loop partitions.
    for(unsigned int i = 0; i < file_text.length(); i++ ){
        if(file_text[i] == ' ' || file_text[i] == '\n'){
            current_word.trim();
            if(current_word == "SETUP:"){
                setup_trigger = true;
            } else if(current_word == "LOOP:"){
                setup = setup.substring(0,setup.length()-5);
                setup_trigger = false;
                loop_trigger = true;
            } else if(current_word == "EOF:"){
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

    Serial.println("===========SETUP==========");
    Serial.println(setup);
    Serial.println("===========LOOP===========");
    Serial.println(loop);
    Serial.println("==========================");
    // Process setup

    int line_number = 1;
    std::shared_ptr<Node> all_created_integer_nodes[50];
    unsigned int current_index = 0;

    if(setup.length() > 0){
        line_number++; // count increase to count the SETUP: tag
        file_setup_list = createCommandList(setup, &line_number, all_created_integer_nodes, &current_index);
        if(file_setup_list.getLength() > 0){
            file_setup_list.setToStart();
            std::shared_ptr<Node> temp = file_setup_list.getCurrentNode();
            temp->execute(robot, all_created_integer_nodes, &current_index);
            temp.reset();
            while(file_setup_list.gotToNextNode()){
                temp = file_setup_list.getCurrentNode();
                if(temp->getType() == NODE_TYPES::ERROR){
                    temp->execute(robot, all_created_integer_nodes, &current_index);
                    return;
                }
                temp->print();
                temp->execute(robot, all_created_integer_nodes, &current_index);
                temp.reset();
            }
        }
    }
    // Process loop
    if(loop.length() > 0){
        line_number++; // count increase to count the LOOP: tag
        file_loop_list = createCommandList(loop, &line_number, all_created_integer_nodes, &current_index);
    } 
    // Keep processing loop until serial input is reached
    if(file_loop_list.getLength() > 0){
        file_loop_list.setToStart();
        std::shared_ptr<Node> temp = file_loop_list.getCurrentNode();
        temp = file_loop_list.getCurrentNode();
        temp->print();
        temp->execute(robot, all_created_integer_nodes, &current_index);
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
            if(temp->getType() == NODE_TYPES::ERROR){
                temp->execute(robot, all_created_integer_nodes, &current_index);
                return;
            }
            temp->print();
            temp->execute(robot, all_created_integer_nodes, &current_index);
            temp.reset();
        }
    }
}

DoubleLinkedList<Node> Interpreter::createCommandList(String text, int* line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int* current_index_int){
    std::shared_ptr<IfNode> possible_if_node;
    std::shared_ptr<WhileNode> possible_while_node;

    // list filled with all open if and while nodes. empty when all nodes are filled and stored in linked_list
    std::shared_ptr<ContainsBodyNode> all_created_if_while_nodes[50];
    int current_amount_of_if_while = 0;
    // return list containing all the nodes of the program
    DoubleLinkedList<Node> linked_list;

    String line = "";
    
    for(unsigned int i = 0; i < text.length(); i++ ){
        line += text[i];
        if(text[i] == '\n' || i == text.length()-1){
            line.trim(); // remove extra spaces, enter etc.
            // if the last node in the if/while node is an ifnode
            if(all_created_if_while_nodes[current_amount_of_if_while-1]->getType() == NODE_TYPES::IFNODE){
                if(line.indexOf('}') == -1){ // no end line detected for ifnode yet
                    possible_if_node = createPossibleIf(line, line_number,all_created_integer_nodes, current_index_int);
                    possible_while_node = createPossibleWhile(line, line_number,all_created_integer_nodes, current_index_int);
                    if(possible_while_node->isViable() == false && possible_if_node->isViable() == false){
                        all_created_if_while_nodes[current_amount_of_if_while-1]->addCommand(parseCommand(line, line_number, all_created_integer_nodes, current_index_int));
                    } else if(possible_if_node->isViable() == true){
                        all_created_if_while_nodes[current_amount_of_if_while] = possible_if_node;
                        current_amount_of_if_while++;
                    } else if(possible_while_node->isViable() == true){
                        all_created_if_while_nodes[current_amount_of_if_while] = possible_while_node;
                        current_amount_of_if_while++;
                    }
                } else if (line.indexOf('}') != -1){ // ifnode finished
                    // get the command and delete the old node from the while/ifnode list
                    IfNode *node = static_cast<IfNode*>(all_created_if_while_nodes[current_amount_of_if_while-1].get());
                    IfNode just_node = *node;
                    std::shared_ptr<Node> temp_node_type = std::make_shared<IfNode>(just_node);
                    current_amount_of_if_while--;
                    if(current_amount_of_if_while > 0){ // if there are more if/while nodes place it in the previous one
                        all_created_if_while_nodes[current_amount_of_if_while-1]->addCommand(temp_node_type);
                    } else{
                        linked_list.append(temp_node_type);
                    }
                }
            line = "";
            *line_number = *line_number+1;  
            continue;
            } else if(all_created_if_while_nodes[current_amount_of_if_while-1]->getType() == NODE_TYPES::WHILENODE){
                if(line.indexOf('}') == -1){
                    possible_if_node = createPossibleIf(line, line_number,all_created_integer_nodes, current_index_int);
                    possible_while_node = createPossibleWhile(line, line_number,all_created_integer_nodes, current_index_int);
                    if(possible_while_node->isViable() == false && possible_if_node->isViable() == false){
                        all_created_if_while_nodes[current_amount_of_if_while-1]->addCommand(parseCommand(line, line_number, all_created_integer_nodes, current_index_int));
                    } else if(possible_if_node->isViable() == true){
                        all_created_if_while_nodes[current_amount_of_if_while] = possible_if_node;
                        current_amount_of_if_while++;
                    } else if(possible_while_node->isViable() == true){
                        all_created_if_while_nodes[current_amount_of_if_while] = possible_while_node;
                        current_amount_of_if_while++;
                    }
                } else if (line.indexOf('}') != -1){ // whileNode finished
                    // get the command and delete the old node from the while/ifnode list
                    WhileNode *node = static_cast<WhileNode*>(all_created_if_while_nodes[current_amount_of_if_while-1].get());
                    WhileNode just_node = *node;
                    std::shared_ptr<Node> temp_node_type = std::make_shared<WhileNode>(just_node);
                    current_amount_of_if_while--;
                    if(current_amount_of_if_while > 0){ // if there are more if/while nodes place it in the previous one
                        all_created_if_while_nodes[current_amount_of_if_while-1]->addCommand(temp_node_type);
                    } else{
                        linked_list.append(temp_node_type);
                    }
                }
            line = "";
            *line_number = *line_number+1;  
            continue;
            } else{
                possible_if_node = createPossibleIf(line, line_number,all_created_integer_nodes, current_index_int);
                possible_while_node = createPossibleWhile(line, line_number,all_created_integer_nodes, current_index_int);
                if(possible_while_node->isViable() == false && possible_if_node->isViable() == false){
                    std::shared_ptr<Node> node_ptr = parseCommand(line, line_number, all_created_integer_nodes, current_index_int);
                    linked_list.append(node_ptr);
                } else if(possible_if_node->isViable() == true){
                    all_created_if_while_nodes[current_amount_of_if_while] = possible_if_node;
                    current_amount_of_if_while++;
                } else if(possible_while_node->isViable() == true){
                    all_created_if_while_nodes[current_amount_of_if_while] = possible_while_node;
                    current_amount_of_if_while++;
                }
            }
            line = "";
            *line_number = *line_number+1;
        }
    }
    return linked_list;
}

void Interpreter::repl(){
    std::shared_ptr<Node> all_created_integer_nodes[50]; // max of 50 integers.
    unsigned int current_index = 0;
    String command = "";
    char character_temp = ' ';
    int line_number = 0;
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
        std::shared_ptr<Node> node_ptr = parseCommand(command, &line_number, all_created_integer_nodes, &current_index);
        node_ptr->execute(robot, all_created_integer_nodes, &current_index);
        node_ptr.reset();
        command = "";
        character_temp = ' ';
    }
}
// give it the node lsits check if it is an integer node then pass just the int, do this for any instance of an int
std::shared_ptr<IfNode> Interpreter::createPossibleIf(String command, int* line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    command.trim();
    String condition_line = "";     
    int index = 3;
    if(command[0] == 'I' && command[1] == 'F'){
        if(command[2] == '('){
            while( command[index] != ')'){
                condition_line += command[index];
                index++;
            }
            if(command[index+1] == '{'){
                return std::shared_ptr<IfNode>( new IfNode(command, condition_line, true, *line_number));        
            } else {
                ErrorNode node = ErrorNode(command, "No { found",*line_number);
                node.execute(robot, all_created_integer_nodes, current_index);
            }
        }
    }
    // If no if statement is found an IfNode is returned with the node type set to error
    return std::shared_ptr<IfNode>( new IfNode("NON_VIABLE", condition_line, false, *line_number));        
}

std::shared_ptr<WhileNode> Interpreter::createPossibleWhile(String command, int* line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    command.trim();
    String condition_line = "";    
    String possible_while = command.substring(0,5);
    int index = 6;
    if(possible_while == parse_words.WHILE && command[5] == '('){
        while( command[index] != ')'){
            condition_line += command[index];
            index++;
        } 
        if(command[index+1] == '{'){
            return std::shared_ptr<WhileNode>( new WhileNode(command, condition_line, true, *line_number));        
        } else {
            ErrorNode node = ErrorNode(command, "No { found",*line_number);
            node.execute(robot, all_created_integer_nodes, current_index);
        }
    }
    // // If no if statement is found an IfNode is returned with the node type set to error
    return std::shared_ptr<WhileNode>( new WhileNode("NON_VIABLE", condition_line, false, *line_number));         
}

std::shared_ptr<Node> Interpreter::parseCommand(String command, int * line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    command.trim();
    int amount_of_spaces = 0;
    for(unsigned int i = 0; i < command.length(); i++){
        if( command[i] == ' '){
            amount_of_spaces++;
        }
    }
    // max words in sentence set to 10, no command should go over 5 words but we double it just in case.
    String string_array[10] = {};
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

    // start lexing and parsing the commands based on the lenght of the sentence
    switch (amount_of_spaces+1)
    {
    case 1:
        return std::shared_ptr<Node> (new CommandNode(command, string_array[0], "", false));
        break;
    case 2:
        if(string_array[0] != parse_words.EOF_){
            for(unsigned int i = 0; i < *current_index; i++){
                IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                if(string_array[0] == integer_node->getName()){
                    return std::shared_ptr<Node> (new CommandNode(command, string_array[0], String(integer_node->getName()), true));
                }
            }
            return std::shared_ptr<Node> (new CommandNode(command, string_array[0], string_array[1], true));
        }
        break;
    case 3:
        if(string_array[1] == "="){
            char letter;
            bool is_int = true;
            for(unsigned int i =0 ; i < string_array[2].length(); i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    is_int = false;
                }
            }
            bool is_integer_node = false;
            for(unsigned int i = 0; i < *current_index; i++){
                IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                if(string_array[2] == integer_node->getName()){
                    is_integer_node = true;
                }
            }
            if(is_int || is_integer_node){
                // create new integer node
                if(*current_index < 50){
                    std::shared_ptr<IntegerNode> pointer;
                    bool already_excisted = false;

                    for(unsigned int i = 0; i < *current_index; i++){
                        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                        if(string_array[0] == integer_node->getName()){
                            already_excisted = true;
                        }
                    }

                    int current_value = 0;
                    if(is_integer_node){
                        for(unsigned int i = 0; i < *current_index; i++){
                            IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                            if(string_array[2] == integer_node->getName()){
                                current_value = integer_node->getValue();
                            }
                        }
                    } else{
                        current_value = string_array[2].toInt();
                    }

                    pointer = std::make_shared<IntegerNode>(command, string_array[0], current_value,*line_number);
                    if(!already_excisted){
                        all_created_integer_nodes[*current_index] = pointer;
                        *current_index = *current_index+1;
                    }
                    return pointer;
                } else{
                    return std::shared_ptr<Node> (new ErrorNode(command, "Max number of integers reached", *line_number));
                }
            }
            // STATE = IDLE
            if( string_array[0] == parse_words.STATE && string_array[1] == "="){
                if(string_array[2] == parse_words.IDLE){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.IDLE, *line_number));
                } else if(string_array[2] == parse_words.REMINDER_BREAK){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_BREAK, *line_number));
                } else if(string_array[2] == parse_words.REMINDER_WATER){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_WATER, *line_number));
                } else if(string_array[2] == parse_words.REMINDER_WALK){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_WALK, *line_number));
                } else if(string_array[2] == parse_words.WEATHER_STATION){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.WEATHER_STATION, *line_number));
                } else if(string_array[2] == parse_words.INTERACTIVE_MODE){
                    return std::shared_ptr<Node> (new SetStateNode(command, parse_words.INTERACTIVE_MODE, *line_number));
                } else {
                    return std::shared_ptr<Node> (new ErrorNode(command, "unknown state", *line_number));
                }
            }
            // weatherstation = ACTIVE
            if(string_array[2] == parse_words.ACTIVE){
                return std::shared_ptr<Node> (new SetterNode(command, string_array[0],parse_words.ACTIVE, 0 ,parse_words.MILLI_SECOND_, *line_number ));
            } else if(string_array[2] == parse_words.NON_ACTIVE){
                return std::shared_ptr<Node> (new SetterNode(command, string_array[0],parse_words.NON_ACTIVE, 0 , parse_words.MILLI_SECOND_, *line_number ));
            } else{
                return std::shared_ptr<Node> (new ErrorNode(command, "unknown choice: Not ACTIVE or NON_ACTIVE", *line_number));
            }
            return std::shared_ptr<Node> (new ErrorNode(command, "unknown command of lenght 3", *line_number));
        } else if(string_array[1] == "+" || string_array[1] == "-" || string_array[1] == "/" ||string_array[1] == "*"){
            std::shared_ptr<IntegerNode> lhs;
            std::shared_ptr<IntegerNode> rhs;
            for(unsigned int i = 0; i < *current_index; i++){
                IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                if(integer_node->getName() == string_array[0]){
                    lhs = std::make_shared<IntegerNode>(*integer_node);
                }
                if(integer_node->getName() == string_array[2]){
                    rhs = std::make_shared<IntegerNode>(*integer_node);
                }
            } 
            char letter;
            bool is_int = true;
            for(unsigned int i = 0 ; i < string_array[2].length(); i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    is_int = false;
                }
            }
            if(is_int){
                rhs = std::shared_ptr<IntegerNode> (new IntegerNode(command, "NONVALUE", string_array[2].toInt(), *line_number));
            } 
            if(string_array[1] == "+"){
                return std::shared_ptr<Node>(new MathNode(command, lhs, rhs, MATH_TYPES::PLUS, *line_number));
            }else if(string_array[1] == "-"){
                return std::shared_ptr<Node>(new MathNode(command, lhs, rhs, MATH_TYPES::MINUS, *line_number));
            }else if(string_array[1] == "/"){
                return std::shared_ptr<Node>(new MathNode(command, lhs, rhs, MATH_TYPES::DIVIDE, *line_number));
            }else if(string_array[1] == "*"){
                return std::shared_ptr<Node>(new MathNode(command, lhs, rhs, MATH_TYPES::MULTIPLY, *line_number));
            } else{
                return std::shared_ptr<Node> (new ErrorNode(command, "unknown operator", *line_number));
            }
        }else { 
            // WAIT 10 MINUTES
            if(string_array[0] == parse_words.WAIT){
                char letter;
                for(unsigned int i =0 ; i < string_array[1].length(); i++){
                    letter = string_array[1][i];
                    if(!isDigit(letter)){
                        return std::shared_ptr<Node> (new ErrorNode(command, "time value not a digit", *line_number));
                        break;
                    }
                }
                if(string_array[2] == parse_words.MINUTE_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.MINUTE_, *line_number));
                } else if(string_array[2] == parse_words.HOUR_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.HOUR_, *line_number));
                } else if (string_array[2] == parse_words.SECOND_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.SECOND_, *line_number));
                } else if (string_array[2] == parse_words.MILLI_SECOND_){
                    return std::shared_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.MILLI_SECOND_, *line_number));
                } else {
                    return std::shared_ptr<Node> (new ErrorNode(command, "Unknown time measurement type", *line_number));
                }
            } else{
                return std::shared_ptr<Node> (new ErrorNode(command, "unknow command of lenght 3", *line_number));
            }
        }
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command of lenght 3", *line_number));
        break;
    case 4:
        //shut_down_after = 10 MINUTE
        if(string_array[0] == parse_words.shut_down_after && string_array[1] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[2].length(); i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    return std::shared_ptr<Node> (new ErrorNode(command, "time value not a digit", *line_number));
                    break;
                }
            }
            if(string_array[3] == parse_words.MINUTE_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.MINUTE_, *line_number));
                } else if(string_array[3] == parse_words.HOUR_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.HOUR_, *line_number));
                } else if (string_array[3] == parse_words.SECOND_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.SECOND_, *line_number));
                } else if (string_array[3] == parse_words.MILLI_SECOND_){
                    return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.MILLI_SECOND_, *line_number));
                } else {
                    return std::shared_ptr<Node> (new ErrorNode(command, "Unknown time measurement type", *line_number));
                }
        }
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command of length 4", *line_number));
        break;
    case 5:
        // weatherstation PERIOD = 10 MINUTE
        if((string_array[1] == parse_words.PERIOD || string_array[1] == parse_words.DURATION) && string_array[2] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[3].length(); i++){
                letter = string_array[3][i];
                if(!isDigit(letter)){
                    return std::shared_ptr<Node> (new ErrorNode(command, "time value not a digit", *line_number));
                    break;
                }
            }
            return std::shared_ptr<Node> (new SetterNode(command, string_array[0] , string_array[1], string_array[3].toInt(), string_array[4], *line_number));
        }
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command", *line_number));
    default:
        return std::shared_ptr<Node> (new ErrorNode(command, "unknown command", *line_number));   
        Serial.println("Unknown command: " + command);
        break;
    }
    return std::shared_ptr<Node> (new ErrorNode(command, "unknown command", *line_number));
}


void Node::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    Serial.println(original_string);
}

NODE_TYPES Node::getType(){
    return type;
}

NODE_TYPES ContainsBodyNode::getType(){
    return node_type;
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

void SetterNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
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

void SetStateNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
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

void CommandNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    // TODO IMPLEMENT getter voor micro lidar en if en while statements.
    char letter;
    bool is_digit = true;
    bool param_is_integer_node = false;
    for(unsigned int i =0 ; i < param.length(); i++){
        letter = param[i];
        if(!isDigit(letter)){
            is_digit = false;
            break;
        }
    }
    for(unsigned int i = 0; i < *current_index; i++){
        IntegerNode* Integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(param == Integer_node->getName()){
            is_digit = true;
            param_is_integer_node = true;
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
        if(param_is_integer_node){
            if(command == parse_words.move_head){
                for(unsigned int i = 0; i < *current_index; i++){
                    IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                    if(param == integer_node->getName()){
                        robot.moveHead(integer_node->getValue());
                    }
                }
            } else if(command == parse_words.move_neck){
                for(unsigned int i = 0; i < *current_index; i++){
                    IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
                    if(param == integer_node->getName()){
                        robot.moveNeck(integer_node->getValue());
                    }
                }
            } 
        }else{
            if(command == parse_words.move_head){
                robot.moveHead(param.toInt());
            } else if(command == parse_words.move_neck){
                robot.moveNeck(param.toInt());
            } 
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
            } else{
                ErrorNode error("Unknown command: ", original_string, line_number);
                error.execute(robot, all_created_integer_nodes, current_index);
            }
        }
    } else{
        if(command == parse_words.EOF_){
            ErrorNode error("Unknown command: ", original_string, line_number);
            error.execute(robot, all_created_integer_nodes, current_index);
        }
    }
}

void WaitNode::execute( Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
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

void IfNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){    
    if(CheckIfConditionTrue(robot, all_created_integer_nodes, current_index)){
        Serial.println("("+condition+") == true");
        body.setToStart();
        std::shared_ptr<Node> body_command = body.getCurrentNode();
        body_command->print();
        body_command->execute(robot, all_created_integer_nodes, current_index);
        body_command.reset();
        while(body.gotToNextNode()){
            body_command = body.getCurrentNode();
            body_command->print();
            body_command->execute(robot, all_created_integer_nodes, current_index);
            body_command.reset();
        }
    }else{
        Serial.println("("+condition+") == false");
    }
}

int IfNode::CheckIfConditionTrue(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    condition.trim();
    String string_array[3] = {};
    int current_array_index = 0;
    int start_index = 0;
    for(unsigned int i = 0; i < condition.length(); i++){
        if( condition[i] == ' '){
            string_array[current_array_index] = condition.substring(start_index,i);
            current_array_index++;
            start_index = i+1;
        }
        if( i == condition.length()-1){
            string_array[current_array_index] = condition.substring(start_index,i+1);
        }
    }
    //getState =/!= STATE
    if( string_array[0] == parse_words.getState){
        ROBOT_STATES lhs = robot.getState();
        ROBOT_STATES rhs = ROBOT_STATES::IDLE;
        if(string_array[2] == parse_words.IDLE){
            rhs = ROBOT_STATES::IDLE;
        } else if(string_array[2] == parse_words.REMINDER_BREAK){
            rhs = ROBOT_STATES::REMINDER_BREAK;
        }else if(string_array[2] == parse_words.REMINDER_WATER){
            rhs = ROBOT_STATES::REMINDER_WATER;
        }else if(string_array[2] == parse_words.REMINDER_WALK){
            rhs = ROBOT_STATES::REMINDER_WALK;
        }else if(string_array[2] == parse_words.WEATHER_STATION){
            rhs = ROBOT_STATES::WEATHER_STATION;
        }else if(string_array[2] == parse_words.INTERACTIVE_MODE){
            rhs = ROBOT_STATES::INTERACTIVE_MODE;
        }else if(string_array[2] == parse_words.OFF){
            rhs = ROBOT_STATES::OFF;
        } else{
            ErrorNode error = ErrorNode(condition, "Condition not recognised: Unknown rhs state ", line_number);
            error.execute(robot, all_created_integer_nodes, current_index);
            return false;
        }
        if(string_array[1] == "=="){
            return lhs == rhs;
        } else if (string_array[1] == "!="){
            return lhs != rhs;
        } else{
            ErrorNode error = ErrorNode(condition, "Condition not recognised: Unknown operator ", line_number);
            error.execute(robot, all_created_integer_nodes, current_index);
            return false;
        }
    }
    //getHeadPos >/</<=/>=/==/!= int
    //getNeckPos >/</<=/>=/==/!= int
    //getLastMovementDetected >/</<=/>=/==/!= int
    //x == y
    int lhs, rhs;
    lhs = 0;
    if(string_array[0] == parse_words.getHeadPos){
        lhs = robot.getHeadPos();
    } else if(string_array[0] == parse_words.getNeckPos){
        lhs = robot.getNeckPos();
    } else if(string_array[0] == parse_words.getLastMovementDetected){
        lhs = robot.getLastMovementDetected();
    }// else{
    //     ErrorNode error = ErrorNode(condition, "Condition not recognised: lhs function call not recognized ", line_number);
    //     error.execute(robot);
    //     return false;
    // }
    char letter_rhs;
    char letter_lhs;
    bool is_digit_rhs = true;
    bool is_digit_lhs = true;
    bool lhs_set = false;
    bool rhs_set = false;

    for(unsigned int i =0 ; i < string_array[2].length(); i++){
        letter_rhs = string_array[2][i];
        if(!isDigit(letter_rhs)){
            is_digit_rhs = false;
        }
    }
    for(unsigned int i =0 ; i < string_array[0].length(); i++){
        letter_lhs = string_array[0][i];
        if(!isDigit(letter_lhs)){
            is_digit_lhs = false;
        }
    }
    for(unsigned int i = 0; i < *current_index; i++){
        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(string_array[0] == integer_node->getName()){
            is_digit_lhs = true;
            lhs = integer_node->getValue();
            lhs_set = true;
        }
    }
    for(unsigned int i = 0; i < *current_index; i++){
        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(string_array[2] == integer_node->getName()){
            is_digit_rhs = true;
            rhs = integer_node->getValue();
            rhs_set = true;
        }
    }
    if(is_digit_lhs && lhs_set == false){
        lhs = string_array[0].toInt();
    }
    if(is_digit_rhs&& rhs_set == false){
        rhs = string_array[2].toInt();
    } else{
        ErrorNode error = ErrorNode(condition, "Condition not recognised: rhs is not an integer ", line_number);
        error.execute(robot, all_created_integer_nodes, current_index);
        return false;
    }
    if(string_array[1] == ">"){
        return lhs > rhs;
    } else if(string_array[1] == "<"){
        return lhs < rhs;
    } else if(string_array[1] == "<="){
        return lhs <= rhs;
    } else if(string_array[1] == ">="){
        return lhs >= rhs;
    } else if(string_array[1] == "=="){
        return lhs == rhs;
    } else if(string_array[1] == "!="){
        return lhs != rhs;
    } else{
        ErrorNode error = ErrorNode(condition, "Condition not recognised: Unknown operator", line_number);
        error.execute(robot, all_created_integer_nodes, current_index);
        return false;
    }
    // check if condition is true or false
    ErrorNode error = ErrorNode(condition, "Condition not recognised: global error", line_number);
    error.execute(robot, all_created_integer_nodes, current_index);
    return false;
}

void IfNode::addCommand(std::shared_ptr<Node> command){
    body.append(command);
}

bool IfNode::isViable(){
    return viable;
}

// TODO loop sudden stop.
void WhileNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    std::shared_ptr<Node> body_command;
    int condition_true = CheckIfConditionTrue(robot, all_created_integer_nodes, current_index);
    if(condition_true == 1){
        Serial.println(condition+" == true");
        body.setToStart();
        body_command = body.getCurrentNode();
        body_command->print();
        body_command->execute(robot, all_created_integer_nodes, current_index);
        body_command.reset();
        int condition_true = CheckIfConditionTrue(robot, all_created_integer_nodes, current_index);
        while(condition_true){
            if(!body.gotToNextNode()){
                body.setToStart();
            }
                body_command = body.getCurrentNode();
                if(body_command->getType() == NODE_TYPES::ERROR){
                    body_command->execute(robot, all_created_integer_nodes, current_index);
                    return;
                }
                body_command->print();
                body_command->execute(robot, all_created_integer_nodes, current_index);
                body_command.reset();
            condition_true = CheckIfConditionTrue(robot, all_created_integer_nodes, current_index);
        }
    } else if(condition_true > 1){
        Serial.println(condition+" == true");
        if(body.getLength() == 1){
            condition_true--;
        }
        body.setToStart();
        body_command = body.getCurrentNode();
        body_command->print();
        body_command->execute(robot, all_created_integer_nodes, current_index);
        body_command.reset();
        while(condition_true > 0){
            if(!body.gotToNextNode()){
                body.setToStart();
                if(condition_true <= 0 ){
                    break;
                }
                condition_true--;
            }
            body_command = body.getCurrentNode();
            if(body_command->getType() == NODE_TYPES::ERROR){
                body_command->execute(robot, all_created_integer_nodes, current_index);
                return;
            }
            body_command->print();
            body_command->execute(robot, all_created_integer_nodes, current_index);
            body_command.reset();
        }
    }
}

int WhileNode::CheckIfConditionTrue(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    condition.trim();
    String string_array[3] = {};
    int current_array_index = 0;
    int start_index = 0;
    for(unsigned int i = 0; i < condition.length(); i++){
        if( condition[i] == ' '){
            string_array[current_array_index] = condition.substring(start_index,i);
            current_array_index++;
            start_index = i+1;
        }
        if( i == condition.length()-1){
            string_array[current_array_index] = condition.substring(start_index,i+1);
        }
    }

    //getState =/!= STATE
    if( string_array[0] == parse_words.getState){
        ROBOT_STATES lhs = robot.getState();
        ROBOT_STATES rhs = ROBOT_STATES::IDLE;
        if(string_array[2] == parse_words.IDLE){
            rhs = ROBOT_STATES::IDLE;
        } else if(string_array[2] == parse_words.REMINDER_BREAK){
            rhs = ROBOT_STATES::REMINDER_BREAK;
        }else if(string_array[2] == parse_words.REMINDER_WATER){
            rhs = ROBOT_STATES::REMINDER_WATER;
        }else if(string_array[2] == parse_words.REMINDER_WALK){
            rhs = ROBOT_STATES::REMINDER_WALK;
        }else if(string_array[2] == parse_words.WEATHER_STATION){
            rhs = ROBOT_STATES::WEATHER_STATION;
        }else if(string_array[2] == parse_words.INTERACTIVE_MODE){
            rhs = ROBOT_STATES::INTERACTIVE_MODE;
        }else if(string_array[2] == parse_words.OFF){
            rhs = ROBOT_STATES::OFF;
        } else{
            ErrorNode error = ErrorNode(condition, "Condition not recognised: Unknown rhs state ", line_number);
            error.execute(robot, all_created_integer_nodes, current_index);
            return false;
        }
        if(string_array[1] == "=="){
            return lhs == rhs;
        } else if (string_array[1] == "!="){
            return lhs != rhs;
        } else{
            ErrorNode error = ErrorNode(condition, "Condition not recognised: Unknown operator ", line_number);
            error.execute(robot, all_created_integer_nodes, current_index);
            return false;
        }
    }
    //getHeadPos >/</<=/>=/==/!= int
    //getNeckPos >/</<=/>=/==/!= int
    //getLastMovementDetected >/</<=/>=/==/!= int
    //x == y
    int lhs, rhs;
    lhs = 0;
    if(string_array[0] == parse_words.getHeadPos){
        lhs = robot.getHeadPos();
    } else if(string_array[0] == parse_words.getNeckPos){
        lhs = robot.getNeckPos();
    } else if(string_array[0] == parse_words.getLastMovementDetected){
        lhs = robot.getLastMovementDetected();
    }
    char letter_rhs;
    char letter_lhs;
    bool is_digit_rhs = true;
    bool is_digit_lhs = true;
    bool lhs_set = false;
    bool rhs_set = false;
    for(unsigned int i =0 ; i < string_array[0].length(); i++){
        letter_lhs = string_array[0][i];
        if(!isDigit(letter_lhs)){
            is_digit_lhs = false;
        }
    }
    for(unsigned int i =0 ; i < string_array[2].length(); i++){
        letter_rhs = string_array[2][i];
        if(!isDigit(letter_rhs)){
            is_digit_rhs = false;
        }
    }

    // If lhs is an integer it will always be a case of WHILE(int)
    if(is_digit_lhs){
        lhs =  string_array[0].toInt();
    }

    for(unsigned int i = 0; i < *current_index; i++){
        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(string_array[0] == integer_node->getName()){
            // when left is an integer node its always a case of WHILE(X)
            lhs =  integer_node->getValue();
        }
    }
    for(unsigned int i = 0; i < *current_index; i++){
        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(string_array[2] == integer_node->getName()){
            is_digit_rhs = true;
            rhs = integer_node->getValue();
            rhs_set = true;
        }
    }
    if(is_digit_lhs && lhs_set == false){
        lhs = string_array[0].toInt();
    }
    if(is_digit_rhs&& rhs_set == false){
        rhs = string_array[2].toInt();
    } else{
        ErrorNode error = ErrorNode(condition, "Condition not recognised: rhs is not an integer ", line_number);
        error.execute(robot, all_created_integer_nodes, current_index);
        return false;
    }

    if(condition.indexOf(' ') == -1){
        return lhs;
    }

    if(string_array[1] == ">"){
        return lhs > rhs;
    } else if(string_array[1] == "<"){
        return lhs < rhs;
    } else if(string_array[1] == "<="){
        return lhs <= rhs;
    } else if(string_array[1] == ">="){
        return lhs >= rhs;
    } else if(string_array[1] == "=="){
        return lhs == rhs;
    } else if(string_array[1] == "!="){
        return lhs != rhs;
    } else{
        ErrorNode error = ErrorNode(condition, "Condition not recognised: Unknown operator", line_number);
        error.execute(robot, all_created_integer_nodes, current_index);
        return false;
    }
    // check if condition is true or false
    ErrorNode error = ErrorNode(condition, "Condition not recognised: global error", line_number);
    error.execute(robot, all_created_integer_nodes, current_index);
    return false;
}

void WhileNode::addCommand(std::shared_ptr<Node> command){
    body.append(command);
}

bool WhileNode::isViable(){
    return viable;
}

void IntegerNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    // If valuename already exists change this value to the value of this node.
    for( unsigned int i = 0; i < *current_index; i++){
        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(value_name == integer_node->getName()){
            integer_node->changeValue(current_value);
        }
    }
    return;
}

int IntegerNode::getValue(){
    return current_value;
}

void IntegerNode::changeValue(int new_value){
    current_value = new_value;
}

String IntegerNode::getName(){
    return value_name;
}

void MathNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    IntegerNode * lhs_temp = lhs.get();
    IntegerNode * rhs_temp = rhs.get();
    for(unsigned int i = 0; i < *current_index; i++){
        IntegerNode* integer_node = static_cast<IntegerNode*>(all_created_integer_nodes[i].get());
        if(lhs->getName() == integer_node->getName()){
            lhs_temp = integer_node;
        }  
        if(rhs->getName() == integer_node->getName()){
            rhs_temp = integer_node;
        }
    }
    if(math_operator == MATH_TYPES::PLUS){
        lhs_temp->changeValue(lhs_temp->getValue() + rhs_temp->getValue());
    } else if(math_operator == MATH_TYPES::MINUS){
        lhs_temp->changeValue(lhs_temp->getValue() - rhs_temp->getValue());
    } else if(math_operator == MATH_TYPES::MULTIPLY){
        lhs_temp->changeValue(lhs_temp->getValue() * rhs_temp->getValue());
    } else if(math_operator == MATH_TYPES::DIVIDE){
        lhs_temp->changeValue(lhs_temp->getValue() / rhs_temp->getValue());
    }
    Serial.println("New value of "+lhs->getName()+" = " + String(lhs_temp->getValue()));
}

void ErrorNode::execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index){
    Serial.println("["+String(line_number)+"]ERROR: " + original_string + " -> " + error_message);
}

void Node::print(){
    Serial.println("["+String(line_number)+"]Node -> " + original_string);
}

void SetterNode::print(){
    Serial.println("["+String(line_number)+"]SetterNode -> " +  to_set + " " + setter_type + " " + String(time_period) + " " + time_measurements);
}

void SetStateNode::print(){
    Serial.println("["+String(line_number)+"]SetStateNode -> set state to " + state);
}

void CommandNode::print(){
    Serial.println("["+String(line_number)+"]CommandNode -> "+ command + "(" + param + ")");
}

void WaitNode::print(){
    Serial.println("["+String(line_number)+"]WaitNode -> wait for "+ String(time_period) + " " + time_measurements);
}

void ErrorNode::print(){
    Serial.println("["+String(line_number)+"]ErrorNode -> " + error_message);
}

void IfNode::print(){
    Serial.println("["+String(line_number)+"]IfNode -> IF(" + condition + ")("+body.getLength()+")");
}

void WhileNode::print(){
    Serial.println("["+String(line_number)+"]WhileNode -> WHILE(" + condition + ")("+body.getLength()+")");
}

void IntegerNode::print(){
    Serial.println("["+String(line_number)+"]" + String(value_name) + " = " + String(current_value));
}

void MathNode::print(){
    if(math_operator == MATH_TYPES::PLUS){
        Serial.println("["+String(line_number)+"]MathNode ->" + lhs->getName() + "+" + String(rhs->getValue()));
    } else if(math_operator == MATH_TYPES::MINUS){
        Serial.println("["+String(line_number)+"]MathNode ->" + lhs->getName() + "-" + String(rhs->getValue()));
    } else if(math_operator == MATH_TYPES::MULTIPLY){
        Serial.println("["+String(line_number)+"]MathNode ->" + lhs->getName() + "*" + String(rhs->getValue()));
    } else if(math_operator == MATH_TYPES::DIVIDE){
        Serial.println("["+String(line_number)+"]MathNode ->" + lhs->getName() + "/" + String(rhs->getValue()));
    }
}