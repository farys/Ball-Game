#include "point3f.h"

Point3f::Point3f()
{
    x=0;y=0;z=0;
}

Point3f::Point3f(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3f Point3f::operator+(Point3f b){
    return Point3f(x + b.x, y + b.y, z + b.z);
}
