Table de hachage avec adressage fermé.

Compiler avec: gcc main.c -o main -lm

Exécuter avec: ./main txt1.txt txt2.txt

où txt1.txt et txt2.txt sont deux fichiers textes à comparer.

Argument supplémentaire: ./main word2.txt txt1.txt 1

Ajouter '1' comme troisième argument permet d'écrire quelques indicateurs en sortie. Par exemple:

Nombre de mots en commun: 33095
Nombre de mots absents du fichier de référence: 25015
Nombre de mots dans le fichier txt2.txt: 58110
Nombre de collisions dans la table de hachage: 37575
Taux de collisions dans la table de hachage : 15.929305%
