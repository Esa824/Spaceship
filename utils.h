#ifndef UTILS_H
#define UTILS_H

#include "menu_screens.h"
#include "structs.h"
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <json-c/json.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <curses.h>
#include <atomic>
#include <vector>

using namespace std;

extern vector<string> spaceship;
extern int health_bar;
extern int enemiesKilled;
extern bool speedUpPowerUpActive;
extern std::chrono::time_point<std::chrono::high_resolution_clock> startTimeOfSpeedUpPowerUp;
extern atomic<int> bullet_x;
extern atomic<int> bullet_y;
extern const char* enemyArt[];
extern const char* explosion[];

void moveSpaceship(int* x, int* y, char where, struct LevelData& levelData);

void handleBullet();

void spawnStars(int num_stars);

void spawnEnemies(int noe);

LevelData getLevelData(int levelNumber);

void spaceShipGame(int Level);

void set_controls(struct Controls controls);

struct Controls get_controls();

#endif
