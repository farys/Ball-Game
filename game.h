#ifndef GAME_H
#define GAME_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <math.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <QList>

#include "maptile.h"
#include "register.h"
#include "c3ds.h"
#include "player.h"

bool testCubePoint(Point3f p, Point3f min, Point3f max);


class Game : public QGLWidget
{
    Q_OBJECT
    
public:
    GLfloat angleX,angleY,angleZ;
    explicit Game(QGLWidget *parent = 0);
    ~Game();
    bool loadLevel(int level);

protected:

    QList<MapTile*> mapTiles;
    QList<C3DS> models;

    Player player;
    bool leftKey, rightKey;

    void initializeGL()
    {
        glClearColor(0, 0, 0, 0);
        glEnable(GL_DEPTH_TEST);
        glShadeModel (GL_SMOOTH);

        glEnable(GL_LIGHTING);
        GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
        GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1 };
        GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(150, this->height()/this->width(), 1, 10000);
        angleY = angleZ = 0;
        angleX = -0.2;
        this->loadLevel(1);
        leftKey = 0;
        rightKey = 0;
        glEnable(GL_TEXTURE_2D);
    }

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
        glRotatef(this->angleX, 1,0, 0);
        glRotatef(this->angleY, 0,1, 0);
        glRotatef(this->angleZ, 0,0, 1);
        //glScalef(0.5, 0.5, 0.5);
        //gluLookAt(0,10,-10, 0,0,0, 0, 1, 0);
        /*glBegin(GL_QUADS);
           glColor3f(1,1,1);

           //przod
           glVertex3f(0.5, 0.5, -0.5);
           glVertex3f(0.5, -0.5, -0.5);
           glVertex3f(-0.5, -0.5, -0.5);
           glVertex3f(-0.5, 0.5, -0.5);

           //tyl
           glVertex3f(0.5, 0.5, 0.5);
           glVertex3f(0.5, -0.5, 0.5);
           glVertex3f(-0.5, -0.5, 0.5);
           glVertex3f(-0.5, 0.5, 0.5);

           //lewa
           glVertex3f(-0.5, -0.5,-0.5);
           glVertex3f(-0.5, 0.5, -0.5);
           glVertex3f(-0.5, 0.5, 0.5);
           glVertex3f(-0.5, -0.5, 0.5);

           //prawa
           glVertex3f(0.5, -0.5,-0.5);
           glVertex3f(0.5, 0.5, -0.5);
           glVertex3f(0.5, 0.5, 0.5);
           glVertex3f(0.5, -0.5, 0.5);

           //gora
           glVertex3f(-0.5, 0.5,-0.5);
           glVertex3f(-0.5, 0.5, 0.5);
           glVertex3f(0.5, 0.5, 0.5);
           glVertex3f(0.5, -0.5, -0.5);

           glEnd();*/

        //glRotatef(0.2, 1,0,0);
        glScalef(0.002, 0.002, 0.002);
glTranslatef(-490,-510,0);

        //map
        for(int i=0; i < mapTiles.length(); i++){
            glPushMatrix();
            Point3f *p = mapTiles[i]->getPosition();

            glTranslatef(p->x, p->y, p->z);
            models[mapTiles[i]->getModelId()].render();
            glPopMatrix();
        }

        //player
        glPushMatrix();
        Point3f *p = player.getPosition();

        glTranslatef(p->x, p->y, p->z);
        glRotatef(this->player.gameState.force.x, 0,0,1 );
        models[player.getModelId()].render();
        glPopMatrix();
    }

    void keyPressEvent(QKeyEvent *event){
        switch(event->key()){
        case Qt::Key_Right:
            rightKey = true;
            break;
        case Qt::Key_Left:
            leftKey = true;
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
        case Qt::Key_Y:
            angleY += 1;
        break;
        }
    }


    SceneActor* actorAt(float x, float y, float z = 0){
        for(int i=0; i < mapTiles.length(); i++){
            Point3f *translation = mapTiles[i]->getPosition();
            Point3f min = models[mapTiles[i]->getModelId()].getMinPoint();
            Point3f max = models[mapTiles[i]->getModelId()].getMaxPoint();

            if(testCubePoint(Point3f(x,y,z), min + *translation, max + *translation)){
                return (SceneActor*) mapTiles[i];
            }
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

        /*for(int i=0; i< this->enemy.length(); i++){
               this->calculatePsychic(this->enemy[i]);
           }*/
        if(rightKey){
            player.gameState.force.x += 0.1;
        }

        if(leftKey){
            player.gameState.force.x -= 0.1;
        }
    }

};

#endif // GAME_H
