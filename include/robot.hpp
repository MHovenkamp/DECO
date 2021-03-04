# ifndef ROBOT_HPP
# define BUZZER_HPP

#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"
#include "internalBleSensors.hpp" 

class Robot{
private:
    Servo head_servo;
    Servo neck_servo;
    PIRSensor pir_sensor;
    InternalBLESensors internal_sensors;
    OledScreen face_screen;
    Animations animations;
public:
    Robot(  const unsigned int head_servo_pin,
            const unsigned int neck_servo_pin,
            const unsigned int pir_sensor_pin ):
            head_servo(Servo(head_servo_pin)),
            neck_servo(Servo(neck_servo_pin)),
            pir_sensor(PIRSensor(pir_sensor_pin))
        {}

    void setup();

    void robotTask();

    void headServoTask();

    void neckServoTask();

    void PIRTask();

    void idleState();

    void rngMovementState();

    void reminderBreak();

    void reminderWalk();

    void reminderWater();
};

#endif
