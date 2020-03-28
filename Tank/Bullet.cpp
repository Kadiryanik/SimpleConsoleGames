#include "Bullet.h"
#include "Tank.h"

/*------------------------------------------------------------------------------*/
Bullet::Bullet(Tank *tank, char direction, coordinate_t coordinate, Bullet *prev){
  this->owner = tank;
  this->direction = direction;

  this->next = NULL;
  this->prev = prev;

  this->c = DEFAULT_ENEMY_BULLET_CHAR;
  if(this->owner->isPlayer){
    this->c = DEFAULT_PLAYER_BULLET_CHAR;
  }

  this->coordinate = coordinate;
}

/*------------------------------------------------------------------------------*/
Bullet::~Bullet(){
  // link prev and next if exist
  if(prev != NULL){
    prev->next = next;
  }
  if(next != NULL){
    next->prev = prev;
  }

  // update owner tank bulletHead
  if(this == owner->bulletHead){
    owner->bulletHead = next;
  }
}

/*------------------------------------------------------------------------------*/
charCoor_t Bullet::updateCoor(){
  charCoor_t ret;
  switch(this->direction){
    case DIRECTION_UP:
      this->coordinate.x -= 1;
      break;
    case DIRECTION_LEFT:
      this->coordinate.y -= 1;
      break;
    case DIRECTION_RIGHT:
      this->coordinate.y += 1;
      break;
    case DIRECTION_DOWN:
      this->coordinate.x += 1;
      break;
  }

  ret.x = this->coordinate.x;
  ret.y = this->coordinate.y;
  ret.c = this->c;

  return ret;
}

/*------------------------------------------------------------------------------*/
charCoor_t Bullet::getCoor(){
  charCoor_t ret;
  ret.x = this->coordinate.x;
  ret.y = this->coordinate.y;
  ret.c = this->c;
  return ret;
}
