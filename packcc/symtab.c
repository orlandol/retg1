
#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

typedef struct Symbol Symbol;
typedef void (*ReleaseSymbolFunc)( Symbol* symbol );

enum SymbolType {
  symbolRun = 1
};

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
  Symbol* currentSymbol = NULL;

  if( symbolTablePtr ) {
    if( (*symbolTablePtr) ) {
      avl_tree_for_each_in_postorder( currentSymbol, (*symbolTablePtr)->symbols, Symbol, node ) {
        avl_tree_remove( &((*symbolTablePtr)->symbols),currentSymbol->node );
        avl_tree_node_set_unlinked( currentSymbol );

        if( currentSymbol ) {
          if( currentSymbol->release ) {
            currentSymbol->release( currentSymbol );
          }

          free( currentSymbol );
          currentSymbol = NULL;
        }
      }

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

void ReleaseSymbolRun( Symbol* symbol ) {
}

unsigned DeclareRun( SymbolTable* symbolTable, unsigned entryPoint ) {
  Symbol* newSymbol = NULL;
  unsigned result = 0;

  if( symbolTable == NULL ) { return 1; }

  newSymbol = calloc(1, sizeof(Symbol));
  if( newSymbol == NULL ) { return 3; }

  newSymbol->release = ReleaseSymbolRun;
  newSymbol->symbolType = symbolRun;
  newSymbol->symbolID = 1234;
  newSymbol->value = entryPoint;

  result = DeclareSymbol(symbolTable, newSymbol);
  if( result == 0 ) { return 0; }

  if( newSymbol ) {
    free( newSymbol );
  }
  return 4;
}

int main( int argc, char** argv ) {
  SymbolTable* symtab = NULL;
  unsigned result = 0;

  symtab = CreateSymbolTable();

  result = DeclareRun(symtab, 1111);

  ReleaseSymbolTable( &symtab );

  return 0;
}

#include "avl_tree.c"
