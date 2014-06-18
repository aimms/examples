// InterruptProgram.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>

/*
This procedure will set the interruptEvent that will cause the execution to
to be interrupted via "ThreadHandler" and "s_Interrupted"
(see file "AimmsInterrupt.cpp")
*/

int SignalInterruptEvent(char *eventName)
{
	HANDLE interruptEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, eventName );

	if ( interruptEvent ) {
		SetEvent( interruptEvent );
		CloseHandle( interruptEvent );

		return 1;
	}

	return 0;
}

/*
This program will read the file "InterruptEventName.txt" to obtain
the name of the event that must be used to signal the interruption.
*/
int main(int argc, char* argv[])
{
	char EventName[1024];
	FILE *fp;

	if (fp = fopen("InterruptEventName.txt", "r")) {

		fscanf(fp,"%s",EventName);
		if (EventName){
			SignalInterruptEvent(EventName);
			return 1;
		}
	}
	return 0;

}

