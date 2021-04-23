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
            return CommandNode(command, NODE_TYPES::COMMAND, string_array[0], 0, false);
        } 
        Serial.println("unknown command: " + command);
        break;
    case 2:
        if(string_array[0] == "interactive_mode" || string_array[0] == "shutdown"){
            return CommandNode(command, NODE_TYPES::COMMAND, string_array[0], 0, false);
        } 
        Serial.println("unknown command: " + command);
        return CommandNode(command, NODE_TYPES::COMMAND, string_array[0], string_array[1], true);
        break;
    case 3:
        if(string_array[1] == "="){
            if(string_array[-1] == "ACTIVE"){
                return SetterNode(command, NODE_TYPES::SETTER, string_array[0],PARSE_WORDS::ACTIVE, 0 ,0 );
            } else if(string_array[-1] == "NON_ACTIVE"){
                return SetterNode(command, NODE_TYPES::SETTER, string_array[0],PARSE_WORDS::NON_ACTIVE, 0 ,0 );
            }
        }
        Serial.println("Incorrect command: " + command);
    default:
        Serial.println("Unknown command: " + command);
        break;
    }
}

void Interface::repl(){
    Serial.println("Enter command.");
    String command = "";
    if(Serial.available()){
        command = Serial.readString();
    }
    Node parsed_command = parseCommand(command); 
}
