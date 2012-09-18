#include "game.h"
#include <QImage>
#include <QLabel>
#include "maptile.h"
#include "cristal.h"
#include "c3ds.h"
#include "cristalmodel.h"
#define BEZ(a) (a>0)? a : -a

Game::Game(QGLWidget *parent) :
    QGLWidget(parent)
{
    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(animate()) );
    connect( timer, SIGNAL(timeout()), this, SLOT(updatePsychics()) );
    timer->start( 8 );//4

    QTimer *fpsTimer = new QTimer( this );
    connect( fpsTimer, SIGNAL(timeout()), this, SLOT(updateFps()) );
    fpsTimer->start(1000);

    fps = frames = 0;
    currentLevel = 0;
}

Game::~Game()
{
}

void Game::initializeGL()
{
    glClearColor(0, 0.3, 0.05, 0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHTING);
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat light_position[] = { 0.0, 10.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_FRONT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(150, this->height()/this->width(), 0.1, 10000);
    angleY = angleZ = 0;
    angleX = -0.2;
    this->loadLevel(currentLevel++);
    leftKey = 0;
    rightKey = 0;
    scaleWorld = 0.002;
    rotateX = 0;
}

bool testCubePoint(Point3f p, Point3f min, Point3f max){

    if(min.x < p.x && max.x > p.x
            && min.y < p.y && max.y > p.y){
        return true;
    }
    return false;
}

bool Game::loadLevel(int level){
    QImage image;
    if(!image.load(QString("data/") + QString::number(level) + ".gif")) return false;

    levelWidth = image.width()*20;
    levelHeight = image.height()*20;

    C3DS m((char*)"./data/models/cube.3DS");
    C3DS m2((char*)"./data/models/kolo.3DS");
    CristalModel cristal;
    C3DS *m3 = (C3DS*) &cristal;
    models.append(m);
    models.append(m2);
    models.append(*m3);

    QColor color;

    for(int x=0; x < image.width(); x++){
        for(int y=0; y < image.height(); y++){

            color.setRgb(image.pixel(x, y));

            if(color.red() == 0 && color.green() == 255){

                Cristal *power = new Cristal();
                power->setModelId(2);
                power->setPosition(new Point3f(x*20, (image.height()-y)*20, 0));
                this->cristals.append(power);
            }

            if(color.red() == 0 && color.blue() == 0 && color.green() == 0){

                MapTile *tile = new MapTile();
                tile->setModelId(0);
                tile->setPosition(new Point3f(x*20, (image.height()-y)*20, 0));
                this->mapTiles.append(tile);
            }

            if(color.red() == 0 && color.green()== 0 && color.blue() == 255){

                this->player.setModelId(1);
                this->player.setPosition(new Point3f(x*20, (image.height()-y)*20, 0));

                levelTranslate.x = (levelWidth > x)? 490 : (levelWidth*0.5f)-10;

                levelTranslate.y = (levelHeight*0.5f);
            }
        }
    }

    return true;
}

void Game::closeLevel(){
    for(int i=0; i < this->mapTiles.length(); i++){
        delete this->mapTiles[i];
    }
    this->mapTiles.clear();

    for(int i=0; i < this->cristals.length(); i++){
        delete this->cristals[i];
    }
    this->cristals.clear();
}

bool Game::nextLevel(){
    closeLevel();
    loadLevel(currentLevel++);
}

void Game::calculatePsychic(SceneActor *target){
    SceneActor *a = NULL;
    Point3f p = *target->getPosition();

    //z gory
    if(((a=this->actorAt(p.x-7, p.y+10)) || (a=this->actorAt(p.x+7, p.y+10))) && a->calculateCollisions()){
        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionDown(target);
        }
    }

    //z prawej
    if(((a=this->actorAt(p.x+10, p.y-7)) || (a=this->actorAt(p.x+10, p.y+7))) && a->calculateCollisions()){
        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionLeft(target);
        }
    }

    //z lewej
    if(((a=this->actorAt(p.x-10, p.y-7)) || (a=this->actorAt(p.x-10, p.y+7))) && a->calculateCollisions()){
        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionRight(target);
        }
    }

    //z dolu
    if(((a=this->actorAt(p.x-6, p.y-10)) || (a=this->actorAt(p.x+6, p.y-10))) && a->calculateCollisions()){

        if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionUp(target);
        } else if(a->getActorType() == ACTOR_POWERUP){
            Cristal *d = (Cristal*)a;
            d->onCollisionUp(target);
        }
    }else {
        target->gameState.force.y -= 0.03; //grawitacja
    }

    p = *target->getPosition();
    Point3f f = target->getForce();
    Point3f newF;

    //kalkulacja nowych sil
    newF.x = (f.x + target->gameState.force.x) * target->getFriction();
    newF.y = (f.y + target->gameState.force.y) * target->getFriction();

    //zdarzenie gdy sila na x przestala oddzialywac
    if(newF.x < 0.001f && newF.x > -0.001f){
        target->gameState.zeroForce = true;
        newF.x = 0;
    }

    //gdy sila na osi y przestala oddzialywac
    if(newF.y < 0.001f && newF.y > -0.001){
        newF.y = 0;
    }

    //maksymalna oddzialywana sila
    if(newF.x > 10){
        newF.x = 10;
    } else if(newF.x < -10) {
        newF.x = -10;
    }

    target->setPosition(new Point3f(p.x+newF.x, p.y+newF.y, 0));
    target->setForce(&newF);
    target->update();
    target->gameState = GameState();
}
