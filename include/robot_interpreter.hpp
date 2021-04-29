# ifndef ROBOT_INTERPRETER_HPP
# define ROBOT_INTERPRETER_HPP

#include <robot.hpp>
#include <robot_interpreter_enums.hpp>

/**
 * @brief enum class containing the interpreter nodes.
 * 
 */
enum class INTERPRETER_MODES{
    IDLE,
    REPL,
    FILE
};

/**
 * @brief Base Node class for parsed commands
 * 
 */
class Node{
public:
    String original_string;
    NODE_TYPES type;
    /**
     * @brief Construct a new Node object
     * 
     * @param original_string : String
     * @param type : NODE_TYPES
     */
    Node(String original_string, NODE_TYPES type):
        original_string(original_string),
        type(type){}
    virtual ~Node(){};

    /**
     * @brief execute node function
     * 
     * @param robot : Robot &
     */
    virtual void execute(Robot & robot);
};

/**
 * @brief SetterNode class, derived from Node.
 * 
 */
class SetterNode: public Node{
private:
    String to_set;
    PARSE_WORDS setter_type;
    int time_period;
    PARSE_WORDS time_measurements;
public:
    /**
     * @brief Construct a new Setter Node object
     * 
     * @param original_string : String
     * @param to_set : String
     * @param setter_type : PARSE_WORDS
     * @param time_period : int
     * @param time_measurement PARSE_WORDS
     * @param type : NODE TYPES
     */
    SetterNode(String original_string, String to_set ,PARSE_WORDS setter_type, int time_period, PARSE_WORDS time_measurement, NODE_TYPES type = NODE_TYPES::SETTER):
    Node(original_string, type),
    to_set(to_set),
    setter_type(setter_type),
    time_measurements(time_measurement){}

    /**
     * @brief execute SetterNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot) override;

    /**
     * @brief Get the Time object
     * 
     * @return unsigned int 
     */
    unsigned int getTime();
};

/**
 * @brief SetStateNode node, derived from Node
 * 
 */
class SetStateNode: public Node{
private:
    PARSE_WORDS state;
public:
    /**
     * @brief Set the State Node object
     * 
     * @param original_string : String
     * @param state : PARSE_WORDS
     * @param type : NODE_TYPES
     */
    SetStateNode(String original_string, PARSE_WORDS state, NODE_TYPES type = NODE_TYPES::STATE_SETTER):
    Node(original_string, type),
    state(state){}

    /**
     * @brief execute SetStateNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot) override;
};

/**
 * @brief CommandNode class, derived from Node
 * 
 */
class CommandNode: public Node{
private:
    String command;
    int param;
    bool has_param = false;
public:
    /**
     * @brief Construct a new Command Node object
     * 
     * @param original_string : String
     * @param command : String
     * @param param : int
     * @param has_param : bool
     * @param type : NODE_TYPES
     */
    CommandNode(String original_string, String command, int param, bool has_param = false, NODE_TYPES type = NODE_TYPES::COMMAND):
    Node(original_string, type),
    command(command),
    param(param){}

    /**
     * @brief execute CommandNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot) override;
};

/**
 * @brief WaitNode class, derived from Node
 * 
 */
class WaitNode: public Node{
private:
    int time_period;
    PARSE_WORDS time_measurements;
public:
    /**
     * @brief Construct a new Wait Node object
     * 
     * @param original_string : String
     * @param time_period : int
     * @param time_measurements : PARSE_WORDS
     * @param type : NODE_TYPES
     */
    WaitNode(String original_string, int time_period, PARSE_WORDS time_measurements, NODE_TYPES type = NODE_TYPES::WAIT):
    Node(original_string, type),
    time_period(time_period),
    time_measurements(time_measurements){}

    /**
     * @brief execute WaitNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot) override;
};

/**
 * @brief ErrorNode class, derived from Node
 * 
 */
class ErrorNode: public Node{
private:
    String error_message;
public:
    /**
     * @brief Construct a new Error Node object
     * 
     * @param original_string : String
     * @param error_message : String
     * @param type : NODE_TYPES
     */
    ErrorNode(String original_string, String error_message,  NODE_TYPES type = NODE_TYPES::ERROR):
    Node(original_string, type),
    error_message( error_message){}

    /**
     * @brief Print the error message in this node
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot) override;
};

/**
 * @brief Interpreter class
 * 
 */
class Interpreter{
private:
    Robot &robot;
    INTERPRETER_MODES mode =  INTERPRETER_MODES::IDLE;

    std::unique_ptr<Node> parseCommand(String command);

    /**
     * @brief REPL functionality
     * 
     */
    void repl();

    /**
     * @brief FILE functionality
     * 
     */
    void file();
public:
    /**
     * @brief Construct a new Interpreter object
     * 
     * @param robot : Robot &
     */
    Interpreter(Robot & robot):
    robot(robot){}

    /**
     * @brief Main run function
     * 
     */
    void run();

};


#endif // ROBOT_INTERPRETER_HPP