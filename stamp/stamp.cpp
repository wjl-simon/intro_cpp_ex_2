#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"
#include <string>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */

#define DIG_LEN 40
#define FILE_SIZE 50000

int leading_zeros(const char* digest)
{
  if(strlen(digest) != DIG_LEN) return -1;

  for(int i = 0; digest[i]!='\0'; i++)
    {
      if(!isxdigit(digest[i])) return -1;

      if(digest[i] != '0') return i;
    }
  
  return DIG_LEN;
}

// Q2
bool file_to_SHA1_digest(const char* filename, char digest[])
{
  ifstream file; file.open(filename);

  if(file.fail())
    {
      strcpy(digest,"error");
      return false;
    }

  // extract the file
  char content[FILE_SIZE]; int count = 0;
  char ch;
  
  file.get(ch);
  while(!file.eof())
    {
      content[count++] = ch;
      file.get(ch);
    }
  content[count] = '\0';
  
  // encode
  file.close();
  text_to_SHA1_digest(content,digest);
  return true;
}

// Q3
#define MAX_HD_LEN 512

bool make_header(const char* recipient, const char* filename, char header[])
{
  // recipient field
  strcpy(header, recipient);
  int len = strlen(header);
  
  // divider
  header[len++] = ':';

  // file SHA1
  if(!file_to_SHA1_digest(filename,header+len)) return false;

  // divider
  len = strlen(header);
  header[len++] = ':';
  header[len] = '\0';

  // append counter
  char counter_str[MAX_HD_LEN];
  if(find_counter(header,counter_str))
    {
      strcpy(header+len,counter_str);
      return true;
    }
  else
    return false;
}


bool find_counter(const char* header, char counter_str[])
{
  // header copy
  char hd[MAX_HD_LEN];
  strcpy(hd,header);
  const int LEN = strlen(hd);

  char digest[DIG_LEN+1];
  
  for(int i = 0; i < 1e8; i++)
    {
      // convert counter
      strcpy(counter_str,to_string(i).c_str());

      // append the counter field
      strcpy(hd+LEN,counter_str);

      // test the digest
      text_to_SHA1_digest(hd,digest);
  
      if(leading_zeros(digest) >= 5) // get a solution
        return true;
    }

  return false;
}

 /*
bool find_counter(const char* header, char counter_str[], int counter)
{
  // base case 1: after 10 million attempts
  if(counter >= 1e8) return false;
  
  // convert counter
  strcpy(counter_str,to_string(counter).c_str());
  
  // header copy
  char hd[MAX_HD_LEN];
  strcpy(hd,header);
  
  // append the counter field
  int len = strlen(hd);
  strcpy(hd+len,counter_str);

  cout << hd << endl;

  // test the digest
  char digest[DIG_LEN+1];
  text_to_SHA1_digest(hd,digest);
  
  if(leading_zeros(digest) >= 5) // get a solution
    return true;
  else
    return find_counter(header,counter_str,++counter);
}
*/

// Q4
MessageStatus check_header(const char* email, const char* header, const char* filename)
{
  // check format
  int col_count = 0; // counting ":"
  for(int i = 0; header[i]!='\0'; i++)
    {   
      if(header[i] == ':') col_count++;
    }
  
  if(col_count != 2) return INVALID_HEADER;

  // check first field match
  char recipient[MAX_HD_LEN];
  for(int i = 0; header[i]!=':'; i++)
    {   
      recipient[i] = header[i];

      if(header[i+1] == ':')
        recipient[i+1] = '\0';
    }

  if(strcmp(recipient,email)) return WRONG_RECIPIENT;

  // check SHAI of the msg body match
  char local_body_dig[DIG_LEN+1];
  char received_body_dig[DIG_LEN+1];
  file_to_SHA1_digest(filename,local_body_dig);

  col_count = 0; // counting ":"
  int pos = 0;
  for(int i = 0; header[i]!='\0'; i++)
    {
      if(header[i] == ':')
        {
          col_count++;
          if(col_count == 2)
            {
              received_body_dig[pos++] = '\0';
              break;
            }
          else
            continue;
        }

      if(col_count == 1)
        {
          received_body_dig[pos++] = header[i];
        }
    }

  if(strcmp(received_body_dig,local_body_dig)) return INVALID_MESSAGE_DIGEST;

  // check the SHA1 of the header is acceptable
  char digest[DIG_LEN+1];
  text_to_SHA1_digest(header,digest);
  if(leading_zeros(digest) >= 5)
    return VALID_EMAIL;
  else
    return INVALID_HEADER_DIGEST;
}
