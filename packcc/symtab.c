
#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

typedef void (*ReleaseSymbolFunc)( struct avl_tree_node** nodePtr );

typedef struct Symbol {
  ReleaseSymbolFunc release;
  unsigned symbolType;
  unsigned symbolID;
  unsigned value;
  struct avl_tree_node* node;
} Symbol;

typedef struct SymbolTable {
  struct avl_tree_node* symbols;
  unsigned symbolCount;
} SymbolTable;

int CompareSymbols( const struct avl_tree_node* leftNode, const struct avl_tree_node* rightNode ) {
  Symbol* leftSymbol = NULL;
  Symbol* rightSymbol = NULL;

  if( leftNode == NULL ) { return 255 + 1; }
  if( rightNode == NULL ) { return 255 + 2; }

  leftSymbol = avl_tree_entry(leftNode, Symbol, node);
  rightSymbol = avl_tree_entry(rightNode, Symbol, node);

  return (int)(leftSymbol->symbolID - rightSymbol->symbolID);
}

SymbolTable* CreateSymbolTable() {
  return calloc(1, sizeof(SymbolTable));
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
  if( symbolTablePtr ) {
    if( (*symbolTablePtr) ) {
      ///TODO: Release all nodes

      free( (*symbolTablePtr) );
      (*symbolTablePtr) = NULL;
    }
  }
}

unsigned DeclareSymbol( SymbolTable* symbolTable, Symbol* symbol ) {
  int result = 0;

  if( symbolTable == NULL ) { return 1; }
  if( symbol == NULL ) { return 2; }

  if( avl_tree_insert(&(symbolTable->symbols), symbol->node, CompareSymbols) ) {
    return 3;
  }

  return 0;
}

int main( int argc, char** argv ) {
  SymbolTable* symtab = NULL;

  symtab = CreateSymbolTable();

  ReleaseSymbolTable( &symtab );

  return 0;
}

#include "avl_tree.c"
