
#include "avl_tree.h"
#include "retlib.h"

unsigned errorCount = 0;

SymbolTable* symTab = NULL;

void TestNewSymbolTable( unsigned testNumber, const char* testExpression,
    SymbolTable* symbolTable ) {

  if( symbolTable == NULL ) {
    printf( "[%02u] '%s' unexpectedly returned "
            "a NULL pointer\n", testNumber, testExpression
    );
    errorCount++;
    return;
  }

  if( symbolTable->root != NULL ) {
    printf( "[%02u] '%s': field root is not NULL as expected.\n",
        testNumber, testExpression
    );
    errorCount++;
  }

  if( symbolTable->count != 0 ) {
    printf( "[%02u] '%s': field count is %u when it should be 0.\n",
        testNumber, testExpression, symbolTable->count
    );
    errorCount++;
  }
}

void TestReleaseSymbolTable( unsigned testNumber,
    const char* testExpression, SymbolTable* symbolTable ) {

  if( symbolTable ) {
    printf( "[%02u] '%s': symbolTable is not NULL as expected.\n",
      testNumber, testExpression
    );
    errorCount++;
  }
}

void ShowSymbols( SymbolTable* symbolTable ) {
  Symbol* symbol = NULL;

  if( !(symbolTable && symbolTable->root) ) {
    printf( "Symbol table is empty\n" );
  } else {
    avl_tree_for_each_in_postorder(symbol, symbolTable->root,
        Symbol, node) {
      printf( "Symbol: '%s'; SymbolType: %u\n",
          symbol->name, symbol->symbolType );
    }
  }
}

int main( int argc, char* argv[] ) {

  printf( "[Starting symbol table tests]\n" );

  symTab = NewSymbolTable();
  TestNewSymbolTable( 1, "symTab = NewSymbolTable()", symTab );

  DeclareRun( symTab, 1234 );
  ShowSymbols( symTab );

  ReleaseSymbolTable( &symTab );
  TestReleaseSymbolTable( 1, "ReleaseSymbolTable( &symTab )", symTab );

  if( errorCount ) {
    printf( "[Symbol table tests failed with %u errors]\n", errorCount );
  } else {
    printf( "[Symbol table tests passed]\n" );
  }

  return 0;
}
