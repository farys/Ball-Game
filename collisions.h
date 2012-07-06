#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "shapes.h"

class Collisions
{
public:
    Collisions();

    static bool Test(SphereShape *s1, SphereShape *s2){
        Point3f *p1 = s1->actor->getPosition();
        Point3f *p2 = s2->actor->getPosition();

        return sqrt((p2->x+p1->x)+(p2->)) < (s1.getRadius()+s2.getRadius());
    }


};

#endif // COLLISIONS_H
