
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
  return NULL;
}

retstring DuplicateString( const retstring sourceString ) {
  return NULL;
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
  return (source ? ((retstringImpl*)(source -
    sizeof(retstringImpl)))->length : 0);
}

size_t StringReservedLength( retstring source ) {
  // Read reserved length from retstring's negative offset header
  return (source ? ((retstringImpl*)(source -
    sizeof(retstringImpl)))->reservedLength : 0);
}

retstring AppendChar( retstring destString, char ch ) {
  retstringImpl* destImpl = NULL;

  if( destString == NULL ) { return NULL; }

  destImpl = (retstringImpl*)(destString - sizeof(retstringImpl));

  return NULL;
}

retstring AppendString( retstring destString, const char* sourceString ) {
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
