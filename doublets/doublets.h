/*  This pre-supplied function looks up a given word in the dictionary
    of approved words. It is assumed that the input word is in uppercase.
    The function returns true if the word is in the dictionary.
    Otherwise returns false. */

#include <iostream>
#include <fstream>


bool dictionary_search(const char *word);

/* add your function prototypes here */

bool valid_step(const char* cur, const char* next);

bool display_chain(const char* chain[], std::ostream& output_stream);

void upper_to_lower(char str[]);

void lower_to_upper(char str[]);

bool valid_chain(const char* chain[]);

bool repeat_before(const char* chain[], int pos);


//Q4
/*
bool find_chain(const char* start, const char* target, const char* answer_chain[],
                int max_steps, int cur_step = 0);

// test if there is an identical str as "word" in chain
bool repeat_before2(const char* chain[], const char* word);

bool find_next_step(const char* cur_word, const char* cur_chain[],
                    int& cur_step, std::ifstream& input);
*/

bool find_chain(const char* start, const char* target, const char* answer_chain[],
                int max_steps, int cur_step = 0);

bool find_next_step(const char* cur_chain[], int& cur_step);
