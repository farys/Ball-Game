#ifndef POINT3F_H
#define POINT3F_H

struct Point3f {
    float x,y,z;

    Point3f();
    Point3f(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point3f operator+(Point3f b){
        return Point3f(x + b.x, y + b.y, z + b.z);
    }

};
#endif // POINT3F_H
