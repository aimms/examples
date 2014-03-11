:: Check-Out.bat
:: This batch file copies the files from the latest project version to the
:: project folder in which this batch file is located.
::

:: Copy all files but my own from the 'Latest Version' (and exclude log and backup files)
xcopy "..\Latest Version\*.*" "." /exclude:Xcheckout.txt /s/y/r/d
attrib -r "Developer 2\*.*" /s
attrib +r "Developer 1\*.*" /s
attrib +r *.*
attrib -r *.bat /s
attrib -r "Developer 1\*.Developerstate" /s

pause
