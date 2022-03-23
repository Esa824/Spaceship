#include "print_star.h"
#include "remove_star.h"
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;
vector<string> spaceship{"           \\\\\\_____", "        ###[==_____>",
                         "           ///     "};
vector<string> enemy{"    \\---/", "  _{ {0} }_", " {_________}"};
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
int main() {
  initscr();
  start_color();
  int count = 0;
  auto x = 0;
  auto y = 0;
  int x_star = 99;
  int y_star = 5;
  int check = 0;
  int check_two = 0;
  int helth_bar_lives = 3;
  move(y, x);
  for (int i = 0; i < spaceship.size(); i++) {
    printw("%s\n", spaceship[i].c_str());
  }
  nodelay(stdscr, true);
  for (;;) {
    if (x_star > 0) {
      x_star--;
      usleep(39999);
      print_star(x_star, y_star);
      remove_star(x_star + 1, y_star);
    }
    move(0, 0);
    int b = getch();
    move_space_ship(x, y, b);
  }
}
