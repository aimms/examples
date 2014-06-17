#include <stdio.h>
#include <string.h>
#include <aimmsapi.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tchar.h>

DLL_EXPORT(void) CSVExport(_TCHAR *name, _TCHAR *location)
{
   int  handle, full, sliced, domain[AIMMSAPI_MAX_DIMENSION],
        tuple[AIMMSAPI_MAX_DIMENSION], storage, i;
   _TCHAR file[256], buffer[256];
   FILE *f;
   struct _stat stat_buf;
   AimmsValue  value;
   AimmsString strvalue;

   if ( _tstat( location, &stat_buf ) == -1 ) {
		   AimmsAPIPassMessage(AIMMSAPI_SEVERITY_CODE_ERROR, _T("Invalid location"));
	   return;
   }

   if ( !(stat_buf.st_mode & _S_IFDIR) ) {
		   AimmsAPIPassMessage(AIMMSAPI_SEVERITY_CODE_ERROR, _T("Location is not a directory"));
	   return;
   }

   /* Create a handle associated with the identifier name passed */
   AimmsIdentifierHandleCreate(name, NULL, NULL, 0, &handle);
   
   /* Get the dimension, domain and storage type of the identifier
      associated with the handle */
   AimmsAttributeDimension (handle, &full, &sliced); 
   AimmsAttributeRootDomain(handle, domain); 
   AimmsAttributeStorage   (handle, &storage); 
   
   if ( storage == AIMMSAPI_STORAGE_STRING  || storage == AIMMSAPI_STORAGE_HANDLE ) {
		   AimmsAPIPassMessage(AIMMSAPI_SEVERITY_CODE_ERROR, _T("Only numerical parameters can be displayed"));
		   AimmsIdentifierHandleDelete(handle);
		   return;
   }
   
   /* Open a file consisting of the identifier name with the extension .csv,
      and print the identifier's name and dimension */
      
   _tcscpy(file, location); 
   _tcscat(file, _T("\\"));
   _tcscat(file, name);
   _tcscat(file, _T(".csv"));
   if ( ! (f = _tfopen(file, _T("w"))) ) {
		   AimmsAPIPassMessage(AIMMSAPI_SEVERITY_CODE_ERROR, _T("Unable to open file for writing."));
		   AimmsIdentifierHandleDelete(handle);
	   return;
   }

   /* Prepare strvalue to hold the locally declared buffer */
   strvalue.String = buffer;
   
   /* Print a header containing the names of the domain sets */
   for ( i = 0; i < full; i++ ) {
       strvalue.Length = 256;
       AimmsAttributeName(domain[i], &strvalue); _ftprintf(f, _T("%s,"), buffer);
   }
   _ftprintf(f, _T("%s\n"), name);

   /* Print all tuples with non-default data values */
   AimmsValueResetHandle(handle);
   if (storage == AIMMSAPI_STORAGE_DOUBLE){
		while ( AimmsValueNext(handle, tuple, &value) ) {
			for ( i = 0; i < full; i++ ) {
				strvalue.Length = 256;
				AimmsSetElementToName(domain[i], tuple[i], &strvalue);
				_ftprintf(f, _T("%s,"), buffer);
			} // for-loop
			_ftprintf(f,_T("%f\n"), value.Double); 
		}//while-loop
   } //if
   else{
		while ( AimmsValueNext(handle, tuple, &value) ) {
			for ( i = 0; i < full; i++ ) {
				strvalue.Length = 256;
				AimmsSetElementToName(domain[i], tuple[i], &strvalue);
				_ftprintf(f, _T("%s,"), buffer);
			}//for-loop
			_ftprintf(f, _T("%d\n"), value.Int); 
		};//while-loop
	};//else

   AimmsIdentifierHandleDelete(handle);
   for ( i = 0; i < full; i++ ) {
       AimmsIdentifierHandleDelete(domain[i]);
   }
   
   fclose(f);
}
