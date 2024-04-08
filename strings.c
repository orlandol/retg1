
#include <stdio.h>
#include <string.h>

#include "retlib.h"

///TODO: Create functions for repetitive test code

int main( int argc, char* argv[] ) {
  retstring s1 = NULL;
  retstring s2 = NULL;
  retstring tmpstr = NULL;
  unsigned uresult;
  unsigned errorCount = 0;
  size_t tmpLen = 0;

  printf( "[Starting string tests]\n" );

  // NewString(non-zero) test cases
  s1 = NewString(5);
  if( s1 == NULL ) {
    printf( "Unexpected return value of NULL in NewString(5)\n" );
    errorCount++;
  }
  tmpLen = StringReservedLength(s1);
  if( tmpLen != 8 ) {
    printf( "Unexpected reserved length of %u from StringReservedLength(s1)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s1->reservedLength == %u in NewString(5)\n", tmpLen );
    errorCount++;
  }
  tmpLen = StringLength(s1);
  if( tmpLen ) {
    printf( "Unxpected length of %u from StringLength(s1)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s1->length == %u in NewString(5)\n", tmpLen );
    errorCount++;
  }
  tmpLen = strlen(s1);
  if( tmpLen ) {
    printf( "Unexpected length of %u from strlen(s1)\n", tmpLen );
    errorCount++;
  }

  // AppendChar test cases
  tmpstr = AppendChar(s1, 'H');
  if( tmpstr == NULL ) {
    printf( "Unexpected NULL return from AppendChar(s1, 'H')\n" );
    errorCount++;
  }
  s1 = tmpstr ? tmpstr : s1;
  tmpLen = StringReservedLength(s1);
  if( tmpLen != 8 ) {
    printf( "Unexpected reserved length of %u from StringReservedLength(s1)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s1->reservedLength == %u in AppendChar(s1, 'H')\n", tmpLen );
    errorCount++;
  }
  tmpLen = StringLength(s1);
  if( tmpLen != 1 ) {
    printf( "Unexpected length of %u from StringLength(s1)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s1->length == %u in AppendChar(s1, 'H')\n", tmpLen );
    errorCount++;
  }
  tmpLen = strlen(s1);
  if( tmpLen != 1 ) {
    printf( "Unexpected length of %u from strlen(s1)\n", tmpLen );
    errorCount++;
  }

  ///TODO: AppendCString test
  ///TODO: AppendChar resize trigger test
  ///TODO: AppendString test

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
  if( s1 != NULL ) {
    printf( "Expected s1 to be NULL after ReleaseString(&s1)\n" );
    errorCount++;
  }

  // DuplicateCString tests
  s1 = DuplicateCString("Hello, world!");
  if( s1 == NULL ) {
    printf( "Unexpected return value of NULL in DuplicateCString('Hello, world!')\n" );
    errorCount++;
  }
  tmpLen = StringReservedLength(s1);
  if( tmpLen != 16 ) {
    printf( "Unexpected reserved length of %u from StringReservedLength(s1)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s1->reservedLength == %u in DuplicateCString('Hello, world!')\n", tmpLen );
    errorCount++;
  }
  tmpLen = StringLength(s1);
  if( tmpLen != 13 ) {
    printf( "Unxpected length of %u from StringLength(s1)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s1->length == %u in DuplicateCString('Hello, world!')\n", tmpLen );
    errorCount++;
  }
  tmpLen = strlen(s1);
  if( tmpLen != 13 ) {
    printf( "Unexpected length of %u from strlen(s1)\n", tmpLen );
    errorCount++;
  }

  // DuplicateString tests
  s2 = DuplicateString(s1);
  if( s2 == NULL ) {
    printf( "Unexpected return value of NULL in DuplicateString(s1)\n" );
    errorCount++;
  }
  tmpLen = StringReservedLength(s2);
  if( tmpLen != 16 ) {
    printf( "Unexpected reserved length of %u from StringReservedLength(s2)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s2->reservedLength == %u in DuplicateString(s2)\n", tmpLen );
    errorCount++;
  }
  tmpLen = StringLength(s2);
  if( tmpLen != 13 ) {
    printf( "Unxpected length of %u from StringLength(s2)\n", tmpLen );
    errorCount++;
    printf( "Unexpected s2->length == %u in DuplicateString(s2)\n", tmpLen );
    errorCount++;
  }
  tmpLen = strlen(s2);
  if( tmpLen != 13 ) {
    printf( "Unexpected length of %u from strlen(s2)\n", tmpLen );
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
  if( s1 != NULL ) {
    printf( "Expected s1 to be NULL after ReleaseString(&s1)\n" );
    errorCount++;
  }

  uresult = ReleaseString(&s2);
  switch( uresult ) {
  case 0: /* expected */ break;
  case 1:
    printf( "Unexpected NULL address &s2 passed to ReleaseString(&s2)\n" );
    errorCount++;
    break;
  default:
    printf( "Unexpected value %u returned by ReleaseString(&s2)\n", uresult );
    errorCount++;
  }
  if( s2 != NULL ) {
    printf( "Expected s2 to be NULL after ReleaseString(&s2)\n" );
    errorCount++;
  }

  if( errorCount ) {
    printf( "[String tests failed with %u errors]\n", errorCount );
  } else {
    printf( "[String tests passed]\n" );
  }

  return 0;
}
