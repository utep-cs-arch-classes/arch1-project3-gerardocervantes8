

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

#define GREEN_LED BIT6
#define RED_LED BIT0



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
void mlAdvance(MovLayer *ml, Region *fence)
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
      }	/**< if outside of fence */
      
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}


u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */

Vec2 centerPos = {
  {0,0}
};



void objectCollisions(){
  /*Region pacDot1;
  abShapeGetBounds((&pacDotsLayer1)->abShape, &((&pacDotsLayer1)->pos), &pacDot1);
  Region pacDot2;
  abShapeGetBounds((&pacDotsLayer2)->abShape, &((&pacDotsLayer2)->pos), &pacDot2);
  Region pacDot3;
  abShapeGetBounds((&pacDotsLayer3)->abShape, &((&pacDotsLayer3)->pos), &pacDot3);
  Region pacDot4;
  abShapeGetBounds((&pacDotsLayer4)->abShape, &((&pacDotsLayer4)->pos), &pacDot4);*/
  /*Region pacDot5;
  abShapeGetBounds((&pacDotsLayer5)->abShape, &((&pacDotsLayer5)->pos), &pacDot5);
  Region pacDot6;
  abShapeGetBounds((&pacDotsLayer6)->abShape, &((&pacDotsLayer6)->pos), &pacDot6);
  Region pacDot7;
  abShapeGetBounds((&pacDotsLayer7)->abShape, &((&pacDotsLayer7)->pos), &pacDot7);
  Region pacDot8;
  abShapeGetBounds((&pacDotsLayer8)->abShape, &((&pacDotsLayer8)->pos), &pacDot8);
  Region pacDot9;
  abShapeGetBounds((&pacDotsLayer9)->abShape, &((&pacDotsLayer9)->pos), &pacDot9);*/
  
  Region pacman;
  abShapeGetBounds((&pacmanLayer0)->abShape, &((&pacmanLayer0)->pos), &pacman);
 
  vec2Add((&centerPos), (&(pacman.topLeft)), (&(pacman.botRight)) );
  centerPos.axes[0] /= 2;
  centerPos.axes[1] /= 2;

  
  Region pacDotRegion;

  int pacDot = 0;
  Layer* pacDotLayer;
  int newX;
  int newY;


  for(pacDot = 0; pacDot < 10; pacDot++){
    switch(pacDot){
    case 0: pacDotLayer = &pacDotsLayer0; newX = 75; newY = 3; break;
    case 1: pacDotLayer = &pacDotsLayer1; newX = 85; newY = 3; break;
    case 2: pacDotLayer = &pacDotsLayer2; newX = 95; newY = 3; break;
    case 3: pacDotLayer = &pacDotsLayer3; newX = 105; newY = 3; break;
    case 4: pacDotLayer = &pacDotsLayer4; newX = 115; newY = 3; break;
    case 5: pacDotLayer = &pacDotsLayer5; newX = 75; newY = 10; break;
    case 6: pacDotLayer = &pacDotsLayer6; newX = 85; newY = 10; break;
    case 7: pacDotLayer = &pacDotsLayer7; newX = 95; newY = 10; break;
    case 8: pacDotLayer = &pacDotsLayer8; newX = 105; newY = 10; break;
    case 9: pacDotLayer = &pacDotsLayer9; newX = 115; newY = 10; break;
    default: break;
      
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
  


  /*
  abShapeGetBounds((&pacDotsLayer0)->abShape, &((&pacDotsLayer0)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer0)->posNext.axes[0] = 75;
    (&pacDotsLayer0)->posNext.axes[1] = 3;
    (&pacDotsLayer0)->pos.axes[0] = 75;
    (&pacDotsLayer0)->pos.axes[1] = 3;
    
    //    (&ml1)->layer->posNext.axes[0] = 75;  //pacDot0
    //(&ml1)->layer->posNext.axes[1] = 3;
    //(&ml1)->layer->pos.axes[0] = 75;  //pacDot0
    //(&ml1)->layer->pos.axes[1] = 3;
    pacDotsGotten++;
  }


  
  abShapeGetBounds((&pacDotsLayer1)->abShape, &((&pacDotsLayer1)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer1)->posNext.axes[0] = 85;
    (&pacDotsLayer1)->posNext.axes[1] = 3;
    (&pacDotsLayer1)->pos.axes[0] = 85;
    (&pacDotsLayer1)->pos.axes[1] = 3;

    // (&ml2)->layer->posNext.axes[0] = 85;  //pacDot1
    //(&ml2)->layer->posNext.axes[1] = 3;
    //(&ml2)->layer->pos.axes[0] = 85;  //pacDot1
    //(&ml2)->layer->pos.axes[1] = 3;
    
    pacDotsGotten++;
  }

  abShapeGetBounds((&pacDotsLayer2)->abShape, &((&pacDotsLayer2)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer2)->posNext.axes[0] = 95;
    (&pacDotsLayer2)->posNext.axes[1] = 3;
    (&pacDotsLayer2)->pos.axes[0] = 95;
    (&pacDotsLayer2)->pos.axes[1] = 3;
    
        (&ml3)->layer->posNext.axes[0] = 95;  //pacDot2
    (&ml3)->layer->posNext.axes[1] = 3;
    (&ml3)->layer->pos.axes[0] = 95;  //pacDot2
    (&ml3)->layer->pos.axes[1] = 3;
    
    pacDotsGotten++;
  }


  abShapeGetBounds((&pacDotsLayer3)->abShape, &((&pacDotsLayer3)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer3)->posNext.axes[0] = 105;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    (&pacDotsLayer3)->pos.axes[0] = 105;
    (&pacDotsLayer3)->pos.axes[1] = 3;

    
       (&ml4)->layer->posNext.axes[0] = 105;  //pacDot3
    (&ml4)->layer->posNext.axes[1] = 3;
    (&ml4)->layer->pos.axes[0] = 105;  //pacDot3
    (&ml4)->layer->pos.axes[1] = 3;
    
    pacDotsGotten++;
  }


  abShapeGetBounds((&pacDotsLayer4)->abShape, &((&pacDotsLayer4)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer4)->posNext.axes[0] = 115;
    (&pacDotsLayer4)->posNext.axes[1] = 3;
    (&pacDotsLayer4)->pos.axes[0] = 115;
    (&pacDotsLayer4)->pos.axes[1] = 3;

    
    (&ml5)->layer->posNext.axes[0] = 115;  //pacDot4
    (&ml5)->layer->posNext.axes[1] = 3;
    (&ml5)->layer->pos.axes[0] = 115;  //pacDot4
    (&ml5)->layer->pos.axes[1] = 3;
    
    pacDotsGotten++;
  }
  abShapeGetBounds((&pacDotsLayer5)->abShape, &((&pacDotsLayer5)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer5)->posNext.axes[0] = 115;
    (&pacDotsLayer5)->posNext.axes[1] = 10;
    (&pacDotsLayer5)->pos.axes[0] = 115;
    (&pacDotsLayer5)->pos.axes[1] = 10;

    
    (&ml6)->layer->posNext.axes[0] = 115;  //pacDot5
    (&ml6)->layer->posNext.axes[1] = 10;
    (&ml6)->layer->pos.axes[0] = 115;  //pacDot5
    (&ml6)->layer->pos.axes[1] = 10;
    
    pacDotsGotten++;
  }
  abShapeGetBounds((&pacDotsLayer6)->abShape, &((&pacDotsLayer6)->pos), &pacDot);
  
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer6)->posNext.axes[0] = 105;
    (&pacDotsLayer6)->posNext.axes[1] = 10;
    (&pacDotsLayer6)->pos.axes[0] = 105;
    (&pacDotsLayer6)->pos.axes[1] = 10;

    
    (&ml7)->layer->posNext.axes[0] = 105;  //pacDot6
    (&ml7)->layer->posNext.axes[1] = 10;
    (&ml7)->layer->pos.axes[0] = 105;  //pacDot6
    (&ml7)->layer->pos.axes[1] = 10;
    
    pacDotsGotten++;
  }

  
  abShapeGetBounds((&pacDotsLayer7)->abShape, &((&pacDotsLayer7)->pos), &pacDot);
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer7)->posNext.axes[0] = 95;
    (&pacDotsLayer7)->posNext.axes[1] = 10;
    (&pacDotsLayer7)->pos.axes[0] = 95;
    (&pacDotsLayer7)->pos.axes[1] = 10;

    
    (&ml8)->layer->posNext.axes[0] = 95;  //pacDot7
    (&ml8)->layer->posNext.axes[1] = 10;
    (&ml8)->layer->pos.axes[0] = 95;  //pacDot7
    (&ml8)->layer->pos.axes[1] = 10;
    
    pacDotsGotten++;
  }
  
  abShapeGetBounds((&pacDotsLayer8)->abShape, &((&pacDotsLayer8)->pos), &pacDot);
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer8)->posNext.axes[0] = 85;
    (&pacDotsLayer8)->posNext.axes[1] = 10;
    (&pacDotsLayer8)->pos.axes[0] = 85;
    (&pacDotsLayer8)->pos.axes[1] = 10;

    
        (&ml9)->layer->posNext.axes[0] = 85;  //pacDot8
    (&ml9)->layer->posNext.axes[1] = 10;
    (&ml9)->layer->pos.axes[0] = 85;  //pacDot8
    (&ml9)->layer->pos.axes[1] = 10;
    
    pacDotsGotten++;
  }

  abShapeGetBounds((&pacDotsLayer9)->abShape, &((&pacDotsLayer9)->pos), &pacDot);
  if(regionsIntersectOptimized(&centerPos, &pacDot)){
    (&pacDotsLayer9)->posNext.axes[0] = 75;
    (&pacDotsLayer9)->posNext.axes[1] = 10;
    (&pacDotsLayer9)->pos.axes[0] = 75;
    (&pacDotsLayer9)->pos.axes[1] = 10;

    
    (&ml10)->layer->posNext.axes[0] = 75;  //pacDot8
    (&ml10)->layer->posNext.axes[1] = 10;
    (&ml10)->layer->pos.axes[0] = 75;  //pacDot8
    (&ml10)->layer->pos.axes[1] = 10;
    
    pacDotsGotten++;
  }*/

  
  /*  if(regionsIntersect(&pacDot0, &pacman)){
    //(&pacDotsLayer0)->posNext.axes[0] = (&pacDotsLayer0)->pos.axes[0] + 5;
    //(&pacDotsLayer0)->posNext.axes[1] = (&pacDotsLayer0)->pos.axes[1] + 5;


    (&pacDotsLayer0)->posNext.axes[0] = 85;
    (&pacDotsLayer0)->posNext.axes[1] = 3;
    pacDotsGotten++;
    
    //Vec2* breakProg;
    //vec2Add(breakProg, &centerPos, &centerPos);
  }
  else if(regionsIntersect(&pacDot1, &pacman)){
    (&pacDotsLayer1)->posNext.axes[0] = 95;
    (&pacDotsLayer1)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }
  else if(regionsIntersect(&pacDot2, &pacman)){
    (&pacDotsLayer2)->posNext.axes[0] = 105;
    (&pacDotsLayer2)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }
  else if(regionsIntersect(&pacDot3, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 115;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }  
  else if(regionsIntersect(&pacDot4, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 120;
    (&pacDotsLayer3)->posNext.axes[1] = 5;
    pacDotsGotten++;
    } */ 
  /*  else if(regionsIntersect(&pacDot5, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 115;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }  
  else if(regionsIntersect(&pacDot6, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 115;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }  
  else if(regionsIntersect(&pacDot7, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 115;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }  
  else if(regionsIntersect(&pacDot8, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 115;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    pacDotsGotten++;
  }  
  else if(regionsIntersect(&pacDot9, &pacman)){
    (&pacDotsLayer3)->posNext.axes[0] = 115;
    (&pacDotsLayer3)->posNext.axes[1] = 3;
    pacDotsGotten++;
  } 

  */ 
}
/**Finds if center of a region1 is inside region2, if so returns true*/
int regionsIntersect(Region* reg1, Region* reg2){
  
  //Finds center of region1
  //Vec2* centerPos = (Vec2*) malloc(sizeof(reg1->topLeft));
  vec2Add((&centerPos), (&(reg1->topLeft)), (&(reg1->botRight)) );

  //  reg2->topLeft.axes[0] += reg2->botRight.axes[0];
  //reg2->topLeft.axes[1] += reg2->botRight.axes[1];
  
  centerPos.axes[0] /= 2;
  centerPos.axes[1] /= 2;

    //If X-axis is between region2
  if (centerPos.axes[0] >= reg2->topLeft.axes[0] && centerPos.axes[0] <= reg2->botRight.axes[0]){
    //Vec2* breakProg;
    //vec2Add(breakProg, &centerPos, &centerPos);
    //If Y-axis is between region2
    if(centerPos.axes[1] <= reg2->botRight.axes[1] && centerPos.axes[1] >= reg2->topLeft.axes[1] ){
      return 1;
    }
  }
  return 0;
}
/**Finds if center of a region1 is inside region2, if so returns true*/
int regionsIntersectOptimized(Vec2* reg1, Region* reg2){
  
  //Finds center of region1
  //Vec2* centerPos = (Vec2*) malloc(sizeof(reg1->topLeft));
  //vec2Add((&centerPos), (&(reg1->topLeft)), (&(reg1->botRight)) );

  //  reg2->topLeft.axes[0] += reg2->botRight.axes[0];
  //reg2->topLeft.axes[1] += reg2->botRight.axes[1];
  
    //If X-axis is between region2
  if (centerPos.axes[0] >= reg2->topLeft.axes[0] && centerPos.axes[0] <= reg2->botRight.axes[0]){
    //Vec2* breakProg;
    //vec2Add(breakProg, &centerPos, &centerPos);
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
  p2sw_init( (BIT3 | BIT2 | BIT1 | BIT0) );

  
    drawString5x7(1,3, "Lives", COLOR_GREEN, COLOR_RED);
  //drawPacmanFont((1,3, "Livesabcde ", COLOR_GREEN, COLOR_RED));
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
  if (count == 15) { //used to be 15
    mlAdvance(&ml0, &fieldFence);
    objectCollisions();
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

/**Changes velocityX and velocityY to match the given direction
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
  default: (*velocityX) = 5; (*velocityY) = 5; return;
  }
}

/**TODO change to 2 methods, one for detecting which input was pressed, the other for changing velocity pacman is moving in*/
/**Changes direction pacman is moving based off of what button was pressed*/
void updatePacmanMovement(){
  
  MovLayer* m1 = (&ml0); //ml0 is pacman's MovLayer
  Vec2 newPos;
  int velocityX;
  int velocityY;
  Region shapeBoundary; //delete?
  
  vec2Add(&newPos, &(m1->layer->posNext), &(m1->velocity));
  
  abShapeGetBounds(m1->layer->abShape, &newPos, &shapeBoundary); //delete?
  
  int direction = 0;
  switch( (P2IFG & (BIT0 | BIT1 | BIT2 | BIT3) ) ){

  case BIT0: direction = 1; break; //Button 1 pressed (sw2)
  case BIT1: direction = 2; break; //Button 2 pressed (sw2)
  case BIT2: direction = 3; break; //Button 3 pressed (sw2)
  case BIT3: direction = 4; break; //Button 4 pressed (sw2)
  default: return;
  }

  changeVelocity( (&velocityX), (&velocityY), direction); /**Updates what the new velocity will be based on direction*/

  m1->velocity.axes[0] = velocityX; //Updates velocity of Object
 
  m1->velocity.axes[1] = velocityY; //Updates velocity of Object

  m1->layer->posNext = newPos;
  
  P2IFG = 0;
}

__interrupt(PORT2_VECTOR) Port_2(){
  updatePacmanMovement();
}

