#include "sceneactor.h"
#include <math.h>
/*
void SceneActor::nextFrame(){
    animPos.setX(animPos.x()+this->animation->direction);
    // > max
    if(animPos.x() >= animation->max_x){
        animPos.setX(animation->start_x);
        animPos.setY(animPos.y()+this->animation->direction);
        if(animPos.y() >= animation->max_y){
            firstFrame();
            this->onEndOfAnimation();
            return;
        }
    }

    // < start
    if(animPos.x() < animation->start_x){
        animPos.setX(animation->max_x-1);
        animPos.setY(animPos.y()+this->animation->direction);
        if(animPos.y() < animation->start_y){
            lastFrame();
            this->onEndOfAnimation();
        }
    }
}
*/
void SceneActor::setPosition(Point3f *p){
    this->pos = *p;
}
/*
void SceneActor::setPosition(QPointF f){
    pos.setX((int)f.rx());
    pos.setY((int)f.ry());
}

QRectF SceneActor::boundingRect() const {
    return QRectF(pos.x(), pos.y(), animation->size_x, animation->size_y);
}

void SceneActor::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
   painter->drawImage(
               pos,
               *this->animation->image,
               QRect(animPos.x() * animation->size_x, animPos.y() * animation->size_y, animation->size_x, animation->size_y)
               );
   nextFrame();
}

void SceneActor::firstFrame(){
    if(this->animation->direction < 0){
        this->lastFrame();
        return;
    }

    animPos.setX(animation->start_x);
    animPos.setY(animation->start_y);
}

void SceneActor::lastFrame(){
    animPos.setX(animation->max_x-1);
    animPos.setY(animation->max_y-1);
}

void SceneActor::randomFrame(){
    animPos.setX(animation->start_x + (rand() % (animation->max_x-animation->start_x)));
    animPos.setY(animation->start_y + (rand() % (animation->max_y-animation->start_y)));
}

QString SceneActor::toString(){
    QString result = "Animacja: ";
    result += "ramka_x=[" + QString::number(animPos.x()) + "/" + QString::number(animation->max_x) + "], ";
    result += "ramka_y=[" + QString::number(animPos.y()) + "/" + QString::number(animation->max_y) + "]";
    return result;
}

void SceneActor::setAnimation(AnimationImage *animation){
    this->animation = animation;
    this->firstFrame();
    this->setSize(QPoint(this->animation->size_x, this->animation->size_y));
}
*/
SceneActor::SceneActor(){
    this->barrier = false;
    this->psychic = false;
    this->life = 1;
    this->friction = 1;
    this->calcCollision = true;
}
