
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "avl_tree.h"

/*
 *  String declarations
 */

typedef struct String {
  size_t maxLength;
  size_t length;
  char data[];
} String;

String* NewString( size_t maxLength );
String* DuplicateCString( const char* cstring );
String* DuplicateString( const String* sourceString );
unsigned ReleaseString( String** stringPtr );

String* AppendString( String* destString, const char* sourceString );

/*
 *  Code generator declarations
 */

// Instruction buffer
typedef struct X86Instruction {
  unsigned fieldsUsed;
} X86Instruction;

typedef struct Executable {
  FILE* handle;
} Executable;

Executable* CreateExecutable( const String* fileName );
unsigned CloseExecutable( Executable** executablePtr );

/*
 *  Parser declarations
 */
typedef struct Parser {
  FILE* handle;

  unsigned line;
  unsigned column;

  unsigned nextLine;
  unsigned nextColumn;

  char ch;
  char nextCh;
} Parser;

Parser* OpenSource( const String* fileName );
unsigned CloseSource( Parser** parserPtr );

int ReadChar( Parser* source );

/*
 *  Main program
 */

Parser* retFile = NULL;
Executable* exeFile = NULL;

unsigned ParseOptions( int runArgsCount, const char* runArgs[] ) {
  return 3;
}

void Cleanup( void ) {
  CloseSource( &retFile );
  CloseExecutable( &exeFile );
}

int main( int argc, char* argv[] ) {
  atexit( Cleanup );

  return 0;
}

/*
 *  String implementation
 */

String* NewString( size_t maxLength ) {
  return NULL;
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
