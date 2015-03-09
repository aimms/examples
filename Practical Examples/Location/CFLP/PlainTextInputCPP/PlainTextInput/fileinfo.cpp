#include "StdAfx.h"
#include "fileinfo.h"
#include "get.h"

// Constructor
fileInfo::fileInfo(_TCHAR *fn, FILE *file)
    :   m_Filename( fn)
    ,   m_File( file )
    ,   m_LineNumber( 1 )
    ,   m_ColumnNumber( 0 )
{
    Trace( _T("fileInfo::fileInfo(%s, file = %s)\n"), fn, file ? _T("not NULL") : _T("NULL") );
} 

// Destructor.
fileInfo::~fileInfo(void) 
{
    Trace( _T("fileInfo::~fileInfo() m_File = %s\n"), m_File ? _T("not NULL") : _T("NULL") );
    if ( m_File ) {
        fclose( m_File );
        m_File = NULL ;
    }
}

// Get a line, possibly including a newline character.
// Returns 0 upon error, 1 upon succes.
int fileInfo::getLine(_TCHAR *buf, int maxSize)
{
    Trace( _T("Hi, I'm fileInfo::getLine(*, %d)\n"), maxSize );

    if ( m_File ) {
        if ( _fgetts( buf, maxSize, file() ) ) {
            Trace(_T("read \"%s\" from file\n"), buf );
            return 1 ;
        } else {
            return 0 ;
        }
    } else {
        return 0 ;
    }
}


int fileInfo::getch(int *pch) // Return a character or feof.
{
    int ch ;

    if ( feof( file() ) || ferror( file() ) ) {
        Error( _T("error reading file") );
        return 0 ;
    }
    ch = _fgettc( file() );
    if ( ch == _T('\n') ) {
        m_LineNumber ++ ;
        m_ColumnNumber = 0 ;
    } else {
        m_ColumnNumber ++ ;
    }
    *pch = ch ;

    return 1 ;
}

// Get an int and return it into i.
// Returns 0 upon error, 1 upon success.
int fileInfo::getInt(int *i)
{
    _TCHAR buf[ 256 ];
    int ch, rc, noDigit ;

    rc = getch(&ch);
    if ( ! rc ) return 0 ;
    while ( _istspace( ch ) ) {
        rc = getch( &ch );
        if ( ! rc ) return 0 ;
    }

    noDigit = 0 ;
    while ( _istdigit( ch ) ) {
        buf[ noDigit ++ ] = static_cast<_TCHAR>( ch ) ;
        if ( feof( file() ) ) break ;
        rc = getch( &ch );
        if ( ! rc ) return 0 ;
    }
    buf[ noDigit ] = _T('\0');
    if ( noDigit ) {
        *i = _tstoi( buf );
    } else {
        *i = 0 ;
        Error( _T("Digit expected\n") );
        return 0 ;
    }
    return 1 ;
}

// Get a double and return it into d.
// Returns 0 upon error, 1 upon success.
int fileInfo::getDouble(double *d)
{
    wchar_t buf[ 256 ];
    int ch, rc, noDigit ;
    wchar_t *dummy ;

    rc = getch(&ch);
    if ( ! rc ) return 0 ;
    while ( _istspace( ch ) ) {
        rc = getch( &ch );
        if ( ! rc ) return 0 ;
    }

    noDigit = 0 ;
    while ( _istdigit( ch ) ) {
        buf[ noDigit ++ ] = static_cast<_TCHAR>( ch ) ;
        if ( feof( file() ) ) break ;
        rc = getch( &ch );
        if ( ! rc ) return 0 ;
    }
    if ( ch == _T('.') ) {
        buf[ noDigit ++ ] = static_cast<_TCHAR>( ch ) ;
        rc = getch( &ch );
        while ( _istdigit( ch ) ) {
            buf[ noDigit ++ ] = static_cast<_TCHAR>( ch ) ;
            if ( feof( file() ) ) break ;
            rc = getch( &ch );
            if ( ! rc ) return 0 ;
        }
    }
    if ( noDigit ) {
        if ( ( ch == L'e' ) || ( ch == L'E') ) {
            buf[ noDigit ++ ] = static_cast<_TCHAR>( ch ) ;
            rc = getch( &ch );
            while ( _istdigit( ch ) ) {
                buf[ noDigit ++ ] = static_cast<_TCHAR>( ch ) ;
                if ( feof( file() ) ) break ;
                rc = getch( &ch );
                if ( ! rc ) return 0 ;
            }
        }
    }
    buf[ noDigit ] = _T('\0');
    if ( noDigit ) {
        *d = wcstod(  buf, &dummy );
    } else {
        *d = 0 ;
        Error( _T("Digit or \'.\' expected\n") );
        return 0 ;
    }
    return 1 ;
}



