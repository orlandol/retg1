
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Source {
  FILE* handle;
  unsigned line;
  unsigned column;
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

  source->ch = filech;

  if( source->ch == '\r' ) {
    source->ch = fgetc(source->handle);
    if( source->ch != '\n' ) {
      fseek( source->handle, -1, SEEK_CUR );
    }

    source->ch = '\n';
  }

  if( source->ch == '\n' ) {
    source->line++;
    source->column = 0;
  }

  source->column++;

  return source->ch;
}

unsigned SkipNonTokens( Source* source ) {
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
    if( source->ch != *keyword ) {
      *source = prevState;
      fseek( source->handle, prevPos, SEEK_SET );

      return 3;
    }

    ReadChar( source );
    keyword++;
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

  SkipNonTokens( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'prototype': %i\n", ftell(source->handle) );
  Match( source, "prototype" );

  SkipNonTokens( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'program': %i\n", ftell(source->handle) );
  Match( source, "program" );

  SkipNonTokens( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'var': %i\n", ftell(source->handle) );
  Match( source, "var" );

  SkipNonTokens( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'run': %i\n", ftell(source->handle) );
  Match( source, "run" );

  SkipNonTokens( source );
  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position before 'end': %i\n", ftell(source->handle) );
  Match( source, "end" );

  printf( "ch == %c; line == %u; column == %u\n", source->ch, source->line, source->column );
  printf( "File position after 'end': %i\n", ftell(source->handle) );

  CloseSourceFile( &source );

  return 0;
}
