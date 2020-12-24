#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

bool valid_step(const char* cur, const char* next)
{
  bool flag = false; // indicating a diffrent char is found
  
  for(int i = 0; cur[i] != '\0'; i++)
    {
      if(!flag) // havent found one
        {
          if(cur[i] != next[i])
            flag = true;
        }
      else // already found one
        {
          if(cur[i] != next[i]) // another diffrent char
            return false;
        }
    }

  if(!flag) // no diffrent char
    return false;
  else
    return dictionary_search(next);
}


#define MAX_STR_LEN 100

bool display_chain(const char* chain[], ostream& output_stream)
{
  char word[MAX_STR_LEN];
  
  for(int i = 0; chain[i]!=NULL; i++)
    {
      if(i > 0 && chain[i+1]!=NULL) // neither the head nor the end
        {
          strcpy(word,chain[i]);
          upper_to_lower(word);
          output_stream << word << endl;
        }
      else // the head or the end
        {
          strcpy(word,chain[i]);
          lower_to_upper(word);
          output_stream << chain[i] << endl;
        }
    }
  
  return true;
}

void upper_to_lower(char str[])
{
  for(int i = 0; str[i]!='\0'; i++)
    str[i] = tolower(str[i]);
}

void lower_to_upper(char str[])
{
  for(int i = 0; str[i]!='\0'; i++)
    str[i] = toupper(str[i]);
}  

bool valid_chain(const char* chain[])
{
  if(chain[0]==NULL || chain[1]==NULL) // only one word: not valid
    return false;
  
  for(int i = 0; chain[i+1]!=NULL; i++)
    {
      if(!valid_step(chain[i],chain[i+1])) // is valid step?
        return false;
      else if(repeat_before(chain,i)) // hasn't repeated?
        return false;
    }
  
  return true;
}

// test if there is a repeated word before the pos-th word
// in the chain
bool repeat_before(const char* chain[], int pos)
{
  for(int i = 0; i < pos; i++)
    {
      if(strcmp(chain[i],chain[pos]) == 0)
        return true;
    }
  
  return false;
}



// Q4
/*
bool find_chain(const char* start, const char* target, const char* answer_chain[],
                int max_steps, int cur_step)
{
  // base case 1
  if(cur_step > max_steps) return false;

  // base case 2: get the final solution
  if(strcmp(target,answer_chain[cur_step]) == 0)
    {
      answer_chain[cur_step+1] = NULL; // append a NULL
      return true;
    }

  // recursion
  ifstream input; input.open("./words.txt");
  if(find_next_step(start,answer_chain,cur_step,input)) // one valid next step
    {
      input.close();
      return find_chain(answer_chain[cur_step], target, answer_chain, max_steps, cur_step);
    }
  else // base case 3: no further valid steps
    {
      input.close();
      return false;
    }
}

// test if there is an identical str as "word" in chain
bool repeat_before2(const char* chain[], const char* word)
{
  for(int i = 0; chain[i]!=NULL; i++)
    {
      if(strcmp(chain[i],word) == 0)
        return true;
    }
  
  return false;
}

bool find_next_step(const char* cur_word, const char* cur_chain[], int& cur_step, ifstream& input)
{
  // base case 1: no solution anymore
  if(input.eof()) return false;
  
  char candidate[MAX_STR_LEN];
  input >> candidate;

  // base case 2: get a solution
  if(valid_step(cur_word,candidate))
    {
      if(!repeat_before2(cur_chain,candidate)) // not repeatted
       {
          cur_chain[++cur_step] = candidate;
          return true;
       }
    }

  // recursion
  return find_next_step(cur_word,cur_chain, cur_step, input);
}
*/



bool find_chain(const char* start, const char* target, const char* answer_chain[],
                int max_steps, int cur_step)
{
  // filling the starting word first
  if(cur_step == 0) answer_chain[0] = start;

  // base case 1
  if(cur_step > max_steps) return false;

  // base case 2: get the final solution
  if(strcmp(target,answer_chain[cur_step]) == 0) return true;

  // recursion
  if(find_next_step(answer_chain,cur_step)) // one valid next step
    return find_chain(start, target, answer_chain, max_steps, cur_step);
  else // base case 3: no further valid steps
      return false;
}


// get a valid chain, given cur_word
bool find_next_step(const char* cur_chain[], int& cur_step, char*** candidate)
{
  char cur_word[MAX_STR_LEN];
  strcpy(cur_word,cur_chain[cur_step]);
  
  char cand[MAX_STR_LEN];
  strcpy(cand,cur_word);
  int sol_num = 0; char*** temp;
  for(int i = 0; cur_word[i]!='\0'; i++)
    {
      for(char j = 'A'; j <='Z'; j++)
        {
          cand[i] = j;
          // the cand must be in the dictionary and form a vaid chain
          if(dictionary_search(cand))
            {
              cur_chain[cur_step+1] = cand;
              cur_chain[cur_step+2] = NULL;

              if(valid_chain(cur_chain))
                {
                  solu_num++;
                  temp = new char[sol_num][MAX_STR_LEN];
                  //cur_step++;
                  //cout << "At current step " << cur_step << endl;
                  //display_chain(cur_chain,cout);
                  return true;
                }
            }
        }
    }
  
  return false;
}

void copy_chain(char*** chain1, char*** chain2)
{
  for(int i = 0; (*chain1)[i]!=NULL; i++)
    (*chain2)[i] = 
}
