#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}


/* add your functions here */

//q1

bool is_complete(char mines[][BD_SIZE], char revealed[][BD_SIZE])
{
  for(int i = 0; i < BD_SIZE; i++)
    {
      for(int j = 0; j < BD_SIZE; j++)
        {
          if((mines[i][j] == '.' && revealed[i][j] != '?') ||
             (mines[i][j] == '*' && revealed[i][j] == '*')
             )
            continue;
          else return false;
        }
    }

  return true;
}

//q2
int count_mines(const char* position, char mines[][BD_SIZE])
{
  // get the pos
  const int ROW = position[0] - 'A';
  const int COL = position[1] - '1';

  int count = 0;
  
  for(int i = -1; i <= 1; i++)
    {
      for(int j = -1; j <= 1; j++)
        {
          if(ROW+i<0 || ROW+i>BD_SIZE-1 || COL+j<0 || COL+j >BD_SIZE-1) continue; // out of the baord

          if(mines[ROW+i][COL+j] == '*') count++;
        }
    }

  return count;
}


//q3
MoveResult make_move(const char* position, char mines[][BD_SIZE], char revealed[][BD_SIZE])
{
  // get the pos first
  const int ROW = position[0] - 'A';
  const int COL = position[1] - '0' - 1;

  if(ROW<0 || ROW>BD_SIZE|| COL<0 || COL>BD_SIZE) return INVALID_MOVE;

  if(revealed[ROW][COL] != '?') return REDUNDANT_MOVE;
  
  if(position[2] =='\0') // the third char
    { 
      if(mines[ROW][COL] == '*') return BLOWN_UP; // boom!
    }
  else if(position[2] == '*')
    {
      revealed[ROW][COL] = '*'; // flag the square
      
      if(is_complete(mines,revealed)) // check end game?
        return SOLVED_BOARD;

      return VALID_MOVE;
    }
  else return INVALID_MOVE;

  
  int count = count_mines(position,mines);
  if(count > 0) // get the adjacent count
    {
      revealed[ROW][COL] = static_cast<char>(count+'0');
      //display_board(revealed);
      return VALID_MOVE;
    }
  else // uncoverd the adjacent ones
    {
      // the square is blank now
      revealed[ROW][COL] = ' ';
      
      char pos[3];
      for(int i = -1; i <= 1; i ++)
        {
          for(int j = -1; j <= 1; j++)
            {
              if(ROW+i < 0 || ROW+i > 8 || COL+j < 0 || COL+j > 8) continue; // out of the baor
              if(i == 0 && j == 0) continue;

              // recursively uncover!
              pos[0] = static_cast<char>(ROW+i+'A'); pos[1] = static_cast<char>(COL+j+'1');
              pos[2] = '\0';
              make_move(pos,mines,revealed);
            }
        }

      return VALID_MOVE;
    }
}


//q4
bool find_safe_move(char revealed[][BD_SIZE], char* move)
{
  for(int i = 0; i < BD_SIZE; i++)
    {
      for(int j = 0; j < BD_SIZE; j++)
        {
          if(!isdigit(revealed[i][j])) continue; // skip non digit

          // centre is x, y covered adjacent square, z flagged adjacent square
          if(helper(i,j,revealed,move)) return true;
        }
    }

  return false; // no safe move
}

bool helper(int a, int b, char revealed[][BD_SIZE], char* move)
{
  const int NUM = revealed[a][b] - '0';

  int covered = 0, flagged = 0; int coord[8][2];
  for(int i = a-1; i <= a+1; i++)
    {
      for(int j = b-1; j <= b+1; j++)
        {
          if(i<0 || i>BD_SIZE-1 || j<0 || j>BD_SIZE-1) continue;
          
          if(i == a && j == b) continue;

          if(revealed[i][j] == '?')
            {
              coord[covered][0] = i, coord[covered][1] = j;
              covered++;
            }
          else if(revealed[i][j] == '*') flagged++;
        }
    }

  int r = coord[covered-1][0], c = coord[covered-1][1];
  char pos[4];
  if(flagged == NUM && covered > 0) // to be walked on
    {
      //revealed[r][c] = ' ';
      pos[0] = static_cast<char>(r+'A');
      pos[1] = static_cast<char>(c+'1');
      pos[2] = '\0';
      strcpy(move, pos);
      return true;
    }
  else if(flagged + covered == NUM && covered > 0) // to be falgged
    {
      //revealed[r][c] = '*';
      pos[0] = static_cast<char>(r+'A');
      pos[1] = static_cast<char>(c+'1');
      pos[2] = '*';
      pos[3] = '\0';
      strcpy(move, pos);
      return true;
    }
  else
    return false;
}
