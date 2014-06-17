#ifndef _AIMMSAPI_H_
#define _AIMMSAPI_H_



#ifndef _ASCII
#define _ASCII(x) x
#endif

#define AIMMSAPI_SUCCESS  1
#define AIMMSAPI_FAILURE  0
#define AIMMSAPI_TRUE     1
#define AIMMSAPI_FALSE    0

#define AIMMSAPI_STORAGE_HANDLE  0x00000000
#define AIMMSAPI_STORAGE_DOUBLE  0x00000001
#define AIMMSAPI_STORAGE_INT     0x00000002
#define AIMMSAPI_STORAGE_BINARY  0x00000003
#define AIMMSAPI_STORAGE_STRING  0x00000004

#define AIMMSAPI_INPUT           0x00000010
#define AIMMSAPI_OUTPUT          0x00000020
#define AIMMSAPI_INPUT_OUTPUT    0x00000040

#define AIMMSAPI_NO_IDTYPE                       0
#define AIMMSAPI_IDTYPE_SIMPLE_SET_ROOT          1
#define AIMMSAPI_IDTYPE_SIMPLE_SET_SUBSET        2
#define AIMMSAPI_IDTYPE_COMPOUND_SET_ROOT        11
#define AIMMSAPI_IDTYPE_COMPOUND_SET_SUBSET      12
#define AIMMSAPI_IDTYPE_SET_RELATION             20
#define AIMMSAPI_IDTYPE_SET_INDEXED              21
#define AIMMSAPI_IDTYPE_PARAMETER_NUMERICS       30
#define AIMMSAPI_IDTYPE_PARAMETER_ELEMENTS       31
#define AIMMSAPI_IDTYPE_PARAMETER_STRINGS        32
#define AIMMSAPI_IDTYPE_PARAMETER_UNITS          33
#define AIMMSAPI_IDTYPE_VARIABLE                 40
#define AIMMSAPI_IDTYPE_VARIABLE_SUFFIX          50
#define AIMMSAPI_IDTYPE_EQUATION_SUFFIX          51
#define AIMMSAPI_IDTYPE_MODEL_SUFFIX             52
#define AIMMSAPI_IDTYPE_FILE_TOKEN_SUFFIX        53
#define AIMMSAPI_IDTYPE_INDEX                    60
#define AIMMSAPI_IDTYPE_ELEMENT_VARIABLE         70

#define AIMMSAPI_FLAG_ORDERED               0x00000001
#define AIMMSAPI_FLAG_RETAINSPECIALS        0x00000002
#define AIMMSAPI_FLAG_RAW                   0x00000004
#define AIMMSAPI_FLAG_NO_INACTIVE_DATA      0x00000008
#define AIMMSAPI_FLAG_READONLY              0x00000010
#define AIMMSAPI_FLAG_ELEMENTS_AS_ORDINALS  0x00000020
#define AIMMSAPI_FLAG_UNITS                 0x00000040
#define AIMMSAPI_FLAG_ELEMENTS_AS_STRINGS   0x00000080         // For XML
#define AIMMSAPI_FLAG_SAVE_REAL_DOMAIN      0x00000100         // For XML
#define AIMMSAPI_FLAG_IS_LOCAL              0x00000200
#define AIMMSAPI_FLAG_CODE_XML_CHARACTERS   0x00000400         // For XML
#define AIMMSAPI_FLAG_DO_NOT_CHECK_TUPLES   0x00000800
#define AIMMSAPI_FLAG_SAVE_DECL_DOMAIN      0x00001000         // For XML
#define AIMMSAPI_FLAG_USE_DOMAIN_COND       0x00002000

#define AIMMSAPI_SEVERITY_CODE_NEVER        0
#define AIMMSAPI_SEVERITY_CODE_ERROR        1
#define AIMMSAPI_SEVERITY_CODE_WARNING      2
#define AIMMSAPI_SEVERITY_CODE_REMARK       3
#define AIMMSAPI_SEVERITY_CODE_ALL          4

#define AIMMSAPI_NO_ELEMENT              0
#define AIMMSAPI_NO_SYMBOL               0
#define AIMMSAPI_MAX_DIMENSION           32
#define AIMMSAPI_ERROR_BUF_SIZE          4096

#define AIMMSAPI_NULL_HANDLE_NUMBER      0
#define AIMMSAPI_NO_ORDINAL              0
#define AIMMSAPI_MAX_NUMB_OF_PROC_ARG    256

#define AIMMSAPI_MAX_PENDING_REQUESTS    32
#define AIMMSAPI_NULL_REQUEST_NUMBER     -1

#define AIMMSAPI_STATUS_PENDING      0
#define AIMMSAPI_STATUS_RUNNING      1
#define AIMMSAPI_STATUS_FINISHED     2
#define AIMMSAPI_STATUS_DELETED      3
#define AIMMSAPI_STATUS_UNKNOWN      4

#define AIMMSAPI_START_HANDLE_NUMBERING   25600
#define AIMMSAPI_START_PROJECT_NUMBERING  25600

#define AIMMSAPI_WAIT_INFINITE      0xFFFFFFFF
#define WAIT_INFINITE               AIMMSAPI_WAIT_INFINITE

// Flags used in AimmsInterruptCallback
#define AIMMSAPI_INTERRUPT_CONTEXT_EXECUTING 0
#define AIMMSAPI_INTERRUPT_CONTEXT_SOLVING   1

// Return values for AimmsInterruptCallback
#define AIMMSAPI_INTERRUPT_NOT      0
#define AIMMSAPI_INTERRUPT_EXECUTE  1
#define AIMMSAPI_INTERRUPT_SOLVE    2



/*
  The status codes as returned by AimmsAPIStatus:

  AIMMS_SUCCESS            0
  AIMMS_WARNING            1
  AIMMS_ASSERTION_FAILED   2
  AIMMS_ARITHMETIC_ERROR   3
  AIMMS_UNDECLARED_ERROR   4
  AIMMS_COMPILATION_ERROR  5
  AIMMS_ABORTED            6
  AIMMS_INTERRUPTED        7
  AIMMS_EDITED_ERROR       8
  AIMMS_FATAL_ERROR        9
*/


enum {
  API_NO_ERROR                              =   0 ,
  API_ERROR_MEMORY                          = 100 ,
  API_ERROR_INCORRECT_IDENTIFIER_NAME             ,
  API_ERROR_INCORRECT_PROCEDURE_NAME              ,
  API_ERROR_NO_VALID_SUFFIX                       ,
  API_ERROR_INCORRECT_IDENTIFIER_TYPE             ,
  API_ERROR_UNEXPECTED_CALL_DOMAIN                ,
  API_ERROR_UNEXPECTED_SLICING                    ,
  API_ERROR_SLICING_LAST_DIM_INDEXED_SET          ,
  API_ERROR_SLICE_NOT_IN_ROOT                     ,
  API_ERROR_NULL_HANDLE                           ,
  API_ERROR_NO_VALID_HANDLE                       ,
  API_ERROR_NO_IDENTIFIER_HANDLE                  ,
  API_ERROR_IS_PROCEDURE_HANDLE                   ,
  API_ERROR_NO_PROCEDURE_HANDLE                   ,
  API_ERROR_NO_VALID_FLAG                         ,
  API_ERROR_NO_INDEXED_IDENTIFIER                 ,
  API_ERROR_NO_ELEMENT_PARAMETER                  ,
  API_ERROR_ROOT_DOMAIN                           ,
  API_ERROR_CALL_DOMAIN_NO_SUBSET_OF_ROOT         ,
  API_ERROR_EMPTY_DEFINED_IDENTIFIER              ,
  API_ERROR_RESET_HANDLE_SCALAR                   ,
  API_ERROR_SEARCH_SCALAR                         ,
  API_ERROR_NEXT_SCALAR                           ,
  API_ERROR_FIRST_RESET_OR_SEARCH                 ,
  API_ERROR_NO_NEXT_ELEMENT                       ,
  API_ERROR_CHANGE_DEFINED_IDENTIFIER             ,
  API_ERROR_CHANGE_NON_OUTPUT_ARGUMENT            ,
  API_ERROR_NO_SIMPLE_COMPOUND_ROOT_SET           ,
  API_ERROR_NO_SIMPLE_SET                         ,
  API_ERROR_NO_SIMPLE_ROOT_SET                    ,
  API_ERROR_NO_COMPOUND_SET                       ,
  API_ERROR_NO_SIMPLE_OR_COMPOUND_SET             ,
  API_ERROR_ELEMENT_ALREADY_IN_SET                ,
  API_ERROR_ELEMENT_NOT_IN_ROOT                   ,
  API_ERROR_ELEMENT_NOT_IN_SET                    ,
  API_ERROR_ORDINAL_NOT_IN_SET                    ,
  API_ERROR_ELEMENT_NAME_NOT_IN_SET               ,
  API_ERROR_TUPLE_NOT_IN_ROOT_OF_COMPOUND_SET     ,
  API_ERROR_TUPLE_NOT_IN_ROOT_DOMAIN              ,
  API_ERROR_TUPLE_NOT_IN_DECLARATION_DOMAIN       ,
  API_ERROR_TUPLE_NOT_IN_CALL_DOMAIN              ,
  API_ERROR_TUPLE_NOT_IN_COMPOUND_DOMAIN          ,
  API_ERROR_NO_PROCEDURE                          ,
  API_ERROR_INVALID_ELEMENT_NUMBER                ,
  API_ERROR_ELEMENT_NOT_IN_RANGE                  ,
  API_ERROR_NO_NON_DEFAULT_ELEMENT_LEFT           ,
  API_ERROR_NOT_0_OR_1                            ,
  API_ERROR_INCORRECT_STORAGE_TYPE_FOR_NON_SCALAR ,
  API_ERROR_INCORRECT_DIMENSION_FOR_ARGUMENT      ,
  API_ERROR_PROC_DIFFERENT_IDENTIFIER_TYPES       ,
  API_ERROR_PROC_DIFFERENT_STORAGE_TYPES          ,
  API_ERROR_PROC_DIFFERENT_DEFAULT_VALUES         ,
  API_ERROR_PROC_NOT_SAME_ROOT_DOMAIN             ,
  API_ERROR_PROC_INCORRECT_ROOT_DOMAIN            ,
  API_ERROR_PROC_INCORRECT_GLOBAL_DECL_DOMAIN     ,
  API_ERROR_PROC_INCORRECT_LOCAL_DECL_DOMAIN      ,
  API_ERROR_PROC_INCORRECT_GLOBAL_SET_RANGE       ,
  API_ERROR_PROC_INCORRECT_LOCAL_SET_RANGE        ,
  API_ERROR_DELETE_PROCEDURE_HANDLE               ,
  API_ERROR_DELETE_PASSED_HANDLE                  ,
  API_ERROR_INCORRECT_SEVERITY_CODE               ,
  API_ERROR_INTEGER_SPECIAL_VALUE                 ,
  API_ERROR_LARGE_DOUBLE_TO_INT                   ,
  API_ERROR_CHANGE_NON_OUTPUT_SUPERSET            ,
  API_ERROR_CHANGE_DEFINED_SUPERSET               ,
  API_ERROR_PROC_RELATION                         ,
  API_ERROR_INCORRECT_HANDLE                      ,
  API_ERROR_INCORRECT_HANDLE_CALL_DOMAIN          ,
  API_ERROR_INCORRECT_HANDLE_PROCEDURE_ARGUMENT   ,
  API_ERROR_NULL_AIMMSSTRING                      ,
  API_ERROR_SCHEDULE_PROCEDURE                    ,
  API_ERROR_INCORRECT_WORKING_DIRECTORY           ,
  API_ERROR_OPEN_PROJECT                          ,
  API_ERROR_STILL_REQUESTS_RUNNING_OR_PENDING     ,
  API_ERROR_TO_MANY_PENDING_REQUESTS              ,
  API_ERROR_INVALID_REQUEST_HANDLE_NUMBER         ,
  API_ERROR_REQUEST_HANDLE_ALREADY_DELETED        ,
  API_ERROR_DELETING_RUNNING_REQUEST              ,
  API_ERROR_REMOVING_PENDING_REQUEST_FAILED       ,
  API_ERROR_DOUBLE_OUT_OF_RANGE                   ,
  API_ERROR_INT_OUT_OF_RANGE                      ,
  API_ERROR_ASSIGN_MULTI_NOT_FOR_SCALAR           ,
  API_ERROR_NEXT_MULTI_NOT_FOR_SCALAR             ,
  API_ERROR_NEXT_MULTI_NO_POSITIVE_N              ,
  API_ERROR_COMPOUND_NOT_IN_ROOT_OF_COMPOUND_SET  ,
  API_ERROR_INVALID_PROJECT_HANDLE                ,
  API_ERROR_NO_CONTROL                            ,
  API_ERROR_ELEMENT_TO_ADD_NOT_IN_ROOT            ,
  API_ERROR_PROCEDURE_ALREADY_RUNNING             ,
  API_ERROR_PROCEDURE_RUNNING_FAILURE             ,
  API_ERROR_ANY_PROJECT_FAILURE                   ,
  API_ERROR_PERMUTE_VALUE_INCORRECT               ,
  API_ERROR_PERMUTE_OF_SLICED_DIM_NOT_ZERO        ,
  API_ERROR_DOUBLY_NONZERO_VALUES_IN_PERMUTATION  ,
  API_ERROR_PERMUTED_LAST_DIM_INDEXED_SET         ,
  API_ERROR_UNEXPECTED_PERMUTATION                ,
  API_ERROR_CHANGE_PERMUTED_HANDLE                ,
  API_ERROR_TUPLE_IS_NULL                         ,
  API_ERROR_NO_VALID_POINTER                      ,
  API_ERROR_PROCEDURE_NO_BODY_CALL                ,
  API_ERROR_UNIT_DOES_NOT_APPLY_TO_IDENTIFIER     ,
  API_ERROR_LICENSE_FAILURE                       ,
  API_ERROR_ILLEGAL_LIBRARY_REFERENCE             ,
  API_NYI
};

/* Set defines for Win32 platform */

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
  #include <tchar.h>
  #include <windows.h>
#else
  #ifdef	UNICODE
    #include <wchar.h>
	#ifndef _TCHAR_DEFINED
		typedef wchar_t _TCHAR;
		#define _TCHAR_DEFINED
	#endif

    #ifndef _T
	  #define _R(x) L ## x
      #define _T(x) _R(x)
    #endif
    #else // #ifdef UNICODE
	#ifndef _TCHAR_DEFINED
		typedef char _TCHAR;
		#define _TCHAR_DEFINED
	#endif
      #ifndef _T
	#define _T(x) x
      #endif
  #endif

typedef void *HWND;

#ifndef CALLBACK
    // MSVC: CALLBACK is a calling convention for callback functions
    #define CALLBACK 
#endif

#endif

#ifndef __DLLEXPORT__
#ifdef _MSC_VER
  #define __DLLEXPORT__ __declspec(dllexport)
  #define __DLLIMPORT__ __declspec(dllimport)
  #define __DLLLOCAL__
#else
  #ifdef GCC_HASCLASSVISIBILITY
    #define __DLLEXPORT__ __attribute__ ((visibility("default")))
    #define __DLLIMPORT__ __attribute__ ((visibility("default")))
    #define __DLLLOCAL__ __attribute__ ((visibility("hidden")))
  #else
    #define __DLLEXPORT__
    #define __DLLIMPORT__
    #define __DLLLOCAL__
  #endif
  #ifndef WINAPI
    #define WINAPI
  #endif
#endif
#endif

#ifndef DLL_EXPORT
#define DLL_EXPORT(type) __DLLEXPORT__ type WINAPI

#ifdef __cplusplus
#define DLL_EXPORT_PROTO(type) extern _ASCII("C") __DLLEXPORT__ type WINAPI
#else
#define DLL_EXPORT_PROTO(type) extern __DLLEXPORT__ type WINAPI
#endif
#endif

#ifdef __IS_AIMMS__
#define AIMMSAPI(type) __DLLEXPORT__ type WINAPI
#else
#define AIMMSAPI(type) __DLLIMPORT__ type WINAPI
#endif


#ifdef __cplusplus
extern _ASCII("C") {
#endif


typedef struct tagAimmsStringW {
  int       Length;
  wchar_t  *String;
} AimmsStringW ;


typedef struct tagAimmsStringA {
  int       Length;
  char     *String;
} AimmsStringA ;

typedef union tagAimmsValueW {
  double        Double;
  int           Int;
  struct {
    int         Length;
    wchar_t    *String;
  };
} AimmsValueW ;
typedef union tagAimmsValueA {
  double        Double;
  int           Int;
  struct {
    int         Length;
    char       *String;
  };
} AimmsValueA;



AIMMSAPI(int) AimmsAttributeNameW( int handle, AimmsStringW *name );
AIMMSAPI(int) AimmsAttributeNameA( int handle, AimmsStringA *name );

AIMMSAPI(int) AimmsAttributeType( int handle, int *type );

AIMMSAPI(int) AimmsAttributeStorage( int handle, int *storage );

AIMMSAPI(int) AimmsAttributeDefaultW( int handle, AimmsValueW *value );
AIMMSAPI(int) AimmsAttributeDefaultA( int handle, AimmsValueA *value );

AIMMSAPI(int) AimmsAttributeDimension( int handle, int *full, int *sliced );

AIMMSAPI(int) AimmsAttributeRootDomain( int handle, int *domain );

AIMMSAPI(int) AimmsAttributeDeclarationDomain( int handle, int *domain );

AIMMSAPI(int) AimmsAttributeCallDomain( int handle, int *domain );

AIMMSAPI(int) AimmsAttributeRestriction( int handle, int *domainhandle );

AIMMSAPI(int) AimmsAttributeSlicing( int handle, int *slicing );

AIMMSAPI(int) AimmsAttributePermutation( int handle, int *permutation );

AIMMSAPI(int) AimmsAttributeFlagsSet( int handle, int flags );

AIMMSAPI(int) AimmsAttributeFlagsGet( int handle, int *flags );

AIMMSAPI(int) AimmsAttributeFlags( int handle, int *flags );

AIMMSAPI(int) AimmsAttributeElementRange( int handle, int *sethandle );

AIMMSAPI(int) AimmsAttributeCompoundDimension( int handle, int *dim );

AIMMSAPI(int) AimmsAttributeCompoundDomain( int handle, int *domain );

AIMMSAPI(int) AimmsAttributeSetUnitW( int handle, wchar_t *unit, wchar_t *convention );
AIMMSAPI(int) AimmsAttributeSetUnitA( int handle, char *unit, char *convention );

AIMMSAPI(int) AimmsAttributeGetUnitW( int handle, AimmsStringW *unitName );
AIMMSAPI(int) AimmsAttributeGetUnitA( int handle, AimmsStringA *unitName );

AIMMSAPI(int) AimmsIdentifierHandleCreateW( wchar_t *name, int *domain, int *slicing,
                                            int flags, int *handle );
AIMMSAPI(int) AimmsIdentifierHandleCreateA( char *name, int *domain, int *slicing,
                                            int flags, int *handle );

AIMMSAPI(int) AimmsIdentifierHandleCreatePermutedW( wchar_t *name, int *domain, int *slicing,
                                                   int *permutation, int flags, int *handle );
AIMMSAPI(int) AimmsIdentifierHandleCreatePermutedA( char *name, int *domain, int *slicing,
                                                   int *permutation, int flags, int *handle );

AIMMSAPI(int) AimmsIdentifierHandleDelete( int handle );

AIMMSAPI(int) AimmsIdentifierEmpty( int handle );

AIMMSAPI(int) AimmsIdentifierCleanup( int handle );

AIMMSAPI(int) AimmsIdentifierUpdate( int handle );

AIMMSAPI(int) AimmsIdentifierDataVersion( int handle, int *version );

// Functions AimmsIdentifierSetUnit and AimmsIdentifierGetUnit are deprecated.
// Please use AimmsAttributeSetUnit and AimmsAttributeGetUnit instead.
AIMMSAPI(int) AimmsIdentifierSetUnitW( int handle, wchar_t *unit, wchar_t *convention );
AIMMSAPI(int) AimmsIdentifierSetUnitA( int handle, char *unit, char *convention );

AIMMSAPI(int) AimmsIdentifierGetUnitW( int handle, AimmsStringW *unitName );
AIMMSAPI(int) AimmsIdentifierGetUnitA( int handle, AimmsStringA *unitName );


AIMMSAPI(int) AimmsValueCard( int handle, int *card );
AIMMSAPI(int) AimmsValueResetHandle( int handle );


AIMMSAPI(int) AimmsValueSearchW( int handle, int *tuple, AimmsValueW *value );
AIMMSAPI(int) AimmsValueSearchA( int handle, int *tuple, AimmsValueA *value );

AIMMSAPI(int) AimmsValueNextW( int handle, int *tuple, AimmsValueW *value );
AIMMSAPI(int) AimmsValueNextA( int handle, int *tuple, AimmsValueA *value );

AIMMSAPI(int) AimmsValueNextMultiW( int handle, int *n, int *tuple, AimmsValueW *value );
AIMMSAPI(int) AimmsValueNextMultiA( int handle, int *n, int *tuple, AimmsValueA *value );

AIMMSAPI(int) AimmsValueRetrieveW( int handle, int *tuple, AimmsValueW *value );
AIMMSAPI(int) AimmsValueRetrieveA( int handle, int *tuple, AimmsValueA *value );

AIMMSAPI(int) AimmsValueAssignW( int handle, int *tuple, AimmsValueW *value );
AIMMSAPI(int) AimmsValueAssignA( int handle, int *tuple, AimmsValueA *value );

AIMMSAPI(int) AimmsValueAssignMultiW( int handle, int n, int *tuple, AimmsValueW *value );
AIMMSAPI(int) AimmsValueAssignMultiA( int handle, int n, int *tuple, AimmsValueA *value );

AIMMSAPI(int) AimmsValueDoubleToMapval( double value, int *mapval );

AIMMSAPI(int) AimmsValueMapvalToDouble( int mapval, double *value );

AIMMSAPI(int) AimmsSetElementNumberW( int handle, wchar_t *name, int allowCreate,
                                     int *elementNumber, int *isCreated );
AIMMSAPI(int) AimmsSetElementNumberA( int handle, char *name, int allowCreate,
                                     int *elementNumber, int *isCreated );
                                     
AIMMSAPI(int) AimmsSetAddElementW( int handle, wchar_t *name, int *element );
AIMMSAPI(int) AimmsSetAddElementA( int handle, char *name, int *element );

AIMMSAPI(int) AimmsSetAddElementMulti( int handle, int n, int *elementNumbers );

AIMMSAPI(int) AimmsSetAddElementRecursiveW( int handle, wchar_t *name, int *element );
AIMMSAPI(int) AimmsSetAddElementRecursiveA( int handle, char *name, int *element );

AIMMSAPI(int) AimmsSetAddElementRecursiveMulti( int handle, int n, int *elementNumbers );

AIMMSAPI(int) AimmsSetDeleteElement( int handle, int element );

AIMMSAPI(int) AimmsSetDeleteElementMulti( int handle, int n, int *elementNumbers );

AIMMSAPI(int) AimmsSetRenameElementW( int handle, int element, wchar_t *name );
AIMMSAPI(int) AimmsSetRenameElementA( int handle, int element, char *name );

AIMMSAPI(int) AimmsSetElementToOrdinal( int handle, int element, int *ordinal );

AIMMSAPI(int) AimmsSetElementToNameW( int handle, int element, AimmsStringW *name );
AIMMSAPI(int) AimmsSetElementToNameA( int handle, int element, AimmsStringA *name );

AIMMSAPI(int) AimmsSetOrdinalToElement( int handle, int ordinal, int *element );

AIMMSAPI(int) AimmsSetOrdinalToNameW( int handle, int ordinal, AimmsStringW *name );
AIMMSAPI(int) AimmsSetOrdinalToNameA( int handle, int ordinal, AimmsStringA *name );

AIMMSAPI(int) AimmsSetNameToElementW( int handle, wchar_t *name, int *element );
AIMMSAPI(int) AimmsSetNameToElementA( int handle, char *name, int *element );

AIMMSAPI(int) AimmsSetNameToOrdinalW( int handle, wchar_t *name, int *ordinal );
AIMMSAPI(int) AimmsSetNameToOrdinalA( int handle, char *name, int *ordinal );

AIMMSAPI(int) AimmsSetCompoundToTuple( int handle, int compound, int *tuple );

AIMMSAPI(int) AimmsSetTupleToCompound( int handle, int *tuple, int *compound );

AIMMSAPI(int) AimmsSetAddTupleToCompound( int handle, int *tuple, int *compound );

AIMMSAPI(int) AimmsSetAddTupleToCompoundRecursive( int handle, int *tuple, int *compound );

AIMMSAPI(int) AimmsProcedureHandleCreateW( wchar_t *procedure, int *handle, int *nargs, int *argtype );
AIMMSAPI(int) AimmsProcedureHandleCreateA( char *procedure, int *handle, int *nargs, int *argtype );

AIMMSAPI(int) AimmsProcedureHandleDelete( int handle );

AIMMSAPI(int) AimmsProcedureArgumentHandleCreate( int prochandle, int argnumber, int *arghandle );

AIMMSAPI(int) AimmsProcedureRunW( int handle, int *argtype, AimmsValueW *arglist, int *result );
AIMMSAPI(int) AimmsProcedureRunA( int handle, int *argtype, AimmsValueA *arglist, int *result );

AIMMSAPI(int) AimmsProcedureAsyncRunCreateW( int handle, int *argtype, AimmsValueW *arglist, int *request );
AIMMSAPI(int) AimmsProcedureAsyncRunCreateA( int handle, int *argtype, AimmsValueA *arglist, int *request );

AIMMSAPI(int) AimmsProcedureAsyncRunDelete( int request );

AIMMSAPI(int) AimmsProcedureAsyncRunStatus( int request, int *status, int *result );

AIMMSAPI(int) AimmsAPIPassMessageW( int severity, wchar_t *message );
AIMMSAPI(int) AimmsAPIPassMessageA( int severity, char *message );

AIMMSAPI(int) AimmsAPIStatus( int *status );

AIMMSAPI(int) AimmsAPILastErrorW( int *code, wchar_t *message );
AIMMSAPI(int) AimmsAPILastErrorA( int *code, char *message );

AIMMSAPI(int) AimmsProjectOpenW( wchar_t *commandline, int *handle );
AIMMSAPI(int) AimmsProjectOpenA( char *commandline, int *handle );

AIMMSAPI(int) AimmsServerProjectOpenW( wchar_t *commandline, int *handle );
AIMMSAPI(int) AimmsServerProjectOpenA( char *commandline, int *handle );

AIMMSAPI(int) AimmsProjectClose( int handle, int interactive );

AIMMSAPI(int) AimmsProjectSwitchW( wchar_t *prjName, int interactive );
AIMMSAPI(int) AimmsProjectSwitchA( char *prjName, int interactive );

AIMMSAPI(int) AimmsProjectWindow( HWND *windo );

AIMMSAPI(int) AimmsProjectInterface( void **ifc );

AIMMSAPI(int) AimmsControlGet( int timeout );

AIMMSAPI(int) AimmsControlRelease( void );

typedef int (CALLBACK* AimmsInterruptCallback)( int );
AIMMSAPI(int) AimmsInterruptCallbackInstall( AimmsInterruptCallback cb );

AIMMSAPI(int) AimmsInterruptPending( void );

AIMMSAPI(int) AimmsThreadAttach( void );

AIMMSAPI(int) AimmsThreadDetach( void );

///////////////////////////////////////////////////////////////////////////////////////

// Error handling support, see section: of AIMMS the language reference.
AIMMSAPI(int) AimmsErrorStatus(void);

AIMMSAPI(int) AimmsErrorCount(void);

AIMMSAPI(const wchar_t *) AimmsErrorMessageW(int errNo);
AIMMSAPI(const char *) AimmsErrorMessageA(int errNo);

AIMMSAPI(int) AimmsErrorSeverity(int errNo);

AIMMSAPI(const wchar_t *) AimmsErrorCodeW(int errNo);
AIMMSAPI(const char *) AimmsErrorCodeA(int errNo);

AIMMSAPI(const wchar_t *) AimmsErrorCategoryW(int errNo);
AIMMSAPI(const char *) AimmsErrorCategoryA(int errNo);

AIMMSAPI(int) AimmsErrorNumberOfLocations(int errNo);

AIMMSAPI(const wchar_t *) AimmsErrorFilenameW(int errNo);
AIMMSAPI(const char *) AimmsErrorFilenameA(int errNo);

AIMMSAPI(const wchar_t *) AimmsErrorNodeW(int errNo, int pos);
AIMMSAPI(const char *) AimmsErrorNodeA(int errNo, int pos);

AIMMSAPI(const wchar_t *) AimmsErrorAttributeNameW(int errNo, int pos);
AIMMSAPI(const char *) AimmsErrorAttributeNameA(int errNo, int pos);

AIMMSAPI(int) AimmsErrorLine(int errNo, int pos);

AIMMSAPI(int) AimmsErrorColumn(int errNo);

#include <time.h>
AIMMSAPI(time_t) AimmsErrorCreationTime(int errNo);

AIMMSAPI(int) AimmsErrorDelete(int errNo);

AIMMSAPI(int) AimmsErrorClear(void);

AIMMSAPI(int) AimmsErrorRaiseW(int severity, const wchar_t *message, const wchar_t *code);
AIMMSAPI(int) AimmsErrorRaiseA(int severity, const char *message, const char *code);

///////////////////////////////////////////////////////////////////////////////////////

// Model Editing Support, see section 31.12 "Model Editing" of 
// Chapter 31: The AIMMS Programming Interface" of AIMMS Language reference.

// Unless specified otherwise each of the functions belo returns either
// AIMMSAPI_SUCCESS or AIMMSAPI_FAILURE indicating success and failure respectively.

// The following handle number is returned when a handle could not be 
// created or opened. In addition, it can be used to indicate the global 
// namespace.
static const int AIMMSAPI_ME_NULL_HANDLE_NUMBER = 0 ;


// The node types are identified by the following constants.
static const int AIMMSAPI_ME_IDTYPE_NONE                   =   0 ;

static const int AIMMSAPI_ME_IDTYPE_SET                    =   1 ;
static const int AIMMSAPI_ME_IDTYPE_CALENDAR               =   2 ;
static const int AIMMSAPI_ME_IDTYPE_HORIZON                =   3 ;
static const int AIMMSAPI_ME_IDTYPE_INDEX                  =   4 ;

static const int AIMMSAPI_ME_IDTYPE_PARAMETER_NUMERIC      =  10 ;
static const int AIMMSAPI_ME_IDTYPE_PARAMETER_ELEMENT      =  11 ;
static const int AIMMSAPI_ME_IDTYPE_PARAMETER_STRING       =  12 ;
static const int AIMMSAPI_ME_IDTYPE_PARAMETER_UNIT         =  13 ;

static const int AIMMSAPI_ME_IDTYPE_VARIABLE_NUMERIC       =  20 ;
static const int AIMMSAPI_ME_IDTYPE_VARIABLE_COMPLMENTARY  =  21 ;
static const int AIMMSAPI_ME_IDTYPE_ARC                    =  22 ;
static const int AIMMSAPI_ME_IDTYPE_UNCERTAINTY_VARIABLE   =  23 ;
static const int AIMMSAPI_ME_IDTYPE_ELEMENT_VARIABLE       =  24 ;
static const int AIMMSAPI_ME_IDTYPE_ACTIVITY               =  25 ;

static const int AIMMSAPI_ME_IDTYPE_CONSTRAINT             =  30 ;
static const int AIMMSAPI_ME_IDTYPE_NODE                   =  31 ;
static const int AIMMSAPI_ME_IDTYPE_UNCERTAINTY_CONSTRAINT =  32 ;
static const int AIMMSAPI_ME_IDTYPE_RESOURCE               =  33 ;

static const int AIMMSAPI_ME_IDTYPE_MATHEMATICAL_PROGRAM   =  40 ;
static const int AIMMSAPI_ME_IDTYPE_QUANTITY               =  41 ;
static const int AIMMSAPI_ME_IDTYPE_CONVENTION             =  42 ;

static const int AIMMSAPI_ME_IDTYPE_ASSERTION              =  50 ;
static const int AIMMSAPI_ME_IDTYPE_MACRO                  =  51 ;

static const int AIMMSAPI_ME_IDTYPE_FILE                   =  60 ;
static const int AIMMSAPI_ME_IDTYPE_DATABASE               =  61 ;

static const int AIMMSAPI_ME_IDTYPE_HANDLE                 =  70 ;
static const int AIMMSAPI_ME_IDTYPE_TAG                    =  71 ;

static const int AIMMSAPI_ME_IDTYPE_PROCEDURE              = 100 ;
static const int AIMMSAPI_ME_IDTYPE_FUNCTION               = 101 ;
static const int AIMMSAPI_ME_IDTYPE_EXTERNAL_PROCEDURE     = 102 ;
static const int AIMMSAPI_ME_IDTYPE_EXTERNAL_FUNCTION      = 103 ;
static const int AIMMSAPI_ME_IDTYPE_INTRINSIC_PROCEDURE    = 104 ;
static const int AIMMSAPI_ME_IDTYPE_INTRINSIC_FUNCTION     = 105 ;
static const int AIMMSAPI_ME_IDTYPE_DATABASE_PROCEDURE     = 106 ;

static const int AIMMSAPI_ME_IDTYPE_DECLARATION_SECTION    = 110 ;
static const int AIMMSAPI_ME_IDTYPE_SECTION                = 111 ;
static const int AIMMSAPI_ME_IDTYPE_MODULE                 = 112 ;
static const int AIMMSAPI_ME_IDTYPE_LIBRARY                = 113 ;
static const int AIMMSAPI_ME_IDTYPE_PREFIX                 = 114 ;

// The attributes of identifiers are identified by the following constants.
static const int AIMMSAPI_ME_ATTR_NONE                     =   0 ;
static const int AIMMSAPI_ME_ATTR_ACTIVATING_CONDITION     =   1 ;
static const int AIMMSAPI_ME_ATTR_ACTUAL_NAME              =   2 ;
static const int AIMMSAPI_ME_ATTR_ARGUMENTS                =   3 ;
static const int AIMMSAPI_ME_ATTR_ASSERT_LIMIT             =   4 ;
static const int AIMMSAPI_ME_ATTR_BASE_UNIT                =   5 ;
static const int AIMMSAPI_ME_ATTR_BEGIN_DATE               =   6 ;
static const int AIMMSAPI_ME_ATTR_BODY                     =   7 ;
static const int AIMMSAPI_ME_ATTR_BODY_CALL                =   8 ;
static const int AIMMSAPI_ME_ATTR_CASE                     =   9 ;
static const int AIMMSAPI_ME_ATTR_CHECKING                 =  10 ;
static const int AIMMSAPI_ME_ATTR_COMMENT                  =  11 ;
static const int AIMMSAPI_ME_ATTR_COMPATIBLE_WITH          =  12 ;
static const int AIMMSAPI_ME_ATTR_COMPLEMENT               =  13 ;
static const int AIMMSAPI_ME_ATTR_CONDITION                =  14 ;
static const int AIMMSAPI_ME_ATTR_CONSTRAINTS              =  15 ;
static const int AIMMSAPI_ME_ATTR_CONVENTION               =  16 ;
static const int AIMMSAPI_ME_ATTR_CONVERSIONS              =  17 ;
static const int AIMMSAPI_ME_ATTR_COST                     =  18 ;
static const int AIMMSAPI_ME_ATTR_CURRENT_PERIOD           =  19 ;
static const int AIMMSAPI_ME_ATTR_DATA_SOURCE              =  20 ;
static const int AIMMSAPI_ME_ATTR_DEFAULT                  =  21 ;
static const int AIMMSAPI_ME_ATTR_DEFINITION               =  22 ;
static const int AIMMSAPI_ME_ATTR_DERIVATIVE_CALL          =  23 ;
static const int AIMMSAPI_ME_ATTR_DEVICE                   =  24 ;
static const int AIMMSAPI_ME_ATTR_DIRECTION                =  25 ;
static const int AIMMSAPI_ME_ATTR_DLL_NAME                 =  26 ;
static const int AIMMSAPI_ME_ATTR_DOMAIN                   =  27 ;
static const int AIMMSAPI_ME_ATTR_END_DATE                 =  28 ;
static const int AIMMSAPI_ME_ATTR_FROM                     =  29 ;
static const int AIMMSAPI_ME_ATTR_FROM_MULTIPLIER          =  30 ;
static const int AIMMSAPI_ME_ATTR_IDENTIFIERS              =  31 ;
static const int AIMMSAPI_ME_ATTR_INDEX                    =  32 ;
static const int AIMMSAPI_ME_ATTR_INDEX_DOMAIN             =  33 ;
static const int AIMMSAPI_ME_ATTR_INHERIT_FROM             =  34 ;
static const int AIMMSAPI_ME_ATTR_INITIAL_DATA             =  35 ;
static const int AIMMSAPI_ME_ATTR_INTERFACE                =  36 ;
static const int AIMMSAPI_ME_ATTR_INTERVAL_LENGTH          =  37 ;
static const int AIMMSAPI_ME_ATTR_LICENSE_FILE             =  38 ;
static const int AIMMSAPI_ME_ATTR_MAPPING                  =  39 ;
static const int AIMMSAPI_ME_ATTR_MARGINAL                 =  40 ;
static const int AIMMSAPI_ME_ATTR_MATHEMATICAL_PROGRAM     =  41 ;
static const int AIMMSAPI_ME_ATTR_MODE                     =  42 ;
static const int AIMMSAPI_ME_ATTR_NAME                     =  43 ;
static const int AIMMSAPI_ME_ATTR_NONVAR_STATUS            =  44 ;
static const int AIMMSAPI_ME_ATTR_OBJECTIVE                =  45 ;
static const int AIMMSAPI_ME_ATTR_OPTIONS                  =  46 ;
static const int AIMMSAPI_ME_ATTR_ORDER_BY                 =  47 ;
static const int AIMMSAPI_ME_ATTR_OWNER                    =  48 ;
static const int AIMMSAPI_ME_ATTR_PARAMETER                =  49 ;
static const int AIMMSAPI_ME_ATTR_PER_IDENTIFIER           =  50 ;
static const int AIMMSAPI_ME_ATTR_PER_QUANTITY             =  51 ;
static const int AIMMSAPI_ME_ATTR_PER_UNIT                 =  52;
static const int AIMMSAPI_ME_ATTR_PERIOD_FORMAT            =  53 ;
static const int AIMMSAPI_ME_ATTR_PREFIX                   =  54 ;
static const int AIMMSAPI_ME_ATTR_PRIORITY                 =  55 ;
static const int AIMMSAPI_ME_ATTR_PROPERTY                 =  56 ;
static const int AIMMSAPI_ME_ATTR_PROTECTED                =  57 ;
static const int AIMMSAPI_ME_ATTR_PUBLIC                   =  58 ;
static const int AIMMSAPI_ME_ATTR_QUANTITY                 =  59 ;
static const int AIMMSAPI_ME_ATTR_RANGE                    =  60 ;
static const int AIMMSAPI_ME_ATTR_RELAX_STATUS             =  61 ;
static const int AIMMSAPI_ME_ATTR_RESTRICT_TO              =  62 ;
static const int AIMMSAPI_ME_ATTR_RETURN_TYPE              =  63 ;
static const int AIMMSAPI_ME_ATTR_SELECTOR                 =  64 ;
static const int AIMMSAPI_ME_ATTR_SOLVER                   =  65 ;
static const int AIMMSAPI_ME_ATTR_SOS_PRIORITY             =  66 ;
static const int AIMMSAPI_ME_ATTR_SOS_WEIGHT               =  67 ;
static const int AIMMSAPI_ME_ATTR_SQL_QUERY                =  68 ;
static const int AIMMSAPI_ME_ATTR_STAGE                    =  69 ;
static const int AIMMSAPI_ME_ATTR_STORED_PROCEDURE         =  70 ;
static const int AIMMSAPI_ME_ATTR_SUBSET_OF                =  71 ;
static const int AIMMSAPI_ME_ATTR_TABLE_NAME               =  72 ;
static const int AIMMSAPI_ME_ATTR_TAGS                     =  73 ;
static const int AIMMSAPI_ME_ATTR_TEXT                     =  74 ;
static const int AIMMSAPI_ME_ATTR_TIMESLOT_FORMAT          =  75 ;
static const int AIMMSAPI_ME_ATTR_TO                       =  76 ;
static const int AIMMSAPI_ME_ATTR_TO_MULTIPLIER            =  77 ;
static const int AIMMSAPI_ME_ATTR_TYPE                     =  78 ;
static const int AIMMSAPI_ME_ATTR_UNIT                     =  79 ;
static const int AIMMSAPI_ME_ATTR_VARIABLES                =  80 ;
static const int AIMMSAPI_ME_ATTR_VIOLATION_PENALTY        =  81 ;
static const int AIMMSAPI_ME_ATTR_WHEN                     =  82 ;

static const int AIMMSAPI_ME_ATTR_APPROXIMATION            =  83 ;
static const int AIMMSAPI_ME_ATTR_DEPENDENCY               =  84 ;
static const int AIMMSAPI_ME_ATTR_DISTRIBUTION             =  85 ;
static const int AIMMSAPI_ME_ATTR_PROBABILITY              =  86 ;
static const int AIMMSAPI_ME_ATTR_REGION                   =  87 ;
static const int AIMMSAPI_ME_ATTR_UNCERTAINTY              =  88 ;

static const int AIMMSAPI_ME_ATTR_ABSENT_BEGIN             =  89 ;
static const int AIMMSAPI_ME_ATTR_ABSENT_END               =  90 ;
static const int AIMMSAPI_ME_ATTR_ABSENT_LENGTH            =  91 ;
static const int AIMMSAPI_ME_ATTR_ABSENT_SIZE              =  92 ;
static const int AIMMSAPI_ME_ATTR_ACTIVITIES               =  93 ;
static const int AIMMSAPI_ME_ATTR_BEGIN_CHANGE             =  94 ;
static const int AIMMSAPI_ME_ATTR_COMES_BEFORE             =  95 ;
static const int AIMMSAPI_ME_ATTR_END_CHANGE               =  96 ;
static const int AIMMSAPI_ME_ATTR_FIRST_ACTIVITY           =  97 ;
static const int AIMMSAPI_ME_ATTR_GROUP_DEFINITION         =  98 ;
static const int AIMMSAPI_ME_ATTR_GROUP_SET                =  99 ;
static const int AIMMSAPI_ME_ATTR_GROUP_TRANSITION         = 100 ;
static const int AIMMSAPI_ME_ATTR_INITIAL_LEVEL            = 101 ;
static const int AIMMSAPI_ME_ATTR_LAST_ACTIVITY            = 102 ;
static const int AIMMSAPI_ME_ATTR_LENGTH                   = 103 ;
static const int AIMMSAPI_ME_ATTR_LEVEL_CHANGE             = 104 ;
static const int AIMMSAPI_ME_ATTR_LEVEL_RANGE              = 105 ;
static const int AIMMSAPI_ME_ATTR_PRECEDES                 = 106 ;
static const int AIMMSAPI_ME_ATTR_SCHEDULE_DOMAIN          = 107 ;
static const int AIMMSAPI_ME_ATTR_SIZE                     = 108 ;
static const int AIMMSAPI_ME_ATTR_TRANSITION               = 109 ;
static const int AIMMSAPI_ME_ATTR_USAGE                    = 110 ;

static const int AIMMSAPI_ME_ATTR_ENCODING                 = 111 ;

// The number of roots available for model editing is stored by the function
// AimmsMeRootCount in its output argument "count", including the main model, 
// libraries and runtime libraries.
// Note
// - count is always at least one as there is always the main model.
// - The root "Predeclared identifiers" is not included in this count.
AIMMSAPI(int) AimmsMeRootCount(int *count);

// The function AimmsMeOpenRoot opens a model editor root and stores the model editor handle in MEH. 
// The argument pos should be in the range 0 .. count (where count is obtained via AimmsMeRootCount).
// If pos == 0 the main model is opened.
// If pos == 1 .. count-1 a library or runtime library is opened.
// If pos == count the root "Predeclared Identifiers" is opened.
AIMMSAPI(int) AimmsMeOpenRoot(int pos, int *MEH);

// The function AimmsMeCreateRuntimeLibrary creates a ne runtime library 
// and stores the resulting model editor handle in MEH.
// The arguments "name" and "prefix" should be valid AIMMS identifier names.
// The ne runtime library will be placed at the end of the ro of libraries.
AIMMSAPI(int) AimmsMeCreateRuntimeLibraryW(const wchar_t *name, const wchar_t *prefix, int *MEH);
AIMMSAPI(int) AimmsMeCreateRuntimeLibraryA(const char *name, const char *prefix, int *MEH);

// The function AimmsMeOpenNode opens a node with name "name" in the namespace determined by 
// model editor handle nMEH and stores the resulting handle in the output argument MEH.
// This function will fail if name is not a valid identifier name, or if nMEH is not an existing
// AIMMS API model editor handle and unequal to AIMMSAPI_ME_NULL_HANDLE_NUMBER.
// If name is valid and nMEH is a valid handle or equal to AIMMSAPI_ME_NULL_HANDLE_NUMBER, but name 
// is not found in that scope, AimmsMeOpenNode will return AIMMSAPI_FAILURE with MEH set to 
// AIMMSAPI_ME_NULL_HANDLE_NUMBER.
AIMMSAPI(int) AimmsMeOpenNodeW(const wchar_t *name, int nMEH, int *MEH);
AIMMSAPI(int) AimmsMeOpenNodeA(const char *name, int nMEH, int *MEH);

// The function AimmsMeNodeExists checks whether name is a declared identifier in scope 
// MEH exists.
// It will check whether nMEH is a valid namespace and return AIMMSAPI_FAILURE when this
// is not a valid namespace.
// It will also check whether the name is a valid identifier name and return 
// AIMMSAPI_FAILURE it is not.
// When this function return AIMMSAPI_SUCCESS exists is 
// 1 when the name is a declared identifier in nMEH
// 0 when the name is not a declared identifier in nMEH
AIMMSAPI(int) AimmsMeNodeExistsW(const wchar_t *name, int nMEH, int *exists);
AIMMSAPI(int) AimmsMeNodeExistsA(const char *name, int nMEH, int *exists);

// The function AimmsMeCreateNode creates a node of type "idtype" belo node pMEH with position pos.
// The resulting model editor handle is stored in MEH. This handle is also opened.
// pMEH is not allowed to be the AIMMSAPI_ME_NULL_HANDLE_NUMBER
// pMEH is not allowed to be the AIMMSAPI_ME_NULL_HANDLE_NUMBER
AIMMSAPI(int) AimmsMeCreateNodeW(const wchar_t *name, int idtype, int pMEH, int pos, int *MEH);
AIMMSAPI(int) AimmsMeCreateNodeA(const char *name, int idtype, int pMEH, int pos, int *MEH);

// The function AimmsMeCLoseNode closes the model editor handle but leaves the node 
// identified by MEH as is.
AIMMSAPI(int) AimmsMeCloseNode(int MEH);

// The function AimmsMeDestroyNode deletes the node MEH and all it subnodes.
// The node MEH must be open, and will be closed.
AIMMSAPI(int) AimmsMeDestroyNode(int MEH);

// The function AimmsMeName returns the name (without any prefixes) of the node 
// identified by the model editor handle MEH into the output argument name.
AIMMSAPI(int) AimmsMeNameW(int MEH, AimmsStringW *name);
AIMMSAPI(int) AimmsMeNameA(int MEH, AimmsStringA *name);

// The function AimmsMeRelativeName returns the name of the node identified by MEH 
// as it should be referenced from within the node rMEH into the output argument rName.
// When rMEH is AIMMSAPI_ME_NULL_HANDLE_NUMBER, then the fully qualified name is
// returned in rName.
AIMMSAPI(int) AimmsMeRelativeNameW(int MEH, int rMEH, AimmsStringW *rName);
AIMMSAPI(int) AimmsMeRelativeNameA(int MEH, int rMEH, AimmsStringA *rName);

// The function AimmsMeType returns the type of node MEH into its output argument meType.
AIMMSAPI(int) AimmsMeType(int MEH, int *meType);

// The function AimmsMeTypeName returns the name of the type typeNo into its output argument tName.
// Note that tName is an element of the predeclared set AllIdentifierTypes.
AIMMSAPI(int) AimmsMeTypeNameW(int typeNo, AimmsStringW *tName);
AIMMSAPI(int) AimmsMeTypeNameA(int typeNo, AimmsStringA *tName);

// The function AimmsMeAllowedChildTypes returns the types of the allowed children of node
// MEH into the output buffer typeBuf respecting the size typeBufsize.
// The number of types is stored into maxTypes which may exceed typeBufsize.
AIMMSAPI(int) AimmsMeAllowedChildTypes(int MEH, int *typeBuf, int typeBufsize, int *maxTypes);

// The function AimmsMeGetAttribute returns the string of attribute attr of model
// editor handle MEH into the output argument text.
AIMMSAPI(int) AimmsMeGetAttributeW(int MEH, int attr, AimmsStringW *text);
AIMMSAPI(int) AimmsMeGetAttributeA(int MEH, int attr, AimmsStringA *text);

// The function AimmsMeSetAttribute(MEH, attr, txt) sets the attribute attr of model 
// editor handle MEH to text txt. It doesn't compile this attribute, but will fail 
// if attribute attr is not applicable to node MEH.
AIMMSAPI(int) AimmsMeSetAttributeW(int MEH, int attr, const wchar_t *txt);
AIMMSAPI(int) AimmsMeSetAttributeA(int MEH, int attr, const char *txt);

// The function AimmsMeAttributes(MEH, attrsBuf, attrBufSize, maxNoAttrs) returns the 
// attributes that are applicable to the node MEH into the output argument attrsBuf 
// respecting the size of this buffer attrBufSize.  The maximum number of attributes 
// that can be specified for node MEH is stored in maxNoAttrs.
AIMMSAPI(int) AimmsMeAttributes(int MEH, int attrsBuf[], int attrBufSize, int *maxNoAttrs);

// The utility function AimmsMeAttributeName(attr, name) returns the name of the 
// attribute into the output argument name.
AIMMSAPI(int) AimmsMeAttributeNameW(int attr, AimmsStringW *name);
AIMMSAPI(int) AimmsMeAttributeNameA(int attr, AimmsStringA *name);

// The function AimmsMeNodeRename(MEH, newName) changes the name of the node MEH to newName.
// In addition, the namechange will be applied to the attribute texts of affected other nodes.
// For the main model and the ordinary (static) libraries a name changes entry will be created.
// A name change entry will not be created in the runtime libraries.
AIMMSAPI(int) AimmsMeNodeRenameW(int MEH, const wchar_t *newName);
AIMMSAPI(int) AimmsMeNodeRenameA(int MEH, const char *newName);

// The function AimmsMeNodeMove(MEH, pMEH, pos) moves the node  MEH to child position pos of node pMEH. 
// If the result is a change of namespace, the corresponding namechange is applied to the attributes 
// that reference this node. In addition, an entry is appended to the corresponding name change file 
// if this node is not a runtime node. Moves from one library to another are not supported, nor is a 
// move into or out of the main model.
AIMMSAPI(int) AimmsMeNodeMove(int MEH, int pMEH, int pos);

// The function AimmsMeNodeChangeType(MEH, newType) changes the type of a node. 
// It attempts to retain available attributes as much as possible. 
AIMMSAPI(int) AimmsMeNodeChangeType(int MEH, int newType);

// The function AimmsMeNodeAllowedTypes can be used to query which types, if any, a particular 
// node can be changed into. The function AimmsMeNodeAllowedTypes(MEH, typeBuf, typeBufsize, 
// maxNoTypes) stores the types into which node MEH can be changed in buffer typeBuf
// respecting the size typeBufsize.  The maximum number of types into which MEH can be
// changed is stored in maxNoTypes.
AIMMSAPI(int) AimmsMeNodeAllowedTypes(int MEH, int* typeBuf, int typeBufsize, int *maxNoTypes);

// The function AimmsMeParent(MEH, pMEH) opens a model editor handle to the parent 
// of MEH and stores this handle in the output argument pMEH.
// In the case this parent node doesn't exist the AIMMSAPI_ME_NULL_HANDLE_NUMBER 
// handle is stored in the output argument but the function AimmsMeParent doesn't fail.
AIMMSAPI(int) AimmsMeParent(int MEH, int *pMEH);

// The function AimmsMeFirst(MEH, fMEH) opens a model editor handle to the first 
// child of MEH and stores this handle in the output argument fMEH.
// In the case this first node doesn't exist the AIMMSAPI_ME_NULL_HANDLE_NUMBER 
// handle is stored in the output argument but the function AimmsMeFirst doesn't fail.
AIMMSAPI(int) AimmsMeFirst(int MEH, int *fMEH);

// The function AimmsMeNext(MEH, nMEH) opens a model editor handle to the node next 
// to MEH and stores this handle in the output argument nMEH.
// In the case this next node doesn't exist the AIMMSAPI_ME_NULL_HANDLE_NUMBER 
// handle is stored in the output argument but the function AimmsMeNext doesn't fail.
AIMMSAPI(int) AimmsMeNext(int MEH, int *nMEH);

// The function AimmsMeImportNode(MEH, fn, pwd) reads a file fn and stores the 
// resulting model structure at the node {\tt MEH}
// When MEH doesn't refer to a section, module, library or model the function
// AimmsMeImportNode will fail.
// If the source at fn is encrypted, it will be checked against the pwd.
AIMMSAPI(int) AimmsMeImportNodeW(int MEH, const wchar_t *fn, const wchar_t *pwd);
AIMMSAPI(int) AimmsMeImportNodeA(int MEH, const char *fn, const char *pwd);

// The function AimmsMeExportNode(MEH, fn, pwd) writes the model structure 
// at node MEH to file fn.
// If fn ends in .amb then the .amb format is used, otherwise the .aim file format is used.
// The pwd argument is ignored when the .aim file format is used.
// If the pwd is empty or NULL, the .amb file will not be encrypted 
// and the password not stored in the .amb file.
AIMMSAPI(int) AimmsMeExportNodeW(int MEH, const wchar_t *fn, const wchar_t *pwd);
AIMMSAPI(int) AimmsMeExportNodeA(int MEH, const char *fn, const char *pwd);

// The central function AimmsMeCompile (MEH) compiles the node MEH and all its sub nodes. 
// When the argument MEH is AIMMSAPI_ME_NULL_HANDLE_NUMBER the entire application (main model 
// and libraries) are compiled and upon succes all procedures are runnable.
AIMMSAPI(int) AimmsMeCompile(int MEH);

// The function AimmsMeIsRunnable(MEH, r) stores 1 in the output argument r if the procedure 
// referenced by MEH is runnable.
AIMMSAPI(int) AimmsMeIsRunnable(int MEH, int *r);

// The function AimmsMeIsReadOnly(MEH, r) stores 1 in the output argument r if the node 
// resides in a read-only library such as the predeclared identifiers,
// or a library that was read from a read only file.
AIMMSAPI(int) AimmsMeIsReadOnly(int MEH, int *r);

///////////////////////////////////////////////////////////////////////////////////////

// Hard interrupt for execution.
// Returns AIMMSAPI_SUCCESS if Aimms was "idle"
// Returns AIMMSAPI_FAILURE if Aimms was actually in the middle of executing statements.
AIMMSAPI(int) AimmsExecutionInterrupt(void);


///////////////////////////////////////////////////////////////////////////////////////
// Interface functions for adding virtual file systems to the Data Management module
// The functions use C++ classes, so we must assume the c++ compiler!
#ifdef __cplusplus
namespace SFS {
	class IFileSystem;
};

// The function AddVirtualFileSystem adds a 'virtual file system' to the Data Management module.
// All folders and files in the added fileSystem should have a unique URL prefix, for example: 'myPrefix:\myFolder1\myFolder2\myFile.txt'
// If deleteOnExit is true, Aimms is allowed to call delete the passed fileSystem when no longer needed.
AIMMSAPI(bool) AddVirtualFileSystem(SFS::IFileSystem *fileSystem, bool deleteOnExit);

// The function RemoveVirtualFileSystem removes a previously added file system (based on pointer comparison)
AIMMSAPI(bool) RemoveVirtualFileSystem(SFS::IFileSystem *fileSystem);
#endif // __cplusplus




#ifdef __cplusplus
}
#endif

#ifndef _NO_AIMMSAPI_STRING_FUNCTION_DEFINES

#ifdef UNICODE

#define AimmsString                         AimmsStringW
#define AimmsValue                          AimmsValueW

#define AimmsAttributeName                  AimmsAttributeNameW
#define AimmsAttributeDefault               AimmsAttributeDefaultW
#define AimmsAttributeSetUnit               AimmsAttributeSetUnitW
#define AimmsAttributeGetUnit               AimmsAttributeGetUnitW
#define AimmsIdentifierHandleCreate         AimmsIdentifierHandleCreateW
#define AimmsIdentifierHandleCreatePermuted AimmsIdentifierHandleCreatePermutedW
#define AimmsIdentifierSetUnit              AimmsIdentifierSetUnitW
#define AimmsIdentifierGetUnit              AimmsIdentifierGetUnitW
#define AimmsValueSearch                    AimmsValueSearchW
#define AimmsValueNext                      AimmsValueNextW
#define AimmsValueNextMulti                 AimmsValueNextMultiW
#define AimmsValueRetrieve                  AimmsValueRetrieveW
#define AimmsValueAssign                    AimmsValueAssignW
#define AimmsValueAssignMulti               AimmsValueAssignMultiW
#define AimmsSetElementNumber               AimmsSetElementNumberW
#define AimmsSetAddElement                  AimmsSetAddElementW
#define AimmsSetAddElementRecursive         AimmsSetAddElementRecursiveW
#define AimmsSetRenameElement               AimmsSetRenameElementW
#define AimmsSetElementToName               AimmsSetElementToNameW
#define AimmsSetOrdinalToName               AimmsSetOrdinalToNameW
#define AimmsSetNameToElement               AimmsSetNameToElementW
#define AimmsSetNameToOrdinal               AimmsSetNameToOrdinalW
#define AimmsProcedureHandleCreate          AimmsProcedureHandleCreateW
#define AimmsProcedureRun                   AimmsProcedureRunW
#define AimmsProcedureAsyncRunCreate        AimmsProcedureAsyncRunCreateW
#define AimmsAPIPassMessage                 AimmsAPIPassMessageW
#define AimmsAPILastError                   AimmsAPILastErrorW
#define AimmsProjectOpen                    AimmsProjectOpenW
#define AimmsServerProjectOpen              AimmsServerProjectOpenW
#define AimmsProjectSwitch                  AimmsProjectSwitchW

#define AimmsErrorMessage                   AimmsErrorMessageW
#define AimmsErrorCode                      AimmsErrorCodeW
#define AimmsErrorCategory                  AimmsErrorCategoryW
#define AimmsErrorFilename                  AimmsErrorFilenameW
#define AimmsErrorNode                      AimmsErrorNodeW
#define AimmsErrorAttributeName             AimmsErrorAttributeNameW
#define AimmsErrorRaise                     AimmsErrorRaiseW

#define AimmsMeCreateRuntimeLibrary         AimmsMeCreateRuntimeLibraryW
#define AimmsMeOpenNode                     AimmsMeOpenNodeW
#define AimmsMeNodeExists                   AimmsMeNodeExistsW
#define AimmsMeCreateNode                   AimmsMeCreateNodeW
#define AimmsMeName                         AimmsMeNameW
#define AimmsMeRelativeName                 AimmsMeRelativeNameW
#define AimmsMeTypeName                     AimmsMeTypeNameW
#define AimmsMeGetAttribute                 AimmsMeGetAttributeW
#define AimmsMeSetAttribute                 AimmsMeSetAttributeW
#define AimmsMeAttributeName                AimmsMeAttributeNameW
#define AimmsMeNodeRename                   AimmsMeNodeRenameW
#define AimmsMeImportNode                   AimmsMeImportNodeW
#define AimmsMeExportNode                   AimmsMeExportNodeW

#else

// NOT UNICODE

#define AimmsString                         AimmsStringA
#define AimmsValue                          AimmsValueA

#define AimmsAttributeName                  AimmsAttributeNameA
#define AimmsAttributeDefault               AimmsAttributeDefaultA
#define AimmsAttributeSetUnit               AimmsAttributeSetUnitA
#define AimmsAttributeGetUnit               AimmsAttributeGetUnitA
#define AimmsIdentifierHandleCreate         AimmsIdentifierHandleCreateA
#define AimmsIdentifierHandleCreatePermuted AimmsIdentifierHandleCreatePermutedA
#define AimmsIdentifierSetUnit              AimmsIdentifierSetUnitA
#define AimmsIdentifierGetUnit              AimmsIdentifierGetUnitA
#define AimmsValueSearch                    AimmsValueSearchA
#define AimmsValueNext                      AimmsValueNextA
#define AimmsValueNextMulti                 AimmsValueNextMultiA
#define AimmsValueRetrieve                  AimmsValueRetrieveA
#define AimmsValueAssign                    AimmsValueAssignA
#define AimmsValueAssignMulti               AimmsValueAssignMultiA
#define AimmsSetElementNumber               AimmsSetElementNumberA
#define AimmsSetAddElement                  AimmsSetAddElementA
#define AimmsSetAddElementRecursive         AimmsSetAddElementRecursiveA
#define AimmsSetRenameElement               AimmsSetRenameElementA
#define AimmsSetElementToName               AimmsSetElementToNameA
#define AimmsSetOrdinalToName               AimmsSetOrdinalToNameA
#define AimmsSetNameToElement               AimmsSetNameToElementA
#define AimmsSetNameToOrdinal               AimmsSetNameToOrdinalA
#define AimmsProcedureHandleCreate          AimmsProcedureHandleCreateA
#define AimmsProcedureRun                   AimmsProcedureRunA
#define AimmsProcedureAsyncRunCreate        AimmsProcedureAsyncRunCreateA
#define AimmsAPIPassMessage                 AimmsAPIPassMessageA
#define AimmsAPILastError                   AimmsAPILastErrorA
#define AimmsProjectOpen                    AimmsProjectOpenA
#define AimmsServerProjectOpen              AimmsServerProjectOpenA
#define AimmsProjectSwitch                  AimmsProjectSwitchA

#define AimmsErrorMessage                   AimmsErrorMessageA
#define AimmsErrorCode                      AimmsErrorCodeA
#define AimmsErrorCategory                  AimmsErrorCategoryA
#define AimmsErrorFilename                  AimmsErrorFilenameA
#define AimmsErrorNode                      AimmsErrorNodeA
#define AimmsErrorAttributeName             AimmsErrorAttributeNameA
#define AimmsErrorRaise                     AimmsErrorRaiseA

#define AimmsMeCreateRuntimeLibrary         AimmsMeCreateRuntimeLibraryA
#define AimmsMeOpenNode                     AimmsMeOpenNodeA
#define AimmsMeNodeExists                   AimmsMeNodeExistsA
#define AimmsMeCreateNode                   AimmsMeCreateNodeA
#define AimmsMeName                         AimmsMeNameA
#define AimmsMeRelativeName                 AimmsMeRelativeNameA
#define AimmsMeTypeName                     AimmsMeTypeNameA
#define AimmsMeGetAttribute                 AimmsMeGetAttributeA
#define AimmsMeSetAttribute                 AimmsMeSetAttributeA
#define AimmsMeAttributeName                AimmsMeAttributeNameA
#define AimmsMeNodeRename                   AimmsMeNodeRenameA
#define AimmsMeImportNode                   AimmsMeImportNodeA
#define AimmsMeExportNode                   AimmsMeExportNodeA

#endif

#endif 

#endif
