// The following functions return 1 upon success, 0 upon failure.
// After returning 0, the error message can be obtained via lastErrorMessage()

// Initializes the this module.
// Called from within dllmain.
extern int get_start( void );

// Terminates the dll
// Called from within dllmain.
extern int get_finish( void );

extern void Trace(_TCHAR *fmt, ... );
extern void Error(_TCHAR *fmt, ... );

// Returns the filehandle for filename in hndl.
DLL_EXPORT_PROTO(int) openFileHandle( _TCHAR *filename, int *hndl );

// Closes file handle
DLL_EXPORT_PROTO(int) closeFileHandle( int hndl );

// Return the last error message.
DLL_EXPORT_PROTO(int) lastErrorMessage( _TCHAR *err );

// Get a line from file, possibly including a newline char.
DLL_EXPORT_PROTO(int) getLine( int hndl, _TCHAR *line, int maxSize );

// Upon success, result stored in i.
DLL_EXPORT_PROTO(int) getInt( int hndl, int *i );

// Upon success, result stored in dbl.
DLL_EXPORT_PROTO(int) getDouble( int hndl, double *dbl );

