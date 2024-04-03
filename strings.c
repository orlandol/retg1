
#include <stdio.h>

#include "retlib.h"

int main( int argc, char* argv[] ) {
  retstring s1 = NULL;

  s1 = NewString(5);

  s1[0] = 't';
  s1[1] = 'e';
  s1[2] = 's';
  s1[3] = 't';

  printf( "%u\n", StringLength(s1) );

  printf( "%u\n", StringReservedLength(s1) );

  printf( "s1 == '%s'\n", s1 );

  return 0;
}
