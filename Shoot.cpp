#include <bits/stdc++.h>
#include <ncurses.h>
void remove_old_shoot(int y, int x) {
  move(y, x);
  printw(" ");
}
void print_shoot(int y, int x) {
  move(y, x);
  printw("-");
}

