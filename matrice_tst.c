#include "bases.h"
#include "matrice.h"

void allocation_matrice_float_tst()
{
  Matrice *m ;
  int n, i , j ;

  n = 10 ;

  m = allocation_matrice_float(n, n) ;
  for(j=0; j<n; j++)
    for(i=0; i<n; i++)
      m->t[j][i] = 1000*i + j ;

  for(j=0; j<n; j++)
    for(i=0; i<n; i++)
      if ( m->t[j][i] != 1000*i + j )
	{
	  eprintf("Le contenu de la matrice s'auto écrase\n") ;
	  return ;
	}
}

void liberation_matrice_float_tst()
{
  Matrice *m ;
  int n ;

  n = 10 ;
  m = allocation_matrice_float(n, n) ;
  liberation_matrice_float(m) ;

  if ( allocation_matrice_float(n, n) != m )
    {
      eprintf("Vous êtes sûr de tout libérer ?\n") ;
      return ;
    }
}
