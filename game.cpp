#include "game.h"
#include <QImage>
#include "maptile.h"
#include "c3ds.h"

#define BEZ(a) (a>0)? a : -a

Game::Game(QGLWidget *parent) :
    QGLWidget(parent)
{
    QTimer *timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(animate()) );
    connect( timer, SIGNAL(timeout()), this, SLOT(updatePsychics()) );
    timer->start( 8 );//4
}

Game::~Game()
{
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

    C3DS m((char*)"./data/models/cube.3DS");
    C3DS m2((char*)"./data/models/kolo.3DS");
    models.append(m);
    models.append(m2);

    QColor color;

    for(int x=0; x < image.width(); x++){
        for(int y=0; y < image.height(); y++){

            color.setRgb(image.pixel(x, y));

           /*if(color.red() == 0 && color.green() == 255){

               PowerUp *power = new PowerUp();
               power->setAnimation(&animationModels[0]);
               power->setTakenAnimation(&animationModels[0]);
               power->setPosition(x*TILE_SIZE, y*TILE_SIZE);
               this->powerUps.append(power);
               this->addItem(power);
           }*/

           if(color.red() == 0 && color.blue() == 0 && color.green() == 0){

               MapTile *tile = new MapTile();
               tile->setModelId(0);
               tile->setPosition(new Point3f(x*20, (image.height()-y)*20, 0));
               this->mapTiles.append(tile);

           }

           /*if(color.red() == 255 && color.green()== 0){

               Enemy *enemy = new Enemy(&this->animationModels, 6);
               enemy->setPosition(x*TILE_SIZE, y*TILE_SIZE);
               this->enemy.append(enemy);
               this->addItem(enemy);
           }

           if(color.red() == 200 && color.green()== 100 && color.blue() == 200){

               EnemyWallTile *wall = new EnemyWallTile();
               wall->setAnimation(&animationModels[2]);
               wall->setPosition(x*TILE_SIZE, y*TILE_SIZE);
               this->addItem(wall);
           }*/

           if(color.red() == 0 && color.green()== 0 && color.blue() == 255){

               this->player.setModelId(1);
               this->player.setPosition(new Point3f(x*20, (image.height()-y)*20, 0));
           }
        }
    }

    return true;
}

void Game::calculatePsychic(SceneActor *target){

        SceneActor *a = NULL;
        Point3f p = *target->getPosition();

        //z gory
        if(((a=this->actorAt(p.x+3, p.y+10+1)) || (a=this->actorAt(p.x+7, p.y+10+1))) && a->calculateCollisions()){
            if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionDown(target);
        }
        }

        //z prawej
        if(((a=this->actorAt(p.x+10+1, p.y+3)) || (a=this->actorAt(p.x+10+1, p.y+7))) && a->calculateCollisions()){
            if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionLeft(target);
            }
        }

        //z lewej
        if(((a=this->actorAt(p.x-11, p.y+3)) || (a=this->actorAt(p.x-11, p.y+7))) && a->calculateCollisions()){
            if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionRight(target);
        }
        }

        //z dolu
        if(((a=this->actorAt(p.x+3, p.y-1)) || (a=this->actorAt(p.x+7, p.y-1))) && a->calculateCollisions()){

            if(a->getActorType() == ACTOR_MAP_TILE){
            MapTile *b = (MapTile*)a;
            b->onCollisionUp(target);
            }/* else if(a->getActorType() == ACTOR_ENEMY){
                Enemy *c = (Enemy*)a;
                c->onCollisionUp(target);
            } else if(a->getActorType() == ACTOR_POWERUP){
                PowerUp *d = (PowerUp*)a;
                d->onCollisionUp(target);
            } else if(a->getActorType() == ACTOR_WALLTILE){
                EnemyWallTile *e = (EnemyWallTile*)a;
                e->onCollisionUp(target);
            }*/
        }else {
            target->gameState.force.y -= 0.03;
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
