#ifndef H_RETLIB
#define H_RETLIB

#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

/*
 *  String declarations
 */

// Includes NULL byte
#define STRING_PADDING ( sizeof(retstringImpl) )

// Pre-define a mask to remove unaligned bits
#define STRING_PADMASK ( ~ (size_t)(7) )

// ex: 0x...F8
#define STRING_MAXSIZE ( ((size_t)(-1) & STRING_PADMASK) )

// ex: 0x...F0
#define STRING_MAXRESERVED ( STRING_MAXSIZE - sizeof(retstringImpl) )

// STRING_MAXRESERVED - NULL byte
#define STRING_MAXLENGTH ( STRING_MAXRESERVED - 1 )

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

typedef struct Symbol {
  retstring* name;
} Symbol;

typedef struct SymbolTable {
  void* root;
} SymbolTable;

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
