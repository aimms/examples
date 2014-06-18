// AimmsInterrupt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "aimmsapi.h"

#define EXPORT_DLL(type) extern "C" type  __declspec(dllexport) WINAPI

static bool s_Interrupted = false;
static bool s_Uninstalling = false;
static HANDLE s_InterruptEvent = 0;

/* The procedure "AimmsInterruptFunction" is installed as interrupt callback procedure
and will be called many times by AIMMS to check if the execution must be interrupted.
This only happens if "s_Interrupted" is true. This is set in "ThreadHandler", if the 
event "s_InterruptEvent" is set or "s_Uninstalling".
*/
int WINAPI AimmsInterruptFunction(int reason) 
{
	// if the interrupt flag is set, interrupt the entire execution

	if ( s_Interrupted ) {
		s_Interrupted = false;
		return AIMMSAPI_INTERRUPT_EXECUTE;
	}

	return AIMMSAPI_INTERRUPT_NOT;
}

/*
This function is executed as part of the thread that waits for the event "s_InterruptEvent".
If this event is set, the flag "s_Interrupted" is set to true and this means that
"AimmsInterruptFunction" will return "AIMMSAPI_INTERRUPT_EXECUTE" (and hence the execution
will be interrupted"
*/
void ThreadHandler(void *data)
{
	while (1) {
		// break from the loop if the event is not signalled or if we're told to uninstall

		if ( WAIT_OBJECT_0 != WaitForSingleObject(s_InterruptEvent, INFINITE) || s_Uninstalling ) break;

		// otherwise set the interrupted flag to true

		s_Interrupted = true;
	} 

	// This thread, being the last one to wait for it, is made responsible for closing the event

	CloseHandle( s_InterruptEvent );
	s_Uninstalling = false;
	s_InterruptEvent = 0;
}

/*
This procedure will install the Interrupt handler. It will create the event that, if set,
indicates that the execution must be interrupted. After the event has been created
succesfully it will create the thread that will wait to see if this event is set.
It will also install the callback procedure that AIMMS will check to see if the 
execution must be interrupted.
*/
EXPORT_DLL(int) InstallInterruptEvent(char *eventName)
{
	if ( s_InterruptEvent ) return AIMMSAPI_FAILURE;

	sprintf( eventName, "AimmsInterruptEvent_%d", GetCurrentProcessId() );
	s_InterruptEvent = CreateEvent( NULL, FALSE, FALSE, eventName );

	if ( s_InterruptEvent ) {

		s_Uninstalling = false;
		HANDLE thread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadHandler, 0, 0, 0 );

		AimmsInterruptCallbackInstall( (AimmsInterruptCallback)AimmsInterruptFunction );

		CloseHandle(thread);

		return AIMMSAPI_SUCCESS;
	}

	return AIMMSAPI_FAILURE;
}

/*
This will uninstall the interrupt handler.
*/
EXPORT_DLL(int) UninstallInterruptEvent(void)
{
	if ( s_InterruptEvent ) {
		s_Uninstalling = true;

		// Signal the event, which will cause the interrupt thread to clean up the event and end itself
		SetEvent(s_InterruptEvent);
		AimmsInterruptCallbackInstall( 0 );

		return AIMMSAPI_SUCCESS;
	}

	return AIMMSAPI_FAILURE;
}

/*
This procedure will set the interruptEvent that will cause the execution to
to be interrupted via "ThreadHandler" and "s_Interrupted".

This (or similar) procedure can be called from any application on this machine
to interrupt the execution.
*/
EXPORT_DLL(int) SignalInterruptEvent(char *eventName)
{
	HANDLE interruptEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, eventName );

	if ( interruptEvent ) {
		SetEvent( interruptEvent );
		CloseHandle( interruptEvent );

		return AIMMSAPI_SUCCESS;
	}

	return AIMMSAPI_FAILURE;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		UninstallInterruptEvent();
		break;
	}
    return TRUE;
}

