# ifndef ROBOT_INTERPRETER_HPP
# define ROBOT_INTERPRETER_HPP

#include <programCode.cpp>
#include <robot.hpp>
#include <robot_interpreter_enums.hpp>
#include <double_linked_list.hpp>

/**
 * @brief enum class containing the interpreter nodes.
 * 
 */
enum class INTERPRETER_MODES{
    IDLE,
    REPL,
    FILE,
    PRECOMPILED_FILE
};

/**
 * @brief Base Node class for parsed commands
 * 
 */
class Node{
public:
    const PARSE_WORDS parse_words;
    String original_string;
    NODE_TYPES type;
    int line_number;
    /**
     * @brief Construct a new Node object
     * 
     * @param original_string : String
     * @param type : NODE_TYPES
     */
    Node(String original_string, int line_number, NODE_TYPES type = NODE_TYPES::NODE):
        original_string(original_string),
        type(type),
        line_number(line_number){}
    virtual ~Node(){};

    /**
     * @brief execute node function
     * 
     * @param robot : Robot &
     */


    // TODO execute maken die gerbuik maakt van de integer node mogelijk moeten casten.
    virtual void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index);

    /**
     * @brief print the infromation of the node
     * 
     */
    virtual void print();

    /**
     * @brief Get the type of the node
     * 
     * @return NODE_TYPES 
     */
    NODE_TYPES getType();
};

/**
 * @brief IntegerNode class holding an integer
 * 
 */
class IntegerNode: public Node{
private:
    int current_value;
    String value_name;
public:
    /**
     * @brief Construct a new Integer Node object
     * 
     * @param original_string : String
     * @param current_value : int
     * @param line_number : int
     * @param type : NODE_TYPES
     */
    IntegerNode(String original_string, String value_name, int current_value, int line_number, NODE_TYPES type = NODE_TYPES::INTEGER_NODE):
        Node(original_string, line_number, type),
        current_value(current_value),
        value_name(value_name){}

    /**
     * @brief execute the integer node
     * 
     * @param robot 
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index);

    /**
     * @brief print the integer node
     * 
     */
    void print();

    /**
     * @brief Get the Value of the integerNode
     * 
     * @return int 
     */
    int getValue();

    /**
     * @brief change the value of the int
     * 
     */
    void changeValue(int new_value);

    /**
     * @brief Get the name of the integer
     * 
     * @return String 
     */
    String getName();
};

/**
 * @brief MathNode to be executed on an integer node
 * 
 */
class MathNode: public Node{
private:
    std::shared_ptr<IntegerNode> lhs;
    std::shared_ptr<IntegerNode> rhs;
    MATH_TYPES math_operator;
public:
    /**
     * @brief Construct a new Math Node object
     * 
     * @param original_string : string
     * @param lhs : IntegerNode
     * @param rhs : int
     * @param line_number : int 
     * @param type : NODE_TYPES
     */
    MathNode(String original_string, std::shared_ptr<IntegerNode> lhs, std::shared_ptr<IntegerNode> rhs, MATH_TYPES math_operator, int line_number, NODE_TYPES type = NODE_TYPES::MATH_NODE):
        Node(original_string, line_number, type),
        lhs(lhs),
        rhs(rhs),
        math_operator(math_operator){}

    /**
     * @brief execute the math node
     * 
     * @param robot 
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index);

    /**
     * @brief print the math node values
     * 
     */
    void print();
};

/**
 * @brief SetterNode class, derived from Node.
 * 
 */
class SetterNode: public Node{
private:
    String to_set;
    String setter_type;
    int time_period;
    String time_measurements;
public:
    /**
     * @brief Construct a new Setter Node object
     * 
     * @param original_string : String
     * @param to_set : String
     * @param setter_type : String
     * @param time_period : int
     * @param time_measurement String
     * @param type : NODE TYPES
     */
    SetterNode(String original_string, String to_set ,String setter_type, int time_period, String time_measurement, int line_number, NODE_TYPES type = NODE_TYPES::SETTER):
    Node(original_string, line_number, type),
    to_set(to_set),
    setter_type(setter_type),
    time_period(time_period),
    time_measurements(time_measurement){}

    /**
     * @brief execute SetterNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;

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
    String state;
public:
    /**
     * @brief Set the State Node object
     * 
     * @param original_string : String
     * @param state : String
     * @param type : NODE_TYPES
     */
    SetStateNode(String original_string, String state, int line_number, NODE_TYPES type = NODE_TYPES::STATE_SETTER):
    Node(original_string, line_number, type),
    state(state){}

    /**
     * @brief execute SetStateNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;
};

/**
 * @brief CommandNode class, derived from Node
 * 
 */
class CommandNode: public Node{
private:
    String command;
    String param;
    bool has_param = false;
public:
    /**
     * @brief Construct a new Command Node object
     * 
     * @param original_string : String
     * @param command : String
     * @param param : String
     * @param has_param : bool
     * @param type : NODE_TYPES
     */
    CommandNode(String original_string, String command, String param, int line_number, bool has_param = false, NODE_TYPES type = NODE_TYPES::COMMAND):
    Node(original_string, line_number, type),
    command(command),
    param(param){}

    /**
     * @brief execute CommandNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;
};

/**
 * @brief WaitNode class, derived from Node
 * 
 */
class WaitNode: public Node{
private:
    int time_period;
    String time_measurements;
public:
    /**
     * @brief Construct a new Wait Node object
     * 
     * @param original_string : String
     * @param time_period : int
     * @param time_measurements : String
     * @param type : NODE_TYPES
     */
    WaitNode(String original_string, int time_period, String time_measurements, int line_number, NODE_TYPES type = NODE_TYPES::WAIT):
    Node(original_string, line_number, type),
    time_period(time_period),
    time_measurements(time_measurements){}

    /**
     * @brief execute WaitNode function
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;
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
    ErrorNode(String original_string, String error_message, int line_number, NODE_TYPES type = NODE_TYPES::ERROR):
    Node(original_string, line_number, type),
    error_message( error_message){}

    /**
     * @brief Print the error message in this node
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;
};

/**
 * @brief COntainsBodyNode base class for nodes containing a boyd of commands.
 * 
 */
class ContainsBodyNode{
private:
    NODE_TYPES node_type;
public:
    /**
     * @brief Construct a new Contains Body Node object
     * 
     * @param node_type 
     */
    ContainsBodyNode(NODE_TYPES node_type):
        node_type(node_type){}

    /**
     * @brief Construct a new Contains Body Node object
     * 
     */
    ContainsBodyNode(){}

    /**
     * @brief Destroy the Contains Body Node object
     * 
     */
    virtual ~ContainsBodyNode(){};

    /**
     * @brief 
     * 
     * @param robot : & Robot
     * @param all_created_integer_nodes : std::shared_ptr<Node> *
     * @param current_index, unsigned int *
     * @return int 
     */
    virtual int CheckIfConditionTrue(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index);

    /**
     * @brief add command to body node
     * 
     * @param command : std::shared_ptr<Node>
     */
    virtual void addCommand(std::shared_ptr<Node> command);

    virtual bool isViable();

    /**
     * @brief Get the onderlying derived object
     * 
     * @return NODE_TYPES 
     */
    NODE_TYPES getType();
};


/**
 * @brief ErrorNode class, derived from Node
 * 
 */
class IfNode: public Node, public ContainsBodyNode{
private:
    String condition;
    DoubleLinkedList<Node> body;
    bool viable;
    /**
     * @brief Check if the condition of the if statement is true or false
     * 
     * @return true 
     * @return false 
     */
    int CheckIfConditionTrue(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;
    
public:
    /**
     * @brief Construct a new Error Node object
     * 
     * @param original_string : String
     * @param condition : String
     * @param type : NODE_TYPES
     */
    IfNode(String original_string, String condition, bool viable, int line_number, NODE_TYPES type = NODE_TYPES::IFNODE):
    Node(original_string, line_number, type),
    ContainsBodyNode(NODE_TYPES::IFNODE),
    condition(condition),
    viable(viable){}

    /**
     * @brief Add command to the body of the if node
     * 
     * @param command 
     */
    void addCommand(std::shared_ptr<Node> command) override;

    /**
     * @brief Print the error message in this node
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;

    /**
     * @brief return if the created ifnode is usable, used by createIfNode function.
     * 
     * @return true 
     * @return false 
     */
    bool isViable() override;
};

class WhileNode: public Node, public ContainsBodyNode{
private:
    String condition;
    DoubleLinkedList<Node> body;
    bool viable;
    /**
     * @brief Check if the condition of the if statement is true or false
     * 
     * @return true 
     * @return false 
     */
    int CheckIfConditionTrue(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;
public:
    /**
     * @brief Construct a new Error Node object
     * 
     * @param original_string : String
     * @param condition : String
     * @param type : NODE_TYPES
     */
    WhileNode(String original_string, String condition, bool viable, int line_number, NODE_TYPES type = NODE_TYPES::WHILENODE):
    Node(original_string, line_number, type),
    ContainsBodyNode(NODE_TYPES::WHILENODE),
    condition(condition),
    viable(viable){}

    /**
     * @brief Add command to the body of the if node
     * 
     * @param command 
     */
    void addCommand(std::shared_ptr<Node> command) override;

    /**
     * @brief Print the error message in this node
     * 
     * @param robot : Robot &
     */
    void execute(Robot & robot, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index) override;

    /**
     * @brief print the infromation of the node
     * 
     */
    void print() override;

    /**
     * @brief return if the created WaitNode is usable, used by createWaitNode function.
     * 
     * @return true 
     * @return false 
     */
    bool isViable() override;
};

/**
 * @brief Interpreter class
 * 
 */
class Interpreter{
private:
    Robot &robot;
    INTERPRETER_MODES mode =  INTERPRETER_MODES::IDLE;
    PARSE_WORDS parse_words;

    /**
     * @brief parse a command and return correct node
     * 
     * @param command : String
     * @return std::shared_ptr<Node> 
     */
    std::shared_ptr<Node> parseCommand(String command, int* line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int *current_index);

    /**
     * @brief Create a Possible IfNode object otherwise return error.
     * 
     * @param command 
     * @return std::shared_ptr<IfNode> 
     */
    std::shared_ptr<IfNode> createPossibleIf(String command, int * line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index);

    /**
     * @brief Create a Possible WhileNode object otherwise return error.
     * 
     * @param command 
     * @return std::shared_ptr<WhileNode> 
     */
    std::shared_ptr<WhileNode> createPossibleWhile(String command, int * line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int * current_index);

    /**
     * @brief REPL functionality
     * 
     */
    void repl();

    /**
     * @brief FILE functionality
     * 
     */
    void file(String file_text);

    /**
     * @brief Create a Command List of the given text
     * 
     * @param text 
     * @return DoubleLinkedList 
     */
    DoubleLinkedList<Node> createCommandList(String text, int* line_number, std::shared_ptr<Node> *all_created_integer_nodes, unsigned int* current_index_int);

    /**
     * @brief Read the given file information from serial monitor
     * 
     * @return String 
     */
    String readFileFromSerial();
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