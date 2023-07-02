
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

#define SYMBOLREF(nodeVar) (nodeVar ? avl_tree_entry(nodeVar, Symbol, node) : NULL)

void ReleaseSymbol( Symbol** symbolPtr );
void ReleaseSymbolTable( SymbolTable* symbolTable );

int CompareSymbolNames( const struct avl_tree_node* leftNode,
  const struct avl_tree_node* rightNode );

int DeclareConst( SymbolTable* symbolTable,
    char* constName, SymbolValue* constValue );
void ReleaseConst( Symbol** symbolPtr );

Symbol* Lookup( SymbolTable* symbolTable, const char* symbolName );

SymbolTable symtab;

void Cleanup() {
  ReleaseSymbolTable( &symtab );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  SymbolValue constVal = {};
  constVal.vtype = valUint;
  constVal.uintValue = 123;

  DeclareConst( &symtab, sdsnew("Test"), &constVal );

  Symbol* testConst = NULL;
  testConst = Lookup(&symtab, "Test");
  printf( "Lookup(%s): %u\n", testConst ? testConst->name : "",
    testConst ? testConst->constSym.value.uintValue : 0 );

  return 0;
}

#include "3rdparty/sds/sds.c"

#include "3rdparty/avl_tree/avl_tree.c"

void ReleaseSymbol( Symbol** symbolPtr ) {
  if( symbolPtr ) {
    if( (*symbolPtr) ) {
      if( (*symbolPtr)->release ) {
        (*symbolPtr)->release( symbolPtr );
      }

      free( (*symbolPtr) );
      (*symbolPtr) = NULL;
    }
  }
}

void ReleaseSymbolTable( SymbolTable* symbolTable ) {
  Symbol* symbolToDelete = NULL;

  if( symbolTable && symbolTable->root ) {
    //avl_tree_for_each_in_postorder(child_struct, root, struct_name, struct_member)
    avl_tree_for_each_in_postorder(symbolToDelete, symbolTable->root, Symbol, node) {
      printf( "Releasing symbol(%s)\n", symbolToDelete->name );

      ReleaseSymbol( &symbolToDelete );
    }
  }
}

int CompareNameToSymbolName( const void* symbolName,
  const struct avl_tree_node* symbol ) {

  if( symbolName &&
      symbol && SYMBOLREF(symbol)->name ) {
    return strcmp((const char*)symbolName, SYMBOLREF(symbol)->name);
  }

  return -1;
}

int CompareSymbolNames( const struct avl_tree_node* leftNode,
  const struct avl_tree_node* rightNode ) {

  if( leftNode && SYMBOLREF(leftNode)->name &&
      rightNode && SYMBOLREF(rightNode)->name ) {
    return strcmp(SYMBOLREF(leftNode)->name, SYMBOLREF(rightNode)->name);
  }

  return -1;
}

int DeclareConst( SymbolTable* symbolTable,
    char* constName, SymbolValue* constValue ) {
  Symbol* newSymbol = NULL;

  if( symbolTable && constName && (*constName) &&
      constValue && constValue->vtype ) {

    if( Lookup(symbolTable, constName) ) {
      return 0;
    }

    newSymbol = calloc(1, sizeof(Symbol));
    if( newSymbol == NULL ) { return; }

    newSymbol->release = ReleaseConst;
    newSymbol->name = sdsnew(constName);
    newSymbol->stype = stypeConst;
    newSymbol->constSym.value = *constValue;

    if( avl_tree_insert(&symbolTable->root,
        &newSymbol->node, CompareSymbolNames) == NULL ) {
      return -1;
    }

    ReleaseConst( &newSymbol );
  }

  return 0;
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

Symbol* Lookup( SymbolTable* symbolTable, const char* symbolName ) {
  struct avl_tree_node* symbolNode = NULL;

  if( symbolTable && symbolTable->root && symbolName && (*symbolName) ) {
    symbolNode = avl_tree_lookup(symbolTable->root, symbolName,
        CompareNameToSymbolName);
  }

  return SYMBOLREF(symbolNode);
}
