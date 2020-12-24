#include "playfair.h"
#include <cstring>

using namespace std;

//Q1
void prepare(const char* input, char* output)
{
  int pos = 0;
  // only copy the alp/num
  for(int i = 0; input[i]!='\0'; i++)
    {
      if(isalnum(input[i]))
        output[pos++] = toupper(input[i]);
    }
  
  if(pos % 2 != 0) // add X if length is 
    output[pos++] = 'X';
  
  output[pos] = '\0'; // add terminator
}

//Q2
#define G_SIZE 6
void grid(const char* codeword, char square[][G_SIZE])
{
  const int LEN = strlen(codeword);
  
  // fill in the chars in codeword first
  int pos = 0;
  for(int k = 0; k < LEN; k++)
    {
      if(occurs_before(codeword,codeword[k],k)) continue;

      square[pos/G_SIZE][pos%G_SIZE] = codeword[k];
      pos++;
    }

  //fill in the letters which are not in the codeword
  for(char ch = 'A'; ch <= 'Z'; ch++)
    {
      // ignored chars that occurs in codeword
      if(occurs_before(codeword,ch,LEN)) continue;
      
      square[pos/G_SIZE][pos%G_SIZE] = ch;
      pos++;
    }

  // fill in the digits
  for(char ch = '0'; ch <= '9'; ch++)
    {
      // ignored chars that occurs in codeword
      if(occurs_before(codeword,ch,LEN)) continue;

      square[pos/G_SIZE][pos%G_SIZE] = ch;
      pos++;
    }
}

// check if before str[pos] , character letter has occurred
bool occurs_before(const char* str, char letter, int pos)
{
  for(int i = 0; i < pos && str[i] != '\0'; i++)
    {
      if(str[i] == letter) return true;
    }

  return false;
}

//Q3
void bigram(char square[][G_SIZE], char inchar1, char inchar2, char& outchar1, char& outchar2)
{
  const int R1 = find_row(square,inchar1);
  const int C1 = find_col(square,inchar1);
  const int R2 = find_row(square,inchar2);
  const int C2 = find_col(square,inchar2);

  // temp
  char encode1 = square[R1][C2];
  char encode2 = square[R2][C1];

  // return
  outchar1 = encode1; outchar2 = encode2;
}


int find_row(char square[][G_SIZE], char ch)
{
  for(int i = 0; i < G_SIZE; i++)
    {
      for(int j = 0; j < G_SIZE; j++)
        {
          if(square[i][j] == ch)
            return i;
        }
    }

  return -1;
}

int find_col(char square[][G_SIZE], char ch)
{
  for(int i = 0; i < G_SIZE; i++)
    {
      for(int j = 0; j < G_SIZE; j++)
        {
          if(square[i][j] == ch)
            return j;
        }
    }

  return -1;
}


//Q4
#define MAX_STR_LEN 512
bool encode(char square[][G_SIZE], const char* prepared, char* encoded)
{
  char out[MAX_STR_LEN];
  
  if(encode_solver(square,prepared,out))
    {
      strcpy(encoded,out);
      return true;
    }
  else
    return false;
}


bool encode_solver(char square[][G_SIZE], const char* prepared, char* encoded)
{   
  //base case 1
  if(*prepared == '\0')
    {
      *encoded = '\0';
      return true;
    }
  // base case 2: odd number of char
  if(*(prepared+1) == '\0') return false;
  
  // normal case: encode
  bigram(square, *prepared, *(prepared+1), *encoded, *(encoded+1));

  if(encode_solver(square,prepared+2,encoded+2)) return true;
  else return false;
}


// BONUS
bool decode(char square[][G_SIZE], const char* prepared, char* decoded)
{
  char out[MAX_STR_LEN];
  
  if(decode_solver(square,prepared,out))
    {
      strcpy(decoded,out);
      return true;
    }
  else
    return false;
}


bool decode_solver(char square[][G_SIZE], const char* prepared, char* decoded)
{   
  //base case 1
  if(*prepared == '\0')
    {
      *decoded = '\0';
      return true;
    }
  // base case 2: odd number of char
  if(*(prepared+1) == '\0') return false;
  
  // normal case: encode
  bigram(square, *prepared, *(prepared+1), *decoded, *(decoded+1));

  if(decode_solver(square,prepared+2,decoded+2)) return true;
  else return false;
}
