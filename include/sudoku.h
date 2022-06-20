//
//  sudoku.h
//  Sudoku Solver
//
//  Created by Hung on 6/15/22.
//

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>

extern int UNSOLVED;
extern int SIZE_ROWS;
extern int SIZE_COLUMNS;

typedef struct Sudoku {
  struct Square*** squares;
  struct Box** boxes;
} Sudoku;

typedef struct Box {
  struct Square** squares;
  int numbers;
  int possible[9];
  int solvable;
  struct Box* next;
} Box;

typedef struct Square {
  int number;
  int possible[9];
  int solvable;
  Box* box;
  int row;
  int column;
} Square;

int** createPuzzle();
void printPuzzle(Square***);
Sudoku* setUpPuzzle(int**);
Sudoku* createSudoku(Square***, Box**);
int updateSudoku(Square*** , int , int);
int checkPuzzle(Square***, Box**);
Box** createBoxes();
int updateBoxes(Square***, int, int);
int boxSingles(Square***, Box**);
int checkRows(Square***, Box**);

#endif
