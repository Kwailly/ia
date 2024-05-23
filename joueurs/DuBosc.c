#include <stdio.h>
#include <stdlib.h>
#include "../include/avalam.h"
#include "../include/moteur.h"

// Fonction testIso : vérifie si une case est isolée
int testIso(T_Position currentPosition, T_Voisins vd, octet o) {
    for (int k = 0; k < vd.nb; k++) {
        if (currentPosition.cols[vd.cases[k]].nb != 0 && vd.cases[k] != o) {
            printf("0");
            return 0; // La case n'est pas isolée
        }
    }
    printf("1");
    return 1; // La case est isolée
}

// Fonction choisirCoup : sélectionne le meilleur coup à jouer
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
    int i;
    int x = 0;
    octet o, d;
    octet myColor = currentPosition.trait; // Couleur du joueur courant
    float points[500] = {0}; // Tableau des points pour chaque coup possible
    ecrireIndexCoup(0); // Coup par défaut

    // Vérifie les conditions spéciales de bonus/malus pour les joueurs
    if (myColor == 1) { 
        if (currentPosition.evolution.bonusJ == 255) {
            ecrireIndexCoup(12); // Coup spécial pour bonus joueur jaune
            return;
        }
        if (currentPosition.evolution.malusJ == 255) {
            ecrireIndexCoup(9); // Coup spécial pour malus joueur jaune
            return;
        }
    } else {
        if (currentPosition.evolution.bonusR == 255) {
            ecrireIndexCoup(13); // Coup spécial pour bonus joueur rouge
            return;
        }
        if (currentPosition.evolution.malusR == 255) {
            ecrireIndexCoup(15); // Coup spécial pour malus joueur rouge
            return;
        }
    }

    // Parcours de la liste des coups possibles
    for (i = 0; i < listeCoups.nb; i++) {
        o = listeCoups.coups[i].origine; // Case d'origine du coup
        d = listeCoups.coups[i].destination; // Case de destination du coup

        T_Voisins vd = getVoisins(d); // Voisins de la case de destination
        T_Voisins vo = getVoisins(o); // Voisins de la case d'origine

        // Vérifie si le coup permet de prendre une tour de 5 pions
        if (((currentPosition.cols[o].couleur == myColor) &&
             (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5)) ||
            (testIso(currentPosition, vd, o) == 1 && currentPosition.cols[o].couleur == myColor)) {
            
            points[i] += 2; // Ajoute des points si le coup est avantageux

            // Vérifie si le coup concerne une tour avec bonus
            if (((o == currentPosition.evolution.bonusJ) || (o == currentPosition.evolution.bonusR)) &&
                (currentPosition.cols[o].couleur == myColor)) {
                points[i] += 2;
            }

            if (((((o == currentPosition.evolution.bonusJ) || (o == currentPosition.evolution.bonusR)) &&
                  ((d == currentPosition.evolution.bonusJ) || (d == currentPosition.evolution.bonusR))) || 
                 ((o == currentPosition.evolution.bonusJ) && (o == currentPosition.evolution.bonusR)) ||
                 ((d == currentPosition.evolution.bonusJ) && (d == currentPosition.evolution.bonusR))) &&
                (currentPosition.cols[o].couleur == myColor)) {
                points[i] += 2;
            }

            // Vérifie si le coup concerne une tour avec malus
            if (((o == currentPosition.evolution.malusJ) || (o == currentPosition.evolution.malusR)) &&
                (currentPosition.cols[o].couleur == myColor)) {
                points[i] -= 2;
            }

            if (((((o == currentPosition.evolution.malusJ) || (o == currentPosition.evolution.malusR)) &&
                  ((d == currentPosition.evolution.malusJ) || (d == currentPosition.evolution.malusR))) || 
                 ((o == currentPosition.evolution.malusJ) && (o == currentPosition.evolution.malusR)) ||
                 ((d == currentPosition.evolution.malusJ) && (d == currentPosition.evolution.malusR))) &&
                (currentPosition.cols[o].couleur == myColor)) {
                points[i] -= 2;
            }

            if (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5) points[i] += 1;
        }

        // Vérifie les autres conditions pour ajuster les points
        if (currentPosition.cols[o].couleur == myColor) {
            for (int l = 0; l < vd.nb; l++) {
                if (currentPosition.cols[vd.cases[i]].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) {
                    x = 0;
                    break;
                }

                if (currentPosition.cols[vd.cases[i]].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb == 4) {
                    for (int p = 0; p < vd.nb; p++) {
                        if (currentPosition.cols[vd.cases[p]].couleur == myColor && currentPosition.cols[vd.cases[p]].nb == 1 && vd.cases[p] != o) {
                            x = 1;
                            break;
                        }
                    }
                }

                if (currentPosition.cols[vd.cases[i]].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb == 3) {
                    for (int m = 0; m < vd.nb; m++) {
                        if (currentPosition.cols[vd.cases[m]].couleur == myColor && currentPosition.cols[vd.cases[m]].nb == 2) {
                            x = 1;
                            break;
                        }

                        if (currentPosition.cols[vd.cases[m]].couleur == myColor && currentPosition.cols[vd.cases[m]].nb == 1 && vd.cases[m] != o) {
                            x = 1;
                            for (int n = 0; n < vd.nb; n++) {
                                if (currentPosition.cols[vd.cases[n]].couleur != myColor && currentPosition.cols[vd.cases[n]].nb == 1) {
                                    x = 0;
                                    break;
                                }
                            }
                            if (x == 0) break;
                        }
                    }
                }
            }

            if (x == 0) {
                points[i] -= 2;
                if (((o == currentPosition.evolution.bonusJ) || (o == currentPosition.evolution.bonusR)) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] -= 2;
                }

                if (((((o == currentPosition.evolution.bonusJ) || (o == currentPosition.evolution.bonusR)) &&
                      ((d == currentPosition.evolution.bonusJ) || (d == currentPosition.evolution.bonusR))) || 
                     ((o == currentPosition.evolution.bonusJ) && (o == currentPosition.evolution.bonusR)) ||
                     ((d == currentPosition.evolution.bonusJ) && (d == currentPosition.evolution.bonusR))) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] -= 2;
                }

                if (((o == currentPosition.evolution.malusJ) || (o == currentPosition.evolution.malusR)) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] += 2;
                }

                if (((((o == currentPosition.evolution.malusJ) || (o == currentPosition.evolution.malusR)) &&
                      ((d == currentPosition.evolution.malusJ) || (d == currentPosition.evolution.malusR))) || 
                     ((o == currentPosition.evolution.malusJ) && (o == currentPosition.evolution.malusR)) ||
                     ((d == currentPosition.evolution.malusJ) && (d == currentPosition.evolution.malusR))) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] += 2;
                }
            }
        } else {
            if (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) {
                x = 0;
            }
            for (int q = 0; q < vd.nb; q++) {
                if (currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[vd.cases[q]].nb == 5) {
                    x = 0;
                    break;
                }
            }
            for (int r = 0; r < vd.nb; r++) {
                if (currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[vd.cases[r]].nb < 5 && currentPosition.cols[vd.cases[r]].couleur == myColor) {
                    x = 1;
                    break;
                }
            }

            if (x == 0) {
                points[i] -= 2;
                if (((o == currentPosition.evolution.bonusJ) || (o == currentPosition.evolution.bonusR)) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] -= 2;
                }

                if (((((o == currentPosition.evolution.bonusJ) || (o == currentPosition.evolution.bonusR)) &&
                      ((d == currentPosition.evolution.bonusJ) || (d == currentPosition.evolution.bonusR))) || 
                     ((o == currentPosition.evolution.bonusJ) && (o == currentPosition.evolution.bonusR)) ||
                     ((d == currentPosition.evolution.bonusJ) && (d == currentPosition.evolution.bonusR))) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] -= 2;
                }

                if (((o == currentPosition.evolution.malusJ) || (o == currentPosition.evolution.malusR)) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] += 2;
                }

                if (((((o == currentPosition.evolution.malusJ) || (o == currentPosition.evolution.malusR)) &&
                      ((d == currentPosition.evolution.malusJ) || (d == currentPosition.evolution.malusR))) || 
                     ((o == currentPosition.evolution.malusJ) && (o == currentPosition.evolution.malusR)) ||
                     ((d == currentPosition.evolution.malusJ) && (d == currentPosition.evolution.malusR))) &&
                    (currentPosition.cols[o].couleur == myColor)) {
                    points[i] += 2;
                }
            }
        }

        // Regroupe les pions adverses pour désavantage
        if ((currentPosition.cols[o].couleur == currentPosition.cols[d].couleur) &&
            currentPosition.cols[d].couleur != myColor) {
            points[i] += 0.1;
        }

        if (currentPosition.cols[o].couleur == currentPosition.cols[d].couleur && currentPosition.cols[o].couleur == myColor) {
            points[i] -= 0.1;
        }
    }

    // Trouve le meilleur coup
    int position = 0;
    float max = -10; 

    for (int k = 0; k < i; k++) {
        if (points[k] > max) {
            max = points[k];
            position = k;
        }
    }

    // Joue le coup
    ecrireIndexCoup(position);
    return;
}
