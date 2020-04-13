#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <math.h>
#include <time.h>


#define MAXCHAR 100


int theodon_bon_hachage(char *chaine, int taille_Hachage) // FNV
{
    //int h = 1123;
    unsigned int h = 1123;
    //int h = 21269;
    int k = 21269;
    //int k = 21269;
    //int t = 421783;
    int i;

    for(i = 0; chaine[i] != '\0'; i++)
    {
        h = ((h * k) ^ chaine[i]) + 33;
        h = h%taille_Hachage;
    }

    return h%taille_Hachage;
}

void insertion(char **mot, int hKey, int taille_Hachage, char* nvChaine)
{
    int i = hKey;
    while(mot[i][0] != '\0')
    {
        i++;
        i = i%taille_Hachage;
        if (i == hKey){printf("Table de hachage trop petite!\n");exit(EXIT_FAILURE);}
    }
  mot[i] = nvChaine;
}

bool contientElement(char **mot, int hKey, int taille_Hachage, char* chaine)
{
    int i = hKey;
    while(strcmp(mot[i],chaine) != 0)
    {
        if (mot[i][0] == '\0'){return false;}
        i++;
        i = i%taille_Hachage;
        if (i == hKey){return false;}
    }
    return true;
}

void remove_char_from_string(char c, char *str)
{
    int i=0;
    int len = strlen(str)+1;

    for(i=0; i<len; i++)
    {
        if(str[i] == c)
        {
            // Décale tous les caractères suivant "c" d'un rang vers la gauche
            strncpy(&str[i],&str[i+1],len-i);
        }
    }
}

char *string_copy(char *str)
{
    char *copy;
    int j;
    copy = malloc(sizeof(*str)*strlen(str));

    for(j = 0; str[j] != '\0'; j++)
    {
        copy[j] = str[j];
    }

    return copy;
}

int main(int argc, const char *argv[]) {

    //int taille_Hachage = 252509;
    int taille_Hachage = 242509;
    //int taille_Hachage = 10;
    //int taille_Hachage = 707923;
    //int taille_Hachage = 1.5*pow(2,19); // Le plus opti en temps mais pas en espace
    char str[MAXCHAR];
    FILE *fp;
    FILE *fp2;

    int hKey;

    char *t[taille_Hachage];

    int i;
    int collision;
    int nbMots;
    int nbMotsFile;
    int nbMotsCommuns;

    if (argc < 3) {
        fprintf(stderr, "%s <fichier1> <fichier2>\n", argv[0]);
        return EXIT_FAILURE;
    }


    for(i = 0; i < taille_Hachage; i++)
    {
          t[i] = malloc(sizeof(char*));
      t[i][0] = '\0';
    }
        

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Impossible d'ouvrir %s",argv[1]);
        return EXIT_FAILURE;
    }
    i = 0;
    collision = 0;
    nbMots=0;
    while (fgets(str, MAXCHAR, fp) != NULL)
    {
        remove_char_from_string('\n',str);
        remove_char_from_string('\r',str);
        remove_char_from_string(' ',str);
        remove_char_from_string('-',str);
        hKey = theodon_bon_hachage(str,taille_Hachage);
        nbMots++;


        if(t[hKey][0]!='\0')
        {
            collision++;
        }

        insertion(t,hKey,taille_Hachage,string_copy(str));
        i++;
    }
    fclose(fp);

    fp2 = fopen(argv[2], "r");

    if (fp2 == NULL)
    {
        printf("Impossible d'ouvrir %s",argv[2]);
        return EXIT_FAILURE;
    }
    nbMotsCommuns = 0;
    nbMotsFile=0;
    while (fgets(str, MAXCHAR, fp2) != NULL)
    {
        remove_char_from_string('\n',str);
        remove_char_from_string('\r',str);
        remove_char_from_string(' ',str);
        remove_char_from_string('-',str);
        hKey = theodon_bon_hachage(str,taille_Hachage);
        nbMotsFile++;

        if(contientElement(t,hKey,taille_Hachage,str))
        {
            nbMotsCommuns++;
            printf("%s\n", str);
        }
    }
    fclose(fp2);

    if (argc > 3 && *argv[3] == '1') {
        printf("Nombre de mots en commun: %d\n", nbMotsCommuns);
        printf("Nombre de mots absents du fichier de référence: %d\n", nbMotsFile-nbMotsCommuns);
        printf("Nombre de mots dans le fichier %s: %d\n", argv[2],nbMotsFile);
        printf("Nombre de collisions dans la table de hachage: %d\n", collision);
        printf("Taux de collisions dans la table de hachage : %f%%\n", (float)100*collision/nbMots);
    }

    return 0;
}