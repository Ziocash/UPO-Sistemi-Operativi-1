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

/**
 * @brief BPID mode definitions.
 */
typedef enum bpidmodes_s
{
    ADD,
    REMOVE
}bpidmodes_s;

/**
 * @brief BPID mode type definition.
 */
typedef bpidmodes_s bpidmode;

/**
 * @brief Execution mode definitions.
 */
typedef enum exemodes_s
{
    FOREGROUND,
    BACKGROUND
}exemodes_s;

/**
 * @brief Execution mode type definition.
 */
typedef exemodes_s exemode;

/**
 * @brief Checks if c is not a special character.
 * 
 * @param c The character
 * @return Returns `1` if c is a special character, otherwise `0`
 */
int inarg(char c); 

/**
 * @brief Prints the prompt and reads a line.
 *
 * @param p The prompt.
 */
int userin(char *p); 

/**
 * @brief Reads a symbol.
 *
 * @param outptr Pointer to a string pointer.
 * 
 * @return Returns the token type
 */
int gettok(char **outptr); 

/**
 * @brief Processes an input line.
 */
void procline(); 

/**
 * @brief Runs a command with the execution mode setting (foreground or background).
 * 
 * @param cline Pointer to string pointer containing the command with arguments.
 * @param mode Execution mode (background or foreground).
 */
void runcommand(char **cline, exemode mode); 

/**
 * @brief BPID handling function.
 * 
 * @param pid Background PID that will be stored/removed from BPID environment variable.
 * @param mode BPID mode (add or remove).
 */
void bpid(pid_t pid, bpidmode mode); 

/**
 * @brief Waits for background tasks.
 * 
 */
void waitbg();

/**
 * @brief SIGINT signal handler, redirects signal onto foreground command.
 * Default action if background command.
 * 
 * @param sig Signal received by the shell.
 */
void sighandler(int sig); 