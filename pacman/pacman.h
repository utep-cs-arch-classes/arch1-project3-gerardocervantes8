

#ifndef pacman_included
#define pacman_included

#include "movingLayer.h"

/**Gets enemy layers and moving layers to link them to the rest of the layers*/
extern Layer enemyLayer1;
extern Layer enemyLayer0;
extern MovLayer movingEnemy1;
extern MovLayer movingEnemy0;


static const AbRectOutline fieldOutline = {	// playing field (outer border)
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 5, screenHeight/2 - 10}
};

static const AbRectOutline obstacleOutline = {	// fences inside outer border
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {15, 9} //obstacle (width), (height)
};


Layer pacDotsLayer5 = {		//< Layer with pacdot
  (AbShape *)&circle2,
  {screenWidth-20, 27}, 
  {0,0}, {0,0},		
  COLOR_ORANGE,
  &enemyLayer0,
};

Layer pacDotsLayer4 = {		//layer with pacdot
  (AbShape *)&circle2,
  {20, 27},
  {0,0}, {0,0},
  COLOR_ORANGE,
  &pacDotsLayer5,
};

Layer pacDotsLayer3 = {	//layer with pacdot	
  (AbShape *)&circle2,
  {screenWidth-20, (screenHeight/2)+4}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer4,
};

Layer pacDotsLayer2 = {   //layer with pacdot
  (AbShape *)&circle2,
  {20, (screenHeight/2)+4}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer3,
};

Layer pacDotsLayer1 = {		//layer with pacdot
  (AbShape *)&circle2,
  {screenWidth-20, (screenHeight-14)}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer2,
};
Layer pacDotsLayer0 = {		//layer with pacdot
  (AbShape *)&circle2,
  {14, (screenHeight-14)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer1,
};

Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2-1, (screenHeight/2)+6},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &pacDotsLayer0
};

Layer pacmanLayer0 = {		//Layer with pacman (the character you control)
  (AbShape *)&circle5,
  {screenWidth/2, (screenHeight-14)}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &fieldLayer, 
};


MovLayer ml6 = { &pacDotsLayer5, {0,0}, &movingEnemy0}; //moving layer for pacdot
MovLayer ml5 = { &pacDotsLayer4, {0,0}, &ml6}; //moving layer for pacdot
MovLayer ml4 = { &pacDotsLayer3, {0,0}, &ml5}; //moving layer for pacdot
MovLayer ml3 = { &pacDotsLayer2, {0,0}, &ml4}; //moving layer for pacdot
MovLayer ml2 = { &pacDotsLayer1, {0,0}, &ml3}; //moving layer for pacdot
MovLayer ml1 = { &pacDotsLayer0, {0,0}, &ml2}; //moving layer for pacdot
MovLayer ml0 = { &pacmanLayer0, {0,0},&ml1}; //Moving layer with Pacman

#endif
