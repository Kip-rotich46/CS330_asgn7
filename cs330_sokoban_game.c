#include <ncurses.h>
#include <stdlib.h>  /* for malloc, exit() */
#include <unistd.h>  /* for sleep, EOF */
#include <string.h>  /* for strcmp(), strcat()  */
#include "sok_header.h"

// compile this with ncurses linked:
// gcc -Wall -g cs330_sokoban_game.c -L. -lsok_helper_vulcan -o cs330_sokoban -lncurses

/* ======= TO DO ======= */
/* 
- write validMove() function
- write movePlayer() function
- test
*/

// Note: in starPusherLevels, largest map is 20 rows x 32 columns
// Note: to dereference a value: *((map + newPlayerY*MAP_COLS) + newPlayerX)

/* some variables we'll use in the game */
int MAP_COLS = 10;
int MAP_ROWS = 10;

// dX and dY are helpers for movement: up, right, down, left
int dX[4] = {0, 1, 0, -1};
int dY[4] = {-1, 0, 1, 0};

int numStarsSolved = 0;  // to represent the number of stars in the correct spot
int NUM_STEPS = 0;      // to keep score

WINDOW *title_bar;
WINDOW *main_win;
WINDOW *score_win;

int WIN_OFFSET_X = 5;
int WIN_OFFSET_Y = 0;
int WIN_WIDTH = 25;
int WIN_TITLE_HEIGHT = 3;
int WIN_MAIN_HEIGHT = 22;    
int WIN_SCORE_HEIGHT = 4;

/* draw the screen, given the map, number of rows, and number of columns */
int drawScreen(int rows, int cols, int *map) {
    int i = 0;
    int j = 0;
    int windowI, windowJ;
    for (i = 0; i < MAP_ROWS; i++) {
        for (j = 0; j < MAP_COLS; j++) {
            windowI = i + 1;  // adding one gets the map inside the border
            windowJ = j + 1;
            int thisSquare = *((map + i * rows) + j);
            if (thisSquare == 1) {
                mvwprintw(main_win, windowI, windowJ, "%c", '#');
            } else if (thisSquare == 2) {
                attron(A_BOLD);
                attron(COLOR_PAIR(2));
                mvwprintw(main_win, windowI, windowJ, "%c", 'P');
                mvwchgat(main_win, windowI, windowJ, 1, A_BOLD, 2, NULL);
                attroff(A_BOLD);
                attroff(COLOR_PAIR(2));
            } else if (thisSquare == 3) {
                attron(COLOR_PAIR(3));
                mvwprintw(main_win, windowI, windowJ, "%c", '$');
                mvwchgat(main_win, windowI, windowJ, 1, A_BOLD, 3, NULL);
                attroff(COLOR_PAIR(3));
            } else if (thisSquare == 4) {
                mvwprintw(main_win, windowI, windowJ, "%c", 'O');
            } else if (thisSquare == 5) {
                attron(COLOR_PAIR(5));
                mvwprintw(main_win, windowI, windowJ, "%c", '$');
                mvwchgat(main_win, windowI, windowJ, 1, A_BOLD, 5, NULL);
                attroff(COLOR_PAIR(5));
            } else if (thisSquare == 6) {
                attron(A_BOLD);
                attron(COLOR_PAIR(2));
                mvwprintw(main_win, windowI, windowJ, "%c", 'P');
                mvwchgat(main_win, windowI, windowJ, 1, A_BOLD, 2, NULL);
                attroff(A_BOLD);
                attroff(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(4));
                mvwprintw(main_win, windowI, windowJ, "%c", '.');
                mvwchgat(main_win, windowI, windowJ, 1, 0, 4, NULL);
                attroff(COLOR_PAIR(4));
            }
        }
    }
    wrefresh(main_win);
    return 0;
}

/* ========== Functions ========== */

/* Checks if a move is valid */
int validMove(int direction, Player *p, int *map) {
    int newPlayerX = p->x + dX[direction];
    int newPlayerY = p->y + dY[direction];

    // Check out-of-bounds conditions
    if (newPlayerX < 0 || newPlayerX >= MAP_COLS || newPlayerY < 0 || newPlayerY >= MAP_ROWS) {
        return 0; // Invalid move: Out of bounds
    }

    int *newSquare = (map + newPlayerY * MAP_COLS + newPlayerX);

    // Check if the new square is a wall
    if (*newSquare == 1) {
        return 0; // Invalid move: Wall
    }

    // Check if the new square contains a star
    if (*newSquare == 3 || *newSquare == 5) {
        int starNewX = newPlayerX + dX[direction];
        int starNewY = newPlayerY + dY[direction];

        // Check if pushing the star is out of bounds
        if (starNewX < 0 || starNewX >= MAP_COLS || starNewY < 0 || starNewY >= MAP_ROWS) {
            return 0; // Invalid move
        }

        int *starNewSquare = (map + starNewY * MAP_COLS + starNewX);

        // Check if the star can be pushed
        if (*starNewSquare == 0 || *starNewSquare == 4) {
            return 1; // Valid move
        } else {
            return 0; // Invalid move
        }
    }

    return 1; // Valid move
}

/* Moves the player and updates the map */
void movePlayer(int direction, Player *p, int *map) {
    int newPlayerX = p->x + dX[direction];
    int newPlayerY = p->y + dY[direction];
    int *currentSquare = (map + p->y * MAP_COLS + p->x);
    int *newSquare = (map + newPlayerY * MAP_COLS + newPlayerX);

    // Check if pushing a star
    if (*newSquare == 3 || *newSquare == 5) {
        int starNewX = newPlayerX + dX[direction];
        int starNewY = newPlayerY + dY[direction];
        int *starNewSquare = (map + starNewY * MAP_COLS + starNewX);

        if (*starNewSquare == 4) {
            *starNewSquare = 5; // Star moves onto goal
            numStarsSolved++;
        } else {
            *starNewSquare = 3; // Star moves to empty space
        }

        *newSquare = 0; // Star moves away
    }

    // Update player position
    if (*newSquare == 4) {
        *newSquare = 6; // Player on goal
    } else {
        *newSquare = 2; // Player in empty space
    }

    if (p->prevSquareValue == 4) {
        *currentSquare = 4; // Restore goal
    } else {
        *currentSquare = 0; // Restore empty space
    }

    p->x = newPlayerX;
    p->y = newPlayerY;
    p->prevSquareValue = *newSquare;
    NUM_STEPS++;
}

/* =======Main Game Loop ========== */
int main() {
    int numCols = MAP_COLS;
    int numRows = MAP_ROWS;
    int *map = malloc((MAP_ROWS * MAP_COLS) * sizeof(int));
    Player *p = malloc(sizeof(Player));
    p->prevSquareValue = 0;

    int numberOfStars = 0;
    read_in_maps(p, map, &numberOfStars);

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    cbreak();

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    start_color();
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);

    title_bar = newwin(WIN_TITLE_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y, WIN_OFFSET_X);
    main_win = newwin(WIN_MAIN_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y + WIN_TITLE_HEIGHT, WIN_OFFSET_X);
    score_win = newwin(WIN_SCORE_HEIGHT, WIN_WIDTH, WIN_OFFSET_Y + WIN_TITLE_HEIGHT + WIN_MAIN_HEIGHT, WIN_OFFSET_X);

    wborder(title_bar, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(main_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(score_win, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(title_bar, 1, (WIN_WIDTH - 16) / 2, "STAR PUSHER GAME");
    refresh();
    wrefresh(title_bar);
    wrefresh(main_win);
    wrefresh(score_win);

    int keepLooping = 1;
    while (keepLooping) {
        drawScreen(numRows, numCols, map);
        mvwprintw(score_win, 1, 1, "Steps: %d", NUM_STEPS);
        wrefresh(score_win);

        int input = getch();
        int delta = -1;
        switch (input) {
            case KEY_UP:
                delta = 0;
                break;
            case KEY_RIGHT:
                delta = 1;
                break;
            case KEY_DOWN:
                delta = 2;
                break;
            case KEY_LEFT:
                delta = 3;
                break;
            case 'q':
            case 'Q':
            case 10:
                keepLooping = 0;
                break;
        }

        if (validMove(delta, p, map)) {
            movePlayer(delta, p, map);
        }

        if (numStarsSolved == numberOfStars) {
            mvwprintw(score_win, 2, (WIN_WIDTH - 10) / 2, "YOU WIN !!");
            wrefresh(score_win);
            drawScreen(numRows, numCols, map);
            sleep(2);
            keepLooping = 0;
        }
    }

    endwin();
    return 0;
}
