/*
 * Le but du shannon-fano dynamique est de ne pas transmettre la table
 * des occurrences. Pour ce faire, on ajoute à la table un symbole ESCAPE
 * qui permet l'ajout d'éléments à la table.
 * Le décompresseur sait qu'après un événement ESCAPE il trouvera
 * la valeur (et non le code) d'un événement à ajouter à la table.
 */

#include "sf.h"
#include "bases.h"
#include "bit.h"
#include "bits.h"
#include "bitstream.h"
#include <string.h>

#define VALEUR_ESCAPE 0x7fffffff /* Plus grand entier positif */

struct evenement {
  int valeur;
  int nb_occurrences;
};

struct shannon_fano {
  int nb_evenements;
  struct evenement evenements[200000];
};

/*
 * Allocation des la structure et remplissage des champs pour initialiser
 * le tableau des événements avec l'événement ESCAPE (avec une occurrence).
 */
struct shannon_fano *open_shannon_fano() {
  struct shannon_fano *fs = NULL;
  ALLOUER(fs, 1);
  fs->nb_evenements = 1;

  fs->evenements[0].nb_occurrences = 1;
  fs->evenements[0].valeur = VALEUR_ESCAPE;

  return fs;
}

/*
 * Fermeture (libération mémoire)
 */
void close_shannon_fano(struct shannon_fano *sf) { free(sf); }

/*
 * En entrée l'événement (sa valeur, pas son code shannon-fano).
 * En sortie la position de l'événement dans le tableau "evenements"
 * Si l'événement n'est pas trouvé, on retourne la position
 * de l'événement ESCAPE.
 */

static int trouve_position(const struct shannon_fano *sf, int evenement) {

  int i = 0;
  int pos_escape = 0;
  while (sf->nb_evenements > i) {
    if (sf->evenements[i].valeur == evenement) {
      return i;
    } else if (sf->evenements[i].valeur == VALEUR_ESCAPE) {
      pos_escape = i;
    }
    i++;
  }
  return pos_escape; /* pour enlever un warning du compilateur */
}

/*
 * Soit le sous-tableau "evenements[position_min..position_max]"
 * Les bornes sont incluses.
 *
 * LES FORTES OCCURRENCES SONT LES PETITS INDICES DU TABLEAU
 *
 * Il faut trouver la séparation.
 * La séparation sera codée comme l'indice le plus fort
 * des fortes occurrences.
 *
 * La séparation minimise la valeur absolue de la différence
 * de la somme des occurrences supérieures et inférieures.
 *
 * L'algorithme (trivial) n'est pas facile à trouver, réfléchissez bien.
 */
static int trouve_separation(const struct shannon_fano *sf, int position_min,
                             int position_max) {
  int res = -1;
  int somme = 0;

  for (int i = position_min; i <= position_max; i++) {
    somme += sf->evenements[i].nb_occurrences;
  }

  int medianne = somme / 2;

  somme = 0;

  for (int i = position_min; i <= position_max; i++) {
    somme += sf->evenements[i].nb_occurrences;
    if (somme > medianne) {
      // printf("\n OE : %d\n" ,abs(medianne - somme));
      res = (abs(medianne - somme) <=
             abs(medianne - (somme - sf->evenements[i].nb_occurrences)))
                ? i
                : i - 1;
      // printf("\n OE : %d\n" ,abs(medianne - (somme -
      // sf->evenements[i].nb_occurrences)));
      break;
    }
  }

  return res;
}

/*
 * Cette fonction (simplement itérative)
 * utilise "trouve_separation" pour générer les bons bit dans "bs"
 * le code de l'événement "sf->evenements[position]".
 */

static void encode_position(struct bitstream *bs, struct shannon_fano *sf,
                            int position) {
  int min = 0;
  int max = sf->nb_evenements - 1;

  int separation = -1;
  while (min != max) {

    separation = trouve_separation(sf, min, max);
    // Booleen a_droite = (separation < position) ? Vrai : Faux;
    put_bit(bs, (separation < position));
    
    if ((separation < position)) {
      min = separation + 1;
    } else {
      max = separation;
    }

    // eprintf("%d %d %d\n", min, max, separation);
  }
  // printf(" : %d encode %d\n", position, sf->evenements[position].valeur);
}

/*
 * Cette fonction incrémente le nombre d'occurrence de
 * "sf->evenements[position]"
 * Puis elle modifie le tableau pour qu'il reste trié par nombre
 * d'occurrence (un simple échange d'événement suffit)
 *
 * Les faibles indices correspondent aux grand nombres d'occurrences
 */

static void incremente_et_ordonne(struct shannon_fano *sf, int position) {
  sf->evenements[position].nb_occurrences++;

  if (position > 0) {
    while (sf->evenements[position].nb_occurrences >
           sf->evenements[position - 1].nb_occurrences) {

      struct evenement temp = sf->evenements[position - 1];
      sf->evenements[position - 1] = sf->evenements[position];
      sf->evenements[position] = temp;
      position--;
      if (position == 0) {
        return;
      }
    }
  }
}

/*
 * Cette fonction trouve la position de l'événement puis l'encode.
 * Si la position envoyée est celle de ESCAPE, elle fait un "put_bits"
 * de "evenement" pour envoyer le code du nouvel l'événement.
 * Elle termine en appelant "incremente_et_ordonne" pour l'événement envoyé.
 */
void put_entier_shannon_fano(struct bitstream *bs, struct shannon_fano *sf,
                             int evenement) {

  int pos = trouve_position(sf, evenement);
  encode_position(bs, sf, pos);

  if (sf->evenements[pos].valeur == VALEUR_ESCAPE) {

    put_bits(bs, sizeof(evenement) * 8, evenement);
    sf->evenements[sf->nb_evenements].valeur = evenement;
    sf->evenements[sf->nb_evenements].nb_occurrences = 1;
    sf->nb_evenements++;
  }
  incremente_et_ordonne(sf, pos);
}

/*
 * Fonction inverse de "encode_position"
 */
static int decode_position(struct bitstream *bs, struct shannon_fano *sf) {
  int min = 0;
  int max = sf->nb_evenements - 1;

  while (max != min) {
    int separation = trouve_separation(sf, min, max );
    int bit = get_bit(
        bs); /* 0 = gauche (<= separation-1), 1 = droite (>= separation+1) */
    
      if (bit) {
      /* On était à droite */
      min = separation + 1;
    } else {
      /* On était à gauche */
      max = separation;
    }
  }
 

  /* Quand la boucle s'arrête, min == position */
  return min ;
}

/*
 * Fonction inverse de "put_entier_shannon_fano"
 *
 * Attention au piège : "incremente_et_ordonne" change le tableau
 * donc l'événement trouvé peut changer de position.
 */
int get_entier_shannon_fano(struct bitstream *bs, struct shannon_fano *sf) {
  int pos = decode_position(bs, sf);
  int evenement = sf->evenements[pos].valeur;
  incremente_et_ordonne(sf, pos);

  if(evenement == VALEUR_ESCAPE){
    evenement = get_bits(bs, sizeof(int) * 8);
    sf->evenements[sf->nb_evenements].valeur = evenement;
    sf->evenements[sf->nb_evenements].nb_occurrences = 1;
    sf->nb_evenements++;
  }
  return evenement;
}

/*
 * Fonctions pour les tests, NE PAS MODIFIER, NE PAS UTILISER.
 */
int sf_get_nb_evenements(struct shannon_fano *sf) { return sf->nb_evenements; }
void sf_get_evenement(struct shannon_fano *sf, int i, int *valeur,
                      int *nb_occ) {
  *valeur = sf->evenements[i].valeur;
  *nb_occ = sf->evenements[i].nb_occurrences;
}
int sf_table_ok(const struct shannon_fano *sf) {
  int i, escape;

  escape = 0;
  for (i = 0; i < sf->nb_evenements; i++) {
    if (i != 0 && sf->evenements[i - 1].nb_occurrences <
                      sf->evenements[i].nb_occurrences) {
      fprintf(stderr, "La table des événements n'est pas triée\n");
      return (0);
    }
    if (sf->evenements[i].valeur == VALEUR_ESCAPE)
      escape = 1;
  }
  if (escape == 0) {
    fprintf(stderr, "Pas de ESCAPE dans la table !\n");
    return (0);
  }
  return 1;
}
