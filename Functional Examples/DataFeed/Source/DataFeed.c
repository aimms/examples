#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include "aimmsapi.h"
#include "aimmsadl.h"

/* This source file contains sample code to emulate a data feed running
   independently from an AIMMS model, and passes data to that model.
   Typical applications that can make use of the functionality used here
   are e.g. financial models which are fed stock data whenever this is
   available.
*/

#define DLL_EXPORT(type) __declspec(dllexport) type WINAPI

#define NR_DEPOTS    3
#define NR_CUSTOMERS 10

double Supply[NR_DEPOTS];
double Demand[NR_CUSTOMERS];

double UnitTransportCost[NR_DEPOTS][NR_CUSTOMERS] =
{ { 1.0, 0.7, 0.3, 0.8, 1.2, 0.3, 1.1, 0.9, 1.0, 0.5 },
  { 0.7, 0.9, 1.3, 0.9, 0.9, 1.3, 1.2, 0.5, 0.7, 1.5 },
  { 1.2, 0.5, 0.9, 1.8, 1.1, 0.7, 0.5, 1.4, 0.5, 1.4 } };

double Transport[NR_DEPOTS][NR_CUSTOMERS];

static int stopped = 1, autorun;

#define uniform(m,n) ((m) + rand() % ((n) - (m) + 1))

static void RandomizeSupplyDemand( void )
{
  int i;
  double sumsupply, sumdemand;
  
  sumsupply = 0;
  sumdemand = 0;
      
  for ( i = 0; i < NR_DEPOTS; i++ ) {
    Supply[i]  = uniform(5,15);
    sumsupply += Supply[i];
  }

  for ( i = 0; i < NR_CUSTOMERS; i++ ) {
    Demand[i]  = uniform(5,10);
    sumdemand += Demand[i];
  }

  for ( i = 0; i < NR_CUSTOMERS; i++ ) {
    Demand[i] *= sumsupply / sumdemand;
  }
}

static void DataFeed( void *ptr )
{
  int retval, cards[2];
  
  cards[0] = NR_DEPOTS;
  cards[1] = NR_CUSTOMERS;

  AimmsThreadAttach();

  while ( ! stopped ) {
    Sleep( 500 + (rand() % 1000) );

    if ( ADLGetControl( 1000) == AIMMSAPI_SUCCESS ) {

      RandomizeSupplyDemand( );

      ADLPutDoubleArray( "Supply", Supply, 1, cards );
      ADLPutDoubleArray( "Demand", Demand, 1, cards + 1 );
      
      if ( autorun ) ADLRun( "MainExecution", &retval );

      ADLReleaseControl( );
    } else {
      MessageBox( NULL, "Data feed could not get control over AIMMS within 1 second",
                  "Data feed error", MB_OK | MB_ICONEXCLAMATION | MB_SETFOREGROUND );
    }
  }

  AimmsThreadDetach();

  _endthread( );
}

DLL_EXPORT(void) StartDataFeed( int runauto )
{
  int cards[2];

  autorun = runauto;

  ADLAttachToProject( );
  
  if ( ! stopped ) return;
  
  cards[0] = NR_DEPOTS;
  cards[1] = NR_CUSTOMERS;

  ADLGenerateElements( "Depots", "d-", NR_DEPOTS );
  ADLGenerateElements( "Customers", "c-", NR_CUSTOMERS );
  ADLPutDoubleArray( "UnitTransportCost", UnitTransportCost, 2, cards );

  stopped  = 0;

  /* The next call will start a separate thread of execution which will
     run the function DataFeed defined above */
  
  _beginthread( DataFeed, 0, NULL );
}


DLL_EXPORT(void) StopDataFeed( void )
{
  if ( ! stopped ) {
    stopped = 1;
  }
}
                                      
DLL_EXPORT(void) SetControl( int *blocked )
{
  /* When this function is called from within the AIMMS GUI, the control
     is obtained by the AIMMS thread. As a result, all calls to ADLGetControl
     from within the DataFeed thread will fail until the control is released
     again by the AIMMS thread. */
  
  if ( *blocked ) {
    if ( ! (ADLGetControl( 0 ) == AIMMSAPI_SUCCESS) ) {
      *blocked = 0;
    }
  } else {
    ADLReleaseControl( );
  }
}


