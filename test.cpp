#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "apishell.h"

using namespace std;

int main() {
    string delims = " \t\n";
    
    // 1: Simple tokenization
    string cl = "ls -l";
    vector<string> cl_tokens = tokenize(cl, delims);
    cout << "TEST 1: Simple tokenization" << endl;
    for (size_t i = 0; i < cl_tokens.size(); i++)
        cout << "[" << cl_tokens[i] << "]" << endl;

    // Build args 1
    cout << "\nBuild args 1" << endl;
    char **arguments = build_args(cl_tokens);
    for (size_t i = 0; i < cl_tokens.size(); i++)
       cout << "[[" << arguments[i] << "]]" << endl;
    args_cleanup(arguments, cl_tokens.size());

    // 2: Multiple spaces
    string cl2 = "ls -l shell  project       cs250";
    vector<string> cl_tokens2 = tokenize(cl2, delims);
    cout << "\nTEST 2: Multiple spaces" << endl;
    for (size_t i = 0; i < cl_tokens2.size(); i++)
        cout << "[" << cl_tokens2[i] << "]" << endl;

    // Build args 2
    cout << "\nBuild args 2" << endl;
    char **arguments2 = build_args(cl_tokens2);
    for (size_t i = 0; i < cl_tokens2.size(); i++)
       cout << "[[" << arguments2[i] << "]]" << endl;
    args_cleanup(arguments2, cl_tokens2.size());

    // 3: Quoted strings
    string cl3 = "echo \"hi world\" and also \"hello world\"";
    vector<string> cl_tokens3 = tokenize(cl3, delims);
    cout << "\nTEST 3: Quoted strings" << endl;
    for (size_t i = 0; i < cl_tokens3.size(); i++)
        cout << "[" << cl_tokens3[i] << "]" << endl;

    // Build args 3
    cout << "\nBuild args 3" << endl;
    char **arguments3 = build_args(cl_tokens3);
    for (size_t i = 0; i < cl_tokens3.size(); i++)
       cout << "[[" << arguments3[i] << "]]" << endl;
    args_cleanup(arguments3, cl_tokens3.size());

    // 4: Background test 1
    cout << "\nTEST 4.1: Background test 1 (with &)" << endl;
    vector<string> cl_tokens4 = tokenize("sleep 5 &", delims);
    bool bg1 = (!cl_tokens4.empty() && cl_tokens4.back() == "&");
    cout << "\"sleep 5 &\" → Background: " << (bg1 ? "Yes" : "No") << endl;

    // Background test 2
    cout << "\nTEST 4.2: Background test 2 (without &)" << endl;
    vector<string> cl_tokens5 = tokenize("ls -l", delims);
    bool bg2 = (!cl_tokens5.empty() && cl_tokens5.back() == "&");
    cout << "\"ls -l\" → Background: " << (bg2 ? "Yes" : "No") << endl;
}