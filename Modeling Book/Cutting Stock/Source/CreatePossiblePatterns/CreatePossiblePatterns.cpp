// CreatePossiblePatterns.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <string.h>
#include <iostream>	
#include <fstream>
#include "aimmsapi.h"

using namespace std;

int RetrieveScalar(_TCHAR *AIMMS_scalar);
void ReadInput();
DLL_EXPORT_PROTO(void) CreateAllPatterns();
void CreatePatterns( int final, int size );

int PatternNumber, RawSize, SmallestFinal, NumberOfFinals;
vector<int> CurrentPattern;
vector<AimmsValue> FinalSize;
int FinalsInPatternHandle, PatternsHandle, FinalsHandle;

BOOL APIENTRY DllMain()
{
    return TRUE;
}

void ReadInput()
{
   //Retrieve the scalars RawSize and SmallestFinal from AIMMS	
   RawSize = RetrieveScalar(_T("RawSize"));
   SmallestFinal = RetrieveScalar(_T("SmallestFinal"));
   
   //Retrieve the number of finals
   int FinalSizeHandle;
   AimmsIdentifierHandleCreate( _T("FinalSize"), NULL, NULL, 0, &FinalSizeHandle );
   AimmsValueCard( FinalSizeHandle, &NumberOfFinals );
   
   //Resize the FinalSize vector to length NumberOfFinals
   FinalSize.resize( NumberOfFinals );
   
   //Fill the FinalSize vector
   int *tuples = new int[NumberOfFinals];
   int result = AimmsValueResetHandle( FinalSizeHandle );
   result = AimmsValueNextMulti( FinalSizeHandle, &NumberOfFinals, tuples, &FinalSize[0] );  
   delete [] tuples;
}

int RetrieveScalar(_TCHAR *AIMMS_Scalar)
{
   int Handle;
   AimmsValue aimmsVal;
   AimmsIdentifierHandleCreate( AIMMS_Scalar, NULL, NULL, 0, &Handle );
   AimmsValueRetrieve( Handle, NULL, &aimmsVal );
   AimmsIdentifierHandleDelete( Handle );
   return (int)aimmsVal.Double ;
}
DLL_EXPORT(void) CreateAllPatterns()
{
    PatternNumber = 0;
    
    //Create the handles to the AIMMS identifiers that need to be filled
    AimmsIdentifierHandleCreate( _T("FinalsInPattern"), NULL, NULL, 0, &FinalsInPatternHandle );
    AimmsIdentifierHandleCreate( _T("CuttingPatterns"), NULL, NULL, 0, &PatternsHandle );
    AimmsIdentifierHandleCreate( _T("Finals"), NULL, NULL, 0, &FinalsHandle );

    ReadInput();

    CurrentPattern.resize( NumberOfFinals );

   //Start the recursive pattern generation
    CreatePatterns( 0, RawSize );

    AimmsIdentifierHandleDelete( FinalsInPatternHandle );
    AimmsIdentifierHandleDelete( PatternsHandle );
    AimmsIdentifierHandleDelete( FinalsHandle );
}

void CreatePatterns( int final, int size )
{
   //The pattern is send to AIMMS when it is filled for all finals
   if ( final == NumberOfFinals ) {
      PatternNumber++;
    
      //Add the pattern to the set CuttingPatterns in AIMMS
      _TCHAR elementName[8];
      _stprintf( elementName, _T("%d"), PatternNumber );
      int tuple[2];
      AimmsSetAddElement( PatternsHandle, elementName, &tuple[1] );
      
      //Fill the FinalsInPattern parameter in AIMMS
      for ( int i=0; i<final; i++ ) {
         AimmsValue jval;
         jval.Double = (double)CurrentPattern[i] ;
         
         AimmsSetOrdinalToElement(FinalsHandle, i+1, &tuple[0] );
         
         AimmsValueAssign( FinalsInPatternHandle, tuple, &jval );   
      }
   }   
   else {   
      //Fill the pattern for the current final and continue recursively
      int ThisFinalSize = (int)FinalSize[final].Double ;
          
      for ( int j = 0; j <= size / ThisFinalSize; j++ ) {
         
         CurrentPattern[final] = j;
         
         CreatePatterns(final + 1, size - j*ThisFinalSize );
      }
   }
}
