
#include <stdio.h>
#include <string.h>

#include "retlib.h"

unsigned errorCount = 0;

void TestPointer( unsigned testNumber, const char* statement,
  const void* pointer ) {

  if( pointer == NULL ) {
    printf( "[%02u] '%s' resulted in a NULL pointer\n",
      testNumber, statement ? statement : ""
    );
    errorCount++;
  }
}

void TestValue( unsigned testNumber, const char* statement,
  unsigned actualValue, unsigned expectedValue ) {

  if( actualValue != expectedValue ) {
    printf( "[%02u] '%s' resulted in %u instead of %u\n",
      testNumber, statement ? statement : "",
      actualValue, expectedValue
    );
    errorCount++;
  }
}

void TestGreater( unsigned testNumber,
  const char* leftStatement, unsigned leftValue,
  const char* rightStatement, unsigned rightValue ) {

  if( leftValue < rightValue ) {
    printf( "[%02u] '%s':%u should be greater than > '%s':%u\n",
      testNumber,
      leftStatement ? leftStatement : "", leftValue,
      rightStatement ? rightStatement : "", rightValue
    );
    errorCount++;
  }
}

void TestEqual( unsigned testNumber,
  const char* leftStatement, unsigned leftValue,
  const char* rightStatement, unsigned rightValue ) {

  if( leftValue != rightValue ) {
    printf( "[%02u] '%s':%u should be equal to '%s':%u\n", testNumber,
      leftStatement, leftValue,
      rightStatement, rightValue
    );
    errorCount++;
  }
}

void TestReleaseString( unsigned testNumber, unsigned result,
  const char* pointerStatement, void* pointer ) {

  switch( result ) {
  case 0: /* expected */ break;
  case 1:
    printf( "[%02u] ReleaseString('%s') received a NULL reference\n",
      testNumber, pointerStatement );
    errorCount++;
    break;
  default:
    printf( "[%02u] ReleaseString('%s') returned an unexpected result"
      "code of %u\n", testNumber, pointerStatement, result );
    errorCount++;
  }
  if( pointer != NULL ) {
    printf( "[%02u] Expected pointer to be NULL after "
      "ReleaseString('%s')\n", testNumber, pointerStatement );
    errorCount++;
  }
}

int main( int argc, char* argv[] ) {
  retstring s1 = NULL;
  retstring s2 = NULL;
  retstring tmpstr = NULL;
  unsigned uresult;
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
  TestPointer( 0, "NewString(5)", s1 );

  reservedLength = StringReservedLength(s1);
  TestValue( 1, "StringReservedLength(s1)", reservedLength, 8 );

  length = StringLength(s1);
  TestValue( 2, "StringLength(s1)", length, 0 );

  clength = strlen(s1);
  TestValue( 3, "strlen(s1)", clength, 0 );

  TestGreater( 4,
    "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length
  );

  TestEqual( 5, "StringLength(s1)", length, "strlen(s1)", clength );

  // AppendChar test cases
  tmpstr = AppendChar(s1, 'H');
  TestPointer( 6, "AppendChar(s1, 'H')", tmpstr );
  s1 = tmpstr ? tmpstr : s1;

  reservedLength = StringReservedLength(s1);
  TestValue( 7, "StringReservedLength(s1)", reservedLength, 8 );

  length = StringLength(s1);
  TestValue( 8, "StringLength(s1)", length, 1 );

  clength = strlen(s1);
  TestValue( 9, "strlen(s1)", clength, 1 );

  TestGreater( 10, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 11, "StringLength(s1)", length, "strlen(s1)", clength );

  ///TODO: AppendCString test
  tmpstr = AppendCString(s1, "ello, ");
  TestPointer( 12, "AppendCString(s1, 'ello, '", tmpstr );
  s1 = tmpstr ? tmpstr : s1;

  ///TODO: AppendChar resize trigger test
  tmpstr = AppendChar(s1, 'W');
  TestPointer( 13, "AppendChar(s1, 'W')", tmpstr );
  s1 = tmpstr ? tmpstr : s1;

  ///TODO: AppendString test
  s2 = DuplicateCString("orld!");
  tmpstr = AppendString(s1, s2);
  s1 = tmpstr ? tmpstr : s1;

  uresult = ReleaseString( &s2 );
  TestReleaseString( 12, uresult, "&s2", s2 );

  uresult = ReleaseString(&s1);
  TestReleaseString( 13, uresult, "&s1", s1 );

  // DuplicateCString tests
  s1 = DuplicateCString("Hello, world!");
  TestPointer( 14, "DuplicateCString('Hello, world!')", s1 );

  reservedLength = StringReservedLength(s1);
  TestValue( 15, "StringReservedLength(s1)", reservedLength, 16 );

  length = StringLength(s1);
  TestValue( 16, "StringLength(s1)", length, 13 );

  clength = strlen(s1);
  TestValue( 17, "strlen(s1)", clength, 13 );

  TestGreater( 18, "StringReservedLEngth(s1)", reservedLength,
    "StringLEngth(s1)", length );

  TestEqual( 19, "StringLength(s1)", length, "strlen(s1)", clength );

  // DuplicateString tests
  s2 = DuplicateString(s1);
  TestPointer( 20, "DuplicateString(s1)", s2 );

  reservedLength = StringReservedLength(s2);
  TestValue( 21, "StringReservedLength(s2)", reservedLength, 16 );

  length = StringLength(s2);
  TestValue( 22, "StringLength(s2)", length, 13 );

  clength = strlen(s2);
  TestValue( 23, "strlen(s2)", clength, 13 );

  TestGreater( 24, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 25, "StringLEngth(s1)", length, "strlen(s1)", clength );

  uresult = ReleaseString(&s1);
  TestReleaseString( 26, uresult, "&s1", s1 );

  uresult = ReleaseString(&s2);
  TestReleaseString( 27, uresult, "&s2", s2 );

  if( errorCount ) {
    printf( "[String tests failed with %u errors]\n", errorCount );
  } else {
    printf( "[String tests passed]\n" );
  }

  return 0;
}
