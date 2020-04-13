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


struct node {
    int data;
    char *c;
    struct node *array[26];
};

struct node* new_node(struct node *h)
{
    unsigned int i;
    h = malloc(sizeof (struct node));
    for (i = 0; i < 26; ++i)
        h->array[i] = 0;
    h->data = 0;
    return h;
}

struct node* insert(struct node *h, char *c, int value)
{
    int i;
    int j;
    char *pc;
    if (strlen(c) == 0)
        return h;
    if (h == NULL)
        h = new_node(h);
    struct node *p = h;
    struct node *ptemp;

        if(c[0]-'a' < 0 || c[0]-'a' > 25)
        {
            printf("La chaîne '%s' contient un caractère interdit: %s.\n", c, &c[0]);
            return h;
        }

        if (p->array[c[0] - 'a'] == NULL)
        {
            p->array[c[0] - 'a'] = malloc(sizeof (struct node));
            p = p->array[c[0] - 'a'];
            p->c = malloc(sizeof(&c[0])*strlen(c));
            strncpy(p->c,&c[0],strlen(c)-0);
            p->data = 1;
            return h;
        }else{
            p = p->array[c[0] - 'a'];
            pc = p->c;

            j = 0;

            while(c[j] == pc[j] && j < strlen(pc) && j < strlen(c))
            {
                j++;
            }

            if(j == strlen(c) && j < strlen(pc))
            {
                p = insert(p,string_copy(&pc[j]),1);
                strncpy(p->c,c,j);
                p->c[j] = '\0';
                return h;
            }
            else if(j == strlen(pc) && j < strlen(c))
            {
                p = insert(p,&c[j],1);
                return h;
            }
            else if(j == strlen(pc) && j == strlen(c))
            {
                return h;
            }else{
                ptemp = malloc(sizeof (struct node));
                for (i = 0; i < 26; ++i)
                {
                    ptemp->array[i] = p->array[i];
                    p->array[i] = NULL;
                }
                ptemp->c = malloc(sizeof(&p->c[j])*strlen(&p->c[j]));
                for (i = 0; i < strlen(pc); ++i)
                {
                    ptemp->c[i] = p->c[i+j];
                };
                ptemp->data = p->data;
                p = insert(p,&c[j],1);
                p->array[ptemp->c[0] - 'a'] = ptemp;
                p->c[j] = '\0';
                p->array[ptemp->c[0] - 'a'] = ptemp;
                p->data = 0;
                return h;
            }
        }
}



bool search(struct node *h, char *s, char *dat)
{
    int j;
    char *pc;
    struct node *p = h;

    if (strlen(s) > 0) {
        if(s[0]-'a' < 0 || s[0]-'a' > 25)
        {
            printf("La chaîne '%s' contient un caractère interdit: %s.\n", s, &s[0]);
            return false;
        }

        if(p->array[s[0]-'a'] != NULL) {
            p = p->array[s[0]-'a'];
            pc = p->c;
            j = 0;

            while(s[j] == pc[j] && pc[j]!='\0' && s[j]!='\0')
            {
                j++;
            }

            if(pc[j]=='\0' && s[j]=='\0')
            {
                return (p->data != 0);
            }
            else if (j == strlen(s))
            {
                return false;
            }
            else
            {
                if(pc[j]!='\0')
                    return false;
                if(p->array[s[j]-'a'] != NULL) {
                    return search(p, &s[j], dat); 
                }
                else
                {
                    return false;
                }
            }
        } else{
            return false;
        }
    } else {
    	// Normalement, ce cas n'apparait jamais
        if (h->data != 0){
            return true;}
        if (h->data == 0){
            return false;
        }
    }
    return false;
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
        fprintf(stderr, "%s <fichier1> <fichier2>\n", argv[0]);
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