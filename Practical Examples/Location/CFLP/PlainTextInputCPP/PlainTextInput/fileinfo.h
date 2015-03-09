#include "stdafx.h"

class fileInfo
{
private:
    tstring m_Filename;
    FILE *m_File ;
    int m_LineNumber ; // Line and column number indicate current position.
    int m_ColumnNumber ;

public:
    fileInfo(_TCHAR *fn, FILE *file); // Constructor.
    ~fileInfo(); // Destructor.

    // Getters.
    FILE *file(void) const { return m_File ; } ;
    int line(void) const { return m_LineNumber ; } ;
    int column(void) const { return m_ColumnNumber ; } ;
    tstring filename(void) const { return m_Filename ; } ;


    // actual work horses.

    int getch(int *pch); // Return a character of feof.

    // Get a line, possibly including a newline character.
    // Returns 0 upon error, 1 upon success.
    int getLine(_TCHAR *buf, int maxSize); 

    // Get an int/double and return it into i/d.
    // Returns 0 upon error, 1 upon success.
    int getInt(int *i);
    int getDouble(double *d);
} ; // end class fileInfo 

