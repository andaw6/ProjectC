
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define FILE_LOGIN "login.bin"
#define SIZE_LOGINS 20
#define FILE_ETUDIANT "etudiant.bin"
#define SIZE_ETUDIANTS 20
#define FILE_PRESENCE "presence.bin"
#define SIZE_PRESENCES 50
#define FILE_LISTE_PRESENCE "liste_presence.txt"


typedef struct{
    int id;
    int status;
    char login[20];
    char password[20];
} CONNEXION;

typedef struct{ int jour, mois, annee; } DATE;

typedef struct{ int heure, minute, second; } HEURE;

typedef struct{
    int id;
    char prenom[30];
    char nom[20];
    char classe[10];
    char matricule[11];
    char codeQR[10];
} ETUDIANT;

typedef struct{
    ETUDIANT etudiant;
    DATE date;
    HEURE heure;
    int status;
} PRESENCE;


// Déclaration des prototypes
int ajouterEtudiant(ETUDIANT etudiant);
int menuAdmin(void);
int menuAdmin1(void);
int ajouterPresence(PRESENCE presence);
int menuEtudiant(void);
int menuEtudiant(void);
int menuClass(void);
int sauveConnection(CONNEXION connect);
int getAllLogins(CONNEXION *logins);
int loginExist(CONNEXION connect, CONNEXION *logins, int size);
int getChoice(char *message);
int verifiCodeQR(char code[], ETUDIANT *etud);
int getAllEtudiant(ETUDIANT *etudiants);
int estMarquerPresent(ETUDIANT etudiant);
int menuGestionFichier(void);
int dateIsEqual(DATE date1, DATE date2);
int afficherListeClasse(char *classe, ETUDIANT* etudiantClass);
int getListePresence(PRESENCE *presences);
int getListPresenceDate(PRESENCE *presences, DATE date);
int dateEstValide(DATE date);
CONNEXION getLogin(void);
CONNEXION getConnection(void);
DATE obtenirDate(void);
DATE getDateActuel(void);
HEURE getHeureActuel(void);
ETUDIANT getEtudiant(int id);
void error(void);
void desactiver(void);
void generateLogin(void);
void getPassWord(char * password);
void reactiver(void);
void generateListEtudiant(void);
void menuConnection(void);
void marquerPresence(ETUDIANT etudiant, int status);
void afficherLaListeDesPresence(void);
void afficherContinuer(void);
void generateFichierPresent(DATE date);
void generateFichierListePresent(void);
char* formater(char *prenom, char *nom, char *classe, char *heure, int max);




// Le programme principal
int main(){

    // generateFichierListePresent();
    menuConnection();

    
    return 0;
}





// L'implémentation des corps des fonctions

void generateListEtudiant(){
    ETUDIANT LIST_ETUDIANT[9] = {
        //id  Prenom    Nom       Classe       matricule    code QR
        {1, "Fatou",   "Diop",   "DEV WEB" , "MAT-FD0001", "202401"}, 
        {2, "Abdou",   "Ndiaye", "DEV WEB" , "MAT-AN0002", "202402"},
        {3, "Khady",   "Diallo", "REF DIG" , "MAT-KD0003", "202403"},
        {4, "Issa",    "Fall",   "REF DIG" , "MAT-IF0004", "202404"},
        {5, "Mamadou", "Sow",    "REF DIG" , "MAT-MS0005", "202405"},
        {6, "Khalifa", "Ciss",   "DEV DATA", "MAT-KC0006", "202406"},
        {7, "Lamine",  "Wade",   "DEV DATA", "MAT-LW0007", "202407"},
        {8, "Amadou",  "Ndoye",  "DEV DATA", "MAT-AN0008", "202408"},
        {9, "Awa",     "Faye",   "DEV DATA", "MAT-AF0009", "202409"}
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

int menuAdmin1(){
    int selected = 0;
    char menu[5][25] = {
        "GESTION DES ÉTUDIANTS ",
        "GÉNÉRATION DE FICHIERS",
        "MARQUER LES PRÉSENCES ",
        "ENVOYER UN MESSAGE",
        "QUITTER"
    };
    char touch;
    struct termios old_settings, new_settings;

    puts("Appuyez sur la touche A pour monter et Q pour descendre:");
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    setbuf(stdin, NULL); // Disable buffering
    
    int nb = 0;
    
    do{
        system("clear");
        puts("----- MENU ADMINISTRATEUR ------");
        for(int i=0; i<5; i++){
            nb = (i == selected)?41:0;
            printf("\033[%dm%s\033[0m\n", nb, menu[i]);
        }
        // puts("Pour descendre ou monter appyer sur la touche 'A'");
        touch = getchar();
        if (touch == 'a' || touch == 'A') {
            selected++;
            if(selected == 5) selected = 0;
        }
    
    }while(touch != '\n');

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
  
    return selected+1;
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
                system("clear");
                choix = menuAdmin();
                // choix = menuAdmin1();
                system("clear");
                ETUDIANT etudiants[SIZE_ETUDIANTS];
                switch (choix){
                    case 1:
                    case 4: 
                        puts("\nOption indisponible.\n");
                        break;
                    case 2:
                        generateFichierListePresent();
                        puts("La liste de tout les présences a été générer dans un fichier liste_presence.txt.\n");
                        do{
                            ch = menuGestionFichier();
                            if(ch == 1){
                                system("clear");
                                DATE dt = obtenirDate();
                                generateFichierPresent(dt);
                            }
                            else if (choix == 2)
                            {
                                system("clear");
                                afficherLaListeDesPresence();
                            }
                            
                        }while(ch != 3);
                        system("clear");
                        continue;
                        
                    case 3:
                        getchar();
                        int quitter = 1, i=0; char code[10], nm;
                        ETUDIANT etudiant;
                        
                        while (quitter)
                        {
                            // system("clear");
                            puts("-------------------------------------------------------------");
                            puts("------------------> MARQUER LES PRESENCES <------------------");
                            puts("-------------------------------------------------------------\n");
                            i = 0;
                            while(1){
                                printf("Entrer votre code [Q pour quitter]: ");
                                nm = getchar();
                                while (nm != '\n'){
                                    if(i==6) nm = ' ';
                                    if(nm != ' '){
                                        code[i] = nm; i++;
                                    }else{
                                        while(getchar() != '\n');
                                        i = 0;
                                        break;
                                    }
                                    nm = getchar();
                                }
                                
                                if(i != 0) break;
                                else{
                                    puts("❌ Code invalide!!!");
                                    afficherContinuer();
                                }
                            }
                            
                            if(i==1 && (code[0] == 'q' || code[0] == 'Q')){
                                puts("\n\tVeuillez vous authentifier.\n");
                                getPassWord(code);
                                if(strcmp(code, connect.password) == 0){
                                    quitter = 0;
                                }else{
                                    puts("\n❌ Vous n'êtes pas le admin.");
                                    afficherContinuer();
                                }
                            }else{
                                if(verifiCodeQR(code, &etudiant)){
                                    marquerPresence(etudiant, 1);   
                                }else{
                                    puts("❌ Code invalide!!!");
                                    afficherContinuer();
                                }
                            }
                        }
                        
                        
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
                        getchar();
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
    
        printf("\nSalut %s %s, vous êtes déja marquer présent\n\n", etudiant.prenom, etudiant.nom);
        afficherContinuer();    
    }else{
        DATE date = getDateActuel();
        HEURE heure = getHeureActuel();
        presence.etudiant = etudiant;
        presence.date = date;
        presence.heure = heure;
        presence.status = status;
        char nm;
        if(ajouterPresence(presence) && status==1){
            printf("✅ Code Valide, %s %s est présent à %02d:%02d:%02d\n", etudiant.prenom, etudiant.nom, heure.heure, heure.minute, heure.second);
            afficherContinuer();
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
    int size = getListPresenceDate(presences, getDateActuel());
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


int menuGestionFichier(){
    int choix;
    do
    {
        puts("\n1) Générer la liste des présences par date");
        puts("2) Afficher la liste des presences d'aujourd'hi: ");
        puts("3) Retour");
        printf("\nVotre choix: ");
        if(scanf("%d", &choix) != 1) error();
    } while (choix < 1 || choix > 3);
    
    return choix;
}

void afficherLaListeDesPresence(){
    ETUDIANT etudiants[SIZE_ETUDIANTS], etud;
    PRESENCE precences[SIZE_PRESENCES];
    DATE date= getDateActuel();
    int sizeP = getListPresenceDate(precences, date);
    int sizeE = getAllEtudiant(etudiants);
    char status[sizeE][8];
    
    puts("La liste des présences et absences.");
    printf("Pour la date %02d/%02d/%d\n\n",date.jour, date.mois, date.annee);
    
    
    int i, j, maxLongueurs[4] = {0};
    
    // Trouver les longueurs maximales des chaînes pour chaque champ
    for (i = 0; i < sizeE; i++) {
        char *st = estMarquerPresent(etudiants[i])?"present":"absent ";
        strcpy(status[i], st);
        // status[i] = estMarquerPresent(etudiants[i])?" present":"absent";
        maxLongueurs[0] = (strlen(etudiants[i].prenom) > maxLongueurs[0]) ? strlen(etudiants[i].prenom) : maxLongueurs[0];
        maxLongueurs[1] = (strlen(etudiants[i].nom) > maxLongueurs[1]) ? strlen(etudiants[i].nom) : maxLongueurs[1];
        maxLongueurs[2] = (strlen(etudiants[i].classe) > maxLongueurs[2]) ? strlen(etudiants[i].classe) : maxLongueurs[2];
        maxLongueurs[3] = (strlen(status[i]) > maxLongueurs[3]) ? strlen(status[i]) : maxLongueurs[3];
    }
    
    printf("| %-*s | %-*s | %-*s | %-*s |\n", maxLongueurs[0], "Prenom", maxLongueurs[1], "Nom", maxLongueurs[2], "Classe", maxLongueurs[3], "Statut");
    // Afficher l'en-tête du tableau
    for (i = 0; i < 4; i++) {
        for (j = 0; j < maxLongueurs[i] + 2; j++)
            printf("*");
        printf(" ");
    }
    printf("\n");
    
    // Afficher le contenu du tableau
    for (i = 0; i < sizeE; i++) {
        printf("| %-*s | %-*s | %-*s | %-*s |\n", maxLongueurs[0], etudiants[i].prenom, maxLongueurs[1], etudiants[i].nom, maxLongueurs[2], etudiants[i].classe, maxLongueurs[3], status[i]);
    }
    
    // Afficher le pied du tableau
    for (i = 0; i < 4; i++) {
        for (j = 0; j < maxLongueurs[i] + 2; j++)
            printf("*");
        printf(" ");
    }
    printf("\n");
    
}

int verifiCodeQR(char code[], ETUDIANT *etud){
    ETUDIANT etudiants[SIZE_ETUDIANTS];
    int size = getAllEtudiant(etudiants);
    for(int i=0; i<size; i++){
        *etud = etudiants[i];
        char cd[10];
        if(strcmp(code, etud->codeQR) == 0) return 1;
    }
    
    return 0;
}

void afficherContinuer(void){
    char nm;
    puts("Appuyer sur une touche pour continuer...");
    scanf("%c", &nm);
    system("clear");
}

int getListPresenceDate(PRESENCE *presences, DATE date){
    PRESENCE allpresences[SIZE_PRESENCES];
    int i, size = 0, size1 = getListePresence(allpresences);
    for(i=0;i<size1; i++){
        if(dateIsEqual(date, allpresences[i].date)){
            presences[size++] = allpresences[i];
        }
    }
    return size;
}

void generateFichierPresent(DATE date){
    PRESENCE presences[SIZE_PRESENCES], presence;
    ETUDIANT etud; char heure[9];
    int size = getListPresenceDate(presences, date);
    system("clear");
    if(size == 0){
        printf("Pour la date %02d/%02d/%d il y a pas de présence.\n", date.jour, date.mois, date.annee);
    }else{
        
        char fichier[30];
        sprintf(fichier, "liste_presente_%02d_%02d_%d.txt", date.jour, date.mois, date.annee);
        FILE *file = fopen(fichier, "w");
    
        int i, maxL[4]={0}, max = 0;
    
        date = presences[0].date;
        maxL[3] = 8; 
        
        for (i = 0; i < size; i++) {
            etud = presences[i].etudiant;
            maxL[0] = (strlen(etud.prenom) > maxL[0]) ? strlen(etud.prenom) : maxL[0];
            maxL[1] = (strlen(etud.nom) > maxL[1]) ? strlen(etud.nom) : maxL[1];
            maxL[2] = (strlen(etud.classe) > maxL[2]) ? strlen(etud.classe) : maxL[2];
        }  
        for(i=0; i<4;i++)
            if(maxL[i]>max) max = maxL[i];
        
        int taille = (max*4)+13;
        char etoile[taille+1];
        memset(etoile, ' ', taille);
        
        for(i=0; i<taille;i++) etoile[i]='*';
        etoile[taille]='\00';
      
        fprintf(file, "La liste des présences pour la date %02d/%02d/%d est:\n", date.jour, date.mois, date.annee);
        fprintf(file, "\n%s\n", formater("Prenom", "Nom", "Classe", "Heure", max));
        fprintf(file, "%s \n", etoile);
    
        for (int i = 0; i<size;i++){
            presence = presences[i];
            etud = presence.etudiant;
            sprintf(heure, "%02d:%02d:%02d", presence.heure.heure, presence.heure.minute, presence.heure.second);
            fprintf(file, "%s\n", formater(etud.prenom, etud.nom, etud.classe, heure, max));
        }
        fprintf(file, "%s\n", etoile);
        fclose(file);
        
        printf("La liste des présences pour la date %02d/%02d/%02d a été générer dans le fichier %s.\n", date.jour, date.mois, date.annee, fichier);
    }
}


char* formater(char *prenom, char *nom, char *classe, char *heure, int max){
    char information[100];
    char *info = information;
    
    sprintf(information, "| %-*s | %-*s | %-*s | %-*s |", max, prenom, max, nom, max, classe, max, heure);
    
    return info;
}


void generateFichierListePresent(){
    FILE *file = fopen(FILE_LISTE_PRESENCE, "w");
    PRESENCE presences[SIZE_PRESENCES], presence;
    ETUDIANT etud;  DATE date;
    char heure[9];
    int size = getListePresence(presences);
    int i, maxL[4]={0}, max = 0;
    
    date = presences[0].date;
    maxL[3] = 8; 
    
    for (i = 0; i < size; i++) {
        etud = presences[i].etudiant;
        maxL[0] = (strlen(etud.prenom) > maxL[0]) ? strlen(etud.prenom) : maxL[0];
        maxL[1] = (strlen(etud.nom) > maxL[1]) ? strlen(etud.nom) : maxL[1];
        maxL[2] = (strlen(etud.classe) > maxL[2]) ? strlen(etud.classe) : maxL[2];
    }
    
    for(i=0; i<4;i++)
        if(maxL[i]>max) max = maxL[i];
    int taille = (max*4)+13;
    char etoile[taille+1];
    memset(etoile, ' ', taille);
    
    for(i=0; i<taille;i++) etoile[i]='*';
    etoile[taille]='\00';
  
    fprintf(file, "La liste des présences pour la date %02d/%02d/%d est:\n", date.jour, date.mois, date.annee);
    fprintf(file, "\n%s\n", formater("Prenom", "Nom", "Classe", "Heure", max));
    fprintf(file, "%s \n", etoile);
    
    for (int i = 0; i<size;){
        presence = presences[i];
        etud = presence.etudiant;
        sprintf(heure, "%02d:%02d:%02d", presence.heure.heure, presence.heure.minute, presence.heure.second);
        if(dateIsEqual(presence.date, date)){
            // fprintf("| %-*s | %-*s | %-*s | %-*s |\n", max, etud.prenom, max, etud.nom, max, etud.classe, max, heure);
            fprintf(file, "%s\n", formater(etud.prenom, etud.nom, etud.classe, heure, max));
            i++;
        }else{
            date = presences[i].date;
            fprintf(file, "%s \n", etoile);
            fprintf(file, "\n\nLa liste des présences pour la date %02d/%02d/%d est:\n", date.jour, date.mois, date.annee);
            fprintf(file, "\n%s\n", formater("Prenom", "Nom", "Classe", "Heure", max));
            fprintf(file, "%s\n", etoile);
        }   
    }
    fprintf(file, "%s\n", etoile);
    

    fclose(file);
}

DATE obtenirDate(){
    DATE date; int i=0;
    do{
        if(i > 1){
            puts("La date n'est pas valide. Recommencez.\n");
        }i++;
        printf("Donner une date au format(jj/mm/aaaa): ");
        if(scanf("%d/%d/%d", &date.jour, &date.mois, &date.annee) != 3) error();
    }while(!dateEstValide(date));
    return date;
}

int dateEstValide(DATE date){
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