#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <math.h>
#include <time.h>


#define MAXCHAR 100


typedef struct Element Element;
struct Element
{
	char* chaine;
	Element *suivant;
};


typedef struct Liste Liste;
struct Liste
{
	Element *premier;
};

int hachage(char *chaine){
	int i =0, nombreHachage = 0;

	for(i=0 ; chaine[i] != '\0' ; i++)
	{
		nombreHachage += chaine[i];
	}
	nombreHachage %= 100;

	return nombreHachage;
}

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

// Cette fonction de hachage donne de légèrement meilleurs résultats
int hash_alternatif(char *chaine, int taille_Hachage) {
    unsigned int hash = 5381;
    int i;
    int k = 11;

    for(i = 0; chaine[i] != '\0'; i++)
    {
      hash = ((hash << 5) + hash) + chaine[i];
      hash = ((hash * k) ^ chaine[i]) + 33;
    }
    
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash = (hash >> 16) ^ hash;

    
    hash = ((hash >> 16) ^ hash) * 0x119de1f3;
    hash = ((hash >> 16) ^ hash) * 0x119de1f3;
    hash = (hash >> 16) ^ hash;

    hash ^= (hash << 13);
    hash ^= (hash >> 17);    
    hash ^= (hash << 5); 

    return hash%taille_Hachage;
}




Liste *initialisation()
{
	Liste *liste = malloc(sizeof(*liste));
	Element *element = malloc(sizeof(*element));

	if(liste == NULL || element == NULL)
	{
		exit(EXIT_FAILURE);
	}

	element->chaine = (char) 0;
	element->suivant = NULL;
	liste->premier = element;


	return liste;
}



void insertion(Liste *liste, char* nvChaine)
{
	Element *nouveau = malloc(sizeof(*nouveau));
	if(liste == NULL || nouveau == NULL)
	{
		exit(EXIT_FAILURE);
	}
	nouveau->chaine = nvChaine;

	nouveau->suivant = liste->premier;
	liste->premier = nouveau;
}


void suppression(Liste *liste)
{
	if(liste == NULL)
	{
		exit(EXIT_FAILURE);
	}

	if(liste->premier != NULL)
	{
		Element *aSupprimer = liste->premier;
		liste->premier = liste->premier->suivant;
		free(aSupprimer);
	}
}




bool contientElementRec(Element *element, char* chaine)
{
	if(element == NULL)
	{
		return false;
	}else{
		if(strcmp(element->chaine,chaine) != 0)
		{
			return true;
		}
		else
		{
			return contientElementRec(element->suivant,chaine);
		}
	}
	
	
}

bool contientElement(Liste *liste, char* chaine)
{
	if(liste == NULL){
		exit(EXIT_FAILURE);
	}

	Element *actuel = liste->premier;

	while(actuel != NULL && actuel->chaine != NULL)
	{
		if(strcmp(actuel->chaine,chaine) == 0)
		{
			return true;
		}
		actuel = actuel->suivant;
	}

	return false;
}




void afficherListe(Liste *liste)
{
	if(liste == NULL){
		exit(EXIT_FAILURE);
	}

	Element *actuel = liste->premier;

	while(actuel != NULL)
	{
		printf("%s -> ",actuel->chaine);
		actuel = actuel->suivant;
	}

	printf("NULL\n");
}

int tailleListe(Liste *liste)
{
	int taille;
	if(liste == NULL){
		return 0;
	}

	Element *actuel = liste->premier;

	taille = 0;
	while(actuel != NULL)
	{
		actuel = actuel->suivant;
		taille++;
	}

	return taille-1;
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
  //int taille_Hachage = 1.5*pow(2,19); // Le plus opti en temps mais pas en espace
  char str[MAXCHAR];
  FILE *fp;
  FILE *fp2;

  int hKey;

  Liste *t[taille_Hachage];

  int i;
  int collision;
  int nbMots;
  int nbMotsCommuns;
  int nbMotsFile;

  if (argc < 3) {
      fprintf(stderr, "%s <fichier1> <fichier2>\n", argv[0]);
      return EXIT_FAILURE;
  }







  for(i = 0; i < taille_Hachage; i++)
  {
	  t[i] = initialisation();
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
		// Nettoyage de la chaine de caractère
		remove_char_from_string('\n',str);
		remove_char_from_string('\r',str);
    	hKey = theodon_bon_hachage(str,taille_Hachage);
    	nbMots++;

    	insertion(t[hKey],string_copy(str));
    	if(tailleListe(t[hKey])>1)
    	{
    		collision++;
    	}
    	i++;
    }

    fclose(fp);




	fp2 = fopen(argv[2], "r");

    if (fp2 == NULL)
    {
        printf("Impossible d'ouvrir %s",argv[2]);
        return EXIT_FAILURE;
    }
    i = 0;
    nbMotsCommuns = 0;
    nbMotsFile=0;
    while (fgets(str, MAXCHAR, fp2) != NULL)
	{
		remove_char_from_string('\n',str);
		remove_char_from_string('\r',str);
    	hKey = theodon_bon_hachage(str,taille_Hachage);
    	nbMotsFile++;

    	if(contientElement(t[hKey],str))
    	{
    		nbMotsCommuns++;
    		printf("%s\n", str);
    	}
    }


    if (argc > 3 && *argv[3] == '1') {
        printf("Nombre de mots en commun: %d\n", nbMotsCommuns);
    	printf("Nombre de mots absents du fichier de référence: %d\n", nbMotsFile-nbMotsCommuns);
    	printf("Nombre de mots dans le fichier %s: %d\n", argv[2],nbMotsFile);
    	printf("Nombre de collisions dans la table de hachage: %d\n", collision);
    	printf("Taux de collisions dans la table de hachage : %f%%\n", (float)100*collision/nbMots);
    }

	return 0;
}
