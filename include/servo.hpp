// ==========================================================================
// servo.hpp
// Copyright (C) 2021  Maaike Hovenkamp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// ==========================================================================

# ifndef SERVO_HPP
# define SERVO_HPP

#include "support.hpp"

/**
 * @brief ServoPositions struct containing coockie cutter coordinates for servo motors
 * 
 */
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
    unsigned int speed = 1;
    unsigned int current_degree_wait;
    unsigned int current_degree_goal = 90;
    unsigned int current_degree = 0;
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
    void turnToDegree(const int degree);

    /**
     * @brief Get the Current Degree 
     * 
     * @return unsigned int 
     */
    unsigned int getCurrentDegree();

};

#endif //SERVO_HPP