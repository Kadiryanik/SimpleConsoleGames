#include <unistd.h>
#include <string.h>

#include "GameArea.h"

/*------------------------------------------------------------------------------*/
#define KEY_INPUT_CHECK_FREQUENCY 6000 // 6ms
#define UPDATE_BULLETS_THOLD      10   // 60ms
#define MOVE_ENEMIES_THOLD        20   // 120ms
#define CREATE_ENEMY_THOLD        150  // (6 * 150)ms

#define MAX_TANK_ON_MAP           15

/*------------------------------------------------------------------------------*/
#define CHAR_SINGLE_PLAYER '1'
#define CHAR_MULTI_PLAYER  '2'

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
extern Tank *tankHead;

/*------------------------------------------------------------------------------*/
char getLastPressed();
void printInfo();

/*------------------------------------------------------------------------------*/
int main(){
  GameArea *gameArea;
  char lastPressed;
  bool multiplayer = false;
  int createEnemiesCount = 0;
  int moveEnemiesCount = 0;
  int updateBulletsCount = 0;

  // print info and wait for pressing any key
  printInfo();
  do{
    lastPressed = getLastPressed();
    if(lastPressed == CHAR_QUIT){
      return 0;
    } else if(lastPressed == CHAR_SINGLE_PLAYER){
      multiplayer = false;
      break;
    } else if(lastPressed == CHAR_MULTI_PLAYER){
      multiplayer = true;
      break;
    }
    usleep(KEY_INPUT_CHECK_FREQUENCY);
  } while(true);

  // init and start game loop
  gameArea = new GameArea(multiplayer);

  if(gameArea != NULL){
    gameArea->draw();

    while(true){
      lastPressed = getLastPressed();
      if(lastPressed == CHAR_QUIT){
        break; // clean up dynamic memory
      } else if(lastPressed != CHAR_PAUSE){
        bool invokeDraw = false;

        if(createEnemiesCount > CREATE_ENEMY_THOLD){
          if(tankHead->tankCount < MAX_TANK_ON_MAP){
            gameArea->createEnemy();
            invokeDraw = true;
          }

          createEnemiesCount = 0;
        }
        if(moveEnemiesCount > MOVE_ENEMIES_THOLD){
          gameArea->moveEnemies();

          moveEnemiesCount = 0;
          invokeDraw = true;
        }
        if(updateBulletsCount > UPDATE_BULLETS_THOLD){
          gameArea->updateBullets();

          updateBulletsCount = 0;
          invokeDraw = true;
        }

        invokeDraw = gameArea->controlPlayers(multiplayer, lastPressed, invokeDraw);

        if(invokeDraw){
          gameArea->draw();
        }

        createEnemiesCount++;
        moveEnemiesCount++;
        updateBulletsCount++;
      } // else mean pause, do nothing

      usleep(KEY_INPUT_CHECK_FREQUENCY);
    }

    // clean up
    if(gameArea) delete gameArea;
  } else{
    cout << "GameArea allocation failed!" << endl;
  }

  return 0;
}

/*------------------------------------------------------------------------------*/
void printInfo(){
  system("clear");
  cout << "*************************************" << endl;
  cout << "*************************************" << endl;
  cout << "**         Tank Game v0.1          **" << endl;
  cout << "*************************************" << endl;
  cout << "**      " << P1_CHAR_UP << "            For P1        **" << endl;
  cout << "**    " << P1_CHAR_LEFT << " " << P1_CHAR_DOWN << " " << 
    P1_CHAR_RIGHT << " [" << P1_CHAR_FIRE << "]   Movement [Fire]  **" << endl;
  cout << "**---------------------------------**" << endl;

  cout << "**      " << P2_CHAR_UP << "            For P2        **" << endl;
  cout << "**    " << P2_CHAR_LEFT << " " << P2_CHAR_DOWN << " " << 
    P2_CHAR_RIGHT << " [" << P2_CHAR_FIRE << "]   Movement [Fire]  **" << endl;

  cout << "**---------------------------------**" << endl;
  cout << "**      " << CHAR_PAUSE << "            Pause         **" << endl;
  cout << "**      " << CHAR_QUIT << "            Quit!         **" << endl;
  cout << "**---------------------------------**" << endl;
  cout << "**  Press " << CHAR_SINGLE_PLAYER << " to start single player **" 
    << endl;
  cout << "**  Press " << CHAR_MULTI_PLAYER << " to start multi player  **" 
    << endl;
  cout << "*************************************" << endl;
  cout << "*************************************" << endl;
}
