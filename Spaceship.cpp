#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <cstdlib>
#include <limits.h>
#include <locale.h>
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
struct enemy {
  int x;
  int y;
  int check;
  int tick;
  enemy *next;
};
struct star {
  int y = 0;
  int x = 0;
  star *next;
};
vector<string> spaceship{"           \\\\\\_____", "        ###[==_____>",
                         "           ///     "};
vector<string> number_of_spaces_for_spaceship{"        ", "            ",
                                              "   "};
vector<string> enemy_spaceship{"\\____/", "<_/__~0_\\", "(/_______\\)"};
vector<string> number_of_spaces_for_alien_spaceship{"      ", "          ",
                                                    "           "};
void remove_space_ship(int &x, int &y) {
  int Y = y;
  int X = x;
  move(Y, X);
  for (int i = 0; i < number_of_spaces_for_spaceship.size(); i++) {
    if (i == 1) {
      X -= 1;
    }
    if (i == 2) {
      X -= 1;
    }
    printw("%s\n", number_of_spaces_for_spaceship[i].c_str());
    Y++;
    move(Y, X);
  }
  move(y, x);
}
void remove_alien_spaceship(int &x, int &y) {
  int Y = y;
  int X = x;
  move(Y, X);
  for (int i = 0; i < number_of_spaces_for_alien_spaceship.size(); i++) {
    printw("%s\n", number_of_spaces_for_alien_spaceship[i].c_str());
    Y++;
    move(Y, X);
  }
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
void move_aline_space_ship(int &y, int &x, int getch) {
  if (getch == KEY_UP) {
    remove_alien_spaceship(y, x);
    y--;
    move(y, x);
    int Y = y;
    for (auto i = 0; i < enemy_spaceship.size(); i++) {
      printw("%s\n", enemy_spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (getch == KEY_DOWN) {
    remove_alien_spaceship(y, x);
    y++;
    move(y, x);
    int Y = y;
    for (auto i = 0; i < enemy_spaceship.size(); i++) {
      printw("%s\n", enemy_spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (getch == KEY_LEFT) {
    remove_alien_spaceship(y, x);
    x--;
    move(y, x);
    int Y = y;
    for (auto i = 0; i < enemy_spaceship.size(); i++) {
      printw("%s\n", enemy_spaceship[i].c_str());
      Y++;
      move(Y, x);
    }
  }
  if (getch == KEY_RIGHT) {
    remove_alien_spaceship(y, x);
    x++;
    move(y, x);
    int Y = y;
    for (auto i = 0; i < enemy_spaceship.size(); i++) {
      printw("%s\n", enemy_spaceship[i].c_str());
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
  printw(" ________");
  move(13, 84);
  printw("|        |");
  move(14, 84);
  printw("| levels |");
  move(15, 84);
  printw("|________|");
  move(14, 88);
  int a = 0;
  while (a != 10) {
    move(0, 0);
    a = getch();
  }
  a = 0;
  clear();
  move(2, 4);
  printw("-----");
  move(3, 4);
  printw("| 1 |");
  move(4, 4);
  printw("-----");
  move(2, 12);
  printw("-----");
  move(3, 12);
  printw("| 2 |");
  move(4, 12);
  printw("-----");
  move(2, 20);
  printw("-----");
  move(3, 20);
  printw("| 3 |");
  move(4, 20);
  printw("-----");

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
enemy *create_memory_for_enemy(int y, int x) {
  enemy *ptr = (enemy *)malloc(sizeof(enemy));
  ptr->y = y;
  ptr->x = x;
  return ptr;
}
void main_shoot(bullet *sec_bullet_ptr) {
  if (sec_bullet_ptr->tick != 121) {
    sec_bullet_ptr->tick++;
    move(sec_bullet_ptr->y, sec_bullet_ptr->x);
    if (sec_bullet_ptr->tick != 120) {
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
  int health_bar = 3;
  int conut = 1;
  ma_result result;
  ma_engine engine;
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize audio engine.");
    return -1;
  }
  ma_engine_play_sound(&engine, argv[1], NULL);
  setlocale(LC_ALL, "");
  initscr();
  curs_set(false);
  noecho();
  print_main_menu();
  keypad(stdscr, true);
  star *star_ptr = create_memory_for_star(myRand(1, 10), myRand(100, 200));
  star *sec_star_ptr = star_ptr;
  //
  enemy *enemys = create_memory_for_enemy(myRand(6, 10), myRand(30, 80));
  enemy *enemys2 = enemys;
  for (int i = 0; i < 5; i++) {
    star_ptr->next = create_memory_for_star(myRand(5, 40), myRand(20, 200));
    star_ptr = star_ptr->next;
  }
  for (int i = 0; i < 1; i++) {
    enemys2->next = create_memory_for_enemy(myRand(1, 10), myRand(100, 200));
    enemys2 = enemys2->next;
  }
  enemys2 = enemys;
  star_ptr = sec_star_ptr;
  auto x = 0;
  auto y = 0;
  auto x_alien = 90;
  auto y_alien = 5;
  int x_shoot_for_alien_spaceship = x_alien;
  int check = 0;
  bullet *bullet_ptr_for_spaceship = create_memory_for_bullet(0, 0, 0);
  bullet *sec_bullet_ptr_for_spaceship = bullet_ptr_for_spaceship;
  bullet *third_bullet_ptr_for_spaceship = bullet_ptr_for_spaceship;
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
    if (!health_bar) {
      return 0;
    }
    if (sec_star_ptr->x > -1) {
      sec_star_ptr->x--;
      move(sec_star_ptr->y, sec_star_ptr->x + 1);
      printw(" ");
      if (sec_star_ptr->x != -1) {
        move(sec_star_ptr->y, sec_star_ptr->x);
        printw("٭");
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
    // spaceship shoot
    if (b == 32 && !sec_bullet_ptr_for_spaceship->check) {
      sec_bullet_ptr_for_spaceship->check = 1;
      sec_bullet_ptr_for_spaceship->tick = 0;
      sec_bullet_ptr_for_spaceship->y = y + 1;
      sec_bullet_ptr_for_spaceship->x = x + 21;
      continue;
    }
    if (sec_bullet_ptr_for_spaceship->check == 1 && b == 32 &&
        number_of_bullets_are < 5) {
      third_bullet_ptr_for_spaceship->next =
          create_memory_for_bullet(x + 21, y + 1, 1);
      third_bullet_ptr_for_spaceship = third_bullet_ptr_for_spaceship->next;
      number_of_bullets_are++;
    }
    if (sec_bullet_ptr_for_spaceship->check == 1) {
      main_shoot(sec_bullet_ptr_for_spaceship);
    }
    if (sec_bullet_ptr_for_spaceship->next != NULL) {
      sec_bullet_ptr_for_spaceship = sec_bullet_ptr_for_spaceship->next;
    } else {
      sec_bullet_ptr_for_spaceship = bullet_ptr_for_spaceship;
    }
    // end spaceship shoot
    // enemy shoot
    if (!enemys2->check && enemys2->x > 0) {
      enemys2->check = 1;
      enemys2->tick = enemys2->x;
    } else if (enemys2->x > 0) {
      if (enemys2->y > y && enemys2->y < y + 3 && enemys2->tick <= x + 9 ||
          enemys2->y > y && enemys2->y < y + 3 && enemys2->tick <= x + 12 ||
          enemys2->y > y && enemys2->y < y + 3 && enemys2->tick <= x + 3) {
        return 0;
      }
      if (sec_bullet_ptr_for_spaceship->y >= enemys2->y &&
              sec_bullet_ptr_for_spaceship->y <= enemys2->y + 3 &&
              sec_bullet_ptr_for_spaceship->x == enemys2->x ||
          sec_bullet_ptr_for_spaceship->y >= enemys2->y &&
              sec_bullet_ptr_for_spaceship->y <= enemys2->y + 3 &&
              sec_bullet_ptr_for_spaceship->x == enemys2->x - 3) {
        enemys2->x = 0;
        continue;
      }
      move(enemys2->y, enemys2->tick);
      printw("-");
      move(enemys2->y, enemys2->tick + 1);
      printw(" ");
      enemys2->tick--;
      if (enemys2->tick < 0) {
        enemys2->check = 0;
      }
    }
    // end enemy shoot
    move_space_ship(x, y, b);
    if (conut % 4 == 0 && enemys2->x > 0) {
      move_aline_space_ship(enemys2->y, enemys2->x, KEY_LEFT);
    } else if (!enemys2->x) {
      remove_alien_spaceship(enemys2->x, enemys2->y);
    }
    if (enemys2->next != NULL) {
      enemys2 = enemys2->next;
    } else {
      enemys2 = enemys;
    }
    conut++;
  }
}
