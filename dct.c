#include "bases.h"
#include "matrice.h"
#include <math.h>
#include "dct.h"

/*
 * La fonction calculant les coefficients de la DCT (et donc de l'inverse)
 * car la matrice de l'inverse DCT est la transposée de la matrice DCT
 *
 * Cette fonction prend beaucoup de temps.
 * il faut que VOUS l'utilisiez le moins possible (UNE SEULE FOIS)
 *
 * FAITES LES CALCULS EN "double"
 *
 * La valeur de Pi est : M_PI
 *
 * Pour ne pas avoir de problèmes dans la suite du TP, indice vos tableau
 * avec [j][i] et non [i][j].
 */

void coef_dct(Matrice *table)
{
	for(int j = 0; j <table->height; j++){
		for(int i = 0; i < table->width; i++){
			if(j == 0){
				table->t[j][i] = 1.0 / sqrt(table->width);
			} else {
				table->t[j][i] = (sqrt(2.0) / sqrt(table->width)) * cos( j * M_PI * ((2.0*i + 1.0)/(2.0 * table->width)) );
			}
		}
	}
}

/*
 * La fonction calculant la DCT ou son inverse.
 *
 * Cette fonction va être appelée très souvent pour faire
 * la DCT du son ou de l'image (nombreux paquets).
 */

void dct(int   inverse,		/* ==0: DCT, !=0 DCT inverse */
	 int nbe,		/* Nombre d'échantillons  */
	 const float *entree,	/* Le son avant transformation (DCT/INVDCT) */
	 float *sortie		/* Le son après transformation */
	 )
{
	Matrice *DCT = allocation_matrice_float(nbe, nbe);
	
	coef_dct(DCT);

	if(inverse){
		Matrice * inv_DCT = allocation_matrice_float(nbe, nbe);
		transposition_matrice(DCT, inv_DCT);
		liberation_matrice_float(DCT);
		DCT = inv_DCT;
	}

	for(int j = 0; j < nbe; j++){
		sortie[j] = 0.0;
		for(int i = 0; i < nbe; i++){
			sortie[j] += DCT->t[j][i] * entree[i];
		}
	}

}
