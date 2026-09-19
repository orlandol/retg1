
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl_tree.h"

/*
 *  Parser pre-requisites
 */

enum ExpressionType {
  etEmpty = 0,
  etValue,
  etDeferred,
  etExpression
};

typedef struct Expression {
  unsigned type;
} Expression;

/*
 *  Symbol Table declarations
 */

typedef struct Keyword {
  char* name;
  unsigned token;
} Keyword;

enum Token {
  tkEOF = 0,
  tkEOL,

  kwProgram = 128,
  kwCallspec,
  kwType,
  kwNewType,
  kwEnum,
  kwConst,
  kwStruct,
  kwUnion,
  kwVar,
  kwImport,
  kwFuncDecl,
  kwFunc,
  kwObject,
  kwConstructor,
  kwDestructor,
  kwInterface,
  kwMethod,
  kwOperator,
  kwRun,

  typeBool = 256,
  typeChar,
  typeFsize,
  typeFunc,
  typeInt,
  typeMethod,
  typeTsize,
  typeUint
};

extern const Keyword toplevelKeywords[];

typedef struct BaseType {
  char* name;
  unsigned token;
  unsigned precision;
  size_t size;
} BaseType;

extern const BaseType baseType[];

typedef struct SymbolTable {
  char* name;
  struct avl_tree_node root;
} SymbolTable;

typedef struct CallSpec {
  unsigned fields;
  unsigned codeType; // asm
  unsigned frameType; // frame | noframe
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

typedef struct GlobalVar {
  TypeSpec typeSpec;
} GlobalVar;

typedef struct Symbol {
  char* name;
  unsigned type;
  union {
    GlobalVar globalVar;
  };
  struct avl_tree_node node;
} Symbol;

SymbolTable* CreateSymbolTable( char* rootName );
void ReleaseSymbolTable( SymbolTable** symtabPtr );

/*
 *  Code Generator declarations
 */

typedef struct Instruction {
  unsigned fields;
  unsigned prefix;
} Instruction;

typedef struct CodeGen {
  FILE* handle;
} CodeGen;

CodeGen* CreateBinary( const char* binaryPath );
void CloseBinary( CodeGen** codegenPtr );

/*
 *  Parser declarations
 */

typedef struct Parser {
  FILE* handle;
  unsigned line;
  unsigned column;
  char ch;
  unsigned nextLine;
  unsigned nextColumn;
  char nextch;
  unsigned tokenLine;
  unsigned tokenColumn;
} Parser;

Parser* OpenSource( const char* sourcePath );
void CloseSource( Parser** parserPtr );

char PeekChar( Parser* source );
char ReadChar( Parser* source );

unsigned NextToken( Parser* source );

/*
 *  Main declarations
 */

enum Mode {
  modeGenerateNew = 1,
  modeBuildSource
} Mode;

enum TargetOS {
  osWin32 = 1
} TargetOS;

enum OSSubsystem {
  win32Console = 1,
  win32GUI = 2
} OSSubsystem;

typedef struct Options {
  char sourceName[_MAX_PATH];
  char outputName[_MAX_PATH];
  unsigned mode;
  unsigned targetOS;
  unsigned subsystem;
} Options;

Options options;

/*
 *  Symbol Table implementation
 */

SymbolTable* CreateSymbolTable( char* rootName ) {
  SymbolTable* newSymbolTable = NULL;

  if( !(rootName && (*rootName)) ) { return NULL; }
  
  newSymbolTable = calloc(1, sizeof(SymbolTable));
  if( newSymbolTable ) {
    newSymbolTable->name = strdup(rootName);
    if( newSymbolTable->name ) {
      return newSymbolTable;
    }

    ReleaseSymbolTable( &newSymbolTable );
  }

  return NULL;
}

void ReleaseSymbolTable( SymbolTable** symtabPtr ) {
  if( symtabPtr ) {
    if( (*symtabPtr) ) {
      // Cleanup tree
      
      free( (*symtabPtr) );
      (*symtabPtr) = NULL;
    }
  }
}

/*
 *  Code Generator implementation
 */

CodeGen* CreateBinary( const char* binaryPath ) {
  CodeGen* newCodegen = NULL;
  FILE* handle = NULL;

  if( !(binaryPath && *binaryPath) ) { return NULL; }

  handle = fopen(binaryPath, "w+");
  if( handle ) {
    newCodegen = calloc(1, sizeof(CodeGen));
    if( newCodegen ) {
      newCodegen->handle = handle;
      return newCodegen;
    }

    // Cleanup on error
    fclose( handle );
    handle = NULL;
  }

  return NULL;
}

void CloseBinary( CodeGen** codegenPtr ) {
  if( codegenPtr ) {
    if( (*codegenPtr) ) {
      if( (*codegenPtr)->handle ) {
        fclose( (*codegenPtr)->handle );
        (*codegenPtr)->handle = NULL;
      }

      free( (*codegenPtr) );
      (*codegenPtr) = NULL;
    }
  }
}

/*
 *  Parser implementation
 */

const Keyword toplevelKeywords[] = {
  "callspec", kwCallspec,
  "const", kwConst,
  "constructor", kwConstructor,
  "destructor", kwDestructor,
  "enum", kwEnum,
  "func", kwFunc,
  "funcdecl", kwFuncDecl,
  "import", kwImport,
  "interface", kwInterface,
  "method", kwMethod,
  "newtype", kwNewType,
  "object", kwObject,
  "operator", kwOperator,
  "program", kwProgram,
  "run", kwRun,
  "struct", kwStruct,
  "type", kwType,
  "union", kwUnion,
  "var", kwVar
};

const BaseType baseType[] = {
  "bool", typeBool, 8, 1,
  "char", typeChar, 8, 1,
  "fsize", typeFsize, 64, 8,
  "func", typeFunc, 32, 4,
  "int", typeInt, 32, 4,
  "method", typeMethod, 64, 8,
  "tsize", typeTsize, 32, 4,
  "uint", typeUint, 32, 4
};

Parser* OpenSource( const char* sourcePath ) {
  Parser* newParser = NULL;
  FILE* handle = NULL;

  if( !(sourcePath && *sourcePath) ) { return NULL; }

  handle = fopen(sourcePath, "rb");
  if( handle ) {
    newParser = calloc(1, sizeof(Parser));
    if( newParser ) {
      newParser->handle = handle;

      ReadChar(newParser);
      newParser->nextLine = 1;
      newParser->nextColumn = 1;
      ReadChar(newParser);
      newParser->line = 1;
      newParser->column = 1;

      return newParser;
    }

    // Cleanup on error
    fclose( handle );
    handle = NULL;
  }

  return NULL;
}

void CloseSource( Parser** parserPtr ) {
  if( parserPtr ) {
    if( (*parserPtr) ) {
      if( (*parserPtr)->handle ) {
        fclose( (*parserPtr)->handle );
        (*parserPtr)->handle = NULL;
      }

      free( (*parserPtr) );
      (*parserPtr) = NULL;
    }
  }
}

char PeekChar( Parser* source ) {
  if( source && source->handle ) {
    return source->ch;
  }
  return 0;
}

char ReadChar( Parser* source ) {
  int filech = EOF;

  if( !(source && source->handle) ) { return 0; }

  source->line = source->nextLine;
  source->column = source->nextColumn;
  source->ch = source->nextch;

  filech = fgetc(source->handle);
  if( filech != EOF ) {
    source->nextch = (char)filech;

    if( source->ch == '\r' ) {
      if( source->nextch == '\n' ) {
        source->nextch = (char)fgetc(source->handle);
      }
      source->ch = '\n';
    }

    if( source->ch == '\n' ) {
      source->nextLine++;
      source->nextColumn = 0;
    }

    source->nextColumn++;

    return source->ch;
  }

  return 0;
}

/*
 *  Main implementation
 */

const char newSourceText1[] = {
  "\n"
  "/* Multi-line\n"
  "  /* nested */ comment */\n"
  "\n"
  "// Single-line comment\n"
  "\n"
  "program "
};

const char newSourceText2[] = {
  "\n"
  "\n"
  "run\n"
  "  echoln( 'Hello, world!' )\n"
  "end\n"
};

void PrintBanner() {
  printf( "\n"
          "Retineo Gen1 Compiler 0.1.0 Alpha Win32\n"
          "Copyright 1993 - 2026 Orlando LLanes\n"
  );
}

void PrintUsage() {
  printf( "\nUsage: retg1 -new source[.ret]\n" );
  printf( "\nUsage: retg1 source[.ret] [-o output.ext] [-win32[:(console | gui)]]\n" );
}

unsigned GenerateNewSource( const char* sourceName ) {
  char programName[_MAX_FNAME] = { };
  FILE* programFile = NULL;

  if( !(sourceName && *sourceName) ) { return 1; }

  _splitpath( sourceName, NULL, NULL, programName, NULL );
  if( *programName == 0 ) { return 2; }

  programFile = fopen(sourceName, "w+");
  if( programFile == NULL ) { return 3; }

  fprintf( programFile,
    "%s%s%s", newSourceText1, programName, newSourceText2 );

  fclose( programFile );
  programFile = NULL;

  return 0;
}

unsigned BuildProgram( Options* buildOptions ) {
  return 2;
}

Options ParseOptions( int argc, char** argv ) {
  char sourceDrive[_MAX_DRIVE] = { };
  char sourceDir[_MAX_DIR] = { };
  char sourceName[_MAX_FNAME] = { };
  char sourceExt[_MAX_EXT] = { };
  char outputDrive[_MAX_DRIVE] = { };
  char outputDir[_MAX_DIR] = { };
  char outputName[_MAX_FNAME] = { };
  char outputExt[_MAX_EXT] = { };
  Options tmpOptions = { };
  const char* arg = NULL;
  int argIndex = 2;
  char argChar = 0;

  if( (argc > 2) && (strcmp(argv[1], "-new") == 0) ) {
    // -new source[.ret]
    if( argc != 3 ) { return tmpOptions; }

    strncpy( tmpOptions.sourceName, argv[2], _MAX_PATH );

    tmpOptions.mode = modeGenerateNew;
    return tmpOptions;
  }

  if( argc < 2 ) { return tmpOptions; }
  _splitpath( argv[1], sourceDrive, sourceDir, sourceName, sourceExt );
  _splitpath( argv[1], outputDrive, outputDir, outputName, NULL );
  strcpy( outputExt, ".exe" );

  while( argIndex < argc ) {
    // -o output.ext
    if( strcmp(argv[argIndex], "-o") == 0 ) {
      if( (argIndex + 1) >= argc ) { return tmpOptions; }

      _splitpath( argv[argIndex + 1], outputDrive, outputDir, outputName, outputExt );

      argIndex += 2;
      continue;
    }

    // -win32
    if( strcmp(argv[argIndex], "-win32") == 0 ) {
      if( tmpOptions.targetOS ) { return tmpOptions; }

      tmpOptions.targetOS = osWin32;
      tmpOptions.subsystem = win32Console;

      argIndex++;
      continue;
    }

    // -win32:console
    if( strcmp(argv[argIndex], "-win32:console") == 0 ) {
      if( tmpOptions.targetOS ) { return tmpOptions; }

      tmpOptions.targetOS = osWin32;
      tmpOptions.subsystem = win32Console;

      argIndex++;
      continue;
    }

    // -win32:gui
    if( strcmp(argv[argIndex], "-win32:gui") == 0 ) {
     if( tmpOptions.targetOS ) { return tmpOptions; }

     tmpOptions.targetOS = osWin32;
      tmpOptions.subsystem = win32GUI;

      argIndex++;
      continue;
    }
    
    return tmpOptions;
  }

  _makepath( tmpOptions.sourceName,
    sourceDrive, sourceDir, sourceName, sourceExt );

  _makepath( tmpOptions.outputName,
    outputDrive, outputDir, outputName, outputExt );

  if( tmpOptions.targetOS == 0 ) { tmpOptions.targetOS = osWin32; }
  if( tmpOptions.subsystem == 0 ) { tmpOptions.subsystem = win32Console; }

  tmpOptions.mode = modeBuildSource;
  return tmpOptions;
}

int main( int argc, char** argv ) {
  int result = 0;

  PrintBanner();

  if( argc <= 1 ) {
    PrintUsage();
    return 1;
  }

  options = ParseOptions(argc, argv);

  switch( options.mode ) {
  case modeGenerateNew:
    if( GenerateNewSource(options.sourceName) ) { result = 2; }
    printf( "\nWrote file '%s'\n", options.sourceName );
    break;

  case modeBuildSource:
    if( BuildProgram(&options) ) { result = 3; }
    break;

  default:
    PrintUsage();
    return 4;
  }

  return 0;
}

#include "avl_tree.c"
