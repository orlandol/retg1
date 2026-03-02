
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl_tree.h"

typedef void (*ReleaseSymbolFunc)( struct Symbol** symbolPtr );

typedef struct GlobalVarSymbol {
  unsigned typeSpec;
} GlobalVarSymbol;

typedef struct Symbol {
  ReleaseSymbolFunc releaseSymbol;

  char* name;
  unsigned symtype;

  union {
    GlobalVarSymbol globalVarSymbol;
  };

  struct avl_tree_node node;
} Symbol;

void ReleaseSymbol( Symbol** symbolPtr ) {
  Symbol* symbol = NULL;

  if( symbolPtr ) {
    symbol = (*symbolPtr);

    if( symbol ) {
      if( symbol->name ) {
        free( symbol->name );
        symbol->name = NULL;
      }

      symbol->symtype = 0;
    }

    free( (*symbolPtr) );
    (*symbolPtr) = NULL;
  }
}

void ReleaseGlobalVarSymbol( Symbol** symbolPtr ) {
  Symbol* symbol = NULL;

  if( symbolPtr ) {
    symbol = (*symbolPtr);

    if( symbol ) {
      symbol->globalVarSymbol.typeSpec = 0;
    }
  }

  ReleaseSymbol( symbolPtr );
}

typedef struct SymbolTable {
  struct avl_tree_node* root;
  unsigned symCount;
} SymbolTable;

int CompareSymbols( const struct avl_tree_node *leftNode,
                   const struct avl_tree_node *rightNode ) {
  Symbol* leftSym = NULL;
  Symbol* rightSym = NULL;

  if( !(leftNode && rightNode) ) { return 256; }

  leftSym = avl_tree_entry(leftNode, Symbol, node);
  rightSym = avl_tree_entry(rightNode, Symbol, node);
  if( !(leftSym && rightSym) ) { return 257; }

  return strcmp(leftSym->name, rightSym->name);
}

SymbolTable* CreateSymbolTable() {
  return calloc(1, sizeof(SymbolTable));
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
  SymbolTable* symbolTable = NULL;
  Symbol* curSymbol = NULL;
  ReleaseSymbolFunc releaseSymbol = NULL;

  if( symbolTablePtr ) {
    symbolTable = (*symbolTablePtr);

    if( symbolTable ) {
      avl_tree_for_each_in_postorder( curSymbol,
          symbolTable->root, Symbol, node ) {

        avl_tree_remove( &(symbolTable->root), &(curSymbol->node) );
        avl_tree_node_set_unlinked( &(curSymbol->node) );

        releaseSymbol = ReleaseSymbol;
        if( curSymbol->releaseSymbol ) {
          releaseSymbol = curSymbol->releaseSymbol;
        }
        releaseSymbol( &curSymbol );
      }

      symbolTable->symCount = 0;
    }

    free( (*symbolTablePtr) );
    (*symbolTablePtr) = NULL;
  }
}

unsigned DeclareSymbol( SymbolTable* symbolTable, Symbol* symbol ) {
  struct avl_tree_node* existingNode = NULL;

  if( symbolTable == NULL ) { return 1; }
  if( !(symbol && symbol->name && (*symbol->name)) ) { return 2; }

  if( symbolTable->symCount == ((unsigned)-1) ) { return 4; }
  symbolTable->symCount++;

  existingNode = avl_tree_insert( &(symbolTable->root),
      &(symbol->node), CompareSymbols );
  if( existingNode ) { return 3; }

  return 0;
}

unsigned DeclareGlobalVar( SymbolTable* symbolTable,
    const char* varName, unsigned varTypespec ) {
  Symbol* newSymbol = NULL;

  if( symbolTable == NULL ) { return 1; }
  if( !(varName && (*varName)) ) { return 2; }
  if( varTypespec == 0 ) { return 3; }

  newSymbol = calloc(1, sizeof(Symbol));
  if( newSymbol == NULL ) { return 4; }

  newSymbol->releaseSymbol = ReleaseGlobalVarSymbol;
  newSymbol->name = strdup(varName);
  newSymbol->globalVarSymbol.typeSpec = varTypespec;
  avl_tree_node_set_unlinked( &(newSymbol->node) );

  if( DeclareSymbol(symbolTable, newSymbol) ) {
    ReleaseGlobalVarSymbol( &newSymbol );

    return 5;
  }

  return 0;
}

SymbolTable* symtab = NULL;

void Cleanup() {
  ReleaseSymbolTable( &symtab );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  symtab = CreateSymbolTable();

  printf( "DeclareGlobalVar(...'test'...) == %u\n", DeclareGlobalVar( symtab, "test", 1234 ) );

  Cleanup();

  printf( "Done\n" );

  return 0;
}

#include "avl_tree.c"
