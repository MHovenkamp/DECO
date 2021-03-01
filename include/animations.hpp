# ifndef ANIMATIONS_HPP
# define ANIMATIONS_HPP

#include "support.hpp"
#include "../bitmaps/face.hpp"

template <unsigned int T>
class Animation{
private:
    std::array<std::array<unsigned char, 1024>, T> bitmaps;
    unsigned int amount_of_frames;
    unsigned int animation_width;
    unsigned int animation_height;
public:
    Animation(std::array<std::array<unsigned char, 1024>, T> bitmaps_outer,unsigned int amount_of_frames, unsigned int animation_width, unsigned int animation_height):
        amount_of_frames( amount_of_frames ),
        animation_width( animation_width ),
        animation_height( animation_height )
    {
        bitmaps = bitmaps_outer;
    };
    unsigned char * getFrame(unsigned int i){
        std::array<unsigned char, 1024> temp;
        uint8_t* return_map = new uint8_t[1024];
        if( i >= amount_of_frames){
            temp = bitmaps[amount_of_frames-1];
        } else {
            temp = bitmaps[i];
        }
        for(unsigned int i = 0; i < 1024; i++){
            return_map[i] = temp[i];
        }
        return return_map;
    }

    uint8_t getAmountOfFrames(){
        return amount_of_frames;
    }

    uint8_t getAnimationWidth(){
        return animation_width;
    }

    uint8_t getAnimationHeight(){
        return animation_height;
    }
};

struct Animations{
	// std::array<std::array<unsigned char, 1024>, 2> face_array = {{face::face_bits, face::face_blink_bits}};
    std::array<std::array<unsigned char, 1024>, 2> face_array = {{face::face_bits, face::face_blink_bits}};
    Animation<2> face_idle = Animation<2>(face_array, 2, 128, 64);
};



#endif // ANIMATIONS_HPP