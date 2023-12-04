# TP-Shell
Réaliser un micro shell, affichant les codes de sortie et les temps d’exécution des programmes lancés

## 1. Affichage d’un message d’accueil, suivi d’un prompt simple: 
Pour afficher le message d'accueil et les message prompt nous utilisons la fonction **write**. 
Expliquons ce que fait cette fonction en détails : 
- **Write** prend 3 arguments : un entier 0 ou 1 si le message/chaîne de caractères est un input 0 ou output 1 (dans notre cas : 1), une chaîne de caractère qui contient le message à afficher et la taille du message qui peut être récupéré en appelant la fonction strlen.

Lorsque nous compilons le fichier c qui contient ce programme nous nommons son exécutable enseash à l'aide de la commande gcc -o enseash Q1.c. Nous avons réussi à créer un microshell qui affiche un message prompt et un message d'accueil.

## 2. Exécution de la commande saisie et retour au prompt (REPL : read–eval–print loop) :
### a. Lecture de la commande saisie : 
Nous voulons importer dans notre microshell les commandes déjà définies pour notre terminal, tels que **fortune** et **date**. 
Pour cela, nous créeons à l'intérieur d'une boucle while infinie un **processus fils** à l'aide de la fonction **fork** qui retourne 0 si nous sommes dans le processus enfant et 1 si nous revenons au **processus père**. Cette méthode sert à créer notre propre programme dans notre nouveau microshell enseash. 

Expliquons comment nous faisons cela :
- Dans une boucle while(1), nous stockons tout d'abord la commande écrite par l'utilsateur après avoir lancé enseash (cf. Q1) et la lisons à l'aide de la fonction **read** (mêmes arguments que write).
- Nous appelons ensuite la fonction fork et stockons son résultat dans une variable de type pid_t appelée pid.
- Si nous sommes dans le processus fils (pid==0 ), nous utilisons la fonction execlp qui permet de lire la commande entrée par l'utilisateur et l'identifier avec une commande existante dans le terminal et l'appliquer.
- Si nous sommes dans le processus père, nous attendons que l'utilsateur entre une commande .

### b. Exécution d’une commande simple (sans argument) :
Pour l'instant, nous sommes arrivées à exécuter des commandes dans notre microshell tels que fortune et date. Ces commandes ne prennent aucun argument.

### c. Retour au prompt enseash % et attente de la commande suivante :
Dans la même boucle, nous faisons appel à une fonction définie dans le document functions.h (Cf. dossier Q2). Cette fonction a pour but d'écrire le message prompt en utilisant write, elle est appelée avant de stocker le processus fils et une fois que nous soyons sortie du processus fils et dans l'attente d'une nouvelle commande.
 
## 3. Gestion de la sortie du shell avec la commande “exit” ou un <ctrl>+d : 
Pour sortir du microshell il faut casser la boucle avec la fonction **exit(EXIT_SUCESS)** dans notre programme.
Nous créeons comme pour le message prompt, une fonction qui affiche un message de bye bye que nous allons appeler dans la suite.

- **Sortir du microshell avec la commande exit :**
Si la commande entrée par l'utilisateur est exit, nous sortons du microshell. Cela se fait grâce à la fonction exceclp qui compare la commande entrée avec exit qui existe déjà dans le terminal. Si nous sommes dans ce cas la fonction du message bye bye est appelé puis nous cassons la boucle avec exit(EXIT_SUCESS).

- **Sortir du microshell avec ctrl+d :**
Pour savoir si l'utilisateur a utilisé ces deux touches, nous stockons dans une variable "bytesRead" la sortie de l'appel de la fonction read(0,command,49) (command est la chaîne de caractères qui stocke la commande entrée par l'utilisateur). Si bytesRead est égale à 0 alors l'utilisateur a bien cliqué sur <ctrl>+D. Dans ce cas là, nous appelons la fonction qui affiche le message bye bye puis nous sortons de la boucle while. 

## 4. Affichage du code de retour (ou du signal) de la commande précédente dans le prompt :
Nous voulons afficher à chaque message prompt l'état du programme compilé avant. 

- Si le processus child du dernier programme compilé s'achève normalement, nous affichons dans le message prompt **enseash [exit:0]**.
- Si le processus child du dernier programme est arrêté par un signal, nous affichichons dans le message prompt **enseash [sign:n]** avec n le nombre du signal utilisé pour arréter le processus child du dernier programme.
Nous utilisons la fonction **WIFEXITED** pour savoir de quelle manière est teminé le processus fils.

## 5. Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime :
En plus du retour exit ou signal sur le dernier programme compilé, nous souhaitons ajouter le temps d'éxecution. 
Pour cela, nous utilisons la fonction **clock_gettime** et l'appelons dans la fonction qui affiche le message prompt. 

Expliquons en details ce que fait les differentes fonctions de **clock_gettime** :
- clock_gettime(CLOCK_REALTIME, &start) permet d'obtenir le temps actuelle du système et le stocke dans la structure start
- clock_gettime(CLOCK_REALTIME, &end) utilise à nouveau clock_gettime pour obtenir le temps actuel et le stocke dans la structure end après la fin de l'exécution du processus fils.
- waitpid(pid, &status, 0) attend que le processus fils identifié par le PID (pid) se termine. Pendant cette attente, le processus parent est bloqué. La fonction waitpid stocke le statut de sortie du processus fils dans la variable status.
- On calcule ensuite la différence de temps, elle est calculée en millisecondes à l'aide des champs tv_sec (secondes) et tv_nsec (nanosecondes) de la structure start et end. Cette différence est stockée dans la variable time_diff.
On implémente ensuite ce temps d'execution dans le prompt_message afin de l'afficher sur notre shell.

## 6. Exécution d’une commande complexe (avec arguments) :
Dans cette partie, nous cherchons à executer une commande avec arguments dans le shell.

- Pour cela nous allons créer une fonction token qui permet d'extraire le premier jeton (token) de la chaîne command. **strtok** est utilisée pour diviser une chaîne en jetons en fonction d'un délimiteur spécifié. Dans ce cas, le délimiteur est un espace (" ").
- On crée un tableau de pointeurs de caractères (args) pour stocker les tokens. MAX_SIZE est le nombre maximum de tokens que nous pouvons gérer.
- La variable arg_count est utilisée pour suivre le nombre de tokens.
- Nous executons ensuite une boucle qui continue jusqu'à ce qu'il n'y ait plus de tokens. La condition vérifie si le jeton actuel n'est pas NULL.

Cette fonction sera implémenté dans la boucle if.

- On remplace la fonction execlp(command, command, NULL) par execvp(args[0], args) qui execute la commande avec arguments.

## 7. Gestion des redirections vers stdin et stdout avec ‘<’ et ‘>’:
Dans cette partie, l'objectif est de gérer la redirection des entrées (<) et des sorties (>) pour les commandes saisies dans le microshell 
On inclut d'abord <fcntl.h>
On définit deux fonctions pour l'input et l'output.
Pour l'input, on effectue les étapes suivantes:
- Trouver le symbole **<** :
       strchr recherche la première occurrence du caractère < dans la chaîne command. S'il est trouvé, il renvoie un pointeur vers cette position.

- Ajuster la chaîne de commande :
        *input_redirect = '\0'; : Termine la chaîne de commande à la position de <.
        input_redirect++; : Déplace le pointeur vers le nom de fichier après <.

- Ignorer les espaces après < :
        while (*input_redirect == ' ') { input_redirect++; } : Ignore les espaces éventuels après <. Cela garantit qu'il n'y a pas d'espaces supplémentaires entre < et le nom de fichier.

- Ouvrir le fichier en lecture :
        int fd = open(input_redirect, O_RDONLY); : Ouvre le fichier spécifié après < en mode lecture seule (O_RDONLY).
        Vérifie si l'ouverture du fichier a réussi, et sinon, affiche un message d'erreur et quitte.

- Rediriger stdin vers le fichier :
        dup2(fd, 0); : Redirige le descripteur de fichier fd (le fichier ouvert) vers l'entrée standard (descripteur de fichier 0).
        close(fd); : Ferme le descripteur de fichier car il n'est plus nécessaire.
  
On fait de même pour l'output mais avec stdout.



