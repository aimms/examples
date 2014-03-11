/* 
 *  AIMMS comes with an import library suitable for Visual Studio 6.0.
 *  If you are using a different C/C++ compiler, you can compile this 
 *  file to create a dummy libaimms.dll, and use the import library 
 *  created by your compiler to successfully execute the link step 
 *  against the actual libaimms.dll.
 */

#define __IS_AIMMS__
#include "aimmsapi.h"

AIMMSAPI(int) AimmsAttributeName( int handle, AimmsString *name ){return 0;}
AIMMSAPI(int) AimmsAttributeType( int handle, int *type ){return 0;}
AIMMSAPI(int) AimmsAttributeStorage( int handle, int *storage ){return 0;}
AIMMSAPI(int) AimmsAttributeDefault( int handle, AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsAttributeDimension( int handle, int *full, int *sliced ){return 0;}
AIMMSAPI(int) AimmsAttributeRootDomain( int handle, int *domain ){return 0;}
AIMMSAPI(int) AimmsAttributeDeclarationDomain( int handle, int *domain ){return 0;}
AIMMSAPI(int) AimmsAttributeCallDomain( int handle, int *domain ){return 0;}
AIMMSAPI(int) AimmsAttributeRestriction( int handle, int *domainhandle ){return 0;}
AIMMSAPI(int) AimmsAttributeSlicing( int handle, int *slicing ){return 0;}
AIMMSAPI(int) AimmsAttributeFlags( int handle, int *flags ){return 0;}
AIMMSAPI(int) AimmsAttributeElementRange( int handle, int *sethandle ){return 0;}
AIMMSAPI(int) AimmsAttributeCompoundDimension( int handle, int *dim ){return 0;}
AIMMSAPI(int) AimmsAttributeCompoundDomain( int handle, int *domain ){return 0;}

AIMMSAPI(int) AimmsIdentifierHandleCreate( char *name, int *domain, int *slicing,
                                           int flags, int *handle ){return 0;}
AIMMSAPI(int) AimmsIdentifierHandleDelete( int handle ){return 0;}
AIMMSAPI(int) AimmsIdentifierEmpty( int handle ){return 0;}
AIMMSAPI(int) AimmsIdentifierCleanup( int handle ){return 0;}
AIMMSAPI(int) AimmsIdentifierUpdate( int handle ){return 0;}
AIMMSAPI(int) AimmsIdentifierDataVersion( int handle, int *version ){return 0;}

AIMMSAPI(int) AimmsValueCard( int handle, int *card ){return 0;}
AIMMSAPI(int) AimmsValueResetHandle( int handle ){return 0;}
AIMMSAPI(int) AimmsValueSearch( int handle, int *tuple, AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsValueNext( int handle, int *tuple, AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsValueNextMulti( int handle, int *n, int *tuple,
                                   AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsValueRetrieve( int handle, int *tuple, AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsValueAssign( int handle, int *tuple, AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsValueAssignMulti( int handle, int n, int *tuple,
                                     AimmsValue *value ){return 0;}
AIMMSAPI(int) AimmsValueDoubleToMapval( double value, int *mapval ){return 0;}
AIMMSAPI(int) AimmsValueMapvalToDouble( int mapval, double *value ){return 0;}

AIMMSAPI(int) AimmsSetAddElement( int handle, char *name, int *element ){return 0;}
AIMMSAPI(int) AimmsSetAddElementRecursive( int handle, char *name, int *element ){return 0;}
AIMMSAPI(int) AimmsSetDeleteElement( int handle, int element ){return 0;}
AIMMSAPI(int) AimmsSetRenameElement( int handle, int element, char *name ){return 0;}
AIMMSAPI(int) AimmsSetElementToOrdinal( int handle, int element, int *ordinal ){return 0;}
AIMMSAPI(int) AimmsSetElementToName( int handle, int element, AimmsString *name ){return 0;}
AIMMSAPI(int) AimmsSetOrdinalToElement( int handle, int ordinal, int *element ){return 0;}
AIMMSAPI(int) AimmsSetOrdinalToName( int handle, int ordinal, AimmsString *name ){return 0;}
AIMMSAPI(int) AimmsSetNameToElement( int handle, char *name, int *element ){return 0;}
AIMMSAPI(int) AimmsSetNameToOrdinal( int handle, char *name, int *ordinal ){return 0;}
AIMMSAPI(int) AimmsSetCompoundToTuple( int handle, int compound, int *tuple ){return 0;}
AIMMSAPI(int) AimmsSetTupleToCompound( int handle, int *tuple, int *compound ){return 0;}
AIMMSAPI(int) AimmsSetAddTupleToCompound( int handle, int *tuple, int *compound ){return 0;}
AIMMSAPI(int) AimmsSetAddTupleToCompoundRecursive( int handle, int *tuple,
                                                   int *compound ){return 0;}

AIMMSAPI(int) AimmsProcedureHandleCreate( char *procedure, int *handle, int *nargs,
                                          int *argtype ){return 0;}
AIMMSAPI(int) AimmsProcedureHandleDelete( int handle ){return 0;}
AIMMSAPI(int) AimmsProcedureRun( int handle, int *argtype, AimmsValue *arglist,
                                 int *result ){return 0;}
AIMMSAPI(int) AimmsProcedureAsyncRunCreate( int handle, int *argtype,
                                            AimmsValue *arglist, int *request ){return 0;}
AIMMSAPI(int) AimmsProcedureAsyncRunDelete( int request ){return 0;}
AIMMSAPI(int) AimmsProcedureAsyncRunStatus( int request, int *status, int *result ){return 0;}

AIMMSAPI(int) AimmsAPIPassMessage( int severity, char *message ){return 0;}
AIMMSAPI(int) AimmsAPIStatus( int *status ){return 0;}
AIMMSAPI(int) AimmsAPILastError( int *code, char *message ){return 0;}

AIMMSAPI(int) AimmsProjectOpen( char *commandline, int *handle ){return 0;}
AIMMSAPI(int) AimmsProjectClose( int handle, int interactive ){return 0;}
AIMMSAPI(int) AimmsProjectWindow( HWND *window ){return 0;}

AIMMSAPI(int) AimmsControlGet( int timeout ){return 0;}
AIMMSAPI(int) AimmsControlRelease( void ){return 0;}

