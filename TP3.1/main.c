#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <time.h>


#define MAXCHAR 100

struct node {
    int data;
    struct node *array[26];
};

struct node* new_node(struct node *h)
{
    int i;
    h = malloc(sizeof (struct node));
    for (i = 0; i < 26; ++i)
        h->array[i] = 0;
    return h;
}

struct node* insert(struct node *h, char *c, int value)
{
    int i;
    if (strlen(c) == 0)
        return h;
    if (h == NULL)
        h = new_node(h);
    struct node *p = h;
    for (i = 0; i < strlen(c); ++i) {
        if(c[i]-'a' < 0 || c[i]-'a' > 25)
        {
            printf("La chaîne '%s' contient un caractère interdit: %s.\n", c, &c[i]);
            return h;
        }

        if (p->array[c[i] - 'a'] == NULL){
            p->array[c[i] - 'a'] = malloc(sizeof (struct node));
        }
        p = p->array[c[i] - 'a'];
    }
    p->data = value;
    return h;
}


bool search(struct node *h, char *s, char *dat)
{
    char l[MAXCHAR] = {0};
    char a[2];
    strcpy(l, dat);
    if (strlen(s) > 0) {
        a[0] = s[0];
        a[1] = '\0';

        if(a[0]-'a' < 0 || a[0]-'a' > 25)
        {
            printf("La chaîne '%s' contient un caractère interdit: %s.\n", s, a);
            return false;
        }

        if(h->array[a[0]-'a'] != NULL) {
            strcat(dat, a);
            return search(h->array[a[0]-'a'], s+1, dat);
        } else{
            return false;
        }
    } else {
        if (h->data != 0){
            return true;
        }
        else
        {
            return false;
        }
    }
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



int main(int argc, const char *argv[])
{
    char str[MAXCHAR];
    char test[MAXCHAR];
    FILE *fp;
    FILE *fp2;

    struct node *h = 0;

    int i;
    int nbMots;
    int nbMotsFile;
    int nbMotsCommuns;

    if (argc < 3) {
        fprintf(stderr, "%s <fichier1> <fichier2> opt\n", argv[0]);
        return EXIT_FAILURE;
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Impossible d'ouvrir %s",argv[1]);
        return EXIT_FAILURE;
    }
    i = 0;
    nbMots=0;
    while (fgets(str, MAXCHAR, fp) != NULL)
    {
        remove_char_from_string('\n',str);
        remove_char_from_string('\r',str);
        remove_char_from_string(' ',str);
        remove_char_from_string('\t',str);
        remove_char_from_string('-',str);
        nbMots++;
        h = insert(h, strcpy(test,str), 1);
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
        remove_char_from_string('\t',str);
        remove_char_from_string('-',str);
        nbMotsFile++;

        if(search(h,str,strcpy(test,"")))
        {
            nbMotsCommuns++;
            printf("%s\n", str);
        }
    }


    if (argc > 3 && *argv[3] == '1') {
        printf("Nombre de mots en commun: %d\n", nbMotsCommuns);
        printf("Nombre de mots absents du fichier de référence: %d\n", nbMotsFile-nbMotsCommuns);
        printf("Nombre de mots dans le fichier %s: %d\n", argv[2],nbMotsFile);
    }
    

    return 0;
}