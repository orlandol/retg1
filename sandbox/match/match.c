
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Source {
  FILE* handle;
  unsigned line;
  unsigned column;
  unsigned nextLine;
  unsigned nextColumn;
  int ch;
} Source;

char ReadChar( Source* source );

Source* OpenSourceFile( const char* fileName ) {
  Source* newSource = NULL;
  FILE* handle = NULL;

  if( (fileName == NULL) || (*fileName == '\0') ) { return NULL; }

  handle = fopen(fileName, "rb");
  if( handle == NULL ) { return NULL; }

  newSource = calloc(1, sizeof(Source));
  if( newSource ) {
    newSource->handle = handle;

    newSource->nextLine = 1;
    newSource->nextColumn = 1;

    ReadChar( newSource );

    newSource->line = 1;
    newSource->column = 1;

    return newSource;
  }

  fclose( handle );
  handle = NULL;

  return NULL;
}

void CloseSourceFile( Source** sourcePtr ) {
  if( sourcePtr ) {
    if( *sourcePtr ) {
      if( (*sourcePtr)->handle ) {
        fclose( (*sourcePtr)->handle );
        (*sourcePtr)->handle = NULL;
      }

      free( *sourcePtr );
      *sourcePtr = NULL;
    }
  }
}

char ReadChar( Source* source ) {
  int filech = EOF;

  if( !(source && source->handle) ) { return 0; }

  source->ch = 0;
  filech = fgetc(source->handle);
  if( filech == EOF ) { return 0; }

  source->line = source->nextLine;
  source->column = source->nextColumn;
  source->ch = filech;

  if( source->ch == '\r' ) {
    source->ch = fgetc(source->handle);
    if( source->ch != '\n' ) {
      fseek( source->handle, -1, SEEK_CUR );
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

unsigned NextToken( Source* source ) {
  if( !(source && source->handle) ) { return 1; }

  while( isspace(source->ch) ) {
    ReadChar( source );
  }

  return 0;
}

unsigned Match( Source* source, const char* keyword ) {
  Source prevState = {};
  long prevPos = 0;
  const char* keywordCh = keyword;

  if( !(source && source->handle) ) { return 1; }

  if( !(keyword && (*keyword)) ) { return 2; }

  prevState = *source;
  prevPos = ftell(source->handle);

  while( *keyword ) {
    // If characters don't match then restore state and return error 3
    if( source->ch != *keyword ) {
      *source = prevState;
      fseek( source->handle, prevPos, SEEK_SET );

      return 3;
    }

    ReadChar( source );
    keyword++;
  }

  // If any tokens remain then restore state and return error 4
  if( (isspace(source->ch) == 0) && (source->ch != '/') ) {
    *source = prevState;
    fseek( source->handle, prevPos, SEEK_SET );

    return 4;
  }

  return 0;
}

int main( int argc, char** argv ) {
  Source* source = NULL;

  source = OpenSourceFile("test.txt");
  if( source == NULL ) {
    printf( "Error opening file 'test.txt'\n" );
    exit(1);
  }

  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );

  NextToken( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'prototype': %i\n", ftell(source->handle) );
  printf( "Return code: %u\n", Match(source, "prototype") );

  NextToken( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'program': %i\n", ftell(source->handle) );
  printf( "Return code: %u\n", Match(source, "program") );

  NextToken( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'programmer': %i\n", ftell(source->handle) );
  printf( "Return code: %u\n", Match(source, "programmer") );

  NextToken( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'var': %i\n", ftell(source->handle) );
  printf( "Return code: %u\n", Match(source, "var") );

  NextToken( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'run': %i\n", ftell(source->handle) );
  printf( "Return code: %u\n", Match(source, "run") );

  NextToken( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'end': %i\n", ftell(source->handle) );
  printf( "Return code: %u\n", Match(source, "end") );

  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position after 'end': %i\n", ftell(source->handle) );

  CloseSourceFile( &source );

  return 0;
}
