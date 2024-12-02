
# Sokoban Game - CS330 Assignment 7

This repository contains the solution for **CS330 - Computer Organization & Assembly Language**, Assignment 7. The goal of the assignment was to implement a working **Sokoban** game using the C programming language, with the integration of `ncurses` for a text-based user interface. 

## Overview

Sokoban is a classic puzzle game where the player needs to push all the stars onto goal squares in a maze-like environment. The player can only push stars (not pull), and must avoid walls and other obstacles while solving the puzzle.

### **Objective**
- **Push all the stars onto the goal squares**.
- **Move the player** using the four cardinal directions (up, down, left, right).
- The player can **only push stars**, not pull them.
- The game checks for invalid moves, such as:
  - Moving into walls.
  - Pushing stars into other stars or walls.
  
## Features

- **Interactive Game Loop**: Players can move in all four directions (up, down, left, right) using the arrow keys.
- **Player and Star Movement**: Players can move the player character and push stars onto goal squares.
- **Map Representation**: The game uses a 2D map represented by a 1D array with pointer arithmetic.
- **Game Victory**: The game tracks the number of stars correctly placed on goal squares and declares a win when all stars are correctly positioned.

## Files in this Project

- `cs330_sokoban_game.c`: The main C source code for the Sokoban game.
- `Makefile`: A file to compile and run the game.
- `libsok_helper_vulcan.a`: A static library for game helper functions.
- `sok_header.h`: Header file containing function declarations and constants.
- `maps.txt`: The configuration file for the game maps.
- `sokoban_submission.zip`: A zip file containing the project files for submission.

## How to Run the Game

### Requirements:
- A Linux-based environment or a terminal with **ncurses** support.
- **GCC** (GNU Compiler Collection) installed for compiling the C program.
- The project is designed to run in **GitHub Codespaces** or a **Linux** environment like **Vulcan**.

### Steps to Run:

1. **Clone the Repository** (if applicable):
   
2. **Compile the Code**:
   In the terminal, run the `make` command to compile the game:
   ```bash
   make
   ```

3. **Run the Game**:
   After compiling, run the game:
   ```bash
   make run
   ```

4. **Control the Game**:
   Use the arrow keys to move the player. Push all the stars onto the goal squares to win.

5. **Quit the Game**:
   Press `Q` or `q` to quit the game.

## Game Controls
- **Arrow Keys**: Move the player up, down, left, or right.
- **Q/q**: Quit the game.

## Notes

- The game is designed to handle simple Sokoban mechanics like pushing stars onto goal squares, but it also ensures that players cannot push stars into walls or other stars.
- The `validMove()` function checks the legality of player moves based on the current map and direction of movement.
- The `movePlayer()` function executes the move if it's valid, updating both the player’s position and the star's position.

## Contributions

This project was developed as part of the CS330 course assignment, and no external contributions were made.

