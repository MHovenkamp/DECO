# ifndef ANIMATIONS_HPP
# define ANIMATIONS_HPP

#include "support.hpp"
#include "../bitmaps/face.hpp"
#include "../bitmaps/walk.hpp"
#include "../bitmaps/water.hpp"
#include "../bitmaps/bigBreak.hpp"

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
    std::array<unsigned char, 1024> getFrame(unsigned int i){
        if( i >= amount_of_frames){
            return bitmaps[amount_of_frames-1];
        } else {
            return bitmaps[i];
        }
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
    std::array<std::array<unsigned char, 1024>, 2> face_array = {{face::face_0, face::face_1}};
    Animation<2> face_idle = Animation<2>(face_array, 2, 128, 64);

    std::array<std::array<unsigned char, 1024>, 5> face_blink_array = {{face_blink::face_blink_0,
                                                                    face_blink::face_blink_1,
                                                                    face_blink::face_blink_2,
                                                                    face_blink::face_blink_3,
                                                                    face_blink::face_blink_4}};
    Animation<5> face_blink = Animation<5>(face_blink_array, 5, 128, 64);

    std::array<std::array<unsigned char, 1024>, 3> walk_array = {{walk::walk_0, walk::walk_1, walk::walk_2}};
    Animation<3> walk = Animation<3>(walk_array, 3, 128, 64);

    std::array<std::array<unsigned char, 1024>, 3> water_array = {{water::water_0, water::water_1, water::water_2}};
    Animation<3> water = Animation<3>(water_array, 3, 128, 64);

    std::array<std::array<unsigned char, 1024>, 4> big_break_array = {{big_break::break_0,big_break::break_0, big_break::break_1, big_break::break_1}};
    Animation<4> big_break = Animation<4>(big_break_array, 4, 128, 64);
};



#endif // ANIMATIONS_HPP