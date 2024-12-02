#ifndef __sok_header__
#define __sok_header__

/* define a struct for player info, called 'p' */
typedef struct {            // Note: doesn't compile if Player on this line
    int x;
    int y;
    int prevSquareValue;
} Player;

/* some variables we'll use in the game */
// these need to match the map
int extern MAP_COLS;
int extern MAP_ROWS;

// for movement: up, right, down, left
int extern dX[4];
int extern dY[4];

int extern numStarsSolved;  // to represent the number of stars in the correct spot
int extern NUM_STEPS;      // to keep score



// functions in the helper library
int read_in_maps(Player *p, int *map, int *totalNumStars);
int TA_validMove(int direction, Player *p, int *map);
void TA_movePlayer(int direction, Player *p, int *map);

#endif