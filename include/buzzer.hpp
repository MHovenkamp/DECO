# ifndef BUZZER_HPP
# define BUZZER_HPP

struct Songs{
    // std::array<std::array<int, 2>, X> attention_please = {};
    // std::array<std::array<int, 2>, X> goodmorning = {};
    std::array<std::array<int, 2>, 8> going_to_sleep = {{ 
                                                            {NOTE_C4,4}, 
                                                            {NOTE_G3,8}, 
                                                            {NOTE_G3,8}, 
                                                            {NOTE_A3,4}, 
                                                            {NOTE_G3,4}, 
                                                            {0,4}, 
                                                            {NOTE_B3,4}, 
                                                            {NOTE_C4,4}
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
            Serial.println(melody[thisNote][0]);
            noteDuration = 1000 / melody[thisNote][1];
            tone(buzzer_pin, melody[thisNote][0], noteDuration);

            pauseBetweenNotes = noteDuration * 1.30;
            delay(pauseBetweenNotes);
            noTone(buzzer_pin);
        }
    }
};


#endif