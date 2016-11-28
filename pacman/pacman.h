

//AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */
//AbRArrow rightArrow = {abRArrowGetBounds, abRArrowCheck, 30};

static short pacDotsGotten = 0;


static AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 5, screenHeight/2 - 10}
};

static AbRectOutline obstacleOutline = {	// playing field 
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {15, 9} //obstacle (width), (height) //18 to 13
};


/*static Layer pacDotsLayer9 = {		//< Layer with an orange circle 
  (AbShape *)&circle2,
  {(screenWidth/2)+20, (screenHeight/2)+45}, //< bit below & right of center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_ORANGE,
  0,
};

static Layer pacDotsLayer8 = {		//< Layer with an orange circle 
  (AbShape *)&circle2,
  {(screenWidth/2)+41, (screenHeight/2)+35}, //< bit below & right of center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_ORANGE,
  &pacDotsLayer9,
  };

static Layer pacDotsLayer7 = {		//< Layer with an orange circle
  (AbShape *)&circle2,
  {55, 45},                               //< bit below & right of center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_ORANGE,
  0,
};

static Layer pacDotsLayer6 = {		//< Layer with an orange circle 
  (AbShape *)&circle2,
  {60, 70}, //< bit below & right of center 
  {0,0}, {0,0},				    //last & next pos 
  COLOR_ORANGE,
  &pacDotsLayer7,
};*/

Layer pacDotsLayer5 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {screenWidth-20, 27}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  0,
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
/*
Layer obstacleLayer4 = {		// playing field as a layer 
  (AbShape *) &obstacleOutline,
  {((screenWidth/4)*3)-3, ((screenHeight/4))+6},//< center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_BLUE,
  &pacDotsLayer0 
};
Layer obstacleLayer3 = {		// playing field as a layer 
  (AbShape *) &obstacleOutline,
  {((screenWidth/4))+3, ((screenHeight/4)*3)+6},//< center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_BLUE,
  &obstacleLayer4
};


Layer obstacleLayer2 = {		// playing field as a layer 
  (AbShape *) &obstacleOutline,
  {(screenWidth/2)-1, (screenHeight/2)+6},//< center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_BLUE,
  &obstacleLayer3
};
Layer obstacleLayer1 = {		// playing field as a layer 
  (AbShape *) &obstacleOutline,
  {((screenWidth/4)*3)-3, ((screenHeight/4)*3)+6},//< center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_BLUE,
  &obstacleLayer2
};

Layer obstacleLayer0 = {		 //playing field as a layer 
  (AbShape *) &obstacleOutline,
  {screenWidth/4+3, (screenHeight/4)+6},//< center 
  {0,0}, {0,0},				    // last & next pos 
  COLOR_BLUE,
  &obstacleLayer1 
};*/

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
//MovLayer ml10 = { &pacDotsLayer9, {0,0}, 0}; //moving layyer for pacman dot
//MovLayer ml9 = { &pacDotsLayer8, {0,0}, &ml10}; //moving layyer for pacman dot

//MovLayer ml8 = { &pacDotsLayer7, {0,0}, 0}; //moving layyer for pacman dot /**MOVE NEXT IS 0***/
//MovLayer ml7 = { &pacDotsLayer6, {0,0}, &ml8}; //moving layer for pacman dot
MovLayer ml6 = { &pacDotsLayer5, {0,0}, 0}; //moving layer for pacman dot
MovLayer ml5 = { &pacDotsLayer4, {0,0}, &ml6}; //moving layer for pacman dot


MovLayer ml4 = { &pacDotsLayer3, {0,0}, &ml5}; //moving layer for pacman dot
MovLayer ml3 = { &pacDotsLayer2, {0,0}, &ml4}; //moving layer for pacman dot
MovLayer ml2 = { &pacDotsLayer1, {0,0}, &ml3}; //moving layer for pacman dot
MovLayer ml1 = { &pacDotsLayer0, {0,0}, &ml2}; //moving layer for pacman dot
MovLayer ml0 = { &pacmanLayer0, {0,0},&ml1}; //Pacman layer movement

