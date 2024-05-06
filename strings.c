
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

void TestString( unsigned testNumber,
  const char* stringStatement, const char* stringValue,
  const char* expectedStatement, const char* expectedStringValue ) {

  if( strcmp(stringValue, expectedStringValue) != 0 ) {
    printf( "[%02u] %s:'%s' doesn't match the expected value of %s:'%s'\n",
      testNumber,
      stringStatement, stringValue, expectedStatement, expectedStringValue
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

void TestClearString( unsigned testNumber, unsigned result,
  const char* pointerStatement ) {

  switch( result ) {
  case 0: /* expected */ break;
  case 1:
    printf( "[%02u] ClearString('%s') received a NULL reference\n",
      testNumber, pointerStatement );
    errorCount++;
    break;
  default:
    printf( "[%02u] ClearString('%s') returned an unexpected result"
      "code of %u\n", testNumber, pointerStatement, result );
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
    printf( "[Cancelling tests because sizeof(char) is %u]\n", sizeof(char) );
    exit(1);
  }

  ///TODO: Test edge calculation logic used within string functions

  {
    // [Logic 01]: Test AppendChar assumptions
    size_t destReservedLength;
    size_t destLength;
    size_t newReservedLength;
    size_t newLength;
    size_t newSize;

    // Case 01: reservedLength and length are 0
    destReservedLength = 0;
    destLength = 0;

    newReservedLength = (destReservedLength + 8) & (~(size_t)7);
    newLength = destLength + 1;

    if( newLength >= destReservedLength ) {
      newSize = sizeof(retstringImpl) + newReservedLength;
      if( newSize < newReservedLength ) {
        printf( "[AppendChar 01: Lines 17-22] reservedLength:%u and length:%u overflowed\n", destReservedLength, destLength );
        errorCount++;
      }
    }

    // Case 02: reservedLength + length + sizeof(retstringImpl) reach the end
    destReservedLength = (size_t)(-1);
    destLength = (size_t)(-1);

    newReservedLength = (destReservedLength + 8) & (~(size_t)7);
    newLength = destLength + 1;

    if( newLength >= destReservedLength ) {
      newSize = sizeof(retstringImpl) + newReservedLength;
      if( newSize < newReservedLength ) {
        printf( "[AppendChar 02: Lines 17-22] reservedLength:%u and length:%u overflowed\n", destReservedLength, destLength );
        errorCount++;
      }
    }
  }

  {
  // [Logic 02]: Test AppendCString and AppendString assumptions
  size_t availableLength = 0;
  size_t newLength = 0;

  // [Logic 03]: Test AppendCString and AppendString assumptions

  // [Logic 04]: Test AppendCString and AppendString assumptions
  size_t newSize = 0;
  }

  ///TODO: Test strcmp and __stricmp to indirectly validate CompareXYZ

  // NewString(non-zero) test cases
  s1 = NewString(5);

  TestPointer( 1, "NewString(5)", s1 );

  TestString( 2, "NewString(5)", s1, "-", "" );

  reservedLength = StringReservedLength(s1);
  TestValue( 3, "StringReservedLength(s1)", reservedLength, 8 );

  length = StringLength(s1);
  TestValue( 4, "StringLength(s1)", length, 0 );

  clength = strlen(s1);
  TestValue( 5, "strlen(s1)", clength, 0 );

  TestGreater( 6,
    "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length
  );

  TestEqual( 7, "StringLength(s1)", length, "strlen(s1)", clength );

  // AppendChar test cases
  tmpstr = AppendChar(s1, 'H');
  s1 = tmpstr ? tmpstr : s1;

  TestPointer( 8, "AppendChar(s1, 'H')", tmpstr );

  TestString( 9, "AppendChar(s1, 'H')", s1, "-", "H" );

  reservedLength = StringReservedLength(s1);
  TestValue( 10, "StringReservedLength(s1)", reservedLength, 8 );

  length = StringLength(s1);
  TestValue( 11, "StringLength(s1)", length, 1 );

  clength = strlen(s1);
  TestValue( 12, "strlen(s1)", clength, 1 );

  TestGreater( 13, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 14, "StringLength(s1)", length, "strlen(s1)", clength );

  // AppendCString test
  tmpstr = AppendCString(s1, "ello, ");
  s1 = tmpstr ? tmpstr : s1;

  TestPointer( 15, "AppendCString(s1, 'ello, ')", tmpstr );

  TestString( 16, "AppendCString(s1, 'ello, ')", s1, "-", "Hello, " );

  reservedLength = StringReservedLength(s1);
  TestValue( 17, "AppencCString(s1, 'ello. ')", reservedLength, 8 );

  length = StringLength(s1);
  TestValue( 18, "StringLEngth(s1)", length, 7 );

  clength = strlen(s1);
  TestValue( 19, "strlen(s1)", clength, 7 );

  TestGreater( 20, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 21, "StringLength(s1)", length, "strlen(s1)", clength );

  // AppendChar resize trigger test
  tmpstr = AppendChar(s1, 'w');
  s1 = tmpstr ? tmpstr : s1;

  TestPointer( 22, "AppendChar(s1, 'w')", tmpstr );

  TestString( 23, "AppendChar(s1, 'w')", s1, "-", "Hello, w" );

  reservedLength = StringReservedLength(s1);
  TestValue( 24, "AppencChar(s1, 'w')", reservedLength, 16 );

  length = StringLength(s1);
  TestValue( 25, "StringLength(s1)", length, 8 );

  clength = strlen(s1);
  TestValue( 26, "strlen(s1)", clength, 8 );

  TestGreater( 27, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 28, "StringLength(s1)", length, "strlen(s1)", clength );

  // DuplicateCString test
  s2 = DuplicateCString("orld!");

  TestPointer( 29, "DuplicateCString('orld!')", s2 );

  TestString( 30, "DuplicateCString('orld!')", s2, "-", "orld!" );

  reservedLength = StringReservedLength(s2);
  TestValue( 31, "DuplicateCString('orld!')", reservedLength, 8 );

  length = StringLength(s2);
  TestValue( 32, "StringLength(s2)", length, 5 );

  clength = strlen(s2);
  TestValue( 33, "strlen(s2)", clength, 5 );

  TestGreater( 34, "StringReservedLength(s2)", reservedLength,
    "StringLength(s2)", length );

  TestEqual( 35, "StringLength(s2)", length, "strlen(s2)", clength );

  // AppendString test
  tmpstr = AppendString(s1, s2);
  s1 = tmpstr ? tmpstr : s1;

  TestPointer( 36, "AppendString(s1, s2)", tmpstr );

  TestString( 37, "AppendString(s1, s2)", s1, "-", "Hello, world!" );

  reservedLength = StringReservedLength(s1);
  TestValue( 38, "AppendString(s1, s2)", reservedLength, 16 );

  length = StringLength(s1);
  TestValue( 39, "StringLength(s1)", length, 13 );

  clength = strlen(s1);
  TestValue( 40, "strlen(s1)", clength, 13 );

  TestGreater( 41, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 42, "StringLength(s1)", length, "strlen(s1)", clength );

  // ClearString test
  uresult = ClearString(s1);
  TestClearString( 43, uresult, "s1" );

  TestString( 44, "ClearString(s1)", s1, "-", "" );

  reservedLength = StringReservedLength(s1);
  TestValue( 45, "ClearString(s1)", reservedLength, 16 );

  length = StringLength(s1);
  TestValue( 46, "StringLength(s1)", length, 0 );

  clength = strlen(s1);
  TestValue( 47, "strlen(s1)", clength, 0 );

  TestGreater( 48, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 49, "StringLength(s1)", length, "strlen(s1)", clength );

  // CompactString test
  tmpstr = CompactString(s1);
  s1 = tmpstr ? tmpstr : s1;

  TestPointer( 50, "CompactString(s1)", tmpstr );

  TestString( 51, "CompactString(s1)", s1, "-", "" );

  reservedLength = StringReservedLength(s1);
  TestValue( 52, "CompactString(s1)", reservedLength, 8 );

  length = StringLength(s1);
  TestValue( 53, "StringLength(s1)", length, 0 );

  clength = strlen(s1);
  TestValue( 54, "strlen(s1)", clength, 0 );

  TestGreater( 55, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 56, "StringLength(s1)", length, "strlen(s1)", clength );

  uresult = ReleaseString( &s2 );
  TestReleaseString( 57, uresult, "&s2", s2 );

  uresult = ReleaseString(&s1);
  TestReleaseString( 58, uresult, "&s1", s1 );

  // DuplicateCString tests
  s1 = DuplicateCString("Hello, world!");

  TestPointer( 59, "DuplicateCString('Hello, world!')", s1 );

  TestString( 60, "DuplicateCString('Hello, world!')", s1,
    "-", "Hello, world!" );

  reservedLength = StringReservedLength(s1);
  TestValue( 61, "StringReservedLength(s1)", reservedLength, 16 );

  length = StringLength(s1);
  TestValue( 62, "StringLength(s1)", length, 13 );

  clength = strlen(s1);
  TestValue( 63, "strlen(s1)", clength, 13 );

  TestGreater( 64, "StringReservedLEngth(s1)", reservedLength,
    "StringLEngth(s1)", length );

  TestEqual( 65, "StringLength(s1)", length, "strlen(s1)", clength );

  // DuplicateString tests
  s2 = DuplicateString(s1);

  TestPointer( 66, "DuplicateString(s1)", s2 );

  TestString( 67, "s2 = DuplicateString(s1)", s2, "s1", s1 );

  reservedLength = StringReservedLength(s2);
  TestValue( 68, "StringReservedLength(s2)", reservedLength, 16 );

  length = StringLength(s2);
  TestValue( 69, "StringLength(s2)", length, 13 );

  clength = strlen(s2);
  TestValue( 70, "strlen(s2)", clength, 13 );

  TestGreater( 71, "StringReservedLength(s1)", reservedLength,
    "StringLength(s1)", length );

  TestEqual( 72, "StringLEngth(s1)", length, "strlen(s1)", clength );

  uresult = ReleaseString(&s1);
  TestReleaseString( 73, uresult, "&s1", s1 );

  uresult = ReleaseString(&s2);
  TestReleaseString( 74, uresult, "&s2", s2 );

  if( errorCount ) {
    printf( "[String tests failed with %u errors]\n", errorCount );
  } else {
    printf( "[String tests passed]\n" );
  }

  return 0;
}
