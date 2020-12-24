#include <iostream>
#include <cctype>
#include <cstring>
#include "playfair.h"

using  namespace std;

void prepare(const char* input, char output[])
{
  int in_size = strlen(input);
  //nonzeroint isaalnum();
  //char toupper();
  //char temp[int_size]; strcpy(temp,input);

  int j = 0; // counter for output
  for(int i = 0; i < in_size; i++)
  {
    if(isalnum(input[i]))
      output[j++] = toupper(input[i]);
  }

  // append at the end
  if(j%2 == 1) // odd number: add an X
  {
    output[j++] = 'X';
  }
  output[j++] = '\0';
}

void grid(const char* codeword, char square[][GRID_SIZE])
{
  int in_size = strlen(codeword);
  int count1 = 0; // count the position of chars in codeword
  int count2 = 0; // increment by 1 if a new letter appears 
  const int MAX_C2 = 26; // there're 26 different letters in English

  for(int i = 0; i < GRID_SIZE; i++){
    for(int j = 0; j < GRID_SIZE; j++){

      // fill in the codeword first
      if(count1 < in_size){
        // find the next char that hasn't appeared before
        while(occurs_before(codeword,codeword[count1],count1)){
          count1++;
        }
        
        square[i][j] = codeword[count1++];
        count2++;
      }
      // fill in the other letters
      else if(count2 < MAX_C2){
        for(char ch = 'A'; ch <= 'Z'; ch++){
          if(!occurs_before(square,ch,i,j)){
            square[i][j] = ch;
            count2++; break;
          }
        }
      }
      // fill in the digits
      else{
        for(char ch = '0'; ch <= '9'; ch++){
          if(!occurs_before(square,ch,i,j)){
            square[i][j] = ch;
            break;
          }
        }
      }
      
    }
  }
  
}

bool occurs_before(const char str[], char letter, int pos)
{
  for(int i = 0; i < pos; i++)
  {
    if(letter == str[i])
      return true;
  }

  return false;
}

bool occurs_before(const char square[][GRID_SIZE], char ch, int row, int col)
{
  for(int i = 0; i <= row; i++)
    {
      for(int j = 0; j < GRID_SIZE; j++)
        {
          if(i == row && j == col ) return false; // at [i][j]: exit
          
          if(ch == square[i][j]) // ch is in the square
            return true;
        }
    }

  return false;
}


bool bigram(const char square[][GRID_SIZE], char inchar1, char inchar2,
            char& outchar1, char& outchar2)
{
  if(inchar1 != '\0' && inchar2 != '\0')
  {
    const int R1 = find_row(square,inchar1);
    const int C1 = find_col(square,inchar1);
    const int R2 = find_row(square,inchar2);
    const int C2 = find_col(square,inchar2);

    outchar1 = square[R1][C2];
    outchar2 = square[R2][C1];

    return true;
  }
  else return false;
}


int find_row(const char square[][GRID_SIZE], char ch)
{
  for(int i = 0; i < GRID_SIZE; i++)
    {
      for(int j = 0; j < GRID_SIZE; j++)
        {
          if(square[i][j] == ch) return i;
        }
    }

  return -1;

}

int find_col(const char square[][GRID_SIZE], char ch)
{
    for(int i = 0; i < GRID_SIZE; i++)
    {
      for(int j = 0; j < GRID_SIZE; j++)
        {
          if(square[i][j] == ch) return j;
        }
    }

  return -1;

}


void encode(const char square[][GRID_SIZE], char* prepared, char encoded[])
{
  if(bigram(square,*prepared,*(prepared+1),*encoded,*(encoded+1)))
  {
    encode(square, prepared+2, encoded+2);
  }

}
