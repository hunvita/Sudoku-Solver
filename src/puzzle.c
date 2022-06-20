//
//  sudoku.c
//  Sudoku Solver
//
//  Created by Hung on 6/15/22.
//

#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

int checkRows(Square*** sudoku, Box** boxes) {
  int sum[9];
  int place[9];
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      place[j] = 0;
      sum[j] = 0;
    }
    for (int j = 0; j < 9; j++) {
      if (sudoku[i][j] -> number != 0) {
        continue;
      }
      for (int k = 0; k < 9; k++) {
        if (sudoku[i][j] -> possible[k] == 0) {
          sum[k]++;
          place[k] = j;
        }
        
      }
    }
    for (int k = 0; k < 9; k++) {
      if (sum[k] == 1) {
        sudoku[i][place[k]] -> number = k + 1;
        sudoku[i][place[k]] -> solvable = 0;
        UNSOLVED--;
        updateSudoku(sudoku, i, place[k]);
        updateBoxes(sudoku, i, place[k]);
        return 1;
      }
    }
  }
  return 0;
}

int boxSingles(Square*** sudoku, Box** boxes) {
  int count;
  int temp;
  int x;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      count = 0;
      for (x = 0; x < 9; x++) {
        if (boxes[i] -> squares[x] -> number != 0) {
          continue;
        }
        if (boxes[i] -> squares[x] -> possible[j] == 0) {
          count++;
          temp = x;
        }
        if (count == 2) {
          break;
        }
      }
      if (count == 1) {
        boxes[i] -> squares[temp] -> number = j + 1;
        UNSOLVED--;
        boxes[i] -> squares[temp] -> solvable = 0;
        
        updateSudoku(sudoku, boxes[i] -> squares[temp] -> row, boxes[i] -> squares[temp] -> column);
        
        return 1;
      }
    }
  }
  return 0;
}

Sudoku* createSudoku(Square*** squares, Box** boxes) {
  Sudoku* sudoku;
  sudoku = malloc(sizeof(Sudoku));
  sudoku -> squares = squares;
  sudoku -> boxes = boxes;
  
  return sudoku;
}

int** createPuzzle() {

  int ** puzzle;
  int array[9][9] = {0, 1, 9,    0, 0, 2,    0, 0, 0,
                     4, 7, 0,    6, 9, 0,    0, 0, 1,
                     0, 0, 0,    4, 0, 0,    0, 9, 0,
                    
                     8, 9, 4,    5, 0, 7,    0, 0, 0,
                     0, 0, 0,    0, 0, 0,    0, 0, 0,
                     0, 0, 0,    2, 0, 1,    9, 5, 8,
    
                     0, 5, 0,    0, 0, 6,    0, 0, 0,
                     6, 0, 0,    0, 2, 8,    0, 7, 9,
                     0, 0, 0,    1, 0, 0,    8, 6, 0};
  
  puzzle = (int**)malloc(sizeof(int*)*9);
  
  for (int i = 0; i < 9; i++) {
    puzzle[i] = (int*)malloc(sizeof(int)*9);
    for (int j = 0; j < 9; j++) {
      puzzle[i][j] = array[i][j];
    }
  }
  return puzzle;
}

void printPuzzle(Square*** puzzle) {
  printf("-------------------------------\n");
  for (int i = 0; i < 9; i++) {
    printf("|");
    for (int j = 0; j < 9; j++) {
      printf(" %d ", puzzle[i][j] -> number);
      if ((j + 1) % 3 == 0)
        printf("|");
    }
    printf("\n");
    if ((i + 1) % 3 == 0)
      printf("-------------------------------\n");
  }
}

Sudoku* setUpPuzzle(int** puzzle) {
  Square*** sudoku;
  Box** boxes;
  sudoku = (Square***)malloc(sizeof(Square**)*9);
  boxes = createBoxes();
  int currentBox = 0;
  
  for (int i = 0; i < SIZE_ROWS; i++) {
    sudoku[i] = (Square**)malloc(sizeof(Square*)*9);
    for (int j = 0; j < SIZE_COLUMNS; j++) {
      sudoku[i][j] = (Square*)malloc(sizeof(Square*)*9);
      sudoku[i][j] -> number = puzzle[i][j];
      sudoku[i][j] -> row = i;
      sudoku[i][j] -> column = j;
      sudoku[i][j] -> solvable = 9;
      
      boxes[currentBox] -> squares[boxes[currentBox] -> numbers] = sudoku[i][j];
      sudoku[i][j] -> box = boxes[currentBox];
      boxes[currentBox] -> numbers++;
      
      for (int x = 0; x < SIZE_ROWS; x++) {
        sudoku[i][j] -> possible[x] = 0;
      }
      // here
      if (j == 2)
        currentBox++;
      if (j == 5)
        currentBox++;
    }
    currentBox -= 2;
    if (i == 2)
      currentBox = 3;
    if (i == 5)
      currentBox = 6;
  }
  for (int i = 0; i < SIZE_ROWS; i++) {
    for (int j = 0; j < SIZE_COLUMNS; j++) {
      if (sudoku[i][j] -> number != 0) {
        sudoku[i][j] -> solvable = 0;
        updateSudoku(sudoku, i, j);
        updateBoxes(sudoku, i, j);
        UNSOLVED--;
      }
    }
  }
  return createSudoku(sudoku, boxes);
}

int updateSudoku(Square*** sudoku, int row, int column) {
  int number = sudoku[row][column] -> number;
  
  for (int x = 0; x < SIZE_ROWS; x++) {
    if (sudoku[x][column] -> possible[number - 1] == 0) {
      sudoku[x][column] -> solvable--;
    }
    sudoku[x][column] -> possible[number - 1] = 1;
  }
  
  for (int x = 0; x < SIZE_COLUMNS; x++) {
    if (sudoku[row][x] -> possible[number - 1] == 0) {
      sudoku[row][x] -> solvable--;
    }
    sudoku[row][x] -> possible[number - 1] = 1;
  }
  
  return 1;
}

int solveSquare(Square* square) {
  for (int x = 0; x < SIZE_ROWS; x++) {
    if (square -> possible[x] == 0) {
      square -> number = x + 1;
      square -> solvable = 0;
      UNSOLVED--;
    }
  }
  return 1;
}


int checkPuzzle(Square*** sudoku, Box** boxes) {
  for (int i = 0; i < SIZE_ROWS; i++) {
    for (int j = 0; j < SIZE_COLUMNS; j++) {
      if (sudoku[i][j] -> solvable == 1) {
        solveSquare(sudoku[i][j]);
        updateSudoku(sudoku, i, j);
        updateBoxes(sudoku, i, j);
        return 1;
      }
    }
  }
  
  if(boxSingles(sudoku, boxes))
    return 1;
  return checkRows(sudoku, boxes);
}

Box** createBoxes() {
  Box** boxes;
  boxes = malloc(sizeof(Box*)*9);
  
  for (int x = 0; x < 9; x++) {
    boxes[x] = malloc(sizeof(Box));
    boxes[x] -> squares = malloc(sizeof(Square*)*9);
    boxes[x] -> numbers = 0;
    boxes[x] -> solvable = 9;
    
    for (int y = 0; y < 9; y++) {
      boxes[x] -> possible[y] = 0;
    }
  }
  return boxes;
}

int updateBoxes(Square*** sudoku, int row, int column) {
  int x;
  int number = sudoku[row][column] -> number;
  Box* box;
  box = sudoku[row][column] -> box;
  
  for (int x = 0; x < 9; x++) {
    if (box -> squares[x] -> possible[number - 1] == 0) {
      box -> squares[x] -> solvable--;
      box -> squares[x] -> possible[number - 1] = 1;
    }
  }
  return 1;
}
