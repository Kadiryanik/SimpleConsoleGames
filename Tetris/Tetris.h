#ifndef TETRIS_H
#define TETRIS_H

#include <unistd.h>

#include "Shape.h"
#include "Shapes/L.h"
#include "Shapes/MirroredL.h"
#include "Shapes/S.h"
#include "Shapes/Square.h"
#include "Shapes/Stick.h"
#include "Shapes/T.h"
#include "Shapes/Z.h"

/*------------------------------------------------------------------------------*/
#define AREA_ROW_LEN    24
#define AREA_COLUMN_LEN 20
#define AREA_WALL_LEN   1

#if AREA_WALL_LEN < 1
#error "AREA_WALL_LEN must bigger than 0!"
#endif /* AREA_WALL_LEN < 1 */

/*------------------------------------------------------------------------------*/
#define CHAR_SPACE       ' '
#define CHAR_WALL        '#'
#define CHAR_LANDING     '*'
#define CHAR_LANDED      '+'

/*------------------------------------------------------------------------------*/
#define CHECK_SPACE   0
#define CHECK_FOOD    1
#define CHECK_OTHER   2 // wall or shapes

/*------------------------------------------------------------------------------*/
#define CHAR_LEFT   'A'
#define CHAR_RIGHT  'D'
#define CHAR_DOWN   'S'

#define CHAR_PAUSE  'P'
#define CHAR_ROTATE 'R'
#define CHAR_QUIT   'Q'

/*------------------------------------------------------------------------------*/
#define RET_LANDING   0
#define RET_LANDED    1
#define RET_NOT_MOVED 2
#define RET_GAME_OVER 3

/*------------------------------------------------------------------------------*/
typedef struct pos{
  int x;
  int y;
} pos_t;

/*------------------------------------------------------------------------------*/
class Tetris{
  private:
    char gameArea[AREA_ROW_LEN][AREA_COLUMN_LEN];
    pos_t cursor;

    void clear(Shape *shape);
    void set(Shape *shape, char c);
    bool check(Shape *shape, char direction);
    void moveDirection(Shape *shape, char direction);
    void checkAndDeleteRow();
    void gameOver(Shape *shape);
  public:
  Tetris();
  ~Tetris();
  void draw();
  int move(Shape *shape, char direction);
  void generateShape(Shape *shape);
};

/*------------------------------------------------------------------------------*/
#endif /* TETRIS_H */
