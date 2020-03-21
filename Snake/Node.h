#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <stdlib.h>

/*------------------------------------------------------------------------------*/
class Node{
public:
  Node *prev;
  Node *next;
  int posX;
  int posY;
  Node();
  ~Node();
  void setPos(int x, int y);
  void setLink(Node *p, Node *n);
};

/*------------------------------------------------------------------------------*/
#endif /* NODE_H */
