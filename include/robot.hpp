# ifndef ROBOT_HPP
# define ROBOT_HPP

#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"
#include "internalBleSensors.hpp" 

enum class ROBOT_STATES{
    IDLE,
    REMINDER_BREAK,
    REMINDER_WATER,
    REMINDER_WALK,
    WEATHER_STATION
};

class Robot{
private:
    Servo &head_servo;
    Servo &neck_servo;
    PIRSensor &pir_sensor;
    InternalBLESensors internal_sensors;
    OledScreen face_screen;
    Animations animations;
    unsigned int break_time = 2 * HOUR;
    unsigned int walk_time = 30 * MINUTE;
    unsigned int water_time = 1 * HOUR;
    ROBOT_STATES current_state = ROBOT_STATES::IDLE;

    void idleState();

    void reminderBreak();

    void reminderWalk();

    void reminderWater();

    void showWeatherStation();
public:
    Robot(  Servo & head_servo,
            Servo & neck_servo,
            PIRSensor & pir_sensor ):
            head_servo(head_servo),
            neck_servo(neck_servo),
            pir_sensor(pir_sensor)
        {}

    void run();

    unsigned int getBreakTime();
    unsigned int getWalkTime();
    unsigned int getWaterTime();

    void setBreakTime(unsigned long time);
    void setWalkTime(unsigned long time);
    void setWaterTime(unsigned long time);

    void setState(ROBOT_STATES state);
    ROBOT_STATES getState();

    void moveHead( int pos );
    void moveNeck( int pos );

    void rngMovement();
    void returnToStartPos();
    void setup();

};

#endif
