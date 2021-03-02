# ifndef OLEDSCREEN_HPP
# define OLEDSCREEN_HPP

#include "support.hpp"
#include "animations.hpp"

class OledScreen{
private:
    int screen_with = 128;
    int screen_heigth = 64;
    int oled_reset = 4;
    int screen_address = 0x3C;
    Adafruit_SSD1306 display;
public:
    OledScreen():
    display(Adafruit_SSD1306(screen_with, screen_heigth, &Wire, oled_reset))
    {}

    void setup(){
        if(!display.begin(SSD1306_SWITCHCAPVCC, screen_address)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;);
        }
        display.display();
        delay(200);
        display.clearDisplay();
    };

    template <unsigned int T>
    void showAnimation(Animation<T>& animation){
        for( unsigned int i = 0; i<animation.getAmountOfFrames(); i++){
            const unsigned int amount_of_pixels = 1024;
            unsigned char bitmap_new[amount_of_pixels] = {};
            std::array<unsigned char, amount_of_pixels> bitmap = animation.getFrame(i);            
            for(unsigned int j = 0; j < amount_of_pixels; j++){
                bitmap_new[j] = bitmap[j];
            }
            display.clearDisplay();
            display.drawBitmap(0,0, bitmap_new, animation.getAnimationWidth(), animation.getAnimationHeight(), WHITE);
            display.display();
            delay(1);
            rtos::ThisThread::sleep_for(MS(333));
        }
    }

};

#endif //OLEDSCREEN_HPP