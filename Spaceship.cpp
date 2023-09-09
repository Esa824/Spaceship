#include <curses.h>
#include "structs.h"
#include "menu_screens.h"

using namespace std;

int main() {
  initscr(); // Initialize the ncurses library
  noecho();
  start_color(); // Initialize the colours
  curs_set(0);
  int COLOR_ORANGE;
  init_color(COLOR_ORANGE, 255, 165, 0);  // Intialize a colour orange
  init_pair(1, COLOR_GREEN, COLOR_BLACK); // Set a colour pair
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_ORANGE, COLOR_BLACK);
  main_menu();
  curs_set(1); // Hide the cursor
  return 0;
}
