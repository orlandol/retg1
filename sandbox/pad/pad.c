
#include <stdio.h>
#include <stdint.h>

#define PADBITS 8

#define PADMASK ((PADBITS) - 1)

#define PAD(size) ((size + PADBITS) & (~(PADMASK)))

int main( int argc, char** argv ) {
  unsigned size;

  for( size = 0; size <= 8; size++ ) {
    if( (size & PADMASK) == 0 ) {
      printf( "size: %u; padded: %u\n", size, PAD(size) );
    }
  }

  return 0;
}
