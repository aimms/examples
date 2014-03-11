#include <stdio.h>
#include <string.h>
#include <aimmsapi.h>

DLL_EXPORT(void) print_double_aimms_identifier_info(char *name)
{
   int  handle, full, sliced, domain[AIMMSAPI_MAX_DIMENSION],
        tuple[AIMMSAPI_MAX_DIMENSION], storage, i;
   char file[256], buffer[256];
   FILE *f;
   
   AimmsValue  value;
   AimmsString strvalue;

   /* Create a handle associated with the identifier name passed */
   AimmsIdentifierHandleCreate(name, NULL, NULL, 0, &handle);
   
   /* Get the dimension, domain and storage type of the identifier
      associated with the handle */
   AimmsAttributeDimension (handle, &full, &sliced); 
   AimmsAttributeRootDomain(handle, domain); 
   AimmsAttributeStorage   (handle, &storage); 
   
   if ( storage != AIMMSAPI_STORAGE_DOUBLE ) return;
   
   /* Open a file consisting of the identifier name with the extension .def,
      and print the identifier's name and dimension */
      
   strcpy(file, name); strcat(file, ".def");
   if ( ! (f = fopen(file, "w")) ) return;
   fprintf(f, "Identifier name: %s\n", name);
   fprintf(f, "Dimension      : %d\n", full);

   /* Prepare strvalue to hold the locally declared buffer */
   strvalue.String = buffer;
   
   /* Print a header containing the names of the domain sets */
   fprintf(f, "\nData values    : \n");
   for ( i = 0; i < full; i++ ) {
       strvalue.Length = 256;
       AimmsAttributeName(domain[i], &strvalue); fprintf(f, "%17s", buffer);
   }
   fprintf(f,"%17s\n","Double value");
   for ( i = 0; i <= full; i++ ) fprintf(f, "%17s", "----------------");
   fprintf(f,"\n");

   /* Print all tuples with nondefault data values */
   AimmsValueResetHandle(handle);
   while ( AimmsValueNext(handle, tuple, &value) ) {
       for ( i = 0; i < full; i++ ) {
           strvalue.Length = 256;
           AimmsSetElementToName(domain[i], tuple[i], &strvalue);
           fprintf(f,"%17s", buffer);
       }
       fprintf(f,"%17.5f\n", value.Double); 
   }
   
   fclose(f);
}
