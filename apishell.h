#ifndef APISHELL_H
#define APISHELL_H

#include <vector>
#include <string>
#include <csignal>

using namespace std;

string handle_quotes(string s);

// returns the tokens of string s as a vector of strings
// using the delimters for splitting string s
vector<string> tokenize(string s, string delimiters);

// returns the args array, ready for use by the exec system calls,
// by converting the vector of strings into an array of char pointers

char **build_args(const vector<string>& tokens);

void debug_args(char *args[], int argc);

bool background(vector<string>& tokens);

// release memory that has been allocated to hold the args corresponding to tokens
void args_cleanup(char **args, int argc);

// SIGCHLD handler reaps background children
void sigchld_handler(int sig);

int check_for_zombies();

#endif