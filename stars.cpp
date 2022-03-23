#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>
void remove_star(int x, int y) {
  move(y, x);
  printw(" ");
}
void print_star(int x, int y) {
  move(y, x);
  printw("*");
}
