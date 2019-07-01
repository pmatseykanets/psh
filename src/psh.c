#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "psh.h"
#include "builtin.h"

char *psh_read_line(void)
{
    int bufsize = PSH_RL_BUFSIZE;
    char *buffer = malloc(sizeof(char) * bufsize);
    if (!buffer)
    {
        fprintf(stderr, "psh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    int pos = 0;
    int c;

    while (1)
    {
        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[pos] = '\0';
            return buffer;
        }
        buffer[pos] = c;
        pos++;

        if (pos >= bufsize)
        {
            bufsize += PSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer)
            {
                fprintf(stderr, "psh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

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
        printf("> ");

        line = psh_read_line();
        // puts(line);
        free(line);
        args = psh_split_line(line);
        status = psh_run(args);

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
