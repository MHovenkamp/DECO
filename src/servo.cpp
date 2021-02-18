#include <../include/servo.hpp>

void Servo::servoTask(){
    digitalWrite(servo_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(servo_pin, HIGH);
    delayMicroseconds(current_degree_wait);
    digitalWrite(servo_pin, LOW);
    delay(5);
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