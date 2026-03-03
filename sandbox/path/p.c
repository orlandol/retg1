
#include <stdlib.h>
#include <stdio.h>

char drive[_MAX_DRIVE] = {};
char dir[_MAX_DIR] = {};
char name[_MAX_FNAME] = {};
char ext[_MAX_EXT] = {};

char path[_MAX_PATH] = {};

int main( int argc, char** argv ) {
  _splitpath_s( argv[0], drive, _MAX_DRIVE, dir, _MAX_DIR, name, _MAX_FNAME, ext, _MAX_EXT );

  _makepath_s( path, _MAX_PATH, drive, dir, name, ext );

  printf( "argv[0] = '%s'\n", argv[0] );
  printf( "drive = '%s'; dir = '%s'; name = '%s'; ext = '%s'\n", drive, dir, name, ext );
  printf( "path = '%s'\n", path );

  return 0;
}
