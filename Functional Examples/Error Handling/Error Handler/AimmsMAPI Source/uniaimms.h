

#ifndef UNIAIMMS
#define UNIAIMMS

#include <limits.h>
#ifdef WIN32
#include <tchar.h>
#else // WIN32
#define _TCHAR char
#define _T(x) x
#endif // WIN32

typedef          char T_CHAR  ; // To indicate the use as (element of) an ordinary ascii string
typedef          char T_BYTE  ; // To indicate the use as a tiny int, eg a boolean or an index position.
typedef unsigned char T_UBYTE ;
typedef   signed char T_SBYTE ;

#define _UNI(x)        L ## x
#define _ASCII(x)      x
#define _ASCII_STR(x)  (T_CHAR *) (x)
#define _ASCII_CHAR(x) (x)

#define SIZEOFSTRING(_s)  (sizeof(_s)/sizeof(_TCHAR))
#define ASSTRINGSIZE(_l)  ((_l)/sizeof(_TCHAR))

#ifdef __cplusplus
    const unsigned short BYTE_ORDER_MARK = 0xFEFF ;
#else
    #define BYTE_ORDER_MARK 0xFEFF
#endif // __cplusplus

#ifdef UNICODE
#define STRCPY_UNI_TO_ASC( _asc, _uni )  \
        wcstombs( _asc, _uni, STRLEN(_uni)+1 )
#define STRCPY_ASC_TO_UNI( _uni, _asc ) \
        mbstowcs( _uni, _asc, strlen(_asc)+1 )
#define STRNCPY_UNI_TO_ASC( _asc, _uni, _n ) \
        wcstombs( _asc, _uni, _n )
#define STRNCPY_ASC_TO_UNI( _uni, _asc, _n ) \
        mbstowcs( _uni, _asc, _n )
#define STRCPY_BSTR_TO_TCHAR( _tchar, _bstr ) \
        _tcscpy( _tchar, _bstr )
#define tstring std::wstring
#define otstringstream std::wostringstream
#define itstringstream std::wistringstream
#define itstream std::wistream
#define otstream std::wostream
#define otfstream std::wofstream
#define itfstream std::wifstream
#else
#define STRCPY_UNI_TO_ASC( _asc, _uni ) \
        strcpy( _asc, _uni )
#define STRCPY_ASC_TO_UNI( _uni, _asc ) \
        strcpy( _uni, _asc )
#define STRNCPY_UNI_TO_ASC( _asc, _uni, _n ) \
        strncpy( _asc, _uni, _n )
#define STRNCPY_ASC_TO_UNI( _uni, _asc, _n ) \
        strncpy( _uni, _asc, _n )
#define STRCPY_BSTR_TO_TCHAR( _tchar, _bstr ) \
                wcstombs( _tchar, _bstr, _tcsclen(_bstr)+1 )
#define tstring std::string
#define otstringstream std::ostringstream
#define itstringstream std::istringstream
#define itstream std::istream
#define otstream std::ostream
#define otfstream std::ofstream
#define itfstream std::ifstream
#endif

#ifdef WIN32
#define __ISASCII _istascii
#define _ACCESS _taccess
#define _CHDIR _tchdir
#define _CHMOD _tchmod
#define _CREAT _tcreat
#define _EXECL _texecl
#define _EXECLE _texecle
#define _EXECLP _texeclp
#define _EXECLPE _texeclpe
#define _EXECV _texecv
#define _EXECVE _texecve
#define _EXECVP _texecvp
#define _EXECVPE _texecvpe
#define _FDOPEN _tfdopen
#define _FINDFIRST _tfindfirst
#define _FINDNEXT _tfindnext
#define _FSOPEN _tfsopen
#define _FULLPATH _tfullpath
#define _GETCWD _tgetcwd
#define _ITOA _itot
#define _LTOA _ltot
#define _MAKEPATH _tmakepath
#define _MKDIR _tmkdir
#define _MKTEMP _tmktemp
#define _OPEN _topen
#define _POPEN _tpopen
#define _PUTENV _tputenv
#define _RMDIR _trmdir
#define _SEARCHENV _tsearchenv
#define _SNPRINTF _sntprintf
#define _SOPEN _tsopen
#define _SPAWNL _tspawnl
#define _SPAWNLE _tspawnle
#define _SPAWNLP _tspawnlp
#define _SPAWNLPE _tspawnlpe
#define _SPAWNV _tspawnv
#define _SPAWNVE _tspawnve
#define _SPAWNVP _tspawnvp
#define _SPAWNVPE _tspawnvpe
#define _SPLITPATH _tsplitpath
#define STRUCT_STAT struct _stat
#define _STAT _tstat
#define _STRDATE _tstrdate
#define _STRDEC _tcsdec
#define _STRDUP _tcsdup
#define _STRICMP _tcsicmp
#define _STRICOLL _tcsicoll
#define _STRINC _tcsinc
#define _STRLWR _tcslwr
    //#define _STRNCNT _tcsnbcnt
    //#define _STRNCNT _tcsnccnt
    //#define _STRNCNT _tcsnccnt
#define _STRNEXTC _tcsnextc
#define _STRNICMP _tcsncicmp
    //#define _STRNICMP _tcsnicmp
#define _STRNICOLL _tcsnicoll
#define _STRNINC _tcsninc
#define _STRNSET _tcsncset
#define _STRREV _tcsrev
#define _STRSET _tcsset
#define _STRSPNP _tcsspnp
#define _STRTIME _tstrtime
#define _STRUPR _tcsupr
#define _TEMPNAM _ttempnam
#define _ULTOA _ultot
#define _UTIME _tutime
#define _VSNPRINTF _vsntprintf
#define ASCTIME _tasctime
#define ATOF _tstof
#define ATOI _ttoi
#define ATOL _ttol
#define CTIME _tctime
#define FGETC _fgettc
#define FGETCHAR _fgettchar
#define FGETS _fgetts
#define FOPEN _tfopen
#define FPRINTF _ftprintf
#define FPUTC _fputtc
#define FPUTCHAR _fputtchar
#define FPUTS _fputts
#define FREOPEN _tfreopen
#define FSCANF _ftscanf
#define GETC _gettc
#define GETCHAR _gettchar
#define GETENV _tgetenv
#define GETS _getts
#define ISALNUM _istalnum
#define ISALPHA _istalpha
#define ISCNTRL _istcntrl
#define ISDIGIT _istdigit
#define ISGRAPH _istgraph
#define ISLOWER _istlower
#define ISPRINT _istprint
#define ISPUNCT _istpunct
#define ISSPACE _istspace
#define ISUPPER _istupper
#define ISXDIGIT _istxdigit
#define MAIN _tmain
#define PERROR _tperror
#define PRINTF _tprintf
#define PUTC _puttc
#define PUTCHAR _puttchar
#define PUTS _putts
#define REMOVE _tremove

// With both Linux/Win32 you may want to use the MoveFile function instead; Linux: separate implemation that moves across file systems, Win32: intrinsic function.
#define RENAME _trename

//#define SCANF scanf
#define SCANF _tscanf
#define SETLOCALE _tsetlocale
#define SPRINTF _stprintf
#define SSCANF _stscanf
#define STRCAT _tcscat
#define STRCMP _tcscmp
#define STRCOLL _tcscoll
#define STRCPY _tcscpy
#define STRCSPN _tcscspn
#define STRFTIME _tcsftime
#define STRLEN _tcsclen
#define STRNCAT _tcsnccat
#define STRNCMP _tcsncmp
#define STRNICMP _tcsnicmp
#define STRICMP _tcsicmp
#define STRNCPY _tcsncpy
#define STRPBRK _tcspbrk
#define STRSPN _tcsspn

#define iSTRLEN(x)  (int)STRLEN(x)

#if defined( UNICODE ) && defined( __cplusplus )
        // CK (June 2003): Purify instrumented code crashes on UNICODE versions of _tcschr (strchr)
        ///                and _tcsstr (strstr).  Since march 2004 also on _tcsrchr (strrchr).
        //#include <strchrstr.h>

#ifndef STRCHR
#define STRCHR pgntls_strchr_inline
            // Return a pointer to the first occurrence of c in str or NULL if there is no char c in str.
            inline _TCHAR *pgntls_strchr_inline( const _TCHAR *str, _TCHAR c )
            {
                size_t i, len ;

                len = STRLEN( str );
                for ( i = 0 ; i < len ; i ++ ) {
                    if ( str[ i ] == c ) {
                        return const_cast<_TCHAR *>(str + i) ;
                    }
                }

                return NULL ;
            }
#endif // STRCHR

#ifndef STRRCHR
#define STRRCHR pgntls_strrchr_inline
            // Return a pointer to the last occurrence of c in str or NULL if there is no char c in str.
            inline _TCHAR *pgntls_strrchr_inline( const _TCHAR *str, _TCHAR c )
            {
                _TCHAR *t, *s ;
                size_t len ;

                s = const_cast<_TCHAR *>(str);
                len = STRLEN( s );
                for ( t = s + (len - 1) ; t >= s ; t -- ) {
                    if ( *t == c ) {
                        return t ;
                    }
                }

                return NULL ;
            }
#endif // STRRCHR

#ifndef STRSTR
#define STRSTR pgntls_strstr_inline
            // Return a pointer to the first occurrence of substr in superstr or NULL if there is no substr in superstr.
            inline _TCHAR *pgntls_strstr_inline( const _TCHAR *superstr, const _TCHAR *substr )
            {
                size_t superlen, sublen, maxtests, i ;

                if ( superstr && substr ) {
                    superlen = STRLEN( superstr );
                    sublen = STRLEN( substr );
                    if ( superlen >= sublen ) {
                        maxtests = superlen - sublen ;
                        for ( i = 0 ; i <= maxtests ; i ++ ) {
                            if ( STRNCMP( superstr + i, substr, sublen ) == 0 ) {
                                return const_cast<_TCHAR *>(superstr + i) ;
                            }
                        }
                    }
                }

                return NULL ;
            }

#endif // STRSTR


#else // UNICODE
#define STRCHR  _tcschr
#define STRSTR  _tcsstr
#define STRRCHR _tcsrchr
#endif // UNICODE

#define STRTOD _tcstod
#define STRTOK _tcstok
#define STRTOL _tcstol
#define STRTOUL _tcstoul
#define STRXFRM _tcsxfrm
#define SYSTEM _tsystem
#define TMPNAM _ttmpnam
#define TOLOWER _totlower
#define TOUPPER _totupper
#define UNGETC _ungettc
#define VFPRINTF _vftprintf
#define VPRINTF _vtprintf
#define VSPRINTF _vstprintf
#define WINMAIN _tWinMain

#else // WIN32

#define _TEOF EOF
#define __ISASCII isascii
#define _ACCESS access
#define _CHDIR chdir
#define _CHMOD chmod
#define _CREAT creat
#define _EXECL execl
#define _EXECLE execle
#define _EXECLP execlp
#define _EXECLPE execlpe
#define _EXECV execv
#define _EXECVE execve
#define _EXECVP execvp
#define _EXECVPE execvpe
#define _FDOPEN fdopen
#define _FINDFIRST findfirst
#define _FINDNEXT findnext
#define _FSOPEN fsopen
#define _FULLPATH fullpath
#define _GETCWD getcwd
#define _ITOA itoa
#define _LTOA ltoa
#define _MAKEPATH makepath
#define _MKDIR mkdir
#define _MKTEMP mktemp
#define _OPEN open
#define _POPEN popen
#define _PUTENV putenv
#define _RMDIR rmdir
#define _SEARCHENV searchenv
#define _SNPRINTF snprintf
#define _SOPEN sopen
#define _SPAWNL spawnl
#define _SPAWNLE spawnle
#define _SPAWNLP spawnlp
#define _SPAWNLPE spawnlpe
#define _SPAWNV spawnv
#define _SPAWNVE spawnve
#define _SPAWNVP spawnvp
#define _SPAWNVPE spawnvpe
#define _SPLITPATH splitpath
#define STRUCT_STAT struct stat
#define _STAT stat
#define _STRDATE strdate
#define _STRDEC strdec
#define _STRDUP strdup
#define _STRICMP strcasecmp
#define _STRICOLL stricoll
#define _STRINC strinc
#define _STRLWR strlwr
#define _STRNEXTC strnextc
#define _STRNICMP strncicmp
#define _STRNICOLL strnicoll
#define _STRNINC strninc
#define _STRNSET strncset
#define _STRREV strrev
#define _STRSET strset
#define _STRSPNP strspnp
#define _STRTIME strtime
#define _STRUPR strupr
#define _TEMPNAM tempnam
#define _ULTOA ultoa
#define _UTIME utime
#define _VSNPRINTF vsnprintf
#define ASCTIME asctime
#define ATOF stof
#define ATOI atoi
#define ATOL atol
#define CTIME ctime
#define FGETC fgetc
#define FGETCHAR fgetchar
#define FGETS fgets
#define FOPEN fopen
#define FPRINTF fprintf
#define FPUTC fputc
#define FPUTCHAR fputchar
#define FPUTS fputs
#define FREOPEN freopen
#define FSCANF fscanf
#define GETC getc
#define GETCHAR getchar
#define GETENV getenv
#define GETS _getts
#define ISALNUM isalnum
#define ISALPHA isalpha
#define ISCNTRL iscntrl
#define ISDIGIT isdigit
#define ISGRAPH isgraph
#define ISLOWER islower
#define ISPRINT isprint
#define ISPUNCT ispunct
#define ISSPACE isspace
#define ISUPPER isupper
#define ISXDIGIT isxdigit
#define MAIN main
#define PERROR perror
#define PRINTF printf
#define PUTC putc
#define PUTCHAR putchar
#define PUTS puts
#define REMOVE remove
#define SCANF scanf

// With both Linux/Win32 you may want to use the MoveFile function instead; Linux: separate implemation that moves across file systems, Win32: intrinsic function.
#define RENAME rename

#define SETLOCALE setlocale
#define SPRINTF sprintf
#define SSCANF sscanf
#define STRCAT strcat
#define STRCMP strcmp
#define STRCOLL strcoll
#define STRCPY strcpy
#define STRCSPN strcspn
#define STRFTIME strftime
#define STRLEN strlen
#define STRNCAT strncat
#define STRNCMP strncmp
#define STRNICMP strncasecmp
#define STRICMP strcasecmp
#define STRNCPY strncpy
#define STRPBRK strpbrk
#define STRSPN strspn

#define iSTRLEN(x)  (int)STRLEN(x)

#define STRCHR  strchr
#define STRSTR  strstr
#define STRRCHR strrchr

#define STRTOD strtod
#define STRTOK strtok
#define STRTOL strtol
#define STRTOUL strtoul
#define STRXFRM strxfrm
#define SYSTEM system
#define TMPNAM tmpnam
#define TOLOWER tolower
#define TOUPPER toupper
#define UNGETC ungetc
#define VFPRINTF vfprintf
#define VPRINTF vprintf
#define VSPRINTF vsprintf
#define WINMAIN main

extern _TCHAR *strlwr( _TCHAR *str );
extern _TCHAR *strupr( _TCHAR *str );
extern wchar_t *wcslwr( wchar_t *str );
extern wchar_t *wcsupr( wchar_t *str );

#endif // WIN32


#ifdef UNICODE
#define sizeof_TCHAR 2
#else // UNICODE
#define sizeof_TCHAR 1
#endif // else UNICODE
#define sizeof_T_BYTE  1
#define sizeof_T_UBYTE 1
#define sizeof_T_SBYTE 1
#define sizeof_T_CHAR  1

#endif // UNIAIMMS

