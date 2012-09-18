#ifndef GAME_H
#define GAME_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <math.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "maptile.h"
#include "register.h"
#include "c3ds.h"
#include "player.h"
#include "cristal.h"

bool testCubePoint(Point3f p, Point3f min, Point3f max);

class Game : public QGLWidget
{
    Q_OBJECT
    
public:

    explicit Game(QGLWidget *parent = 0);
    ~Game();
    bool loadLevel(int level);
    void closeLevel();
    bool nextLevel();
protected:

    QList<MapTile*> mapTiles;
    QList<Cristal*> cristals;
    QList<C3DS> models;

    Player player;
    bool leftKey, rightKey, downKey;
    int rotateX;
    int frames;
    int fps;
    int currentLevel;
    Point3f levelTranslate;
    float scaleWorld;
    int levelWidth, levelHeight;
    GLfloat angleX,angleY,angleZ;
    void initializeGL();

    void resizeGL(int w, int h)
    {
        glViewport(0, 0, (GLint)w, (GLint)h);
    }

    void paintGL()
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        /*
gluLookAt(
            this->player.getPosition()->x, this->player.getPosition()->y+10, this->player.getPosition()->z-10,
            this->player.getPosition()->x, this->player.getPosition()->y, this->player.getPosition()->z,
            0, 1, 0);*/

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        qglColor(QColor(255, 255, 0));
        this->renderText(-1,0.9,-.5, QString("FPS: ") + QString::number(fps));
        this->renderText(-1,0.8,-.5, QString("Cristals: ") + QString::number(this->cristals.length()));
        this->renderText(-1,0.7,-.5,
                         QString("Player pos: (") + QString::number(this->player.getPosition()->x) +
                         QString(", ") + QString::number(this->player.getPosition()->y)
                         + QString(", ") + QString::number(this->player.getPosition()->z)
                         + QString(")"));


        glRotatef(this->angleX, 1, 0, 0);
        glRotatef(this->angleY, 0, 1, 0);
        glRotatef(this->angleZ, 0, 0, 1);

        Point3f *p = player.getPosition();
        glScalef(scaleWorld, scaleWorld, scaleWorld);

        //pilka ma srodek w p(x,y) -10 +10 za gabaryty
        if(p->x > 490 && levelWidth-510 > p->x) {
            levelTranslate.x = p->x;
        }

        //pilka ma na spodzie y=0
        if(p->y > 500 && levelHeight-510 > p->y) {
            levelTranslate.y = p->y;
        }
        glTranslatef(-levelTranslate.x, -levelTranslate.y, 0);

        //map
        for(int i=0; i < mapTiles.length(); i++){
            glPushMatrix();
            Point3f *p = mapTiles[i]->getPosition();

            glTranslatef(p->x, p->y, p->z);
            models[mapTiles[i]->getModelId()].render();
            glPopMatrix();
        }

        //cristals
        for(int i=0; i < cristals.length(); i++){
            glPushMatrix();
            Point3f *p = cristals[i]->getPosition();

            glTranslatef(p->x, p->y, p->z);
            glRotatef(cristals[i]->rotateZ, 1,1,1);
            cristals[i]->rotateZ += 0.6;
            models[cristals[i]->getModelId()].render();
            glPopMatrix();
        }

        //player
        glTranslatef(p->x, p->y, p->z);
        //glPushMatrix();
        this->rotateX += this->player.getForce().x * 1.5;
        glRotatef(this->rotateX * -1, 0,0,1 );
        //glPopMatrix();
        models[player.getModelId()].render();

        frames++;
    }

    void keyPressEvent(QKeyEvent *event){
        switch(event->key()){
        case Qt::Key_Right:
            rightKey = true;
            break;
        case Qt::Key_Left:
            leftKey = true;
            break;
        case Qt::Key_Down:
            downKey = true;
            break;
        }
    }

    void keyReleaseEvent(QKeyEvent *event){
        switch(event->key()){
        case Qt::Key_Right:
            rightKey = false;
            break;
        case Qt::Key_Left:
            leftKey = false;
            break;
        case Qt::Key_Down:
            downKey = false;
            break;
        case Qt::Key_Y:
            angleY += 1;
            break;
        case Qt::Key_Z:
            angleZ += 1;
            break;
        case Qt::Key_F9:
            scaleWorld -= 0.0001;
            break;
        case Qt::Key_F10:
            scaleWorld += 0.0001;
            break;
        }
    }

    SceneActor* actorAt(float x, float y, float z = 0){
        /* Map tiles */
        for(int i=0; i < mapTiles.length(); i++){
            Point3f *translation = mapTiles[i]->getPosition();
            Point3f min = models[mapTiles[i]->getModelId()].getMinPoint();
            Point3f max = models[mapTiles[i]->getModelId()].getMaxPoint();

            if(testCubePoint(Point3f(x,y,z), min + *translation, max + *translation)){
                return (SceneActor*) mapTiles[i];
            }
        }

        /* Cristals */
        if(cristals.length()){
            Point3f min = models[cristals[0]->getModelId()].getMinPoint();
            Point3f max = models[cristals[0]->getModelId()].getMaxPoint();

            for(int i=0; i < cristals.length(); i++){
                Point3f *translation = cristals[i]->getPosition();

                if(testCubePoint(Point3f(x,y,z), min + *translation, max + *translation)){ //sprawdzam czy punkt (x,y,z) znajduje sie wewnatrz prostopadloscianu krysztalu
                    SceneActor *c = (SceneActor*) cristals[i]; //pobieram wskaznik na krysztal
                    this->cristals.removeAt(i); //usuwam krysztal z listy, aby go nie wyswietlac wiecej, ani nie liczyc kolizji
                    return c; //zwracam krysztal
                }
            }
        }else {
            nextLevel();
        }

        return NULL;
    }

    void calculatePsychic(SceneActor *target);
public slots:
    void animate(){
        //           angleZ += 1;
        this->updateGL();
    }
    void updatePsychics(){
        this->calculatePsychic(&this->player);

        if(rightKey){
            player.gameState.force.x += 0.1;
        }

        if(leftKey){
            player.gameState.force.x -= 0.1;
        }

        if(downKey){
            player.gameState.force.y -= 0.1;
        }
    }

    void updateFps(){
        fps = frames;
        frames = 0;
    }

};

#endif // GAME_H
