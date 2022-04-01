#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <cstdlib>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>
using namespace std;
int myRand(int fromA, int toB) {
  if (fromA > toB)
    return myRand(toB, fromA);
  int d = toB - fromA;
  int r = rand() % (d + 1);
  return r + fromA;
}
struct bullet {
  int y;
  int x;
  int tick;
  int check;
  bullet *next;
};
struct enemy_bullet {
  int y;
  int x;
  int tick;
  int check;
  bullet *next;
};
struct star {
  int y = 0;
  int x = 0;
  star *next;
};
vector<string> spaceship{"           \\\\\\_____", "        ###[==_____>",
                         "           ///     "};
vector<string> number_of_spaces_for_spaceship{"        ", "            ", "   "};
vector<string> enemy{"  \\____/", "<_/__~0_\\", "(/_______\\)"};
vector<string> number_of_spaces_for_alien_spaceship{"      ", "          ", "           "};
void remove_space_ship(int &x, int &y) {
  int Y = y;
  int X = x;
  move(Y, X);
  for (int i = 0; i < number_of_spaces_for_spaceship.size(); i++) {
    if (i == 1) {
      X -= 3;
    }
    if (i == 2) {
      X += 3;
    }
    printw("%s\n", number_of_spaces_for_spaceship[0].c_str());
    Y++;
    move(Y, X);
  }
  move(y, x);
}
void remove_alien_spaceship(int x){
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
void move_aline_space_ship(int y, int x, int getch){
  if(getch == KEY_UP){
    
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
  int a = 0;
  while (a != 10) {
    move(0, 0);
    a = getch();
  }
  clear();
}
void print_helth_bar(int x) {
  move(0, x);
  printw("   _ _");
  move(1, x);
  printw(" /  V  \\");
  move(2, x);
  printw(" |     |");
  move(3, x);
  printw("  \\   /");
  move(4, x);
  printw("   \\ /");
  move(5, x);
  printw("    *");
}
bullet *create_memory_for_bullet(int x, int y, int check) {
  bullet *ptr = (bullet *)malloc(sizeof(bullet));
  ptr->x = x;
  ptr->y = y;
  ptr->check = check;
  return ptr;
}
star *create_memory_for_star(int y, int x) {
  star *ptr = (star *)malloc(sizeof(bullet));
  ptr->y = y;
  ptr->x = x;
  return ptr;
}
void shoot(bullet *sec_bullet_ptr) {
  if (sec_bullet_ptr->tick < 182) {
    sec_bullet_ptr->tick++;
    move(sec_bullet_ptr->y, sec_bullet_ptr->x);
    if (sec_bullet_ptr->tick != 181) {
      printw("-");
    }
    move(sec_bullet_ptr->y, sec_bullet_ptr->x - 1);
    printw(" ");
    sec_bullet_ptr->x++;
  } else {
    sec_bullet_ptr->check = 0;
  }
}

int main(int argc, char **argv) {
  initscr();
  noecho();
  keypad(stdscr,true);
  print_main_menu();
  star *star_ptr = create_memory_for_star(myRand(5, 40), myRand(20, 200));
  star *sec_star_ptr = star_ptr;
  for (int i = 0; i < 5; i++) {
    star_ptr->next = create_memory_for_star(myRand(5, 40), myRand(20, 200));
    star_ptr = star_ptr->next;
  }
  star_ptr = sec_star_ptr;
  auto x = 0;
  auto y = 0;
  auto y_enemy = myRand(5, 50);
  auto x_enemy = myRand(100, 200);
  move(y_enemy, x_enemy);
  int Y = y_enemy;
  for (int i = 0; i < enemy.size(); i++) {
    printw("%s\n", enemy[i].c_str());
    Y++;
    move(Y, x_enemy);
  }
  bullet *bullet_ptr = create_memory_for_bullet(0, 0, 0);
  bullet *sec_bullet_ptr = bullet_ptr;
  bullet *third_bullet_ptr = bullet_ptr;
  int number_of_bullets_are = 0;
  int count = 0;
  int x_star = 99;
  int y_star = 5;
  move(y, x);
  for (int i = 0; i < spaceship.size(); i++) {
    printw("%s\n", spaceship[i].c_str());
  }
  nodelay(stdscr, true);
  for (;;) {
    if (sec_star_ptr->x > -1) {
      sec_star_ptr->x--;
      move(sec_star_ptr->y, sec_star_ptr->x + 1);
      printw(" ");
      if (sec_star_ptr->x != -1) {
        move(sec_star_ptr->y, sec_star_ptr->x);
        printw(".");
      }
    } else {
      sec_star_ptr->x = myRand(20, 200);
    }
    if (sec_star_ptr->next != NULL) {
      sec_star_ptr = sec_star_ptr->next;
    } else {
      sec_star_ptr = star_ptr;
    }
    move(0, 0);
    int b = getch();
    usleep(4999);
    if (b == 32 && !sec_bullet_ptr->check) {
      sec_bullet_ptr->check = 1;
      sec_bullet_ptr->tick = 0;
      sec_bullet_ptr->y = y + 1;
      sec_bullet_ptr->x = x + 21;
      continue;
    }
    if (sec_bullet_ptr->check == 1 && b == 32 && number_of_bullets_are < 10) {
      third_bullet_ptr->next = create_memory_for_bullet(x + 21, y + 1, 1);
      third_bullet_ptr = third_bullet_ptr->next;
      number_of_bullets_are++;
    }
    if (sec_bullet_ptr->check == 1) {
      shoot(sec_bullet_ptr);
    }
    if (sec_bullet_ptr->next != NULL) {
      sec_bullet_ptr = sec_bullet_ptr->next;
    } else {
      sec_bullet_ptr = bullet_ptr;
    }
    move_space_ship(x, y, b);
  }
}
