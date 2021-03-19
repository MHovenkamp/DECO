# ifndef ROBOT_HPP
# define ROBOT_HPP

#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"
#include "internalBleSensors.hpp" 
#include "buzzer.hpp"

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
    Buzzer buzzer;
    unsigned int break_time = 2 * HOUR;
    unsigned int walk_time = 30 * MINUTE;
    unsigned int water_time = 1 * HOUR;

    unsigned int break_time_duration = 15 * MINUTE;
    unsigned int walk_time_duration = 5 * MINUTE;
    unsigned int water_time_duration = 30 * SECOND;
    ROBOT_STATES current_state = ROBOT_STATES::IDLE;

    void idleState();

    void reminderBreak();

    void reminderWalk();

    void reminderWater();

    void showWeatherStation();
public:
    Robot(  Servo & head_servo,
            Servo & neck_servo,
            PIRSensor & pir_sensor,
            int buzzer_pin ):
            head_servo(head_servo),
            neck_servo(neck_servo),
            pir_sensor(pir_sensor),
            buzzer( Buzzer(buzzer_pin) )
        {}

    void run();

    unsigned int getBreakTime();
    unsigned int getWalkTime();
    unsigned int getWaterTime();

    void setBreakTime(unsigned long time);
    void setWalkTime(unsigned long time);
    void setWaterTime(unsigned long time);

    void setBreakTimeDuration(unsigned long time);
    void setWalkTimeDuration(unsigned long time);
    void setWaterTimeDuration(unsigned long time);

    void setState(ROBOT_STATES state);
    ROBOT_STATES getState();

    void moveHead( int pos );
    void moveNeck( int pos );

    void rngMovement();
    void returnToStartPos();
    void setup();

    template <unsigned int T>
    void playSound(std::array<std::array<int, 2>, T> song){
        buzzer.playSong<T>(song);
    }

};

#endif
