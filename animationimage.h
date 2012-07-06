#ifndef SCENEANIMATIONIMAGE_H
#define SCENEANIMATIONIMAGE_H
#include <QImage>

struct AnimationImage {
    int start_x;
    int start_y;
    int max_x;
    int max_y;
    int size_x;
    int size_y;
    int direction;

    QImage *image;
    QString *name;

    AnimationImage(int start_x, int start_y, int max_x, int max_y, int size_x, int size_y, int direction, QImage *image);
};


#endif // SCENEANIMATIONIMAGE_H
