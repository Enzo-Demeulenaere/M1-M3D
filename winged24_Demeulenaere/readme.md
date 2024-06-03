enzo.demeulenaere.etu@univ-lille.fr

### Réalisé
 
Tout a été fait et fonctionne, comprenant également la question Bonus

### Difficultés rencontrées

Aucune

### Réponses aux questions éventuelles du sujet :

Question 3:

Le cas du cube semble présenter quelques particularités, en effet lorsqu'on pointe la source lumineuse vers un sommet, environ dans la direction de sa normale, une ou 2 faces alentours (dépendant du sommet éclairé) semble avoir un éclairage incorrect où la moitié de la face est totalement assombrie 

Question 4:

Dans le TP, je remarque que rien ne fait référence au bouton "Boundaries" présent dans la fenêtre, et il semblerait que ce bouton log un nombre de boundaries valant 0 ppour le cube et la vache, en revanche ce nombre vaut 32 pour le bonhomme sur lequel nous observons désormais des arrêtes déssinées en rouge au niveau des chevilles.

Ces arrêtes "boundaries" représentent probablement les arrêtes du bord qui n'appartiennent qu'à une seule face.

Question 5:

Une première implémentation me permets d'afficher le bonhomme ave des normales, cependant je sais que certaines normales pourraient ne pas être calculées, c'est le cas lorsque l'on teste, on remarque des parties assombries vers les chevilles du bonhomme

Dans la seconde implémentation, si on trouve une bordure, on reprends de l'edge de départ mais l'on parcours les faces et edges "dans l'autre sens" et on arrete une fois l'autre bordure trouvée. Cela nous permets d'avoir des normales cohérentes pour tout les points au niveau des chevilles. Notre bonhomme apparait donc parfaitement 