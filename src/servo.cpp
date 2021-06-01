#include <../include/servo.hpp>

void Servo::setup(){
    int steps = 2000/180;
    current_degree_wait = 500 + ( 90 * steps );
    current_degree_goal = 90;
    while(current_degree != 90){
        if( current_degree_goal > current_degree){
        current_degree += speed;
        } else if( current_degree_goal < current_degree ){
            current_degree -= speed;
        }
        int steps = 2000/180;
        current_degree_wait = 500 + ( current_degree * steps );
        
        digitalWrite(servo_pin, LOW);
        wait_us(2);
        digitalWrite(servo_pin, HIGH);
        wait_us(current_degree_wait);
        digitalWrite(servo_pin, LOW);
        rtos::ThisThread::sleep_for(MS(20));
    }
    current_degree = getCurrentDegree();
}

void Servo::servoTask(){
    if( current_degree_goal > current_degree){
        current_degree += speed;
    } else if( current_degree_goal < current_degree ){
        current_degree -= speed;
    }
    int steps = 2000/180;
    current_degree_wait = 500 + ( current_degree * steps );
    
    digitalWrite(servo_pin, LOW);
    wait_us(2);
    digitalWrite(servo_pin, HIGH);
    wait_us(current_degree_wait);
    digitalWrite(servo_pin, LOW);
    // rtos::ThisThread::sleep_for(MS(20)); // 20 ms standard for sg932r TowerPro, change when changing servo
};

void Servo::turnToDegree(const int degree){
    int new_degree = degree;
    if(new_degree < 0){
        new_degree = 0;    
    }else if(new_degree > 180 ){
        new_degree = 180;
    }
    current_degree_goal = new_degree;
};

unsigned int Servo::getCurrentDegree(){
    return current_degree;
}