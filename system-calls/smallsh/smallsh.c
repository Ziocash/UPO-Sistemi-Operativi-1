#include "smallsh.h"

pid_t foreground_pid = 0;

void waitbg()
{
    int returnvalue = 0;
    pid_t pid;

    while((pid = waitpid(-1, &returnvalue, WNOHANG)) > 0)
    {
        printf("PID %d terminated with exit value %d\n", pid, WEXITSTATUS(returnvalue));
        bpid(pid, REMOVE);
    }
}

void procline(void) /* tratta una riga di input */
{

    char *arg[MAXARG + 1];              // array di puntatori per runcommand
    int toktype;                        // tipo del simbolo nel comando
    int narg;                           // numero di argomenti considerati finora
    exemode functype = FOREGROUND;          // tipo di funzione da eseguire (FOREGROUND o BACKGROUND)

    narg = 0;

    do
    {

        /* mette un simbolo in arg[narg] 
       ed esegue un'azione a seconda del tipo di simbolo */

        switch (toktype = gettok(&arg[narg]))
        {
            case ARG:

                /* se argomento: passa al prossimo simbolo */

                if (narg < MAXARG)
                    narg++;
                break;

            case AMPERSAND:
                /*se background: imposto il functype su BACKGROUND*/
                functype = BACKGROUND;
                break;
            
            case SEMICOLON:
            case EOL:
                if (narg != 0)
                {
                    arg[narg] = NULL;
                    runcommand(arg, functype);
                }

                if(toktype == EOL)
                    waitbg();

                /* se non fine riga (descrizione comando finisce con ';')
                    bisogna ricominciare a riempire arg dall'indice 0 */

                narg = 0;
                break;
        }
    }
    while (toktype != EOL); /* fine riga, procline finita */
}

void runcommand(char **cline, exemode mode) /* esegue un comando */
{
    /*bpid command execution*/
    if (strcmp(*cline, "bp") == 0)
    {
        printf("\nBPID: %s\n", getenv("BPID"));
        return;
    }

    /*close command*/
    if(strcmp(*cline, "exit") == 0)
    {
        waitbg();
        exit(0);
    }

    pid_t pid;
    int exitstat, ret;

    pid = fork();
    if (pid == (pid_t)-1)
    {
        perror("smallsh: fork failed");
        return;
    }

    if (pid == (pid_t)0)
    { /* processo figlio */

        /* esegue il comando il cui nome e' il primo elemento di cline,
       passando cline come vettore di argomenti */

        execvp(*cline, cline);
        perror(*cline);
        exit(1);
    }

    /* non serve "else"... ma bisogna aver capito perche' :-)  */

    if(mode == FOREGROUND)
    {
        signal(SIGINT, sighandler);
        foreground_pid = pid;
        ret = waitpid(pid, &exitstat, 0);
    }
    else
    {
        signal(SIGINT, SIG_DFL);
        bpid(pid, ADD);
        printf("Background process. PID: %d\n", pid);
    }

    if (ret == -1)
        perror("wait");
}

/*SIGINT handler*/
void sighandler(int sig)
{
    if(sig == SIGINT)
    {
        kill(foreground_pid, SIGINT);
        foreground_pid = 0;
    }
    signal(SIGINT, SIG_DFL);
}

void bpid(pid_t pid, bpidmode mode)
{
    char *BPID = getenv("BPID");

    char *stringpid = calloc(sizeof(pid) + 1, sizeof(char));
    if (stringpid == NULL)
    {
        perror("Error during memory allocation: ");
        return;
    }

    if (mode == ADD)
    {
        char *temp = calloc(sizeof(BPID) + sizeof(pid) + 1, sizeof(char));
        if (temp == NULL)
        {
            perror("Error during memory allocation: ");
            free(stringpid);
            return;
        }
        strcpy(temp, BPID);
        sprintf(stringpid, "%d", pid);
        strcat(temp, ":");
        strcat(temp, stringpid);
        if (temp[0] == ':')
            memmove(temp, temp + 1, strlen(temp));
        setenv("BPID", temp, 1);
        free(temp);
    }
    else if (mode == REMOVE)
    {
        char *temp = strtok(BPID, ":");
        sprintf(stringpid, "%d", pid);
        if (strchr(BPID, ':') == NULL)
            BPID = "";

        char *remres = (char *)calloc(sizeof(BPID) + 1, sizeof(char));

        while (temp != NULL)
        {
            if (strcmp(temp, stringpid) != 0)
            {
                strcat(remres, ":");
                strcat(remres, temp);
            }
            temp = strtok(NULL, ":");
        }
        if (remres[0] == ':')
            memmove(remres, remres + 1, strlen(remres));
        setenv("BPID", remres, 1);
        free(temp);
        free(remres);
    }
    else
    {
        errno = EINVAL;
        perror("Incorrect mode: ");
    }
    free(stringpid);
}

int main()
{
    int bpidres = setenv("BPID", "", 1);
    if (bpidres != 0)
        perror("BPID environment variable creation failed: ");

    char *prompt = NULL;
    char *home = getenv("HOME");
    char *user = getenv("USER");
    size_t dim = 0;
    dim = strlen(home) + strlen(user) + 5;
    prompt = (char *)calloc(dim, sizeof(char));
    if(prompt == NULL)
    {
        perror("Allocation error: ");
        exit(EXIT_FAILURE);
    }
    sprintf(prompt, "%%%s:%s:", user, home);

    while (userin(prompt) != EOF)
        procline();

    unsetenv("BPID");
    free(prompt);
    return EXIT_SUCCESS;
}