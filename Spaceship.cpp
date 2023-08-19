#include <atomic>
#include <chrono>
#include <curses.h>
#include <json-c/json.h>
#include <mutex>
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

typedef struct {
  int x, y;
} Enemy;

struct Controls {
  const char *up;
  const char *down;
  const char *left;
  const char *right;
  const char *shoot;
};

vector<string> spaceship = {"__", "| \\", "=[_|H)--._____", "=[+--,-------'",
                            "[|_/\"\""};
const char *enemyArt[] = {
    "                                _____.-----._____",
    "                   ___----~~~~~~. ... ..... ... .~~~~~~----___",
    "                =================================================",
    "                   ~~~-----......._____________.......-----~~~",
    "                    (____)          \\   |   /          (____)",
    "                      ||           _/   |   \\_           ||",
    "                       \\\\_______--~  //~~~\\\\  ~--_______//",
    "                        `~~~~---__   \\\\___//   __---~~~~'",
    "                                  ~~-_______-~~"};

atomic<int> bullet_x(-1); // Atomic variable to store bullet's x-coordinate
atomic<int> bullet_y(-1); // Atomic variable to store bullet's y-coordinate

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
  while (input != 27) {

    if (input == 'w' && option != 1) {
      option--;
    } else if (input == 's' && option != 3) {
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
    input = getch();
  }
}

void moveSpaceship(int *x, int *y, char where) {
  switch (where) {
  case 'w':
    (*y)--;
    break;
  case 's':
    (*y)++;
    break;
  case 'a':
    (*x)--;
    break;
  case 'd':
    (*x)++;
    break;
  case 32:                  // Spacebar
    bullet_y.store(*y + 2); // Store bullet's initial y-coordinate
    bullet_x.store(*x);     // Store bullet's initial x-coordinate
    break;
  }
}

mutex bulletMutex;

void handleBullet() {
  while (1) {
    int x = bullet_x.load();
    int y = bullet_y.load();
    if (x >= 0 && y >= 0 && x <= 300) {
      // Lock the mutex before accessing the bullet coordinates
      bulletMutex.lock();

      mvprintw(y, x, "-"); // Print bullet at specified coordinates
      refresh();
      this_thread::sleep_for(chrono::milliseconds(50)); // Adjust speed
      mvprintw(y, x, " "); // Erase bullet using spaceship character
      refresh();
      x++;
      bullet_x.store(x);

      // Unlock the mutex after modifying the bullet coordinates
      bulletMutex.unlock();
    } else {
      bullet_x.store(-1); // Reset bullet's x-coordinate
      bullet_y.store(-1); // Reset bullet's y-coordinate
    }
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

  while (1) {
    clear();

    int bulletX = bullet_x.load();
    int bulletY = bullet_y.load();

    for (int i = 0; i < numberOfEnemies; i++) {
      int artHeight = sizeof(enemyArt) / sizeof(enemyArt[0]);
      for (int j = 0; j < artHeight; j++) {
        mvprintw(enemies[i].y + j, enemies[i].x - strlen(enemyArt[0]),
                 enemyArt[j]);
      }

      if (bulletX >= enemies[i].x - strlen(enemyArt[0]) && bulletX < maxX &&
          bulletY >= enemies[i].y && bulletY < enemies[i].y + artHeight) {
        // Bullet hit the enemy, reset enemy's position
        enemies[i].x = maxX - 1;
        enemies[i].y = rand() % maxY;
        bullet_x.store(-1);
        bullet_y.store(-1);
      }

      enemies[i].x--;

      if (enemies[i].x < 0) {
        enemies[i].x = maxX - 1;
        enemies[i].y = rand() % maxY;
      }
    }

    refresh();
    usleep(100000); // Adjust the speed

    if (getch() == 'q') {
      break;
    }
  }
}

void display_levels();

void main_menu() {
  int x = 30;
  int y = 0;
  int option = 1;
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
  while (true) {
    input = getch();
    if (input == 'w' && option != 1) {
      option--;
    } else if (input == 's' && option != 3) {
      option++;
    } else if (input == 10) { // ASCII code for Enter
      break;
    }
  }
  switch (option) {
  case 1:
    break;
  case 2:
    display_controls();
    break;
  case 3:
    endwin();
    exit(0);
  }
}

int main() {
  initscr(); // Initialize the ncurses library
  main_menu();
  curs_set(0); // Hide the cursor

  int x = 0; // Initial x-coordinate of spaceship
  int y = 0; // Initial y-coordinate of spaceship

  nodelay(stdscr, TRUE); // Enable non-blocking input
  keypad(stdscr, TRUE);  // Enable special key codes like arrow keys

  thread enemyThread(
      [&]() { spawnEnemies(2); });   // Create a thread for spawning enemies
  thread bulletThread(handleBullet); // Create a thread for handling the bullet

  while (1) {
    clear(); // Clear the screen

    // Display the spaceship
    for (size_t i = 0; i < spaceship.size(); i++) {
      mvprintw(y + i, x, spaceship[i].c_str());
    }

    char input = getch(); // Get user input
    if (input == 'q') {
      break; // Exit the loop if 'q' is pressed
    }

    // Move spaceship and handle bullet based on input
    moveSpaceship(&x, &y, input);

    // Sleep briefly to control the speed
    this_thread::sleep_for(chrono::milliseconds(50));

    refresh();
  }

  bulletThread.join(); // Wait for the bullet thread to finish
  enemyThread.join();  // Wait for the enemy thread to finish
  endwin();            // End the ncurses mode

  return 0;
}
