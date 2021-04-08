# ifndef ROBOT_HPP
# define ROBOT_HPP

#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"
#include "internalBleSensors.hpp" 
#include "buzzer.hpp"
#include "microLidar.hpp"

/**
 * @brief enum class ROBOT_STATES, all posible robot states
 * 
 */
enum class ROBOT_STATES{
    IDLE,
    REMINDER_BREAK,
    REMINDER_WATER,
    REMINDER_WALK,
    WEATHER_STATION,
    OFF
};

/**
 * @brief main Robot class supporting the social robotics robot.
 * 
 */
class Robot{
private:
    Servo &head_servo;
    Servo &neck_servo;
    PIRSensor &pir_sensor;
    InternalBLESensors internal_sensors;
    OledScreen face_screen;
    Animations animations;
    Buzzer buzzer;
    MicroLidar lidar;

    unsigned int break_time = 2 * HOUR;
    unsigned int walk_time = 30 * MINUTE;
    unsigned int water_time = 1 * HOUR;
    unsigned int shutdown_after = 5 * MINUTE;

    bool break_time_active = true;
    bool walk_time_active = true;
    bool water_time_active = true;

    unsigned int break_time_duration = 15 * MINUTE;
    unsigned int walk_time_duration = 5 * MINUTE;
    unsigned int water_time_duration = 30 * SECOND;
    ROBOT_STATES current_state = ROBOT_STATES::IDLE;
    std::array<std::array<int, 3>,10> surroundings_map;
    unsigned int map_steps_neck = 20;
    unsigned int map_steps_head = 20;
    int difference_map_x;
    int difference_map_y;
    int distance_found_object;
    int found_object_x;
    int found_object_y;

    /**
     * @brief Idle state of the robot; idle animation.
     * 
     */
    void idleState();

    /**
     * @brief Break reminder state of the robot; attention movement, break animation
     * 
     */
    void reminderBreak();

    /**
     * @brief Walk reminder state of the robot; attention movement, notification, walk animation
     * 
     */
    void reminderWalk();

    /**
     * @brief Water reminder state of the robot; attention movement, notification, water animation
     * 
     */
    void reminderWater();

    /**
     * @brief Weather station state of the robot; weatherStation animation
     * 
     */
    void showWeatherStation();

public:
    /**
     * @brief Construct a new Robot object
     * 
     * @param head_servo : Servo
     * @param neck_servo : Servo
     * @param pir_sensor : PIRSensor
     * @param buzzer_pin : Buzzer
     */
    Robot(  Servo & head_servo,
            Servo & neck_servo,
            PIRSensor & pir_sensor,
            int buzzer_pin ):
            head_servo(head_servo),
            neck_servo(neck_servo),
            pir_sensor(pir_sensor),
            buzzer( Buzzer(buzzer_pin) )
        {}

    /**
     * @brief main run function of the robot
     * 
     */
    void run();

    /**
     * @brief Get the Break Time loop interval
     * 
     * @return unsigned int 
     */
    unsigned int getBreakTime();
    /**
     * @brief Get the Walk Time loop interval
     * 
     * @return unsigned int 
     */
    unsigned int getWalkTime();
    /**
     * @brief Get the Water Time loop interval
     * 
     * @return unsigned int 
     */
    unsigned int getWaterTime();

    /**
     * @brief Set the Break Time loop interval and if break reminder is active
     * 
     * @param time : unsigned long, loop interval
     * @param active : bool
     */
    void setBreakTime(unsigned long time, bool active);
    /**
     * @brief Set the Walk Time loop interval and if walk reminder is active
     * 
     * @param time : unsigned long, loop interval
     * @param active : bool
     */
    void setWalkTime(unsigned long time, bool active);
    /**
     * @brief Set the Water Time loop interval and if water reminder is active
     * 
     * @param time : unsinged long, loop interal
     * @param active : bool
     */
    void setWaterTime(unsigned long time, bool active);
    /**
     * @brief Set the Shutdown After loop interval and if water reminder is active
     * 
     * @param time : unsigned long, shut down after x time of inactivity. 
     */
    void setShutdownAfter(unsigned long time);

    /**
     * @brief Set the Break Time Duration 
     * 
     * @param time : unsinged long
     */
    void setBreakTimeDuration(unsigned long time);
    /**
     * @brief Set the Walk Time Duration 
     * 
     * @param time 
     */
    void setWalkTimeDuration(unsigned long time);
    /**
     * @brief Set the Water Time Duration 
     * 
     * @param time 
     */
    void setWaterTimeDuration(unsigned long time);

    /**
     * @brief Set the State of the robot
     * 
     * @param state : ROBOT_STATES, robot state to set state to
     */
    void setState(ROBOT_STATES state);
    /**
     * @brief Get the current State of the robot
     * 
     * @return ROBOT_STATES
     */
    ROBOT_STATES getState();

    /**
     * @brief move the head servo, head up and down
     * 
     * @param pos : int, between 40 and 140
     */
    void moveHead( int pos );
    /**
     * @brief move the neck servo, head left and rigth
     * 
     * @param pos pos : int, between 40 and 140
     */
    void moveNeck( int pos );

    /**
     * @brief move the robot randomly
     * 
     */
    void rngMovement();
    /**
     * @brief return robot to starting position, 90 degrees for head servo and 90 degress for neck servo.
     * 
     */
    void returnToStartPos();
    /**
     * @brief setup the robot, call before use of other functions
     * 
     */
    void setup();

    /**
     * @brief play preprogramed song on buzzer
     * 
     * @tparam T : unsigned int, amount of notes
     * @param song : std::array<std::array<int, 2>, T>, song to be played
     */
    template <unsigned int T>
    void playSound(std::array<std::array<int, 2>, T> song){
        buzzer.playSong<T>(song);
    }

    /**
     * @brief shut down robot untill reactivate by movement on front of PIR sensor. 
     * 
     */
    void shutDown();

    /**
     * @brief update the surroundings map to detect a person.
     * 
     */
    void scanSurroundings();

    /**
     * @brief get x and y closest objec
     * 
     */
    void findClosestObject();

    /**
     * @brief Makes the robot search out the user and look at hiim/follow him.
     * 
     */
    void interactiveMode();

    bool followClosestObject();
};

#endif
