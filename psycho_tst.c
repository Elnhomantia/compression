#include "bases.h"
#include "psycho.h"

void psycho_tst()
{
  static float t[] =
  {
    10000,
    -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -3, 1, -1, 1,
    10,
    -1, 1, -1, 3, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1,
    -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -3, 1, -1, 1,
    -10,
    -1, 1, -1, 3, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1,
    -1, 1, -1, 3, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1,
  } ;
  static float t_ok[] =
  { 10000, -1, 1, -1, 1, -1, 1, -1, 1, -1, 0, 0, 0, 0, 0, -3, 0, 0, 0, 10, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 0, 0, 0, 0, 0, -3, 0, 0, 0, -10, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 0, 0, 3, 0, 0, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1 } ;

  float tt[TAILLE(t)] ;
  int i ;
    

  memcpy(tt, t, sizeof(t)) ;
  psycho(TAILLE(tt), tt, 1) ;

  if ( 0 )
    {
      for(i=0; i<TAILLE(tt); i++)
	printf("%g, ", tt[i]) ;
      printf("\n") ;
    }

  for(i=0; i<TAILLE(tt); i++)
    if ( t_ok[i] != tt[i] )
      {
	eprintf("Voir le source du test : %s:%d\n", __FILE__, __LINE__) ;
	eprintf("Index %d, sous trouvez %f au lieu de %f\n", i,tt[i], t_ok[i]);
	return ;
      }
}
