enzo.demeulenaere.etu@univ-lille.fr

### Réalisé
 
Tout a été fait et cela fonctionne, certains visuels peuvent être retrouvés dans les snapshots

### Difficultés rencontrées

Aucune grande difficulté outre le fait de bien comprendre les concepts

### Réponses aux questions éventuelles du sujet :

- Question 8 

Un VBO a pour rôle de garder en mêmoire nos données

Un VAO a pour rôle de relier les VBO et les attributs du vertex shader

Un Program Shader a pour rôle de définir la position de chaque sommet ainsi que la couleur de chaque pixel

- Question 13 

Il faudrait donner les sommets dans l'ordre suivant : 0,3,2,5,1,4

- Question 15

Nous remarquons que les couleurs ne sont pas placées aux mêmes endroits, en réalité elles ont été associées aux sommets dans l'ordre d'apparition dans le VBO, l'association des couleurs n'a pas pris en compte les indices de GL_ELEMENT_ARRAY_BUFFER qui ne s'applique que sur la position des sommets

- Question 16 

La couleur rouge a disparu en raison du fait qu'elle était associé au somme d'indice 5 qui depuis n'existe plus, on utilise dès lors le sommet 2 qui a pour couleur du vert