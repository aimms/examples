/* This is the implementation of the

                AIMMS Dense Library (ADL)
  
   It offers a collection of functions, to be used in the context of an
   external program which wishes to use AIMMS for optimization, for dense
   data transfer with AIMMS and the execution of simple procedures (without
   arguments) contained in an AIMMS project.
    
   This library is intended for everyone who wishes, from within an external
   program, to communicate data with AIMMS contained in multidimensional
   arrays. In that case, you can use this library to prevent mapping the data
   contained in such arrays onto the sparse data structures used inside AIMMS and
   accessible through the AIMMS API. The library also includes various sanity and
   type checks. 
      
   For those who need to communicate data in a sparse manner, the implementation
   of the functions of the ADL may provide a good example for learning the steps
   required to use the AIMMS API.                                                 */


#include <windows.h>
#include "aimmsapi.h"
#include "aimmsadl.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

static int  ADLNrErrors     = 0;
static int  ADLErrorBufSize = 0;
static char **ADLErrors     = NULL;

static int  ADLAPIErrorCode;
static char ADLAPIErrorString[256];
static char ADLLastErrorString[256];

int ADLNrOfErrors(void)
{
    return ADLNrErrors;
}

char *ADLErrorMessage( int n )
{
    if ( n < ADLNrErrors && ADLErrors[n] )
        return ADLErrors[n];
    else  
        return "";
}

static void ADLAddMessage( char *message )
{
    char *mesbuf;
    
    ADLNrErrors++;
    
    if ( ADLNrErrors > ADLErrorBufSize ) {
        char **newbuf;
        
        ADLErrorBufSize += 1024;
        newbuf = (char **) realloc( ADLErrors, ADLErrorBufSize*sizeof( char ** ) );
        
        if ( newbuf )
            ADLErrors = newbuf;
        else
            ADLErrorBufSize -= 1024;
    }
    
    mesbuf = (char *)malloc( (strlen(message) + 1)*sizeof( char * ) );
    if ( mesbuf ) {
        strcpy( mesbuf, message );
        ADLErrors[ ADLNrErrors - 1 ] = mesbuf;
    } else
        ADLErrors[ ADLNrErrors - 1 ]; 
}

static void ADLFreeMessages( void )
{
    int i;
    
    for ( i = 0; i < ADLNrErrors && i < ADLErrorBufSize ; i++ )
        if ( ADLErrors[i] ) {
            free( ADLErrors[i] );
            ADLErrors[i] = NULL;
        }
}

/* Basic macro used in all functions to verify whether an AIMMS project has
   been loaded prior to calling a function.                                 */

#define CHECK_FOR_PROJECT                                                 \
    if ( ! ProjectHandle ) {                                              \
        sprintf( ADLLastErrorString, "No project has been opened yet" );  \
        ADLAddMessage( ADLLastErrorString );                              \
        return AIMMSAPI_FAILURE;                                          \
    } 

/* Macro used in most functions to check result of AIMMS API function and
   delete a previously created identifier handle before leaving the function. */

#define CHECK_FOR_SUCCESS(message)                                  \
    if ( res != AIMMSAPI_SUCCESS ) {                                \
        AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );   \
        sprintf( ADLLastErrorString, "%s: %s",                      \
                 message, ADLAPIErrorString );                      \
        ADLAddMessage( ADLLastErrorString );                        \
        AimmsIdentifierHandleDelete( handle );                      \
        return AIMMSAPI_FAILURE;                                    \
    }

/* Same as above, but also delete additional domain handles created as
   part of the process.                                                */

#define CHECK_FOR_SUCCESS_DOM(message)                              \
    if ( res != AIMMSAPI_SUCCESS ) {                                \
        AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );   \
        sprintf( ADLLastErrorString, "%s: %s",                      \
                 message, ADLAPIErrorString );                      \
        ADLAddMessage( ADLLastErrorString );                        \
        AimmsIdentifierHandleDelete( handle );                      \
        for ( i = 0; i < dim; i++ )                                 \
            AimmsIdentifierHandleDelete( domain[i] );               \
        return AIMMSAPI_FAILURE;                                    \
    }

static int ProjectHandle = 0;

int ADLAttachToProject( void )
{
    HWND AimmsWin = NULL;
    
    AimmsProjectWindow( &AimmsWin );
    
    if ( ! AimmsWin ) {
        ADLAddMessage( "Cannot attach to an AIMMS session that is not yet running" );
        return AIMMSAPI_FAILURE;
    }
    
    ProjectHandle = -1;
    return AIMMSAPI_SUCCESS;
}

int ADLProjectOpen( char *Commandline )
{
    int res;
    
    /* First free all error messages stored so far */
    
    ADLFreeMessages( );
    
    /* We only open a single project at a time */
    
    if ( ! ProjectHandle ) {
        res = AimmsProjectOpen( Commandline, &ProjectHandle );
        
        if ( res != AIMMSAPI_SUCCESS ) {
            AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
            sprintf( ADLLastErrorString, "Unable to open AIMMS with \"%s\": %s",
                Commandline, ADLAPIErrorString );
            ADLAddMessage( ADLLastErrorString );
        }
        
        return res;
    } else {
        sprintf( ADLLastErrorString, "Can call AIMMS only once during a single session" );
        ADLAddMessage( ADLLastErrorString );
        return AIMMSAPI_FAILURE;
    }
}

int ADLProjectClose( int interactive )
{
    int res;
    
    CHECK_FOR_PROJECT;
    
    if ( ProjectHandle == -1 ) {
        ADLAddMessage( "Unable to close AIMMS session not opened through the AIMMS API" );
        return AIMMSAPI_FAILURE;
    }
    
    res = AimmsProjectClose( ProjectHandle, interactive );
    ProjectHandle = 0;
    return res;
}

int ADLGetControl( int timeout )
{
    int res;
    
    CHECK_FOR_PROJECT;
    
    res = AimmsControlGet( timeout );
    if ( res != AIMMSAPI_SUCCESS )
        ADLAddMessage( "Unable to get exclusive control" );
    
    return res;
}

int ADLReleaseControl( void )
{
    CHECK_FOR_PROJECT;
    
    return AimmsControlRelease( );
}

int ADLRun( char *procedure, int *retval )
{
    int res, handle, nargs, argtype[16];
    
    CHECK_FOR_PROJECT;
    
    /* Obtain the procedure handle ... */
    
    res = AimmsProcedureHandleCreate( procedure, &handle, &nargs, argtype );
    CHECK_FOR_SUCCESS("Unable to obtain handle to procedure");
    
    /* ... which should have no arguments */
    
    if ( nargs != 0 ) {
        AimmsProcedureHandleDelete( handle );
        sprintf( ADLLastErrorString, "Procedure %s has %d argument(s) (expected 0)",
            procedure, nargs ); 
        ADLAddMessage( ADLLastErrorString );
        return AIMMSAPI_FAILURE;
    }
    
    res = AimmsProcedureRun( handle, NULL, NULL, retval );
    
    if ( res != AIMMSAPI_SUCCESS ) {
        AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
        sprintf( ADLLastErrorString, "Procedure run failed: %s",
            ADLAPIErrorString );
        ADLAddMessage( ADLLastErrorString );
    }
    
    AimmsProcedureHandleDelete( handle );
    return res;
}


/* This function creates a handle to an AIMMS identifier and
   generates an appropriate error message when unsuccessful */

static int ADLCreateHandle( char *id, int *handle )
{
    int res;
    
    CHECK_FOR_PROJECT;
    
    res = AimmsIdentifierHandleCreate( id, NULL, NULL, 0, handle );
    if ( res != AIMMSAPI_SUCCESS ) {
        AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
        sprintf( ADLLastErrorString, "Unable to obtain handle to identifier \"%s\": %s",
            id, ADLAPIErrorString );
        ADLAddMessage( ADLLastErrorString );
    }
    
    return res;
}

int ADLGetSetCard( char *setname, int *n )
{
    int handle, res, type;
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( setname, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeType( handle, &type );
    CHECK_FOR_SUCCESS("Unable to obtain identifier type");
    
    if ( type != AIMMSAPI_IDTYPE_SIMPLE_SET_ROOT && type != AIMMSAPI_IDTYPE_SIMPLE_SET_SUBSET ) {
        sprintf( ADLLastErrorString, "Identifier %s is not a simple set", setname );
        ADLAddMessage( ADLLastErrorString );
        AimmsIdentifierHandleDelete( handle );
        return AIMMSAPI_FAILURE;
    }
    
    res = AimmsValueCard( handle, n );
    
    if ( res != AIMMSAPI_SUCCESS ) {
        AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
        sprintf( ADLLastErrorString, "Unable to retrieve set cardinality: %s", ADLAPIErrorString );
        ADLAddMessage( ADLLastErrorString );
    }
    
    AimmsIdentifierHandleDelete( handle );
    
    return res;
}

int ADLGetElements( char *setname, char **elements, int recs, int size, int *card )
{
    int handle, res, i, type;
    AimmsString strvalue;
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( setname, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeType( handle, &type );
    CHECK_FOR_SUCCESS("Unable to obtain identifier type");
    
    if ( type != AIMMSAPI_IDTYPE_SIMPLE_SET_ROOT && type != AIMMSAPI_IDTYPE_SIMPLE_SET_SUBSET ) {
        sprintf( ADLLastErrorString, "Identifier %s is not a simple set", setname );
        ADLAddMessage( ADLLastErrorString );
        AimmsIdentifierHandleDelete( handle );
        return AIMMSAPI_FAILURE;
    }
    
    res = AimmsValueCard( handle, card );
    
    if ( *card > recs ) {
        sprintf( ADLLastErrorString, "Actual set size (%d) exceeds buffer size (%d)",
            *card, recs );
        ADLAddMessage( ADLLastErrorString );
        AimmsIdentifierHandleDelete( handle );
        return AIMMSAPI_FAILURE;
    }
    
    res = AIMMSAPI_SUCCESS;
    
    for ( i = 0; i < *card; i++ ) {
        strvalue.Length = size;
        strvalue.String = elements[i];
        
        if ( AimmsSetOrdinalToName( handle, i+1, &strvalue ) != AIMMSAPI_SUCCESS ) {
            sprintf( ADLLastErrorString, "Unable to retrieve element #%d of set %s",
                i+1, setname );
            ADLAddMessage( ADLLastErrorString );
            res = AIMMSAPI_FAILURE;
            break;
        }
    }
    
    AimmsIdentifierHandleDelete( handle );
    return res;
}

int ADLPutElements( char *setname, char **elements, int card )
{
    int handle, res, i, element, type;
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( setname, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeType( handle, &type );
    CHECK_FOR_SUCCESS("Unable to obtain identifier type");
    
    if ( type != AIMMSAPI_IDTYPE_SIMPLE_SET_ROOT && type != AIMMSAPI_IDTYPE_SIMPLE_SET_SUBSET ) {
        sprintf( ADLLastErrorString, "Identifier %s is not a simple set", setname );
        ADLAddMessage( ADLLastErrorString );
        AimmsIdentifierHandleDelete( handle );
        return AIMMSAPI_FAILURE;
    }
    
    res = AimmsIdentifierEmpty( handle );
    CHECK_FOR_SUCCESS("Unable to empty handle");
    
    res = AimmsIdentifierCleanup( handle );
    CHECK_FOR_SUCCESS("Unable to cleanup set");
    
    res = AIMMSAPI_SUCCESS;
    
    for ( i = 0; i < card; i++ ) {
        if ( AimmsSetAddElementRecursive( handle, elements[i], &element ) != AIMMSAPI_SUCCESS ) {
            AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
            sprintf( ADLLastErrorString, "Unable to add element '%s' to set %s: %s",
                elements[i], setname, ADLAPIErrorString );
            ADLAddMessage( ADLLastErrorString );
            
            if ( ADLAPIErrorCode == API_ERROR_ELEMENT_ALREADY_IN_SET ) continue;
            
            res = AIMMSAPI_FAILURE;
            break;
        }
    }
    
    AimmsIdentifierHandleDelete( handle );
    return res;
}

int ADLGenerateElements( char *setname, char *prefix, int card )
{
    int handle, res, i, element, type;
    char elname[256];
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( setname, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeType( handle, &type );
    CHECK_FOR_SUCCESS("Unable to obtain identifier type");
    
    if ( type != AIMMSAPI_IDTYPE_SIMPLE_SET_ROOT && type != AIMMSAPI_IDTYPE_SIMPLE_SET_SUBSET ) {
        sprintf( ADLLastErrorString, "Identifier %s is not a simple set", setname );
        ADLAddMessage( ADLLastErrorString );
        AimmsIdentifierHandleDelete( handle );
        return AIMMSAPI_FAILURE;
    }
    
    res = AimmsIdentifierEmpty( handle );
    CHECK_FOR_SUCCESS("Unable to empty handle");
    
    res = AimmsIdentifierCleanup( handle );
    CHECK_FOR_SUCCESS("Unable to cleanup set");
    
    res = AIMMSAPI_SUCCESS;
    
    for ( i = 0; i < card; i++ ) {
        sprintf( elname, "%s%d", prefix, i+1 ); 
        if ( AimmsSetAddElementRecursive( handle, elname, &element ) != AIMMSAPI_SUCCESS ) {
            AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
            sprintf( ADLLastErrorString, "Unable to add element '%s' to set %s: %s",
                elname, setname, ADLAPIErrorString );
            ADLAddMessage( ADLLastErrorString );
            res = AIMMSAPI_FAILURE;
            break;
        }
    }
    
    AimmsIdentifierHandleDelete( handle );
    return res;
}

int ADLGetArraySize( char *id, int *dim, int *cards )
{
    int handle, res, sliced, i, domain[AIMMSAPI_MAX_DIMENSION];
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( id, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeDimension( handle, dim, &sliced );
    CHECK_FOR_SUCCESS("Unable to obtain dimension");
    
    /* By default, Aimms transfers data defined over the declaration domain,
       so those are the sizes that need to be returned. */
    
    res = AimmsAttributeDeclarationDomain( handle, domain );
    CHECK_FOR_SUCCESS("Unable to obtain declaration domain");
    
    for ( i = 0; i < *dim; i++ ) {
        if ( AimmsValueCard( domain[i], cards + i ) != AIMMSAPI_SUCCESS )
            res = AIMMSAPI_FAILURE;
        AimmsIdentifierHandleDelete( domain[i] );
    }
    
    if ( res == AIMMSAPI_FAILURE ) {
        AimmsAPILastError( &ADLAPIErrorCode, ADLAPIErrorString );
        sprintf( ADLLastErrorString, "Unable to retrieve domain sizes: %s", ADLAPIErrorString );
        ADLAddMessage( ADLLastErrorString );
    }
    
    AimmsIdentifierHandleDelete( handle );
    return res;
}

int ADLGetArray( char *id, void *val, int recs, int size, int type, int dim, int *cards )
{
    int handle, res, adim, sliced, storage, i, j, recreq, pos, icorr, avail,
        domain[AIMMSAPI_MAX_DIMENSION], tuple[AIMMSAPI_MAX_DIMENSION],
        running[AIMMSAPI_MAX_DIMENSION], ords[AIMMSAPI_MAX_DIMENSION];
    AimmsValue value;
    int    *ival  = (int *)val,     idef;
    double *dval  = (double *)val,  ddef;
    char   **sval = (char **)val,  *sdef, *sbuf;
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( id, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeDimension( handle, &adim, &sliced );
    CHECK_FOR_SUCCESS("Unable to obtain dimension");
    
    res = AimmsAttributeStorage( handle, &storage );
    CHECK_FOR_SUCCESS("Unable to obtain identifier storage type")   
        
        if ( ( dim != adim ) ||
            ! ( ( ( type == ADL_TYPE_INT || type == ADL_TYPE_DOUBLE ) &&
            ( storage == AIMMSAPI_STORAGE_DOUBLE || storage == AIMMSAPI_STORAGE_INT ) ) ||
            ( ( type == ADL_TYPE_STRING ) && ( storage == AIMMSAPI_STORAGE_STRING )     )    ) ) {
            sprintf( ADLLastErrorString, "Incorrectly specified dimension or incompatible data types" );
            ADLAddMessage( ADLLastErrorString );
            AimmsIdentifierHandleDelete( handle );
            return AIMMSAPI_FAILURE;
        }
        
        res = AimmsAttributeDeclarationDomain( handle, domain );
        CHECK_FOR_SUCCESS("Unable to obtain declaration domain");
        
        recreq = 1;
        for ( i = 0; i < dim; i++ ) {
            res = AimmsValueCard( domain[i], &cards[i] );
            CHECK_FOR_SUCCESS_DOM("Unable to obtain cardinality of domain set");
            recreq *= cards[i];
        }
        
        if ( recreq > recs ) {
            sprintf( ADLLastErrorString, "Actual data size (%d) exceeds specified buffer size (%d)",
                recreq, recs );
            ADLAddMessage( ADLLastErrorString );
            AimmsIdentifierHandleDelete( handle );
            for ( i = 0; i < dim; i++ ) 
                AimmsIdentifierHandleDelete( domain[i] );
            return AIMMSAPI_FAILURE;
        }
        
        if ( type == ADL_TYPE_STRING ) {
            sdef = (char *) _alloca( size*sizeof(char) );
            sbuf = (char *) _alloca( size*sizeof(char) );
            value.Length = size;
            value.String = sbuf;
        }
        
        /* First deal with the scalar (0-dimensional) case */
        
        if ( dim == 0 ) {
            res = AimmsValueRetrieve( handle, NULL, &value );
            CHECK_FOR_SUCCESS_DOM("Unable to retrieve value");
            
            switch ( type ) {
            case ADL_TYPE_INT:
                *ival = ( storage == AIMMSAPI_STORAGE_INT ) ? (value.Int) : (int)(value.Double);
                break;
            case ADL_TYPE_DOUBLE:
                *dval = ( storage == AIMMSAPI_STORAGE_INT ) ? (double)(value.Int) : (value.Double);
                break;
            case ADL_TYPE_STRING:
                strcpy( *sval, value.String );
                break;
            }
            
            return AIMMSAPI_SUCCESS;
        }
        
        /* The remainder of the function deals with the generic non-scalar case */
        
        res = AimmsAttributeDefault( handle, &value );
        CHECK_FOR_SUCCESS_DOM("Unable to retrieve default");
        
        switch ( type ) {
        case ADL_TYPE_INT:
            idef = ( storage == AIMMSAPI_STORAGE_INT ) ? (value.Int) : (int)(value.Double);
            break;
        case ADL_TYPE_DOUBLE:
            ddef = ( storage == AIMMSAPI_STORAGE_INT ) ? (double)(value.Int) : (value.Double);
            break;
        case ADL_TYPE_STRING:
            strcpy( sdef, value.String );
            value.Length = size;
            break;
        }
        
        res = AimmsValueResetHandle( handle );
        CHECK_FOR_SUCCESS_DOM("Unable to initialize data retrieval");
        
        if ( AimmsValueNext( handle, tuple, &value ) == AIMMSAPI_SUCCESS ) {
            avail = 1;
            for ( j = 0; j < dim; j++ ) {
                res = AimmsSetElementToOrdinal( domain[j], tuple[j], ords + j );
                CHECK_FOR_SUCCESS_DOM("Unable to obtain ordinal numbers");
                ords[j] -= 1;
            }
        } else {
            avail = 0;
        }
        
        /*
         *  In the following while loop we visit all tuples in the multidimensional array.
         *  The following variables play a role in the algorithm:
         *
         *  running[i] contains the running index for position i in the tuple [i_1, .., i_dim]
         *  cards[i]   contains the maximum cardinality for position i (i.e. running[i] = 0, .., cards[i]-1)
         *  ords[i]    contains the tuple corresponding to the value last retrieved from AIMMS
         *  avail      indicates whether there is still (sparse) AIMMS data available
         *  pos        contains the position in val corresponding to tuple running[i] once completed.
         *  icorr      contains the maximum i below which running[i] == ords[i]
         */
        
        i     = -1;
        pos   = 0;
        icorr = 0;
        
        do {
            if ( i < dim - 1 ) {
                
            /* We have not yet a complete tuple, so we need to proceed to the next running index
               multiplying pos by the appropriate cardinality */
                
                i++;
                pos *= cards[i];
                running[i] = 0;
            } else {
                
                /* We have found a complete tuple! Below is the actual work that
                   needs to be done for such a tuple */
                
                /* First, find whether the running array matches the ords array */
                
                for ( j = icorr; (avail && (j < dim) && (running[j] == ords[j])); j++ ) {
                    icorr++;
                }
                
                switch ( type ) {
                case ADL_TYPE_INT:
                    ival[pos] = ( (! avail) || j < dim ) ? idef :
                    (( storage == AIMMSAPI_STORAGE_INT ) ? (value.Int) : (int)(value.Double));
                    break;
                case ADL_TYPE_DOUBLE:
                    dval[pos] = ( (! avail) || j < dim ) ? ddef :
                    (( storage == AIMMSAPI_STORAGE_INT ) ? (double)(value.Int) : (value.Double));
                    break;
                case ADL_TYPE_STRING:
                    strcpy( sval[pos], ( (! avail) || j < dim ) ? sdef : value.String );
                    value.Length = size;
                    break;
                }
                
                /* Finally, get a new tuple, if there was a match */
                
                if ( avail && j == dim ) 
                    if ( AimmsValueNext( handle, tuple, &value ) == AIMMSAPI_SUCCESS ) {
                        avail = 1;
                        icorr = 0;
                        
                        for ( j = 0; j < dim; j++ ) {
                            res = AimmsSetElementToOrdinal( domain[j], tuple[j], ords + j );
                            CHECK_FOR_SUCCESS_DOM("Unable to obtain ordinal numbers");
                            ords[j] -= 1;
                        } 
                    } else {
                        avail = 0;
                    }
                    
                    /* The general algorithm continues here: increase the running index if possible,
                       or go back as many positions as necessary to find the next running index
                       that can be increased */
                    
                    while ( i != -1 && ++running[i] == cards[i] ) {
                        pos /= cards[i];
                        i--;
                        icorr = min(i,icorr);
                    }
                    
                    /* Increment pos, to account for incrementing running[i] */
                    
                    pos++;
            }
        } while ( i != -1 );
        
        AimmsIdentifierHandleDelete( handle );
        for ( i = 0; i < dim; i++ )
            AimmsIdentifierHandleDelete( domain[i] );
        
        return res;
}

int ADLPutArray( char *id, void *val, int type, int dim, int *cards )
{
    int handle, res, adim, sliced, storage, card, i, pos,
        domain[AIMMSAPI_MAX_DIMENSION], tuple[AIMMSAPI_MAX_DIMENSION],
        running[AIMMSAPI_MAX_DIMENSION];
    AimmsValue value;
    int    *ival  = (int *)val,     idef;
    double *dval  = (double *)val,  ddef;
    char   **sval = (char **)val,  *sdef;
    
    CHECK_FOR_PROJECT;
    
    res = ADLCreateHandle( id, &handle );
    if ( res != AIMMSAPI_SUCCESS ) return res;
    
    res = AimmsAttributeDimension( handle, &adim, &sliced );
    CHECK_FOR_SUCCESS("Unable to obtain dimension");
    
    res = AimmsAttributeStorage( handle, &storage );
    CHECK_FOR_SUCCESS("Unable to obtain identifier storage type")   
        
        if ( ( dim != adim ) ||
            ! ( ( ( type == ADL_TYPE_INT || type == ADL_TYPE_DOUBLE ) &&
            ( storage == AIMMSAPI_STORAGE_DOUBLE || storage == AIMMSAPI_STORAGE_INT ) ) ||
            ( ( type == ADL_TYPE_STRING ) && ( storage == AIMMSAPI_STORAGE_STRING )     )    ) ) {
            sprintf( ADLLastErrorString, "Incorrectly specified dimension or incompatible data types" );
            ADLAddMessage( ADLLastErrorString );
            AimmsIdentifierHandleDelete( handle );
            return AIMMSAPI_FAILURE;
        }
        
        res = AimmsAttributeDeclarationDomain( handle, domain );
        CHECK_FOR_SUCCESS("Unable to obtain declaration domain");
        
        for ( i = 0; i < dim; i++ ) {
            res = AimmsValueCard( domain[i], &card );
            CHECK_FOR_SUCCESS_DOM("Unable to obtain cardinality of domain set");
            
            if ( card != cards[i] ) {
                sprintf( ADLLastErrorString,
                    "Actual cardinality (%d) of domain #%d does not match specified cardinality (%d)",
                    card, i+1, cards[i] );
                ADLAddMessage( ADLLastErrorString );
                AimmsIdentifierHandleDelete( handle );
                for ( i = 0; i < dim; i++ ) 
                    AimmsIdentifierHandleDelete( domain[i] );
                return AIMMSAPI_FAILURE;
            }
        }
        
        if ( type == ADL_TYPE_STRING ) {
            sdef = (char *) _alloca( 1024*sizeof(char) );
            value.Length = 1024;
            value.String = sdef;
        }
        
        res = AimmsAttributeDefault( handle, &value );
        CHECK_FOR_SUCCESS_DOM("Unable to retrieve default");
        
        switch ( type ) {
        case ADL_TYPE_INT:
            idef = ( storage == AIMMSAPI_STORAGE_INT ) ? (value.Int) : (int)(value.Double);
            break;
        case ADL_TYPE_DOUBLE:
            ddef = ( storage == AIMMSAPI_STORAGE_INT ) ? (double)(value.Int) : (value.Double);
            break;
        case ADL_TYPE_STRING:
            strcpy( sdef, value.String );
            break;
        }
        
        res = AimmsIdentifierEmpty( handle );
        CHECK_FOR_SUCCESS_DOM("Unable to empty handle");
        
        i     = -1;
        pos   = 0;
        
        do {
            if ( i < dim - 1 ) {
                
                /* We have not yet a complete tuple, so we need to proceed to the next running index */
                
                i++;
                pos *= cards[i];
                running[i] = 0;
                
                /* Compute the ordinal number corresponding to running[i] */
                
                res = AimmsSetOrdinalToElement( domain[i], running[i] + 1, tuple + i );
                CHECK_FOR_SUCCESS_DOM("Unable to obtain element numbers");
                
            } else {
                
                /* Below is the actual work that needs to be done for a complete tuple */
                
                switch ( type ) {
                case ADL_TYPE_INT:
                    if ( ival[pos] != idef ) {
                        if ( storage == AIMMSAPI_STORAGE_INT )
                            value.Int    = ival[pos];
                        else
                            value.Double = (double)ival[pos];
                        res = AimmsValueAssign( handle, tuple, &value );
                        CHECK_FOR_SUCCESS_DOM("Unable to assign value from multi-dimensional array");
                    }
                    break;
                case ADL_TYPE_DOUBLE:
                    if ( dval[pos] != ddef ) {
                        if ( storage == AIMMSAPI_STORAGE_INT )
                            value.Int    = (int)dval[pos];
                        else
                            value.Double = dval[pos];
                        res = AimmsValueAssign( handle, tuple, &value );
                        CHECK_FOR_SUCCESS_DOM("Unable to assign value from multi-dimensional array");
                    }
                    break;
                case ADL_TYPE_STRING:
                    if ( strcmp( sval[pos], sdef ) ) {
                        strcpy( value.String, sval[pos] );
                        value.Length = strlen( value.String );
                        res = AimmsValueAssign( handle, tuple, &value );
                        CHECK_FOR_SUCCESS_DOM("Unable to assign value from multi-dimensional array");
                    }
                    break;
                }
                
                /* Increase the running index if possible, or go back as many positions as
                   necessary to find the next running index that can be increased */
                
                while ( i != -1 && ++running[i] == cards[i] ) {
                    pos /= cards[i];
                    i--;
                }
                
                /* Compute the ordinal number corresponding to running[i] */
                
                if ( i != -1 ) {
                    res = AimmsSetOrdinalToElement( domain[i], running[i] + 1, tuple + i );
                    CHECK_FOR_SUCCESS_DOM("Unable to obtain element numbers");
                }
                
                pos++;
            }
        } while ( i != -1 );
        
        AimmsIdentifierHandleDelete( handle );
        for ( i = 0; i < dim; i++ )
            AimmsIdentifierHandleDelete( domain[i] );
        
        return res;
}

int ADLMultiToOne( int dim, int *ords, int *cards )
{
    int res, i;
    
    if ( dim == 0 ) return 0;
    
    res = ords[0];
    
    for ( i = 1; i < dim; i++ ) {
        res = ords[i] + res*cards[i] ;
    }
    
    return res;
}

