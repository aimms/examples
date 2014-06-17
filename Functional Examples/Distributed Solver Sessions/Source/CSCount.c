#include <windows.h>

#define DLL_EXPORT(type) __declspec(dllexport) type WINAPI

void CountPatternsRec( int *count, int depth, int dim, int size, int *sizes )
{
  int j;

  /* After we have assigned the final pattern type, we have created a
     new pattern which must be counted. In all other cases, we must
     call the function recursively for the next pattern type, with a
     remaining size equal to the current remaining size minus the size
     of the current pattern type times the selected number of cuts. */
    
  if ( depth == dim ) (*count)++;
  else
    for ( j = 0; j <= size / sizes[depth]; j++ )
      CountPatternsRec( count, depth + 1, dim, size - j*sizes[depth], sizes );
}

DLL_EXPORT(void) CountPatterns( int dim, int size, int *sizes, int *count )
{
  *count = 0;
  
  CountPatternsRec( count, 0, dim, size, sizes );
}
