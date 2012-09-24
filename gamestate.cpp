#include "gamestate.h"

GameState::GameState()
{
    this->life = 0;
    this->cristals = 0;
    this->force = Point3f(0, 0, 0);
    this->zeroForce = false;
}

GameState GameState::operator+(GameState b){
   GameState c;
   c.force.x = this->force.x + b.force.x;
   c.force.y = this->force.y + b.force.y;
   c.force.z = this->force.z + b.force.z;
   c.cristals = this->cristals + b.cristals;
   c.life = this->life + b.life;
   return c;
}

bool GameState::operator !=(GameState b){
    if((this->force.x != b.force.x)
            || (this->force.y != b.force.y )
            || b.cristals != this->cristals
            || b.life != this->life
            || this->zeroForce != b.zeroForce){
        return true;
    }
    return false;
}

GameState GameState::operator-(GameState b){
   GameState c;
   c.force.x = this->force.x - b.force.x;
   c.force.y = this->force.y - b.force.y;
   c.force.z = this->force.z - b.force.z;

   c.cristals = this->cristals - b.cristals;
   c.life = this->life - b.life;
   return c;
}
