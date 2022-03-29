#include "Print_shoot.h"
#include "Print_star.h"
#include "Remove_old_shoot.h"
#include "Remove_star.h"
#include <cstdlib>
#include <limits.h>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>
using namespace std;
struct bullet {
  int y;
  int x;
  int tick;
  int check;
  bullet *next;
};
vector<string> spaceship{"           \\\\\\_____", "        ###[==_____>",
                         "           ///     "};
vector<string> number_of_spaces{"        ", "            ", "   "};
void remove_space_ship(int &x, int &y) {
  int Y = y;
  int X = x;
  move(Y, X);
  for (int i = 0; i < number_of_spaces.size(); i++) {
    if (i == 1) {
      X -= 3;
    }
    if (i == 2) {
      X += 3;
    }
    printw("%s\n", number_of_spaces[0].c_str());
    Y++;
    move(Y, X);
  }
  move(y, x);
}
void move_space_ship(int &x, int &y, int key) {
  if (key == 115) {
    remove_space_ship(x, y);
    // move down
    y++;
    move(y, x);
    int Y = y;
    for (int i = 0; i < spaceship.size(); i++) {
      printw("%s\n", spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (key == 119) {
    remove_space_ship(x, y);
    // move up
    y--;
    move(y, x);
    int Y = y;
    for (int i = 0; i < spaceship.size(); i++) {
      printw("%s\n", spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (key == 100) {
    remove_space_ship(x, y);
    // move right
    x++;
    int Y = y;
    move(y, x);
    for (int i = 0; i < spaceship.size(); i++) {
      printw("%s\n", spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (key == 97) {
    remove_space_ship(x, y);
    // move left
    x--;
    int Y = y;
    move(y, x);
    for (int i = 0; i < spaceship.size(); i++) {
      printw("%s\n", spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
}
void print_main_menu() {
  unordered_map<int, vector<int>> map;
  move(6, 70);
  printw("000000  000000   000000  000000  000000");
  move(7, 70);
  printw("0       0    0   0    0  0       0     ");
  move(8, 70);
  printw("000000  000000   000000  0       000000");
  move(9, 70);
  printw("     0  0        0    0  0       0     ");
  move(10, 70);
  printw("000000  0        0    0  000000  000000");
  move(12, 84);
  printw("________");
  move(13, 84);
  printw("|      |");
  move(14, 84);
  printw("| play |");
  move(15, 84);
  printw("|______|");
  move(14, 88);
  while (getch() != 10) {
  }
  clear();
}
void print_helth_bar() {
  move(0, 0);
  printw("   _ _");
  move(1, 0);
  printw(" /  V  \\");
  move(2, 0);
  printw(" |     |");
  move(3, 0);
  printw("  \\   /");
  move(4, 0);
  printw("   \\ /");
  move(5, 0);
  printw("    *");
}
bullet *create_memory() {
  bullet *ptr = (bullet *)malloc(sizeof(bullet));
  return ptr;
}
int main() {
  initscr();
  start_color();
  print_main_menu();
  bullet *ptr = create_memory();
  ptr->check = 0;
  int count = 0;
  auto x = 0;
  auto y = 0;
  int x_star = 99;
  int y_star = 5;
  int check_two = 0;
  int helth_bar_lives = 3;
  move(y, x);
  for (int i = 0; i < spaceship.size(); i++) {
    printw("%s\n", spaceship[i].c_str());
  }
  nodelay(stdscr, true);
  for (;;) {
    if (x_star > -1) {
      usleep(29999);
      x_star--;
      remove_star(x_star + 1, y_star);
      if (x_star != -1) {
        print_star(x_star, y_star);
      }
    }
    move(0, 0);
    int b = getch();
    if (ptr->tick == 199) {
      ptr->tick = 0;
      ptr->check = 0;
    }
    // making it shoot
    if (b == 32 && ptr->check == 0) {
      ptr->y = y + 1;
      ptr->x = x + 21;
      ptr->tick = 0;
      ptr->check = 1;
    }
    if (ptr->check == 1) {
      if (x_star < 0) {
        usleep(19999);
      }
      if (ptr->tick != 198) {
        move(ptr->y, ptr->x);
      }
      printw("-");
      move(ptr->y, ptr->x - 1);
      printw(" ");
      ptr->x++;
      ptr->tick++;
    }
    // end making it shoot
    if (b == 27) {
      clear();
      print_main_menu();
      move(y, x);
      int Y = y;
      for (int i = 0; i < spaceship.size(); i++) {
        printw("%s\n", spaceship[i].c_str());
        Y++;
        move(Y, x);
      }
    }
    move_space_ship(x, y, b);
  }
}
