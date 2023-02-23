#include <curses.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

// Function to generate random numbers between a given range
int myRand(int fromA, int toB) {
  if (fromA > toB)
    return myRand(toB, fromA);
  int d = toB - fromA;
  int r = rand() % (d + 1);
  return r + fromA;
}

// Function to read the contents of a file into a string
string ReadFileIntoString(string path) {
  ifstream file(path);
  string content((istreambuf_iterator<char>(file)),
                 istreambuf_iterator<char>());
  return content;
}

// Function to print game over screen
void print_game_over_screen(int x, int y) {
 
  clear();
  move(0 + y, 0 + x);
  printw(
      " ______________________    _   ______________   ____________________");
  move(1 + y, 0 + x);
  printw("|\\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  ||| |              | |\\ \\ \\ "
         "\\ \\ \\ \\ \\ \\ \\ \\");
  move(2 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| |              | | / / / / / / / / / / |");
  move(3 + y, 0 + x);
  printw("|\\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  | ||| |              | |\\ \\ \\ "
         "\\ \\ \\ \\ \\ \\ \\  |");
  move(4 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| |              | | / / / / / / / / / / |");
  move(5 + y, 0 + x);
  printw(
      "|:| |:| |:| |:| |:| |:| | ||| |  ____________| | |:| |:| |:| |:| |:| |");
  move(6 + y, 0 + x);
  printw("||/ \\|/ \\|/ \\|/ \\|/ \\|/ | |-| | |              | \\|/ \\|/ \\|/ "
         "\\|/ \\|/ |");
  move(7 + y, 0 + x);
  printw(
      "|:< >:< >:< >:< >:< >:< | ||| | |              | >:< >:< >:< >:< >:< |");
  move(8 + y, 0 + x);
  printw("||/ \\|/ \\|/ \\|/ \\|/ \\|/ | |-| | |__________    | \\|/ \\|/ \\|/ "
         "\\|/ \\|/ |");
  move(9 + y, 0 + x);
  printw(
      "|:< >:< >:< >:< >:< >:< | ||| |            |   | >:< >:< >:< >:< >:< |");
  move(10 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| |            |   | / / / / / / / / / / |");
  move(11 + y, 0 + x);
  printw("|\\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  | ||| |            |   |\\ \\ \\ "
         "\\ \\ \\ \\ \\ \\ \\  |");
  move(12 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| |            |   | / / / / / / / / / / |");
  move(13 + y, 0 + x);
  printw(
      "|:| |:| |:| |:| |:| |:| | ||| |  __________|   | |:| |:| |:| |:| |:| |");
  move(14 + y, 0 + x);
  printw("||/ \\|/ \\|/ \\|/ \\|/ \\|/ | |-| | |              | \\|/ \\|/ \\|/ "
         "\\|/ \\|/ |");
  move(15 + y, 0 + x);
  printw(
      "|:< >:< >:< >:< >:< >:< | ||| | |              | >:< >:< >:< >:< >:< |");
  move(16 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| | |____________  | / / / / / / / / / / |");
  move(17 + y, 0 + x);
  printw("|\\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  | ||| |              | |\\ \\ \\ "
         "\\ \\ \\ \\ \\ \\ \\  |");
  move(18 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| |              | | / / / / / / / / / / |");
  move(19 + y, 0 + x);
  printw("|\\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\  | ||| |              | |\\ \\ \\ "
         "\\ \\ \\ \\ \\ \\ \\  |");
  move(20 + y, 0 + x);
  printw(
      "| / / / / / / / / / / / | |-| |              | | / / / / / / / / / / |");
  move(21 + y, 0 + x);
  printw(
      "|_______________________/ |_| |______________| |_____________________/");

  // button restrart
  move(100, 100);
  printw(" _________ ");
  move(101, 100);
  printw("|         |");
  move(102, 100);
  printw("| restart |");
  move(103, 100);
  printw("|_________|");

  while (getch() != 27) {
  }
}

// Struct to store bullet data
struct bullet {
  int y;
  int x;
  int tick;
  bool valid_bullet;
  bullet *next;
};

// Struct to store enemy data
struct enemy {
  int x;
  int y;
  int check;
  enemy *next;
};

// Struct to store star data
struct star {
  int y = 0;
  int x = 0;
  star *next;
};

// Vector of strings to store the spaceship design
vector<string> spaceship{"           \\\\\\_____", "        ###[==_____>",
                         "           ///     "};

// Vector of strings to store the spaces used to remove the spaceship
vector<string> number_of_spaces_for_spaceship{"        ", "            ",
                                              "   "};

// Vector of strings to store the enemy spaceship design
vector<string> enemy_spaceship{"\\____/", "<_/__~0_\\", "(/_______\\)"};

// Vector of strings to store the spaces used to remove the enemy spaceship
vector<string> number_of_spaces_for_alien_spaceship{"      ", "          ",
                                                    "           "};

// Function to remove the spaceship from its current position
void remove_space_ship(int &x, int &y) {
  int Y = y;
  int X = x;
  for (int i = 0; i < number_of_spaces_for_spaceship.size(); i++) {
    move(Y, X);
    printw("%s\n", number_of_spaces_for_spaceship[i].c_str());
    Y++;
  }
  move(y, x);
}
void remove_alien_spaceship(int &x, int &y) {
  int Y = y;
  int X = x;
  for (int i = 0; i < number_of_spaces_for_alien_spaceship.size(); i++) {
    move(Y, X);
    printw("%s\n", number_of_spaces_for_alien_spaceship[i].c_str());
    Y++;
  }
}
void move_space_ship(int &x, int &y, int key) {
  // remove the existing spaceship from the current position
  remove_space_ship(x, y);

  // check the key pressed and move the spaceship accordingly
  switch (key) {
  case 115:
    // move down
    y++;
    break;
  case 119:
    // move up
    y--;
    break;
  case 100:
    // move right
    x++;
    break;
  case 97:
    // move left
    x--;
    break;
  }

  // draw the spaceship at the new position
  int Y = y;
  for (int i = 0; i < spaceship.size(); i++) {
    move(Y, x);
    printw("%s\n", spaceship[i].c_str());
    Y++;
  }
}
// This function moves the alien spaceship based on the key press.
// It first removes the current position of the spaceship using the
// remove_alien_spaceship function. Then, it changes the position of the
// spaceship based on the key press. getch is the input from the keyboard.
void move_aline_space_ship(int &y, int &x, int getch) {
  remove_alien_spaceship(y, x); // Remove the current position of the spaceship

  switch (
      getch) { // Check the value of getch and move the spaceship accordingly
  case 1:
    y--; // Move up
    break;
  case 2:
    y++; // Move down
    break;
  case 3:
    x--; // Move left
    break;
  case 4:
    x++; // Move right
    break;
  }

  int Y = y; // Save the new y-coordinate in a new variable Y
  // Draw the spaceship at the new position
  for (auto i = 0; i < enemy_spaceship.size(); i++) {
    move(Y, x);                                 // Go to the new position
    printw("%s\n", enemy_spaceship[i].c_str()); // Print the spaceship character
    Y++;                                        // Move to the next line
  }
}

// Indicator function, displays a cursor or selector on the main menu
void indicator(int number, int y, int x) {
  // Calculate the position of the cursor based on the given number
  number *= 4;
  move(y + number, x);
  printw("----------------------"); // Print top horizontal line
  move(y + number, x - 34);
  printw("----------------------"); // Print bottom horizontal line

  // Print the diagonal lines to create the cursor shape
  move(y - 1 + number, x - 35);
  printw("                      \\");
  move(y - 2 + number, x - 36);
  printw("                      \\");
  move(y + 1 + number, x - 35);
  printw("                      /");
  move(y + 2 + number, x - 36);
  printw("                      /");
  move(y + 2 + number, x + 1);
  printw("\\");
  move(y - 1 + number, x);
  printw("/");
  move(y + 1 + number, x);
  printw("\\");
  move(y - 2 + number, x + 1);
  printw("/");
}

// Remove Indicator function, removes the cursor or selector from the main menu
void remove_indicator(int number, int y, int x) {
  // Calculate the position of the cursor based on the given number
  number *= 4;
  x = 95;
  move(y + number, x);
  printw("                      "); // Print whitespace over the top horizontal
                                    // line
  move(y + number, x - 34);
  printw("                      "); // Print whitespace over the bottom
                                    // horizontal line

  // Print whitespace over the diagonal lines to remove the cursor shape
  move(y - 1 + number, x - 35);
  printw("                        ");
  move(y - 2 + number, x - 36);
  printw("                        ");
  move(y + 1 + number, x - 35);
  printw("                       ");
  move(y + 2 + number, x - 36);
  printw("                       ");
  move(y - 2 + number, x + 1);
  printw(" ");
  move(y - 1 + number, x);
  printw(" ");
  move(y + 1 + number, x);
  printw("  ");
  move(y + 2 + number, x + 1);
  printw("  ");
}
void print_main_menu() {
  // move cursor to start position of the logo
  move(6, 70);
  // print first line of the logo
  printw("000000  000000   000000  000000  000000");
  // move cursor to the next line
  move(7, 70);
  // print second line of the logo
  printw("0       0    0   0    0  0       0     ");
  // move cursor to the next line
  move(8, 70);
  // print third line of the logo
  printw("000000  000000   000000  0       000000");
  // move cursor to the next line
  move(9, 70);
  // print fourth line of the logo
  printw("     0  0        0    0  0       0     ");
  // move cursor to the next line
  move(10, 70);
  // print fifth line of the logo
  printw("000000  0        0    0  000000  000000");
  // move cursor to the position of the buttons
  move(12, 84);
  // print the first button (levels)
  printw(" ______");
  move(13, 84);
  printw("|      |");
  move(14, 84);
  printw("| play |");
  move(15, 84);
  printw("|______|");
  // print the second button (controls)
  move(16, 84);
  printw(" ________");
  move(17, 84);
  printw("|        |");
  move(18, 84);
  printw("|controls|");
  move(19, 84);
  printw("|________|");
  // print the third button (exit)
  move(20, 84);
  printw(" ________");
  move(21, 84);
  printw("|        |");
  move(22, 84);
  printw("|  exit  |");
  move(23, 84);
  printw("|________|");
  // initialize the indicator position to 0
  int indicator_number = 0;
  // store the result of the getch() function
  int getch_result = 0;
  // x and y coordinates for indicator and remove_indicator
  int x, y;
  x = 95;
  y = 14;
  // display the indicator
  indicator(indicator_number, y, x);
  // loop until the user presses the enter key
  while (getch_result != 10) {
    // get the user input
    getch_result = getch();
    // move the indicator up if the user presses 'w'
    if (getch_result == 119) {
      remove_indicator(indicator_number, y, x);
      if (indicator_number > 0) {
        indicator_number -= 1;
      } else {
        indicator_number = 2;
      }
      indicator(indicator_number, y, x);
    }
    // move the indicator down if the user presses 's'
    else if (getch_result == 115) {
      remove_indicator(indicator_number, y, x);
      if (indicator_number < 2) {
        indicator_number += 1;
      } else {
        indicator_number = 0;
      }
      indicator(indicator_number, y, x);
    }
  }
  // if the user has selected the exit button
  if (indicator_number == 2) {
    // exit the program
    system("killall a.out");
  }
  // Loop while indicator_number is equal to 1
  while (indicator_number == 1) {
    // Get the result of getch function
    int getch_result = getch();

    // Clear the screen
    clear();

    // Print the instructions for moving up
    move(11, 75);
    printw("Move Up - W");

    // Print the instructions for moving left
    move(14, 75);
    printw("Move Left - A");

    // Print the instructions for moving down
    move(17, 75);
    printw("Move Down - S");

    // Print the instructions for moving right
    move(20, 75);
    printw("Move Right - D");

    // Print the instructions for shooting
    move(23, 75);
    printw("Shoot - Space");

    // Print the instructions for turning off/on the music
    move(26, 75);
    printw("Turn off/on the music - M");

    // Check if the result of getch function is 27
    if (getch_result == 27) {
      // Execute the restart_program command
      system("restart_program");
    }
  }

  // Check if the indicator_number is equal to 0
  if (indicator_number == 0) {
    // Clear the screen
    clear();

    // Print the first game mode
    move(12, 84);
    printw(" _______ ");
    move(13, 84);
    printw("|       |");
    move(14, 84);
    printw("| Story |");
    move(15, 84);
    printw("|_______|");

/*    // Print the second game mode
    move(16, 84);
    printw(" ______________");
    move(17, 84);
    printw("|              |");
    move(18, 84);
    printw("| Multi-player |");
    move(19, 84);
    printw("|______________|"); */
    // initialize the inner_indicator position to 0
    int inner_indicator_number = 0;
    // initalize the x and y coordinates for indicator and remove_indicator
    int inner_x, inner_y = 0;
    inner_x = 95;
    inner_y = 14;
    // store the result of the getch() function
    int getch_result = 0;
    // display the indicator
    indicator(inner_indicator_number, inner_y, inner_x);
    // loop until the user presses the enter key
    while (getch_result != 10) {
      // get the user input
      getch_result = getch();
      // move the indicator up if the user presses 'w'
      if (getch_result == 119) {
        remove_indicator(inner_indicator_number, inner_y, inner_x);
        if (inner_indicator_number > 0) {
          inner_indicator_number -= 1;
        } else {
          inner_indicator_number = 0;
        }
        indicator(inner_indicator_number, inner_y, inner_x);
      }
      // move the indicator down if the user presses 's'
      else if (getch_result == 115) {
        remove_indicator(inner_indicator_number, inner_y, inner_x);
        if (inner_indicator_number < 0) {
          inner_indicator_number += 1;
        } else {
          inner_indicator_number = 0;
        }
        indicator(inner_indicator_number, inner_y, inner_x);
      }
    }
  }

  // Clear the screen
  clear();
}

void print_health_bar(int x) {
  // print the health bar in the specified x coordinate
  move(0, x);
  printw("   _ _");
  move(1, x);
  printw(" /\\  /\\");
  move(2, x);
  printw(" | \\/  |");
  move(3, x);
  printw("  \\   /");
  move(4, x);
  printw("   \\ /");
  move(5, x);
  printw("    *");
}
void print_game_over(string path) {
  // clear the screen and print the contents of the file located at path
  clear();
  printf("%s", ReadFileIntoString(path).c_str());
  // infinite loop
  while (1 == 1)
    ;
}
bullet *create_memory_for_bullet(int x, int y, int check) {
  // allocate memory for a bullet object and set its properties
  bullet *ptr = (bullet *)malloc(sizeof(bullet));
  ptr->x = x;
  ptr->y = y;
  ptr->valid_bullet = check;
  return ptr;
}
star *create_memory_for_star(int y, int x) {
  // allocate memory for a star object and set its properties
  star *ptr = (star *)malloc(sizeof(bullet));
  ptr->y = y;
  ptr->x = x;
  return ptr;
}
enemy *create_memory_for_enemy(int y, int x) {
  // allocate memory for an enemy object and set its properties
  enemy *ptr = (enemy *)malloc(sizeof(enemy));
  ptr->y = y;
  ptr->x = x;
  return ptr;
}
void main_shoot(bullet *sec_bullet_ptr, enemy *enemy2) {
  // move the bullet and update its position and state
  if (sec_bullet_ptr->tick != 121) {
    sec_bullet_ptr->tick++;
    move(sec_bullet_ptr->y, sec_bullet_ptr->x);
    printw("-");
    if (sec_bullet_ptr->x == enemy2->x) {
      remove_alien_spaceship(enemy2->x, enemy2->y);
    }
    move(sec_bullet_ptr->y, sec_bullet_ptr->x - 1);
    printw(" ");
    sec_bullet_ptr->x++;
  } else {
    sec_bullet_ptr->valid_bullet = false;
    move(sec_bullet_ptr->y, sec_bullet_ptr->x - 1);
    printw(" ");
  }
}
int main(int argc, char **argv) {
  int health_bar = 3;       // Initialize the health bar to 3
  bool runningmusic = true; // Initialize a flag for running music
  ma_result result; // Initialize a variable for the result of ma_engine_init()
                    // function
  ma_engine engine; // Initialize the audio engine

  // Initialize the audio engine
  result = ma_engine_init(NULL, &engine);
  if (result !=
      MA_SUCCESS) { // Check if the audio engine initialization is successful
    printf("Failed to initialize audio engine."); // Print error message
    return -1;                                    // Return failure
  }

  // Play the audio file
  ma_engine_play_sound(&engine, "../Spaceship/Creo_-_Dimension.mp3", NULL);

  // Initialize the ncurses library
  setlocale(LC_ALL, "");
  initscr();
  curs_set(false); // Hide the cursor
  noecho();        // Do not display typed characters

  // Print the main menu
  print_main_menu();

  // Enable the keypad for standard screen
  keypad(stdscr, true);

  // Allocate memory for the stars
  star *star_ptr = create_memory_for_star(myRand(1, 10), myRand(100, 200));
  star *sec_star_ptr = star_ptr;

  // Allocate memory for the enemies
  enemy *enemys = create_memory_for_enemy(myRand(1, 10), myRand(100, 200));
  enemy *enemys2 = enemys;

  // Create additional stars
  for (int i = 0; i < 5; i++) {
    star_ptr->next = create_memory_for_star(myRand(5, 40), myRand(20, 200));
    star_ptr = star_ptr->next;
  }

  // Create additional enemies
  for (int i = 0; i < 0; i++) {
    enemys2->next = create_memory_for_enemy(myRand(1, 10), myRand(100, 200));
    enemys2 = enemys2->next;
  }

  // Reset the pointers to the start of the linked lists
  enemys2 = enemys;
  star_ptr = sec_star_ptr;

  // Initialize variables for the spaceship position
  auto x = 0;
  auto y = 0;

  // Initialize variables for the alien spaceship position
  auto x_alien = 90;
  auto y_alien = 5;

  // Initialize a variable to store the x-coordinate of the alien spaceship's
  // shot
  int x_shoot_for_alien_spaceship = x_alien;

  // This line creates memory for the first bullet
  bullet *bullet_ptr_for_spaceship = create_memory_for_bullet(0, 0, 0);

  // This line assigns the first bullet pointer to a second pointer
  bullet *sec_bullet_ptr_for_spaceship = bullet_ptr_for_spaceship;

  // This line assigns the first bullet pointer to a third pointer
  bullet *third_bullet_ptr_for_spaceship = bullet_ptr_for_spaceship;

  // This line initializes a variable to keep track of the number of bullets
  int number_of_bullets_are = 0;

  // This line initializes a counter variable
  int count = 0;

  // These lines initialize variables for the position of the star
  int x_star = 99;
  int y_star = 5;

  // This line moves the cursor to a specific position
  move(y, x);

  // This loop prints the spaceship on the screen
  for (int i = 0; i < spaceship.size(); i++) {
    printw("%s\n", spaceship[i].c_str());
  }

  // This line enables non-blocking input
  nodelay(stdscr, true);

  // This infinite loop updates the state of the game
  for (;;) {
    // This block moves the star
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

    // This line gets the character input
    int character = getch();

    // This line introduces a delay
    usleep(4999);

    /* Check if character is a space and if
     * sec_bullet_ptr_for_spaceship->valid_bullet is false */
    if (character == 32 && !sec_bullet_ptr_for_spaceship->valid_bullet) {
      /* Set sec_bullet_ptr_for_spaceeship->valid_bullet to true */
      sec_bullet_ptr_for_spaceship->valid_bullet = true;
      /* Reset sec_bullet_ptr_for_spaceship->4ick */
      sec_bullet_ptr_for_spaceship->tick = 0;
      /* Set the y-coordinate of sec_bullet_ptr_for_spaceship */
      sec_bullet_ptr_for_spaceship->y = y + 1;
      /* Set the x-coordinate of sec_bullet_ptr_for_spaceship */
      sec_bullet_ptr_for_spaceship->x = x + 21;
    }

    /* Check if sec_bullet_ptr_for_spaceeship->valid_bullet is true, character
     * is a space and the number of bullets is less than 5 */
    if (/*sec_bullet_ptr_for_spaceship->valid_bullet == true && */ character ==
            32 &&
        number_of_bullets_are < 5) {
      /* Create a new bullet and set it as the next bullet of
       * third_bullet_ptr_for_spaceship */
      third_bullet_ptr_for_spaceship->next =
          create_memory_for_bullet(x + 21, y + 1, 1);
      /* Update third_bullet_ptr_for_spaceship to point to the newly created
       * bullet */
      third_bullet_ptr_for_spaceship = third_bullet_ptr_for_spaceship->next;
      /* Increase the number of bullets */
      number_of_bullets_are++;
    }

    /* Check if sec_bullet_ptr_for_spaceeship->valid_bullet is true */
    if (sec_bullet_ptr_for_spaceship->valid_bullet == true) {
      /* Call main_shoot function with sec_bullet_ptr_for_spaceship as argument
       */
      main_shoot(sec_bullet_ptr_for_spaceship, enemys2);
    }

    /* Check if sec_bullet_ptr_for_spaceship has a next bullet */
    if (sec_bullet_ptr_for_spaceship->next != NULL) {
      /* Update sec_bullet_ptr_for_spaceship to point to the next bullet */
      sec_bullet_ptr_for_spaceship = sec_bullet_ptr_for_spaceship->next;
    } else {
      /* Reset sec_bullet_ptr_for_spaceship to point to bullet_ptr_for_spaceship
       */
      sec_bullet_ptr_for_spaceship = bullet_ptr_for_spaceship;
    }
    // Check if enemy's check is 0 and its x coordinate is greater than 0
    if (!enemys2->check && enemys2->x > 0) {
      // Sleep for 4 milliseconds
      usleep(4000);
      // Set enemy's check to 1
      enemys2->check = 1;

      // Check if enemy's x coordinate is greater than 0
    } else if (enemys2->x > 0) {
      // Check if enemy's y coordinate is greater than the player's y coordinate
      // and less than player's y coordinate + 3 and its x coordinate is less
      // than or equal to player's x coordinate + 9 OR its y coordinate is
      // greater than player's y coordinate and less than player's y coordinate
      // + 3 and its x coordinate is less than or equal to player's x coordinate
      // + 12 OR its y coordinate is greater than player's y coordinate and less
      // than player's y coordinate + 3 and its x coordinate is less than or
      // equal to player's x coordinate + 3
      if (enemys2->y > y && enemys2->y < y + 3 && enemys2->x <= x + 9 ||
          enemys2->y > y && enemys2->y < y + 3 && enemys2->x <= x + 12 ||
          enemys2->y > y && enemys2->y < y + 3 && enemys2->x <= x + 3) {
        // Check if health bar is 0
        health_bar--;
        if (health_bar == 0) {
          // Call function to print game over screen
          print_game_over_screen(0, 0);
        }
        // Decrement enemy's x coordinate
        enemys2->x--;
        // Move cursor to enemy's current y and x coordinate
        move(enemys2->y, enemys2->x);
        // Print "-"
        printw("-");
        // Move cursor to next position
        move(enemys2->y, enemys2->x + 1);
        // Print a space
        printw(" ");
        // Check if enemy's x coordinate is less than 0
        if (enemys2->x < 0) {
          // Set enemy's check to 0
          enemys2->check = 0;
          // Move cursor to next position
          move(enemys2->y, enemys2->x + 1);
          // Print a space
          printw(" ");
        }
      }
      // Call function to move the enemy
      move_aline_space_ship(enemys2->y, enemys2->x, myRand(3, 4));
      // Check if enemys2 has a next node in the linked list, if it does, set
      // enemys2 to the next node. If it does not, set enemys2 back to the head
      // of the linked list (enemys).
      if (enemys2->next != NULL) {
        enemys2 = enemys2->next;
      } else {
        enemys2 = enemys;
      }
      // Move the player's spaceship.
      move_space_ship(x, y, character);

      // Check if the value of `character` is 109, and if `runningmusic` is
      // true. If both conditions are met, stop the music engine and set
      // `runningmusic` to false. If `character` is 109 and `runningmusic` is
      // false, initialize the engine and play the specified sound file. Set
      // `runningmusic` to true.
      if (character == 109 && runningmusic == true) {
        ma_engine_stop(&engine);
        runningmusic = false;
      } else if (character == 109 && runningmusic == false) {
        ma_engine_init(NULL, &engine);
        ma_engine_play_sound(&engine, "../Spaceship/Creo_-_Dimension.mp3",
                             NULL);
        runningmusic = true;
      }
    }
  }
  endwin();
}
