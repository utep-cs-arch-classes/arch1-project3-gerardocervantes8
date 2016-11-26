

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

#define GREEN_LED BIT6
#define RED_LED BIT0


AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */
AbRArrow rightArrow = {abRArrowGetBounds, abRArrowCheck, 30};

static int pacDotsGotten = 0;

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 4, screenHeight/2 - 8}
};

Layer pacDotsLayer3 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {(screenWidth/2)+45, (screenHeight/2)+25}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  0,
};

Layer pacDotsLayer2 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {((screenWidth/3)*2), ((screenHeight/4)*3)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer3,
};

Layer pacDotsLayer1 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {(screenWidth/3), (screenHeight/4)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer2,
};
Layer pacDotsLayer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {55, 65}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer1,
};

Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2-1, (screenHeight/2)+4},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &pacDotsLayer0 //previously &layer3
};

Layer pacmanLayer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle7,
  {(screenWidth/2)+15, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &fieldLayer, //previously &layer1,
};


/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
MovLayer ml1 = { &pacDotsLayer0, {0,0}, 0}; //moving layyer for pacman dot
MovLayer ml0 = { &pacmanLayer0, {0,0},&ml1}; //Pacman layer movement


/**< not all layers move */ //USED TO BE &layer3 //CHANGED NAME FROM m13 to m10


/**movLayers move future position of layers, and layers (probes the layers?)

Updates next pos to be new pos, and redraws the layer*/
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
  Region pacDot0;
  abShapeGetBounds((&pacDotsLayer0)->abShape, &((&pacDotsLayer0)->pos), &pacDot0);
  Region pacDot1;
  abShapeGetBounds((&pacDotsLayer1)->abShape, &((&pacDotsLayer1)->pos), &pacDot1);
  Region pacDot2;
  abShapeGetBounds((&pacDotsLayer2)->abShape, &((&pacDotsLayer2)->pos), &pacDot2);
  Region pacDot3;
  abShapeGetBounds((&pacDotsLayer3)->abShape, &((&pacDotsLayer3)->pos), &pacDot3);
  Region pacman;
  abShapeGetBounds((&pacmanLayer0)->abShape, &((&pacmanLayer0)->pos), &pacman);
  
  if(regionsIntersect(&pacDot0, &pacman)){
    //(&pacDotsLayer0)->posNext.axes[0] = (&pacDotsLayer0)->pos.axes[0] + 5;
    //(&pacDotsLayer0)->posNext.axes[1] = (&pacDotsLayer0)->pos.axes[1] + 5;


    (&pacDotsLayer0)->posNext.axes[0] = 50;
    (&pacDotsLayer0)->posNext.axes[1] = 4;
    pacDotsGotten++;
    
    //Vec2* breakProg;
    //vec2Add(breakProg, &centerPos, &centerPos);
  }
  
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
  if (count == 15) {
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
  }
}

/**TODO change to 2 methods, one for detecting which input was pressed, the other for changing velocity pacman is moving in*/
/**Changes direction pacman is moving based off of what button was pressed*/
void updatePacmanMovement(){
  
  MovLayer* m1 = (&ml0);
  Vec2 newPos;
  int velocityX;
  int velocityY;
  Region shapeBoundary;
  
  vec2Add(&newPos, &(m1->layer->posNext), &(m1->velocity));
  
  abShapeGetBounds(m1->layer->abShape, &newPos, &shapeBoundary);
  
  int direction = 0;
  switch( (P2IFG & (BIT0 | BIT1 | BIT2 | BIT3) ) ){

  case BIT0: direction = 1; break; //Button 1 pressed (sw2)
  case BIT1: direction = 2; break; //Button 2 pressed (sw2)
  case BIT2: direction = 3; break; //Button 3 pressed (sw2)
  case BIT3: direction = 4; break; //Button 4 pressed (sw2)
  }

  changeVelocity( (&velocityX), (&velocityY), direction); /**Updates what the new velocity will be based on direction*/

  m1->velocity.axes[0] = velocityX; //Updates velocity of Object
 
  m1->velocity.axes[1] = velocityY; //Updates velocity of Object

  m1->layer->posNext = newPos;
  
  P2IFG = 0;
}

__interrupt(PORT2_VECTOR) Port_2(){
  //  updateVelocityUpdate();
  updatePacmanMovement();
}

