#ifndef APISHELL_H
#define APISHELL_H

#include <vector>
#include <string>

using namespace std;

string handle_quotes(string s);

// returns the tokens of string s as a vector of strings
// using h=the delimters for splitting string s
vector<string> tokenize(string s, string delimiters);

// returns the args array, ready for use by the exec system calls,
// by converting the vector of strings into an array of char pointers

char **build_args(const vector<string>& tokens);

void debug_args(char *args[], int argc);

// release memory that has been allocated to hold the args
// corresponding to tokens
void args_cleanup(char **args, int argc);

#endif