#include <windows.h>
#include <mmsystem.h>

#define DLL_EXPORT(type) __declspec(dllexport) type WINAPI

DLL_EXPORT(int) PlayWavFile( char *wavfile, int async )
{
	return PlaySound( wavfile, NULL, 
		async ? SND_ASYNC : SND_SYNC );
}
