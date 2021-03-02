// #include "support.hpp"
//#include "animations.hpp"
#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"

OledScreen test_screen;
Animations animations;

rtos::Thread screen;

void screen_loop(){
  while(true){
    test_screen.showAnimation<2>( animations.face_idle );
    test_screen.showAnimation<5>( animations.face_blink );
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  test_screen.setup();
  screen.start(screen_loop);
};


void loop(){}
