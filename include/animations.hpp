# ifndef ANIMATIONS_HPP
# define ANIMATIONS_HPP

#include "support.hpp"
#include "../bitmaps/face.hpp"
#include "../bitmaps/walk.hpp"
#include "../bitmaps/water.hpp"
#include "../bitmaps/bigBreak.hpp"
#include "../bitmaps/searching.hpp"

/**
 * @brief Animation class, class with saved bitmaps containing animations
 * 
 * @tparam T amount of frames in animation
 */
template <unsigned int T>
class Animation{
private:
    std::array<std::array<unsigned char, 1024>, T> bitmaps;
    const unsigned int amount_of_frames;
    const unsigned int animation_width;
    const unsigned int animation_height;
    const unsigned int amount_of_frames_per_second;
public:
    /**
     * @brief Construct a new Animation object
     * 
     * @param bitmaps_outer, std::array<std::array<unsigned char, 1024>, T> arrays containing bitmaps
     * @param amount_of_frames, unsigned int 
     * @param animation_width, unsigned int in pixels
     * @param animation_height, unsigned int in pixels
     */
    Animation(std::array<std::array<unsigned char, 1024>, T> bitmaps_outer,unsigned int amount_of_frames, unsigned int animation_width, unsigned int animation_height, unsigned int amount_of_frames_per_second):
        amount_of_frames( amount_of_frames ),
        animation_width( animation_width ),
        animation_height( animation_height ),
        amount_of_frames_per_second( amount_of_frames_per_second )
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

    /**
     * @brief Get the Amount Of Frames
     * 
     * @return uint8_t 
     */
    uint8_t getAmountOfFrames(){
        return amount_of_frames;
    }

    /**
     * @brief Get the Animation Width
     * 
     * @return uint8_t 
     */
    uint8_t getAnimationWidth(){
        return animation_width;
    }

    /**
     * @brief Get the Animation Height
     * 
     * @return uint8_t 
     */
    uint8_t getAnimationHeight(){
        return animation_height;
    }

    /**
     * @brief Get the Amount Of Frames Per Second 
     * 
     * @return uint8_t 
     */
    uint8_t getAmountOfFramesPerSecond(){
        return amount_of_frames_per_second;
    }
};

/**
 * @brief struct Animations containing all animations that can be played.
 * 
 */
struct Animations{
    std::array<std::array<unsigned char, 1024>, 2> face_array = {{face::face_0, face::face_1}};
    Animation<2> face_idle = Animation<2>(face_array, 2, 128, 64, 10);

    std::array<std::array<unsigned char, 1024>, 5> face_blink_array = {{face_blink::face_blink_0,face_blink::face_blink_1,face_blink::face_blink_2,face_blink::face_blink_3,face_blink::face_blink_4}};
    Animation<5> face_blink = Animation<5>(face_blink_array, 5, 128, 64, 10);

    std::array<std::array<unsigned char, 1024>, 3> walk_array = {{walk::walk_0, walk::walk_1, walk::walk_2}};
    Animation<3> walk = Animation<3>(walk_array, 3, 128, 64, 10);

    std::array<std::array<unsigned char, 1024>, 3> water_array = {{water::water_0, water::water_1, water::water_2}};
    Animation<3> water = Animation<3>(water_array, 3, 128, 64, 10);

    std::array<std::array<unsigned char, 1024>, 4> big_break_array = {{big_break::break_0,big_break::break_0, big_break::break_1, big_break::break_1}};
    Animation<4> big_break = Animation<4>(big_break_array, 4, 128, 64, 10);

    std::array<std::array<unsigned char, 1024>, 1> searching_up_left_array = {{searching::searching_up_left}};
    Animation<1> searching_up_left = Animation<1>(searching_up_left_array, 1, 128, 64, 1000);

    std::array<std::array<unsigned char, 1024>, 1> searching_up_right_array = {{searching::searching_up_right}};
    Animation<1> searching_up_right = Animation<1>(searching_up_right_array, 1, 128, 64, 1000);

    std::array<std::array<unsigned char, 1024>, 1> searching_down_left_array = {{searching::searching_down_left}};
    Animation<1> searching_down_left = Animation<1>(searching_down_left_array, 1, 128, 64, 1000);

    std::array<std::array<unsigned char, 1024>, 1> searching_down_right_array = {{searching::searching_down_right}};
    Animation<1> searching_down_right = Animation<1>(searching_down_right_array, 1, 128, 64, 1000);

};



#endif // ANIMATIONS_HPP