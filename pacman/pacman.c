
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "pacman.h"
#include "buzzer.h"
#include "sound.h"
//#include "enemy.h"

#define GREEN_LED BIT6
#define RED_LED BIT0
#define SWITCHES 0x0f  //Switches 1, 2, 3, and 4 in port 2

/**Keeps track of amount of pacDots player has collected*/
static char pacDotsGotten = 0;

/**Used as a temporary variable to store center of pacmanLayer0
Used to avoid having to call malloc*/
Vec2 centerPos = {
  {0,0}
};


/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shapes which will be check to see if they are touching the fence
 *  \param fence The region which will serve as a boundary for ml
 */
/**If moving layer will be touching the region, turns the moving layer's velocity to 0*/
void checkFences(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] <= fence->topLeft.axes[axis]) || //if touches fence
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	ml->velocity.axes[axis] = 0;
      }	/**< if outside of fence */
      
      
    } /**< for axis */
    
  } /**< for ml */
}


/**Finds if center of a region1 is inside region2, if so returns true*/
int isPlaceInRegion(Vec2* reg1, Region* reg2){
  
  //If X-axis is in region2
  if (centerPos.axes[0] >= reg2->topLeft.axes[0] && centerPos.axes[0] <= reg2->botRight.axes[0]){
    //If Y-axis is in region2
    if(centerPos.axes[1] <= reg2->botRight.axes[1] && centerPos.axes[1] >= reg2->topLeft.axes[1] ){
      return 1;
    }
  }
  return 0;
}

/**Finds if center of region 1 is inside of region2
@Returns 1 if center of reg1 is in region 2, otherwise returns 0*/
int regionsIntersect(Region* reg1, Region* reg2){
  
  vec2Add((&centerPos), (&(reg1->topLeft)), (&(reg1->botRight)) );
  centerPos.axes[0] /= 2;  
  centerPos.axes[1] /= 2;/**Finds center of region 1*/
  
  return isPlaceInRegion(&centerPos, reg2);
}

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
/**If pixel is over the fence then changes direction of velocity*/
/**Calculates newPos based on the velocity */

/**If moving layer will be touching the region, turns the moving layer's velocity to 0*/
void checkFencesOutside(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if(regionsIntersect(&shapeBoundary, fence)){
	ml->velocity.axes[axis] = 0;
      }
      
    } /**< for axis */
    
  } /**< for ml */
}


u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

static Region fieldFence;		/**< fence around playing field  */

//Obstacle fences initalized in drawAllLayers();
static Region obstacleFence0, obstacleFence1, obstacleFence2, obstacleFence3, obstacleFence4; 


/**Writes the score of the game on top of the screen
@PARAM pacDots is how many pacdots the player has collected*/
void updatePacDotText(const int pacDots){
  short width = screenWidth/3+20;
  short height = 3;
  int color = COLOR_RED;
  int BGcolor = COLOR_GREEN;
  
  char text[] = "0"; 
  switch(pacDots){
  case 0: text[0] = '0'; break;
  case 1: text[0] = '1'; break;
  case 2: text[0] = '2'; break;
  case 3: text[0] = '3'; break;
  case 4: text[0] = '4'; break;
  case 5: text[0] = '5'; break;
  case 6: text[0] = '6'; break;
  default: text[0] = '7'; break;
  }
  
  drawString5x7(width, height, text, color, BGcolor);
}

/**Called when the game has ended, turns off cpu and interrupts and displays a message with the outcome
@PARAM state, used to determine the outcome of the game.
state == 1 implies player lost
state == 2 implies player won*/
void gameEnds(const int state){

  if(state != 1 && state != 2){
    return;
  }
  clearScreen(COLOR_GREEN);
  sound_stop();
  if(state == 1){
    drawString5x7((screenWidth/2) -15, screenHeight/2-5, "LOSER", COLOR_CYAN, COLOR_RED);
  }
  else{
    drawString5x7((screenWidth/2) -15, screenHeight/2-5, "WINNER", COLOR_CYAN, COLOR_RED);
  }
  
  
  or_sr(0x10); //CPU OFF
  and_sr(~8); //Interrupts off
}


/**Finds what the center position of pacman is Vec2. 
 Stores result into centerPos structure*/
void _pacmanCenterPos(){
  Region pacman;
  abShapeGetBounds((&pacmanLayer0)->abShape, &((&pacmanLayer0)->pos), &pacman);
  vec2Add((&centerPos), (&(pacman.topLeft)), (&(pacman.botRight)) );
  centerPos.axes[0] /= 2;
  centerPos.axes[1] /= 2; //Finds center of pacman TODO turn into a method/function

}

/**Finds if player touches a pacdot.
Moves the pacdots location if it was collected and increases pacdot counter
Plays sound if player got a pacdot and updates score*/
void objectCollisions(){
  
  _pacmanCenterPos();
  
  Region pacDotRegion;
  Layer* pacDotLayer;
  int pacDot, newX, newY;

  //Finds if any of the pacdots collide with pacman
  for(pacDot = 0; pacDot < 6; pacDot++){
    switch(pacDot){
    case 0: pacDotLayer = &pacDotsLayer0; newX = 75; newY = 3; break;
    case 1: pacDotLayer = &pacDotsLayer1; newX = 85; newY = 3; break;
    case 2: pacDotLayer = &pacDotsLayer2; newX = 95; newY = 3; break;
    case 3: pacDotLayer = &pacDotsLayer3; newX = 105; newY = 3; break;
    case 4: pacDotLayer = &pacDotsLayer4; newX = 115; newY = 3; break;
    case 5: pacDotLayer = &pacDotsLayer5; newX = 75; newY = 10; break;
    default: return;
    }
    
    abShapeGetBounds(pacDotLayer->abShape, &(pacDotLayer->pos), &pacDotRegion);
    
    if(isPlaceInRegion(&centerPos, &pacDotRegion)){
      pacDotLayer->posNext.axes[0] = newX;
      pacDotLayer->posNext.axes[1] = newY;
      pacDotLayer->pos.axes[0] = newX;
      pacDotLayer->pos.axes[1] = newY;
      
      pacDotsGotten++;
      updatePacDotText(pacDotsGotten);
      sound_start(3);
    }
  }

  if(pacDotsGotten == 6){ /**Player won!*/
    gameEnds(2);
  }
  
  
}

/**Finds if pacman collides with any of the enemies
   Ends game and tells player they lost if they touched the enemy*/
void enemyCollision(){

  _pacmanCenterPos();
  
  Region enemyRegion;
  Layer* enemyLayer;
  int enemy;
  for(enemy = 0; enemy < 2; enemy++){
    switch(enemy){
    case 0: enemyLayer = &enemyLayer0; break;
    case 1: enemyLayer = &enemyLayer1; break;
    }
    abShapeGetBounds(enemyLayer->abShape, &(enemyLayer->pos), &enemyRegion);
  
    if( isPlaceInRegion(&centerPos, &enemyRegion) ){
      gameEnds(1);
    }
  }
 
}

/**Increases the region by given amount of pixels in all directions
@param region you want to increase size of
@param pixels is the amount of pixels you want to increase region by*/
void increaseRegion(Region* region, const int pixels){
  
  region->topLeft.axes[0] = region->topLeft.axes[0] - pixels;
  region->topLeft.axes[1] = region->topLeft.axes[1] - pixels;
  region->botRight.axes[0] = region->botRight.axes[0] + pixels;
  region->botRight.axes[1] = region->botRight.axes[1] + pixels;
  
}

/**Contains structures of obstacle fences, obstacle fences are only drawn once, which is why they are drawn
in a method and not saved globally.  Regions for the obstacle fences are initalized, and all the layers
are drawn when the method is called.*/
void drawAllLayers(){
  
  Layer obstacleLayer4 = {		//top-right 
    (AbShape *) &obstacleOutline,
    {((screenWidth/4)*3)-7, ((screenHeight/4))+6},//< center 
    {0,0}, {0,0},				    // last & next pos 
    COLOR_BLUE,
    &pacmanLayer0 
  };
  
  Layer obstacleLayer3 = {                 //bottom-left
    (AbShape *) &obstacleOutline,
    {((screenWidth/4)+3), ((screenHeight/4)*3)+6},//< center 
    {0,0}, {0,0},				    // last & next pos 
    COLOR_BLUE,
    &obstacleLayer4
  };
  
  
  Layer obstacleLayer2 = {		//center
    (AbShape *) &obstacleOutline,
    {(screenWidth/2)-1, (screenHeight/2)+6},//< center 
    {0,0}, {0,0},				    // last & next pos 
    COLOR_BLUE,
    &obstacleLayer3
  };
  
  
  Layer obstacleLayer1 = {		// bottom-right
    (AbShape *) &obstacleOutline,
    {((screenWidth/4)*3)-3, ((screenHeight/4)*3)+6},//< center 
    {0,0}, {0,0},				    // last & next pos 
    COLOR_BLUE,
    &obstacleLayer2
  };
  
  
  Layer obstacleLayer0 = {		 //top-left
    (AbShape *) &obstacleOutline,
    {screenWidth/4, (screenHeight/4)+6},//< center 
    {0,0}, {0,0},				    // last & next pos 
    COLOR_BLUE,
    &obstacleLayer1 
  };
  
  
  layerInit(&obstacleLayer0);
  layerDraw(&obstacleLayer0);
  
  layerGetBounds(&obstacleLayer0, &obstacleFence0);
  layerGetBounds(&obstacleLayer1, &obstacleFence1);
  layerGetBounds(&obstacleLayer2, &obstacleFence2);
  layerGetBounds(&obstacleLayer3, &obstacleFence3);
  layerGetBounds(&obstacleLayer4, &obstacleFence4); 

  increaseRegion(&obstacleFence0, 6);
  increaseRegion(&obstacleFence1, 6);
  increaseRegion(&obstacleFence2, 6);
  increaseRegion(&obstacleFence3, 6);
  increaseRegion(&obstacleFence4, 6);
  
}

/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;
  
  configureClocks();

  lcd_init();

  buzzer_init();
  shapeInit();

  drawAllLayers();
  layerGetBounds(&fieldLayer, &fieldFence);
  
  p2sw_init( SWITCHES );
  
  drawString5x7( 7, 4, "PACMAN", COLOR_GREEN, COLOR_RED);
  updatePacDotText(pacDotsGotten);
    
  enableWDTInterrupts();      /**< enable periodic interrupt */

  or_sr(0x8);	              /**< GIE (enable interrupts) */

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;

    movLayerDraw(&ml0, &pacmanLayer0);
    
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static char count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;

  if (count >= 16) { //used to be 15
    checkFences(&ml0, &fieldFence);
    //Finds if any moving  will touch obstacle fence next position, if they will, set that objects velocity to 0
    checkFencesOutside(&ml0, &obstacleFence0); 
    checkFencesOutside(&ml0, &obstacleFence1);
    checkFencesOutside(&ml0, &obstacleFence2);
    checkFencesOutside(&ml0, &obstacleFence3);
    checkFencesOutside(&ml0, &obstacleFence4);
    mlAdvance(&ml0); /**Advances all moving layers*/
    objectCollisions(); /**Finds for collisions with pacdots or enemies*/
    enemyCollision();
    sound_update(0); //Updates sound
    enemyAI(1); //Updates AI's velocity if their velocity is 0 from touching a wall
    enemyAI(2);
    
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

/*Changes velocityX and velocityY to match the given direction
direction == 1 implies up direction
direction == 2 implies down direction
direction == 3 implies right direction
direction == 4 implies left direction
*/
void changeVelocity(int* velocityX, int* velocityY, int direction){
  switch( direction ){
  case 1: (*velocityX) = 0; (*velocityY) = -3; break; //pacman moving up
  case 2: (*velocityX) = 0; (*velocityY) = 3; break; //pacman moving down
  case 3: (*velocityX) = -3; (*velocityY) = 0; break; //pacman moving right
  case 4: (*velocityX) = 3; (*velocityY) = 0; break; //pacman moving left
  default: (*velocityX) = 5; (*velocityY) = 5;
  }
}

/**TODO change to 2 methods, one for detecting which input was pressed, the other for changing velocity pacman is moving in*/
/**Changes direction pacman is moving based off of what button was pressed*/
void updatePacmanMovement(){
  
  MovLayer* pacmanML = &ml0; //ml0 is pacman's MovLayer
  Layer* pacmanLayer = &pacmanLayer0; 
  Vec2 newPos;
  int velocityX, velocityY;
  
  vec2Add(&newPos, &(pacmanLayer->posNext), &(pacmanML->velocity));
  
  int direction = 0;

  /**Determines what direction pacman will go to*/
  switch( (P2IFG & (SWITCHES) ) ){

  case BIT0: direction = 1; break; //Button 1 pressed (sw2)
  case BIT1: direction = 2; break; //Button 2 pressed (sw2)
  case BIT2: direction = 3; break; //Button 3 pressed (sw2)
  case BIT3: direction = 4; break; //Button 4 pressed (sw2)
  default: return;
  }

  changeVelocity( (&velocityX), (&velocityY), direction); //Updates what the new velocity will be based on direction

  pacmanML->velocity.axes[0] = velocityX; //Updates velocity of Object
 
  pacmanML->velocity.axes[1] = velocityY; //Updates velocity of Object

  pacmanLayer->posNext = newPos;
  
  P2IFG = 0;
}

__interrupt(PORT2_VECTOR) Port_2(){
  updatePacmanMovement();
}

