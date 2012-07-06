#ifndef SHAPES_H
#define SHAPES_H
#include "point3f.h"
#include "sceneactor.h"

class Shape {
public:
    SceneActor *actor;
};

class SphereShape : public Shape{
public:
    SphereShape* getShape(){
        return (SphereShape*) this;
    }
    void setRadius(float radius){
        this->radius = radius;
    }
    float getRadius(){
        return this->radius;
    }
protected:
    float radius;
};

class CubeShape : public Shape{
public:
    Point3f min, max;

    CubeShape* getShape(){
        return (CubeShape*) this;
    }

    void setMin(Point3f min){
        this->min = min;
    }

    void setMax(Point3f max){
        this->max = max;
    }
};

#endif // SHAPES_H
