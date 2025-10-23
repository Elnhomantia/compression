#include "matrice.h"
#include "bases.h"
#include "image.h"

/*
 * Allocation d'une matrice carrée de float.
 * (tableau de pointeur sur tableau de flottants)
 */

Matrice *allocation_matrice_float(int height, int width) {
  Matrice *m = NULL;

  ALLOUER(m, 1);

  m->height = height;
  m->width = width;

  ALLOUER(m->t, height);
  for (int i = 0; i < height; i++) {
    ALLOUER(m->t[i], width);
  }
  return m;
}

/*
 * Libération
 */

void liberation_matrice_float(Matrice *m) {
  for (int i = 0; i < m->height; i++)
    free(m->t[i]);
  free(m->t);
  free(m);

}

/*
 * Produit matriciel de matrices carrées (le résultat est déjà alloué).
 *             resultat = a * b
 */

void produit_matrices_float(const Matrice *a, const Matrice *b,
                            Matrice *resultat) {
  int j, i, k;
  float s;

  assert(a->width == b->height);
  assert(a->width == b->width);
  assert(a->height == b->height);
  assert(a->width == resultat->width);
  assert(a->height == resultat->height);
  for (j = 0; j < a->height; j++)
    for (i = 0; i < a->width; i++) {
      s = 0;
      for (k = 0; k < a->width; k++)
        s += a->t[j][k] * b->t[k][i];
      resultat->t[j][i] = s;
    }
}

/*
 * Produit matrices carrée vecteur
 *             resultat = m * v
 * Le résultat est supposé annulé
 */

void produit_matrice_vecteur(const Matrice *a, const float *v,
                             float *resultat) {
  int j, i;
  float s;

  for (j = 0; j < a->height; j++) {
    s = 0;
    for (i = 0; i < a->width; i++)
      s += a->t[j][i] * v[i];
    resultat[j] = s;
  }
}

/*
 * Transposition d'une matrice carrée (le résultat est déjà alloué).
 *        a_t est la transposée de a
 */

void transposition_matrice_partielle(const Matrice *a, Matrice *resultat,
                                     int height, int width) {
  int i, j;

  assert(a->width == resultat->height);
  assert(a->height == resultat->width);
  for (j = 0; j < height; j++)
    for (i = 0; i < width; i++)
      resultat->t[i][j] = a->t[j][i];
}

void transposition_matrice(const Matrice *a, Matrice *resultat) {
  transposition_matrice_partielle(a, resultat, a->height, a->width);
}

/*
 * Affiche
 */

void affiche_matrice(const Matrice *a, FILE *f) {
  int i, j;

  for (j = 0; j < a->height; j++) {
    for (i = 0; i < a->width; i++)
      fprintf(f, " %8.4g", a->t[j][i]);
    fprintf(f, "\n");
  }
}

/*
 * Cela vous permettra d'écrire plus facilement
 * la matrice de flottant dans un fichier image
 */
struct image *creation_image_a_partir_de_matrice_float(const Matrice *m) {
  int j, i;
  struct image *image;

  image = allocation_image(m->height, m->width);

  for (j = 0; j < image->hauteur; j++)
    for (i = 0; i < image->largeur; i++) {
      if (m->t[j][i] > 255)
        image->pixels[j][i] = 255;
      else if (m->t[j][i] < 0)
        image->pixels[j][i] = 0;
      else
        image->pixels[j][i] = m->t[j][i];
    }

  return image;
}

/*
 * Affichage directe de la matrice de flottant sur l'écran
 * Attention, si vous affichez l'image après transformation ondelette
 * le résultat semblera très bruité car les valeurs négatives
 * vont être transformées en blancs.
 */
void affiche_matrice_image(const Matrice *m) {
  FILE *f;
  struct image *image;
  image = creation_image_a_partir_de_matrice_float(m);
  f = popen("xv -", "w");
  ecriture_image(f, image);
  pclose(f);
  liberation_image(image);
}
