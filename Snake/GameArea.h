#ifndef GAME_AREA_H
#define GAME_AREA_H

#include "Snake.h"

/*------------------------------------------------------------------------------*/
#define AREA_ROW_LEN    25
#define AREA_COLUMN_LEN 70

/*------------------------------------------------------------------------------*/
#define CHAR_SPACE       ' '
#define CHAR_WALL        '#'
#define CHAR_SNAKE_HEAD  'O'
#define CHAR_SNAKE_TAIL  '*'
#define CHAR_FOOD        '+'

/*------------------------------------------------------------------------------*/
#define CHECK_SPACE   0
#define CHECK_FOOD    1
#define CHECK_OTHER   2 // snake or wall

/*------------------------------------------------------------------------------*/
class GameArea{
  public:
    GameArea();
    ~GameArea();
    void init(Snake *const snake);
    void draw(Snake *const snake);
    int check(int posX, int posY);
    void set(int posX, int posY, char c);
};

/*------------------------------------------------------------------------------*/
#endif /* GAME_AREA_H */
