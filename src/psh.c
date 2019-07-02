#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include "psh.h"
#include "builtin.h"

char **psh_split_line(char *line)
{
    int bufsize = PSH_TOK_BUFSIZE;
    int pos = 0;

    char **tokens = malloc(bufsize * sizeof(char *));
    if (!tokens)
    {
        fprintf(stderr, "psh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, PSH_TOK_DELIM);
    while (token)
    {
        tokens[pos] = token;
        pos++;

        if (pos > bufsize)
        {
            bufsize += PSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "psh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, PSH_TOK_DELIM);
    }
    tokens[pos] = NULL;

    return tokens;
}

int psh_exec(char **args)
{
    pid_t pid, wpid;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("psh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("psh");
    }
    else
    {
        // Parent process
        int status;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int psh_run(char **args)
{
    // An empty command; nothing to do
    if (args[0] == NULL)
    {
        return 1;
    }

    // Check for built-ins
    int (*fptr)(char **) = get_builtin(args[0]);
    if (fptr)
    {
        // printf("found builtin: %s\n", args[0]);
        return fptr(args);
    }

    // Run an external command
    // printf("executing external process: %s\n", args[0]);
    return psh_exec(args);
}

void psh_loop(void)
{
    char *line;
    int status = 1;
    char **args;

    do
    {
        line = readline("> ");
        args = psh_split_line(line);
        status = psh_run(args);
        
        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv)
{
    // Parse flags

    // Run command loop
    psh_loop();

    return EXIT_SUCCESS;
}
