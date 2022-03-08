#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;
vector<string> spaceship{"           \\\\\\_____", "        ###[==_____>",
                         "           ///     "};
vector<string> enemy{"    \\---/", "  _{ {0} }_", " {_________}"};
vector<string> stars{"   |", " -0- ", "  |", "* -0- -) *"};
auto move_space_ship(int &x, int &y, int key) {
  if (key == 115) {
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
  printf("\033[33mred text\n");
  initscr();
  COLOR_PAIR(5);
  auto x = 0;
  auto y = 0;
  int helth_bar_lives = 3;
  move(y, x);
  for (int i = 0; i < spaceship.size(); i++) {
    printw("%s\n", spaceship[i].c_str());
  }
  for (;;) {
    move(0, 0);
    int b = getch();
    move(y, x);
    clear();
    move_space_ship(x, y, b);
    if (b == 27) {
      break;
    }
  }
}
