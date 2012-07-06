#include "animationimage.h"

AnimationImage::AnimationImage(int start_x, int start_y, int max_x, int max_y, int size_x, int size_y, int direction, QImage *image){
    this->max_x = max_x;
        this->max_y = max_y;
        this->size_x = size_x;
        this->size_y = size_y;
        this->start_x = start_x;
        this->start_y = start_y;
        this->image = image;
        //this->name = name;
        this->direction = direction;
}
