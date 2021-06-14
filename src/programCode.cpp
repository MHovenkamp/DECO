#include <Arduino.h>


struct code{
    String program_code = R"(
SETUP:
    move_head 0
    move_neck 0
LOOP:
    done = 0
    head = 0
    neck = 0
    WHILE(done == 0){
        getDistance
        IF(getDistance < 300){
            done = 1
        }
        WHILE(neck < 180){
            neck + 20
            move_neck neck
            WAIT 1 SECOND
            IF(getDistance < 300){
                getDistance
                done = 1
            }
        }
        neck = 0
        IF(head < 180){
            head + 20
        }
        move_head head
        IF(head >= 180){
            head = 0
        }
        WAIT 1 SECOND
    }
    IF(done == 1){
        return_to_start_pos
        playSound notification
    }
    WHILE(done == 1){
        WAIT 1 SECOND
    }
EOF:
    )";
};