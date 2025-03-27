
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

  paddedReservedLength = (reserveLength + 8) & STRING_PADMASK;

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

  newReservedLength = (destStringImpl->length + 8) & STRING_PADMASK;
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

  newReservedLength = (destReservedLength + STRING_PADDING)
                      & STRING_PADMASK;
  newLength = destLength + 1;

  if( newLength >= destReservedLength ) {
    newSize = sizeof(retstringImpl) + newReservedLength;
    if( newSize < newReservedLength ) { return NULL; }

    tmpImpl = realloc(destImpl, newSize);
    if( tmpImpl == NULL ) { return NULL; }

    destImpl = tmpImpl;
    destImpl->reservedLength = newReservedLength;
  }

  destString[destLength] = ch;
  destString[newLength] = '\0';
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
  newDestReservedLength = (newDestLength + STRING_PADDING)
                          & STRING_PADMASK;

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
  newDestReservedLength = (newDestLength + STRING_PADDING)
                          & STRING_PADMASK;

  // The resulting reservedLength will be the greater of either
  // destReservedLength or (destLength + sourceLength + padding)
  // to prevent excessive memory consumption by adding
  // sourceReservedLength to destReservedLength on each call
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

SymbolTable* NewSymbolTable( void ) {
  return calloc(1, sizeof(SymbolTable));
}

int CompareSymbolsByName( const struct avl_tree_node* left,
    const struct avl_tree_node* right ) {
  Symbol* leftSymbol = NULL;
  Symbol* rightSymbol = NULL;

  if( left ) { leftSymbol = avl_tree_entry(left, Symbol, node); }
  if( right ) { rightSymbol = avl_tree_entry(right, Symbol, node); }

  if( leftSymbol == NULL ) { return 256 + 1; }
  if( rightSymbol == NULL ) { return 256 + 2; }

  return CompareStrings(leftSymbol->name, rightSymbol->name);
}

int CompareSymbolName( const void* symbolName,
    const struct avl_tree_node* symbolNode ) {
  Symbol* symbol = NULL;

  if( !(symbolName && (*(retstring)symbolName)) ) { return 256 + 1; }

  if( symbolNode ) { symbol = avl_tree_entry(symbolNode, Symbol, node); }
  if( symbol == NULL ) { return 256 + 2; }

  return CompareStrings(symbol->name, ((retstring)symbolName));
}

void ReleaseSymbol( Symbol** symbolPtr ) {
  unsigned result;

  if( symbolPtr ) {
    if( (*symbolPtr) ) {
      if( (*symbolPtr)->symbolDestructor ) {
        result = (*symbolPtr)->symbolDestructor( (*symbolPtr) );

        if( (*symbolPtr)->name ) {
          ReleaseString( &((*symbolPtr)->name) );
        }

        (*symbolPtr)->symbolType = 0;
      }

      free( (*symbolPtr) );
      (*symbolPtr) = NULL;
    }
  }
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
  Symbol* symbol = NULL;

  if( symbolTablePtr ) {
    if( (*symbolTablePtr) ) {
      avl_tree_for_each_in_postorder(symbol,
          (*symbolTablePtr)->root, Symbol, node) {
        if( symbol ) {
          ///BUG: remove before unlinked? Or vice-versa?
          avl_tree_remove( &((*symbolTablePtr)->root), symbol->node );
          avl_tree_node_set_unlinked( symbol->node );

          ReleaseSymbol( &symbol );
        }
      }

      free( (*symbolTablePtr) );
      (*symbolTablePtr) = NULL;
    }
  }
}

unsigned LookupSymbol( SymbolTable* symbolTable,
    const retstring symbolName, Symbol** symbolPtr ) {
  struct avl_tree_node* symbolNode = NULL;

  if( symbolTable == NULL ) { return 1; }
  if( !(symbolName && (*symbolName)) ) { return 2; }
  if( symbolPtr == NULL ) { return 3; }

  symbolNode = avl_tree_lookup(symbolTable->root, symbolName,
      CompareSymbolName);
  if( symbolNode == NULL ) { return 4; }

  (*symbolPtr) = avl_tree_entry(symbolNode, Symbol, node);

  return 0;
}

int EmptyCompare( const struct avl_tree_node* left,
    const struct avl_tree_node* right ) {

  return 1;
}

///BUG: Test program crashes in this function
unsigned DeclareSymbol( SymbolTable* symbolTable, Symbol* symbol ) {
  if( symbolTable == NULL ) { return 1; }
  if( symbol == NULL ) { return 2; }

  if( symbolTable->count == (unsigned)(-1) ) { return 3; }

  if( avl_tree_insert(&(symbolTable->root), symbol->node,
      EmptyCompare) ) {
//      CompareSymbolsByName) ) {
    return 4; //Error: Duplicate entry
  }

  symbolTable->count++;

  return 0;
}

unsigned RemoveSymbol( SymbolTable* symbolTable,
    const retstring symbolName ) {
  struct avl_tree_node* symbolNode = NULL;
  Symbol* symbol = NULL;

  if( !(symbolTable && symbolTable->count) ) { return 1; }
  if( !(symbolName && (*symbolName)) ) { return 2; }

  symbolNode = avl_tree_lookup(symbolTable->root, symbolName,
    CompareSymbolName);
  if( symbolNode == NULL ) { return 3; }

  symbol = avl_tree_entry(symbolNode, Symbol, node);
  if( symbol == NULL ) { return 4; }

  ///BUG: remove before unlinked? Or vice-versa?
  avl_tree_remove( &(symbolTable->root), symbolNode );
  avl_tree_node_set_unlinked( symbolNode );

  ReleaseSymbol( &symbol );

  symbolTable->count--;

  return 0;
}


unsigned RunSymbolDestructor( Symbol* runSymbol ) {
  if( runSymbol == NULL ) { return 1; }

  return 2;
}

unsigned DeclareRun( SymbolTable* symbolTable, unsigned entryPoint ) {
  Symbol* newSymbol = NULL;
  unsigned result = 0;

  if( symbolTable == NULL ) { return 1; }

  newSymbol = calloc(1, sizeof(Symbol));
  if( newSymbol == NULL ) { return 3; }

  newSymbol->name = DuplicateCString("run");
  newSymbol->symbolDestructor = RunSymbolDestructor;
  newSymbol->symbolType = symRun;
  newSymbol->data.runSymbol.entryPoint = entryPoint;
  result = DeclareSymbol(symbolTable, newSymbol);
  if( result ) {
    ReleaseSymbol( &newSymbol );
    return 4;
  }

  return 0;
}

unsigned RemoveRun( SymbolTable* symbolTable ) {
  return RemoveSymbol(symbolTable, "run");
}

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
