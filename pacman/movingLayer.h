

#ifndef movingLayer_included
#define movingLayer_included

#include <shape.h>


/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude)
 */
typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

void movLayerDraw(MovLayer* movLayers, Layer* layers);

/**Every moveLayer linked to the given movelayer, pos becomes posNext, used for movement*/
void mlAdvance(MovLayer *ml);

#endif // included
