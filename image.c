#include "image.h"
#include "bit.h"
#include <string.h>

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
    char ligne[MAXLIGNE];
    lire_ligne(f, ligne);
    if(strcmp( ligne, "P5\n") != 0)
    {
      //Erreur
    }
    lire_ligne(f, ligne);

    int largeur = atoi(strtok(ligne, " "));
    int hauteur = atoi(strtok(NULL, " "));
   // eprintf("%d, %d", largeur, hauteur);
    lire_ligne(f, ligne);
    

    struct image *im = allocation_image(hauteur, largeur);

    for(int i = 0; i < hauteur; i++)
    {
      for(int j = 0; j < largeur; j++)
      {
         fread(&im->pixels[i][j], sizeof(**im->pixels), 1, f);
      }
    }


  return im; /* pour enlever un warning du compilateur */
}

/*
 * Écriture de l'image (toujours au format PGM)
 */

void ecriture_image(FILE *f, const struct image *image) {
  fprintf(f, "P5\n");
  fprintf(f, "%d %d\n", image->largeur, image->hauteur);
  fprintf(f, "255\n");

  for(int i = 0; i < image->hauteur; i++)
  {
    for(int j = 0; j < image->largeur; j++)
    {
       fwrite(&image->pixels[i][j], sizeof(**image->pixels), 1, f);
    }
  }


}
