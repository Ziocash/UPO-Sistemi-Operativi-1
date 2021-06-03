# ITALIAN

**Utilizzando le system call POSIX per la gestione di semafori (sem_init, sem_wait e sem_post) e
le funzioni per la gestione dei processi, risolvere il seguente problema:**
Una tribù di N selvaggi mangia in comune da una pentola che può contenere fino ad M porzioni di
stufato, si assume che inizialmente la pentola sia piena. Quando un selvaggio ha fame controlla la
pentola:

- se non ci sono porzioni, sveglia il cuoco ed attende che questo abbia completamente riempito di nuovo la pentola prima di servirsi;
- se la pentola contiene almeno una porzione, se ne appropria.
Il cuoco controlla che ci siano delle porzioni e, se ci sono, si addormenta, altrimenti cuoce M porzioni
e le mette nella pentola. Ciascun selvaggio deve mangiare NGIRI volte prima di terminare.

Il numero di selvaggi N, di porzioni M e NGIRI dovranno essere richiesti come argomenti da linea di comando per facilitare esperimenti al variare di tali parametri. I selvaggi ed il cuoco devono essere implementati come processi separati che lavorano su variabili e semafori condivisi. Definire anche un parametro che permetta di contare complessivamente quante volte il cuoco riempie la pentola prima del termine del programma. Il programma termina quando tutti i selvaggi hanno completato il loro ciclo. Durante l'esecuzione stampare opportuni messaggi al fine di determinare il comportamento dei vari processi.
