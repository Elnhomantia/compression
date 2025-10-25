#include "image.h"
#include "bit.h"
#include <cstring>

/*
 * Lecture d'une ligne du fichier.
 * On saute les lignes commençant par un "#" (commentaire)
 * On simplifie en considérant que les lignes ne dépassent pas MAXLIGNE
 */

void lire_ligne(FILE *f, char *ligne) {
  unsigned int tete_ecriture = 0;
  char data = 0;
  size_t nb_block_lue = 0;

  Booleen was_hastag_line = Faux;

  do {
    was_hastag_line = Faux;
    nb_block_lue = fread(&data, sizeof(data), 1, f);
    if (nb_block_lue == 0) {
      return;
    }
    if (data == '#') {
      was_hastag_line = Vrai;
      do {
        nb_block_lue = fread(&data, sizeof(data), 1, f);
      } while (data != '\n');
      continue;
    }
    ligne[tete_ecriture++] = data;

  } while (data != '\n' || was_hastag_line);

  //   eprintf("%s", ligne);
}

/*
 * Allocation d'une image
 */

struct image *allocation_image(int hauteur, int largeur) {
  struct image *im = NULL;

  ALLOUER(im, 1);

  im->hauteur = hauteur;
  im->largeur = largeur;

  ALLOUER(im->pixels, hauteur);
  for (int i = 0; i < hauteur; i++) {
    ALLOUER(im->pixels[i], largeur);
  }
  return im;
}

/*
 * Libération image
 */

void liberation_image(struct image *image) {
  for (int i = 0; i < image->hauteur; i++) {
    free(image->pixels[i]);
  }
  free(image->pixels);
  free(image);
}

/*
 * Allocation et lecture d'un image au format PGM.
 * (L'entête commence par "P5\nLargeur Hauteur\n255\n"
 * Avec des lignes de commentaire possibles avant la dernière.
 */

struct image *lecture_image(FILE *f) {
    // char ligne[MAXLIGNE];
    // lire_ligne(f, ligne);
    // lire_ligne(f, ligne);

    // unsigned int line_size = strlen(ligne);

    // for(int i = line_size - 1; i >= 0; )

    // char nb1[MAXLIGNE];

    // char nb2[MAXLIGNE];


//   return 0; /* pour enlever un warning du compilateur */
}

/*
 * Écriture de l'image (toujours au format PGM)
 */

void ecriture_image(FILE *f, const struct image *image) {}
