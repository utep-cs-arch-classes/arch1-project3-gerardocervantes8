

#include <shape.h>
#include "enemy.h"


/**Enemies always have the same movement, 
if you restart the game, enemies will always move the same directions (no randomness),
after enough time, it becomes difficult to know what they will do*/

void enemyDraw(){
   layerInit(&enemyLayer0);
   layerDraw(&enemyLayer0);
}

/**Finds if any enemies are currently not moving (velocity is 0),
if so changes the direction of the enemy. 
@Parameter is the enemy you want to check.*/
void enemyAI(int i){

  MovLayer* enemyMovLayer;

  //Finds enemy indicated by param
  switch(i){
  case 1: enemyMovLayer = &movingEnemy0; break; 
  case 2: enemyMovLayer = &movingEnemy1; break;
  default: return;
  }
  
  static char enemyState = 0;
  Vec2 enemySpeed =  enemyMovLayer->velocity;
  int x, y;
    
  /**If enemy is not moving, then changes it's velocity*/
  if( enemySpeed.axes[0] == 0 && enemySpeed.axes[1] == 0){
    switch(enemyState){
    case 0: x = 5; y = -5; break; //Goes Bottom-right
    case 1: x = 0; y = 6; break; //Goes up
    case 2: x = -5; y = 0; break; //Goes left
    case 3: x = 5; y = 0; break; //Goes right
    case 4: x = -5; y = 4; break; //Goes goes top-left
    case 5: x = 0; y = -4; break; //Goes Down 
    case 6: x = 5; y = 0; break; //Goes right
    case 7: x = 0; y = 6; break; //Goes up
    case 8: x = -5; y = 0; break; //Goes left
    case 9: x = -5; y = -4; break; //Goes goes bottom-left
    case 10: x = 5; y = 0; break; //Goes right
    case 11: x = 5; y = 7; break; //Goes top-right
    case 12: x = -5; y = 0; break; //Goes left
    case 13: x = 0; y = -4; break; //Goes Down 
    case 14: x = 0; y = 6; enemyState = -1; break; //Goes up
    }
    enemyMovLayer->velocity.axes[0] = x;
    enemyMovLayer->velocity.axes[1] = y;
    
    enemyState++;
  }

}
