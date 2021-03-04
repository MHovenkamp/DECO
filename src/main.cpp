#include "robot.hpp"


void setup() {
  Serial.begin(9600);
  delay(2000);
  Robot robot_test(4,3,2);
  robot_test.robotTask();
};


void loop(){}
