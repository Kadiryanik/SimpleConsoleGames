#include "Tetris.h"

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
static int score = 0;

/*------------------------------------------------------------------------------*/
#define CURSOR_DEFAULT_POS_X (AREA_WALL_LEN - 1)
#define CURSOR_DEFAULT_POS_Y ((AREA_COLUMN_LEN / 2) - AREA_WALL_LEN)

#define DELETE_ROW_FREQUENCY     300000 // 0.3s
#define GAMEOVER_BLINK_FREQUENCY 600000 // 0.3s
#define AGAIN_THOLD              3

/*------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC-FUNCTIONS---------------------------------*/
/*------------------------------------------------------------------------------*/
Tetris::Tetris(){
    /* initialize random seed: */
  srand (time(NULL));

  for(int i = 0; i < AREA_ROW_LEN; i++){
    for(int j = 0; j < AREA_COLUMN_LEN; j++){
      if(i < AREA_WALL_LEN || j < AREA_WALL_LEN || i >= AREA_ROW_LEN - AREA_WALL_LEN || j >= AREA_COLUMN_LEN - AREA_WALL_LEN){
        gameArea[i][j] = CHAR_WALL;
        continue;
      }
      gameArea[i][j] = CHAR_SPACE;
    }
  }

  // initilize cursor pointer
  cursor.x = CURSOR_DEFAULT_POS_X;
  cursor.y = CURSOR_DEFAULT_POS_Y;
}

/*------------------------------------------------------------------------------*/
Tetris::~Tetris(){
}

/*------------------------------------------------------------------------------*/
void Tetris::draw(){
  system("clear");
  for(int i = 0; i < AREA_ROW_LEN; i++){
    for(int j = 0; j < AREA_COLUMN_LEN; j++){
      cout << gameArea[i][j];
    }
    cout << endl;
  }

  cout << "Score : " << score << endl;
}

/*------------------------------------------------------------------------------*/
int Tetris::move(Shape *shape, char direction){
  int ret;
  if(check(shape, direction)){
    if(direction == CHAR_LEFT || direction == CHAR_DOWN || direction == CHAR_RIGHT){
      moveDirection(shape, direction);
      ret = RET_LANDING;
    } else if(direction == CHAR_ROTATE){
      shape->rotate();
      ret = RET_LANDING;
    } else{
      ret = RET_NOT_MOVED;
    }
  } else if(direction == CHAR_DOWN){ // trying to moving down and could't
    // check the top of shape for game over
    if(cursor.x == CURSOR_DEFAULT_POS_X){
      gameOver(shape);
      ret = RET_GAME_OVER;
    } else{
      // score each landed
      score++;

      // set shape as landed
      set(shape, CHAR_LANDED);
      ret = RET_LANDED;

      // check is any row full
      checkAndDeleteRow();

      // reset the cursor for new one
      cursor.x = CURSOR_DEFAULT_POS_X;
      cursor.y = CURSOR_DEFAULT_POS_Y;
    }
  } else{
    ret = RET_NOT_MOVED;
  }
  return ret;
}

/*------------------------------------------------------------------------------*/
void Tetris::generateShape(Shape *shape){
  if(shape != NULL) delete shape;

  int classId = rand() % SUPPORTED_SHAPE_NUM;

  switch(classId){
    case 0:
      shape = new L();
      break;
    case 1:
      shape = new MirroredL();
      break;
    case 2:
      shape = new S();
      break;
    case 3:
      shape = new Square();
      break;
    case 4:
      shape = new Stick();
      break;
    case 5:
      shape = new T();
      break;
    case 6:
      shape = new Z();
      break;
    default:
      break;
  }
}

/*------------------------------------------------------------------------------*/
/*---------------------------PRIVATE-FUNCTIONS----------------------------------*/
/*------------------------------------------------------------------------------*/
void Tetris::clear(Shape *shape){
  // cursor never overflow so do not need to check
  for(int i = cursor.x; i < cursor.x + shape->height; i++){
    for(int j = cursor.y; j < cursor.y + shape->width; j++){
      if(gameArea[i][j] == CHAR_LANDING){
        gameArea[i][j] = CHAR_SPACE;
      }
    }
  }
}

/*------------------------------------------------------------------------------*/
void Tetris::set(Shape *shape, char c){
  // cursor never overflow so do not need to check
  for(int i = 0; i < shape->height; i++){
    for(int j = 0; j < shape->width; j++){
      if(shape->matrix[i][j] != 0){
        gameArea[cursor.x + i][cursor.y + j] = c;
      }
    }
  }
}

/*------------------------------------------------------------------------------*/
bool Tetris::check(Shape *shape, char direction){
  int currentRotate = shape->currentMatrixIndex;
  pos_t newCursor = cursor;
  bool ret = true;
  
  if(direction == CHAR_LEFT){
    newCursor.y--;
  } else if(direction == CHAR_DOWN){
    newCursor.x++;
  } else if(direction == CHAR_RIGHT){
    newCursor.y++;
  } else if(direction == CHAR_ROTATE){
    shape->rotate();
  }

  for(int i = 0; i < shape->height; i++){
    for(int j = 0; j < shape->width; j++){
      char charOnArea = gameArea[newCursor.x + i][newCursor.y + j];
      int valOnMatrix = shape->matrix[i][j];

      if(valOnMatrix != 0 && (charOnArea == CHAR_LANDED || charOnArea == CHAR_WALL)){
        ret = false;
        break;
      }
    }
  }

  if(direction == CHAR_ROTATE){
    // rotate back to the old position
    while(currentRotate != shape->currentMatrixIndex){
      shape->rotate();
    }
  }

  return ret;
}

/*------------------------------------------------------------------------------*/
void Tetris::moveDirection(Shape *shape, char direction){
  clear(shape);
  if(direction == CHAR_LEFT){
    cursor.y--;
  } else if(direction == CHAR_DOWN){
    cursor.x++;
  } else if(direction == CHAR_RIGHT){
    cursor.y++;
  } // else didn't expect
  set(shape, CHAR_LANDING);
}

/*------------------------------------------------------------------------------*/
void Tetris::checkAndDeleteRow(){
  int rowIndexes[AREA_ROW_LEN];
  int rowIndexesCount = 0;

  // put into rowIndexes up to down
  for(int i = AREA_WALL_LEN; i < AREA_ROW_LEN - AREA_WALL_LEN; i++){
    bool rowFull = true;
    for(int j = AREA_WALL_LEN; j < AREA_COLUMN_LEN - AREA_WALL_LEN; j++){
      if(gameArea[i][j] != CHAR_LANDED){
        rowFull = false;
        break;
      }
    }
    if(rowFull){
      rowIndexes[rowIndexesCount++] = i;
    }
  }

  if(rowIndexesCount > 0){
    int againCounter = 0;
    while(againCounter < AGAIN_THOLD){
      for(int i = 0; i < rowIndexesCount; i++){
        for(int j = AREA_WALL_LEN; j < AREA_COLUMN_LEN - AREA_WALL_LEN; j++){
          gameArea[rowIndexes[i]][j] = CHAR_SPACE;
        }
      }
      draw();
      usleep(DELETE_ROW_FREQUENCY / 2);
      
      for(int i = 0; i < rowIndexesCount; i++){
        for(int j = AREA_WALL_LEN; j < AREA_COLUMN_LEN - AREA_WALL_LEN; j++){
          gameArea[rowIndexes[i]][j] = CHAR_LANDED;
        }
      }
      draw();
      usleep(DELETE_ROW_FREQUENCY);

      againCounter++;
    }

    score += (rowIndexesCount * 10);

    int newPos = AREA_ROW_LEN - AREA_WALL_LEN - 1;
    for(int i = AREA_ROW_LEN - AREA_WALL_LEN - 1; i > AREA_WALL_LEN; i--){
      // checking rowIndexes down to up
      if(rowIndexesCount > 0 && rowIndexes[rowIndexesCount - 1] == i){ // full row detected
        rowIndexesCount--;
        continue;
      }
      for(int j = AREA_WALL_LEN; j < AREA_COLUMN_LEN - AREA_WALL_LEN; j++){
        gameArea[newPos][j] = gameArea[i][j];
      }
      newPos--;
    }
    draw();
  }
}

/*------------------------------------------------------------------------------*/
void Tetris::gameOver(Shape *shape){
  clear(shape);
  set(shape, CHAR_LANDED);

  // fill the emptyGameArea
  char holder[AREA_ROW_LEN][AREA_COLUMN_LEN];
  for(int i = 0; i < AREA_ROW_LEN; i++){
    for(int j = 0; j < AREA_COLUMN_LEN; j++){
      holder[i][j] = gameArea[i][j];
    }
  }

  int againCounter = 0;
  while(againCounter < AGAIN_THOLD){
    // clear gameArea
    for(int i = 0; i < AREA_ROW_LEN; i++){
      for(int j = 0; j < AREA_COLUMN_LEN; j++){    
        if(gameArea[i][j] == CHAR_LANDED){
          gameArea[i][j] = CHAR_SPACE;
        }
      }
    }
    draw();
    usleep(GAMEOVER_BLINK_FREQUENCY);

    // set back the original
    for(int i = 0; i < AREA_ROW_LEN; i++){
      for(int j = 0; j < AREA_COLUMN_LEN; j++){
        gameArea[i][j] = holder[i][j];
      }
    }

    draw();
    usleep(GAMEOVER_BLINK_FREQUENCY / 2);
    againCounter++;
  }
}

/*------------------------------------------------------------------------------*/
