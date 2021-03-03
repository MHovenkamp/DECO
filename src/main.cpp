// #include "support.hpp"
//#include "animations.hpp"
#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"
#include "buzzer.hpp"

OledScreen test_screen;
Animations animations;
Buzzer buzzer(6);
Songs songs;

// rtos::Thread screen;

// void screen_loop(){
//   while(true){
//     test_screen.showAnimation<2>( animations.face_idle );
//     test_screen.showAnimation<5>( animations.face_blink );
//     for(unsigned int i = 0; i <10; i++){
//       test_screen.showAnimation<3>( animations.walk );
//     }
//     for(unsigned int i = 0; i <10; i++){
//       test_screen.showAnimation<3>( animations.water );
//     }
//     for(unsigned int i = 0; i <10; i++){
//       test_screen.showAnimation<4>( animations.big_break );
//     }
//   }
// }

void setup() {
  Serial.begin(9600);
  delay(2000);
  // test_screen.setup();
  // screen.start(screen_loop);


  buzzer.playSong<songs.going_to_sleep.size()>( songs.going_to_sleep );
};


void loop(){}
