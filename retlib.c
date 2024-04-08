
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
  if( sourceStringImpl->reservedLength & 7 ) {
    return NULL; }

  newStringImpl = calloc(1, sourceStringImpl->reservedLength);
  if( newStringImpl == NULL ) { return NULL; }

  *newStringImpl = *sourceStringImpl;

  memcpy( newStringImpl->contents, sourceStringImpl->contents,
    sourceStringImpl->reservedLength );

  return newStringImpl->contents;
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
  return NULL;
}

retstring AppendString( retstring destString, retstring sourceString ) {
  return NULL;
}

int CompareToCString( retstring left, const char* right ) {
  return 3;
}

int CompareStrings( retstring left, retstring right ) {
  return 3;
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
