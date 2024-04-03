
#include "avl_tree.h"

#include "retlib.h"

/*
 *  String implementation
 */

retstring NewString( size_t maxLength ) {
  retstringImpl* newString = NULL;
  size_t stringSize = 0;
  size_t paddedMaxLength = 0;

  paddedMaxLength = (maxLength + 8) & (~((size_t)7));

  stringSize = sizeof(retstring) + paddedMaxLength;

  if( stringSize > paddedMaxLength ) {
    newString = calloc(1, stringSize);
    if( newString ) {
      newString->maxLength = paddedMaxLength;

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
  return 2;
}

size_t StringLength( retstring source ) {
  return (source ? ((retstringImpl*)(source -
    sizeof(retstringImpl)))->length : 0);
}

size_t StringReservedLength( retstring source ) {
  return (source ? ((retstringImpl*)(source -
    sizeof(retstringImpl)))->maxLength : 0);
}

retstring AppendChar( retstring destString, char ch ) {
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
