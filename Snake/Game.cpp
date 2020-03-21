#include <unistd.h>

#include "GameArea.h"

/*------------------------------------------------------------------------------*/
#define DRAW_FREQUENCY 50000 // 50ms

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
char getLastPressed();
void printInfo();

/*------------------------------------------------------------------------------*/
int main(){
  GameArea *gameArea = new GameArea();
  Snake *snake = new Snake(gameArea);
  char lastPressed;

  // print info and wait for pressing any key
  printInfo();
  while(getLastPressed() == (char)0);

  // init and start game loop
  gameArea->init(snake);
  while(true){
    lastPressed = getLastPressed();
    if(lastPressed != CHAR_PAUSE){
      gameArea->draw(snake);
      if(!snake->move(lastPressed)){
        // move fails or pressed exit character
        break;
      }
    }
    usleep(DRAW_FREQUENCY);
  }

  // clean up
  delete gameArea;
  delete snake;

  // all done
  return 0;
}

/*------------------------------------------------------------------------------*/
void printInfo(){
  system("clear");
  cout << "*******************************" << endl;
  cout << "*******************************" << endl;
  cout << "**      Snake Game v0.1      **" << endl;
  cout << "*******************************" << endl;
  cout << "**        " << CHAR_UP << "       For        **" << endl;
  cout << "**      " << CHAR_LEFT << " " << CHAR_DOWN << " " << CHAR_RIGHT << "   Movement     **" << endl;
  cout << "**        " << CHAR_PAUSE << "      Pause       **" << endl;
  cout << "**        " << CHAR_EXIT << "      Quit!       **" << endl;
  cout << "**  Press any key to start!  **" << endl;
  cout << "*******************************" << endl;
  cout << "*******************************" << endl;
}