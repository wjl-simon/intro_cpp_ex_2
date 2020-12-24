
#include <iostream>
#include <fstream>
#include <cstring>
#include "spell.h"
#include <climits>

#define STEP_SIZE 5 // used for the q3

using namespace std;

// q1
int frequency(const char* target)
{
  ifstream in; in.open("./words.dat");

  int freq; char key[MAX_LENGTH];

  // get the next line if not matched
  while(!in.eof() && strcmp(key,target) != 0)
  {
    in >> freq >> key;
  }
  in.close();

  if(strcmp(key,target) == 0)
  {
    return freq;
  }
  else // no match at all: frequency is zero
    return 0;
}


// q2: d-l distance function
int d_l_dis(const char* word1, const char* word2, int i, int j)
{
  if(min(i,j) == 0) return max(i,j);

  if(i > 1 && j > 1 && word1[i-1]==word2[j-2] && word1[i-2]==word2[j-1])
  {
    return min(d_l_dis(word1, word2, i-1, j) + 1,
               d_l_dis(word1, word2, i, j-1) + 1,
               d_l_dis(word1, word2, i-1, j-1) + indicator(word1, word2, i, j),
               d_l_dis(word1, word2, i-2, j-2) + 1
               ); 
  }
  else
  {
    return min(d_l_dis(word1, word2, i-1, j) + 1,
               d_l_dis(word1, word2, i, j-1) + 1,
               d_l_dis(word1, word2, i-1, j-1) + indicator(word1, word2, i, j)
               );
  }

}

int edit_distance(const char* word1, const char* word2, int max_dis)
{
  int len1 = strlen(word1), len2 = strlen(word2);
  //return d_l_dis(word1, word2, len1, len2, max_dis);

  // compare the first a few characters of two words
  for(int i = 0; i <= min(len1, len2); i = i + min(len1, len2)/STEP_SIZE + 1)
  {
    if(d_l_dis(word1, word2, i, i) > max_dis)
      return  max_dis + 1;
  }

  // do the full search if we happen to get out of the for loop
  return d_l_dis(word1, word2, len1, len2);
}


int max(int a, int b){ return a>=b ? a : b;}


int min(int a, int b){ return a<=b ? a : b;}


int min(int a, int b, int c)
{
  int temp = (a<=b ? a : b);
  return temp<=c ? temp : c;
}

int min(int a, int b, int c, int d)
{
  int temp = (a<=b ? a : b);
  temp = (temp<=c ? temp : c);
  return temp<=d ? temp : d;
}


int indicator(const char* word1, const char* word2, int i, int j)
{
  return word1[i-1]==word2[j-1] ? 0 : 1;
}


// q3
bool spell_correct(const char* word, char fixed[])
{
  if(frequency(word)) // frequency is nonzero: word is already in the dictionary
  {
    strcpy(fixed,word);
    return false;
  }

  // otherwise: suggest a correct spelling
  ifstream input; input.open("./words.dat");

  int freq; char key[MAX_LENGTH];

  const char* NO_MATCH = "NO MATCH";
  char cand1[MAX_LENGTH]; strcpy(cand1, NO_MATCH); // candidate word of distance 1 with max freq 
  char cand2[MAX_LENGTH]; strcpy(cand2, NO_MATCH); // with distance 2

  int cand1_max_freq = 0, cand2_max_freq = 0; // their corresonding max freq

  int current_dis = 0; // edit distance bwtween current word and the target word
  while(!input.eof())
  {
    input >> freq >> key;
    current_dis = edit_distance(word,key,2);
    //current_dis = edit_distance_bonus(word,key,2);
    
    if(current_dis == 1) // distance of 1
    {
      if(freq > cand1_max_freq) // only update when the freq is higher
      {
        strcpy(cand1,key);
        cand1_max_freq = freq;
      }
    }
    else if(current_dis == 2) // distance of 2
    {
      if(freq > cand2_max_freq)
      {
        strcpy(cand2,key);
        cand2_max_freq = freq;
      }
    }
  }

  input.close();

  if(strcmp(cand1,NO_MATCH)) // there's a word with distance of 1
  {
    strcpy(fixed, cand1);
    return true;
  }
  else if(strcmp(cand2, NO_MATCH)) // a word with distance of 2
  {
    strcpy(fixed, cand2);
    return true;
  }
  else // no word with distance <= 2
  {
    strcpy(fixed, word);
    return false;
  }
}

// Bonus
int edit_distance_bonus(const char* a, const char* b, int max_dis)
{
  const int a_len = strlen(a);
  const int b_len = strlen(b);

  // base case 1
  if(a_len == 0) return b_len;
  else if(b_len == 0) return a_len;

  // recursion
  int x1 = edit_distance_bonus(a+1, b, max_dis) + 1;
  int x2 = edit_distance_bonus(a, b+1, max_dis) + 1;
  int indicator = *a != *b ? 1 : 0;
  int x3 = edit_distance_bonus(a+1, b+1, max_dis) + indicator;

  int min = x1 < x2 ? x1 : x2;
  min = min < x3 ? min : x3;
  
  if(a_len>1 && b_len>1 && *a==*(b+1) && *(a+1)==*b)
    {
      int x4 = edit_distance_bonus(a+2, b+2, max_dis) + 1;
      min =  min < x4 ? min : x4;

      return min <= max_dis ? min : max_dis + 1;
    }
  else
    return min <= max_dis ? min : max_dis + 1;
}
