#ifndef H_RETLIB
#define H_RETLIB

#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

/*
 *  String declarations
 */

#define STRING_PADDING 8 // Includes NULL byte

typedef struct retstringImpl {
  size_t reservedLength;
  size_t length;
  char contents[];
} retstringImpl;

typedef char* retstring;

retstring NewString( size_t maxLength );
retstring DuplicateCString( const char* cstring );
retstring DuplicateString( const retstring  );
retstring CompactString( retstring destString );
unsigned ReleaseString( retstring* retstringPtr );

size_t StringLength( retstring source );
size_t StringReservedLength( retstring source );

unsigned ClearString( retstring destString );

retstring AppendChar( retstring destString, char ch );
retstring AppendCString( retstring destString, const char* sourceString );
retstring AppendString( retstring destString, retstring sourceString );

int CompareToCString( retstring left, const char* right );
int CompareStrings( retstring left, retstring right );

int CompareToCStringNC( retstring left, const char* right );
int CompareStringsNC( retstring left, retstring right );

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

Executable* CreateExecutable( const retstring fileName );
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

Parser* OpenSource( const retstring fileName );
unsigned CloseSource( Parser** parserPtr );

int ReadChar( Parser* source );

#endif
