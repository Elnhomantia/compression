/*
 * Créé le Wed Jan  9 16:23:10 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_MATRICE_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_MATRICE_H

#include "bases.h"

typedef struct {
  int width, height ;
  float **t ;
} Matrice ;

Matrice* allocation_matrice_float(int height, int width) ;
void liberation_matrice_float(Matrice*) ;

/*
 * Fonctions gracieusement fournies
 */

void produit_matrices_float(const Matrice *a, const Matrice *b, Matrice *resultat) ; /**/
void transposition_matrice(const Matrice *a, Matrice *resultat) ; /**/
void transposition_matrice_partielle(const Matrice *a, Matrice *resultat, int width, int height) ; /**/
void produit_matrice_vecteur(const Matrice *a, const float *v, float *resultat) ; /**/
void affiche_matrice(const Matrice *a, FILE *f) ; /**/

struct image* creation_image_a_partir_de_matrice_float(const Matrice *m) ; /**/

#endif
