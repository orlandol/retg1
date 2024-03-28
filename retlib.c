
#include "avl_tree.h"

#include "retlib.h"

/*
 *  String implementation
 */

String* NewString( size_t maxLength ) {
  String* newString = NULL;
  size_t newMaxLength = 0;
  size_t newPaddedMaxLength = 0;

  newMaxLength = maxLength + 1;
  newPaddedMaxLength = (newMaxLength + 8) & ((size_t)~7);

  if( (sizeof(String) + newMaxLength) >= sizeof(String) ) {
    newString = calloc(1, sizeof(String) + newPaddedMaxLength);

    if( newString ) {
      newString->maxLength = newPaddedMaxLength;
    }
  }

  return newString;
}

String* DuplicateCString( const char* cstring ) {
  return NULL;
}

String* DuplicateString( const String* sourceString ) {
  return NULL;
}

unsigned ReleaseString( String** stringPtr ) {
  return 2;
}

String* AppendString( String* destString, const char* sourceString ) {
  return NULL;
}

int CompareToCString( String* left, const char* right ) {
  return 3;
}

int CompareStrings( String* left, String* right ) {
  return 3;
}

/*
 *  Symbol table implementation
 */

/*
 *  Code generator implementation
 */

Executable* CreateExecutable( const String* fileName ) {
  return NULL;
}

unsigned CloseExecutable( Executable** executablePtr ) {
  return 2;
}

/*
 *  Parser implementation
 */

Parser* OpenSource( const String* fileName ) {
  return NULL;
}

unsigned CloseSource( Parser** parserPtr ) {
  return 2;
}

int ReadChar( Parser* source ) {
  return EOF;
}
