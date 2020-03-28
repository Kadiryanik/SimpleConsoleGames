#include <iostream>
#include <cstdio>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <stropts.h>

#include "GameArea.h"

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
static char lastPressed = (char)0;

/*------------------------------------------------------------------------------*/
static int keyboadHit() {
  static const int STDIN = 0;
  static bool initialized = false;

  if (! initialized) {
    // Use termios to turn off line buffering
    termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);
    initialized = true;
  }

  int bytesWaiting;
  ioctl(STDIN, FIONREAD, &bytesWaiting);
  return bytesWaiting;
}

/*------------------------------------------------------------------------------*/
char getLastPressed(){
  if(keyboadHit()){
    cin >> lastPressed;

    // to upper case for control characters
    lastPressed = toupper(lastPressed);
  }

  // hold val
  char temp = lastPressed;

  if(lastPressed != CHAR_PAUSE){
    // if not pause clean up last pressed
    lastPressed = (char)0;
  }

  return temp;
}
