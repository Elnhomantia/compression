#include "bases.h"
#include "bitstream.h"
#include "exception.h"
#include "image.h"
#include "intstream.h"
#include "matrice.h"
#include "rle.h"
#include "sf.h"
#include "ondelette.h"

/*
 * Cette fonction effectue UNE SEULE itération d'une ondelette 1D
 * Voici quelques exemples de calculs
 *
 * Entree            Sortie
 * A                   A
 * A B               (A+B)/2 (A-B)/2
 * A B C             (A+B)/2    C    (A-B)/2
 * A B C D           (A+B)/2 (C+D)/2 (A-B)/2 (C-D)/2
 * A B C D E         (A+B)/2 (C+D)/2   E     (A-B)/2 (C-D)/2
 * A B C D E F       (A+B)/2 (C+D)/2 (E+F)/2 (A-B)/2 (C-D)/2 (E-F)/2
 */

void ondelette_1d(const float *entree, float *sortie, int nbe) {

  int milieu = nbe / 2 + (nbe & 1);
  if (nbe == 1) {
    *sortie = *entree;
  } else {
    for (int i = 0; i < nbe; i++) {
      if (i < nbe / 2) {
        sortie[i] = (entree[i * 2] + entree[i * 2 + 1]) / 2.;
      } else if (i == nbe / 2 && nbe & 1) {
        sortie[i] = entree[nbe - 1];
      } else {
        sortie[i] =
            (entree[(i - milieu) * 2] - entree[(i - milieu) * 2 + 1]) / 2.;
      }
    }
  }
}

/*
 * Comme pour la DCT, on applique dans un sens puis dans l'autre.
 *
 * La fonction reçoit "image" et la modifie directement.
 *
 * Vous pouvez allouer plusieurs images intermédiaires pour
 * simplifier la fonction.
 *
 * Par exemple pour une image  3x6
 *    3x6 ondelette horizontale
 *    On transpose, on a donc une image 6x3
 *    6x3 ondelette horizontale
 *    On transpose à nouveau, on a une image 3x6
 *    On ne travaille plus que sur les basses fréquences (moyennes)
 *    On ne travaille donc que sur le haut gauche de l'image de taille 2x3
 *
 * On recommence :
 *    2x3 horizontal
 *    transposee => 3x2
 *    3x2 horizontal
 *    transposee => 2x3
 *    basse fréquences => 1x2
 *
 * On recommence :
 *    1x2 horizontal
 *    transposee => 2x1
 *    2x1 horizontal (ne fait rien)
 *    transposee => 1x2
 *    basse fréquences => 1x1
 *
 * L'image finale ne contient qu'un seul pixel de basse fréquence.
 * Les autres sont des blocs de plus ou moins haute fréquence.
 * Sur une image 8x8 :
 *
 * M   	F1H  F2H  F2H  F3H  F3H  F3H  F3H
 * F1V 	F1HV F2H  F2H  F3H  F3H  F3H  F3H
 * F2V 	F2V  F2HV F2HV F3H  F3H  F3H  F3H
 * F2V 	F2V  F2HV F2HV F3H  F3H  F3H  F3H
 * F3V 	F3V  F3V  F3V  F3HV F3HV F3HV F3HV
 * F3V 	F3V  F3V  F3V  F3HV F3HV F3HV F3HV
 * F3V 	F3V  F3V  F3V  F3HV F3HV F3HV F3HV
 * F3V 	F3V  F3V  F3V  F3HV F3HV F3HV F3HV
 *
 * La fréquence F2 est plus petite (moins haute) que la fréquence F3
 * F1H  Indique que c'est une fréquence horizontale
 * F1V  Indique que c'est une fréquence verticale
 * F1HV Indique que c'est une fréquence calculée dans les 2 directions
 *
 */

void ondelette_2d(Matrice *image) {
  int height = image->height;
  int width = image->width;

  Matrice *tmp1 = allocation_matrice_float(image->height, image->width);
  Matrice *tmp2 = allocation_matrice_float(image->width, image->height);
  Matrice *tmp3 = allocation_matrice_float(image->width, image->height);

  while (height != 1 || width != 1) {

    for (int j = 0; j < height; j++) {
      ondelette_1d(image->t[j], tmp1->t[j], width);
    }

    transposition_matrice_partielle(tmp1, tmp2, height, width);

    for (int j = 0; j < width; j++) {
      ondelette_1d(tmp2->t[j], tmp3->t[j], height);
    }

    transposition_matrice_partielle(tmp3, image, width, height);

    height = (height + 1) / 2;
    width = (width + 1) / 2;
  }
  liberation_matrice_float(tmp1);
  liberation_matrice_float(tmp2);
  liberation_matrice_float(tmp3);
}

/*
 * Quantification de l'ondelette.
 * La facteur de qualité initial s'applique à la fréquence la plus haute.
 * Quand on divise la fréquence par 2 on divise qualité par 8
 * tout en restant supérieur à 1.
 * Une qualité de 1 indique que l'on a pas de pertes.
 */

void quantif_ondelette(Matrice *image, float qualite) {
  int height = image->height;
  int width = image->width;
  float q = qualite;

  if (q < 1.0f)
    q = 1.0f;

  while (height != 1 || width != 1) {
    int mid_h = (height + 1) / 2;
    int mid_l = (width + 1) / 2;

    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        if (j >= mid_h || i >= mid_l) {
          image->t[j][i] = roundf(image->t[j][i] / q);
        }
      }
    }

    q /= 8.0f;
    if (q < 1.0f) {
      q = 1.0f;
    }

    height = mid_h;
    width = mid_l;
  }

  image->t[0][0] = roundf(image->t[0][0] / q);
}

/*
 * Sortie des coefficients dans le bonne ordre afin
 * d'être bien compressé par la RLE.
 * Cette fonction n'est pas optimale, elle devrait faire
 * un parcours de Péano sur chacun des blocs.
 */

void codage_ondelette(Matrice *image, FILE *f) {
  int j, i;
  float *t, *pt;
  struct intstream *entier, *entier_signe;
  struct bitstream *bs;
  struct shannon_fano *sf;
  int hau, lar;

  /*
   * Conversion de la matrice en une table linéaire
   * Pour pouvoir utiliser la fonction "compresse"
   */
  hau = image->height;
  lar = image->width;
  ALLOUER(t, hau * lar);
  pt = t;

  while (hau != 1 || lar != 1) {
    for (j = 0; j < hau; j++)
      for (i = 0; i < lar; i++)
        if (j >= (hau + 1) / 2 || i >= (lar + 1) / 2)
          *pt++ = image->t[j][i];

    hau = (hau + 1) / 2;
    lar = (lar + 1) / 2;
  }
  *pt = image->t[0][0];
  /*
   * Compression RLE avec Shannon-Fano
   */
  bs = open_bitstream("-", "w");
  sf = open_shannon_fano();
  entier = open_intstream(bs, Shannon_fano, sf);
  entier_signe = open_intstream(bs, Shannon_fano, sf);

  compresse(entier, entier_signe, image->height * image->width, t);

  close_intstream(entier);
  close_intstream(entier_signe);
  close_bitstream(bs);
  free(t);
}

/*
*******************************************************************************
* Fonctions inverses
*******************************************************************************
*/

void ondelette_1d_inverse(const float *entree, float *sortie, int nbe) {
  if (nbe == 1) {
    *sortie = *entree;
    return;
  }

  int mid = nbe / 2;
  int milieu = mid + (nbe & 1);

  for (int k = 0; k < mid; k++) {
    float moy = entree[k];
    float diff = entree[milieu + k];
    sortie[2 * k] = moy + diff;
    sortie[2 * k + 1] = moy - diff;
  }

  if (nbe & 1) {
    sortie[nbe - 1] = entree[mid];
  }
}

void ondelette_2d_inverse(Matrice *image) {
  int height = image->height;
  int width = image->width;
  int dimsH[32], dimsW[32];
  int niveau = 0;
  dimsH[0] = height;
  dimsW[0] = width;
  while (dimsH[niveau] > 1 || dimsW[niveau] > 1) {
    dimsH[niveau + 1] = (dimsH[niveau] + 1) / 2;
    dimsW[niveau + 1] = (dimsW[niveau] + 1) / 2;
    niveau++;
  }

  Matrice *tmp1 = allocation_matrice_float(height, width);
  Matrice *tmp2 = allocation_matrice_float(width, height);
  Matrice *tmp3 = allocation_matrice_float(width, height);


  for (int lvl = niveau - 1; lvl >= 0; lvl--) {
    int bigH = dimsH[lvl];
    int bigW = dimsW[lvl];

    transposition_matrice_partielle(image, tmp3, bigH, bigW);

    for (int j = 0; j < bigW; j++) {
      ondelette_1d_inverse(tmp3->t[j], tmp2->t[j], bigH);
    }
    transposition_matrice_partielle(tmp2, tmp1, bigW, bigH);
    for (int i = 0; i < bigH; i++) {
      ondelette_1d_inverse(tmp1->t[i], image->t[i], bigW);
    }
  }

  liberation_matrice_float(tmp1);
  liberation_matrice_float(tmp2);
  liberation_matrice_float(tmp3);
}

void dequantif_ondelette(Matrice *image, float qualite) {
  int height = image->height;
  int width = image->width;
  float q = qualite;

  if (q < 1.0f)
    q = 1.0f;


  while (height != 1 || width != 1) {
    int mid_h = (height + 1) / 2;
    int mid_l = (width + 1) / 2;

    for (int j = 0; j < height; j++) {
      for (int i = 0; i < width; i++) {
        if (j >= mid_h || i >= mid_l) {
          image->t[j][i] = image->t[j][i] * q;
        }
      }
    }

    /* réduire la qualité pour la prochaine octave (division par 8), en restant
     * >= 1 */
    q /= 8.0f;
    if (q < 1.0f){
      q = 1.0f;
    }

    height = mid_h;
    width = mid_l;
  }

  /* restaurer la composante basse-fréquence finale */
  image->t[0][0] = image->t[0][0] * q;
}

void decodage_ondelette(Matrice *image, FILE *f) {
  int j, i;
  float *t, *pt;
  struct intstream *entier, *entier_signe;
  struct bitstream *bs;
  struct shannon_fano *sf;
  int largeur = image->width, hauteur = image->height;

  /*
   * Decompression RLE avec Shannon-Fano
   */
  ALLOUER(t, hauteur * largeur);
  bs = open_bitstream("-", "r");
  sf = open_shannon_fano();
  entier = open_intstream(bs, Shannon_fano, sf);
  entier_signe = open_intstream(bs, Shannon_fano, sf);

  decompresse(entier, entier_signe, hauteur * largeur, t);

  close_intstream(entier);
  close_intstream(entier_signe);
  close_bitstream(bs);

  /*
   * Met dans la matrice
   */
  pt = t;
  while (hauteur != 1 || largeur != 1) {
    for (j = 0; j < hauteur; j++)
      for (i = 0; i < largeur; i++)
        if (j >= (hauteur + 1) / 2 || i >= (largeur + 1) / 2)
          image->t[j][i] = *pt++;

    hauteur = (hauteur + 1) / 2;
    largeur = (largeur + 1) / 2;
  }
  image->t[0][0] = *pt++;

  free(t);
}

/*
 * Programme de test.
 * La ligne suivante compile, compresse et décompresse l'image
 * et affiche la taille compressée.

export QUALITE=1  # Qualité de "quantification"
export SHANNON=1  # Si 1, utilise shannon-fano dynamique
ondelette <DONNEES/bat710.pgm 1 >xxx && ls -ls xxx && ondelette_inv <xxx | xv -

 */

void ondelette_encode_image(float qualite) {
  struct image *image;
  Matrice *im;
  int i, j;

  image = lecture_image(stdin);
  assert(fwrite(&image->hauteur, 1, sizeof(image->hauteur), stdout) ==
         sizeof(image->hauteur));
  assert(fwrite(&image->largeur, 1, sizeof(image->largeur), stdout) ==
         sizeof(image->largeur));
  assert(fwrite(&qualite, 1, sizeof(qualite), stdout) == sizeof(qualite));

  im = allocation_matrice_float(image->hauteur, image->largeur);
  for (j = 0; j < image->hauteur; j++)
    for (i = 0; i < image->largeur; i++)
      im->t[j][i] = image->pixels[j][i];

  fprintf(stderr, "Compression ondelette, image %dx%d\n", image->largeur,
          image->hauteur);
  ondelette_2d(im);
  fprintf(stderr, "Quantification qualité = %g\n", qualite);
  quantif_ondelette(im, qualite);
  fprintf(stderr, "Codage\n");
  codage_ondelette(im, stdout);

  //  affiche_matrice_float(im, image->hauteur, image->largeur) ;
}

void ondelette_decode_image() {
  int hauteur, largeur;
  float qualite;
  struct image *image;
  Matrice *im;

  assert(fread(&hauteur, 1, sizeof(hauteur), stdin) == sizeof(hauteur));
  assert(fread(&largeur, 1, sizeof(largeur), stdin) == sizeof(largeur));
  assert(fread(&qualite, 1, sizeof(qualite), stdin) == sizeof(qualite));

  im = allocation_matrice_float(hauteur, largeur);

  fprintf(stderr, "Décodage\n");
  decodage_ondelette(im, stdin);

  fprintf(stderr, "Déquantification qualité = %g\n", qualite);
  dequantif_ondelette(im, qualite);

  fprintf(stderr, "Décompression ondelette, image %dx%d\n", largeur, hauteur);
  ondelette_2d_inverse(im);

  //  affiche_matrice_float(im, hauteur, largeur) ;
  image = creation_image_a_partir_de_matrice_float(im);
  ecriture_image(stdout, image);
}
