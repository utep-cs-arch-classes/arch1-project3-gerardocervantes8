

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

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 2, screenHeight/2 - 8}
};
Layer pacDotsLayer2 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {((screenWidth/3)*2), ((screenHeight/4)*3)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  0,
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

/*Layer layer4 = {
  (AbShape *)&rightArrow,
  {(screenWidth/2)+10, (screenHeight/2)+5}, // bit below & right of center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_PINK,
  &pacDotsLayer0
  };*/

/*Layer layer3 = {		//< Layer with an orange circle 
  (AbShape *)&circle8,
  {(screenWidth/2)+10, (screenHeight/2)+5}, //< bit below & right of center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_VIOLET,
  0, //Used to be &layer4
};*/


Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, (screenHeight/2)+4},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &pacDotsLayer0 //previously &layer3
};

/*Layer layer1 = {		// Layer with a red square 
  (AbShape *)&rect10,
  {screenWidth/2, screenHeight/2}, //< center 
  {0,0}, {0,0},				   // last & next pos 
  COLOR_RED,
  &fieldLayer,
  };*/

Layer pacmanLayer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle7,
  {(screenWidth/2)+45, (screenHeight/2)+25}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &fieldLayer, //previously &layer1,
};

Layer layer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {(screenWidth/2)+45, (screenHeight/2)+25}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacmanLayer0, 
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

MovLayer ml0 = { &pacmanLayer0, {1,1}, 0 }; // Arrow

/**< not all layers move */ //USED TO BE &layer3 //CHANGED NAME FROM m13 to m10

//MovLayer ml1 = { &layer1, {1,2}, &ml3 }; 
//MovLayer ml0 = { &layer0, {2,1}, &ml1 }; 


/**movLayers move future position of layers, and layers (probes the layers?)*/
movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
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
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (2*velocity);
      }	/**< if outside of fence */
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}


u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */


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


  
  layerInit(&layer0);
  layerDraw(&layer0);
 

  layerGetBounds(&fieldLayer, &fieldFence);
  p2sw_init( (BIT3 | BIT2 | BIT1 | BIT0) );

  drawString5x7(10,3, "hello", COLOR_GREEN, COLOR_RED);

  enableWDTInterrupts();      /**< enable periodic interrupt */

  
  or_sr(0x8);	              /**< GIE (enable interrupts) */


  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
  
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
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

__interrupt(PORT2_VECTOR) Port_2(){

  MovLayer* m1 = (&ml0);
  Vec2 newPos;
  int velocityX;
  int velocityY;
  Region shapeBoundary;
  
  vec2Add(&newPos, &(m1->layer->posNext), &(m1->velocity));  
  abShapeGetBounds(m1->layer->abShape, &newPos, &shapeBoundary);
   
  if(P2IFG & BIT0 == BIT0){
    /*    velocityX = m1->velocity.axes[0]+1;
	  velocityY = m1->velocity.axes[1]+1;     */
    velocityX = 0; /**Moves pacman velocity up*/
    velocityY = -1;
  }
  else if(P2IFG & BIT1 == BIT1){
    velocityX = 0; /**Moves pacman velocity down*/
    velocityY = 1;

  }
  else if(P2IFG & BIT2 == BIT2){
    velocityX = -1; /**Moves pacman velocity to left*/
    velocityY =  0;

  }
  else if(P2IFG & BIT3 == BIT3){
    velocityX = 1; /**Moves pacman velocity to right*/
    velocityY = 0;
    
  }
  else{ /**Unkown button press*/
    P2IFG = 0;
    return;
  }

  m1->velocity.axes[0] = velocityX;
  newPos.axes[0] += (2*velocityX);


  m1->velocity.axes[1] = velocityY;
  newPos.axes[1] += (2*velocityY);




  /*  velocity = m1->velocity.axes[0] = -m1->velocity.axes[0];
  newPos.axes[0] += (2*velocity);


  velocity = m1->velocity.axes[1] = -m1->velocity.axes[1];
  newPos.axes[1] += (2*velocity);*/

  
  m1->layer->posNext = newPos;
  
  P2IFG = 0;
}

