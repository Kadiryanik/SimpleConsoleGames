#ifndef TANK_H
#define TANK_H

#include <iostream>

#include "Bullet.h"

/*------------------------------------------------------------------------------*/
#define TANK_HEIGHT 3
#define TANK_WIDTH  3

/*------------------------------------------------------------------------------*/
#define TANK_HEAD_CHAR ((char)(0x0e))
#define TANK_BODY_CHAR ((char)(0x0f))

/*------------------------------------------------------------------------------*/
const char shape[TANK_HEIGHT * 4][TANK_WIDTH] = {
  { TANK_BODY_CHAR, TANK_HEAD_CHAR, TANK_BODY_CHAR },
  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },
  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },

  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },
  { TANK_HEAD_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },
  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },

  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },
  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_HEAD_CHAR },
  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },

  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },
  { TANK_BODY_CHAR, TANK_BODY_CHAR, TANK_BODY_CHAR },
  { TANK_BODY_CHAR, TANK_HEAD_CHAR, TANK_BODY_CHAR }
};

/*------------------------------------------------------------------------------*/
#define SHAPE_UP_OFFSET    0
#define SHAPE_LEFT_OFFSET  (TANK_HEIGHT * 1)
#define SHAPE_RIGHT_OFFSET (TANK_HEIGHT * 2)
#define SHAPE_DOWN_OFFSET  (TANK_HEIGHT * 3)

/*------------------------------------------------------------------------------*/
#define DEFAULT_HEALTH_START    3

#define P1_DEFAULT_HEAD_CHAR    ((char)(0x10))
#define P1_DEFAULT_BODY_CHAR    ((char)(0x11))
#define P1_DEFAULT_COORDINATE_X 36
#define P1_DEFAULT_COORDINATE_Y 19
#define P1_DEFAULT_HEAD_COLOR   COLOR_FG_WHITE
#define P1_DEFAULT_BODY_COLOR   COLOR_FG_RED

#define P2_DEFAULT_HEAD_CHAR    ((char)(0x12))
#define P2_DEFAULT_BODY_CHAR    ((char)(0x13))
#define P2_DEFAULT_COORDINATE_X 36
#define P2_DEFAULT_COORDINATE_Y 38
#define P2_DEFAULT_HEAD_COLOR   COLOR_FG_WHITE
#define P2_DEFAULT_BODY_COLOR   COLOR_FG_GREEN

#define ENEMY_DEFAULT_HEAD_CHAR    ((char)(0x14))
#define ENEMY_BODY_1_CHAR          ((char)(0x15))
#define ENEMY_BODY_2_CHAR          ((char)(0x16))
#define ENEMY_BODY_3_CHAR          ((char)(0x17))
#define ENEMY_DEFAULT_BODY_CHAR    ENEMY_BODY_3_CHAR
#define ENEMY_DEFAULT_HEAD_COLOR   COLOR_FG_RED

#define ENEMY_DEFAULT_COORDINATE_X 1
#define ENEMY_DEFAULT_COORDINATE_Y 1

/*------------------------------------------------------------------------------*/
#define TOTAL_CONTROL_NUM 5

/*------------------------------------------------------------------------------*/
#define DIRECTION_NONE   ((char)0x01)
#define DIRECTION_UP     ((char)0x01)
#define DIRECTION_LEFT   ((char)0x02)
#define DIRECTION_RIGHT  ((char)0x03)
#define DIRECTION_DOWN   ((char)0x04)

/*------------------------------------------------------------------------------*/
#define FRIEND_FIRE (false)

/*------------------------------------------------------------------------------*/
#define P1_CHAR_UP     'W'
#define P1_CHAR_LEFT   'A'
#define P1_CHAR_RIGHT  'D'
#define P1_CHAR_DOWN   'S'
#define P1_CHAR_FIRE   'F'

/*------------------------------------------------------------------------------*/
#define P2_CHAR_UP     'U'
#define P2_CHAR_LEFT   'H'
#define P2_CHAR_RIGHT  'K'
#define P2_CHAR_DOWN   'J'
#define P2_CHAR_FIRE   'L'

/*------------------------------------------------------------------------------*/
#define ENEMY_CHAR_UP     ((char)(0x19))
#define ENEMY_CHAR_LEFT   ((char)(0x1a))
#define ENEMY_CHAR_RIGHT  ((char)(0x1b))
#define ENEMY_CHAR_DOWN   ((char)(0x1c))
#define ENEMY_CHAR_FIRE   ((char)(0x1d))

#define ENEMY_BULLET_CHAR ((char)(0x1d))

/*------------------------------------------------------------------------------*/
#define TANK_CONTROL_RET_NONE    0
#define TANK_CONTROL_RET_MOVED   1
#define TANK_CONTROL_RET_ROTATED 2
#define TANK_CONTROL_RET_FIRED   3

/*------------------------------------------------------------------------------*/
class Tank{
    coordinate_t headCoorOffset;
    char controlMap[TOTAL_CONTROL_NUM];
    bool setEnemyBody();
    bool checkControlMap(char control);
    void rotate(char direction);
    char getDirection(char control);
  public:
    static int tankCount;
    Tank *prev;
    Tank *next;
    bool isPlayer;
    bool deleteAfterBulletHit;
    Bullet *bulletHead;

    char direction;
    char headChar;
    char bodyChar;
    coordinate_t coordinate;
    coordinate_t lastCoordinate;
    char matrix[TANK_HEIGHT][TANK_WIDTH];
    int health;
    int score;

    Tank(int x, int y, int = DEFAULT_HEALTH_START, 
      char = ENEMY_DEFAULT_HEAD_CHAR, char = ENEMY_DEFAULT_BODY_CHAR,
      char * = NULL, bool = false);
    ~Tank();
    int control(char control);
    Bullet* fire();
    void resetCoordinate();
    bool reduceHealth();
    void updateScore();
};

#endif /* TANK_H */
