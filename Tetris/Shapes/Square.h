#ifndef SQUARE_H
#define SQUARE_H

#include "../Shape.h"

/*------------------------------------------------------------------------------*/
class Square : public Shape{
  public:
    Square();
    ~Square();
    void rotate();
};

/*------------------------------------------------------------------------------*/
#endif /* SQUARE_H */
