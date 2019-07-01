#define PSH_RL_BUFSIZE 1024
#define PSH_TOK_BUFSIZE 64
#define PSH_TOK_DELIM " \t\r\n\a"

char *psh_read_line(void);
char **psh_split_line(char *line);
int psh_exec(char **args);