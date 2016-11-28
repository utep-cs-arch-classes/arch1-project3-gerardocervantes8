
/**Author: Gerardo Cervantes
Instructor: Eric Freudenthal
Assignment: Make a game with the MSP430
Class: Computer Architecture 1 - CS 3432*/


/**Starts playing given sound*/
void sound_start(int typeOfSound);

/**Used to play a song from an array*/
void sound_play_song();

/**Stops currently playing song*/
void sound_stop();

/**Updates the buzzer and song that is playing*/
void sound_update(int winStreak);

/**Plays the winning sound_stop
Sound that is played is determined by the winStreak*/
void sound_win_play(int winStreak);
