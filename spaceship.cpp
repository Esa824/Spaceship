#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
using namespace std;
vector<string> vec{"           \\\\\\_____", "        ###[==_____>",
                   "           ///     "};
vector<string> enemy{"    \\---/", "  _{ {0} }_", " {_________}"};
void draw_space_ship(int &x, int &y, int key) {
  if (key == 115) {
    y++;
    move(y, x);
    int Y = y;
    for (int i = 0; i < vec.size(); i++) {
      printw("%s\n", vec[i].c_str());
    }
  }
  if (key == 119) {
    y--;
    move(y, x);
    int Y = y;
    for (int i = 0; i < vec.size(); i++) {
      printw("%s\n", vec[i].c_str());
    }
  }
  if (key == 100) {
    x++;
    int Y = y;
    move(y, x);
    for (int i = 0; i < vec.size(); i++) {
      printw("%s\n", vec[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (key == 97) {
    x--;
    int Y = y;
    move(y, x);
    for (int i = 0; i < vec.size(); i++) {
      printw("%s\n", vec[i].c_str());
      Y++;
      move(Y, x);
    }
  }
}
int main() {
  initscr();
  int x = 0;
  int y = 0;
  move(y, x);
  for (int i = 0; i < vec.size(); i++) {
    printw("%s\n", vec[i].c_str());
  }
  for (;;) {
    int b = getch();
    clear();
    draw_space_ship(x, y, b);
    if (b == 27) {
      break;
    }
  }
}
