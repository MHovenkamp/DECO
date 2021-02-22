#include <../include/servo.hpp>

void Servo::servoTask(){
    digitalWrite(servo_pin, LOW);
    wait_us(2);
    digitalWrite(servo_pin, HIGH);
    wait_us(current_degree_wait);
    digitalWrite(servo_pin, LOW);
    rtos::ThisThread::sleep_for(MS(50));
};

void Servo::turnToDegree(int degree){
    if(degree < 0){
        degree = 0;    
    }else if(degree > 180 ){
        degree = 180;
    }
    int steps = 2000/180;
    current_degree_wait = 500 + ( degree * steps );
};