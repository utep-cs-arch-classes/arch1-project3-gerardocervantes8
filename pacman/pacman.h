

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



Layer pacDotsLayer5 = {		//< Layer with pacdot, top-right
  (AbShape *)&circle2,
  {screenWidth-20, 27}, 
  {0,0}, {0,0},		
  COLOR_DARK_ORANGE,
  &enemyLayer0,
};

Layer pacDotsLayer4 = {		//layer with pacdot, top-left
  (AbShape *)&circle2,
  {20, 27},
  {0,0}, {0,0},
  COLOR_ORANGE,
  &pacDotsLayer5,
};

Layer pacDotsLayer3 = {	//layer with pacdot, middle-right
  (AbShape *)&circle2,
  {screenWidth-20, (screenHeight/2)+4}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer4,
};

Layer pacDotsLayer2 = {   //layer with pacdot, middle-left
  (AbShape *)&circle2,
  {20, (screenHeight/2)+4}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer3,
};

Layer pacDotsLayer1 = {		//layer with pacdot, bottom-right
  (AbShape *)&circle2,
  {screenWidth-20, (screenHeight-14)}, 
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer2,
};
Layer pacDotsLayer0 = {		//layer with pacdot, bottom-left
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

Layer pacmanLayer0 = {		//Layer with pacman (character you control)
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
MovLayer ml0 = { &pacmanLayer0, {0,0}, &ml1}; //Moving layer with Pacman

#endif

/**Used to create pacman, work in progress*/
/*
const unsigned char chordVecs5[6] = {
    5, // dist along axis = 0
    5, // dist along axis = 1
    5, // dist along axis = 2
    4, // dist along axis = 3
    3, // dist along axis = 4
    0, // dist along axis = 5
};

// true if pixel is in circle centered at centerPos
int abCircleChecked(const AbCircle *circle, const Vec2 *centerPos, const Vec2 *pixel)
{
  u_char radius = circle->radius;
  int axis;
  Vec2 relPos;
  vec2Sub(&relPos, pixel, centerPos); // vector from center to pixel

  int x = relPos.axes[0];
  int y = relPos.axes[1];
  if( (x > y) && (x > 0 )  ){
    return 0; 
  }
  vec2Abs(&relPos);		       project to first quadrant 

  return (relPos.axes[0] <= radius && circle->chords[relPos.axes[0]] >= relPos.axes[1]);
}

void
abCircleGetBoundsO(const AbCircle *circle, const Vec2 *centerPos, Region *bounds)
{
  u_char axis, radius = circle->radius;
  for (axis = 0; axis < 2; axis ++) {
    bounds->topLeft.axes[axis] = centerPos->axes[axis] - radius;
    bounds->botRight.axes[axis] = centerPos->axes[axis] + radius;
  }
  regionClipScreen(bounds);
}

const AbCircle pacman5 = {  abCircleGetBoundsO, abCircleChecked, chordVecs5, 5};*/

