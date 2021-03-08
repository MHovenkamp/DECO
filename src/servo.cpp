#include <../include/servo.hpp>

void Servo::setup(){
    int steps = 2000/180;
    current_degree_wait = 500 + ( 90 * steps );
    for(unsigned int i = 0; i < 100; i++){
        digitalWrite(servo_pin, LOW);
        wait_us(2);
        digitalWrite(servo_pin, HIGH);
        wait_us(current_degree_wait);
        digitalWrite(servo_pin, LOW);
    }
    current_degree = 90;
}

void Servo::servoTask(){
    if( current_degree_goal > current_degree){
        current_degree += 1;
    } else if( current_degree_goal < current_degree ){
        current_degree -= 1;
    }
    int steps = 2000/180;
    current_degree_wait = 500 + ( current_degree * steps );
    
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
    current_degree_goal = degree;
};