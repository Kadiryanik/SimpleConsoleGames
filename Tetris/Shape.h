#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <stdlib.h>

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
#define SUPPORTED_SHAPE_NUM 7

/*------------------------------------------------------------------------------*/
class Shape{
  public:
    int height;
    int width;
    int **matrix;
    int currentMatrixIndex;
    Shape(int height, int width){
      this->height = height;
      this->width = width;
      this->currentMatrixIndex = 0;

      matrix = new int*[height];
      for(int i = 0; i < height; i++){
        matrix[i] = new int[width];
      }
    }
    virtual void rotate() {
    }
};

/*------------------------------------------------------------------------------*/
#endif /* SHAPE_H */
