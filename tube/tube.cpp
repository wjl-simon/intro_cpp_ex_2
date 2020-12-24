#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
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

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}


//q1
bool get_symbol_position(char const *const * map, int height, int width, char target, int& r, int& c)
{
  for(int i = 0; i < height; i++)
    {
      for(int j = 0; j < width; j++)
        {
          if(map[i][j] == target)
            {
              r = i, c = j;
              return true;
            }
        }
    }

  r = -1, c = -1;
  return false;
}


// q2
#define MAX_STR_LEN 512
char get_symbol_for_station_or_line(const char* name)
{
  ifstream in; in.open("./stations.txt");

  // search for stations first
  char sym; char n[MAX_STR_LEN]; // symbol and name being read
  in >> sym;
  while(!in.eof())
    {
      in.getline(n,MAX_STR_LEN); // get the name
      
      if(strcmp(name,n+1) != 0) // name has a leading whitspace
        in >> sym;
      else
        {
          in.close();
          return sym;
        }
    }

  // search for the lines then
  in.close(); in.open("./lines.txt");
  
  in >> sym;
  while(!in.eof())
    {
      in.getline(n,MAX_STR_LEN); // get the name

      if(strcmp(name,n+1) != 0)
        in >> sym;
      else
        {
          in.close();
          return sym;
        }
    }
  
  // can't find
  in.close();
  return ' ';
}

//q3
int validate_route(char** map, int height, int width,
                   const char* start_station, const char* route, char* destination)
{
  char rou[MAX_STR_LEN]; strcpy(rou,route);
  
  // test for a valid name
  if(!is_station(start_station)) return ERROR_START_STATION_INVALID;

  // get pos of the start
  char sym; int r_start, c_start;
  sym = get_symbol_for_station_or_line(start_station);
  get_symbol_position(map,height,width,sym,r_start,c_start);

  // move
  Direction dir;
  int r_pre = r_start, c_pre = c_start;
  int r_cur = r_start, c_cur = c_start;
  int r_next, c_next;

  char ch_cur = map[r_cur][c_cur]; // current/next/previous symbo
  char ch_next, ch_pre = ch_cur;
  
  int change = 0; // number of times of changing lines
  while(*rou != '\0')
    {     
      dir = translate_dir(rou);
      switch(dir)
        {
        case N:
          r_next = r_cur-1; c_next = c_cur; break;
        case S:
          r_next = r_cur+1; c_next = c_cur; break;
        case W:
          r_next = r_cur; c_next = c_cur-1; break;
        case E:
          r_next = r_cur; c_next = c_cur+1; break;
        case NE:
          r_next = r_cur-1; c_next = c_cur+1; break;
        case NW:
          r_next = r_cur-1; c_next = c_cur-1; break;
        case SE:
          r_next = r_cur+1; c_next = c_cur+1; break;
        case SW:
          r_next = r_cur+1; c_next = c_cur-1; break;
        default:
          return ERROR_INVALID_DIRECTION;
        }

      // ouside?
      if(r_next < 0 || r_next > height || c_next < 0 || c_next > width)
        return ERROR_OUT_OF_BOUNDS;

      // off track?
      ch_next = map[r_next][c_next];
      if(ch_next == ' ') return ERROR_OFF_TRACK;

      // line hopping?
      if(isalnum(ch_cur)) // currently at station
        {
          if(isalnum(ch_next)) // next is station
            return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
        }
      else if(!isalnum(ch_next) && ch_next!= ch_cur) // line hopping without a station
        return ERROR_LINE_HOPPING_BETWEEN_STATIONS;

      // test for retracing
      if(!isalnum(ch_pre) && !isalnum(ch_cur) && !isalnum(ch_next))
        {
          if(ch_pre==ch_next && r_pre==r_next && c_pre==c_next)
            return ERROR_BACKTRACKING_BETWEEN_STATIONS;
        }

      // num of changing lines
      if(!isalnum(ch_pre) && isalnum(ch_cur) && !isalnum(ch_next))
        {
          if(ch_pre != ch_next) change++;
        }

      // update
      r_pre = r_cur; c_pre = c_cur;
      r_cur = r_next; c_cur = c_next;
      ch_pre = ch_cur; ch_cur = ch_next;
    }

  // check the endpoint destination
  if(!isalnum(ch_cur)) return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  else // get the destin name
    {
      ifstream in; in.open("./stations.txt");

      char sym; char name[MAX_STR_LEN]; // symbol and station name being read
      in >> sym;
      while(!in.eof())
        {
          in.getline(name,MAX_STR_LEN); // get the name
      
          if(sym != ch_cur) // name has a leading whitspace
            in >> sym;
          else
            {
              in.close();
              strcpy(destination,name+1);
              return change;
            }
        }

      in.close(); return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
    }
}

// is stn name a valid name?
bool is_station(const char* stn)
{
  ifstream in; in.open("./stations.txt");

  char sym; char name[MAX_STR_LEN]; // symbol and station name being read
  in >> sym;
  while(!in.eof())
    {
      in.getline(name,MAX_STR_LEN); // get the name
      
      if(strcmp(stn,name+1) != 0) // name has a leading whitspace
        in >> sym;
      else
        {
          in.close();
          return true;
        }
    }

  in.close();
  return false;
}

// read the first dirction in route and resolve the direstion
Direction translate_dir(char* route)
{
  char d[3]; int i;
  
  for(i = 0; route[i]!=',' && route[i]!='\0'; i++)
    d[i] = route[i];

  d[i] = '\0';

  //eliminte the very first dirction
  if(route[i]=='\0') // the last direction as well
    route[0] = '\0';
  else
    {
      strcpy(route,route+i+1);
    }

  return string_to_direction(d);
}
