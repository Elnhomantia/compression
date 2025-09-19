#ifndef DCT_H
#define DCT_H

void coef_dct(Matrice *table) ;
void dct(int inverse, int nbe, const float *entree, float *sortie ) ;

#endif
