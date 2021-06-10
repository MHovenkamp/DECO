# ifndef OLEDSCREEN_HPP
# define OLEDSCREEN_HPP

#include "support.hpp"
#include "animations.hpp"

enum class ROBOT_FRAMES{
    FACE_BLINK,
    FACE_IDLE,
    WALK_REMINDER,
    WATER_REMINDER,
    BREAK_REMINDER,
    SEARCHING_LEFT_UP,
    SEARCHING_RIGHT_UP,
    SEARCHING_LEFT_DOWN,
    SEARCHING_RIGHT_DOWN
};


/**
 * @brief OledScreen driver
 * 
 */
class OledScreen{
private:
    const int screen_with = 128;
    const int screen_heigth = 64;
    const int oled_reset = 4;
    const int screen_address = 0x3C;
    Adafruit_SSD1306 display;
    bool active = true;
    ROBOT_FRAMES animation_choice = ROBOT_FRAMES::FACE_IDLE;
    bool show_animation = true;
    Animations animation_presets;
    
    /**
     * @brief sub call sending i2C commands to display
     * 
     * @tparam T 
     * @param animation 
     */
    template <unsigned int T>
    void showAnimationSubCall(Animation<T> &animation){
        if(show_animation){
            for( unsigned int i = 0; i<animation.getAmountOfFrames(); i++){
                const unsigned int amount_of_pixels = 1024;
                unsigned char bitmap_new[amount_of_pixels] = {};
                std::array<unsigned char, amount_of_pixels> bitmap = animation.getFrame(i);          
                for(unsigned int j = 0; j < amount_of_pixels; j++){
                    bitmap_new[j] = bitmap[j];
                }
                display.clearDisplay();
                display.drawBitmap(0,0, bitmap_new, animation.getAnimationWidth(), animation.getAnimationHeight(), WHITE);
                // delay(1000/animation.getAmountOfFramesPerSecond());
                rtos::ThisThread::sleep_for(MS(1000/animation.getAmountOfFramesPerSecond()));
            }
        }
    }

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
     * @brief displays the current buffer on oled, only call from I2C thread.
     * 
     */
    void flashOled(){
        display.display();
    }

    /**
     * @brief clear display
     * 
     */
    void clearScreen(){
        display.clearDisplay();   
    }

    /**
     * @brief Set the Animation object
     * 
     * @param animation_choice_p 
     */
    void setAnimation(ROBOT_FRAMES animation_choice_p){
        show_animation = true;
        animation_choice = animation_choice_p;
    }

    /**
     * @brief show set animation on display
     * 
     */
    void showAnimation(){
        if( animation_choice ==  ROBOT_FRAMES::FACE_IDLE){
            showAnimationSubCall<2>(animation_presets.face_idle);
        } else if( animation_choice ==  ROBOT_FRAMES::FACE_BLINK){
            showAnimationSubCall<5>(animation_presets.face_blink);
        } else if( animation_choice ==  ROBOT_FRAMES::WALK_REMINDER){
            showAnimationSubCall<3>(animation_presets.walk);
        } else if(animation_choice ==  ROBOT_FRAMES::WATER_REMINDER){
            showAnimationSubCall<3>(animation_presets.water);
        } else if(animation_choice ==  ROBOT_FRAMES::BREAK_REMINDER){
            showAnimationSubCall<4>(animation_presets.big_break);
        } else if(animation_choice ==  ROBOT_FRAMES::SEARCHING_LEFT_UP){
            showAnimationSubCall<1>(animation_presets.searching_up_left);
        } else if(animation_choice ==  ROBOT_FRAMES::SEARCHING_RIGHT_UP){
            showAnimationSubCall<1>(animation_presets.searching_up_right);
        } else if(animation_choice ==  ROBOT_FRAMES::SEARCHING_LEFT_DOWN){
            showAnimationSubCall<1>(animation_presets.searching_down_left);
        } else if(animation_choice ==  ROBOT_FRAMES::SEARCHING_RIGHT_DOWN){
            showAnimationSubCall<1>(animation_presets.searching_down_right);
        } else{
            showAnimationSubCall<2>(animation_presets.face_idle);
        }
    }


    /**
     * @brief showText function thats show given string on screen
     * 
     * @tparam T : unsigned int
     * @param text : char*, text to show on screen
     * @param amount_of_symbols : std::array<unsigned int, T>, amount of symbols per line
     * @param amount_of_lines : unsigned int, amount of lines
     */
    template <unsigned int T>
    void setDisplayText( char* text, std::array<unsigned int, T> amount_of_symbols, unsigned int amount_of_lines){
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
    }

    /**
     * @brief flashes text current buffer after call setDisplayText
     * 
     */
    void flashTextDisplay(){
        show_animation = false;
    }

};

#endif //OLEDSCREEN_HPP