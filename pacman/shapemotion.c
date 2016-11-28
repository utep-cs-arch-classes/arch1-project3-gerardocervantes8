

/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "pacman.h"
#include "sound.h"

#define GREEN_LED BIT6
#define RED_LED BIT0
#define SWITCHES 0x0f  //Switches 1, 2, 3, and 4 in port 2


Vec2 centerPos = {
  {0,0}
};


/*Updates next pos to be new pos, and redraws the layer*/
movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */

  /**Each moving layer updates their pos to posNext*/
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds); /**Gets bounds of layer*/
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



//Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
/**If pixel is over the fence then changes direction of velocity*/
/**Calculates newPos based on the velocity */
/*void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) || //if touches fence
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	int prevVelocity = ml->velocity.axes[axis];
	int velocity = -prevVelocity;
	
	newPos.axes[axis] += (2*velocity);
	ml->velocity.axes[axis] = 0;
      }	//< if outside of fence
      
      
    } // for axis 
    ml->layer->posNext = newPos;
  } //< for ml 
  }*/


void mlAdvance(MovLayer *ml)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    
    ml->layer->posNext = newPos;
  } //< for ml 
}


/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
/**If pixel is over the fence then changes direction of velocity*/
/**Calculates newPos based on the velocity */

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
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) || //if touches fence
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	//int prevVelocity = ml->velocity.axes[axis];
	//int velocity = -prevVelocity;
	
	//newPos.axes[axis] += (2*velocity);
	ml->velocity.axes[axis] = 0;
      }	/**< if outside of fence */
      
      
    } /**< for axis */
    
  } /**< for ml */
}

int regionsIntersect(Region* reg1, Region* reg2){
  vec2Add((&centerPos), (&(reg1->topLeft)), (&(reg1->botRight)) );

  centerPos.axes[0] /= 2;  
  centerPos.axes[1] /= 2;

  if(centerPos.axes[0] >= reg2->topLeft.axes[0] && centerPos.axes[0] <= reg2->botRight.axes[0]){

    if(centerPos.axes[1] <= reg2->botRight.axes[1] && centerPos.axes[1] >= reg2->topLeft.axes[1]){
      return 1;
    }
  }
  return 0;
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
      /*if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) || //if touches fence
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
	//int prevVelocity = ml->velocity.axes[axis];
	//int velocity = -prevVelocity;
	
	//newPos.axes[axis] += (2*velocity);
	
      }	///< if outside of fence
      else{
	ml->velocity.axes[axis] = 0;
      }*/
      if(regionsIntersect(&shapeBoundary, fence)){
	ml->velocity.axes[axis] = 0;
      }
      
    } /**< for axis */
    
  } /**< for ml */
}


u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */
Region obstacleFence0, obstacleFence1, obstacleFence2, obstacleFence3, obstacleFence4; 




void objectCollisions(){
  
  Region pacman;
  abShapeGetBounds((&pacmanLayer0)->abShape, &((&pacmanLayer0)->pos), &pacman);
  vec2Add((&centerPos), (&(pacman.topLeft)), (&(pacman.botRight)) );
  centerPos.axes[0] /= 2;
  centerPos.axes[1] /= 2; //Finds center of pacman TODO turn into a method/function

  
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
      //case 6: pacDotLayer = &pacDotsLayer6; newX = 85; newY = 10; break;
      //case 7: pacDotLayer = &pacDotsLayer7; newX = 95; newY = 10; break;
      //case 8: pacDotLayer = &pacDotsLayer8; newX = 105; newY = 10; break;
      //case 9: pacDotLayer = &pacDotsLayer9; newX = 115; newY = 10; break;
    default: return;
    }
    
    abShapeGetBounds(pacDotLayer->abShape, &(pacDotLayer->pos), &pacDotRegion);
    
    if(regionsIntersectOptimized(&centerPos, &pacDotRegion)){
      pacDotLayer->posNext.axes[0] = newX;
      pacDotLayer->posNext.axes[1] = newY;
      pacDotLayer->pos.axes[0] = newX;
      pacDotLayer->pos.axes[1] = newY;
      
      pacDotsGotten++;
    }
  }
  
}

/**Finds if center of a region1 is inside region2, if so returns true*/
int regionsIntersectOptimized(Vec2* reg1, Region* reg2){
  
  //If X-axis is between region2
  if (centerPos.axes[0] >= reg2->topLeft.axes[0] && centerPos.axes[0] <= reg2->botRight.axes[0]){
    //If Y-axis is between region2
    if(centerPos.axes[1] <= reg2->botRight.axes[1] && centerPos.axes[1] >= reg2->topLeft.axes[1] ){
      return 1;
    }
  }
  return 0;
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

  shapeInit();
  //p2sw_init(1);
  //shapeInit();
  
  
  
  layerInit(&pacmanLayer0);
  layerDraw(&pacmanLayer0);

  
  layerGetBounds(&fieldLayer, &fieldFence);
  layerGetBounds(&obstacleLayer0, &obstacleFence0);
  layerGetBounds(&obstacleLayer1, &obstacleFence1);
  layerGetBounds(&obstacleLayer2, &obstacleFence2);
  layerGetBounds(&obstacleLayer3, &obstacleFence3);
  layerGetBounds(&obstacleLayer4, &obstacleFence4);
  p2sw_init( SWITCHES );

  
  drawString5x7(1,3, "Lives", COLOR_GREEN, COLOR_RED);
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
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count >= 15) { //used to be 15
    //mlAdvance(&ml0, &fieldFence);
    checkFences(&ml0, &fieldFence);
    checkFencesOutside(&ml0, &obstacleFence0);
    checkFencesOutside(&ml0, &obstacleFence1);
    checkFencesOutside(&ml0, &obstacleFence2);
    checkFencesOutside(&ml0, &obstacleFence3);
    checkFencesOutside(&ml0, &obstacleFence4);
    mlAdvance(&ml0);
    objectCollisions();
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

