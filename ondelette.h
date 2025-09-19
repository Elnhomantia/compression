/*
 * Créé le Mon Nov 25 10:56:49 2002 par Thierry EXCOFFIER
 */

#ifndef _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_ONDELETTE_H
#define _HOME_EXCO_REDACTEX_COURS_TRANS_COMP_IMAGE_TP_DCT2_ONDELETTE_H

void ondelette_1d(const float *entree, float *sortie, int nbe) ;
void ondelette_2d(Matrice *image) ;

void ondelette_1d_inverse(const float *entree, float *sortie, int nbe) ;
void ondelette_2d_inverse(Matrice *image) ;

void ondelette_encode_image(float qualite) ; /**/
void ondelette_decode_image() ; /**/


#endif
