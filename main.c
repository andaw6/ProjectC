#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <termios.h>
#include <unistd.h>

#define FILE_LOGIN "login.bin"
#define SIZE_LOGINS 10

typedef struct{
    int id;
    int status;
    char login[20];
    char password[20];
} CONNEXION;


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

// Le programme principal
int main(){
    menuConnection();

    return 0;
}

// L'implémentation des corps des fonctions

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
     int quit = 1, choix;
    while (quit){
        system("clear");
        CONNEXION connect = getConnection();
        
        system("clear"); // Nétoyage du terminal
        if(connect.status == 1){
            do{
                choix = menuAdmin();
                switch (choix){
                    case 1:
                    case 2:
                    case 4:
                        puts("\nOption indisponible.\n");
                        break;
                    case 3:
                    
                    
                    break;
                }
                
            }while(choix != 5);
            getchar();
        }else{
            do{
                choix = menuEtudiant();
                switch (choix){
                    case 1:
                    
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