#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAXLINE 120

void handle_quotes(char *line) {
    char *src = line;
    char *dst = line;
    bool in_quotes = false;

    while (*src != '\0') {
        if (*src == '"') {
            in_quotes = !in_quotes;
            src++;
        } else if (*src == ' ' && in_quotes) {
            *dst = '\t';
            dst++;
            src++;
        } else {
            *dst = *src;
            dst++;
            src++;
        }
    }
    *dst = '\0';
}

int parse_input(char *line, char *args[], int *background) {
    int argc = 0;
    *background = 0;

    handle_quotes(line);

    char *token = strtok(line, " ");

    while (token != NULL && argc < MAXLINE/2) {
        if (strcmp(token, "&") == 0) {
            *background = 1;
        } else {
            for (int i = 0; i < strlen(token); i++) {
                if (token[i] == '\t') {
                    token[i] = ' ';
                }
            }
            args[argc] = token;
            argc++;
        }

        token = strtok(NULL, " ");
    }

    args[argc] = NULL;
    return argc;
}


void debug_args(char *args[], int argc) {
    printf("Argument Count: %d\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("args[%d] = '%s'\n", i, args[i]);
    }
    printf("args[%d] = NULL \n", argc);
}

int main() {
    char *args[MAXLINE/2 +1];
    char line[MAXLINE];
    int should_run = 1;

    while (should_run) {
        int background = 0;
        int argc = 0;

        printf("osclass> ");
        fflush(stdout);

        if (fgets(line, MAXLINE, stdin) == NULL) {
            break;
        }

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (line[0] == '\0') {
            continue;
        }

        argc = parse_input(line, args, &background);

        if (argc == 0) {
            continue;
        }

        debug_args(args, argc);

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            _exit(1);
        } else {
            if (!background) {
                int status;
                if (waitpid(pid, &status, 0) < 0) {
                    perror("waitpid");
                }
            } else {
                printf("[background pid %d]\n", pid);
            }
        }
    }

    return 0;
}