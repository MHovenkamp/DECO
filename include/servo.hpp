# ifndef SERVO_HPP
# define SERVO_HPP

#include "support.hpp"

/**
 * @brief Servo class, driver class to support servo SG92 and sg92R
 * 
 */
class Servo{
private:
    int servo_pin;
    int current_degree_wait;
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
};

#endif //SERVO_HPP