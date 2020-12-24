#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

// Q1

#define TOTAL_M 3
#define TOTAL_C 3

char** make_river_scene(const char* left, const char* boat)
{
  // get num of M and C at the left bank
  int num_M_left = 0, num_C_left = 0;
  bool boat_left = false;
  for(int i = 0; left[i]!='\0'; i++)
    {
      if(left[i] == 'M') num_M_left++;
      else if(left[i] == 'C') num_C_left++;
      else if(left[i] == 'B') boat_left = true;
    }

  // get num of M and C on the boat
  int num_M_boat = 0, num_C_boat = 0;
  for(int i = 0; boat[i]!='\0'; i++)
    {
      if(boat[i] == 'M') num_M_boat++;
      else if(boat[i] == 'C') num_C_boat++;
    }

  // get the num of M and C at he right bank
  int num_M_right = TOTAL_M - num_M_left - num_M_boat;
  int num_C_right = TOTAL_C - num_C_left - num_C_boat;

  
  // draw!
  char** scene = create_scene();
  // left bank
  add_to_scene(scene,0,0,"bank.txt");
  
  // right bank
  add_to_scene(scene,0,53,"bank.txt");
  
  // add sun
  add_to_scene(scene,3,30,"sun.txt");

  // add river
  add_to_scene(scene,19,19,"river.txt");

  // add boat
  if(boat_left)
    add_to_scene(scene,17,19,"boat.txt");
  else
    add_to_scene(scene,17,36,"boat.txt");
  
  // add left M
  for(int i = 0; i < num_M_left; i++)
    add_to_scene(scene,2,1+6*i,"missionary.txt");

  // add left C
  for(int i = 0; i < num_C_left; i++)
    add_to_scene(scene,11,1+6*i,"cannibal.txt");

  // add right M
  for(int i = 0; i < num_M_right; i++)
    add_to_scene(scene,2,SCENE_WIDTH-18+6*i,"missionary.txt");

  // add right C
  for(int i = 0; i < num_C_right; i++)
    add_to_scene(scene,11,SCENE_WIDTH-18+6*i,"cannibal.txt");

  // add M on boat
  if(boat_left)
    {
      for(int i = 0; i < num_M_boat; i++)
        add_to_scene(scene,11,22+i*6,"missionary.txt");
    }
  else
    {
      for(int i = 0; i < num_M_boat; i++)
        add_to_scene(scene,11,39+i*6,"missionary.txt");
    }

  // add C on boat
  if(num_C_boat == 2)
    {
      if(boat_left)
        {
          for(int j = 0; j < num_C_boat; j++)
            add_to_scene(scene,11,22+j*6,"cannibal.txt");
        }
      else
        {
          for(int j = 0; j < num_C_boat; j++)
            add_to_scene(scene,11,39+j*6,"cannibal.txt");
        }
    }
  else if(num_C_boat == 1)
    {
      if(boat_left)
        add_to_scene(scene,11,28,"cannibal.txt");
      else
        add_to_scene(scene,11,45,"cannibal.txt");
    }

  return scene;  
}



//Q2

#define BOAT_CAP 2

int perform_crossing(char* left, const char* target)
{  
  // get target entity on the boat
  int num_C_boat = 0, num_M_boat = 0;
  for(int i = 0; target[i]!='\0'; i++)
    {
      if(target[i]=='C') num_C_boat++;
      else if(target[i]=='M') num_M_boat++;
    }

  // check the boat capacity
  if(num_C_boat + num_M_boat < 1 || num_C_boat + num_M_boat > BOAT_CAP)
    return ERROR_INVALID_MOVE;

  // get num of M and C at the left bank (before crossing)
  int num_M_left = 0, num_C_left = 0;
  bool boat_left = false; // boat's position
  for(int i = 0; left[i]!='\0'; i++)
    {
      if(left[i] == 'M') num_M_left++;
      else if(left[i] == 'C') num_C_left++;
      else if(left[i] == 'B') boat_left = true;
    }

  // get the num of M and C at the right (before crissing)
  int num_M_right = TOTAL_M - num_M_left, num_C_right = TOTAL_C - num_C_left;
  
  // check the M/C num
  if(boat_left)
    {
      if(num_M_left < num_M_boat)
        return ERROR_INVALID_MISSIONARY_COUNT;
      
      if(num_C_left < num_C_boat) return ERROR_INVALID_CANNIBAL_COUNT;
    }
  else
    {
      if(num_M_right < num_M_boat) return ERROR_INVALID_MISSIONARY_COUNT;

      if(num_C_right < num_C_boat) return ERROR_INVALID_CANNIBAL_COUNT;
    }


  // transfering from L to R
  if(boat_left)
    { 
      // 0. update left bank
      char cur_M_left[7]; char cur_C_left[7];
      for(int i = 0; i <= num_M_left - num_M_boat; i++)
        {
          if(i < num_M_left - num_M_boat)
            cur_M_left[i] = 'M';
          else
            cur_M_left[i] = '\0';
        }
      
      for(int i = 0; i <= num_C_left - num_C_boat; i++)
        {
          if(i < num_C_left - num_C_boat)
            cur_C_left[i] = 'C';
          else
            cur_C_left[i] = '\0';
        }
      strcpy(left,cur_M_left); int len = strlen(left);
      strcpy(left+len,cur_C_left); len = strlen(left);
      strcpy(left+len,"B");

      // 1. load the boat
      char** scene = make_river_scene(left,target);
      cout << "Loading the boat..." << endl;
      print_scene(scene);
      destroy_scene(scene);
      
      // 2. move the boat
      remove_B(left);
      scene = make_river_scene(left,target);
      cout << endl << "Crossing the river..." << endl;
      print_scene(scene);
      destroy_scene(scene);

      // 3. unload the boat
      scene = make_river_scene(left,"");
      cout << endl << "Unloading the boat..." << endl;
      print_scene(scene);
      destroy_scene(scene);
    }
  else // transfering from R to L
    {
      // 1. load the boat
      char** scene = make_river_scene(left,target);
      cout << "Loading the boat..." << endl;
      print_scene(scene);
      destroy_scene(scene);

      // 2. move the boat
      add_B(left);
      scene = make_river_scene(left,target);
      cout << endl << "Crossing the river..." << endl;
      print_scene(scene);
      destroy_scene(scene);
      
      // 3. update the left bank
      int len = strlen(left);
      strcpy(left+len,target);

      // 4. unload the boat
      scene = make_river_scene(left,"");
      cout << endl << "Unloading the boat..." << endl;
      print_scene(scene);
      destroy_scene(scene);
    }


  // check if M will be eaten
  if(TOTAL_M - count_char(left,'M') > 0) // right bank
    {
      if(TOTAL_M - count_char(left,'M') <
         TOTAL_C - count_char(left,'C'))
        return ERROR_MISSIONARIES_EATEN;
    }

  if(count_char(left,'M') > 0) // left bank
    {
      if(count_char(left,'M') < count_char(left,'C'))
        return ERROR_MISSIONARIES_EATEN;
    }

  // check for endgame condition
  if(strcmp(left,"") == 0)
    return VALID_GOAL_STATE;
  else
    return VALID_NONGOAL_STATE;
}


void remove_B(char* str)
{
  // locate the pos of B
  int pos = -1;
  for(int i = 0; str[i]!='\0'; i++)
    {
      if(str[i] == 'B') pos = i;
    }

  // remove B
  if(pos != -1)
    {
      for(int i = pos; str[i]!='\0'; i++)
        str[i] = str[i+1];
    }
}

void add_B(char* str)
{
  int len = strlen(str);
  str[len++] = 'B'; str[len] = '\0';
}


int count_char(const char* str, char ch)
{
  int count = 0;
  for(int i = 0; str[i]!='\0'; i++)
    {
      if(str[i] == ch) count++;
    }
  return count;
}



// Q3
#define MAX_STR_LEN 100
int play_game()
{  
  char left[10]; strcpy(left,"MMMCCCB");
  char right[10]; strcpy(right,"");
  
  // start a game
  cout << "New game..." << endl;
  char** scene = make_river_scene(left,"");
  print_scene(scene);
  destroy_scene(scene);
  
  char target[MAX_STR_LEN];
  int state_code;
  
  while(strcmp(left,"")!=0)
    {
      cout << "Please type in the target(s) to be tansferred" << endl;
      cin >> target;

      // perform crossing
      state_code = perform_crossing(left,target);
      cout << "After crossing left = " << left << endl;

      if(state_code == VALID_NONGOAL_STATE)
        continue;
      else
        return state_code;
    }
  
  return VALID_GOAL_STATE;
}


// Bonus challenge
int find_solution(const char* left, char* answer)
{
  // copy left
  char left_copy[10]; strcpy(left_copy,left);
  strcpy(answer,"");
  
  // find solution
  if(solver(left_copy,answer,""))
    return VALID_GOAL_STATE;
  else
    return ERROR_BONUS_NO_SOLUTION;
}

bool solver(char* left, char* answer, const char* last_target)
{
  // base case 1: solution already found
  if(!strcmp(left,"")) return true;

  /* Try diiffrent target */
  char target[BOAT_CAP+1]; char left_copy[10];
  int status = 0; string ans;
  for(int i = 0 ; i < 5; i++)
    {
      switch(i)
        {
        case 0:
          strcpy(target,"M"); break;
        case 1:
          strcpy(target,"C"); break;
        case 2:
          strcpy(target,"MM"); break;
        case 3:
          strcpy(target,"CC"); break;
        case 4:
          strcpy(target,"MC"); break;
        }
      
      if(!strcmp(last_target,target)) continue;
      
      strcpy(left_copy,left);
      status = perform_crossing2(left_copy,target); // perform crossing
      
      ans = string(answer); // a temporary "answer"
      if(strcmp(last_target,"")) ans.push_back(',');
      ans.append(target);

      if(!strcmp(left_copy,"")) // final solution
        {
          strcpy(answer,ans.c_str());
          return true;
        }
      
      if(status == VALID_NONGOAL_STATE) // valid but hasnt solved
        {
          char answer_temp[MAX_STR_LEN];
          strcpy(answer_temp,ans.c_str());
          if(solver(left_copy,answer_temp,target)) // has further valid step?
            {
              strcpy(answer,ans.c_str());
              return true;
            }
        }
    }
  
  return false; // no solution: go back
}





// Crossing without printing
int perform_crossing2(char* left, const char* target)
{  
  // get target entity on the boat
  int num_C_boat = 0, num_M_boat = 0;
  for(int i = 0; target[i]!='\0'; i++)
    {
      if(target[i]=='C') num_C_boat++;
      else if(target[i]=='M') num_M_boat++;
    }

  // check the boat capacity
  if(num_C_boat + num_M_boat < 1 || num_C_boat + num_M_boat > BOAT_CAP)
    return ERROR_INVALID_MOVE;

  // get num of M and C at the left bank (before crossing)
  int num_M_left = 0, num_C_left = 0;
  bool boat_left = false; // boat's position
  for(int i = 0; left[i]!='\0'; i++)
    {
      if(left[i] == 'M') num_M_left++;
      else if(left[i] == 'C') num_C_left++;
      else if(left[i] == 'B') boat_left = true;
    }

  // get the num of M and C at the right (before crissing)
  int num_M_right = TOTAL_M - num_M_left, num_C_right = TOTAL_C - num_C_left;
  
  // check the M/C num
  if(boat_left)
    {
      if(num_M_left < num_M_boat)
        return ERROR_INVALID_MISSIONARY_COUNT;
      
      if(num_C_left < num_C_boat) return ERROR_INVALID_CANNIBAL_COUNT;
    }
  else
    {
      if(num_M_right < num_M_boat) return ERROR_INVALID_MISSIONARY_COUNT;

      if(num_C_right < num_C_boat) return ERROR_INVALID_CANNIBAL_COUNT;
    }


  // transfering from L to R
  if(boat_left)
    { 
      // 0. update left bank
      char cur_M_left[7]; char cur_C_left[7];
      for(int i = 0; i <= num_M_left - num_M_boat; i++)
        {
          if(i < num_M_left - num_M_boat)
            cur_M_left[i] = 'M';
          else
            cur_M_left[i] = '\0';
        }
      
      for(int i = 0; i <= num_C_left - num_C_boat; i++)
        {
          if(i < num_C_left - num_C_boat)
            cur_C_left[i] = 'C';
          else
            cur_C_left[i] = '\0';
        }
      strcpy(left,cur_M_left); int len = strlen(left);
      strcpy(left+len,cur_C_left);
    }
  else // transfering from R to L
    {
      // 3. update the left bank
      int len = strlen(left);
      strcpy(left+len,target);
      add_B(left);
    }


  // check if M will be eaten
  if(TOTAL_M - count_char(left,'M') > 0) // right bank
    {
      if(TOTAL_M - count_char(left,'M') <
         TOTAL_C - count_char(left,'C'))
        return ERROR_MISSIONARIES_EATEN;
    }

  if(count_char(left,'M') > 0) // left bank
    {
      if(count_char(left,'M') < count_char(left,'C'))
        return ERROR_MISSIONARIES_EATEN;
    }

  // check for endgame condition
  if(strcmp(left,"") == 0)
    return VALID_GOAL_STATE;
  else
    return VALID_NONGOAL_STATE;
}
