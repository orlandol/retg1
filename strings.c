
#include <stdio.h>

#include "retlib.h"

int main( int argc, char* argv[] ) {
  retstring s1 = NULL;
  unsigned uresult;
  unsigned errorCount = 0;

  printf( "[Starting string tests]\n" );

  // NewString(non-zero) test cases
  s1 = NewString(5);
  if( s1 == NULL ) {
    printf( "Unexpected return value of NULL in NewString(5)\n" );
    errorCount++;
  }
  if( StringLength(s1) ) {
    printf( "Expected length of 0 from StringLength(s1)\n" );
    errorCount++;
    printf( "Expected s1->length == 0 in NewString(5)\n" );
    errorCount++;
  }
  if( StringReservedLength(s1) != 8 ) {
    printf( "Expected reserved length of 8 from StringReservedLength(s1)\n" );
    errorCount++;
    printf( "Expected s1->reservedLength == 8 in NewString(5)\n" );
    errorCount++;
  }

  uresult = ReleaseString(&s1);
  switch( uresult ) {
  case 0: /* expected */ break;
  case 1:
    printf( "Unexpected NULL address &s1 passed to ReleaseString(&s1)\n" );
    errorCount++;
    break;
  default:
    printf( "Unexpected value %u returned by ReleaseString(&s1)\n", uresult );
    errorCount++;
  }

  if( errorCount ) {
    printf( "[String tests failed with %u errors]\n", errorCount );
  } else {
    printf( "[String tests passed]\n" );
  }

  return 0;
}
