
#include <stdio.h>
#include <string.h>

#include "retlib.h"

int main( int argc, char* argv[] ) {
  retstring s1 = NULL;
  retstring s2 = NULL;
  retstring tmpstr = NULL;
  unsigned uresult;
  unsigned errorCount = 0;
  size_t reservedLength = 0;
  size_t length = 0;
  size_t clength = 0;

  printf( "[Starting string tests]\n" );

  switch( sizeof(char) ) {
  case 1: // sizeof(char) == 1
    break;

  default:
    printf( "[Cancelling tests because sizeof(char) is %u\n", sizeof(char) );
    exit(1);
  }

  ///TODO: Test edge calculation logic used within string functions

  {
  // [Logic 01]: Test AppendChar assumptions
  }

  {
  // [Logic 02]: Test AppendCString and AppendString assumptions
  size_t availableLength = 0;
  size_t newLength = 0;

  // [Logic 03]: Test AppendCString and AppendString assumptions

  // [Logic 04]: Test AppendCString and AppendString assumptions
  size_t newSize = 0;
  }

  // NewString(non-zero) test cases
  s1 = NewString(5);
  if( s1 == NULL ) {
    printf( "[00] NewString(5) returned a NULL pointer\n" );
    errorCount++;
  }
  reservedLength = StringReservedLength(s1);
  if( reservedLength != 8 ) {
    printf( "[01] StringReservedLength(s1) returned %u instead of %u\n", reservedLength, 8 );
    errorCount++;
    printf( "[02] s1->reservedLength is %u instead of %u after NewString(5)\n", reservedLength, 8 );
    errorCount++;
  }
  length = StringLength(s1);
  if( length != 0 ) {
    printf( "[03] StringLength(s1) retuned %u instead of %u\n", length, 0 );
    errorCount++;
    printf( "[04] s1->length is %u instead of %u after NewString(5)\n", length, 0 );
    errorCount++;
  }
  clength = strlen(s1);
  if( clength != 0 ) {
    printf( "[05] strlen(s1) returned %u instead of %u\n", clength, 0 );
    errorCount++;
  }
  if( reservedLength < length ) {
    printf( "[06] StringReservedLength(s1):%u should be greater than StringLength(s1):%u\n", reservedLength, length );
    errorCount++;
  }
  if( length != clength ) {
    printf( "[07] StringLength(s1):%u should be equal to strlen(s1):%u\n", length, clength );
    errorCount++;
  }

  // AppendChar test cases
  tmpstr = AppendChar(s1, 'H');
  if( tmpstr == NULL ) {
    printf( "[08] AppendChar(s1, 'H') returned a NULL pointer\n" );
    errorCount++;
  }
  s1 = tmpstr ? tmpstr : s1;
  reservedLength = StringReservedLength(s1);
  if( reservedLength != 8 ) {
    printf( "[09] StringReservedLength(s1) returned %u instead of %u\n", reservedLength, 8 );
    errorCount++;
    printf( "[10] s1->reservedLength is %u instead of %u after AppendChar(s1, 'H')\n", reservedLength, 8 );
    errorCount++;
  }
  length = StringLength(s1);
  if( length != 1 ) {
    printf( "[11] StringLength(s1) returned %u instead of %u\n", length, 1 );
    errorCount++;
    printf( "[12] s1->length is %u instead of %u after AppendChar(s1, 'H')\n", length, 1 );
    errorCount++;
  }
  clength = strlen(s1);
  if( clength != 1 ) {
    printf( "[13] strlen(s1) returned %u instead of %u\n", clength, 1 );
    errorCount++;
  }
  if( reservedLength < length ) {
    printf( "[14] StringReservedLength(s1):%u should be greater than StringLength(s1):%u\n", reservedLength, length );
    errorCount++;
  }
  if( length != clength ) {
    printf( "[15] StringLength(s1):%u should be equal to strlen(s1):%u\n", length, clength );
    errorCount++;
  }

  ///TODO: AppendCString test
  tmpstr = AppendCString(s1, "ello, ");
  s1 = tmpstr ? tmpstr : s1;

  ///TODO: AppendChar resize trigger test
  ///TODO: AppendString test

  uresult = ReleaseString(&s1);
  switch( uresult ) {
  case 0: /* expected */ break;
  case 1:
    printf( "[16] ReleaseString(&s1) received &s1 as a NULL pointer\n" );
    errorCount++;
    break;
  default:
    printf( "[17] ReleaseString(&s1) returned an unexpected result code of %u\n", uresult );
    errorCount++;
  }
  if( s1 != NULL ) {
    printf( "[18] Expected s1 to be NULL after ReleaseString(&s1)\n" );
    errorCount++;
  }

  // DuplicateCString tests
  s1 = DuplicateCString("Hello, world!");
  if( s1 == NULL ) {
    printf( "[19] DuplicateCString('Hello, world!') returned a NULL pointer\n" );
    errorCount++;
  }
  reservedLength = StringReservedLength(s1);
  if( reservedLength != 16 ) {
    printf( "[20] StringReservedLength(s1) returned %u instead of %u\n", reservedLength, 16 );
    errorCount++;
    printf( "[21] s1->reservedLength is %u instead of %u after DuplicateCString('Hello, world!')\n", reservedLength, 16 );
    errorCount++;
  }
  length = StringLength(s1);
  if( length != 13 ) {
    printf( "[22] StringLength(s1) returned %u instead of %u\n", length, 13 );
    errorCount++;
    printf( "[23] s1->length is %u instead of %u after DuplicateCString('Hello, world!')\n", length, 13 );
    errorCount++;
  }
  clength = strlen(s1);
  if( clength != 13 ) {
    printf( "[24] strlen(s1) returned %u instead of %u\n", clength, 13 );
    errorCount++;
  }
  if( reservedLength < length ) {
    printf( "[25] StringReservedLength(s1):%u should be greater than StringLength(s1):%u\n", reservedLength, length );
    errorCount++;
  }
  if( length != clength ) {
    printf( "[26] StringLength(s1):%u should be equal to strlen(s1):%u\n", length, clength );
    errorCount++;
  }

  // DuplicateString tests
  s2 = DuplicateString(s1);
  if( s2 == NULL ) {
    printf( "[27] DuplicateString(s1) returned a NULL pointer\n" );
    errorCount++;
  }
  reservedLength = StringReservedLength(s2);
  if( reservedLength != 16 ) {
    printf( "[28] StringReservedLength(s2) returned %u instead of %u\n", reservedLength, 16 );
    errorCount++;
    printf( "[29] s2->reservedLength is %u instead of %u after DuplicateString(s1)\n", reservedLength, 16 );
    errorCount++;
  }
  length = StringLength(s2);
  if( length != 13 ) {
    printf( "[30] StringLength(s2) returned %u instead of %u\n", length, 13 );
    errorCount++;
    printf( "[31] s2->length is %u instead of %u after DuplicateString(s1)\n", length, 13 );
    errorCount++;
  }
  clength = strlen(s2);
  if( clength != 13 ) {
    printf( "[32] strlen(s2) returned %u instead of %u\n", clength, 13 );
    errorCount++;
  }
  if( reservedLength < length ) {
    printf( "[33] StringReservedLength(s1):%u should be greater than StringLength(s1):%u\n", reservedLength, length );
    errorCount++;
  }
  if( length != clength ) {
    printf( "[34] StringLength(s1):%u should be equal to strlen(s1):%u\n", length, clength );
    errorCount++;
  }

  uresult = ReleaseString(&s1);
  switch( uresult ) {
  case 0: /* expected */ break;
  case 1:
    printf( "[35] ReleaseString(&s1) received &s1 as a NULL pointer\n" );
    errorCount++;
    break;
  default:
    printf( "[36] ReleaseString(&s1) returned an unexpected result code of %u\n", uresult );
    errorCount++;
  }
  if( s1 != NULL ) {
    printf( "[37] Expected s1 to be NULL after ReleaseString(&s1)\n" );
    errorCount++;
  }

  uresult = ReleaseString(&s2);
  switch( uresult ) {
  case 0: /* expected */ break;
  case 1:
    printf( "[38] ReleaseString(&s2) received &s2 as a NULL pointer\n" );
    errorCount++;
    break;
  default:
    printf( "[39] ReleaseString(&s2) returned an unexpected result code of %u\n", uresult );
    errorCount++;
  }
  if( s2 != NULL ) {
    printf( "[40] Expected s2 to be NULL after ReleaseString(&s2)\n" );
    errorCount++;
  }

  if( errorCount ) {
    printf( "[String tests failed with %u errors]\n", errorCount );
  } else {
    printf( "[String tests passed]\n" );
  }

  return 0;
}
