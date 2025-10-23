#include "psycho.h"
#include "bases.h"

/*
 * Soit F1!=0 et F2!=0 deux ``fréquences'' quelconques du son avec F1!=F2
 *      A1 et A2 leurs amplitudes respectives.
 * La fréquence du son est l'indice dans le tableau "dct".
 *
 *
 * Si   C * abs(A1)   <   abs( A2 / (F2 - F1) )
 *   Alors Annuler A1
 *
 * Ne pas annuler la fréquence nulle.
 *
 *
 * Essayez aussi avec d'autre formules et comparer
 * les taux de compressions et la qualité auditive du résultat.
 *
 * La formule qui est donnée ici ne représente pas la réalité.
 * En fait ce sont des fonctions tabulées qui sont utilisées.
 * Ces fonctions tabulées sont déterminées expérimentalement.
 */

/*
 * Le tableau "dct" est directement modifié.
 * Il contient déjà les coefficients de la dct
 */

void psycho(int nbe, float *dct, float c) {
    
  for (int f1 = 1; f1 < nbe; f1++) {
    for (int f2 = 1; f2 < nbe; f2++) {
      if (f1 != f2) {
        if (c * fabs(dct[f1]) < fabs(dct[f2] / (f2 - f1))) {
          dct[f1] = 0.0;
          break;
        }
      }
    }
  }
}
