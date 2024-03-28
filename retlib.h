#ifndef H_RETLIB
#define H_RETLIB

#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

/*
 *  String declarations
 */

#define STRING_PADDING 8 // Includes NULL byte

typedef struct String {
  size_t maxLength;
  size_t length;
  char data[];
} String;

String* NewString( size_t maxLength );
String* DuplicateCString( const char* cstring );
String* DuplicateString( const String*  );
unsigned ReleaseString( String** stringPtr );

String* AppendString( String* destString, const char* sourceString );

int CompareToCString( String* left, const char* right );
int CompareStrings( String* left, String* right );

/*
 *  Symbol table declarations
 */

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

#endif
