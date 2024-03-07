#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <termios.h>
#include <unistd.h>
#include <time.h>

#define FILE_LOGIN "login.bin"
#define SIZE_LOGINS 20
#define FILE_ETUDIANT "etudiant.bin"
#define SIZE_ETUDIANTS 20
#define FILE_PRESENCE "presence.bin"
#define SIZE_PRESENCES 50


typedef struct{
    int id;
    int status;
    char login[20];
    char password[20];
} CONNEXION;

typedef struct{
    int jour, mois, annee;
} DATE;

typedef struct{
    int heure, minute, second;
} HEURE;

typedef struct{
    int id;
    char prenom[30];
    char nom[20];
    char classe[10];
} ETUDIANT;

typedef struct{
    ETUDIANT etudiant;
    DATE date;
    HEURE heure;
    int status;
} PRESENCE;


// Déclaration des prototypes
void generateLogin(void);
void error(void);
int menuAdmin(void);
int menuEtudiant(void);
int menuEtudiant(void);
int sauveConnection(CONNEXION connect);
void menuConnection(void);
int getAllLogins(CONNEXION *logins);
int loginExist(CONNEXION connect, CONNEXION *logins, int size);
CONNEXION getLogin(void);
CONNEXION getConnection(void);
void desactiver(void);
void getPassWord(char * password);
void reactiver(void);
int getChoice(char *message);
DATE getDateActuel(void);
HEURE getHeureActuel(void);
ETUDIANT getEtudiant(int id);
int getAllEtudiant(ETUDIANT *etudiants);
void generateListEtudiant(void);
int ajouterEtudiant(ETUDIANT etudiant);
int dateIsEqual(DATE date1, DATE date2);
int getListeAllPresenceToday(PRESENCE *presences);
int getListePresence(PRESENCE *presences);
void marquerPresence(ETUDIANT etudiant, int status);
int ajouterPresence(PRESENCE presence);
int estMarquerPresent(ETUDIANT etudiant);
void afficherLaListeDesPresence(void);
int menuClass(void);
int afficherListeClasse(char *classe, ETUDIANT* etudiantClass);
int menuGestionFichier(void);


// Le programme principal
int main(){
    // afficherLaListeDesPresence();
    menuConnection();

    
    return 0;
}

// L'implémentation des corps des fonctions

void generateListEtudiant(){
    ETUDIANT LIST_ETUDIANT[9] = {
        //id  Prenom    Nom      Classe 
        {1, "Fatou",   "Diop",   "DEV WEB" }, 
        {2, "Abdou",   "Ndiaye", "DEV WEB" },
        {3, "Khady",   "Diallo", "REF DIG" },
        {4, "Issa",    "Fall",   "REF DIG" },
        {5, "Mamadou", "Sow",    "REF DIG" },
        {6, "Khalifa", "Ciss",   "DEV DATA"},
        {7, "Lamine",  "Wade",   "DEV DATA"},
        {8, "Amadou",  "Ndoye",  "DEV DATA"},
        {9, "Awa",     "Faye",   "DEV DATA"}
    };
    
    FILE *file=fopen(FILE_ETUDIANT, "wb");
    fclose(file);
    
    for(int i=0; i<9;i++){
        ajouterEtudiant(LIST_ETUDIANT[i]);
    }

}

void generateLogin(){
    CONNEXION LIST_LOGIN[10] = {
        //id, status, login, password
        {0, 1,"admin", "admin"},
        {1, 0,"fatou", "1234"},
        {2, 0,"abdou", "1234"},
        {3, 0,"khady", "1234"},
        {4, 0,"issa", "1234"},
        {5, 0,"mamadou", "1234"},
        {6, 0,"khalifa", "1234"},
        {7, 0,"lamine", "1234"},
        {8, 0,"amadou", "1234"},
        {9, 0,"awa", "1234"},
    };
    
    FILE *file=fopen(FILE_LOGIN, "wb");
    fclose(file);
    
    for (int i=0; i<10; i++){
        sauveConnection(LIST_LOGIN[i]);
    }
    
}

int menuAdmin(){
    int choix;
    do
    {
        puts("\nMENU ADMINISTRATEUR\n");
        puts("1) GESTION DES ÉTUDIANTS ");
        puts("2) GÉNÉRATION DE FICHIERS");
        puts("3) MARQUER LES PRÉSENCES ");
        puts("4) ENVOYER UN MESSAGE    ");
        puts("5) QUITTER               ");
        printf("\nVotre choix: ");
        if(scanf("%d", &choix) != 1) error();
        
    } while (choix<1||choix>5);
    return choix;
}

int menuClass(){
    int choix;
    do
    {
        puts("\nLa liste des classe:");
        puts("1) DEV WEB");
        puts("2) DEV DATA");
        puts("3) REF DIG");
        printf("\nVotre choix: ");
        if(scanf("%d", &choix) != 1) error();
    } while (choix>3||choix<1);
    
    return choix;
}

int menuEtudiant(){
    int choix;
    do{
        puts("\nMENU ETUDIANT\n");
        puts("1) MARQUER MA PRÉSENCE");
        puts("2) NOMBRE DE MINUTES D’ABSENCE");
        puts("3) MES MESSAGES (0)");
        puts("4) QUITTER");
        printf("\nVotre choix: "); 
        if(scanf("%d", &choix) != 1) error();
        
    }while(choix>4||choix<1);
    return choix;
}

void error(void){
    while(getchar() != '\n');
    puts("\nVous n'avez pas saisie de chiffre.\n");
}

int sauveConnection(CONNEXION connect){
    FILE *file = fopen(FILE_LOGIN, "ab");
    
    if(file == NULL){
        char *newFile;
        sprintf(newFile, "touch %s", FILE_LOGIN);
        system(newFile);
        sauveConnection(connect);
    }
    
    int rst = fwrite(&connect, sizeof(CONNEXION), 1, file);
    

    fclose(file);
    
    return rst; 
};

void menuConnection(void){
    int quit = 1, choix, ch, size=0; 
    while (quit){
        system("clear");
        CONNEXION connect = getConnection();
        
        system("clear"); // Nétoyage du terminal
        if(connect.status == 1){
            do{
                choix = menuAdmin();
                system("clear");
                ETUDIANT etudiants[SIZE_ETUDIANTS];
                switch (choix){
                    case 1:
                    case 4:
                        puts("\nOption indisponible.\n");
                        break;
                    case 2:
                        do{
                            ch = menuGestionFichier();
                            if(ch == 1){
                                system("clear");
                                afficherLaListeDesPresence();
                            }
                        }while(ch != 2);
                        system("clear");
                        continue;
                        
                    case 3:
                        
                        switch (menuClass())
                        {
                            case 1:
                                size = afficherListeClasse("DEV WEB", etudiants);
                                break;
                            case 2:
                                size = afficherListeClasse("DEV DATA", etudiants);
                                break;
                            case 3:
                                size = afficherListeClasse("REF DIG", etudiants);
                                break;
                        }
                        do{
                            printf("\nVotre choix: ");
                            if(scanf("%d", &ch) != 1) error(); 
                        }while(ch<1||ch>size);
                        
                        marquerPresence(etudiants[ch-1], 1);
                        
                    break;
                }
                
            }while(choix != 5);
            getchar();
        }else{
            ETUDIANT etudiant = getEtudiant(connect.id);
            do{
                choix = menuEtudiant();
                switch (choix){
                    case 1:
                        marquerPresence(etudiant, 1);
                        break;
                    case 2:
                    case 3:
                        puts("\nOption indisponible.\n");
                    break;
                }
            
            }while(choix != 4);
            getchar();
        }
        system("clear");
        if(!getChoice("\nVoulez-vous vous reconnecter")) quit = 0;
    }
        

}

int getChoice(char *message){
    int condition; char ch; 
    do{
        printf("%s? (o)ui ou (n)on: ",message);
        scanf("%s", &ch);
        condition = ch != 'n' && ch != 'N' && ch != 'o' && ch != 'O';
        if(condition){
            puts("\nErreur: votre choix n'est pas bonne. Réessayez svp...\n");
        }
    }while(condition);
    getchar();
    return (ch == 'n' || ch == 'N') ? 0: 1;
}

CONNEXION getConnection(){
    CONNEXION logins[SIZE_LOGINS], connect;
    
    int index = 0, size = getAllLogins(logins);
    
    while (1){
        connect = getLogin();
        if((index = loginExist(connect, logins, size))){
            return logins[index-1];
        }else{
            puts("\nLe login ou le mot de pass n'est pas correcte\n");
        }
    }
}

int getAllLogins(CONNEXION *logins){
    FILE *file = fopen(FILE_LOGIN, "rb");
    int size = 0;
    
    if(file == NULL) generateLogin();
        
    CONNEXION connect;
    
    // On récupere les connections des utilisateurs
    while (fread(&connect, sizeof(CONNEXION), 1, file)){
        logins[size++] = connect;
    }
    
    fclose(file);
    
    return size;
}

int loginExist(CONNEXION connect, CONNEXION *logins, int size){

    for(int i=0; i<size; i++){
        CONNEXION log = logins[i];
        if(strcmp(log.password, connect.password) == 0 && strcmp(log.login, connect.login) == 0){
            return i+1;
        }
        
    }
    return 0;
}

CONNEXION getLogin(){
    CONNEXION connect;
    char login[15], nm;
    int quit = 1, i=0;
    while (quit){
        printf("Donner votre login: ");
        nm = getchar();
        while(nm != '\n'){
            if(nm != ' '){
                login[i] = nm; i++;
            }else{
                while(getchar() != '\n');
                i=0; break;
            }
            nm = getchar();
        }
        if(i!=0) quit = 0;
        else{
            puts("\nErreur: veuillez retapez votre login\n");
        } 
    }
    
    strcpy(connect.login, login);
    getPassWord(connect.password);
    
    return connect;
}

// Le corps de la fonction qui permet d'obtenir le mot de passe d'un utlisateur
void getPassWord(char * password){
    int i = 0;
    char ch;
    
    printf("Entrez votre mot de passe : ");
    desactiver(); // Masquage de la saisie de l'utilisateur

    while (1) {
        ch = getchar(); // On recupere les caractères que l'utilisateur à saisie
 
        if (ch == '\n') {
            password[i] = '\0';
            break;
        } else {
            password[i++] = ch;
            printf("*"); // Affichage des étoiles 
            fflush(stdout); // Forcer l'affichage de l'étoile
        }
     }
    putchar('\n');
    reactiver(); // Réaffichage de la saisie de l'utilisateur
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

DATE getDateActuel(){
    DATE date;
    time_t dt = time(0); 
    struct tm *dateActuel = localtime(&dt); 
    
    date.jour = dateActuel->tm_mday;
    date.mois = dateActuel->tm_mon + 1;
    date.annee = dateActuel->tm_year + 1900;
    
    return date;
}

HEURE getHeureActuel(){
    HEURE heure;
    time_t temps= time(0); 
    
    struct tm *tempsActual = localtime(&temps);
    heure.heure = tempsActual->tm_hour;
    heure.minute = tempsActual->tm_min;
    heure.second = tempsActual->tm_sec;
    return heure;
}

ETUDIANT getEtudiant(int id){
    ETUDIANT etudiants[SIZE_ETUDIANTS], e;
    int size = getAllEtudiant(etudiants);
    for(int i=0; i<size; i++){
        if(etudiants[i].id == id) return etudiants[i];
    }
    return e;
}

int getAllEtudiant(ETUDIANT *etudiants){
    FILE *file = fopen(FILE_ETUDIANT, "rb");
    int size = 0;
    
    if(file == NULL){
        // file = fopen(FILE_ETUDIANT, "wb");
        generateListEtudiant();
    }
    
    ETUDIANT etudiant;
    while(fread(&etudiant, sizeof(ETUDIANT), 1, file)){
        etudiants[size++] = etudiant;
    }
    
    fclose(file);
    
    return size;
}

int ajouterEtudiant(ETUDIANT etudiant){
    FILE *file = fopen(FILE_ETUDIANT, "ab");
    
    if(file == NULL){
        char *newFile;
        sprintf(newFile, "touch %s", FILE_ETUDIANT);
        system(newFile);
        ajouterEtudiant(etudiant);
    }

    int rst = fwrite(&etudiant, sizeof(ETUDIANT), 1, file);
    
    fclose(file);
    
    return rst;// On retourne le resultat si O l'étudiant n'est pas ajouter et sion 1 l'étudiant est ajouter
}

void marquerPresence(ETUDIANT etudiant, int status){
    PRESENCE presence;
    
    if(estMarquerPresent(etudiant)){
        printf("\nSalut %s %s, vous êtes déja marquer présent à %d:%d\n", etudiant.prenom, etudiant.nom, presence.heure.heure, presence.heure.minute);
    }else{
        DATE date = getDateActuel();
        HEURE heure = getHeureActuel();
        presence.etudiant = etudiant;
        presence.date = date;
        presence.heure = heure;
        presence.status = status;
        if(ajouterPresence(presence) && status==1){
            printf("\nSalut %s %s vous être marquer presence le %02d/%02d/%02d à %02d:%02d:%02d\n", etudiant.prenom, etudiant.nom, date.jour, date.mois, date.annee, heure.heure, heure.minute, heure.second);
        }
    }
}

int getListePresence(PRESENCE *presences){
    FILE *file = fopen(FILE_PRESENCE, "rb");
    if(file == NULL) 
        file = fopen(FILE_PRESENCE, "wb");
        
    int size = 0;
    PRESENCE presence;
    while (fread(&presence, sizeof(PRESENCE), 1, file))
    {
        presences[size++] = presence;
    }
    
    return size;
}

// Fonction pour donner la liste de tout les étudiants
int getListeAllPresenceToday(PRESENCE *presences){
    PRESENCE presenceToday[SIZE_PRESENCES], presence;
    int size=0, size1 = getListePresence(presenceToday);
    for(int i=0; i<size1;i++){
        presence = presenceToday[i];
        if(dateIsEqual(getDateActuel(), presence.date))
            presences[size++] = presence;
    }
    
    return size;
}

int dateIsEqual(DATE date1, DATE date2){
    if(date1.annee == date2.annee && date1.mois == date2.mois && date1.jour == date2.jour)
        return 1;
    return 0;
}

int ajouterPresence(PRESENCE presence){
    FILE *file = fopen(FILE_PRESENCE, "ab");
    
    if(file == NULL){
        char *newFile;
        sprintf(newFile, "touch %s", FILE_PRESENCE);
        system(newFile);
        ajouterPresence(presence);
    }

    int rst = fwrite(&presence, sizeof(PRESENCE), 1, file);
    
    fclose(file);
    
    return rst;
}

int estMarquerPresent(ETUDIANT etudiant){
    PRESENCE presences[SIZE_PRESENCES];
    ETUDIANT etud;
    int size = getListeAllPresenceToday(presences);
    for(int i =0; i<size;i++){
        etud = presences[i].etudiant;
        if(etud.id == etudiant.id && presences[i].status == 1)
            return 1;
    }
    return 0;
}

int afficherListeClasse(char *classe, ETUDIANT* etudiantClass){
    system("clear");
    ETUDIANT etud,
             etudiants[SIZE_ETUDIANTS];
    
    int size1=0, size = getAllEtudiant(etudiants);
    printf("La liste des étudiants de la classe %s est:\n\n", classe);
    for (int i=0; i<size;i++){
        etud = etudiants[i];
        if(strcmp(etud.classe, classe) == 0){
            etudiantClass[size1++] = etud;
            printf("%d- %s %s\n", size1, etud.prenom, etud.nom);
        }
    }
    return size1;
}

void afficherLaListeDesPresence(){
    ETUDIANT etudiants[SIZE_ETUDIANTS], etud;
    PRESENCE precences[SIZE_PRESENCES];
    DATE date= getDateActuel();
    int sizeP = getListeAllPresenceToday(precences);
    int sizeE = getAllEtudiant(etudiants);
    
    printf("Pour la date %02d/%02d/%d\n",date.jour, date.mois, date.annee);
    puts("La liste des présences et absence. \n");
    
    for(int i=0, j=0; i<sizeE; i++){
        etud = etudiants[i];
        char *status = estMarquerPresent(etud)?" present":"absent";
        printf("- %s | %s | %s | %s\n", etud.prenom, etud.nom, etud.classe, status);
    }
    
}

int menuGestionFichier(){
    int choix;
    do
    {
        puts("\n1) Afficher la liste des presences: ");
        puts("2) Retour");
        printf("\nVotre choix: ");
        if(scanf("%d", &choix) != 1) error();
    } while (choix != 1 && choix != 2);
    
    return choix;
}