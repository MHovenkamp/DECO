# ifndef BUZZER_HPP
# define BUZZER_HPP

/**
 * @brief struct Songs containing al playable tunes, currently not in use since tone() not functional in platformio.
 * 
 */
struct Songs{
    std::array<std::array<int, 2>, 4> start_up = {{
                                                        {NOTE_A4,4}, {NOTE_F4,4}, {NOTE_E4,4}, {NOTE_F4,4}
                                                    }};
    std::array<std::array<int, 2>, 4> shut_down = {{ 
                                                        {NOTE_A3,4}, {NOTE_F3,4}, {NOTE_E3,4}, {NOTE_F3,4}
                                                    }};
    std::array<std::array<int, 2>, 2> notification = {{
                                                        {NOTE_A4, 4}, {NOTE_D4, 4}
                                                    }};
};


class Buzzer{
private:
    int buzzer_pin;
public:
    Buzzer( int buzzer_pin ):
    buzzer_pin( buzzer_pin ){
        pinMode(buzzer_pin, OUTPUT);
    }

    template <int T>
    void playSong( std::array<std::array<int, 2>, T> &melody ){
        int noteDuration = 0;
        int pauseBetweenNotes = 0;
        for (int thisNote = 0; thisNote < T; thisNote++) {
            noteDuration = 1000 / melody[thisNote][1];
            tone(buzzer_pin, melody[thisNote][0], noteDuration);

            pauseBetweenNotes = noteDuration * 1.30;
            delay(pauseBetweenNotes);
            noTone(buzzer_pin);
        }
    }
};


#endif