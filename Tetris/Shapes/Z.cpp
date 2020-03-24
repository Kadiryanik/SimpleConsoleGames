#include "Z.h"

#define BIGGEST_EDGE    3
#define TOTAL_SHAPE_NUM 2

const int shapes[BIGGEST_EDGE * TOTAL_SHAPE_NUM][BIGGEST_EDGE] = {
  { 1, 1, 0 },
  { 0, 1, 1 },
  { 0, 0, 0 },

  { 0, 1, 0 },
  { 1, 1, 0 },
  { 1, 0, 0 }
};

/*------------------------------------------------------------------------------*/
Z::Z() : Shape(BIGGEST_EDGE, BIGGEST_EDGE){
  // init first shape as matrix
  for(int i = 0; i < BIGGEST_EDGE; i++){
    for(int j = 0; j < BIGGEST_EDGE; j++){
      matrix[i][j] = shapes[(currentMatrixIndex * BIGGEST_EDGE) + i][j];
    }
  }
}

/*------------------------------------------------------------------------------*/
Z::~Z(){
  for( int i = 0 ; i < height; i++){
      delete[] matrix[i];
  }
  // delete actual matrix
  delete[] matrix;
}

/*------------------------------------------------------------------------------*/
void Z::rotate(){
  currentMatrixIndex = (++currentMatrixIndex) % TOTAL_SHAPE_NUM;

  for(int i = 0; i < BIGGEST_EDGE; i++){
    for(int j = 0; j < BIGGEST_EDGE; j++){
      matrix[i][j] = shapes[(currentMatrixIndex * BIGGEST_EDGE) + i][j];
    }
  }
}

/*------------------------------------------------------------------------------*/
