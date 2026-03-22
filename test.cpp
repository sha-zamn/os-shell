#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "apishell.h"

using namespace std;

int main() {
    string delims = " \t\n";
    
    // TEST 1
    string cl = "ls -l";
    vector<string> cl_tokens = tokenize(cl, delims);
    
    cout << "TEST 1: Simple tokenization" << endl;
    for (size_t i = 0; i < cl_tokens.size(); i++)
        cout << "[" << cl_tokens[i] << "]" << endl;

    // TEST 2
    string cl2 = "ls -l shell project       cs250";
    vector<string> cl_tokens2 = tokenize(cl2, delims);
    
    cout << "\nTEST 2: Multiple spaces" << endl;
    for (size_t i = 0; i < cl_tokens2.size(); i++)
        cout << "[" << cl_tokens2[i] << "]" << endl;
    
    // TEST 3
    cout << "\nTEST ARGS BUILD 1" << endl;
    char **arguments = build_args(cl_tokens);
    for (size_t i = 0; i < cl_tokens.size(); i++)
       cout << "[[" << arguments[i] << "]]" << endl;
    args_cleanup(arguments, cl_tokens.size());

    cout << "\nTEST ARGS BUILD 2" << endl;
    char **arguments2 = build_args(cl_tokens2);
    for (size_t i = 0; i < cl_tokens2.size(); i++)
       cout << "[[" << arguments2[i] << "]]" << endl;
    args_cleanup(arguments2, cl_tokens2.size());

    // TEST 4
    cout << "\nTEST 3: Quoted strings" << endl;
    string cl3 = "echo \"hi world\" test";
    vector<string> cl_tokens3 = tokenize(cl3, delims);
    for (size_t i = 0; i < cl_tokens3.size(); i++)
        cout << "[" << cl_tokens3[i] << "]" << endl;

    return 0;
}