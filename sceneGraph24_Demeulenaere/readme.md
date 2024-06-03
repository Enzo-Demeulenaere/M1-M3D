enzo.demeulenaere.etu@univ-lille.fr

### Réalisé
 
Tout a été fait et cela fonctionne

### Difficultés rencontrées

Il m'a fallu un peu de temps pour comprendre la derniere question avec le placement de la caméra

### Réponses aux questions éventuelles du sujet :

Exercice 6:

Afin de placer la caméra correctement, il faut comprendre que celle-ci a des coordonnées par rapport au world (Mworld->eye) et que la modelview représente Meye->local. Il nous faut décomposer Meye->local en Meye->world,Mworld->local, avec Mworld->local étant le graphe de scène prédéfini. Il suffit alors de donner à la modelview la transformation Meye->world qui correspond à "l'inverse" de Mworld->eye, valant une translation T suivi d'une rotation R. Il nous reste à appliquer d'abord la rotation inverse R-1  puis la translation inverse T-1, d'où l'application de `camera_.orientation.inverse()` puis de `-camera_position`