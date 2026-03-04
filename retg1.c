
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "retg1.h"

typedef struct Options {
  retstr* sourceName;
  retstr* asmName;
  retstr* objName;
  retstr* binaryName;
} Options;

unsigned ParseOptions( int argc, char** argv, Options* options ) {
  char sourceDrive[_MAX_DRIVE] = {};
  char sourceDir[_MAX_DIR] = {};
  char sourceName[_MAX_FNAME] = {};
  char sourceExt[_MAX_EXT] = {};

  char binaryDrive[_MAX_DRIVE] = {};
  char binaryDir[_MAX_DIR] = {};
  char binaryName[_MAX_FNAME] = {};
  char binaryExt[_MAX_EXT] = ".exe";

  char asmExt[_MAX_EXT] = ".asm";
  char objExt[_MAX_EXT] = ".obj";

  char fileName[_MAX_PATH] = {};

  // Validate parameters
  if( argc <= 1 ) { return 1; }
  if( argv == NULL ) { return 2; }
  if( options == NULL ) { return 3; }

  // Copy/Infer source file name
  if( _splitpath_s(argv[1], sourceDrive, _MAX_DRIVE, sourceDir, _MAX_DIR,
      sourceName, _MAX_FNAME, sourceExt, _MAX_EXT) != 0 ) {
    return 4;
  }
  if( strcmp(sourceExt, ".ret") != 0 ) {
    strcpy(sourceExt, ".ret");
  }
  if( _makepath_s(fileName, _MAX_PATH,
      sourceDrive, sourceDir, sourceName, sourceExt) != 0 ) {
    return 5;
  }
  options->sourceName = retstrCopyStr(fileName);

  // Copy/Infer binary file name
  strncpy( binaryDrive, sourceDrive, _MAX_DRIVE );
  strncpy( binaryDir, sourceDir, _MAX_DIR );
  strncpy( binaryName, sourceName, _MAX_FNAME );
  if( argc >= 3 ) {
    if( _splitpath_s(argv[2], binaryDrive, _MAX_DRIVE, binaryDir, _MAX_DIR,
        binaryName, _MAX_FNAME, binaryExt, _MAX_EXT) != 0 ) {
      return 6;
    }
  }
  if( _makepath_s(fileName, _MAX_PATH,
      binaryDrive, binaryDir, binaryName, binaryExt) != 0 ) {
    return 7;
  }
  options->binaryName = retstrCopyStr(fileName);

  // Create assembler file name
  if( _makepath_s(fileName, _MAX_PATH,
      binaryDrive, binaryDir, binaryName, asmExt) != 0 ) {
    return 8;
  }
  options->binaryName = retstrCopyStr(fileName);

  // Create object file name
  if( _makepath_s(fileName, _MAX_PATH,
      binaryDrive, binaryDir, binaryName, objExt) != 0 ) {
    return 9;
  }
  options->binaryName = retstrCopyStr(fileName);

  return 0;
}

Options options = {};

unsigned warningCount = 0;
unsigned errorCount = 0;

void PrintBanner() {
  printf( "\nRetineo G1 Win32 x86 Compiler - "
          "Copyright 1993-2026 Orlando LLanes\n\n" );
}

void PrintUsage() {
  printf( "  usage: retg1 source[.ret] [binary.exe]\n\n" );
}

void PrintSummary( const char* generatedName,
                   unsigned ParseWarnings, unsigned parseErrors ) {
  printf( "  Completed building '%s': %u warning(s); %u error(s)\n\n",
    generatedName, warningCount, errorCount );
}

void Cleanup() {
  retstrRelease( &options.sourceName );
  retstrRelease( &options.asmName );
  retstrRelease( &options.objName );
  retstrRelease( &options.binaryName );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  PrintBanner();

  if( argc <= 1 ) {
    PrintUsage();
    return 1;
  }

  ParseOptions( argc, argv, &options );

  PrintSummary( options.binaryName, warningCount, errorCount );

  Cleanup();

  return 0;
}
