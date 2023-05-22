#include "Util.h"
#include <math.h>

//Returns the straight line distance between two points
float dist(float x1, float y1, float x2, float y2)
{
    float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return dist;
}

//Returns map tile based on pixel location
int pixelToTile(int num)
{
    return int(num / 64);
}
