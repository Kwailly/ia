#include <stdio.h>
#include <stdlib.h>
#include "../include/avalam.h"
#include "../include/moteur.h"

int testIso(T_Position currentPosition, T_Voisins vd, octet o) {
    for (int i = 0; i < vd.nb; i++) {
        if (currentPosition.cols[vd.cases[i]].nb != 0 && vd.cases[i] != o) {
            printf("0");
            return 0;
        }
    }
    printf("1");
    return 1;
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
    int i;
    octet o, d;
    octet myColor = currentPosition.trait;
    float points[500] = {0};
    ecrireIndexCoup(0); // Coup par défaut

    if (myColor == 0) {
        if (currentPosition.evolution.bonusJ == 255) {
            ecrireIndexCoup(10);
            return;
        }
        if (currentPosition.evolution.malusJ == 255) {
            ecrireIndexCoup(9);
            return;
        }
    } else {
        if (currentPosition.evolution.bonusR == 255) {
            ecrireIndexCoup(13);
            return;
        }
        if (currentPosition.evolution.malusR == 255) {
            ecrireIndexCoup(16);
            return;
        }
    }

    // Parcours de la liste des coups possibles
    for (i = 0; i < listeCoups.nb; i++) {
        o = listeCoups.coups[i].origine;
        d = listeCoups.coups[i].destination;

        T_Voisins vd = getVoisins(d);
        T_Voisins vo = getVoisins(o);

        // Prendre une tour adverse de hauteur 4
        if (((currentPosition.cols[o].couleur == myColor) &&
             (currentPosition.cols[d].nb == 4) &&
             (currentPosition.cols[d].couleur != myColor)) ||
            (testIso(currentPosition, vd, o) == 1 && currentPosition.cols[o].couleur == myColor)) {
            points[i] += 2;
            // Prendre une tour avec bonus 1
            if ((o == currentPosition.evolution.bonusJ || o == currentPosition.evolution.bonusR) &&
                (currentPosition.cols[o].couleur == myColor)) {
                points[i] += 2;
            }

            // Prendre une tour avec bonus 2
            if (((o == currentPosition.evolution.bonusJ || o == currentPosition.evolution.bonusR) &&
                 (d == currentPosition.evolution.bonusJ || d == currentPosition.evolution.bonusR) ||
                 (o == currentPosition.evolution.bonusJ && o == currentPosition.evolution.bonusR) ||
                 (d == currentPosition.evolution.bonusJ && d == currentPosition.evolution.bonusR)) &&
                (currentPosition.cols[o].couleur == myColor)) {
                points[i] += 2;
            }
        }

        // Regrouper les pions adverses
        if ((currentPosition.cols[o].couleur == currentPosition.cols[d].couleur) &&
            currentPosition.cols[d].couleur != myColor) {
            points[i]++;
        }

        // Vérifier si la hauteur totale est égale à 5
        if ((currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 5 &&
            currentPosition.cols[o].couleur == myColor) {
            points[i] += 0.5;
        }
    }

    // Trouver le meilleur coup
    int position = 0;
    float max = -10;

    for (int k = 0; k < i; k++) {
        if (points[k] > max) {
            max = points[k];
            position = k;
        }
    }

    // Jouer le coup
    ecrireIndexCoup(position);
    return;
}
