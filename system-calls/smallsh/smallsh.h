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

#define EOL 1       /* end of line */
#define ARG 2       /* argomento normale */
#define AMPERSAND 3 /* & */
#define SEMICOLON 4 /* ; */

#define MAXARG 512 /* numero massimo di argomenti */
#define MAXBUF 512 /* lunghezza massima riga di input */

/*bpid mode definitions*/
typedef enum bpidmodes_s
{
    ADD,
    REMOVE
}bpidmodes_s;

/*bpid mode type definition*/
typedef bpidmodes_s bpidmode;

/*execution mode definitions*/
typedef enum exemodes_s
{
    FOREGROUND,
    BACKGROUND
}exemodes_s;

/*execution mode type definition*/
typedef exemodes_s exemode;

/* verifica se c non è un carattere speciale */
int inarg(char c); 

/* stampa il prompt e legge una riga */
int userin(char *p); 

/* legge un simbolo */
int gettok(char **outptr); 

/* tratta una riga di input */
void procline(); 

/* esegue un comando */
void runcommand(char **cline, exemode mode); 

/*add or remove pid from BPID environment variable*/
void bpid(pid_t pid, bpidmode mode); 

/*waits for background tasks*/
void waitbg();

/**/
void sighandler(int sig); 