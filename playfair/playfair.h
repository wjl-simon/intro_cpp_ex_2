#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#define GRID_SIZE 6

// Q1
void prepare(const char* input, char output[]);


// Q2
void grid(const char* codeword, char square[][GRID_SIZE]);

bool occurs_before(const char square[][GRID_SIZE], char ch, int row, int col);

bool occurs_before(const char str[], char letter, int pos);


// Q3
bool bigram(const char square[][GRID_SIZE], char inchar1, char inchar2,
            char& outchar1, char& outchar2);

int find_row(const char square[][GRID_SIZE], char ch);

int find_col(const char square[][GRID_SIZE], char ch);

//Q4

void encode(const char square[][GRID_SIZE], char* prepared, char encoded[]);

#endif
