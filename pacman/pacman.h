

#ifndef pacman_included
#define pacman_included

#include "movingLayer.h"

extern Layer enemyLayer1;
extern Layer enemyLayer0;
extern MovLayer movingEnemy1;
extern MovLayer movingEnemy0;



static short pacDotsGotten = 0;


static const AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 5, screenHeight/2 - 10}
};

static const AbRectOutline obstacleOutline = {	// playing field 
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {15, 9} //obstacle (width), (height) //18 to 13
};

Layer pacDotsLayer5 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {screenWidth-20, 27}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &enemyLayer0,
};

Layer pacDotsLayer4 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {20, 27}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer5,
};

Layer pacDotsLayer3 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {screenWidth-20, (screenHeight/2)+4}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer4,
};

Layer pacDotsLayer2 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {20, (screenHeight/2)+4}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer3,
};

Layer pacDotsLayer1 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {screenWidth-20, (screenHeight-14)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer2,
};
Layer pacDotsLayer0 = {		/**< Layer with an orange circle */
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

Layer pacmanLayer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle5,
  {screenWidth/2, (screenHeight-14)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &fieldLayer, 
};


MovLayer ml6 = { &pacDotsLayer5, {0,0}, &movingEnemy0}; //moving layer for pacman dot
MovLayer ml5 = { &pacDotsLayer4, {0,0}, &ml6}; //moving layer for pacman dot
MovLayer ml4 = { &pacDotsLayer3, {0,0}, &ml5}; //moving layer for pacman dot
MovLayer ml3 = { &pacDotsLayer2, {0,0}, &ml4}; //moving layer for pacman dot
MovLayer ml2 = { &pacDotsLayer1, {0,0}, &ml3}; //moving layer for pacman dot
MovLayer ml1 = { &pacDotsLayer0, {0,0}, &ml2}; //moving layer for pacman dot
MovLayer ml0 = { &pacmanLayer0, {0,0},&ml1}; //Pacman layer movement

#endif
