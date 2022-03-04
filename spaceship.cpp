#include <cstdio>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
using namespace std;
vector<string> vec{"           \\\\\\_____", "        ###[==_____>", "           ///     "};
void draw_space_ship(int &x, int &y, int key) {
  if (key == 115) {
    move(x + 1, y + 1);
    x++;
    y++;
    for (int i = 0; i < vec.size(); i++) {
      printw("%s\n", vec[i].c_str());
    }
  }
  if (key == 119) {
    move(x - 1, y - 1);
    x--;
    y--;
    for (int i = 0; i < vec.size(); i++) {
      printw("%s\n", vec[i].c_str());
    }
  }
}
int main() {
  initscr();
  int x = 0;
  int y = 0;
  move(x, y);
  int check = 1;
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
