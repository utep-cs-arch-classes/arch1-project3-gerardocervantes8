

#ifndef enemy_included
#define enemy_included

#include <shape.h>
#include "movingLayer.h"


const AbRect rect10 = {abRectGetBounds, abRectCheck, {6,6}}; /**< 10x10 rectangle */

Layer enemyLayer1 = {		/**< Layer with an orange circle */
  (AbShape *)&rect10,
  {screenWidth/2-30, screenHeight/2 -55}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  0,
};

Layer enemyLayer0 = {		/**< Layer with an orange circle */
  (AbShape *)&rect10,
  {screenWidth-30, screenHeight/2 - 15}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &enemyLayer1,
};

MovLayer movingEnemy1 = { &enemyLayer1, {5,0}, 0}; //ml8
MovLayer movingEnemy0 = { &enemyLayer0, {-5,0}, &movingEnemy1};

void enemyDraw();
void enemyAI(int i);


#endif
