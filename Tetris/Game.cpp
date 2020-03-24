#include "Tetris.h"

/*------------------------------------------------------------------------------*/
#define KEY_INPUT_CHECK_FREQUENCY 5000 // 5ms
#define MOVING_DOWN_THOLD         60   // Per 300ms

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
char getLastPressed();
void printInfo();

/*------------------------------------------------------------------------------*/
int main(){
  Tetris *tetris;
  Shape *currentShape;
  int movingDownCounter = 0;
  char lastPressed;

  // print info and wait for pressing any key
  printInfo();
  do{
    lastPressed = getLastPressed();
    if(lastPressed == CHAR_QUIT){
      return 0;
    }
    usleep(KEY_INPUT_CHECK_FREQUENCY);
  } while(lastPressed == (char)0);
  // init and start game loop
  tetris = new Tetris();
  tetris->draw();

  currentShape = new Square();
  while(true){
    lastPressed = getLastPressed();
    if(lastPressed == CHAR_QUIT){
      return 0;
    } else if(lastPressed != CHAR_PAUSE){
      int ret;
      if(movingDownCounter > MOVING_DOWN_THOLD){
        // hack the pressed key
        lastPressed = CHAR_DOWN;

        movingDownCounter = 0;
      }
      ret = tetris->move(currentShape, lastPressed);

      if(ret != RET_NOT_MOVED){
        if(ret == RET_LANDING){
        } else if(ret == RET_LANDED){
          tetris->generateShape(currentShape);
        } else if(ret == RET_GAME_OVER){
          cout << "Game Over" << endl;
          break;
        }
        tetris->draw();
      } // else redraw is not necessary

      // increment the counter
      movingDownCounter++;
    } // else mean pause, do nothing

    usleep(KEY_INPUT_CHECK_FREQUENCY);
  }

  // clean up
  if(tetris) delete tetris;
  if(currentShape) delete currentShape;

  // all done
  return 0;
}

/*------------------------------------------------------------------------------*/
void printInfo(){
  system("clear");
  cout << "*******************************" << endl;
  cout << "*******************************" << endl;
  cout << "**     Tetris Game v0.1      **" << endl;
  cout << "*******************************" << endl;
  cout << "**      " << CHAR_LEFT << " " << CHAR_DOWN << " " << CHAR_RIGHT << "   Movement     **" << endl;
  cout << "**        " << CHAR_ROTATE << "      Rotate      **" << endl;
  cout << "**        " << CHAR_PAUSE << "      Pause       **" << endl;
  cout << "**        " << CHAR_QUIT << "      Quit!       **" << endl;
  cout << "**  Press any key to start!  **" << endl;
  cout << "*******************************" << endl;
  cout << "*******************************" << endl;
}