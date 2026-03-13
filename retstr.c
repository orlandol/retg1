#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "retg1.h"

typedef struct retstrImpl {
  uint32_t length;
  uint32_t totalSize;
} retstrImpl;

typedef struct blankRetstrImpl {
  uint32_t length;
  uint32_t totalSize;
  char     data[8];
} blankRetstrImpl;

static const blankRetstrImpl blankRetStringData = {
  0,
  8,
  ""
};

static const retstr* blankRetString =
  ((retstr*)&blankRetStringData) + sizeof(retstrImpl);

retstr* retstrAllocate( uint32_t size ) {
  retstrImpl* newStrImpl = NULL;
  retstr* newStr = NULL;
  uint32_t paddedSize = 0;
  uint32_t totalSize = 0;

  paddedSize = RETSTR_PAD(size);
  totalSize = sizeof(retstrImpl) + paddedSize;
  if( totalSize < size ) { return NULL; }

  newStrImpl = calloc(1, totalSize);
  if( newStrImpl ) {
    newStrImpl->totalSize = totalSize;
    newStr = ((retstr*)newStrImpl) + sizeof(retstrImpl);
  }

  return newStr;
}

retstr* retstrCopy( const retstr* rstring ) {
  retstrImpl* newStrImpl = NULL;
  retstr* newStr = NULL;
  retstrImpl* rstringImpl = (retstrImpl*)(rstring - sizeof(retstrImpl));
  uint32_t paddedSize = 0;
  uint32_t totalSize = 0;

  if( !(rstring && (*rstring)) ) { return NULL; }

  totalSize = retstrTotalSize(rstring);
  if( totalSize == 0 ) { return retstrAllocate(0); }

  newStrImpl = calloc(1, totalSize);
  if( newStrImpl ) {
    newStrImpl->length = retstrLength(rstring);
    newStrImpl->totalSize = totalSize;

    newStr = ((retstr*)newStrImpl) + sizeof(retstrImpl);

    memcpy( newStr, rstringImpl, totalSize );
  }

  return newStr;
}

retstr* retstrCopyStr( const char* cstring ) {
  retstrImpl* newStrImpl = NULL;
  retstr* newStr = NULL;
  uint32_t paddedSize = 0;
  uint32_t totalSize = 0;
  size_t clength = 0;

  if( !(cstring && (*cstring)) ) { return NULL; }

  clength = strlen(cstring);

  paddedSize = RETSTR_PAD(clength);
  totalSize = sizeof(retstrImpl) + paddedSize;
  if( totalSize < clength ) { return NULL; }

  newStrImpl = calloc(1, totalSize);
  if( newStrImpl ) {
    newStrImpl->length = clength;
    newStrImpl->totalSize = totalSize;

    newStr = ((retstr*)newStrImpl) + sizeof(retstrImpl);

    strncpy( newStr, cstring, clength );
  }

  return newStr;
}

void retstrRelease( retstr** retstrPtr ) {
  if( retstrPtr ) {
    if( (*retstrPtr) ) {
      free( ((*retstrPtr) - sizeof(retstrImpl)) );
      (*retstrPtr) = NULL;
    }
  }
}

uint32_t retstrLength( const retstr* retString ) {
  if( retString ) {
    return ((retstrImpl*)(retString - sizeof(retstrImpl)))->length;
  }

  return 0;
}

uint32_t retstrTotalSize( const retstr* retString ) {
  if( retString ) {
    return ((retstrImpl*)(retString - sizeof(retstrImpl)))->totalSize;
  }

  return 0;
}
