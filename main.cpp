// testing the api shell utility helper functions
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "apishell.h"

using namespace std;

#define MAXLINE 120

int main()
{
    // testing tokenzation
    string delimiters = " \t\n";
    int should_run = 1;
    
    while (should_run) {
        cout << "osclass> ";
        cout.flush();
        
        string command_line;
        getline(cin, command_line);
    
        vector<string> tokens = tokenize(command_line, delimiters);
        
        if (tokens.empty()) {
            continue;
        }

        bool background = false;
        if (tokens.back() == "&") {
            background = true;
            tokens.pop_back();
        }

        if (tokens[0] == "exit") {
            should_run = 0;
            continue;
        }

        char **args = build_args(tokens);
        int argc = tokens.size();

        debug_args(args, argc);

        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            // Child process
            execvp(args[0], args);
            perror("execvp");
            _exit(1);
        } else {
            // Parent process
            if (!background) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                cout << "[background pid " << pid << "]" << endl;
            }
        }

        // Cleanup
        args_cleanup(args, argc);
    }
    
    return 0;
}