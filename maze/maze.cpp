#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <string>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}




bool find_marker(char ch, char** maze, int height, int width,
                 int& row, int& col)
{
  for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
        {
          if(maze[i][j] == ch){
            row = i; col = j;
            return true;
          }
        }
    }

  row = -1; col = -1;
  return false;
}


bool valid_solution(const char* path, char** maze, int height, int width)
{

  // locate the entry
  int entry_r, entry_c;
  find_marker('>',maze,height,width,entry_r,entry_c);

  const int p_len = strlen(path); // path length
  
  int cur_r = entry_r, cur_c = entry_c; // current pos
  
  for(int k = 0; k < p_len; k++)
    {
      switch(path[k])
        {
        case 'N':
          cur_r--; break;

        case 'S':
          cur_r++; break;

        case 'E':
          cur_c++; break;
          
        case 'W':
          cur_c--; break;
        }
      if(k != p_len-1) // non-last-step
        {
          if(maze[cur_r][cur_c] != ' ')
            return false;
        }
      else if(maze[cur_r][cur_c] == 'X') // last step at exit
        {
          return true;
        }
      else
        return false;
    }
  
  return false;
}

#define DIR_NUM 4 // max number of possible directions

bool path_solver(char** maze, const int row, const int col, const int start_row,
                 const int start_col, char start, char end, string& sol)
{
  // at the starting position
  if(row == start_row && col == start_col)
  {
    maze[row][col] = '#';
  }
      
  // base case: solution already found
  if(maze[row][col] == end) return true;

  char dir = ' ';
  int cur_row = row, cur_col = col;

  // look around at [row][col] to see if there is any valid step
  for(int i = 0; i < DIR_NUM; i++)
    {      
      switch(i)
        {
        case 0:
          cur_row = row - 1; cur_col = col;
          dir = 'N'; break;

        case 1:
          cur_row = row + 1; cur_col = col;
          dir = 'S'; break;

        case 2:
          cur_col = col + 1; cur_row = row;
          dir = 'E'; break;
          
        case 3:
          cur_col = col - 1; cur_row = row;
          dir = 'W'; break;
        }
      
      // final solution found!
      if(maze[cur_row][cur_col] == end)
          {
            sol.push_back(dir); // update the solution
            maze[cur_row][cur_col] = '#';
            return true;
          }
      
      // final solution hasn't been found but this is a legal step
      if(maze[cur_row][cur_col] == ' ')
        {
          sol.push_back(dir); // update the solution
          maze[cur_row][cur_col] = '#';
            
          // final solution hasn't been found yet, go forwards success: this step is valid
          if(path_solver(maze, cur_row, cur_col, start_row, start_col, start, end, sol))
            return true;
          else // going forward fails: this step invalid: look for another direction
          {
            if(!sol.empty())
              {
                sol.pop_back();
                maze[cur_row][cur_col] = ' ';
                //return false;
              }
          }
        }
      // else: this is not a legal step: try another direction
    }
  
  // no futher legal steps standing at [row][col]: the last step is wrong
  if(!sol.empty())
  {
    sol.pop_back();
    maze[row][col] = ' ';
  }
  return false;
}



string find_path(char** maze, int height, int width, char start, char end)
{
  // locate the entry
  int entry_r, entry_c;
  find_marker(start,maze,height,width,entry_r,entry_c);

  string sol = "";
  if(!path_solver(maze, entry_r, entry_c, entry_r, entry_c, start, end, sol))
    {
      //maze[entry_r][entry_c] = start;
      sol = string("no solution");
    }
  
  return sol;
}
