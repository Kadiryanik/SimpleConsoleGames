#include "Tank.h"

/*------------------------------------------------------------------------------*/
Tank *tankHead = NULL;
int Tank::tankCount;

/*------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC-FUNCTIONS---------------------------------*/
/*------------------------------------------------------------------------------*/
Tank::Tank(int x, int y, int health, char headChar, char bodyChar, \
    char *controlMap, bool isPlayer){
  if(tankHead == NULL){
    tankHead = this;
    this->prev = NULL;
  } else{
    Tank *ptr = tankHead;
    while(ptr->next != NULL){
      ptr = ptr->next;
    }

    ptr->next = this;
    this->prev = ptr;
  }
  this->next = NULL;

  this->isPlayer = isPlayer;
  this->deleteAfterBulletHit = false;
  this->bulletHead = NULL;
  this->headChar = headChar;
  this->bodyChar = bodyChar;
  this->coordinate.x = x;
  this->coordinate.y = y;
  this->lastCoordinate = this->coordinate;
  this->health = health;
  this->score = 0;

  if(!(this->isPlayer)){
    setEnemyBody();
  }

  if(controlMap == NULL){
    this->controlMap[0] = ENEMY_CHAR_UP;
    this->controlMap[1] = ENEMY_CHAR_LEFT;
    this->controlMap[2] = ENEMY_CHAR_RIGHT;
    this->controlMap[3] = ENEMY_CHAR_DOWN;
    this->controlMap[4] = ENEMY_CHAR_FIRE;
  } else{
    for(int i = 0; i < TOTAL_CONTROL_NUM; i++){
      this->controlMap[i] = controlMap[i];
    }
  }

  // will set direction
  rotate(getDirection(this->controlMap[0]));

  this->tankCount++;
}

/*------------------------------------------------------------------------------*/
Tank::~Tank(){
  // link prev and next if exist
  if(prev != NULL){
    prev->next = next;
  }
  if(next != NULL){
    next->prev = prev;
  }

  // delete bullets if exist
  Bullet *bullet = this->bulletHead;
  while(bullet != NULL){
    Bullet *next = bullet->next;
    delete bullet;
    bullet = next;
  }

  // update tankHead
  if(this == tankHead){
    tankHead = next;
  }
  this->tankCount--;
}

/*------------------------------------------------------------------------------*/
int Tank::control(char control){
  // is deleteAfterBulletHit true mean tank will delete do not move
  if(this->deleteAfterBulletHit || checkControlMap(control)){
    return TANK_CONTROL_RET_NONE; // not for us
  }

  char d = getDirection(control);

  int ret = TANK_CONTROL_RET_MOVED;
  if(control == P1_CHAR_FIRE || control == P2_CHAR_FIRE || \
      control == ENEMY_CHAR_FIRE){
    ret = TANK_CONTROL_RET_FIRED;
  } else if(this->direction == d){
    // hold old coordinate for restore if necessary
    lastCoordinate = coordinate;

    switch(d){
      case DIRECTION_UP:
        coordinate.x -= 1;
        break;
      case DIRECTION_LEFT:
        coordinate.y -= 1;
        break;
      case DIRECTION_RIGHT:
        coordinate.y += 1;
        break;
      case DIRECTION_DOWN:
        coordinate.x += 1;
        break;
      default:
        ret = TANK_CONTROL_RET_NONE; // invalid move, do not proceed
        break;
    }
  } else{
    rotate(d);
    ret = TANK_CONTROL_RET_ROTATED;
  }

  return ret;
}

/*------------------------------------------------------------------------------*/
Bullet* Tank::fire(){
  coordinate_t coordinate;
  
  // set tank head as bullet coordinate
  coordinate.x = this->coordinate.x + this->headCoorOffset.x;
  coordinate.y = this->coordinate.y + this->headCoorOffset.y;

  char d = this->direction;
  switch(d){
    case DIRECTION_UP:
      coordinate.x -= 1;
      break;
    case DIRECTION_LEFT:
      coordinate.y -= 1;
      break;
    case DIRECTION_RIGHT:
      coordinate.y += 1;
      break;
    case DIRECTION_DOWN:
      coordinate.x += 1;
      break;
  }

  Bullet *ptr = this->bulletHead;
  if(ptr == NULL){
    this->bulletHead = new Bullet(this, d, coordinate, NULL);
    return this->bulletHead;
  } else{
    while(ptr->next != NULL){
      ptr = ptr->next;
    }

    ptr->next = new Bullet(this, d, coordinate, ptr);
    return ptr->next;
  }
}

/*------------------------------------------------------------------------------*/
void Tank::resetCoordinate(){
  coordinate = lastCoordinate;  
}

/*------------------------------------------------------------------------------*/
bool Tank::reduceHealth(){
  bool ret = false;
  this->health--;
  if(this->isPlayer){
    if(this->health < 1){
      ret = true;
    }
  } else{
    ret = setEnemyBody();
  }
  return ret;
}

/*------------------------------------------------------------------------------*/
void Tank::updateScore(){
  this->score += 10;
}

/*------------------------------------------------------------------------------*/
/*----------------------------PRIVATE-FUNCTIONS---------------------------------*/
/*------------------------------------------------------------------------------*/
// TODO: make this much more readable
bool Tank::setEnemyBody(){
  if(this->health >= 3){
    this->bodyChar = ENEMY_BODY_3_CHAR;
  } else if(this->health == 2){
    this->bodyChar = ENEMY_BODY_2_CHAR;
  } else if(this->health == 1){
    this->bodyChar = ENEMY_BODY_1_CHAR;
  } else{
    return true;
  }

  return false;
}

/*------------------------------------------------------------------------------*/
bool Tank::checkControlMap(char control){
  for(int i = 0; i < TOTAL_CONTROL_NUM; i++){
    if(controlMap[i] == control){
      return false; // for us
    }
  }
  return true; // not for us
}

/*------------------------------------------------------------------------------*/
void Tank::rotate(char direction){
  int offset;

  switch(direction){
    case DIRECTION_UP:
      offset = SHAPE_UP_OFFSET;
      break;
    case DIRECTION_LEFT:
      offset = SHAPE_LEFT_OFFSET;
      break;
    case DIRECTION_RIGHT:
      offset = SHAPE_RIGHT_OFFSET;
      break;
    case DIRECTION_DOWN:
      offset = SHAPE_DOWN_OFFSET;
      break;
    default:
      return; // invalid rotate, do not proceed
  }

  this->direction = direction;

  for(int i = 0; i < TANK_HEIGHT; i++){
    for(int j = 0; j < TANK_WIDTH; j++){
      matrix[i][j] = shape[offset + i][j];
      if(matrix[i][j] == TANK_HEAD_CHAR){
        headCoorOffset.x = i;
        headCoorOffset.y = j;
      }
    }
  }
}

/*------------------------------------------------------------------------------*/
char Tank::getDirection(char control){
  char d;

  switch(control){
    case P1_CHAR_UP:
    case P2_CHAR_UP:
    case ENEMY_CHAR_UP:
      d = DIRECTION_UP;
      break;
    case P1_CHAR_LEFT:
    case P2_CHAR_LEFT:
    case ENEMY_CHAR_LEFT:
      d = DIRECTION_LEFT;
      break;
    case P1_CHAR_RIGHT:
    case P2_CHAR_RIGHT:
    case ENEMY_CHAR_RIGHT:
      d = DIRECTION_RIGHT;
      break;
    case P1_CHAR_DOWN:
    case P2_CHAR_DOWN:
    case ENEMY_CHAR_DOWN:
      d = DIRECTION_DOWN;
      break;
    default:
      return DIRECTION_NONE;
  }

  return d;
}
