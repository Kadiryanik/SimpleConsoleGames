#include "S.h"

#define BIGGEST_EDGE    3
#define TOTAL_SHAPE_NUM 2

#define CC_CHAR_SHAPE   CHAR_SPACE
#define CC_COLOR_SHAPE  COLOR_FG_GREEN

const int shapes[BIGGEST_EDGE * TOTAL_SHAPE_NUM][BIGGEST_EDGE] = {
  { 0, 1, 1 },
  { 1, 1, 0 },
  { 0, 0, 0 },

  { 1, 0, 0 },
  { 1, 1, 0 },
  { 0, 1, 0 }
};

/*------------------------------------------------------------------------------*/
S::S() : Shape(BIGGEST_EDGE, BIGGEST_EDGE, CC_CHAR_SHAPE, CC_COLOR_SHAPE){
  // init first shape as matrix
  for(int i = 0; i < BIGGEST_EDGE; i++){
    for(int j = 0; j < BIGGEST_EDGE; j++){
      matrix[i][j] = shapes[(currentMatrixIndex * BIGGEST_EDGE) + i][j];
    }
  }
}

/*------------------------------------------------------------------------------*/
S::~S(){
  for( int i = 0 ; i < height; i++){
      delete[] matrix[i];
  }
  // delete actual matrix
  delete[] matrix;
}

/*------------------------------------------------------------------------------*/
void S::rotate(){
  currentMatrixIndex = (currentMatrixIndex + 1) % TOTAL_SHAPE_NUM;

  for(int i = 0; i < BIGGEST_EDGE; i++){
    for(int j = 0; j < BIGGEST_EDGE; j++){
      matrix[i][j] = shapes[(currentMatrixIndex * BIGGEST_EDGE) + i][j];
    }
  }
}

/*------------------------------------------------------------------------------*/
