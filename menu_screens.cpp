#include "menu_screens.h"
#include "utils.h"
#include <atomic>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <json-c/json.h>
#include <string.h>
#include <thread>
#include <unistd.h>
#include <curses.h>

using namespace std;

void display_death_menu() {
  int x = 30;
  int y = 0;
  attron(COLOR_PAIR(3));
  mvprintw(y++, x,
           "000000000     000000000           0      00000000000 0        0");
  mvprintw(y++, x,
           "0        0    0                  0 0          0      0        0");
  mvprintw(y++, x,
           "0         0   0                 0   0         0      0        0");
  mvprintw(y++, x,
           "0          0  0                0     0        0      0        0");
  mvprintw(y++, x,
           "0           0 0               0       0       0      0        0");
  mvprintw(y++, x,
           "0           0 000000000      00000000000      0      0000000000");
  mvprintw(y++, x,
           "0          0  0             0           0     0      0        0");
  mvprintw(y++, x,
           "0         0   0            0             0    0      0        0");
  mvprintw(y++, x,
           "0        0    0           0               0   0      0        0");
  mvprintw(y++, x,
           "0       0     0          0                 0  0      0        0");
  mvprintw(y++, x,
           "00000000      000000000 0                   0 0      0        0");
  mvprintw(y++, x, "                             _________ ");
  mvprintw(y++, x, "                            |         |");
  mvprintw(y++, x, "                            | Restart |");
  mvprintw(y++, x, "                            |_________|");
  mvprintw(y++, x, "                             __________ ");
  mvprintw(y++, x, "                            |          |");
  mvprintw(y++, x, "                            | Mainmenu |");
  mvprintw(y++, x, "                            |__________|");
  attroff(COLOR_PAIR(3));
  while (1) {
  }
}

void display_controls() {

  struct Controls controls = get_controls();
  int x = 30;
  int y = 0;
  int option = 1;
  mvprintw(y++, x,
           "000000000 0 0 0 0 0 0 0 0 0 0 0 0           0 00000000000 "
           "000000000 0 0 0 0 0 0 0 0 0 0 0 0           000000000");
  mvprintw(y++, x,
           "0         0                   0 0 0         0      0      0       "
           "0 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0  0        0      0      0       "
           "0 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0   0       0      0      "
           "000000000 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0    0      0      0      0        "
           " 0                   0 0           0        ");
  mvprintw(y++, x,
           "0         0                   0 0     0     0      0      00       "
           " 0                   0 0           000000000");
  mvprintw(y++, x,
           "0         0                   0 0      0    0      0      0 0      "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "0         0                   0 0       0   0      0      0  0     "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "0         0                   0 0        0  0      0      0   0    "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "0         0                   0 0         0 0      0      0    0   "
           " 0                   0 0                   0");
  mvprintw(y++, x,
           "000000000 0 0 0 0 0 0 0 0 0 0 0 0          0       0      0     0  "
           " 0 0 0 0 0 0 0 0 0 0 0 00000000000 000000000");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                          up-|___|-%s",
           controls.up);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                        down-|___|-%s",
           controls.down);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                        left-|___|-%s",
           controls.left);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                       right-|___|-%s",
           controls.right);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  mvprintw(y++, x, "                                              ___  ");
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                       shoot-|___|-%s",
           controls.shoot);
  mvprintw(y++, x, "                                             | . |");
  mvprintw(y++, x, "                                             |___|");
  refresh();
  char input;
  int llos;
  while (input != 27) {
    switch (llos) {
    case 13:
      mvprintw(13, x, "                                          up-|___|-%s ",
               controls.up);
    case 18:
      mvprintw(18, x, "                                        down-|___|-%s ",
               controls.down);
    case 23:
      mvprintw(23, x, "                                        left-|___|-%s ",
               controls.left);
    case 28:
      mvprintw(28, x, "                                       right-|___|-%s ",
               controls.right);
    case 33:
      mvprintw(33, x, "                                       shoot-|___|-%s ",
               controls.shoot);
    }
    if (input == 'w' && option != 1) {
      option--;
    } else if (input == 's' && option != 5) {
      option++;
    } else if (input == 'r') {
      char newControl = (char)getch();
      switch (option) {
      case 1:
        controls.up = &newControl;
        set_controls(controls);
        break;
      case 2:
        controls.down = &newControl;
        set_controls(controls);
        break;
      case 3:
        controls.left = &newControl;
        set_controls(controls);
        break;
      case 4:
        controls.right = &newControl;
        set_controls(controls);
        break;
      case 5:
        controls.shoot = &newControl;
        set_controls(controls);
        break;
      }
    }
    switch (option) {
    case 1:
      mvprintw(13, x, "                                         *up-|___|-%s*",
               controls.up);
      llos = 13;
      break;
    case 2:
      mvprintw(18, x, "                                       *down-|___|-%s*",
               controls.down);
      llos = 18;
      break;
    case 3:
      mvprintw(23, x, "                                       *left-|___|-%s*",
               controls.left);
      llos = 23;
      break;
    case 4:
      mvprintw(28, x, "                                      *right-|___|-%s*",
               controls.left);
      llos = 28;
      break;
    case 5:
      mvprintw(33, x, "                                      *shoot-|___|-%s*",
               controls.left);
      llos = 33;
      break;
    }
    input = getch();
  }
}

void display_levels() {
  int x = 0;
  int y = 0;
  int option = 1;
  clear();
  attron(COLOR_PAIR(5));
  mvprintw(y++, x,
           "                                                  0            "
           "0000000000 0                   0   0000000000  0    0000000000");
  mvprintw(y++, x,
           "         		                          0           0        "
           "    0                 0   0            0    0");
  mvprintw(y++, x,
           "	 		                          0           0        "
           "     0               0    0            0    0        ");
  mvprintw(y++, x,
           "	 		                          0           0        "
           "      0             0     0            0    0        ");
  mvprintw(y++, x,
           "	 		                          0           0        "
           "       0           0      0            0    0        ");
  mvprintw(y++, x,
           "	 		                          0            "
           "0000000000      0         0        0000000000  0     000000000");
  mvprintw(y++, x,
           "	 		                          0           0        "
           "         0       0        0            0             0");
  mvprintw(y++, x,
           "	 		                          0           0        "
           "          0     0         0            0             0");
  mvprintw(y++, x,
           "	 		                          0           0        "
           "           0   0          0            0             0");
  mvprintw(y++, x,
           "	 		                          0           "
           "00000000000           0            00000000000  0     000000000");
  mvprintw(y++, x,
           "	                +         +        +     ________  ________  "
           "________  ________  ________  ________  ________  ________     +   "
           "      +        + ");
  mvprintw(y++, x,
           "	                 \\________|_______/     |        ||        || "
           "       ||        ||        ||        ||        ||        |     "
           "\\________|_______/");
  mvprintw(y++, x,
           "	                  |*******+******|------|   (1)  ||  (2)   ||  "
           "(3)   ||  (4)   ||  (5)   ||  (6)   ||  (7)   ||   (8)  "
           "|------|*******+******|");
  mvprintw(y++, x,
           "	                  "
           "|*******+******|------|________||________||________||________||____"
           "____||________||________||________|------|*******+******|");
  mvprintw(y++, x,
           "	                  |*******+******|       ________  ________  "
           "________  ________  ________  ________  ________  ________       "
           "|*******+******|");
  mvprintw(y++, x,
           "	                  |*******+******|------|        ||        ||  "
           "      ||        ||        ||        ||        ||        "
           "|------|*******+******|");
  mvprintw(y++, x,
           "	                  |*******+******|------|  (9)   ||  (10)  ||  "
           "(11)  ||  (12)  ||  (13)  ||  (14)  ||  (15)  ||  (16)  "
           "|------|*******+******|");
  mvprintw(y++, x,
           "	                  |*******+******|      "
           "|________||________||________||________||________||________||______"
           "__||________|      |*******+******|");
  mvprintw(y++, x,
           "	                 |\\--------------/|     ________  ________  "
           "________  ________  ________  ________  ________  ________       "
           "|\\--------------/|");
  mvprintw(y++, x,
           "	                   !!!!!!!!!!!!!!       |        ||        ||  "
           "      ||        ||        ||        ||        ||        |       "
           "!!!!!!!!!!!!!!");
  mvprintw(y++, x,
           "	                   !!!!!!!!!!!!!!       |  (17)  ||  (18)  ||  "
           "(19)  ||  (20)  ||  (21)  ||  (22)  ||  (23)  ||  (24)  |       "
           "!!!!!!!!!!!!!!");
  mvprintw(y++, x,
           "	                   !!!!!!!!!!!!!!       "
           "|________||________||________||________||________||________||______"
           "__||________|       !!!!!!!!!!!!!!");
  mvprintw(y++, x,
           "	                   !!!!!!!!!!!!!!        ________  ________  "
           "________  ________  ________  ________  ________  ________        "
           "!!!!!!!!!!!!!! ");
  mvprintw(y++, x,
           "	                                        |        ||        ||  "
           "      ||        ||        ||        ||        ||        |");
  mvprintw(y++, x,
           "	                                        |  (25)  ||  (26)  ||  "
           "(27)  ||  (28)  ||  (29)  ||  (30)  ||  (31)  ||  (32)  |");
  mvprintw(y++, x,
           "	                                        "
           "|________||________||________||________||________||________||______"
           "__||________|");
  mvprintw(y++, x,
           "	                                         ________  ________  "
           "________  ________  ________  ________  ________  ________");
  mvprintw(y++, x,
           "	                                        |        ||        ||  "
           "      ||        ||        ||        ||        ||        |");
  mvprintw(y++, x,
           "	                                       /|  (33)  ||  (34)  ||  "
           "(35)  ||  (36)  ||  (37)  ||  (38)  ||  (39)  ||  (40)  |\\");
  mvprintw(y++, x,
           "                                              "
           "/#|________||________||________||________||________||________||____"
           "____||________|#\\");
  mvprintw(
      y++, x,
      "                                             /#/                        "
      "                                                        \\#\\");
  mvprintw(
      y++, x,
      "                                            /#/                         "
      "                                                         \\#\\");
  attroff(COLOR_PAIR(5));
  attron(COLOR_PAIR(3));
  mvprintw(y++, x,
           "                                 +_________/#/________+            "
           "                                                      "
           "+________\\#\\ _________+");
  mvprintw(y++, x,
           "                                \\$$$$$$$$$/#/$$$$$$$$$/           "
           "                                                       "
           "\\$$$$$$$$$\\#\\$$$$$$$$$/");
  mvprintw(y++, x,
           "                               {/----------+----------\\}          "
           "                                                      "
           "{/----------+----------\\}");
  mvprintw(y++, x,
           "                               {[(*********+**********)]}          "
           "                                                      "
           "{[(*********+**********)]}");
  mvprintw(y++, x,
           "                               {[(*********+**********)]}          "
           "                                                      "
           "{[(*********+**********)]}");
  mvprintw(y++, x,
           "                               |\\--------------------/|           "
           "                                                      "
           "|\\--------------------/|");
  mvprintw(
      y++, x,
      "                                   !!!!!!!!!!!!!!!!!!                   "
      "                                                    !!!!!!!!!!!!!!!!!");
  attroff(COLOR_PAIR(3));
  refresh();
  char input;
  while (input != 10) {
    if (input == 'w' && option <= 8) {
      option -= 8;
    } else if (input == 's' && option <= 33) {
      option += 8;
    } else if (input == 'd' && option % 8 != 0) {
      option++;
    } else if (input == 'a' && option % 8 != 1) {
      option--;
    } else if (input == 27) {
      return;
    }
    input = getch();
  }
  spaceShipGame(option);
}

void main_menu() {
  int x = 30;
  int y = 0;
  int option = 1;
  attron(COLOR_PAIR(4));
  mvprintw(y++, x,
           "000000000 000000000      0            000000000 000000000 "
           "000000000 0        0 00000000000 000000000");
  mvprintw(y++, x,
           "0         0       0     0 0           0         0         0        "
           " 0        0      0      0       0");
  mvprintw(y++, x,
           "0         0       0    0   0          0         0         0        "
           " 0        0      0      0       0");
  mvprintw(y++, x,
           "0         000000000   0     0         0         0         0        "
           " 0        0      0      000000000");
  mvprintw(y++, x,
           "0         0          0       0        0         0         0        "
           " 0        0      0      0");
  mvprintw(y++, x,
           "000000000 0         00000000000       0         000000000 "
           "000000000 0000000000      0      0");
  mvprintw(y++, x,
           "        0 0        0           0      0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "        0 0       0             0     0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "        0 0      0               0    0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "        0 0     0                 0   0         0                 "
           "0 0        0      0      0");
  mvprintw(y++, x,
           "000000000 0    0                   0  000000000 000000000 "
           "000000000 0        0 00000000000 0");

  y += 10;
  x += 40;

  mvprintw(y++, x, " /----\\ ");
  mvprintw(y++, x, "|levels|");
  mvprintw(y++, x, " \\----/ ");
  y++;
  mvprintw(y++, x, " /------\\ ");
  mvprintw(y++, x, "|controls|");
  mvprintw(y++, x, " \\------/ ");
  y++;
  x += 2;
  mvprintw(y++, x, " /--\\ ");
  mvprintw(y++, x, "|quit|");
  mvprintw(y++, x, " \\--/ ");

  refresh();
  char input;
  int llos;
  while (true) {
    switch (option) {
    case 1:
      mvprintw(22, x - 3, "*|levels|*");
      llos = 22;
      break;
    case 2:
      mvprintw(26, x - 3, "*|controls|*");
      llos = 26;
      break;
    case 3:
      mvprintw(30, x - 3, "  *|quit|*");
      llos = 30;
      break;
    }
    input = getch();
    if (input == 'w' && option != 1) {
      option--;
    } else if (input == 's' && option != 3) {
      option++;
    } else if (input == 10) { // ASCII code for Enter
      attroff(COLOR_PAIR(4));
      switch (option) {
      case 1:
        display_levels();
        break;
      case 2:
        display_controls();
        break;
      case 3:
        endwin();
        exit(0);
      }
      attron(COLOR_PAIR(4));
    }
    switch (llos) {
    case 22:
      mvprintw(22, x - 3, " |levels| ");
      break;
    case 26:
      mvprintw(26, x - 3, " |controls| ");
      break;
    case 30:
      mvprintw(30, x - 3, "   |quit| ");
      break;
    }
  }
}
