
/**Author: Gerardo Cervantes
Instructor: Eric Freudenthal
Assignment: Make a game with the MSP430
Class: Computer Architecture 1 - CS 3432*/


#include <msp430.h>
#include "sound.h"


/**Indicates what sound should be playing*/
static char isPlayingSound = 0;

/* Type of Sound indices:
0 no sound
1 play winning beep
2 play losing sound song*/


//static int loseSound[5] = {400,500,600,715,795};

/**400 and 500 pitch*/
//static char loseSound[2] = {4,5};


/**Given the winStreak, updates the sound to check if need to change the buzzer based on hte song playing
@Parameter winstreak is used to be able to play the win sound based on the current winStreak*/
void sound_update(int winStreak){
  switch(isPlayingSound){
  case 1: break; //sound_win_play(winStreak); 
  case 2: break; //sound_play_song();
  case 3: sound_pacDot(); break;
  case 4: sound_pacDot(); break;
  case 5: sound_pacDot(); break;
  case 6: sound_pacDot(); break;
  case 7: sound_pacDot(); break;
   
  }
}

/**Plays the win sound, win sound is dependent on the winStreak*/
/*void sound_win_play(int winStreak){
  static char soundCounter = 0;
  buzzer_set_period(sound_win_pitch(winStreak));
    //Amount of time win sound lasts
  if(soundCounter > 15){ 
    sound_stop();
    soundCounter = 0;
  }
  else{
    soundCounter++;
  }
}

/**Starts sound of given song*/
void sound_start(int typeOfSound){
  
  isPlayingSound = typeOfSound;
  if(isPlayingSound){ 
    buzzer_start();
  }
}

/**Creates sound for when pacdot is collected*/
void sound_pacDot(){
  if(isPlayingSound == 3 || isPlayingSound == 4){
    buzzer_set_period(400);
    isPlayingSound++;
  }
  else if(isPlayingSound == 5 || isPlayingSound == 6){
    buzzer_set_period(500);
    isPlayingSound++;
  }
  else{
    sound_stop();
  }
  
}

/**Stops song that is playing*/
void sound_stop(){
  isPlayingSound = 0;
  buzzer_stop();
}


/**Sound starts playing lose song*/
/*void sound_play_song(){
  static char soundCounter = 0;
  static char note = 0;
  int soundToPlay = loseSound[note] * 100;
  buzzer_set_period(soundToPlay);

  //Length that each note will play to
  if(soundCounter > 2){
    //Has finished playing all notes of the sound effect
    if(note >= (sizeof(loseSound)-1) ){
      sound_stop();
      note = 0;
    }
    //Increment to the next note of the song
    else{
      note++;
      soundCounter = 0;
    }
  }
  else{
    soundCounter++;
  }
}
*/

/**Returns the pitch that win sound should be
@Parameter winStreak used to change pitch of sound*/
/*int sound_win_pitch(int winStreak){
  
  return (winStreak*100) + 100;
  }*/

    
