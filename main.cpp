// testing the api shell utility helper functions
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <csignal>
#include <sys/wait.h>
#include "apishell.h"
#include "circular_buffer.h"

using namespace std;

#define MAXLINE 120
#define MAX_HISTORY 10

int main()
{
    // testing tokenzation
    string delimiters = " \t\n";
    int should_run = 1;

    signal(SIGCHLD, sigchld_handler);

    CircularBuffer history(MAX_HISTORY);
    
    while (should_run) {
        cout << "osclass> ";
        cout.flush();
        
        string command_line;
        getline(cin, command_line);

        if (!command_line.empty() && command_line[0] == '!') {
            string num_str = command_line.substr(1);
            
            if (num_str == "!") {
                if (history.back().empty()) {
                    cout << "No such command in history." << endl;
                    continue;
                }
            command_line = history.back();
            cout << command_line << endl;
            } 
            // !N = parse digits
            else if (isdigit(num_str[0])) {
                size_t cmd_no = 0;
                for (char c : num_str) {
                    if (!isdigit(c)) {
                        cout << "No such command in history." << endl;
                        continue;
                    }
                    cmd_no = cmd_no * 10 + (c - '0');
                }
                if (history.get_cmd(cmd_no).empty()) {
                    cout << "No command in history." << endl;
                    continue;
                }
                command_line = history.get_cmd(cmd_no);;
                cout << command_line << endl;
            } 
            else {
                cout << "No such command in history." << endl;
                continue;
            }
        }
    
        vector<string> tokens = tokenize(command_line, delimiters);
        
        if (tokens.empty()) continue;

        if (tokens[0] == "history") {
            history.display_history();
            cout << "DEBUG: total_commands=" << history.total_commands() 
             << " size=" << history.size() << endl;
            continue;
        }
    
        if (tokens[0] == "exit") {
            int zombies = check_for_zombies();
            if (zombies > 0) {
                cout << "Warning: Reaped " << zombies << " zombie process(es) on exit" << endl;
            }
            should_run = 0;
            continue;
        }

        if (tokens[0] == "checkzombies") {
            int zombies = check_for_zombies();
            if (zombies == 0) {
                cout << "No zombie processes detected" << endl;
            } else {
                cout << "Reaped " << zombies << " zombie process(es)" << endl;
            }
            continue;
        }

        history.add_cmd(command_line);

        bool is_bg = background(tokens);

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
            if (!is_bg) {
                waitpid(pid, nullptr, 0);
            } else {
                cout << "Parent does not wait." << endl;
                cout << "[background pid " << pid << "]" << endl;
            }
        }

        // Cleanup
        args_cleanup(args, argc);
    }
    
    return 0;
}