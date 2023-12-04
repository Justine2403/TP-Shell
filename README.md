# TP-Shell
Réaliser un micro shell, affichant les codes de sortie et les temps d’exécution des programmes lancés

## 1. Affichage d’un message d’accueil, suivi d’un prompt simple: 
Pour afficher le message d'accueil et les message prompt nous utilisons la fonction write. 
Expliquons ce que fait cette fonction en détails : 
- Write prend 3 arguments : un entier 0 ou 1 pour iniquer si le message/chaîne de caractères est un input 0 ou output 0 (dans notre cas : 1), une chaîne de caractère qui contient le message à afficher et la taille du message qui peut être récupéré en appelant la fonction strlen.

Lorsque nous compilons le fichier c qui contient ce programme nous nommons son exécutable enseash à l'aide de la commande gcc -o enseash Q1.c. Nous avons réussi à créer un microshell qui affiche un message prompt et un message d'accueil.

## 2. Exécution de la commande saisie et retour au prompt (REPL : read–eval–print loop) :
### a. Lecture de la commande saisie : 
Nous voulons importer dans notre microshell les commandes déjà définies pour notre terminal, tels que "fortune" et "date". 
Pour cela, nous créeons à l'intérieur d'une boucle while infinie un processus fils à l'aide de la fonction fork qui retourne 0 si nous sommes dans le processus enfant et 1 si nous revenons au processus père. Cette méthode sert à créer notre propre programme dans notre nouveau microshell enseash. 

Expliquons comment nous faisons cela :
- Dans une boucle while(1), nous stockons tout d'abord la commande écrite par l'utilsateur après avoir lancé enseash (cf. Q1) et la lisant à l'aide de la fonction read (mêmes arguments que write).
- Nous appelons ensuite la fonction fork et stockons son résultat dans une variable de type pid_t appelée pid.
- Si nous sommes dans le processus fils (pid==0 ), nous utilisons la fonction execlp qui permet de lire la commande entrée par l'utilisateur et l'identifier avec une commande existante dans le terminal et l'appliquer.
- Si nous sommes dans le processus père, nous attendons que l'utilsateur entre une commande .

### b. Exécution d’une commande simple (sans argument) :
Pour l'instant, nous sommes arrivées à exécuter des commandes dans notre microshell tels que fortune et date. Ces commandes ne prennent aucun argument.

### c. Retour au prompt enseash % et attente de la commande suivante :
Dans la même boucle, nous faisons appel à une fonction définie dans le document functions.h (Cf. dossier Q2). Cette fonction a pour but d'écrire le message prompt en utilisant write, elle est appelée avant de stocker le processus fils et une fois que nous soyons sortie du processus fils et dans l'attente d'une nouvelle commande.
 
## 3. Gestion de la sortie du shell avec la commande “exit” ou un <ctrl>+d : 

