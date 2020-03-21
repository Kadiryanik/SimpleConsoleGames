#include "GameArea.h"

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
static char gameArea[AREA_ROW_LEN][AREA_COLUMN_LEN];

/*------------------------------------------------------------------------------*/
static void generateFood(){
  int posX, posY;
  do{
    posX = 1 + rand() % (AREA_ROW_LEN - 1);
    posY = 1 + rand() % (AREA_COLUMN_LEN - 1);
  } while(gameArea[posX][posY] != CHAR_SPACE);

  gameArea[posX][posY] = CHAR_FOOD;
}

/*------------------------------------------------------------------------------*/
GameArea::GameArea(){
  cout << "Initializing GameArea.." << endl;
}

/*------------------------------------------------------------------------------*/
GameArea::~GameArea(){
  cout << "Destroying GameArea.." << endl;
}

/*------------------------------------------------------------------------------*/
void GameArea::init(Snake *const snake){
  cout << "Initializing GameArea.." << endl;

  /* initialize random seed: */
  srand (time(NULL));

  for(int i = 0; i < AREA_ROW_LEN; i++){
    for(int j = 0; j < AREA_COLUMN_LEN; j++){
      if(i == 0 || j == 0 || i == AREA_ROW_LEN - 1 || j == AREA_COLUMN_LEN - 1){
        gameArea[i][j] = CHAR_WALL;
        continue;
      }
      gameArea[i][j] = CHAR_SPACE;
    }
  }

  bool settingHead = true;
  Node *ptr = snake->getHead();
  
  while(ptr != NULL){
    if(settingHead == true){
      gameArea[ptr->posX][ptr->posY] = CHAR_SNAKE_HEAD;
      settingHead = false;
    } else{
      gameArea[ptr->posX][ptr->posY] = CHAR_SNAKE_TAIL;
    }
    ptr = ptr->next;
  }

  generateFood();
}

/*------------------------------------------------------------------------------*/
void GameArea::draw(Snake *const snake){
  system("clear");
  for(int i = 0; i < AREA_ROW_LEN; i++){
    for(int j = 0; j < AREA_COLUMN_LEN; j++){
      cout << gameArea[i][j];
    }
    cout << endl;
  }

  cout << "Score : " << snake->getScore() << endl;
}

/*------------------------------------------------------------------------------*/
int GameArea::check(int posX, int posY){
  if(gameArea[posX][posY] == CHAR_SPACE){
    return CHECK_SPACE;
  } else if(gameArea[posX][posY] == CHAR_FOOD){
    generateFood();
    return CHECK_FOOD;
  }

  return CHECK_OTHER;
}

/*------------------------------------------------------------------------------*/
void GameArea::set(int posX, int posY, char c){
  // posX or posY never overflow so do not need to check
  gameArea[posX][posY] = c;
}
