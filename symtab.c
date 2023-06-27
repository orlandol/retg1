
#include <stdlib.h>
#include <stdio.h>

#include "3rdparty/sds/sdsalloc.h"
#include "3rdparty/sds/sds.h"

#include "3rdparty/avl_tree/avl_tree.h"

enum ValueType {
  valNone = 0,
  valUint,
  valInt,
  valString,
  valChar,
  valBool
};

typedef union SymbolValue {
  unsigned vtype;
  union {
    unsigned uintValue;
    int intValue;
    sds stringValue;
    char charValue;
    unsigned boolValue;
  };
} SymbolValue;

enum SymbolType {
  stypeUndefined = 0,
  stypeConst
};

typedef struct ConstSymbol {
  SymbolValue value;
} ConstSymbol;

typedef struct Symbol {
  void (*release)( struct Symbol** symbol );

  sds name;

  unsigned stype;
  union {
    ConstSymbol constSym;
  };

  struct avl_tree_node node;
} Symbol;

typedef struct SymbolTable {
  Symbol* root;
} SymbolTable;

typedef struct ParseState {
  SymbolTable symtab;
} ParseState;

#define SYMBOLREF(nodeVar) avl_tree_entry(nodeVar, Symbol, node)

void ReleaseSymbolTable( SymbolTable** symbolTablePtr );

int CompareSymbolNames( const struct avl_tree_node* leftNode,
  const struct avl_tree_node* rightNode );

void DeclareConst( SymbolTable* symbolTable,
    const sds constName, SymbolValue* constValue );
void ReleaseConst( Symbol** symbolPtr );

Symbol* Lookup( SymbolTable* symbolTable, const sds symbolName );

SymbolTable symtab;

void Cleanup() {
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  SymbolValue constVal = {};
  constVal.vtype = valUint;
  constVal.uintValue = 123;
  DeclareConst( &symtab, sdsnew("Test"), &constVal );

  return 0;
}

#include "3rdparty/sds/sds.c"

#include "3rdparty/avl_tree/avl_tree.c"

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
}

int CompareSymbolNames( const struct avl_tree_node* leftNode,
  const struct avl_tree_node* rightNode ) {

  if( leftNode && SYMBOLREF(leftNode)->name &&
      rightNode && SYMBOLREF(rightNode)->name ) {
    return strcmp(SYMBOLREF(leftNode)->name, SYMBOLREF(rightNode)->name);
  }

  return -1;
}

void DeclareConst( SymbolTable* symbolTable,
    const sds constName, SymbolValue* constValue ) {
  Symbol* newSymbol = NULL;

  if( symbolTable && constName && (*constName) &&
      constValue && constValue->vtype ) {

    newSymbol = calloc(1, sizeof(Symbol));
    if( newSymbol == NULL ) { return; }

    newSymbol->release = ReleaseConst;
    newSymbol->stype = stypeConst;
    newSymbol->constSym.value = (*constValue);

    if( avl_tree_insert(&symbolTable->root,
        &newSymbol->node, CompareSymbolNames) != NULL ) {

      free( newSymbol );
      newSymbol = NULL;
    }
  }
}

void ReleaseConst( Symbol** symbolPtr ) {
  if( symbolPtr ) {
    if( (*symbolPtr) ) {
      (*symbolPtr)->release = NULL;
      (*symbolPtr)->stype = 0;
      sdsfree( (*symbolPtr)->name );

      free( (*symbolPtr) );
      (*symbolPtr) = NULL;
    }
  }
}

Symbol* Lookup( SymbolTable* symbolTable, const sds symbolName ) {
  Symbol searchSymbol = {};

  if( symbolTable && symbolTable->root && symbolName && (*symbolName) ) {
    searchSymbol.name = symbolName;
    return SYMBOLREF(avl_tree_lookup_node(symbolTable->root,
        &searchSymbol.node, CompareSymbolNames));
  }
  return NULL;
}
