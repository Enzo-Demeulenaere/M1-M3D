enzo.demeulenaere.etu@univ-lille.fr

### Réalisé
 
Tout a été fait (avec la question Bonus) et cela fonctionne, des snapshots ont été prises au cours du TP et peuvent être retrouvées dans le répo

### Difficultés rencontrées

Aucune grande difficulté, outre le fait de comprendre la question bonus et d'appliquer la matrice transform sur les positions et normales

### Réponses aux questions éventuelles du sujet :

- Question 7

Le résultat obtenu est donc la superposition de 4 triangles, un premier triangle rouge est dessiné, puis un triangle vert le surplombe, puis un bleu et enfin un cyan avec les triangles "pivotant" dans le sens trigonométrique avec chaque triangle recouvrant la moitié du précédent.

Ce que nous pouvons remarquer c'est que nous n'observons aucun pixel rouge puisque ce premier triangle est totalement recouvert par le second (vert) et le dernier (cyan) ainsi nous pourrions ne pas donner les indices du premier triangle et observer graphiquement le même résultat.

- Question 9

Nous n'avons plus aucun affichage mais cela s'explique par le fait que nous avons demandé de clear à la profondeur 1 qui est la profondeur maximale, puis nous donnons comme consigne avec le GL_GREATER de ne garder que la plus grande valeur de profondeur entre le pixel que nous essayons de dessiner et celui déjà en mémoire qui est un pixel blanc dans le fond, autrement dit nous demandons d'afficher les pixels les plus "profonds" et ceux-ci ont été défini au préalable comme des pixels blancs

En définissant le clearDepth à 0 nous "dessinons" des pixels blanc en profondeur 0 et ne decidons de garder que les pixels les plus "profonds" et ceux-ci correspondent aux pixels des faces vertes et cyan du tétrahedre

En définissant le clearDepth à 0.5 on "coupe" notre tétrahedre au "milieu" des face cyan et vertes (selon l'axe z) en effet celles ci sont définies par des points aux coordoonées Z valant soit 1 soit -1, une fois normalisées, ces coordonnées s'étalent entre 0 et 1 et le clear depth à 0.5 (avec le GL_GREATER) signifie que nous gardons que les pixels les plus profonds qui ont une profondeur supérieure à 0.5 or seule une partie des pixels des faces cyan et vertes verifient cette contrainte

