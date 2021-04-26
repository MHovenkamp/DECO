# ifndef INTERFACE_HPP
# define INTERFACE_HPP

#include <support.hpp>
#include <robot.hpp>
#include <interface_enums.hpp>

enum class INTERFACE_MODES{
    IDLE,
    REPL,
    FILE
};

class Node{
public:
    String original_string;
    NODE_TYPES type;
    Node(String original_string, NODE_TYPES type):
        original_string(original_string),
        type(type){}

    virtual void execute(Robot & robot) = 0;
};

class SetterNode: public Node{
private:
    String to_set;
    PARSE_WORDS setter_type;
    int time_period;
    PARSE_WORDS time_measurements;
public:
    SetterNode(String original_string, String to_set ,PARSE_WORDS setter_type, int time_period, PARSE_WORDS time_measurement, NODE_TYPES type = NODE_TYPES::SETTER):
    Node(original_string, type),
    to_set(to_set),
    setter_type( setter_type),
    time_measurements( time_measurements){}

    void execute(Robot & robot);

    unsigned int getTime();
};

class SetStateNode: public Node{
private:
    PARSE_WORDS state;
public:
    SetStateNode(String original_string, PARSE_WORDS state, NODE_TYPES type = NODE_TYPES::STATE_SETTER):
    Node(original_string, type),
    state(state){}

    void execute(Robot & robot);
};

class CommandNode: public Node{
private:
    String command;
    int param;
    bool has_param = false;
public:
    CommandNode(String original_string, String command, int param, bool has_param = false, NODE_TYPES type = NODE_TYPES::COMMAND):
    Node(original_string, type),
    command(command),
    param(param){}

    void execute(Robot & robot);
};

class WaitNode: public Node{
private:
    int time_period;
    PARSE_WORDS time_measurements;
public:
    WaitNode(String original_string, int time_period, PARSE_WORDS time_measurements, NODE_TYPES type = NODE_TYPES::WAIT):
    Node(original_string, type),
    time_period(time_period),
    time_measurements(time_measurements){}

    void execute(Robot & robot);
};

class ErrorNode: public Node{
private:
    String error_message;
public:
    ErrorNode(String original_string, String error_message,  NODE_TYPES type = NODE_TYPES::ERROR):
    Node(original_string, type),
    error_message( error_message){}

    void execute(Robot & robot);
};

class Interface{
private:
    Robot &robot;
    INTERFACE_MODES mode =  INTERFACE_MODES::IDLE;

    Node parseCommand(String command);
public:
    Interface(Robot & robot):
    robot(robot){}

    void run();

    void repl();

    void file();
};


#endif // INTERFACE_HPP