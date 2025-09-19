#include "bases.h"
#include "matrice.h"
#include "jpg.h"

void dct_image_tst()
{
  int i, j ;

  static float im1[3] = { 1, 2, 3 } ; 
  static float im2[3] = { 4, 5, 6 } ; 
  static float im3[3] = { 7, 8, 9 } ;
  static float *imt[] = { im1, im2, im3 } ;
  static Matrice im = {3, 3, imt} ;

  static float jm1[3] = { 1, 2, 3 } ; 
  static float jm2[3] = { 4, 5, 6 } ; 
  static float jm3[3] = { 7, 8, 9 } ; 
  static float *jmt[] = { jm1, jm2, jm3 } ;
  static Matrice jm = {3, 3, jmt} ;

  static float ir1[3] = {15      ,-2.44949,0} ;
  static float ir2[3] = {-7.34847,0       ,0} ;
  static float ir3[3] = {0       ,0       ,0} ;
  static float *irt[] = { ir1, ir2, ir3 } ;
  static Matrice ir = {3, 3, irt} ;

  dct_image(0, &im) ;

  for(j=0; j<3; j++)
    for(i=0; i<3; i++)
      if ( fabs(im.t[j][i] - ir.t[j][i]) > 1e-4 )
	{
	  eprintf("[%d][%d] = %g au lieu de %g\n", j, i, im.t[j][i], ir.t[j][i]) ;
	}

  dct_image(1, &ir) ;
  for(j=0; j<3; j++)
    for(i=0; i<3; i++)
      if ( fabs(jm.t[j][i] - ir.t[j][i]) > 1e-4 )
	{
	  eprintf("*[%d][%d] = %g au lieu de %g\n", j, i, ir.t[j][i], jm.t[j][i]) ;
	}
}

void quantification_tst()
{
  int i, j ;

  static float im1[3] = { 1, 2, 3 } ; 
  static float im2[3] = { 4, 5, 6 } ; 
  static float im3[3] = { 7, 8, 9 } ; 
  static float *imt[] = { im1, im2, im3 } ;
  static Matrice im = {3, 3, imt} ;

  static float ir1[3] = { 0.5, 0.666667, 0.75 } ; 
  static float ir2[3] = { 1.33333, 1.25, 1.2 } ; 
  static float ir3[3] = { 1.75 ,1.6 ,  1.5 } ; 
  static float *irt[] = { ir1, ir2, ir3 } ;
  static Matrice ir = {3, 3, irt} ;

  quantification(3, 1, &im, 0) ;

  for(j=0; j<3; j++)
    for(i=0; i<3; i++)
      if ( fabs(im.t[j][i] - ir.t[j][i]) > 1e-4 )
	{
	  eprintf("[%d][%d] = %g au lieu de %g\n", j, i, im.t[j][i], ir.t[j][i]) ;
	}

  quantification(3, 1, &im, 1) ;
  quantification(3, 1, &im, 0) ;

  for(j=0; j<3; j++)
    for(i=0; i<3; i++)
      if ( fabs(im.t[j][i] - ir.t[j][i]) > 1e-4 )
	{
	  eprintf("[%d][%d] = %g au lieu de %g\n", j, i, im.t[j][i], ir.t[j][i]) ;
	}

}


void zigzag_tst()
{
#define ZZ(N,Y,X) old_i=i ; old_j=j ; zigzag(N,&j,&i) ; if ( i!=X || j!=Y ) eprintf("Mauvais zigzag dans carré de coté %d.\nAprès X=%d Y=%d j'attend X=%d Y=%d et vous donnez X=%d Y=%d\n", N,old_i, old_j,X,Y,i,j) ;

  int i, j, old_i, old_j ;
  i = 0 ;
  j = 0 ;
  ZZ(3, 0,1) ; 
  ZZ(3, 1,0) ; 
  ZZ(3, 2,0) ; 
  ZZ(3, 1,1) ; 
  ZZ(3, 0,2) ; 
  ZZ(3, 1,2) ; 
  ZZ(3, 2,1) ; 
  ZZ(3, 2,2) ; 
  i = 0 ;
  j = 0 ;
  ZZ(4, 0,1) ; 
  ZZ(4, 1,0) ; 
  ZZ(4, 2,0) ; 
  ZZ(4, 1,1) ; 
  ZZ(4, 0,2) ; 
  ZZ(4, 0,3) ; 
  ZZ(4, 1,2) ; 
  ZZ(4, 2,1) ; 
  ZZ(4, 3,0) ; 
  ZZ(4, 3,1) ; 
  ZZ(4, 2,2) ; 
  ZZ(4, 1,3) ; 
  ZZ(4, 2,3) ; 
  ZZ(4, 3,2) ; 
  ZZ(4, 3,3) ; 
  i = 0 ;
  j = 0 ;
  ZZ(5, 0,1) ; 
  ZZ(5, 1,0) ; 
  ZZ(5, 2,0) ; 
  ZZ(5, 1,1) ; 
  ZZ(5, 0,2) ; 
  ZZ(5, 0,3) ; 
  ZZ(5, 1,2) ; 
  ZZ(5, 2,1) ; 
  ZZ(5, 3,0) ; 
  ZZ(5, 4,0) ; 
  ZZ(5, 3,1) ; 
  ZZ(5, 2,2) ; 
  ZZ(5, 1,3) ; 
  ZZ(5, 0,4) ; 
  ZZ(5, 1,4) ; 
  ZZ(5, 2,3) ; 
  ZZ(5, 3,2) ; 
  ZZ(5, 4,1) ; 
  ZZ(5, 4,2) ; 
  ZZ(5, 3,3) ; 
  ZZ(5, 2,4) ; 
  ZZ(5, 3,4) ; 
  ZZ(5, 4,3) ; 
  ZZ(5, 4,4) ; 
}
