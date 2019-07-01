#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

char *builtin_name[] = {
    "cd",
    "exit",
    "help",
    "pwd"};

int (*builtin_func[])(char **) = {
    &psh_cd,
    &psh_exit,
    &psh_help,
    &psh_pwd};

int psh_builtins_size()
{
    return sizeof(builtin_name) / sizeof(char *);
}

int (*get_builtin(char *name))(char **)
{
    for (int i = 0; i < psh_builtins_size(); i++)
    {
        if (strcmp(name, builtin_name[i]) == 0)
        {
            return builtin_func[i];
        }
    }

    return NULL;
}

int psh_cd(char **args)
{
    char *path = args[1];

    if (args[1] == NULL || strcmp(path, "~") == 0)
    {
        path = getenv("HOME");
    }

    if (chdir(path) != 0)
    {
        perror("psh");
    }

    return 1;
}

int psh_exit(char **args)
{
    return 0;
}

int psh_help(char **args)
{
    printf("psh\n");
    printf("Peter's shell\n");
    printf("\n");
    printf("Commands:\n");

    for (int i = 0; i < psh_builtins_size(); i++)
    {
        printf("  %s\n", builtin_name[i]);
    }

    return 1;
}

int psh_pwd(char **args)
{
    char cwd[PATH_MAX];
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("psh");
        return 1;
    }

    printf("%s\n", cwd);
    return 1;
}
