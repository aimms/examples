// get.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "get.h"

#include <stdio.h>
#include <assert.h>

#include <vector>
#include <string>

#include "fileinfo.h"

bool tracing = false ;
_TCHAR errmsg[ 4048 ];

void Trace(_TCHAR *fmt, ... )
{
    FILE *fp ;
    va_list args;

    if ( tracing ) {
        fp = FOPEN( _T("get.log"), _T("a") );
        if ( fp ) {
            va_start( args, fmt );
            _vftprintf( fp, fmt, args );
            va_end( args );
            fclose( fp );
        }
    }
}

// Register an error.
void Error(_TCHAR *fmt, ... )
{
    va_list args;

    // Store the error in the global errmsg.
    va_start( args, fmt );
    _vsntprintf( errmsg, sizeof( errmsg ) / sizeof( _TCHAR ), fmt, args );
    va_end( args );

    Trace( _T("  Error:\"%s\"\n"), errmsg );
}

DLL_EXPORT_PROTO(int) lastErrorMessage( _TCHAR *err )
{
    _tcsncpy( err, errmsg, 255 );

    return 1 ;
}

static std::vector<fileInfo*> fileHandles ;

// Returns the file handle for filename in hndl.
DLL_EXPORT_PROTO(int) openFileHandle( wchar_t *filename, int *hndl )
{
    FILE *file = FOPEN( filename, _T("r") );

    Trace(_T("Hi, I'm openFileHandle(%s, *)\n"), filename );

    if ( ! file )  {
        Error( _T("File \"%s\" could not be opened"), filename );
        return 0 ;
    }

    fileInfo * fi = new fileInfo( filename, file );
        
    if ( ! fi ) {
        Error( _T("fileInfo object could not be created") );
        return 0 ;
    }

    int sz = static_cast<int>( fileHandles.size() );

    // Check for an empty slot and use it when available.
    for ( int i = 0 ; i < sz ; i ++ ) {
        if ( fileHandles[ i ] == NULL ) {
            *hndl = i ;
            fileHandles[ i ] = fi ;

            return 1 ;
        }
    }

    // No empty slot found.
    // Place at end.
    fileHandles.push_back( fi );
    *hndl = sz ;

    assert( sz+1 == fileHandles.size() );

    return 1 ;
}

// Closes file handle
DLL_EXPORT_PROTO(int) closeFileHandle( int hndl )
{
    int sz = static_cast<int>( fileHandles.size() );

    Trace(_T("Hi, I'm closeFileHandle(%d)\n"), hndl );    

    if ( ( 0 <= hndl ) && ( hndl < sz ) ) {
        if ( fileHandles[ hndl ] ) {
            fileInfo *fi = fileHandles[ hndl ];
            fileHandles[ hndl ] = NULL ;
            fclose( fi->file() );

            delete fi ;
        } else {
            // File already closed.
            Error( _T("File %d already closed"), hndl );

            return 0 ;
        }
    } else {
        Error( _T("File %d does not exist"), hndl );
        // Handle out of range.
        return 0;
    }

    return 1 ;
}

// Get a line from file in hndl, possibly including newline.
DLL_EXPORT_PROTO(int) getLine( int hndl, _TCHAR *line, int maxSize )
{
    int sz = static_cast<int>( fileHandles.size() );

    Trace(_T("Hi, I'm getLine(%d, *, %d)\n"), hndl, maxSize );
    
    if ( ( 0 <= hndl ) && ( hndl < sz ) ) {
        fileInfo *fi = fileHandles[ hndl ];
        if ( fi ) {
            return fi -> getLine( line, maxSize );
        } else {
            // Invalid handle.
            Error( _T("getLine(): Invalid handle %d"), hndl );
            return 0 ;
        }
    } else {
        // Handle out of range.
        Error( _T("getLine(): Handle out of range. %d"), hndl );
        return 0;
    }

    return 1 ;
}

// Upon success, result stored in i.
DLL_EXPORT_PROTO(int) getInt( int hndl, int *i )
{
    int sz = static_cast<int>( fileHandles.size() );

    Trace(_T("Hi, I'm getInt(%d, *)\n"), hndl  );
    
    if ( ( 0 <= hndl ) && ( hndl < sz ) ) {
        fileInfo *fi = fileHandles[ hndl ];
        if ( fi ) {
            return fi -> getInt( i );
        } else {
            // Invalid handle.
            Error( _T("getInt(): Invalid handle %d"), hndl );
            return 0 ;
        }
    } else {
        // Handle out of range.
        Error( _T("getInt(): Handle out of range. %d"), hndl );
        return 0;
    }

    return 1 ;
}


// Upon success, result stored in dbl.
DLL_EXPORT_PROTO(int) getDouble( int hndl, double *dbl )
{
    int sz = static_cast<int>( fileHandles.size() );

    Trace(_T("Hi, I'm getDouble(%d, *)\n"), hndl  );

    if ( ( 0 <= hndl ) && ( hndl < sz ) ) {
        fileInfo *fi = fileHandles[ hndl ];
        if ( fi ) {
            return fi -> getDouble( dbl );
        } else {
            // Invalid handle.
            Error( _T("getDouble(): Invalid handle %d"), hndl );
            return 0 ;
        }
    } else {
        // Handle out of range.
        Error( _T("getDouble(): Handle out of range. %d"), hndl );
        return 0;
    }

    return 1 ;
}

// Initializes the locals of this module.
int get_start( void )
{
    fileHandles.clear();
    return 1 ;
}

// cleanup of the locals of this module.
int get_finish( void )
{
    get_start();

    return 1 ;
}


