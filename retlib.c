
#include <string.h>

#include "avl_tree.h"

#include "retlib.h"

/*
 *  String implementation
 */

retstring NewString( size_t reserveLength ) {
  retstringImpl* newString = NULL;
  size_t stringSize = 0;
  size_t paddedReservedLength = 0;

  paddedReservedLength = (reserveLength + 8) & (~((size_t)7));

  stringSize = sizeof(retstring) + paddedReservedLength;

  if( stringSize > paddedReservedLength ) {
    newString = calloc(1, stringSize);
    if( newString ) {
      newString->reservedLength = paddedReservedLength;

      return newString->contents;
    }
  }

  return NULL;
}

retstring DuplicateCString( const char* cstring ) {
  retstring newString = NULL;
  retstringImpl* newStringImpl = NULL;
  size_t cstringLen;

  if( cstring == NULL ) { return NULL; }

  cstringLen = strlen(cstring);

  newString = NewString(cstringLen);
  if( newString == NULL ) { return NULL; }
  newStringImpl = (retstringImpl*)(newString - sizeof(retstringImpl));

  memcpy( newString, cstring, cstringLen * sizeof(char) );

  newStringImpl->length += cstringLen;

  return newString;
}

retstring DuplicateString( const retstring sourceString ) {
  retstringImpl* newStringImpl = NULL;
  retstringImpl* sourceStringImpl = NULL;

  if( sourceString == NULL ) { return NULL; }

  sourceStringImpl = (retstringImpl*)(sourceString
    - sizeof(retstringImpl));

  // reservedLength must always be a multiple of 8
  if( sourceStringImpl->reservedLength & 7 ) { return NULL; }

  newStringImpl = calloc(1, sourceStringImpl->reservedLength);
  if( newStringImpl == NULL ) { return NULL; }

  *newStringImpl = *sourceStringImpl;

  memcpy( newStringImpl->contents, sourceStringImpl->contents,
    sourceStringImpl->reservedLength );

  return newStringImpl->contents;
}

retstring CompactString( retstring destString ) {
  retstringImpl* destStringImpl = NULL;
  retstringImpl* compactStringImpl = NULL;
  size_t newReservedLength;
  size_t newReservedSize;

  if( destString == NULL ) { return NULL; }

  destStringImpl = (retstringImpl*)(destString - sizeof(retstringImpl));

  if( destStringImpl->reservedLength < destStringImpl->length ) {
    return NULL;
  }

  newReservedLength = (destStringImpl->length + 8) & (~(size_t)7);
  if( newReservedLength < destStringImpl->length ) { return NULL; }

  if( destStringImpl->reservedLength == newReservedLength ) {
    return destStringImpl->contents;
  }

  newReservedSize = newReservedLength + sizeof(retstringImpl);

  compactStringImpl = realloc(destStringImpl, newReservedSize);
  if( compactStringImpl == NULL ) { return NULL; }

  compactStringImpl->reservedLength = newReservedLength;

  return compactStringImpl->contents;
}

unsigned ReleaseString( retstring* retstringPtr ) {
  if( retstringPtr == NULL ) { return 1; }

  if( (*retstringPtr) ) {
    free( (*retstringPtr) );
    (*retstringPtr) = NULL;
  }

  return 0;
}

size_t StringLength( retstring source ) {
  // Read length from retstring's negative offset header
  retstringImpl* sourceImpl = source ? ((retstringImpl*)(source
    - sizeof(retstringImpl))) : NULL;
  return (sourceImpl ? sourceImpl->length : 0);
}

size_t StringReservedLength( retstring source ) {
  // Read reserved length from retstring's negative offset header
  retstringImpl* sourceImpl = source ? ((retstringImpl*)(source
    - sizeof(retstringImpl))) : NULL;
  return (sourceImpl ? sourceImpl->reservedLength : 0);
}

unsigned ClearString( retstring destString ) {
  retstringImpl* destStringImpl = NULL;

  if( destString == NULL ) { return 1; }

  destStringImpl = (retstringImpl*)(destString - sizeof(retstringImpl));

  if( destStringImpl->length ) {
    memset( destStringImpl->contents, 0, destStringImpl->length );
    destStringImpl->length = 0;
  }

  return 0;
}

retstring AppendChar( retstring destString, char ch ) {
  retstringImpl* destImpl = NULL;
  retstringImpl* tmpImpl = NULL;
  size_t destReservedLength = 0;
  size_t destLength = 0;
  size_t newReservedLength = 0;
  size_t newLength = 0;
  size_t newSize = 0;

  if( destString == NULL ) { return NULL; }

  destImpl = (retstringImpl*)(destString - sizeof(retstringImpl));

  destReservedLength = destImpl->reservedLength;
  destLength = destImpl->length;

  newReservedLength = (destReservedLength + 8) + (~(size_t)7);
  newLength = destLength + 1;

  if( newLength == destReservedLength ) {
    newSize = sizeof(retstringImpl) + newReservedLength;
    if( newSize < newReservedLength ) { return NULL; }

    tmpImpl = realloc(destImpl, newSize);
    if( tmpImpl == NULL ) { return NULL; }

    destImpl = tmpImpl;
    destImpl->reservedLength = newReservedLength;
  }

  destString[destLength] = ch;
  destImpl->length = newLength;

  return destImpl->contents;
}

retstring AppendCString( retstring destString, const char* sourceString ) {
  retstringImpl* destStringImpl = NULL;
  retstringImpl* tempStringImpl = NULL;
  size_t newDestReservedLength = 0;
  size_t newDestLength = 0;
  size_t sourceLength = 0;
  size_t availableLength = 0;
  size_t newSize = 0;

  if( destString == NULL ) { return NULL; }
  if( sourceString == NULL ) { return NULL; }

  destStringImpl = (retstringImpl*)(destString - sizeof(retstringImpl));

  sourceLength = strlen(sourceString);

  // [Logic 02]: Check for potential wraparound of new length
  availableLength = ((size_t)(-1)) - destStringImpl->length - 1;
  if( availableLength < sourceLength ) { return NULL; }

  // [Logic 03]: Check for potential wraparound of new reserved length
  newDestLength = destStringImpl->length + sourceLength;
  newDestReservedLength = (newDestLength + 8) & (~(size_t)-1);

  if( destStringImpl->reservedLength < newDestReservedLength ) {
    // [Logic 04]: Check for potential wraparound of new size
    newSize = sizeof(retstringImpl) + newDestReservedLength;
    if( newSize < newDestLength ) { return NULL; }

    tempStringImpl = realloc(destStringImpl, newSize);
    if( tempStringImpl == NULL ) { return NULL; }

    destStringImpl = tempStringImpl;
    destStringImpl->reservedLength = newDestReservedLength;
   }

  memcpy( &destStringImpl->contents[destStringImpl->length],
    sourceString, sourceLength );

  destStringImpl->contents[newDestLength] = 0;
  destStringImpl->length = newDestLength;

  return destStringImpl->contents;
}

retstring AppendString( retstring destString, retstring sourceString ) {
  // The resulting reservedLength will be the greater of either
  // destReservedLength or (destLength + sourceLength + padding)
  // to prevent excessive memory consumption by adding
  // sourceReservedLength to destReservedLength on each call
  retstringImpl* destStringImpl = NULL;
  retstringImpl* sourceStringImpl = NULL;
  retstringImpl* tempStringImpl = NULL;
  size_t newDestReservedLength = 0;
  size_t newDestLength = 0;
  size_t availableLength = 0;
  size_t newSize = 0;

  if( destString == NULL ) { return NULL; }
  if( sourceString == NULL ) { return NULL; }

  destStringImpl = (retstringImpl*)(destString - sizeof(retstringImpl));
  sourceStringImpl = (retstringImpl*)(sourceString - sizeof(retstringImpl));

  // [Logic 02]: Check for potential wraparound of new length
  availableLength = ((size_t)(-1)) - destStringImpl->length - 1;
  if( availableLength < sourceStringImpl->length ) { return NULL; }

  // [Logic 03]: Check for potential wraparound of new reserved length
  newDestLength = destStringImpl->length + sourceStringImpl->length;
  newDestReservedLength = (newDestLength + 8) & (~(size_t)-1);

  if( destStringImpl->reservedLength < newDestReservedLength ) {
    // [Logic 04]: Check for potential wraparound of new size
    newSize = sizeof(retstringImpl) + newDestReservedLength;
    if( newSize < newDestLength ) { return NULL; }

    tempStringImpl = realloc(destStringImpl, newSize);
    if( tempStringImpl == NULL ) { return NULL; }

    destStringImpl = tempStringImpl;
    destStringImpl->reservedLength = newDestReservedLength;
   }

  memcpy( &destStringImpl->contents[destStringImpl->length],
    sourceString, sourceStringImpl->length );

  destStringImpl->contents[newDestLength] = 0;
  destStringImpl->length = newDestLength;

  return destStringImpl->contents;
}

int CompareToCString( retstring left, const char* right ) {
  return (left && right) ? strcmp(left, right) : (255 + 3);
}

int CompareStrings( retstring left, retstring right ) {
  return (left && right) ? strcmp(left, right) : (255 + 3);
}

int CompareToCStringNC( retstring left, const char* right ) {
  return (left && right) ? _stricmp(left, right) : (255 + 3);
}

int CompareStringsNC( retstring left, retstring right ) {
  return (left && right) ? _stricmp(left, right) : (255 + 3);
}

/*
 *  Symbol table implementation
 */

/*
 *  Code generator implementation
 */

Executable* CreateExecutable( const retstring fileName ) {
  return NULL;
}

unsigned CloseExecutable( Executable** executablePtr ) {
  return 2;
}

/*
 *  Parser implementation
 */

Parser* OpenSource( const retstring fileName ) {
  return NULL;
}

unsigned CloseSource( Parser** parserPtr ) {
  return 2;
}

int ReadChar( Parser* source ) {
  return EOF;
}
