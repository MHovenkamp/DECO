# ifndef SERVO_HPP
# define SERVO_HPP

#include "support.hpp"

struct ServoPositions{
    int left_half = 135;
    int left_full = 180;
    int right_hal = 45;
    int right_full = 0;
    int up_half = 135;
    int up_full = 180;
    int down_half = 45;
    int down_full = 0;
};


/**
 * @brief Servo class, driver class to support servo SG92 and sg92R
 * 
 */
class Servo{
private:
    const int servo_pin;
    unsigned int current_degree_wait;
    unsigned int current_degree_goal;
    unsigned int current_degree;
public:
    /**
     * @brief Construct a new Servo object
     * 
     * @param servo_pin, pin number for servo to be attached to, digital pin
     */
    Servo(int servo_pin):
    servo_pin(servo_pin){
        pinMode(servo_pin, OUTPUT);
    }

    /**
     * @brief setup servo
     * 
     */
    void setup();

    /**
     * @brief continues sending of servo commands to servo, run in task scheduler loop
     * 
     */
    void servoTask();

    /**
     * @brief gives the servo the command to turn to given degree.
     * 
     * @param degree, degree you want servo to turn to between 0 and 180
     */
    void turnToDegree(int degree);

    unsigned int getCurrentDegree();

};

#endif //SERVO_HPP