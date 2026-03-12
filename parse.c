
#include "retg1.h"

unsigned NextToken( Source* source ) {
  return 2;
}

unsigned Match( Source* source, const char* withString ) {
  return 3;
}

unsigned Submatch( Source* source, const char* withString ) {
  return 3;
}

unsigned ParseProgram( Source* source, Binary* binary ) {
  return 3;
}
