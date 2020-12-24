#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <fstream>
#include <string>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */

//Q1
#define MAX_STR_LEN 512
int count_words(const char* line)
{
  int count = 1; char out[MAX_STR_LEN];
  // if empty?
  if(!get_word(line,count,out)) return 0;

  // non empty
  while(get_word(line,++count,out));

  return --count;
}


//Q2
bool find_phonetic_ending(const char* word, char* phonetic_ending)
{
  ifstream in; in.open("dictionary.txt");

  char str1[MAX_STR_LEN] = {}; char str2[MAX_STR_LEN] = {};

  in >> str1; // get the first word
  while(!in.eof())
    {
      for(int i = 0; i < MAX_STR_LEN; i++) str2[i] = '\0';

      in.getline(str2,MAX_STR_LEN); // phonemes
      
      if(strcmp(str1,word) == 0) // the word is in dic!
        {
          in.close();
          extract_phonetic_ending(str2,phonetic_ending);
          return true;
        }
      else
        {
          for(int i = 0; i < MAX_STR_LEN; i++) str1[i] = '\0';
          
          in >> str1;
        }
    }

  in.close();
  return false;
}

void extract_phonetic_ending(const char* phonemes, char* phonetic_ending)
{
  const int LEN = count_words(phonemes); // num of phonemes
  char str[MAX_STR_LEN] = {};

  // find the last phoneme that has a vowel
  int vol_pos = 0;
  for(int i = LEN; i > 0; i--)
    {
      for(int j = 0; j < MAX_STR_LEN; j++) str[j] = '\0';

      get_word(phonemes,i,str);
      
      if(*str == 'A' || *str == 'E' || *str == 'I' ||
         *str == 'O' || *str == 'U')
        {
          vol_pos = i;
          break;
        } 
    }

  // if no vowel: the last one
  if(vol_pos == 0)
    get_word(phonemes,LEN,phonetic_ending);
  else // has vowel
    {
      for(int i = vol_pos; i <= LEN; i++)
        {
          for(int j = 0; j < MAX_STR_LEN; j++) str[j] = '\0';
      
          get_word(phonemes,i,str);

          if(i == vol_pos)
            strcpy(phonetic_ending,str);
          else
            strcat(phonetic_ending,str);
        }
    }
}


//Q3
bool find_rhyme_scheme(const char* filename, char* scheme)
{
  ifstream in; in.open(filename);
  if(in.fail()) return false;

  rhyming_letter(RESET);

  char ph_e[MAX_STR_LEN] = {}; char last_word[MAX_STR_LEN];
  int word_count;
  int pos = 0; // for the sheme
  while(!in.eof())
    {
      for(int i = 0; i < MAX_STR_LEN; i++) ph_e[i] = '\0';

      // get a line of the poem
      in.getline(ph_e,MAX_STR_LEN);

      // get the phonetic ending
      word_count = count_words(ph_e);
      get_word(ph_e,word_count,last_word);
      if(!find_phonetic_ending(last_word,ph_e))
        {
          in.close();
          return false;
        }
      // find scheme
      scheme[pos++] = rhyming_letter(ph_e);
    }
  
  scheme[pos] = '\0'; 
  in.close();
  return true;
}


//Q4
const char* identify_sonnet(const char* filename)
{
  char scheme[MAX_STR_LEN] = {};
  char sh[MAX_STR_LEN] = {};
  char pe[MAX_STR_LEN] = {};
  char sp[MAX_STR_LEN] = {};

  // get the scheme
  find_rhyme_scheme(filename,scheme);

  // get the shakespearean one
  find_rhyme_scheme("shakespeare.txt",sh);
  find_rhyme_scheme("petrarch.txt",pe);
  find_rhyme_scheme("spenser.txt",sp);

  if(strcmp(sh,scheme) == 0) return "Shakespearean";
  else if(strcmp(pe,scheme) == 0) return "Petrarchan";
  else if(strcmp(sp,scheme) == 0) return "Spenserian";
  else return "Unknown";
}
