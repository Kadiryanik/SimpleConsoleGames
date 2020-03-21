#include "GameArea.h"
#include "Snake.h"

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
#define DIRECTION_LEFT   0
#define DIRECTION_UP     1
#define DIRECTION_RIGHT  2
#define DIRECTION_DOWN   3

/*------------------------------------------------------------------------------*/
Snake::Snake(GameArea *area, int posX, int posY, int len, int direction){
  cout << "Initializing Snake.." << endl;

  if(area == NULL){
    cout << "GameArea cant NULL!" << endl;
    exit(-1);
  }

  this->gameArea = area;

  head = new Node();
  if(head == NULL){
    cout << "Snake constructor failed to alloc head!" << endl;
    exit(-1);
  }
  head->setPos(posX, posY);
  this->len = len;

  Node *ptr = head;
  for(int i = 1; i < this->len; i++){
    while(ptr->next != NULL); // breaks when ptr->next is NULL
    ptr->next = new Node();

    if(ptr->next == NULL){
      cout << "Snake constructor failed to alloc tail!" << endl;
      exit(-1);
    }
    ptr->next->setLink(ptr, NULL);
    ptr->next->setPos(posX + i, posY);

    ptr = ptr->next;
  }

  this->direction = direction;
}

/*------------------------------------------------------------------------------*/
Snake::~Snake(){
  cout << "Destroying Snake.." << endl;
  Node *ptr = head;
  while(ptr->next != NULL){
    ptr = ptr->next;
  }

  while(ptr->prev != NULL){
    ptr = ptr->prev;
    delete ptr->next;
  }
  delete head;
}

/*------------------------------------------------------------------------------*/
bool Snake::move(char pressedChar){
  int posX = 0;
  int posY = 0;

  // set direction to current
  direction = this->direction;

  // get direction
  if(pressedChar == CHAR_LEFT){
    if(this->direction != DIRECTION_RIGHT){
      direction = DIRECTION_LEFT;
    }
  } else if(pressedChar == CHAR_UP){
    if(this->direction != DIRECTION_DOWN){
      direction = DIRECTION_UP;
    }
  } else if(pressedChar == CHAR_RIGHT){
    if(this->direction != DIRECTION_LEFT){
      direction = DIRECTION_RIGHT;
    }
  } else if(pressedChar == CHAR_DOWN){
    if(this->direction != DIRECTION_UP){
      direction = DIRECTION_DOWN;
    }
  } else if(pressedChar == CHAR_EXIT){
    return false;
  } // wrong key pressed using old direction

  this->direction = direction;

  // set new position
  if(direction == DIRECTION_LEFT){
    posX = head->posX;
    posY = head->posY - 1;
  } else if(direction == DIRECTION_UP){
    posX = head->posX - 1;
    posY = head->posY;
  } else if(direction == DIRECTION_RIGHT){
    posX = head->posX;
    posY = head->posY + 1;
  } else if(direction == DIRECTION_DOWN){
    posX = head->posX + 1;
    posY = head->posY;
  } // else wrong directioni didn't expect

  // handle overflows
  if(direction == DIRECTION_LEFT){
    if(posY == 0){
      posY = AREA_COLUMN_LEN - 2;
    }
  } else if(direction == DIRECTION_UP){
    if(posX == 0){
      posX = AREA_ROW_LEN - 2;
    }
  } else if(direction == DIRECTION_RIGHT){
    if(posY == AREA_COLUMN_LEN - 1){
      posY = 1;
    }
  } else if(direction == DIRECTION_DOWN){
    if(posX == AREA_ROW_LEN - 1){
      posX = 1;
    }
  }

  // update gameArea
  int checkResult = gameArea->check(posX, posY);
  if(checkResult == CHECK_OTHER){
    return false;
  } else if(checkResult == CHECK_FOOD){
    // set new head and links
    Node *newHead = new Node();
    newHead->posX = posX;
    newHead->posY = posY;
    newHead->next = head;
    head->prev = newHead;

    // set old head as tail
    gameArea->set(head->posX, head->posY, CHAR_SNAKE_TAIL);

    // set newHead as head
    head = newHead;
    gameArea->set(head->posX, head->posY, CHAR_SNAKE_HEAD);

    len += 1;
  } else if(checkResult == CHECK_SPACE){
    gameArea->set(posX, posY, CHAR_SNAKE_HEAD);

    // update linked list
    Node *ptr = head->next;

    // go to the end of list
    while(ptr->next != NULL){
      ptr = ptr->next;
    }

    // clear last node character
    gameArea->set(ptr->posX, ptr->posY, CHAR_SPACE);

    // update positions
    while(ptr->prev != NULL){
      Node *prev = ptr->prev;

      ptr->setPos(prev->posX, prev->posY);
      gameArea->set(ptr->posX, ptr->posY, CHAR_SNAKE_TAIL);

      ptr = prev;
    }

    // finally update head pos
    head->setPos(posX, posY);
  } else{
    cout << "Unknown character on gameArea!" << endl;
    return false;
  }

  return true;
}

/*------------------------------------------------------------------------------*/
Node* Snake::getHead(){
  return this->head;
}

/*------------------------------------------------------------------------------*/
int Snake::getScore(){
  return len * 10;
}
