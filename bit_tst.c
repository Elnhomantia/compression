#include "bases.h"
#include "bit.h"

void nb_bits_utile_tst()
{
  int i ;
  static int n[] = { 0, 1, 2, 2, 3, 3, 3, 3, 4 } ;

  for( i=0; i<TAILLE(n); i++)
    if ( nb_bits_utile(i) != n[i] )
      {
	eprintf("nb_bits_utile(%d) != %d\n", i, n[i]) ;
	return ;
      }
  if ( nb_bits_utile(1ul << 31) != 32 )
	eprintf("nb_bits_utile(1ul << 31) != 32\n") ;
  if ( sizeof(unsigned long) > 4 )
  {
    if ( nb_bits_utile(1ul << 63) != 64 )
	  eprintf("nb_bits_utile(1ul << 63) != 64\n") ;
  }
}

void pow2_tst()
{
  int i ;

  if ( pow2(0) != 1 )
    {
      eprintf("pow2(0) != 1\n") ;
      return ;
    }
  for(i=0;i<8*sizeof(long)-1;i++)
    if ( pow2(i+1) != pow2(i)*2 )
      {
	eprintf("pow2(%d) != pow2(%d)*2\n", i+1, i) ;
	return ;
      }
  if ( pow2(70) != 0 )
    fprintf(stderr,"...\npow2(70)!=0 (vous trouvez %ld) c'est un warning sans importance\n                                         : ",pow2(70));
}


void prend_bit_tst()
{
  int i, j, k, s ;

  for(i=0;i<1000;i++)
    {
      j = rand() ;
      s = 0 ;
      for(k=0;k<8*sizeof(unsigned long);k++)
	if ( prend_bit(j, k) == Vrai )
	  s |= pow2(k) ;
	else
	  if ( prend_bit(j, k) != Faux )
	    {
	      eprintf("Retourne ni Faux, ni Vrai\n") ;
	      return ;
	    }
      if ( s != j )
	{
	  eprintf("Mauvais\n") ;
	  return ;
	}
    }
  if ( prend_bit(-1, 65) == 1 )
    fprintf(stderr,"...\nprend_bit(-1,65)!=0 c'est un warning sans importance\n                                         : ");
}

void pose_bit_tst()
{
  int i, j, k, s ;

  if ( pose_bit(0,0,2) != 1 )
    {
      eprintf("Veuillez relire l'énoncé : Faux==0, Vrai sinon\n") ;
      return ;
    }
  if ( pose_bit(1,0,2) != 1 )
    {
      eprintf("Veuillez relire l'énoncé : Faux==0, Vrai sinon\n") ;
      return ;
    }
  if ( pose_bit(1,0,-2) != 1 )
    {
      eprintf("Veuillez relire l'énoncé : Faux==0, Vrai sinon\n") ;
      return ;
    }

  for(i=0;i<1000;i++)
    {
      j = rand() ;
      s = rand() ;
      for(k=0; k < 8*sizeof(unsigned long); k++)
	s = pose_bit(s, k, prend_bit(j, k)) ;
      if ( j != s )
	{
	  eprintf("Mauvais: relisez votre function...\n") ;
	  return ;
	}
    }	
  if ( pose_bit(0, 65, 1) != 0 )
    fprintf(stderr,"...\npose_bit(0,65,1)!=0 c'est un warning sans importance\n                                         : ");

}
