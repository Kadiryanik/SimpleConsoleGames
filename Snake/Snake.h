#ifndef SNAKE_H
#define SNAKE_H

#include "Node.h"

/*------------------------------------------------------------------------------*/
class GameArea;

/*------------------------------------------------------------------------------*/
#define INIT_POS_X      3
#define INIT_POS_Y      3
#define INIT_LEN        4
#define INIT_DIRECTION  1

/*------------------------------------------------------------------------------*/
#define CHAR_LEFT   'A'
#define CHAR_UP     'W'
#define CHAR_RIGHT  'D'
#define CHAR_DOWN   'S'

#define CHAR_PAUSE  'P'
#define CHAR_EXIT   'Q'

/*------------------------------------------------------------------------------*/
class Snake{
  private:
    GameArea *gameArea;
    Node *head;
    int len;
    int direction;
  public:
    Snake(GameArea* = NULL, int = INIT_POS_X, int = INIT_POS_Y, int = INIT_LEN, int = INIT_DIRECTION);
    ~Snake();
    bool move(char direction);
    Node* getHead();
    int getScore();
};

/*------------------------------------------------------------------------------*/
#endif /* SNAKE_H */
