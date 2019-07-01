int psh_builtins_size();

int (*get_builtin(char *name)) (char **);

int psh_cd(char **args);
int psh_exit(char **args);
int psh_help(char **args);
int psh_pwd(char **args);
