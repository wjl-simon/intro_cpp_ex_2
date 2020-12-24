#ifndef SPELL_H
#define SPELL_H

#define MAX_LENGTH 512

#include <climits>


// q1
int frequency(const char* target);


// q2: d-l distance function
// default max_dis = 0: unlimited steps to explore the d_l_distance
int d_l_dis(const char* word1, const char* word2, int i, int j);

int edit_distance(const char* word1, const char* word2, int max_dis = INT_MAX);

int max(int a, int b);

int min(int a, int b);

int min(int a, int b, int c);

int min(int a, int b, int c, int d);

int indicator(const char* word1, const char* word2, int i, int j);


// q3
bool spell_correct(const char* word, char fixed[]);

// Bonus
int edit_distance_bonus(const char* a, const char* b, int max_dis = INT_MAX);

#endif
