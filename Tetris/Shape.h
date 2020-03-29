#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <stdlib.h>

#include "CharColor.h"

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
#define SUPPORTED_SHAPE_NUM 7

// TODO: fix this defination
#define CHAR_SPACE          ' '

/*------------------------------------------------------------------------------*/
class Shape{
  public:
    int height;
    int width;
    charColor_t charColor;
    int **matrix;
    int currentMatrixIndex;

    Shape(int height, int width, char c, int color, int index = 0){
      this->height = height;
      this->width = width;
      this->charColor.c = c;
      this->charColor.color = color;
      this->currentMatrixIndex = index;

      matrix = new int*[height];
      for(int i = 0; i < height; i++){
        matrix[i] = new int[width];
      }
    }
    virtual ~Shape(){
    }
    virtual void rotate(){
    }
};

/*------------------------------------------------------------------------------*/
#endif /* SHAPE_H */
