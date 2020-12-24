#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#include <cctype>

//Q1
void prepare(const char* input, char* output);

//Q2
#define G_SIZE 6
void grid(const char* codeword, char square[][G_SIZE]);

// check if before str[pos] , character letter has occurred
bool occurs_before(const char* str, char letter, int pos);


//Q3
void bigram(char square[][G_SIZE], char inchar1, char inchar2, char& outchar1, char& outchar2);

int find_row(char square[][G_SIZE], char ch);

int find_col(char square[][G_SIZE], char ch);


//Q4
bool encode(char square[][G_SIZE], const char* prepared, char* encoded);
bool encode_solver(char square[][G_SIZE], const char* prepared, char* encoded);


// BONUS
bool decode(char square[][G_SIZE], const char* prepared, char* decoded);
bool decode_solver(char square[][G_SIZE], const char* prepared, char* decoded);
#endif
