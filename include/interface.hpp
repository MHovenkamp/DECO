# ifndef INTERFACE_HPP
# define INTERFACE_HPP

#include <support.hpp>
#include <interface_enums.hpp>

enum class INTERFACE_MODES{
    IDLE,
    REPL,
    FILE
};

class Node{
private:
    String original_string;
    NODE_TYPES type;
public:
    Node(String original_string, NODE_TYPES type):
        original_string(original_string),
        type(type){}

    virtual void execute() = 0;
};

class SetterNode: public Node{
private:
    String to_set;
    PARSE_WORDS setter_type;
    int time_period;
    PARSE_WORDS time_measurements;
public:
    SetterNode(String original_string, NODE_TYPES type, String to_set ,PARSE_WORDS setter_type, int time_period, PARSE_WORDS time_measurement):
    Node(original_string, type),
    to_set(to_set),
    setter_type( setter_type),
    time_measurements( time_measurements){}

    void execute();
};

class CommandNode: public Node{
private:
    String command;
    int param;
    bool has_param = false;
public:
    CommandNode(String original_string, NODE_TYPES type, String command, int param, bool has_param = false):
    Node(original_string, type),
    command(command),
    param(param){}

    void execute();
};

class Interface{
private:
    INTERFACE_MODES mode =  INTERFACE_MODES::IDLE;

    Node parseCommand(String command);
public:
    Interface();

    void run();

    void repl();

    void file();
};


#endif // INTERFACE_HPP