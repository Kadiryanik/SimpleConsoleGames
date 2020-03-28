#ifndef BULLET_H
#define BULLET_H

#include <iostream>

#include "CharColor.h"

/*------------------------------------------------------------------------------*/
class Tank;

/*------------------------------------------------------------------------------*/
// Bullet draw character
#define DEFAULT_BULLET_CHAR        ((char)164)

// Bullet character on map
#define DEFAULT_PLAYER_BULLET_CHAR ((char)0x05)
#define DEFAULT_ENEMY_BULLET_CHAR  ((char)0x06)

/*------------------------------------------------------------------------------*/
typedef struct coordinate{
  int x;
  int y;
} coordinate_t;

/*------------------------------------------------------------------------------*/
typedef struct charCoor{
  int x;
  int y;
  char c;
} charCoor_t;

/*------------------------------------------------------------------------------*/
class Bullet{
    char direction;
  public:
    Tank *owner;
    Bullet *next;
    Bullet *prev;

    char c;
    coordinate_t coordinate;

    Bullet(Tank *owner, char direction, coordinate_t coordinate, Bullet *prev);
    ~Bullet();
	 charCoor_t updateCoor();
	 charCoor_t getCoor();
};

#endif /* BULLET_H */
