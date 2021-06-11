#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

//Print integer
#define log_num(x) printf("%d\n", x)
//Print string
#define log_str(x) printf("%s\n", x)
//Print character
#define log_char(x) printf("%c\n", x)

//Colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

//Background mode
#define BACKGROUND 99
//Foreground mode
#define FOREGROUND 0

//BPID remove pid mode
#define REMOVE 0
//BPID add pid mode
#define ADD 1

#define EOL 1       /* end of line */
#define ARG 2       /* argomento normale */
#define AMPERSAND 3 /* & */
#define SEMICOLON 4 /* ; */

#define MAXARG 512 /* numero massimo di argomenti */
#define MAXBUF 512 /* lunghezza massima riga di input */

int inarg(char c); /* verifica se c non è un carattere speciale */

int userin(char *p); /* stampa il prompt e legge una riga */

int gettok(char **outptr); /* legge un simbolo */

void procline(); /* tratta una riga di input */

void runcommand(char **cline); /* esegue un comando */

void bpid(pid_t pid, int mode); /*add or remove pid from BPID environment variable*/

void promptget(char *prompt);