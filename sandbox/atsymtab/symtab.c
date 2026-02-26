
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl_tree.h"

typedef struct Symbol {
  char* name;
  unsigned symtype;
  struct avl_tree_node node;
} Symbol;

typedef struct SymbolTable {
  struct avl_tree_node* root;
  unsigned symCount;
} SymbolTable;

int compare_nodes( const struct avl_tree_node *leftNode,
                          const struct avl_tree_node *rightNode ) {
  Symbol* leftSym = NULL;
  Symbol* rightSym = NULL;

  if( !(leftNode && rightNode) ) { return 254; }

  leftSym = avl_tree_entry(leftNode, Symbol, node);
  rightSym = avl_tree_entry(rightNode, Symbol, node);
  if( !(leftSym && rightSym) ) { return 255; }

  return strcmp(leftSym->name, rightSym->name);
}

int main( int argc, char** argv ) {
  return 0;
}

#include "avl_tree.c"
