enzo.demeulenaere.etu@univ-lille.fr

### Réalisé
 
Tout a été fait et cela fonctionne, cependant pour la dernière question je n'ai pas exactement le même résultat que sur la vidéo du TP, il semblerait que d'autres camarades aient le même résultat

### Difficultés rencontrées

Difficulté à comprendre la dernière partie

### Réponses aux questions éventuelles du sujet :

**Partie 2**

**Question 2**

V: coordoonées de l'observateur (dans le repère local)
L: coordoonées de la source de lumière (repère local)
N: normale du point


**Question 4**

L'ensemble parait plus éclairé qu'avec la technique du vertexLighting, on remarque surtout l'apparition de halos lumineux au sol et sur la sphère.

L'aspect plus sombre du sol peut provenir du fait que le sol peut-être dessiné avec 4 sommets très éloignés l'un de l'autre et plus éloignés de la source lumineuse qui ont donc un éclairage plus faible appliqué sur chaque pixel

**Partie 3**

**Question 1**

Nous appliquons à fTexCoord les coordonnées retenues dans gl_Position qui sont des coordonnées qui dépendent de la modelView et donc du repère de l'oeil d'où le fait que la texture suit la caméra.

De plus ces coordonnées sont reportées dans le repère [-1,1] de l'oeil alors que la texture a des coordonnées dans [0,1] expliquant ainsi sa position dans le coin supérieur droit

La texture n'apparait pas dans le fond de la scène en raison du fait qu'on l'applique sur des sommets et que le fond de la scène n'en contient pas.

**Question 2**

L'option GL_REPEAT fait en sorte que la texture se repete en recuperant la partie entière de chaque coordonnées dans l'espace [-1,1] et en la reportant dans l'espace [0,1]

**Question 4**

En rapportant correctement les coordonnées dans l'intervalle [0,1], on fait en sorte que notre texture s'applique selon le bon repère qui correspond à l'entièreté de l'écran

**Question 5**

1. 

Utiliser l'attribut position pour appliquer la texture assigne à chaque vertex une coordonnée de texture à sa création mais ceci n'est pas calculé à nouveau lorsque les vertex se déplacent durant l'animation d'où le fait que les textures soient "collées" aux objets

2.

Ici on semble "projeter" la texture depuis les coordoonnées de l'oeil, constituant ainsi une piste interessante pour la projection de diapositives

**Exercice 2 Question 5**

Les objets apparaissent également sur le sol car les textures "traversent" les objets, en effet on projette la texture sur tout point dans la direction du projecteur, y compris les points "derrière" d'autres points.
Le nez de l'avion parait noir dans l'exemple en raison du fait qu'il n'est pas dans le champ de vision du projecteur et il n'est par conséquent par tout à fait éclairé

**Question 9**

texCoord.z correspond à la profondeur du sommet dans l'image dans le repère local, alors que fragColor.r correspond à la "luminance" du pixel, ces valeurs de profondeur existent bien dans l'intervalle [0,1]

