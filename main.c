#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

  /////////////////////////////////////////////////////////////  
 // Définition des constantes pour le nom des fichiers      //
/////////////////////////////////////////////////////////////
#define FILE_CONNEXION "connexion.bin"
#define FILE_ETUDIANT "etudiants.bin"
#define FILE_PRESENCE "presence.bin"
#define FILE_MESSAGE "messages.bin"

  /////////////////////////////////////////////////////////////  
 // Définition des constantes pour la taille des tableaux   //
/////////////////////////////////////////////////////////////
#define SIZE_ARRAY_CONNEXION 20
#define SIZE_ARRAY_STUDENT 30
#define SIZE_ARRAY_PRESENCE 50
#define SIZE_ARRAY_MESSAGE 200



  /////////////////////////////////////////////////////////////
 // Déclaration des structures ou enregistrement            //
/////////////////////////////////////////////////////////////

// Structure pour les dates
typedef struct{ int jour, mois, annee; } Date;

// Structure pour les heures
typedef struct{ int heure, minute, second; } Heure;

// Structure pour la connexion
typedef struct{
    int id;
    char login[10];
    char password[10];
    int status;
} Connexion;

// Structure pour l'étudiant
typedef struct{
    int id;
    char prenom[30];
    char nom[20];
    char classe[10];
    char code[10];
    int blocked;
} Etudiant;

// Structure pour les présences et absence
typedef struct{
    Etudiant etudiant;
    Date date;
    Heure heure;
    int status;
} Presence;

// Structure pour les messages
typedef struct{
    int id;
    Etudiant dest;
    char message[200];
    int status;
    Date date;
    Heure heure;
} Message;



  /////////////////////////////////////////////////////////////////
 // Déclaration des prototypes de fonction ou procédure         //
/////////////////////////////////////////////////////////////////

//---------------------------------------------//
int getAllConnexion(Connexion *connexions);
int getAllPresence(Presence *presences);
int getAllMessages(Message *messages);
int getAllStudent(Etudiant *etudiants);
int getListPresence(Presence *presences, Date date);
int getListClass(char classe[], Etudiant *etud);
int getMessageForStudent(Etudiant etud, Message *message);
Etudiant getStudent(int id);
//---------------------------------------------//
int addNewConnexion(Connexion connexion);
int addNewPresence(Presence presence);
int addNewMessages(Message message);
int addNewStudent(Etudiant etudiant);
//---------------------------------------------//
void generateStudent(void);
void generateConnexion(void);
//---------------------------------------------//
int dateIsValide(Date date);
int isMarkPresence(Etudiant etudiant);
int codeIsCorrect(char code[], Etudiant *etud);
int dateIsEqual(Date date1, Date date2);
//---------------------------------------------//
int getChoice(char msg[]);
Date getDate(void);
Connexion getLogin(void);
Connexion getConnexion(void);
void getPassWord(char *password);
void error(void);
void getMessageInput(char *message);
//---------------------------------------------//
void showListPresence(void);
void showListEtudiant(void);
void showListAbsence(void);
void showContinue(void);
char* formater(char *prenom, char *nom, char *classe, char *heure, int max);
//---------------------------------------------//
Date actualDate(void);
Heure actualHeure(void);
void desactiver(void);
void reactiver(void);
//---------------------------------------------//
int menuAdmin(void);
int menuUser(int nb);
int menuMessage(void);
void menuConnexion(void);
int menuGenerateFile(void);
//---------------------------------------------//
void generateFile(char fichier[], int mode, Date date);
//---------------------------------------------//
void markPresence(Etudiant etudiant, int status);
//---------------------------------------------//
void optionMarkPresence(Connexion connect);
void optionGestionStudent(void);
void optionSendMessage(void);
void optionGetMessage(Etudiant etud, Message *msg, int size);
void optionGenerateFile(void);
//---------------------------------------------//
Connexion createConnexion();
Etudiant createStudent();



   //////////////////////////////////////////////////////////////
  // Fonction pour la récupération de toutes les informations //
 // sur les fichiers                                         //
//////////////////////////////////////////////////////////////

// Fonction qui permet de récupérer toutes les logins et mot de passe
int getAllConnexion(Connexion *connexions){ 
    FILE *file = fopen(FILE_CONNEXION, "rb");
    
    if(file==NULL) {
        generateConnexion();
        file = fopen(FILE_CONNEXION, "rb");
    }
    
    int size = 0; // La taille du tableau
    Connexion connect; // Variable de récupération des données
    
    // On récupere les données dans un fichier
    while (fread(&connect, sizeof(Connexion), 1, file))
        connexions[size++] = connect; // on stocke les données dans le tableau
    
    fclose(file); // on ferme le fichier
    
    return size; // On retourne la taille du tableau 
}

// Fonction qui permet de récupérer toutes les présences
int getAllPresence(Presence *presences){ 
    FILE *file = fopen(FILE_PRESENCE, "rb");
    int size = 0; // La taille du tableau
    Presence presence; // variable de récupération de donnée
    
    // Si le fichier n'existe pas, on le crée
    if(file == NULL) file = fopen(FILE_PRESENCE, "wb");
    
    // On récupére les données dans le fichier
    while (fread(&presence, sizeof(Presence), 1, file))
        presences[size++] = presence; // on stocke les données dans le tableau
    
    fclose(file); // Fermeture du fichier
    
    return size; // On retourne la taille du tableau
}

// Fonction qui permet de récupérer tous les messages
int getAllMessages(Message *messages){ 
    FILE *file = fopen(FILE_MESSAGE, "rb");
    int size = 0; // La taille du tableau
    Message message; // Variable de récupération de donnée    
    
    // Si le fichier n'existe pas, on le crée
    if(file == NULL) file = fopen(FILE_PRESENCE, "wb");
    
    // On récupére les données dans le fichier
    while (fread(&message, sizeof(Message), 1, file))
        messages[size++] = message; // On stocke les données dans le tableau
    
    fclose(file); // Fermeture du fichier
    
    return size; // On retourne la taille du tableau
}

// Fonction qui permet de récupérer tous les étudiants
int getAllStudent(Etudiant *etudiants){
    FILE *file = fopen(FILE_ETUDIANT, "rb");
    // On générer les étudiants par défaut s'il n'exist pas
    if(file==NULL) {
        generateStudent();
        file = fopen(FILE_ETUDIANT, "rb");
    }
    
    int size = 0; // La taille du tableau
    Etudiant etudiant; // Variable de récupération de donnée
    
    // On récupére les données dans le fichier
    while (fread(&etudiant, sizeof(Etudiant), 1, file))
        etudiants[size++] = etudiant; // On stocke les données dans le tableau
    
    fclose(file); // Fermeture du fichier
    
    return size; // On retourne la taille du tableau
}

// Fonction qui permet de récupérer la liste des présences d'une date donner
int getListPresence(Presence *presences, Date date){
    Presence allPresence[SIZE_ARRAY_PRESENCE];
    
    // On récupére la liste de tout les présences
    int size1=0, size = getAllPresence(allPresence);
    
    // On filtre les présences correspondant à la date donner en paramètre
    for(int i=0; i<size; i++){
        if(dateIsEqual(date, allPresence[i].date)==0){
            presences[size1++] = allPresence[i];
        }
    }
    return size1; // On retourne la taille du tableau
}

// Fonction qui permet la récupération des étudiants d'une classe donner
int getListClass(char classe[], Etudiant *etud){
    Etudiant etudiant[SIZE_ARRAY_STUDENT];
    // On récupére tout les étudiants
    int size1 = 0, size = getAllStudent(etudiant);
    // On recherche les étudiants du même classe donner
    for(int i=0; i<size; i++){
        if(strcmp(etudiant[i].classe, classe) == 0){
            etud[size1++] = etudiant[i];
        }
    }
    
    return size1; // On retourne la taille du tableau
}

// Fonction qui permet de récupérer tout les messages d'un étudaint
int getMessageForStudent(Etudiant etud, Message *message){
    Message msgs[SIZE_ARRAY_MESSAGE];
    int size1 = 0, size = getAllMessages(msgs);
    for(int i=0; i<size;i++){
        if(msgs[i].dest.id == etud.id && msgs[i].status == 1)
            message[size1++] = msgs[i];
    }
    return size1;
}

// Fonction qui permet de récupérer un étudiant par rapport à son id
Etudiant getStudent(int id){
    Etudiant etuds[SIZE_ARRAY_STUDENT], e;
    int size = getAllStudent(etuds);
    for(int i=0; i<size; i++){
        if(etuds[i].id == id) return etuds[i];
    }
    return e;
}

  //////////////////////////////////////////////////////////////
 // Fonction pour l'ajout des informations dans les fichiers //
//////////////////////////////////////////////////////////////

// Fonction qui permet d'ajouter une connexion 
int addNewConnexion(Connexion connexion){
    FILE *file = fopen(FILE_CONNEXION, "ab");
    if(file == NULL) file = fopen(FILE_CONNEXION, "wb");
    
    // On ajoute les informations qui dans les fichiers
    int rst = fwrite(&connexion, sizeof(Connexion), 1, file);
    
    fclose(file); // On ferme le ficheir
    
    return rst; // On retourne le resultat
}

// Fonction pour ajouter les présences dans les fichiers 
int addNewPresence(Presence presence){
    FILE *file = fopen(FILE_PRESENCE, "ab");
    if(file == NULL) file = fopen(FILE_PRESENCE, "wb");
    
    // On ajoute les informations dans le fichier
    int rst = fwrite(&presence, sizeof(Presence), 1, file);
    
    fclose(file); // on ferme le fichier
    
    return rst; // On retourne le resultat
}

// Fonction pour ajouter les messages dans les fichiers 
int addNewMessages(Message message){
    FILE *file = fopen(FILE_MESSAGE, "ab");
    if(file == NULL) file = fopen(FILE_MESSAGE, "wb");
    
    // On ajoute les informations dans le fichier
    int rst = fwrite(&message, sizeof(Message), 1, file);
    
    fclose(file); // On ferme le fichier
    
    return rst; // On retourne le resultat
}

// Fonction pour ajouter les étudiants dans les fichiers
int addNewStudent(Etudiant etudiant){
    FILE *file = fopen(FILE_ETUDIANT, "ab");
    if(file == NULL) file = fopen(FILE_ETUDIANT, "wb");
    
    // On ajoute les informations dans le fichier
    int rst = fwrite(&etudiant, sizeof(Etudiant), 1, file);
    
    fclose(file); // On ferme le fichier
    
    return rst; // On retourne le resultat
}



  /////////////////////////////////////////////////////////////
 // Fonction pour la génération des information prédéfinie  //
/////////////////////////////////////////////////////////////

// Fonction pour la génération des étudiants par défaut
void generateStudent(){
    Etudiant STUDENT[12]={
    //  id  Prenom       Nom        Classe    Code      blocké
        {1,  "Fatou",   "Ndiaye",  "DEV WEB",  "202401", 0},
        {2,  "Mamadou", "Diop",    "DEV WEB",  "202402", 0},
        {3,  "Moussa",  "Yade",    "DEV WEB",  "202403", 0},
        {4,  "Fatima",  "Diaw",    "DEV DATA", "202404", 0},
        {5,  "Khady",   "Diop",    "DEV DATA", "202405", 0},
        {6,  "Lamine",  "Wade",    "DEV DATA", "202406", 0},
        {7,  "Awa",     "Ciss",    "DEV DATA", "202407", 0},
        {8,  "Papi",    "Gueye",   "REF DIG",  "202408", 0},
        {9,  "Basir",   "Diaw",    "REF DIG",  "202409", 0},
        {10, "Sokhna",  "Mbengue", "REF DIG",  "202410", 0},
        {11, "Diatou",  "Fall",    "REF DIG",  "202411", 0},
        {12, "Aicha",   "Diatta",  "REF DIG",  "202412", 0},
    };
    
    // On ajoute les étudiants par défaut
    for(int i = 0; i<12; i++){
        addNewStudent(STUDENT[i]);
    }
}

// Fonction pour la génération des connexions par défaut
void generateConnexion(){
    Connexion CONNEXION[14]={
       //id  login     password  status
        {0,  "admin",   "admin", 1},
        {1,  "fatou",   "1234",  0},
        {2,  "mamadou", "1234",  0},
        {3,  "moussa",  "1234",  0},
        {4,  "fatima",  "1234",  0},
        {5,  "khady",   "1234",  0},
        {6,  "lamine",  "1234",  0},
        {7,  "awa",     "1234",  0},
        {8,  "papi",    "1234",  0},
        {9,  "basir",   "1234",  0},
        {10, "sokhna",  "1234",  0},
        {11, "diatou",  "1234",  0},
        {12, "aicha",   "1234",  0},
        {27, "ehac",    "ehac6", 1},
    };
    
    // On ajoute les login ou connexion par défaut
    for(int i=0; i<14; i++){
        addNewConnexion(CONNEXION[i]);
    }
}


  /////////////////////////////////////////////////////////////
 // Fonction pour les vérifications des informations        //
/////////////////////////////////////////////////////////////

// Fonction qui vérifie si une date est valide
int dateIsValide(Date date){ 
    int jour = date.jour, mois = date.mois, annee = date.annee;
    
    if(mois<1 || mois>12 || annee<1 || jour < 1) return 0;
    if(mois == 2){
        if(annee%400==0 || (annee%4==0 && annee%100!=0)){
            return jour>29 ? 0 : 1;
        }else{
            return jour>28 ? 0 : 1;
        }
    }
    else if(mois==6 || mois==4 || mois==9 || mois==11)
        return jour>30 ? 0 : 1;
    else 
        return jour>31 ? 0 : 1;
}

// Fonction qui vérifie si un étudiant est marquer présent ou pas
int isMarkPresence(Etudiant etudiant){
    Presence presence[SIZE_ARRAY_PRESENCE];
    
    // on récupére la liste de tout les présences de la date actuel
    int size = getListPresence(presence, actualDate());
    
    // On boucle sur la liste des présences pour vérifier si l'étudiant est déja marquer présent
    for(int i = 0; i<size; i++){
        if(presence[i].etudiant.id == etudiant.id && presence[i].status == 1) 
            return 1; // Ss'il marquer on retourne 1 pour vrai
    }
    return 0; // Sinon on retourne 0 pour faut
}

// Fonction qui vérifie si un code est valide
int codeIsCorrect(char code[], Etudiant *etud){ 
    Etudiant etudiants[SIZE_ARRAY_STUDENT];
    
    // On récupére la liste de tout les étudiants
    int size = getAllStudent(etudiants);
    
    // On parcourt la liste des étudiants
    for(int i=0; i<size; i++){
        *etud = etudiants[i];
        // On retourne 1 pour vrai si le code correspond
        if(strcmp(code, etud->code) == 0) return 1; 
    }
    
    return 0; // On retourne 0 pour faut si le code ne correspond pas
}

// Fonction qui permet de comparer deux dates
int dateIsEqual(Date date1, Date date2){ 
    if(date1.annee>date2.annee) return 1;
    else if(date1.annee<date2.annee) return 2;
    else{
        if(date1.mois>date2.mois) return 1;
        else if(date1.mois<date2.mois) return 2;
        else{
            if(date1.jour>date2.jour) return 1;
            else if(date1.jour<date2.jour) return 2;
            else return 0;
        }
    }
}


  ////////////////////////////////////////////////////////////
 // Fonction de récupération des données par l'utilisateur //
////////////////////////////////////////////////////////////

// Fonction pour la récupération du choix par oui ou non de l'utilisateur 
int getChoice(char msg[]){
    int condition; char ch; 
    do{
        printf("%s ? (o)ui ou (n)on: ", msg);
        scanf("%s", &ch);
        condition = ch != 'n' && ch != 'N' && ch != 'o' && ch != 'O';
        if(condition){
            puts("\nErreur: votre choix n'est pas bonne. Réessayez svp...\n");
        }
    }while(condition);
    getchar();
    return (ch == 'n' || ch == 'N') ? 0: 1;
}

// Fonction pour la récupération d'une date par la saisie de l'utilisateur
Date getDate(){
    Date date; int i=0;
    do{
        if(i > 1){
            puts("La date n'est pas valide. Recommencez.\n");
        }i++;
        printf("Donner une date au format(jj/mm/aaaa): ");
        if(scanf("%d/%d/%d", &date.jour, &date.mois, &date.annee) != 3) error();
    }while(!dateIsValide(date));
    return date;
}

// Fonnction pour récupérer la saisi du login et du mot de passe de l'utilisateur
Connexion getLogin(){
    Connexion connect;
    char login[10], nm;
    int quit = 1, i=0;
    
    // On préremplir les champs de login par le caractère de fin d'une chaine de caractère \0
    memset(connect.login, '\0', sizeof(connect.login));
    
    while (quit){
        memset(login, '\00', sizeof(login));
        printf("Donner votre login: ");
        nm = getchar();
        while(nm != '\n'){
            if(nm != ' '){
                if(i<10){
                    login[i] = nm; i++;
                }else i = 0;
            }else{
                while(getchar() != '\n');
                i=0; break;
            }
            nm = getchar();
        }
        if(i>=4) quit = 0;
        else{
            puts("\nErreur: veuillez retapez votre login\n");
        } 
    }
    
    strcpy(connect.login, login);
    do{
        getPassWord(connect.password); // On récupére le mot de passe
    
    // On boucle tantque la longueur du mot de passe est inférieur à 4
    } while (strlen(connect.password) < 4);
    
    
    return connect; // On retourne la connexion
}

// Fonction qui  permet d'obtenir la connexion de l'utilisateur
Connexion getConnexion(){
    Connexion connect, connexion[SIZE_ARRAY_CONNEXION]; 
    int size = getAllConnexion(connexion);
    while (1){
        menuConnexion();// affichage du menu de connexion
        connect = getLogin();
        for(int i=0; i<size; i++){
            if(
                strcmp(connexion[i].login, connect.login) == 0 && strcmp(connexion[i].password, connect.password) == 0
            ) return connexion[i]; 
        }
        system("clear");
        puts("\n❌ Le login ou le mot de pass n'est pas correcte");
    }
}

// Fonction qui permet de récupérer le mot de passe saisie par l'utilisateur
void getPassWord(char *password){
    int i = 0, ch;
        
    desactiver();
    printf("Enter votre mot de passe: ");
    while ((ch = getchar()) != '\n') {
        if (ch == 127 || ch == 8) { // Touche de retour arrière (backspace)
            if (i > 0) {
                i--;
                password[i] = '\0';
                printf("\b \b"); // Efface le dernier caractère affiché
            }
        } else if (ch != '\n') {
            password[i++] = ch;
            printf("*");
            fflush(stdout);
        } else {
            break;
        }
    }
    password[i] = '\0';
    reactiver();
    puts("");
}

// Fonction pour les erreurs de saisie de l'utilisateur
void error(){
    while(getchar() != '\n');
}

// Fonction pour récupérer le message que l'utilisateur veut envoyer
void getMessageInput(char *message){
    char ch; int i;
    while (1){
        printf("Entrer votre message: \n|=> ");
        ch = getchar(); i = 0;
        while (ch != '\n'){
            message[i++] = ch;   
            ch = getchar();
        }
        if(i!=0) break;
    }
}



  ////////////////////////////////////////////////////////////
 // Fonction pour les affichages                           //
////////////////////////////////////////////////////////////
void showListPresence(){}
void showListEtudiant(){}
void showListAbsence(){}

// Fonction qui permet d'afficher un message
void showMessage(Message msg, int n){
    const int larg=46;
    char dest[larg];
    sprintf(dest, "* Desinataire: %s %s %s", msg.dest.prenom, msg.dest.nom, msg.dest.classe);
    int taille=strlen(dest);
    if(taille != larg-1){
        for(int i=taille; i<larg-1;i++){
            dest[i] = ' ';
        }
        dest[larg-1] = '*';
    }
    
    for(int i=0; i<larg;i++) printf("*");
    printf("\n*                 MESSAGE[%d]                 *\n", n);
    puts("*                 ==========                 *");
    printf("* Expéditeur: Administrateur                 *\n");
    printf("%s\n", dest);
    printf("* Date: %02d/%02d/%02d                           *\n",msg.date.jour,msg.date.mois,msg.date.annee);
    printf("* Heure: %02d:%02d:%02d                            *\n",msg.heure.heure,msg.heure.minute,msg.heure.second);
    puts("*                  CONTENUE                  *");
    puts("*                 +--------+                 *");
    
    // char message[]="Salut tout le monde j'espère que tout va bien aujourd'hui c'est le jour de la remise des diplômes. On vous attend avec impatience.";
    char message[200];
    memset(message, '\0', sizeof(message));
    strcpy(message, msg.message);
    int size = sizeof(message)/sizeof(message[0]);
    char text[50][30], nm;
    memset(text, '\0', sizeof(text));
    int nbM = 0, i, j,k;
    for(i=j=0; i<size;i++){
        nm = message[i];
        if(nm == ' '){
            nbM++;
            j=0;
        }else{
            text[nbM][j++] = nm;
        }
    }
    int t, nb = 0;

    printf("*");
    for(i=k=0; i<=nbM;i++){
        t = strlen(text[i])+1;
        k+=t;
        if(k<larg-3) printf(" %s", text[i]);
        else{
            if(k==larg-1){
                printf(" %s*\n*", text[i]); k=0;
            }else{
                k -= t;
                for(;k<larg-1; k++) printf(" ");
                printf("*\n* %s", text[i]);
                k=t+1;
            }
        }
    } 
    if(k<larg){ for(;k<larg-1;k++) printf(" "); }
    printf("*\n");
    for(int i=0; i<larg;i++) printf("*");
    puts("");
}


// Fonction pour le formatage de l'affichage des donner;
char* formater(char *prenom, char *nom, char *classe, char *heure, int max){
    char information[100];
    char *info = information;
    
    sprintf(information, "| %-*s | %-*s | %-*s | %-*s |", max, prenom, max, nom, max, classe, max, heure);
    
    return info;
}

// fonction qui affiche la message pour continuer
void showContinue(){
    char nm;
    puts("Appuyer sur la touche entrer pour continuer...");
    scanf("%c", &nm);
    system("clear");
}



  ////////////////////////////////////////////////////////////
 // Fonction pour les menu                                 //
////////////////////////////////////////////////////////////

// Fonction pour l'affichage du menu de l'administrateur
int menuAdmin(){ 
    int choix;
    do{
        system("clear");
        puts("\n");
        puts("\t-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
        puts("\t-+           MENU ADMINISTRATEUR           +-");
        puts("\t-+          =====================          +-");
        puts("\t-+      1) GESTION DES ÉTUDIANTS           +-");
        puts("\t-+      2) GÉNÉRATION DE FICHIERS          +-");
        puts("\t-+      3) MARQUER LES PRÉSENCES           +-");
        puts("\t-+      4) ENVOYER UN MESSAGE              +-");
        puts("\t-+      5) DÉCONNEXION                     +-");
        puts("\t-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
        printf("\n=> ");
        
        if (scanf("%d", &choix) != 1) while(getchar() != '\n');
    }while (choix>5||choix<1);
    
    return choix;
}

// Fonction pour l'affichage du menu de l'étudiant
int menuUser(int nb){ 
    int choix;
    do{
        system("clear");
        puts("\n");
        puts("\t-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
        puts("\t-+              MENU ÉTUDIANT              +-");
        puts("\t-+             ===============             +-");
        puts("\t-+      1) MARQUER MA PRÉSENCE             +-");
        puts("\t-+      2) NOMBRE DE MINUTES D’ABSENCE     +-");
      printf("\t-+      3) MES MESSAGES (%d)                +-\n", nb);
        puts("\t-+      4) DÉCONNEXION                     +-");
        puts("\t-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-");
        printf("\n=> ");
        
        if (scanf("%d", &choix) != 1) while(getchar() != '\n');
        
    }while (choix>4||choix<1);
    
    return choix;
} 

// Fonction pour l'affichage du menu de connexion
void menuConnexion(){
    puts("");
            puts("=============================================================");
            puts("=                    ===================                    =");
    puts("=====================\033[41m PAGE DE CONNEXION \033[0m=====================");
            puts("=                    ===================                    =");
            puts("=============================================================");
}

// Fonction pour l'affichage du menu des messages
int menuMessage(){
    int choix;
    do{  
        puts("");
        puts("*******************************************************");
        puts("** 1° Envoyer message a tout le monde                **");
        puts("** 2° Envoyer message a une classe                   **");
        puts("** 3° Envoyer message a un ou plusieurs étudiant(s)  **");
        puts("** 4° Retour au menu                                 **");
        puts("*******************************************************");
        printf("\n=> ");
    
        if(scanf("%d", &choix) != 1) error();
        system("clear");
    } while (choix>4||choix<1);
    
    return choix;
}

// Fonction pour l'affichage du menu générer fichier
int menuGenerateFile(){
    int choix;
    do{
        puts("\n1- Générer la liste des présences par date.");
        puts("2- Retour.");
        printf("|=> ");
        if(scanf("%d", &choix) != 1) error();
    } while (choix>2||choix<1);
    
    return choix;
}


  /////////////////////////////////////////////////////////////////
 // Fonction pour récupérer les informations système            //
/////////////////////////////////////////////////////////////////

// Fonction pour la récupération de la date actuel
Date actualDate(){
    Date date;
    time_t dt = time(0); 
    struct tm *dateActuel = localtime(&dt); 
    
    date.jour = dateActuel->tm_mday;
    date.mois = dateActuel->tm_mon + 1;
    date.annee = dateActuel->tm_year + 1900;
    
    return date;
}

// Fonction pour récupération de l'heure actuel
Heure actualHeure(){
    Heure heure;
    time_t temps= time(0); 
    
    struct tm *tempsActual = localtime(&temps);
    heure.heure = tempsActual->tm_hour;
    heure.minute = tempsActual->tm_min;
    heure.second = tempsActual->tm_sec;
    return heure;
}

// Le corps de la fonction qui permet de masquer la saisie de l'utilisateur
void desactiver(void) {
    struct termios old_attr, new_attr;
     
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
}

// Le corps de la fonction qui permet de afficher la saisie d'un utilisateur
void reactiver(void) {
    struct termios old_attr, new_attr; 
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
}



  ////////////////////////////////////////////////////////////
 // Fonction pour la génération de fichier                 //
////////////////////////////////////////////////////////////

// Fonction qui de générer un fichier des présences d'une date donner
void generateFile(char fichier[], int mode, Date date){
    Presence presences[SIZE_ARRAY_PRESENCE], presence;
    Etudiant etud; char heure[9];
    int size = getListPresence(presences, date);
    
    if(size == 0){
        printf("\n⚠️ Il n'y a pas de présence pour cette: %02d/%02d/%02d.\n\n", date.jour, date.mois, date.annee);
    }else{
        int i, maxL[4]={0}, max = 0;
        maxL[3] = 8; 
        
        // On détermine pour chaque colonne à remplir sa taille maximal 
        for (i = 0; i < size; i++) {
            etud = presences[i].etudiant;
            maxL[0] = (strlen(etud.prenom) > maxL[0]) ? strlen(etud.prenom) : maxL[0];
            maxL[1] = (strlen(etud.nom) > maxL[1]) ? strlen(etud.nom) : maxL[1];
            maxL[2] = (strlen(etud.classe) > maxL[2]) ? strlen(etud.classe) : maxL[2];
        }
        
        // On récupére la plus longue chaine de caractère à afficher
        for(i=0; i<4;i++)
            if(maxL[i]>max) max = maxL[i];
            
        int taille = (max*4)+13; // On récupére la larguer du tableau a afficher
        char etoile[taille+1];
        memset(etoile, ' ', taille); // On préremplie le tableau étoile par des caractères de fin de chaine
        
        for(i=0; i<taille;i++) etoile[i]='*'; // On remplie le tableau étoile par des étoiles
        etoile[taille]='\00'; // On marque la fin du tableau de chaine de caractère
    
        FILE *file = fopen(fichier, mode?"a":"w"); // On ouvre le fichier a remplir
    
        char pm[]="Prenom", nm[]="Nom", cl[]="Classe", hr[]="Heure";
    
        fprintf(file, "\nLa liste des présences pour la date %02d/%02d/%d est:\n", date.jour, date.mois, date.annee);
        fprintf(file, "\n%s\n", formater(pm, nm, cl, hr, max)); // On affiche les en têt du tableau
        fprintf(file, "%s \n", etoile); // On affiche les étoiles
        
        for (int i = 0; i<size;i++){
                presence = presences[i];
                etud = presence.etudiant;
                sprintf(heure, "%02d:%02d:%02d", presence.heure.heure, presence.heure.minute, presence.heure.second);
                fprintf(file, "%s\n", formater(etud.prenom, etud.nom, etud.classe, heure, max));
        }
        fprintf(file, "%s\n", etoile);
        fclose(file);
        
        printf("\nLa liste des présences pour la date: %02d/%02d/%d a été générer dans le fichier %s.\n\n", date.jour, date.mois, date.annee, fichier);
    }
    
}



  ////////////////////////////////////////////////////////////
 // Fonction pour ....                                     //
////////////////////////////////////////////////////////////

// Fonction pour marquer les présences
void markPresence(Etudiant etudiant, int status){
    Presence presence;
    
    // Si l'étudiant est déja marqué present
    if(isMarkPresence(etudiant)){
        printf("\n👋 Salut %s %s, vous êtes déja marquer présent\n\n", etudiant.prenom, etudiant.nom);
        error(); showContinue();
    }else{ // S'il n'est pas encore marqué present on le marque
        Heure h = actualHeure();
        presence.etudiant = etudiant;
        presence.date = actualDate();
        presence.heure = h;
        presence.status = status;
        if(addNewPresence(presence) && status==1){
            printf("✅ Code Valide, %s %s est présent à %02d:%02d:%02d\n", etudiant.prenom, etudiant.nom, h.heure, h.minute, h.second);
            error(); showContinue();
        }
    }
}


  ////////////////////////////////////////////////////////////
 // Fonction pour pour les options de l'utilisateur        //
////////////////////////////////////////////////////////////

// Fonction qui fait les marquages de présence côté admin
void optionMarkPresence(Connexion connect){
    getchar();
    int quit=1, i=0; char code[10], nm;
    Etudiant etud;
    system("clear");
    while (quit){
        puts("-----------------------------------------------------");
        puts("||------------> MARQUER LES PRESENCES <------------||");
        puts("-----------------------------------------------------\n");
        i = 0;
        while (1){
            // On préremplie le tableau code de caractère de fin de chaine
            memset(code, '\0', sizeof(code));
            printf("Entrer votre code [Q pour quitter]: ");
            nm = getchar(); // On récupére le premier touche que l'utilisateur tape
            while (nm != '\n'){
                if(i==6) nm = ' ';
                if(nm != ' '){
                    code[i] = nm; i++;
                }else{
                    while(getchar() != '\n');
                    i = 0; break;
                }
                nm = getchar(); // On récupére une a une les touches taper par l'utilisateur
            }
            
            if(i != 0) break;
            else {puts("❌ Code invalide!!!"); showContinue();}
        }
        
        if(i==1 && (code[0] == 'q' || code[0] == 'Q')){
            puts("\n\tVeuillez vous authentifier.\n");
            getPassWord(code);
            
            // On vérifie si le code saisie par l'utilisateur correspond à celui de l'admin
            if(strcmp(code, connect.password) == 0) quit = 0;
            else{
                system("clear");
                puts("\n❌ Vous n'êtes pas le admin.");
                showContinue(); // On affiche le message pour continuer
            }
        }else{
            if(codeIsCorrect(code, &etud)) markPresence(etud, 1);
            else {puts("❌ Code invalide!!!"); showContinue();}
        }
        
    }
    
}

// Fonction qui fait la gestion des étudiants
void optionGestionStudent(){}

// Fonction qui fait la gestion des messages
void optionSendMessage(){
    int choix, size, sizeE, ch; 
    char msg[] = "Voulez vous envoyer un autre message", classe[15];
    const char LIST_CLASS[3][10] = {"DEV WEB", "DEV DATA", "REF DIG"};
    Message messages[SIZE_ARRAY_MESSAGE], message;
    Etudiant etudiants[SIZE_ARRAY_STUDENT];
    sizeE = getAllStudent(etudiants);
    
    do{
        system("clear");
        puts("\n\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
          puts("\t+=+                     MESSAGERIE                     +=+");
          puts("\t+=+                     ==========                     +=+");
          puts("\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
            choix = menuMessage(); // On récupére le choix de l'utilisateur
        // On préremplie le tableau message de caractère de fin de chaine
        memset(message.message, '\0', sizeof(message.message));
        getchar();
        switch (choix){
            case 1:
                do{
                    
                    // On demande le messge a 
                    getMessageInput(message.message);  
                    size = getAllMessages(messages) + 1;
                    
                    message.date = actualDate();
                    message.heure = actualHeure();
                    message.status = 1;
                    
                    // On envoie le message a tout les étudiants
                    for(int i=0; i<sizeE; i++){
                        message.dest = etudiants[i];
                        message.id = size + i; 
                        addNewMessages(message); // On enregistre les messages 
                    }
                    
                    system("clear"); // On efface le terminal
                    puts("\nLe message a été envoyer a tout le monde.\n");
                    
                    // On demande si l'utilisateur veut envoyer à nouveau une message à tout le monde
                    ch = getChoice(msg);
                    
                    system("clear"); // On efface le terminal
                    
                    // On préremplie le tableau message de caractère de fin de chaine
                    memset(message.message, '\0', sizeof(message.message));
                }while(ch);
                break;
            case 2:
                int i, j, quit; char nm;
                do{
                    quit=1;
                    // On boucle tantque l'utilisateur n'aura pas saisie une bonne classe
                    while (quit){
                        system("clear"); // On efface le terminal
                        
                        // On préremplie le tableau classe avec des caractères de fin de chaine
                        memset(classe, '\0', sizeof(classe));
                        
                        i=j=0; // On initialise les variables i et j à 0
                        // On boucle tantque l'utilisateur n'aura saisie au moins une caractère
                        // getchar();
                        while (1){
                            printf("Entrer la classe: ");
                            nm = getchar(); // On récupére le permier caractère saisi par l'utilisateur
                            
                            // On vérifie si l'utilisateur a moins saisie une caractère différent de '\n' ou la touche entrer
                            while (nm != '\n'){
                                classe[i++] = nm;
                                nm = getchar();
                            }
                            
                            // S'il la saisie au moins une caractère on arrête de boucler
                            if(i != 0) break; 
                        }
                        
                        // On vérifie s'il a saisie une classe valide
                        for(j=0; j<3; j++){
                            // Si c'est le cas on arrêt de lui demander de saisir a nouveau la classe
                            if(strcmp(LIST_CLASS[j], classe) == 0){
                                quit = 0; break;
                            }
                        }
                    }
                    system("clear"); // On efface le terminal
                    getMessageInput(message.message);
                    size = getAllMessages(messages) + 1;
                    sizeE = getListClass(classe, etudiants);
                    
                    // Récupération de la date et l'heure actuel
                    message.date = actualDate();
                    message.heure = actualHeure();
                    message.status = 1; // On dit que le message n'a pas encore était lu
                    
                    // On envoie le message a tout les étudiants
                    for(int i=0; i<sizeE; i++){
                        message.dest = etudiants[i];
                        message.id = size + i; 
                        addNewMessages(message); // On enregistre les messages 
                    }
                    
                    system("clear"); // On efface le terminal
                    // On affiche une message pour dire que le message a été envoyer
                    printf("\nLe message a été envoyer à tout les étudiants de la classe: %s\n\n", classe);
                    ch = getChoice(msg); // On vérifie s'il veut envoyer un autre message
                    
                } while (ch);
                
                break;
            case 3:
                char codes[SIZE_ARRAY_STUDENT][12];
                int nbC=0, trouver;
                do{      
                    // On préremplie le tableau code de caractère de fin de chaine
                    memset(codes, '\0', sizeof(codes));
                    while (1){
                        i=nbC=0;
                        printf("Entrer le(s) code(s): ");
                        nm = getchar();
                        while (nm != '\n'){
                            if(nm == ','){
                                nbC++; i=0;
                            }else{
                                if(nm != ' '){
                                    codes[nbC][i++] = nm;
                                }
                            }
                            nm = getchar();
                        }
                        if(i!=0) break;
                    }
                    
                    // On récupére les messages, les étudiants et leurs nombres
                    size = getAllMessages(messages) + 1;
                    sizeE = getAllStudent(etudiants);
                    
                    // On préremplie le tableau code de caractère de fin de chaine
                    memset(message.message, '\0', sizeof(message.message));
                    getMessageInput(message.message);
                    
                    // Récupération de la date et l'heure actuel
                    message.date = actualDate();
                    message.heure = actualHeure();
                    message.status = 1; // On dit que le message n'a pas encore était lu
                    
                    // On envoie le message a tout les étudaints dont leur code a été spécifier
                    for(i=0; i<=nbC; i++){
                        trouver = 0;
                        for(j = 0; j<sizeE; j++){
                            if(strcmp(codes[i], etudiants[j].code) == 0){
                                message.dest = etudiants[j];
                                trouver = 1; break;
                            }
                        }
                        
                        // On vérifie si le code donner est le code d'un des étudiants
                        if(trouver){ // Si c'est le cas on lui envoi un message
                            message.id = size + i;
                            addNewMessages(message);
                            printf("\n✅ Code Valide: %s, le message a été envoyer à %s %s classe %s.\n", codes[i], message.dest.prenom, message.dest.nom, message.dest.classe);
                        }else{ // Sinon on affiche que le code n'est pas valide
                            printf("\n❌ Code Invalide: %s !!! Message non envoyer.\n", codes[i]);
                        }
                    }
                    puts("");
                    showContinue();
                    ch = getChoice(msg);    
                
                } while (ch);
                
                break;
        }
    
    }while(choix != 4);
}

// Fonction qui message a l'utilisateur de lire les messages
void optionGetMessage(Etudiant etud, Message *msg, int size){
    system("clear");
    puts("\n\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
      puts("\t+=+                     MESSAGERIE                     +=+");
      puts("\t+=+                     ==========                     +=+");
      puts("\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      
    if(size==0){
        puts("\nVous n'avez pas de nouveau message.\n");
        error(); showContinue();
    }else{
        system("clear");
        // On affiche les messages reçu
        for(int i=0; i<size; i++){
            // printf("%s %s [%d]=> %s\n", etud.prenom, etud.nom, i+1, msg[i].message);
            showMessage(msg[i], i+1);
        }   
        error(); puts("");
        showContinue();
            
        Message allMessage[SIZE_ARRAY_MESSAGE], message;
        // On récuper tout les messages
        int size1 = getAllMessages(allMessage);
        // On ouvre le fichier où sont stocker les messages pour les mettres à jour.
        FILE *file = fopen(FILE_MESSAGE, "wb");
        // On fait une recherche sur les messages que l'utilisateur a ouvert pour les marquer comme lu
        for(int i=0; i<size1; i++){
            message = allMessage[i];
            for(int j = 0; j<size; j++){
                if(message.id == msg[j].id){
                    message.status = 0;
                    break;
                }
            }
            // On met à jour les données ou message
            fwrite(&message, sizeof(Message), 1, file);
        }
        
        fclose(file);// On ferme le fichier
    }
}

// Fonction qui permet de faire la génération de fichier
void optionGenerateFile(){
    Presence presences[SIZE_ARRAY_PRESENCE];
    int ch, size = getAllPresence(presences);
    Date date = presences[0].date;
    char fichier[36]="liste_des_presences.txt";
    for(int i=0; i<size; i++){
        if(i==0)
            generateFile(fichier, 1, date);
        else{
            if(dateIsEqual(date, presences[i].date)){
                date = presences[i].date;
                generateFile(fichier, 1, date);
            }
        }
    }
    system("clear");
    puts("\nLa liste de tout les présences a été générer dans un fichier liste_des_presences.txt.\n");
    do{
        ch = menuGenerateFile();
        if(ch == 1){
            system("clear"); // On efface le terminal
            Date dt = getDate(), dtA=actualDate();
            
            if(dateIsEqual(dt, dtA) == 1){
                printf("\n❌ La date %02d/%02d/%d n'est pas encore venu. Aujourd'hui c'est le %02d/%02d/%d.\n\n", dt.jour, dt.mois, dt.annee, dtA.jour, dtA.mois, dtA.annee);
            }else{
                memset(fichier, '\0', sizeof(fichier));
                sprintf(fichier, "liste_prensence_%02d_%02d_%02d.txt", dt.jour, dt.mois, dt.annee);
                generateFile(fichier, 0, dt);
            }
        }
    }while(ch != 2);
}



  /////////////////////////////////////////////////////////
 //  Fonction la création de donnée                     //
/////////////////////////////////////////////////////////

// Fonction pour la création d'un nouveau login et mot de passe
Connexion createConnexion(){
    Connexion connect;
   Connexion connexion[SIZE_ARRAY_CONNEXION];
    int size = getAllConnexion(connexion);
    
    connect = getLogin();
    if(size == 0){
        connect.id = 1;
    }else{
        for(int i = 0; i<size; i++){
            if(strcmp(connect.login, connexion[i].login) == 0 && strcmp(connect.password, connexion[i].password) == 0){
                createConnexion();
            }
        }
    }
    return connect;
}

Etudiant createStudent(){
    Etudiant etudiant;
    
    return etudiant;
}





  /////////////////////////////////////////////////////////
 //               La fonction principal                 //
/////////////////////////////////////////////////////////
int main(){
   
    // Déclaration des variables
    int quitter=1, choix;
    char msg[] = "Voulez-vous vous reconnecter";
    
    // On boucle tant que la variable quitter est différent de 0
    while (quitter){
        system("clear"); // On efface le terminale
        // Récupération de la connexion
        Connexion connect = getConnexion();
        // system("clear"); // On efface le terminale
        
        // Si c'est un admine qui se connecte
        if(connect.status == 1){
            do{
                choix = menuAdmin(); // affichage du menu de l'admin
                switch (choix){
                    case 1: break;
                    case 2: optionGenerateFile();
                        break;
                    case 3: optionMarkPresence(connect);
                        break;
                    case 4: optionSendMessage();
                        break;
                }
            }while(choix != 5);
        }
        // Si c'est un étudiant qui se connecte
        else{
            Message msgs[SIZE_ARRAY_MESSAGE];
            Etudiant etud = getStudent(connect.id);
            int nb;
            do{
                nb = getMessageForStudent(etud, msgs);
                choix = menuUser(nb); // affichage du menu de l'étudiant
                switch (choix){
                    case 1:
                        markPresence(etud, 1);
                        break;
                    case 2:
                        break;
                    case 3:
                        optionGetMessage(etud, msgs, nb);
                        break;
                }
                
            } while (choix != 4);
        }
        
        system("clear");
        // On récupére si l'utilisateur veut quitter l'application ou se reconnecter
        quitter = getChoice(msg);
    }
    
    system("clear"); // On efface le terminal
    puts("\n\tMerci d'avoir utiliser l'application gestion d'étudiant\n");
    
    return 0;
}