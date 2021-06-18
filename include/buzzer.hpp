// ==========================================================================
// buzzer.hpp
// Copyright (C) 2021  Maaike Hovenkamp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// ==========================================================================

# ifndef BUZZER_HPP
# define BUZZER_HPP

#include "notes.hpp"

/**
 * @brief struct Songs containing al playable tunes, currently not in use since tone() not functional in platformio.
 * 
 */
struct Songs{
    Notes notes;
    std::array<std::array<int, 2>, 4> start_up = {{
                                                        {notes.NOTE_A4,4}, {notes.NOTE_F4,4}, {notes.NOTE_E4,4}, {notes.NOTE_F4,4}
                                                    }};
    std::array<std::array<int, 2>, 4> shut_down = {{ 
                                                        {notes.NOTE_A3,4}, {notes.NOTE_F3,4}, {notes.NOTE_E3,4}, {notes.NOTE_F3,4}
                                                    }};
    std::array<std::array<int, 2>, 2> notification = {{
                                                        {notes.NOTE_A4, 4}, {notes.NOTE_D4, 4}
                                                    }};
};

/**
 * @brief Buzzer class to support Piezo type buzzer
 * 
 */
class Buzzer{
private:
    int buzzer_pin;
public:
    /**
     * @brief Construct a new Buzzer object
     * 
     * @param buzzer_pin : int
     */
    Buzzer( int buzzer_pin ):
    buzzer_pin( buzzer_pin ){
        pinMode(buzzer_pin, OUTPUT);
    }

    /**
     * @brief playSong function, plays a song on buzzer
     * 
     * @tparam T : int, amount of notes
     * @param melody std::array<std::array<int, 2>, T> &, song to play
     */
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