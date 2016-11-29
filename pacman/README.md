

#PACMAN


##How to Install game to MSP430

Do the command "make all", in the main directoy to
install the program onto the msp430.

~~~
$ make all
~~~

Enter the timingGame directory and then do

~~~
$ make load
~~~

So that the program can be loaded onto the MSP430


To delete binaries:

~~~
$ make clean
~~~

##Summary of Game
The program is pacman and is ran on a msp430.

Pacman utilizes the 4 buttons on the extension to the board, sound, and the led screen to display the game.
Once you load the program onto the msp430, the game will start.
To control pacman you will need to use the 4 buttons on the msp430 extension board.

Objective:  You must collect all the pacdots (6 of them) in the game.
How can you lose? You will lose if you touch an enemy, you can tell who an enemy is by their red color.
Enemies are always red.
The enemies in this game are the red sliding disks

Enemies will be able to move, so watch out.

You can't go through any of the blue fences that are in the game.
Fences are always blue.

##Game Input

4 buttons

* Button 1:  Up button
* Button 2:  Down button
* Button 3:  Left button
* Button 4:  Right button

##Game Output

Screen

* Color Red: Enemies
* Color Blue: Fences
* Color Yellow: Player (pacman) and pacdots
* Color Green: Game over screen. (Either win or loss)
* Color Green/Red: Text to display score
* Color Black: Is the background color used, can't interact with the background

Other output

* Buzzer:   Sound is outputted using the buzzer.
* Green LED: Indicates when the cpu is on (for efficiency).


##Files in Repository

* pacman.c (contains main)
* buzzer.c buzzer.h
* sound.c sound.h
* movingLayer.c movingLayer.h
* enemy.c enemy.h
* Makefile



##Internally how the game runs

The program first starts by initalizing everything it will need to use.  The program then starts drawing all the layers.  Interrupts are then turned on and CPU is turned off.  The CPU then turns on every time an interrupts happens.  The interrupt handler then takes care of advancing every moving layer and doing any needed operation for the games that are needed before the CPU is turned off again.

In the case of pacman, during an interrupt.  The game checks for collisions between fences, pacdots and other objects.  


* Buttons:

##Optimizations

There were optimizations that were done to the program.  Most of the optimizations were done to reduce
amount of memory used in ram.  While making the program, I found there was very little ram to use, causing errors
when trying to use more.

* Obstacle fence optimization:  Instead of saving positions, color, and shape of the obstacle fence.  Intead we only save the region (which we were already saving for object collision), the region will be saved for object collision.  With the optimization, we only draw the fence once, then we can free the struct we used to draw it, and only keep the regions for collision detection.  Since we only drew once, if an object collides with the fences they will disappear, but with the regions we saved, we can ensure that no object touches the fence.

* Constants:  Global variables (including structs) that didn't change had the constant modifier added to them.  This would cause the variable to be added to flash memory instead.  With flash memory there was no struggle with using it all, so it was fine to change them to constants.

* Variable Size:  I made sure to make global variables char instead of short if there was enough space for the variable in char. Paying attention to all the global variables declared and rethinking whether they should be char, short, or long was done for this optimization.


##Artifical Intelligence for Enemies

The enemy AI in this game does not use randomness.  So the enemies follow a set pattern/states.  The enemies were made faster than the player because the AI was not smart enough to get the player.  There are 2 enemies in the game.  Knowing where they will be is difficult despite them not being random.  This is because they have many states, and their states change whenever they hit a wall.


Whenever any moving objects hits a wall, their velocity turns to 0.  The artifical intelligence in this game checks whenever the velocity of the enemies is 0, if it is, then it updates their velocity to a new velocity.  The enemies can go in all 8 directions.  Up, down, left, right, down-left, down-right, up-left, up-right.  The next state they go to determines what direction.  I decided what direction each state will have seemingly arbitrarily.



##Overview of Files


* pacman.c

Contains the main method. Starts by initalizing, then proceeds by drawing onto the lcd screen.  The CPU is then turned off.
The game redraws the screen everytime there is an interrupt.  There are 15 interrupts in a second that are used to redraw the screen
and to calculate collisions, advance moving layer, update AI, update sound.  Green LED being off indicates that the cpu is off, green led
is on when cpu is on, the exception to this is when player is in a game over screen.

* pacman.h

Contain the structs used for to make Layers and moving Layers, they contains all the objects that are displayed in the game
Contains all the pacdots, enemies (gotten from extern file), fences, and pacman which are drawn on the lcd screen. 

* enemies.c, enemies.h

Contains the struct of the enemy.  Contains method which figures out next movement of the enemy. Next movement is only
updating if the velocity of the enemy was previously 0 (encountered wall).

* buzzer.c, buzzer.h

The buzzer.c file is used to be able to turn on the sound.
buzzer_init() should be called to be able to use.

* sound.c, sound.h

Contains the winning and lose sounds. Can be used to plays or stop the sounds.
Songs are contained in an integer array.

* movingLayer.c, movingLayer.h

Contains the structure used for creating a moving layer.  Also contains the functions/methods for advancing the moving layer
to it's next position.



##Things to Improve on

* There was many more objects I wanted to add the game but couldn't because of hardware limitations.  Knowing this in advance, I could have change
how some of the current objects operate and could have chnaged how I decided to make the game.  A lot of developer time was done on fixing bugs
based on hardware limitations, having a better idea about them and how to optimize them would have made it easier.

* Modularizing the game more, divide the program up into more C files.  Including making a C file with pacdots, which would contains all the pacdot
structs, object collision for the pacdots, and keeping track of amount of pacdots player has.  Also another C file containing all the fences, their structs
and regions associated with them.

* Adding more songs would have helped the game greatly improve.

* Improving the game over screen and making it so they could restart the game with button presses instead of having to reload the game
or press the reset button on the msp430.  With the way that the program was done, this would be hard to implement because most information about inital
position of objects is not saved (in order to optimize memory usage).

* Adding more features, including lives, levels, and more maps.  Though hardware limitations would have made it very difficult.



##Credits

Used demo code by Eric Freudenthal to help me make the game.
Demo code greatly helped in understanding how to program for the LCD in the MSP430.
Got help from the TA's in the lab

Instructor: Eric Freudenthal

Teacher assistants: Daniel Cervantes, David Pruitt

Author: Gerardo Cervantes


##Bugs

There is glitch where if you press 2 buttons simultaneously, then the pacman can go through fences and appear outside the fences.  Example:  If you want on the top-right corner of the outer fence, then if you press buttons 1 and 4 simultaneously multiple times, then the pacman will go out of the outer fence.  This glitch can make it so you can go anywhere in the map.
