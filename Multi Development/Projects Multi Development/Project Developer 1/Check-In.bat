:: Check-In.bat
:: This batch file copies the files from the developer library to the
:: latest version project folder.


:: Copy from my own to 'Latest Version' (and exclude log and backup files)
xcopy ".\Developer 1\*.*" "..\Latest Version\Developer 1"  /exclude:Xcheckin.txt /s/y/r/d

:: Make the copied files read-only
attrib +R "..\Latest Version\Developer 1\*.*" /s
:: except for the Developerstate file
attrib -R "..\Latest Version\Developer 1\*.Developerstate" /s

pause