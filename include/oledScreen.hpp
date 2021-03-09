# ifndef OLEDSCREEN_HPP
# define OLEDSCREEN_HPP

#include "support.hpp"
#include "animations.hpp"

/**
 * @brief OledScreen driver
 * 
 */
class OledScreen{
private:
    int screen_with = 128;
    int screen_heigth = 64;
    int oled_reset = 4;
    int screen_address = 0x3C;
    Adafruit_SSD1306 display;
    
public:
    /**
     * @brief Construct a new Oled Screen object
     * 
     */
    OledScreen():
    display(Adafruit_SSD1306(screen_with, screen_heigth, &Wire, oled_reset))
    {}

    /**
     * @brief setup function for display, call before use.
     * 
     */
    void setup(){
        if(!display.begin(SSD1306_SWITCHCAPVCC, screen_address)) {
            Serial.println(F("SSD1306 allocation failed"));
            for(;;);
        }
        display.display();
        delay(200);
        display.clearDisplay();
    };

    /**
     * @brief showAnimation function that shows a given Animation on the screen
     * 
     * @tparam T amount of frames
     * @param animation Animation object containing animation bitmaps.
     */
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
            rtos::ThisThread::sleep_for(MS(1000/animation.getAmountOfFramesPerSecond()));
        }
    }

    template <unsigned int T>
    void showText( char* text, std::array<unsigned int, T> amount_of_symbols, unsigned int amount_of_lines){
        display.setFont(&FreeSans9pt7b);
        display.clearDisplay();
        display.setTextSize(1);      
        display.setTextColor(SSD1306_WHITE); 
        display.cp437(true);         
        unsigned int current_y = 12;
        unsigned int current_x;
        unsigned int current_symbol = 0;
        unsigned int line_width = 0;
        float temp; 
        for( unsigned int j = 0; j < amount_of_lines; j++){
            line_width = (9 * amount_of_symbols[j]);
            temp = (float(screen_with) - float(line_width)) / float(2);
            current_x = int(temp) - 1;
            if(line_width > 80){
                current_x = 0;
            }
            display.setCursor(current_x, current_y);
            for( unsigned int i = 0; i < amount_of_symbols[j]; i++){
                if( text[i] == '\n'){
                    current_y += 20;
                    display.setCursor(current_x,current_y);
                } else {
                    display.write(text[current_symbol]);
                }
                current_symbol++;
            }  
        }
        display.display();
    }

};

#endif //OLEDSCREEN_HPP