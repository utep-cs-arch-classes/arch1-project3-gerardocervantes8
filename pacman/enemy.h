

#ifndef enemy_included
#define enemy_included

#include <shape.h>
#include "movingLayer.h"


/**Shape that the enemies will be*/
const AbRect rect10 = {abRectGetBounds, abRectCheck, {6,6}}; /**< 6x6 rectangle */

Layer enemyLayer1 = {	  //Layer with second enemy
  (AbShape *)&rect10,
  {screenWidth/2-30, screenHeight/2 -55}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  0,
};

Layer enemyLayer0 = {	//Layer with first enemy
  (AbShape *)&rect10,
  {screenWidth-30, screenHeight/2 - 15}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &enemyLayer1,
};

MovLayer movingEnemy1 = { &enemyLayer1, {5,0}, 0};  //Moving layer with second enemy
MovLayer movingEnemy0 = { &enemyLayer0, {-5,0}, &movingEnemy1}; //Moving layer with first enemy

void enemyDraw();
void enemyAI(int i);


#endif
