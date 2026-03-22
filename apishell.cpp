// implementation of the shell API
// use C++ as much as possible
// api_shell.cc
// date: 02/10/2026

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include "apishell.h"

using namespace std;

string handle_quotes(string s) {
    string result;
    bool in_quotes = false;
    
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '"') {
            in_quotes = !in_quotes;
        } else if (s[i] == ' ' && in_quotes) {
            result += '\x01';
        } else {
            result += s[i];
        }
    }
    return result;
}

vector<string> tokenize(string s, string delimiters)
{
    vector<string> tokens;
    s = handle_quotes(s);

    int slen = s.size();
    char *buffer = new char[slen + 1];
    // copy the sequence of characters from s to the buffer
    strcpy(buffer, s.c_str()); //converting c string to char *string
    //start tokenization
    char *token = strtok(buffer, delimiters.c_str());
    while (token != nullptr)
    {
        string token_str(token);
        // convert back to spaces
        for (size_t i = 0; i < token_str.length(); i++) {
            if (token_str[i] == '\x01') {
                token_str[i] = ' ';
            }
        }
        tokens.push_back(string(token_str)); //building c++ string from the token
        token = strtok(nullptr, delimiters.c_str());
    }
    // end of tokenization
    delete[] buffer;
    // return the vector of tokens
    return tokens;
}

char **build_args(const vector<string>& tokens)
{
    int vlen = tokens.size();
    char **args = new char*[vlen + 1];
    for (int i = 0; i < vlen; i++)
    {
        // allocate and create the c string from the corrosponding string token
        args[i] = strdup(tokens[i].c_str());
    }
    args[vlen] = NULL;
    // return args which is exec-format ready
    return args;
}

void debug_args(char *args[], int argc) {
    cout << "Argument Count: " << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << "args[" << i << "] = '" << args[i] << "'" << endl;
    }
    cout << "args[" << argc << "] = NULL" << endl;
}

void args_cleanup(char **args, int argc)
{
    for (int i = 0; i < argc; i++)
        delete args[i];
    delete [] args; //check if argc =0 or ptr = null
}