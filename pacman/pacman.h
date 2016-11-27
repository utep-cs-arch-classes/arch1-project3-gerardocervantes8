
AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}}; /**< 10x10 rectangle */
AbRArrow rightArrow = {abRArrowGetBounds, abRArrowCheck, 30};

static int pacDotsGotten = 0;

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {screenWidth/2 - 5, screenHeight/2 - 10}
};


Layer pacDotsLayer9 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {(screenWidth/2)+20, (screenHeight/2)+45}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  0,
};

Layer pacDotsLayer8 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {(screenWidth/2)+41, (screenHeight/2)+35}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer9,
};

Layer pacDotsLayer7 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {55, 45}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer8,
};

Layer pacDotsLayer6 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {60, 70}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer7,
};

Layer pacDotsLayer5 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {80, 60}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer6,
};

Layer pacDotsLayer4 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {100, 100}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer5,
};


Layer pacDotsLayer3 = {		/**< Layer with an orange circle */
  (AbShape *)&circle2,
  {(screenWidth/2)+45, (screenHeight/2)+25}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_ORANGE,
  &pacDotsLayer4,
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
  {screenWidth/2-1, (screenHeight/2)+6},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &pacDotsLayer0 //previously &layer3
};

Layer pacmanLayer0 = {		/**< Layer with an orange circle */
  (AbShape *)&circle5,
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
MovLayer ml10 = { &pacDotsLayer9, {0,0}, 0}; //moving layyer for pacman dot
MovLayer ml9 = { &pacDotsLayer8, {0,0}, &ml10}; //moving layyer for pacman dot

MovLayer ml8 = { &pacDotsLayer7, {0,0}, &ml9}; //moving layyer for pacman dot
MovLayer ml7 = { &pacDotsLayer6, {0,0}, &ml8}; //moving layyer for pacman dot
MovLayer ml6 = { &pacDotsLayer5, {0,0}, &ml7}; //moving layyer for pacman dot
MovLayer ml5 = { &pacDotsLayer4, {0,0}, &ml6}; //moving layyer for pacman dot


MovLayer ml4 = { &pacDotsLayer3, {0,0}, &ml5}; //moving layyer for pacman dot
MovLayer ml3 = { &pacDotsLayer2, {0,0}, &ml4}; //moving layyer for pacman dot
MovLayer ml2 = { &pacDotsLayer1, {0,0}, &ml3}; //moving layyer for pacman dot
MovLayer ml1 = { &pacDotsLayer0, {0,0}, &ml2}; //moving layyer for pacman dot
MovLayer ml0 = { &pacmanLayer0, {0,0},&ml1}; //Pacman layer movement
