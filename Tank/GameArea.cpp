#include <iomanip>
#include "GameArea.h"

/*------------------------------------------------------------------------------*/
using namespace std;

/*------------------------------------------------------------------------------*/
#define MAP_WALL    'X'
#define MAP_BRICK_1 '7'
#define MAP_BRICK_2 '8'
#define MAP_BRICK_3 '9'
#define MAP_BIRD    'B'

#define CHAR_BIRD   '$'

/*------------------------------------------------------------------------------*/
extern Tank *tankHead;

/*------------------------------------------------------------------------------*/
Tank *p1 = NULL, *p2 = NULL;

/*------------------------------------------------------------------------------*/
static char gameArea[AREA_ROW_LEN][AREA_COLUMN_LEN] = {
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
  "X                                                          X",
  "X                                                          X",
  "X                                                          X",
  "X   777   888   777   888   9999   888   777   888   777   X",
  "X   777   888   777   888   9999   888   777   888   777   X",
  "X                                                          X",
  "X                                                          X",
  "X                                                          X",
  "X                                                          X",
  "X   888   777   888   777   9999   777   888   777   888   X",
  "X   888   777   888   777   9999   777   888   777   888   X",
  "X                                                          X",
  "X                                                          X",
  "X                                                          X",
  "XXXX   777   888   777888   XXXX   777888   777   888   XXXX",
  "XXXX   777   888   777888   XXXX   777888   777   888   XXXX",
  "XXXX   888   777   888   888    777   777   888   777   XXXX",
  "XXXX   888   777   888   888    777   777   888   777   XXXX",
  "XXXX   777   888   777     777888     888   777   888   XXXX",
  "XXXX   777   888   777     777888     888   777   888   XXXX",
  "X                                                          X",
  "X                                                          X",
  "X                                                          X",
  "X   888   777   888   777          777   888   777   888   X",
  "X   888   777   888   777777    777777   888   777   888   X",
  "X   777   888   777   888   8888   888   777   888   777   X",
  "X   777   888   777   888   8888   888   777   888   777   X",
  "X   888   777   888   777777    777777   888   777   888   X",
  "X   888   777   888   777          777   888   777   888   X",
  "X                                                          X",
  "X   777   888   777   888          888   777   888   777   X",
  "X   777   888   777   888          888   777   888   777   X",
  "X                            99                            X",
  "X   888   777   888   777   9999   777   888   777   888   X",
  "X   888   777   888   777  99  99  777   888   777   888   X",
  "X                          999999                          X",
  "X                          99BB99                          X",
  "X                          99BB99                          X",
  "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

/*------------------------------------------------------------------------------*/
static charColor_t getCharColor(int i, int j){
  charColor_t ret;
  switch(gameArea[i][j]){
    case MAP_WALL:
      ret.c = CHAR_SPACE;
      ret.color = COLOR_FG_WHITE;
      break;
    case MAP_BRICK_1:
    case ENEMY_BODY_1_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = COLOR_FG_CYAN;
      break;
    case MAP_BRICK_2:
    case ENEMY_BODY_2_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = COLOR_FG_BLUE;
      break;
    case MAP_BRICK_3:
    case ENEMY_BODY_3_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = COLOR_FG_PURPLE;
      break;
    case MAP_BIRD:
      ret.c = CHAR_BIRD;
      ret.color = COLOR_FG_RED;
      break;
    case DEFAULT_PLAYER_BULLET_CHAR:
      ret.c = DEFAULT_BULLET_CHAR;
      ret.color = COLOR_BG_GREEN;
      break;
    case DEFAULT_ENEMY_BULLET_CHAR:
      ret.c = DEFAULT_BULLET_CHAR;
      ret.color = COLOR_BG_RED;
      break;
    case P1_DEFAULT_HEAD_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = P1_DEFAULT_HEAD_COLOR;
      break;
    case P1_DEFAULT_BODY_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = P1_DEFAULT_BODY_COLOR;
      break;
    case P2_DEFAULT_HEAD_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = P2_DEFAULT_HEAD_COLOR;
      break;
    case P2_DEFAULT_BODY_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = P2_DEFAULT_BODY_COLOR;
      break;
    case ENEMY_DEFAULT_HEAD_CHAR:
      ret.c = CHAR_SPACE;
      ret.color = ENEMY_DEFAULT_HEAD_COLOR;
      break;
    default:
      ret.c = CHAR_SPACE;
      ret.color = COLOR_NONE;
      break;
  }

  return ret;
}

/*------------------------------------------------------------------------------*/
/*-----------------------------PUBLIC-FUNCTIONS---------------------------------*/
/*------------------------------------------------------------------------------*/
GameArea::GameArea(bool multiplayer){
  char controlMapP1[TOTAL_CONTROL_NUM] = { P1_CHAR_UP, P1_CHAR_LEFT, 
    P1_CHAR_RIGHT, P1_CHAR_DOWN, P1_CHAR_FIRE };

  p1 = new Tank(P1_DEFAULT_COORDINATE_X, P1_DEFAULT_COORDINATE_Y,
              DEFAULT_HEALTH_START, P1_DEFAULT_HEAD_CHAR, P1_DEFAULT_BODY_CHAR, 
              controlMapP1, true);

  if(multiplayer){
    char controlMapP2[TOTAL_CONTROL_NUM] = { P2_CHAR_UP, P2_CHAR_LEFT, 
      P2_CHAR_RIGHT, P2_CHAR_DOWN, P2_CHAR_FIRE };
    p2 = new Tank(P2_DEFAULT_COORDINATE_X, P2_DEFAULT_COORDINATE_Y, 
                DEFAULT_HEALTH_START, P2_DEFAULT_HEAD_CHAR, P2_DEFAULT_BODY_CHAR,
                controlMapP2, true);
  }

  /* initialize random seed: */
  srand (time(NULL));

  // set on area
  if(p1 != NULL){
    set(p1);
  }
  if(p2 != NULL){
    set(p2);
  }
}

/*------------------------------------------------------------------------------*/
GameArea::~GameArea(){
  gameover();
}

/*------------------------------------------------------------------------------*/
static void printColorfull(charColor_t cc){
  cout << "\033[;" << cc.color << "m" << cc.c << "\033[0m";
}

/*------------------------------------------------------------------------------*/
void GameArea::draw(){
  system("clear");
  for(int i = 0; i < AREA_ROW_LEN; i++){
    for(int j = 0; j < AREA_COLUMN_LEN; j++){
      charColor_t cc = getCharColor(i, j);
      if(cc.color == COLOR_NONE){
        cout << cc.c;
      } else{
        printColorfull(cc);
      }
    }
    cout << endl;
  }

  cout << "\033[1m";
  cout << setw(10) << "Health" << setw(10) << "Score" << endl;
  if(p1 != NULL){
    cout << "P1 " << setw(5) << p1->health << setw(10) << p1->score << endl;
  }
  if(p2 != NULL){
    cout << "P2 " << setw(5) << p2->health << setw(10) << p2->score << endl;
  }
  cout << "\033[0m";
}

/*------------------------------------------------------------------------------*/
static bool checkArea(coordinate_t coordinate){
  for(int i = coordinate.x; i < coordinate.x + TANK_HEIGHT; i++){
    for(int j = coordinate.y; j < coordinate.y + TANK_WIDTH; j++){
      if(gameArea[i][j] != CHAR_SPACE){
        return false;
      }
    }
  }
  return true;
}

/*------------------------------------------------------------------------------*/
void GameArea::createEnemy(){
  int retry = 30;
  int startPoint = 1;
  int toPoint = AREA_ROW_LEN / 2;

  // every fifth enemy's create position on second half 
  if(tankHead->tankCount % 5 == 0){
    startPoint = AREA_ROW_LEN / 2;
    toPoint = AREA_ROW_LEN - 1;
  }

  while(retry > 0){
    coordinate_t coordinate;
    coordinate.x = startPoint + (rand() % toPoint); // first half 
    coordinate.y = 1 + (rand() % (AREA_COLUMN_LEN - 1)); 
    if(checkArea(coordinate)){
      // create new tank, auto links
      set(new Tank(coordinate.x, coordinate.y, \
        1 + rand() % DEFAULT_HEALTH_START));

      // create enemy succeed
      return;
    }

    retry--;
  }
}

/*------------------------------------------------------------------------------*/
bool GameArea::moveEnemies(){
  bool invokeDraw = false;
  Tank *tank = tankHead;

  char controlMap[TOTAL_CONTROL_NUM] = { ENEMY_CHAR_UP, ENEMY_CHAR_LEFT, \
    ENEMY_CHAR_RIGHT, ENEMY_CHAR_DOWN, ENEMY_CHAR_FIRE };

  while(tank != NULL){
    if(!(tank->isPlayer)){
      invokeDraw = control(tank, \
        controlMap[rand() % TOTAL_CONTROL_NUM], invokeDraw);
    }

    tank = tank->next;
  }

  return invokeDraw;
}

/*------------------------------------------------------------------------------*/
bool GameArea::controlPlayers(bool multiplayer, char control, bool invokeDraw){
  if(p1 != NULL && !(p1->deleteAfterBulletHit)){
    invokeDraw = this->control(p1, control, invokeDraw);
  }
  if(p2 != NULL && !(p2->deleteAfterBulletHit)){
    invokeDraw = this->control(p2, control, invokeDraw);
  }

  return invokeDraw;
}

/*------------------------------------------------------------------------------*/
bool GameArea::control(Tank *tank, char control, bool invokeDraw){
  switch(tank->control(control)){
    case TANK_CONTROL_RET_NONE:
      break;
    case TANK_CONTROL_RET_MOVED:
      if(checkMovement(tank)){
        clear(tank, true);
        set(tank);
        invokeDraw = true;
      } else{
        tank->resetCoordinate();
      }
      break;
    case TANK_CONTROL_RET_ROTATED:
      set(tank);
      invokeDraw = true;
      break;
    case TANK_CONTROL_RET_FIRED:
      checkAndSetBullet(tank->fire());
      invokeDraw = true;
      break;
    default:
      break;
  }

  return invokeDraw;
}

/*------------------------------------------------------------------------------*/
void GameArea::updateBullets(){
  Tank *tank = tankHead;

  // each tank
  while(tank != NULL){
    // each bullet
    Bullet *bullet = tank->bulletHead;
    while(bullet != NULL){
      // clear current coordinate
      coordinate_t coor = bullet->coordinate;
      gameArea[coor.x][coor.y] = CHAR_SPACE;

      // get updated info
      charCoor_t cCoor = bullet->updateCoor();

      Bullet *next = bullet->next;
      if(!checkBulletHit(bullet)){
        // update bullet coordinate on gameArea
        gameArea[cCoor.x][cCoor.y] = cCoor.c;
        bullet = bullet->next;
      } else{
        bullet = next;
      }
    }

    Tank *next = tank->next;
    if(tank->deleteAfterBulletHit && tank->bulletHead == NULL){
      // this way continue to print score
      if(tank != p1 && tank != p2){
        delete tank;
      }

    }
    tank = next;
  }
}

/*------------------------------------------------------------------------------*/
void GameArea::set(Tank *tank){
  for(int i = 0; i < TANK_HEIGHT; i++){
    for(int j = 0; j < TANK_WIDTH; j++){
      if(tank->matrix[i][j] == TANK_HEAD_CHAR){
        gameArea[tank->coordinate.x + i][tank->coordinate.y + j] = tank->headChar;
      } else{
        gameArea[tank->coordinate.x + i][tank->coordinate.y + j] = tank->bodyChar;
      }
    }
  }
}

/*------------------------------------------------------------------------------*/
void GameArea::gameover(){
  Tank *tank = tankHead;

  // each tank
  while(tank != NULL){
    Tank *next = tank->next;
    delete tank;
    tank = next;
  }

  cout << endl << "\033[1;" << COLOR_BG_RED << "mGameOver" << "\033[0m" << endl;
  exit(0);
}

/*------------------------------------------------------------------------------*/
/*----------------------------PRIVATE-FUNCTIONS---------------------------------*/
/*------------------------------------------------------------------------------*/
bool GameArea::checkMovement(Tank *tank){
  if(tank->direction == DIRECTION_UP){
    for(int j = tank->coordinate.y; j < tank->coordinate.y + TANK_WIDTH; j++){
      if(gameArea[tank->coordinate.x][j] != CHAR_SPACE){
        return false;
      }
    }
  } else if(tank->direction == DIRECTION_LEFT){
    for(int i = tank->coordinate.x; i < tank->coordinate.x + TANK_HEIGHT; i++){
      if(gameArea[i][tank->coordinate.y] != CHAR_SPACE){
        return false;
      }
    }
  } else if(tank->direction == DIRECTION_RIGHT){
    for(int i = tank->coordinate.x; i < tank->coordinate.x + TANK_HEIGHT; i++){
      if(gameArea[i][tank->lastCoordinate.y + TANK_WIDTH] != CHAR_SPACE){
        return false;
      }
    }
  } else if(tank->direction == DIRECTION_DOWN){
    for(int j = tank->coordinate.y; j < tank->coordinate.y + TANK_WIDTH; j++){
      if(gameArea[tank->lastCoordinate.x + TANK_HEIGHT][j] != CHAR_SPACE){
        return false;
      }
    }
  } // DIRECTION_NONE didnt expect

  return true;
}

/*------------------------------------------------------------------------------*/
static bool deleteBullet(Tank *tank, Bullet *pass, coordinate_t coordinate){
  Bullet *ptr = tank->bulletHead;
  while(ptr != NULL){
    if(ptr != pass && ptr->coordinate.x == coordinate.x 
                    && ptr->coordinate.y == coordinate.y){
      // clear other bullet's char and memory
      gameArea[coordinate.x][coordinate.y] = CHAR_SPACE;
      delete ptr;
      return true;
    }
   
    ptr = ptr->next;
  }

  return false;
}

/*------------------------------------------------------------------------------*/
bool GameArea::checkBulletHit(Bullet *bullet){
  bool bulletHit = true;

  switch(gameArea[bullet->coordinate.x][bullet->coordinate.y]){
    case MAP_WALL:
      // just delete bullet
      break;
    case MAP_BRICK_1:
      gameArea[bullet->coordinate.x][bullet->coordinate.y] = CHAR_SPACE;
      break;
    case MAP_BRICK_2:
      gameArea[bullet->coordinate.x][bullet->coordinate.y] = MAP_BRICK_1;
      break;
    case MAP_BRICK_3:
      gameArea[bullet->coordinate.x][bullet->coordinate.y] = MAP_BRICK_2;
      break;
    case MAP_BIRD:
      gameover();
      break;
    case DEFAULT_PLAYER_BULLET_CHAR:
    case DEFAULT_ENEMY_BULLET_CHAR:
      // TODO: find and delete other bullet
      findAndDeleteBullets(bullet, bullet->coordinate);
      break;
    case P1_DEFAULT_HEAD_CHAR:
    case P1_DEFAULT_BODY_CHAR:
      if((FRIEND_FIRE || bullet->c == DEFAULT_ENEMY_BULLET_CHAR) && \
          p1 != NULL && p1->reduceHealth()){
        if(p2 == NULL || p2->deleteAfterBulletHit){
          gameover();
        } else{
          clear(p1, false);
          p1->deleteAfterBulletHit = true;

          // updateBullets will delete tank when bullets hit
        }
      }
      break;
    case P2_DEFAULT_BODY_CHAR:
    case P2_DEFAULT_HEAD_CHAR:
      if((FRIEND_FIRE || bullet->c == DEFAULT_ENEMY_BULLET_CHAR) && \
          p2 != NULL && p2->reduceHealth()){
        if(p1 == NULL || p1->deleteAfterBulletHit){
          gameover();
        } else{
          clear(p2, false);
          p2->deleteAfterBulletHit = true;

          // updateBullets will delete tank when bullets hit
        }
      }
      break;
    case ENEMY_BODY_1_CHAR:
    case ENEMY_BODY_2_CHAR:
    case ENEMY_BODY_3_CHAR:
    case ENEMY_DEFAULT_HEAD_CHAR:
      if(FRIEND_FIRE || bullet->c == DEFAULT_PLAYER_BULLET_CHAR){
        Tank *enemy = findEnemy(bullet->coordinate);
        if(enemy != NULL){
          if(enemy->reduceHealth()){
            enemy->lastCoordinate = enemy->coordinate;
            clear(enemy, true);
            if(enemy->bulletHead == NULL){
              delete enemy;
            } else{
              enemy->deleteAfterBulletHit = true;
            }
          } else{
            // set for char update
            set(enemy);
          }

          // deactive zombie score
          if(!(bullet->owner->deleteAfterBulletHit)){
            bullet->owner->updateScore();
          }
        } // else didin't expect
      }
      break;
    default:
      bulletHit = false;
      break;
  }

  if(bulletHit){
    delete bullet;
  }

  return bulletHit;
}

/*------------------------------------------------------------------------------*/
void GameArea::checkAndSetBullet(Bullet *bullet){
  if(!checkBulletHit(bullet)){
    charCoor_t cCoor = bullet->getCoor();
    gameArea[cCoor.x][cCoor.y] = cCoor.c;
  }
}

/*------------------------------------------------------------------------------*/
void GameArea::findAndDeleteBullets(Bullet *pass, coordinate_t coordinate){
  Tank *tank = tankHead;
  while(tank != NULL){
    if(deleteBullet(tank, pass, coordinate)){
      // bullet deleted
      return;
    }
    tank = tank->next;
  }
}

/*------------------------------------------------------------------------------*/
Tank* GameArea::findEnemy(coordinate_t coordinate){
  Tank *tank = tankHead;
  while(tank != NULL){
    if(tank->coordinate.x <= coordinate.x &&
        coordinate.x < tank->coordinate.x + TANK_HEIGHT &&
        tank->coordinate.y <= coordinate.y &&
        coordinate.y < tank->coordinate.y + TANK_WIDTH){
      return tank;
    }
    tank = tank->next;
  }
  return NULL;
}

/*------------------------------------------------------------------------------*/
void GameArea::clear(Tank *tank, bool lastPosition){
  int x = tank->coordinate.x;
  int y = tank->coordinate.y;

  if(lastPosition){
    x = tank->lastCoordinate.x;
    y = tank->lastCoordinate.y;
  }

  for(int i = x; i < x + TANK_HEIGHT; i++){
    for(int j = y; j < y + TANK_WIDTH; j++){
      gameArea[i][j] = CHAR_SPACE;
    }
  }
}
