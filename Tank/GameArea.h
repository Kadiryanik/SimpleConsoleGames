#ifndef GAME_AREA_H
#define GAME_AREA_H

#include <iostream>
#include <stdlib.h>

#include "Tank.h"

/*------------------------------------------------------------------------------*/
#define AREA_ROW_LEN    40
#define AREA_COLUMN_LEN 61

/*------------------------------------------------------------------------------*/
#define CHAR_SPACE  ' '
#define CHAR_PAUSE  'P'
#define CHAR_QUIT   'Q'

/*------------------------------------------------------------------------------*/
class GameArea{
    bool checkMovement(Tank *tank);
    bool checkBulletHit(Bullet *bullet);
    void checkAndSetBullet(Bullet *bullet);
    void findAndDeleteBullets(Bullet *pass, coordinate_t coordinate);
    Tank* findEnemy(coordinate_t coordinate);
    void clear(Tank *tank, bool lastPosition);
  public:
    GameArea(bool multiplayer);
    ~GameArea();
    void draw();
    void createEnemy();
    bool moveEnemies();
    bool controlPlayers(bool multiplayer, char control, bool invokeDraw);
    bool control(Tank *tank, char control, bool invokeDraw);
    void updateBullets();
    void set(Tank *tank);
    void gameover();
};

#endif /* GAME_AREA_H */
