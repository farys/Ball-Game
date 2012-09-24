#ifndef POINT3F_H
#define POINT3F_H

struct Point3f {
    float x,y,z;

    Point3f();
    Point3f(float x, float y, float z);
    Point3f operator+(Point3f b);
};
#endif // POINT3F_H
