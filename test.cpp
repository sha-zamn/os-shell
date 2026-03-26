#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include "apishell.h"
#include "circular_buffer.h"

using namespace std;

int main() {
    string delims = " \t\n";
    
    // 1: Simple tokenization
    string cl1 = "ls -l";
    vector<string> cl_tokens1 = tokenize(cl1, delims);
    cout << "TEST 1: Simple tokenization" << endl;
    cout << "Input: \"" << cl1 << "\"" << endl;
    for (size_t i = 0; i < cl_tokens1.size(); i++)
        cout << "[" << cl_tokens1[i] << "]" << endl;

    // Build args 1
    cout << "Build args 1" << endl;
    char **arguments1 = build_args(cl_tokens1);
    for (size_t i = 0; i < cl_tokens1.size(); i++)
       cout << "[[" << arguments1[i] << "]]" << endl;
    args_cleanup(arguments1, cl_tokens1.size());

    // 2: Multiple spaces & tabs
    string cl2 = "ls -l shell  project       cs250";
    vector<string> cl_tokens2 = tokenize(cl2, delims);
    cout << "\nTEST 2: Multiple spaces & tabs" << endl;
    cout << "Input: \"" << cl2 << "\"" << endl;
    for (size_t i = 0; i < cl_tokens2.size(); i++)
        cout << "[" << cl_tokens2[i] << "]" << endl;

    // Build args 2
    cout << "Build args 2" << endl;
    char **arguments2 = build_args(cl_tokens2);
    for (size_t i = 0; i < cl_tokens2.size(); i++)
       cout << "[[" << arguments2[i] << "]]" << endl;
    args_cleanup(arguments2, cl_tokens2.size());

    // 3: Quoted strings
    string cl3 = "echo \"hi world\" and also \"hello world\"";
    vector<string> cl_tokens3 = tokenize(cl3, delims);
    cout << "\nTEST 3: Quoted strings" << endl;
    cout << "Input: \"" << cl3 << "\"" << endl;
    for (size_t i = 0; i < cl_tokens3.size(); i++)
        cout << "[" << cl_tokens3[i] << "]" << endl;

    // Build args 3
    cout << "Build args 3" << endl;
    char **arguments3 = build_args(cl_tokens3);
    for (size_t i = 0; i < cl_tokens3.size(); i++)
       cout << "[[" << arguments3[i] << "]]" << endl;
    args_cleanup(arguments3, cl_tokens3.size());

    // 4: Unclosed quote (treats everything after starting quote as one arg)
    string cl4 = "echo \"unclosed quote string";
    vector<string> cl_tokens4 = tokenize(cl4, delims);
    cout << "\nTEST 4: Unclosed quote strings" << endl;
    cout << "Input: \"" << cl4 << "\"" << endl;
    for (size_t i = 0; i < cl_tokens4.size(); i++)
        cout << "[" << cl_tokens4[i] << "]" << endl;

    // Build args 4
    cout << "Build args 4" << endl;
    char **arguments4 = build_args(cl_tokens4);
    for (size_t i = 0; i < cl_tokens4.size(); i++)
       cout << "[[" << arguments4[i] << "]]" << endl;
    args_cleanup(arguments4, cl_tokens4.size());

    // 5: Background test with &
    string cl5 = "./delay &";
    vector<string> cl_tokens5 = tokenize(cl5, delims);
    background(cl_tokens5); // Remove & for building args
    cout << "\nTEST 5: Background execution with &" << endl;
    cout << "Input: \"" << cl5 << "\"" << endl;
    for (size_t i = 0; i < cl_tokens5.size(); i++)
        cout << "[" << cl_tokens5[i] << "]" << endl;

    // Build args 5
    cout << "Build args 5:" << endl;
    char **arguments5 = build_args(cl_tokens5);
    for (size_t i = 0; i < cl_tokens5.size(); i++)
       cout << "[[" << arguments5[i] << "]] ";
    cout << endl;

    time_t start_bg = time(nullptr);
    
    pid_t pid_bg = fork();
    if (pid_bg == 0) {
        execvp(arguments5[0], arguments5);
        perror("execvp failed");
        _exit(1);
    } else if (pid_bg > 0) {
        sleep(5);  // Wait for child to finish
        time_t end_bg = time(nullptr);
        cout << "Parent resumed after " << (end_bg - start_bg) << " seconds" << endl;

        int zombies = check_for_zombies();
        cout << "Zombies reaped: " << zombies << endl;
        
        waitpid(pid_bg, nullptr, 0);
    }

    args_cleanup(arguments5, cl_tokens5.size());

    // 7: History feature tests
    CircularBuffer add_hist;
    add_hist.add_cmd("whoami");
    add_hist.add_cmd("ls");
    add_hist.add_cmd("pwd");

    cout << "\nTEST 7.1: History !!" << endl;
    string last = add_hist.back();
    cout << "!! recalls: \"" << last << "\" " << endl;

    cout << "\nTEST 7.2: History !N" << endl;
    cout << "!1 \"" << add_hist.get_cmd(1) << "\"" << endl;
    cout << "!3 \"" << add_hist.get_cmd(3) << "\"" << endl; //cmd1 was only read, not reexecuted

    cout << "\nTEST 7.3: History !N error handling" << endl;
    cout << "!11 " << add_hist.get_cmd(11) << (add_hist.get_cmd(11).empty() ? " is not in buffer" : " still exists in buffer [Error]") << endl;

    return 0;
}