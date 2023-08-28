#ifndef MENU_SCREENS_H
#define MENU_SCREENS_H

#include "structs.h"

void display_death_menu();
void set_controls(struct Controls controls);
struct Controls get_controls();
void display_controls();
void display_levels();
void main_menu();
void spaceShipGame();
struct LevelData getLevelData(int levelNumber);
void spawnEnemies(int noe);
void spawnStars(int num_stars);
void handleBullet();
void moveSpaceship(int *x, int *y, char where, struct LevelData &levelData);

#endif // MENU_SCREENS_H
