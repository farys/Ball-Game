#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "point3f.h"
class GameState
{
public:
    GameState();

    int life;
    Point3f force;
    int cristals;
    bool zeroForce;

    GameState operator+(GameState b){
       GameState c;
       c.force.x = this->force.x + b.force.x;
       c.force.y = this->force.y + b.force.y;
       c.force.z = this->force.z + b.force.z;
       c.cristals = this->cristals + b.cristals;
       c.life = this->life + b.life;
       return c;
    }

    bool operator !=(GameState b){
        if((this->force.x != b.force.x)
                || (this->force.y != b.force.y )
                || b.cristals != this->cristals
                || b.life != this->life
                || this->zeroForce != b.zeroForce){
            return true;
        }
        return false;
    }

    GameState operator-(GameState b){
       GameState c;
       c.force.x = this->force.x - b.force.x;
       c.force.y = this->force.y - b.force.y;
       c.force.z = this->force.z - b.force.z;

       c.cristals = this->cristals - b.cristals;
       c.life = this->life - b.life;
       return c;
    }

};

#endif // GAMESTATE_H
