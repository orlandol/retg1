
#include <stdio.h>

typedef struct Source {
  FILE* handle;
} Source;

Source* OpenSourceFile( const char* fileName ) {
  return NULL;
}

void CloseSourceFile( Source** sourcePtr ) {
}

char ReadChar( Source* source ) {
  return 0;
}

unsigned Match( Source* source, const char* keyword ) {
  return 3;
}

int main( int argc, char** argv ) {
  Match( "program" );
  Match( "prototype" );
  Match( "var" );
  Match( "run" );

  return 0;
}
