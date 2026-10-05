
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

enum PointerType {
  ptrData = 1,
  ptrDataOwner
};

enum Token {
  typeBool = 256,
  typeChar,
  typeFsize,
  typeFunc,
  typeInt,
  typeMethod,
  typeTsize,
  typeUint
};

typedef struct SymbolTable {
  char* name;
//  struct avl_tree_node root;
} SymbolTable;

enum CodeType {
  ctAsm = 1
};

enum FrameType {
  ftFrame = 1,
  ftNoframe
};

enum ParamOrder {
  poForward = 1,
  poReverse
};

typedef struct CallSpec {
  char* name; // cdecl | pascal | stdcall | ...
  unsigned codeType; // asm
  unsigned frameType; // frame | noframe
  unsigned paramOrder; // forward | reverse
} CallSpec;

// [@] [baseType[:precision] | typeName] [ '[' ArrayDimensions ']' ]
typedef struct TypeSpec {
  unsigned pointerType; // [@]
  unsigned baseType; // [baseType[:precision]]
  unsigned basePrecision; // [:precision]
  char* typeName; // typeName if simpleType is 0
  SymbolTable arrayDimensions; // ['[' ArrayDimensions ']']
} TypeSpec;

typedef struct FuncSpec {
  unsigned fields;
  CallSpec callspec;
  TypeSpec returnType;
  SymbolTable paramList;
} FuncSpec;

typedef struct FuncType {
  unsigned fields;
  CallSpec callspec;
  TypeSpec returnType;
  SymbolTable paramList;
  SymbolTable arrayDimensions; // ['[' ArrayDimensions ']']
} FuncType;

typedef struct MethodSpec {
  unsigned fields;
  CallSpec callspec;
  TypeSpec returnType;
  SymbolTable paramList;
} MethodSpec;

typedef struct MethodType {
  unsigned fields;
  CallSpec callspec;
  TypeSpec returnType;
  SymbolTable paramList;
  SymbolTable arrayDimensions; // ['[' ArrayDimensions ']']
} MethodType;

char* MangleFuncName( char* funcName, FuncSpec* funcSpec ) {
  char* mangledName = NULL;
  char* callcon = NULL;
  int mangledLength = 0;

  if( !(funcName && *funcName && funcSpec) ) { return NULL; }

  mangledLength = snprintf( NULL, 0, "f%s@%s@%s@%s",
    funcName, funcSpec->callspec.name ? funcSpec->callspec.name : "",
    "",
    ""
  ); 
  if( mangledLength <= 0 ) { return NULL; }

  mangledName = malloc((mangledLength + 1) * sizeof(char));
  if( mangledName == NULL ) { return NULL; }

  snprintf( mangledName, mangledLength + 1, "f%s@%s@%s@%s",
    funcName, funcSpec->callspec.name ? funcSpec->callspec.name : "",
    "",
    ""
  );

  return mangledName;
}

char* MangleOperator( FuncSpec* funcSpec ) {
  return NULL;
}

char* MangleMethodName( FuncSpec* funcSpec ) {
  return NULL;
}

int main( int argc, char** argv ) {
  FuncSpec funcSpec = { };
  char* funcName = NULL;

  funcSpec.callspec.name = "cdecl";

  funcName = MangleFuncName("fn", &funcSpec);
  printf( "MangleFuncName('fn', ...) == '%s'\n", funcName );
  if( funcName ) {
    free( funcName );
    funcName = NULL;
  }
  
  return 0;
}
