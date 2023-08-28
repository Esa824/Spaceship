#include "menu_screens.h"
#include <atomic>
#include <chrono>
#include <curses.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <json-c/json.h>
#include <ncurses.h>
#include <string.h>
#include <thread>
#include <unistd.h>

using namespace std;

vector<string> spaceship = {"__", "| \\", "=[_|H)--._____", "=[+--,-------'",
                            "[|_/\"\""};
static int health_bar = 1;
static int enemiesKilled = 0;
static bool speedUpPowerUpActive = false;
static std::chrono::time_point<std::chrono::high_resolution_clock>
    startTimeOfSpeedUpPowerUp;
const char *enemyArt[] = {"  ^^^", "{(-+-)}", "{(-+-)}", "{(-+-)}", "  ***"};
atomic<int> bullet_x(-1); // Atomic variable to store bullet's x-coordinate
atomic<int> bullet_y(-1); // Atomic variable to store bullet's y-coordinate
const char *explosion[] = {"* * * *", "* * * *", "* * * *", "* * * *"};

void moveSpaceship(int *x, int *y, char where, struct LevelData &levelData) {
  switch (where) {
  case 'w':
    (*y)--;
    if (speedUpPowerUpActive) {
      (*y)--;
    }
    break;
  case 's':
    (*y)++;
    if (speedUpPowerUpActive) {
      (*y)++;
    }
    break;
  case 'a':
    (*x)--;
    if (speedUpPowerUpActive) {
      (*x)--;
    }
    break;
  case 'd':
    (*x)++;
    if (speedUpPowerUpActive) {
      (*x)++;
    }
    break;
  case ' ':                  // Spacebar
    bullet_y.store(*y + 3);  // Store bullet's initial y-coordinate
    bullet_x.store(*x + 13); // Store bullet's initial x-coordinate
    break;
  case 'm':
    if (levelData.healthUpPowerUp != 0) {
      levelData.healthUpPowerUp--;
      if (health_bar != 1) {
        health_bar--;
      }
    }
  case 'n':
    if (levelData.speedUpPowerUp != 0) {
      levelData.speedUpPowerUp--;
      speedUpPowerUpActive = true;
      startTimeOfSpeedUpPowerUp = chrono::high_resolution_clock::now();
    }
  }
}

void handleBullet() {
  while (1) {
    int x = bullet_x.load();
    int y = bullet_y.load();

    if (x >= 0 && y >= 0) {
      attron(COLOR_PAIR(3));
      mvprintw(y, x, "-"); // Print bullet at specified coordinates
      attroff(COLOR_PAIR(3));
      refresh();
      this_thread::sleep_for(chrono::milliseconds(30)); // Adjust speed
      mvprintw(y, x, " ");                              // Erase bullet
      refresh();
      x++;
      bullet_x.store(x);

      if (x >= COLS) {
        bullet_x.store(-1);
        bullet_y.store(-1);
      }
    }
  }
}

void spawnStars(int num_stars) {
  srand(time(NULL));

  std::vector<Star> stars;

  while (true) {
    clear();

    if (stars.size() < num_stars) {
      Star newStar;
      newStar.x = rand() % getmaxx(stdscr);
      newStar.y = rand() % getmaxy(stdscr);
      stars.push_back(newStar);
    }

    for (int i = 0; i < stars.size(); ++i) {
      mvprintw(stars[i].y, stars[i].x, ".");

      // Move stars
      stars[i].x--;
      if (stars[i].x < 0) {
        stars[i].x = getmaxx(stdscr) - 1;
        stars[i].y = rand() % getmaxy(stdscr);
      }
    }

    refresh();
    std::this_thread::sleep_for(
        std::chrono::milliseconds(50)); // Adjust the delay
  }
}

void spawnEnemies(int noe) {
  int numberOfEnemies = noe;

  srand(time(NULL)); // Seed the random number generator

  int maxX, maxY;
  getmaxyx(stdscr, maxY, maxX);

  Enemy enemies[numberOfEnemies];
  for (int i = 0; i < numberOfEnemies; i++) {
    enemies[i].x = maxX - 1; // Start from the right side of the screen
    enemies[i].y = rand() % maxY;
  }

  clear(); // Clear the screen before entering the loop

  while (1) {
    int bulletX = bullet_x.load();
    int bulletY = bullet_y.load();

    for (int i = 0; i < numberOfEnemies; i++) {
      int artHeight = sizeof(enemyArt) / sizeof(enemyArt[0]);
      for (int j = 0; j < artHeight; j++) {
        attron(COLOR_PAIR(1));
        mvprintw(enemies[i].y + j, enemies[i].x - strlen(enemyArt[0]),
                 enemyArt[j]);
        attron(COLOR_PAIR(1));
      }

      if (bulletX >= enemies[i].x - strlen(enemyArt[0]) && bulletX < maxX &&
          bulletY >= enemies[i].y && bulletY < enemies[i].y + artHeight) {
        for (int k = 0; k < artHeight; k++) {
          mvprintw(enemies[i].y + k, enemies[i].x - strlen(enemyArt[0]),
                   explosion[k]);
          refresh();
          usleep(200000);
        }
        // Bullet hit the enemy, reset enemy's position
        enemies[i].x = maxX - 1;
        enemies[i].y = rand() % maxY;
        bullet_x.store(-1);
        bullet_y.store(-1);
        enemiesKilled++;
      }

      enemies[i].x--;

      if (enemies[i].x < 0) {
        enemies[i].x = maxX - 1;
        enemies[i].y = rand() % maxY;
        health_bar++;
      }
    }

    refresh(); // Refresh the display after updating positions

    usleep(50000); // Adjust the speed
  }
}

LevelData getLevelData(int levelNumber) {
  LevelData levelData;

  std::ifstream file("levels.json");
  if (!file.is_open()) {
    std::cerr << "Error opening levels.json" << std::endl;
    return levelData;
  }

  json_object *rootObj;
  json_object *levelsArray;

  char *fileContent =
      new char[65536]; // Assume the file is not larger than 64KB
  file.read(fileContent, 65536);

  rootObj = json_tokener_parse(fileContent);
  delete[] fileContent;

  if (json_object_object_get_ex(rootObj, "levels", &levelsArray)) {
    int arrayLen = json_object_array_length(levelsArray);
    for (int i = 0; i < arrayLen; ++i) {
      json_object *levelObj = json_object_array_get_idx(levelsArray, i);
      json_object *levelNumberObj;
      if (json_object_object_get_ex(levelObj, "levelNumber", &levelNumberObj)) {
        int levelNum = json_object_get_int(levelNumberObj);
        if (levelNum == levelNumber) {
          json_object *timeLimitObj, *enemiesKilledObj, *totalEnemiesObj,
              *healthUpPowerUpObj, *speedUpPowerUpObj;
          json_object_object_get_ex(levelObj, "timeLimit", &timeLimitObj);
          json_object_object_get_ex(levelObj, "enemiesKilled",
                                    &enemiesKilledObj);
          json_object_object_get_ex(levelObj, "totalEnemies", &totalEnemiesObj);
          json_object_object_get_ex(levelObj, "healthUpPowerUp",
                                    &healthUpPowerUpObj);
          json_object_object_get_ex(levelObj, "speedUpPowerUp",
                                    &speedUpPowerUpObj);

          levelData.levelNumber = levelNum;
          levelData.timeLimit = json_object_get_int(timeLimitObj);
          levelData.enemiesKilled = json_object_get_int(enemiesKilledObj);
          levelData.totalEnemies = json_object_get_int(totalEnemiesObj);
          levelData.healthUpPowerUp = json_object_get_int(healthUpPowerUpObj);
          levelData.speedUpPowerUp = json_object_get_int(speedUpPowerUpObj);

          break;
        }
      }
    }
  }

  json_object_put(rootObj);
  file.close();

  return levelData;
}

void spaceShipGame() {

  auto levelData = getLevelData(1);
  auto startTime = std::chrono::high_resolution_clock::now();
  int x = 0; // Initial x-coordinate of spaceship
  int y = 0; // Initial y-coordinate of spaceship

  nodelay(stdscr, TRUE); // Enable non-blocking input
  keypad(stdscr, TRUE);  // Enable special key codes like arrow keys

  thread enemyThread(
      [&]() { spawnEnemies(2); }); // Create a thread for spawning enemies
  thread starThread([&]() { spawnStars(20); });
  thread bulletThread(handleBullet); // Create a thread for handling the bullet

  while (1) {
    clear(); // Clear the screen
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - startTime;
    std::chrono::duration<double> timeBeenSinceSpeedUpPowerUpActivated =
        currentTime - startTimeOfSpeedUpPowerUp;
    double timeBeenSinceSpeedUpPowerUpActivatedInSeconds =
        timeBeenSinceSpeedUpPowerUpActivated.count();
    double elapsedTimeInSeconds = elapsed.count();
    attron(COLOR_PAIR(health_bar)); // Turn a colour pair on
    mvprintw(0, 10, "Elapsed Time: %f", elapsedTimeInSeconds);
    mvprintw(0, 40, "Time Left: %f",
             levelData.timeLimit - elapsedTimeInSeconds);
    mvprintw(0, 70, "Enemies Killed: %d", enemiesKilled);
    mvprintw(0, 100, "Health PowerUps: %d", levelData.healthUpPowerUp);
    mvprintw(0, 130, "Speed PowerUps: %d", levelData.speedUpPowerUp);

    // Display the spaceship
    for (size_t i = 0; i < spaceship.size(); i++) {
      mvprintw(y + i, x, spaceship[i].c_str());
    }
    attroff(COLOR_PAIR(health_bar));
    char input = getch(); // Get user input
    if (input == 'q') {
      break; // Exit the loop if 'q' is pressed
    }
    if (timeBeenSinceSpeedUpPowerUpActivatedInSeconds >= 25) {
      speedUpPowerUpActive = false;
    }

    // Move spaceship and handle bullet based on input
    moveSpaceship(&x, &y, input, levelData);

    // Sleep briefly to control the speed
    this_thread::sleep_for(chrono::milliseconds(50));
    if (health_bar >= 4) {
      clear();
      display_death_menu();
    }
    refresh();
  }
}

void display_death_menu() {
  int x = 30;
  int y = 0;
  attron(COLOR_PAIR(3));
  mvprintw(y++, x,
           "000000000     000000000           0      00000000000 0        0");
  mvprintw(y++, x,
           "0        0    0                  0 0          0      0        0");
  mvprintw(y++, x,
           "0         0   0                 0   0         0      0        0");
  mvprintw(y++, x,
           "0          0  0                0     0        0      0        0");
  mvprintw(y++, x,
           "0           0 0               0       0       0      0        0");
  mvprintw(y++, x,
           "0           0 000000000      00000000000      0      0000000000");
  mvprintw(y++, x,
           "0          0  0             0           0     0      0        0");
  mvprintw(y++, x,
           "0         0   0            0             0    0      0        0");
  mvprintw(y++, x,
           "0        0    0           0               0   0      0        0");
  mvprintw(y++, x,
           "0       0     0          0                 0  0      0        0");
  mvprintw(y++, x,
           "00000000      000000000 0                   0 0      0        0");
  mvprintw(y++, x, "                             _________ ");
  mvprintw(y++, x, "                            |         |");
  mvprintw(y++, x, "                            | Restart |");
  mvprintw(y++, x, "                            |_________|");
  mvprintw(y++, x, "                             __________ ");
  mvprintw(y++, x, "                            |          |");
  mvprintw(y++, x, "                            | Mainmenu |");
  mvprintw(y++, x, "                            |__________|");
  attroff(COLOR_PAIR(3));
  while (1) {
  }
}

void set_controls(struct Controls controls) {

  const char *filename = "controls.json";
  FILE *file = fopen(filename, "r+");
  if (file == NULL) {
    fprintf(stderr, "Error opening file: %s\n", filename);
    return;
  }

  char buffer[1024];
  size_t file_size = fread(buffer, 1, sizeof(buffer), file);
  buffer[file_size] = '\0';

  struct json_object *json_obj = json_tokener_parse(buffer);
  if (json_obj == NULL) {
    fprintf(stderr, "Error parsing JSON\n");
    fclose(file);
    return;
  }

  struct json_object *controls_obj;
  if (json_object_object_get_ex(json_obj, "controls", &controls_obj)) {
    json_object_object_add(controls_obj, "up",
                           json_object_new_string(controls.up));
    json_object_object_add(controls_obj, "down",
                           json_object_new_string(controls.down));
    json_object_object_add(controls_obj, "left",
                           json_object_new_string(controls.left));
    json_object_object_add(controls_obj, "right",
                           json_object_new_string(controls.right));
    json_object_object_add(controls_obj, "shoot",
                           json_object_new_string(controls.shoot));

    fseek(file, 0, SEEK_SET);
    fprintf(file, "%s",
            json_object_to_json_string_ext(json_obj, JSON_C_TO_STRING_SPACED));
  } else {
    fprintf(stderr, "Controls not found in JSON\n");
  }

  json_object_put(json_obj);
  fclose(file);
}

struct Controls get_controls() {

  struct Controls controls = {0}; // Initialize all fields to NULL

  const char *filename = "controls.json";
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening file: %s\n", filename);
    return controls;
  }

  // Calculate the size of the file
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer =
      (char *)malloc(file_size + 1); // Allocate memory for the entire file
  if (buffer == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    fclose(file);
    return controls;
  }

  if (fread(buffer, 1, file_size, file) != file_size) {
    fprintf(stderr, "Error reading from file\n");
    fclose(file);
    free(buffer);
    return controls;
  }

  buffer[file_size] = '\0'; // Null-terminate the buffer

  fclose(file);

  struct json_object *json_obj = json_tokener_parse(buffer);
  free(buffer);

  if (json_obj == NULL) {
    fprintf(stderr, "Error parsing JSON\n");
    return controls;
  }

  struct json_object *controls_obj;
  if (json_object_object_get_ex(json_obj, "controls", &controls_obj)) {
    struct json_object *up_obj, *down_obj, *left_obj, *right_obj, *shoot_obj;
    if (json_object_object_get_ex(controls_obj, "up", &up_obj)) {
      controls.up = json_object_get_string(up_obj);
    }
    if (json_object_object_get_ex(controls_obj, "down", &down_obj)) {
      controls.down = json_object_get_string(down_obj);
    }
    if (json_object_object_get_ex(controls_obj, "left", &left_obj)) {
      controls.left = json_object_get_string(left_obj);
    }
    if (json_object_object_get_ex(controls_obj, "right", &right_obj)) {
      controls.right = json_object_get_string(right_obj);
    }
    if (json_object_object_get_ex(controls_obj, "shoot", &shoot_obj)) {
      controls.shoot = json_object_get_string(shoot_obj);
    }
  } else {
    fprintf(stderr, "Controls not found in JSON\n");
  }

  json_object_put(json_obj);

  return controls;
}

void display_controls() {

  struct Controls controls = get_controls();
  int x = 30;
  int y = 0;
  int option = 1;
  mvprintw(y++, x,
           "000000000 0 0 0 0 0 0 0 0 0 0 0 0           0 00000000000 "
           "000000000 0 0 0 0 0 0 0 0 0 0 0 0           000000000");
  mvprintw(y++, x,
           "0         0                   0 0 0         0      0      0       "
           "0 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0  0        0      0      0       "
           "0 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0   0       0      0      "
           "000000000 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0    0      0      0      0        "
           " 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0     0     0      0      00       "
           " 0                   0 0           000000000");
  mvprintw(y++, x,
           "0         0                   0 0      0    0      0      0 0      "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "0         0                   0 0       0   0      0      0  0     "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "0         0                   0 0        0  0      0      0   0    "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "0         0                   0 0         0 0      0      0    0   "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "000000000 0 0 0 0 0 0 0 0 0 0 0 0          0       0      0     0  "
           " 0 0 0 0 0 0 0 0 0 0 0 00000000000 000000000");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                          up-|___|-%s",
           controls.up);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                        down-|___|-%s",
           controls.down);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                        left-|___|-%s",
           controls.left);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                       right-|___|-%s",
           controls.right);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                       shoot-|___|-%s",
           controls.shoot);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  refresh();
  char input;
  int llos;
  while (input != 27) {
    switch (llos) {
    case 13:
      mvprintw(13, x, "                                          up-|___|-%s ",
               controls.up);
    case 18:
      mvprintw(18, x, "                                        down-|___|-%s ",
               controls.down);
    case 23:
      mvprintw(23, x, "                                        left-|___|-%s ",
               controls.left);
    case 28:
      mvprintw(28, x, "                                       right-|___|-%s ",
               controls.right);
    case 33:
      mvprintw(33, x, "                                       shoot-|___|-%s ",
               controls.shoot);
    }
    if (input == 'w' && option != 1) {
      option--;
    } else if (input == 's' && option != 5) {
      option++;
    } else if (input == 'r') {
      char newControl = (char)getch();
      switch (option) {
      case 1:
        controls.up = &newControl;
        set_controls(controls);
        break;
      case 2:
        controls.down = &newControl;
        set_controls(controls);
        break;
      case 3:
        controls.left = &newControl;
        set_controls(controls);
        break;
      case 4:
        controls.right = &newControl;
        set_controls(controls);
        break;
      case 5:
        controls.shoot = &newControl;
        set_controls(controls);
        break;
      }
    }
    switch (option) {
    case 1:
      mvprintw(13, x, "                                         *up-|___|-%s*",
               controls.up);
      llos = 13;
      break;
    case 2:
      mvprintw(18, x, "                                       *down-|___|-%s*",
               controls.down);
      llos = 18;
      break;
    case 3:
      mvprintw(23, x, "                                       *left-|___|-%s*",
               controls.left);
      llos = 23;
      break;
    case 4:
      mvprintw(28, x, "                                      *right-|___|-%s*",
               controls.left);
      llos = 28;
      break;
    case 5:
      mvprintw(33, x, "                                      *shoot-|___|-%s*",
               controls.left);
      llos = 33;
      break;
    }
    input = getch();
  }
}

void display_levels() {
  int x = 30;
  int y = 0;
  int option = 1;
  clear();
  attron(COLOR_PAIR(5));
  mvprintw(y++, x,
           "0           000000000  0                   0  000000000  0         "
           "  000000000");
  mvprintw(y++, x,
           "0           0           0                 0   0          0         "
           "  0        ");
  mvprintw(y++, x,
           "0           0            0               0    0          0         "
           "  0        ");
  mvprintw(y++, x,
           "0           0             0             0     0          0         "
           "  0        ");
  mvprintw(y++, x,
           "0           0              0           0      0          0         "
           "  0        ");
  mvprintw(y++, x,
           "0           000000000       0         0       000000000  0         "
           "  000000000");
  mvprintw(y++, x,
           "0           0                0       0        0          0         "
           "          0");
  mvprintw(y++, x,
           "0           0                 0     0         0          0         "
           "          0");
  mvprintw(y++, x,
           "0           0                  0   0          0          0         "
           "          0");
  mvprintw(y++, x,
           "00000000000 000000000            0            000000000  "
           "00000000000 000000000");
  mvprintw(y++, x,
           " ________  ________  ________  ________  ________  ________  "
           "________  ________");
  mvprintw(y++, x,
           "|        ||        ||        ||        ||        ||        ||      "
           "  ||        |");
  mvprintw(y++, x,
           "|   1    ||   2    ||   3    ||   4    ||   5    ||   6    ||   7  "
           "  ||   8    |");
  mvprintw(y++, x,
           "|________||________||________||________||________||________||______"
           "__||________|");
  mvprintw(y++, x,
           " ________  ________  ________  ________  ________  ________  "
           "________  ________");
  mvprintw(y++, x,
           "|        ||        ||        ||        ||        ||        ||      "
           "  ||        |");
  mvprintw(y++, x,
           "|   9    ||   10   ||   11   ||   12   ||   13   ||   14   ||   15 "
           "  ||   16   |");
  mvprintw(y++, x,
           "|________||________||________||________||________||________||______"
           "__||________|");
  mvprintw(y++, x,
           " ________  ________  ________  ________  ________  ________  "
           "________  ________");
  mvprintw(y++, x,
           "|        ||        ||        ||        ||        ||        ||      "
           "  ||        |");
  mvprintw(y++, x,
           "|   17   ||   18   ||   19   ||   20   ||   21   ||   22   ||   23 "
           "  ||   24   |");
  mvprintw(y++, x,
           "|________||________||________||________||________||________||______"
           "__||________|");
  mvprintw(y++, x,
           " ________  ________  ________  ________  ________  ________  "
           "________  ________");
  mvprintw(y++, x,
           "|        ||        ||        ||        ||        ||        ||      "
           "  ||        |");
  mvprintw(y++, x,
           "|   25   ||   26   ||   27   ||   28   ||   29   ||   30   ||   31 "
           "  ||   32   |");
  mvprintw(y++, x,
           "|________||________||________||________||________||________||______"
           "__||________|");
  mvprintw(y++, x,
           " ________  ________  ________  ________  ________  ________  "
           "________  ________");
  mvprintw(y++, x,
           "|        ||        ||        ||        ||        ||        ||      "
           "  ||        |");
  mvprintw(y++, x,
           "|   33   ||   34   ||   35   ||   36   ||   37   ||   38   ||   39 "
           "  ||   40   |");
  mvprintw(y++, x,
           "|________||________||________||________||________||________||______"
           "__||________|");
  attroff(COLOR_PAIR(5));
  refresh();
  char input;
  while (input != 10) {
    if (input == 'w' && option <= 8) {
      option -= 8;
    } else if (input == 's' && option <= 33) {
      option += 8;
    } else if (input == 'd' && option % 8 != 0) {
      option++;
    } else if (input == 'a' && option % 8 != 1) {
      option--;
    } else if (input == 27) {
      return;
    }
    input = getch();
  }
  spaceShipGame();
}

void main_menu() {
  int x = 30;
  int y = 0;
  int option = 1;
  attron(COLOR_PAIR(4));
  mvprintw(y++, x,
           "000000000 000000000      0            000000000 000000000 "
           "000000000 0        0 00000000000 000000000");
  mvprintw(y++, x,
           "0         0       0     0 0           0         0         0        "
           " 0        0      0      0       0");
  mvprintw(y++, x,
           "0         0       0    0   0          0         0         0        "
           " 0        0      0      0       0");
  mvprintw(y++, x,
           "0         000000000   0     0         0         0         0        "
           " 0        0      0      000000000");
  mvprintw(y++, x,
           "0         0          0       0        0         0         0        "
           " 0        0      0      0");
  mvprintw(y++, x,
           "000000000 0         00000000000       0         000000000 "
           "000000000 0000000000      0      0");
  mvprintw(y++, x,
           "        0 0        0           0      0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "        0 0       0             0     0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "        0 0      0               0    0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "        0 0     0                 0   0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "000000000 0    0                   0  000000000 000000000 "
           "000000000 0        0 00000000000 0");

  y += 10;
  x += 40;

  mvprintw(y++, x, " /----\\ ");
  mvprintw(y++, x, "|levels|");
  mvprintw(y++, x, " \\----/ ");
  y++;
  mvprintw(y++, x, " /------\\ ");
  mvprintw(y++, x, "|controls|");
  mvprintw(y++, x, " \\------/ ");
  y++;
  x += 2;
  mvprintw(y++, x, " /--\\ ");
  mvprintw(y++, x, "|quit|");
  mvprintw(y++, x, " \\--/ ");

  refresh();
  char input;
  int llos;
  while (true) {
    switch (option) {
    case 1:
      mvprintw(22, x - 3, "*|levels|*");
      llos = 22;
      break;
    case 2:
      mvprintw(26, x - 3, "*|controls|*");
      llos = 26;
      break;
    case 3:
      mvprintw(30, x - 3, "  *|quit|*");
      llos = 30;
      break;
    }
    input = getch();
    if (input == 'w' && option != 1) {
      option--;
    } else if (input == 's' && option != 3) {
      option++;
    } else if (input == 10) { // ASCII code for Enter
      attroff(COLOR_PAIR(4));
      switch (option) {
      case 1:
        display_levels();
        break;
      case 2:
        display_controls();
        break;
      case 3:
        endwin();
        exit(0);
      }
      attron(COLOR_PAIR(4));
    }
    switch (llos) {
    case 22:
      mvprintw(22, x - 3, " |levels| ");
      break;
    case 26:
      mvprintw(26, x - 3, " |controls| ");
      break;
    case 30:
      mvprintw(30, x - 3, "   |quit| ");
      break;
    }
  }
}
