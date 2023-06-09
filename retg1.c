
/*
 *  Retineo G1 Copyright 1993 to 2023 Orlando Llanes
 *
 *  Released under the MIT license.
 */

#include <stdlib.h>
#include <stdio.h>

/*
 *  Base declarations
 */

/*
 *  Retineo string C declarations
 */

typedef struct retstring {
  size_t size;
  char data[];
} retstring;

/*
 *  Parser declarations
 */

typedef struct Parser {
  FILE* handle;

  unsigned line;
  unsigned column;
  unsigned nextline;
  unsigned nextcolumn;

  int ch;
  int nextch;
} Parser;

Parser* OpenSourceFile( const char* fileName );
void CloseSourceFile( Parser** parserPtr );

int ReadChar( Parser* source );

/*
 *  Main program
 */

Parser* parser = NULL;

void Cleanup() {
  CloseSourceFile( &parser );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  parser = OpenSourceFile("test.ret");

  printf( "[L%u,C%u]: ch(%i) nextch(%i)\n", parser->line, parser->column, parser->ch, parser->nextch );

  ReadChar( parser );

  printf( "[L%u,C%u]: ch(%i) nextch(%i)\n", parser->line, parser->column, parser->ch, parser->nextch );

  ReadChar( parser );

  printf( "[L%u,C%u]: ch(%i) nextch(%i)\n", parser->line, parser->column, parser->ch, parser->nextch );

  return 0;
}

/*
 *  Retineo string C implementation
 */

/*
 *  Parser implementation
 */

Parser* OpenSourceFile( const char* fileName ) {
  Parser* newParser = NULL;

  if( fileName && (*fileName) ) {
    newParser = calloc(1, sizeof(Parser));

    if( newParser ) {
      newParser->handle = fopen(fileName, "rb");

      if( newParser->handle ) {
        newParser->ch = EOF;
        newParser->nextch = EOF;

        ReadChar( newParser );

        newParser->line = 1;
        newParser->column = 1;
        newParser->nextline = 1;
        newParser->nextcolumn = 1;

        ReadChar( newParser );

        return newParser;
      }
    }
  }

  CloseSourceFile( &newParser );
  return NULL;
}

void CloseSourceFile( Parser** parserPtr ) {
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

int ReadChar( Parser* source ) {
  if( source && source->handle ) {
    source->line = source->nextline;
    source->column = source->nextcolumn;

    source->ch = source->nextch;
    source->nextch = fgetc(source->handle);

    if( source->ch == 10 ) {
      if( source->nextch == 13 ) {
        source->nextch = fgetc(source->handle);
      }

      source->ch = 13;
    }

    if( source->ch == 13 ) {
      source->nextline++;
      source->nextcolumn = 0;
    }

    source->nextcolumn++;

    return source->ch;
  }

  return EOF;
}
