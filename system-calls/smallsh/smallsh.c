#include "smallsh.h"

void procline(void) /* tratta una riga di input */
{

    char *arg[MAXARG + 1];              // array di puntatori per runcommand
    int toktype;                        // tipo del simbolo nel comando
    int narg;                           // numero di argomenti considerati finora
    int functype = FOREGROUND;          // tipo di funzione da eseguire (FOREGROUND o BACKGROUND)

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

        case EOL:
        case SEMICOLON:

            /* se fine riga o ';' esegue il comando ora contenuto in arg, mettendo NULL per indicare la fine degli argomenti: serve a execvp */

            if (narg != 0)
            {
                arg[narg] = NULL;
                runcommand(arg);
            }

            /* se non fine riga (descrizione comando finisce con ';')
                bisogna ricominciare a riempire arg dall'indice 0 */

            if (toktype != EOL)
                narg = 0;

            break;
        }
    }

    while (toktype != EOL); /* fine riga, procline finita */
}

void runcommand(char **cline) /* esegue un comando */
{
    pid_t pid;
    int exitstat, ret;

    pid = fork();
    if (pid == (pid_t)-1)
    {
        perror("smallsh: fork fallita");
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

    /* qui aspetta sempre e comunque - i comandi in background 
     richiederebbero un trattamento diverso */

    ret = wait(&exitstat);

    if (ret == -1)
        perror("wait");
}

void bpid(pid_t pid, bpidmode mode)
{
    char *BPID = getenv("BPID");

    char *stringpid = calloc(sizeof(pid) + 1, sizeof(char));
    if (stringpid == NULL)
    {
        perror("Error during memory allocation");
        return;
    }

    if (mode == ADD)
    {
        char *temp = calloc(sizeof(BPID) + sizeof(pid) + 1, sizeof(char));
        if (temp == NULL)
        {
            perror("Error during memory allocation");
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
        perror("Incorrect mode");
    }
    free(stringpid);
}

int main()
{
    int bpidres = setenv("BPID", "", 1);
    if (bpidres != 0)
        perror("Impossibile creare variabile d'ambiente BPID");

    char *prompt = NULL;
    char *home = getenv("HOME");
    char *user = getenv("USER");
    size_t dim = 0;
    dim = strlen(home) + strlen(user) + 5;
    prompt = (char *)calloc(dim, sizeof(char));
    if(prompt == NULL)
    {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }
    sprintf(prompt, "%%%s:%s:", user, home);

    while (userin(prompt) != EOF)
        procline();

    unsetenv("BPID");
    free(prompt);
    return EXIT_SUCCESS;
}

/*
bpid(15200, ADD);
log_str(getenv("BPID"));
bpid(15204, ADD);
bpid(15201, ADD);
bpid(15202, ADD);
log_str(getenv("BPID"));
bpid(15200, REMOVE);
bpid(15202, REMOVE);
log_str(getenv("BPID"));*/