#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <sys/wait.h>
#include <cerrno>
#include "apishell.h"

using namespace std;

// Handle quoted strings by replacing quotes with \x01
string handle_quotes(string s) {
    char *buffer = new char[s.size() + 1];
    strcpy(buffer, s.c_str());

    char *src = buffer;
    char *dst = buffer;
    bool in_quotes = false;

    while (*src != '\0') {
        if (*src == '"') {
            in_quotes = !in_quotes;
            src++;  //skip the quote
        } else if (*src == ' ' && in_quotes) {
            *dst = '\x01';  //placeholder
            dst++;
            src++;
        } else {
            *dst = *src;
            dst++;
            src++;
        }
    }
    *dst = '\0';  //null-terminate

    // convert back to c++ string
    string result(buffer);
    delete[] buffer;

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
        // convert quotes placeholder back to spaces
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

// Background check
bool background(vector<string>& tokens) {
    if (!tokens.empty() && tokens.back() == "&") {
        tokens.pop_back();
        return true;
    }
    return false;
}

void args_cleanup(char **args, int argc)
{
    for (int i = 0; i < argc && args[i] != nullptr; i++) {
        free(args[i]);  //strdup uses malloc
    }
    delete [] args;
}

void debug_args(char *args[], int argc) {
    cout << "Argument Count: " << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << "args[" << i << "] = '" << args[i] << "'" << endl;
    }
    cout << "args[" << argc << "] = NULL" << endl;
}

void sigchld_handler(int sig) {
    int saved_errno = errno;  // Preserve errno
    // Reap all finished children
    while (waitpid(-1, nullptr, WNOHANG) > 0) {
    }
    errno = saved_errno;  // Restore errno
}

int check_for_zombies() {
    int zombie_count = 0;
    int status;

    while (waitpid(-1, &status, WNOHANG) > 0) { //allows the parent to check on child termination status without stopping its own execution
        zombie_count++;
    }
    
    return zombie_count;
}