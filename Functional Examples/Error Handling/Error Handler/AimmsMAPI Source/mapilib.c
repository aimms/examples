/*
 *   Interface on top of the Simple MAPI API, to create and send e-mail
 *   messages from within e.g. the AIMMS environment. 
 *
 *   The accompanying AIMMS project imports the functions exported here.
 *
 *   Feel free to modify the source to suit your own needs.
 */ 

#include <windows.h>
#include <mapi.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>         // must be included before aimmsapi.h
#include "uniaimms.h"
#include "aimmsapi.h"

#define NOTE_SIZE (1<<16)
#define SUBJ_SIZE (1<<8)
#define MAX_RECIP (1<<10)
#define MAX_ATTCH (1<<5)

static HINSTANCE      hlibMAPI = 0;
static LPMAPISENDMAIL lpfnMAPISendMail = NULL;
static LPMAPILOGON    lpfnMAPILogon = NULL;
static LHANDLE        hMAPISession = 0;

static int AIMMSMapiErrorMode = 1;
static _TCHAR AIMMSMapiLastErrorMessage[256];

#define PASS_WARNING( text )                                                                        \
	if ( AIMMSMapiErrorMode != 0 ) {                                                                \
	    if ( AIMMSMapiErrorMode == 1 ) AimmsAPIPassMessage( AIMMSAPI_SEVERITY_CODE_WARNING, text ); \
	    else MessageBox( 0, text, _T("AIMMS Mapi Warning"), MB_OK | MB_ICONWARNING );               \
	} else STRCPY( AIMMSMapiLastErrorMessage, text )

#define PASS_ERROR( text )                                                                          \
	if ( AIMMSMapiErrorMode != 0 ) {                                                                \
	    if ( AIMMSMapiErrorMode == 1 ) AimmsAPIPassMessage( AIMMSAPI_SEVERITY_CODE_ERROR, text );   \
	    else MessageBox( 0, text, _T("AIMMS Mapi Error"), MB_OK | MB_ICONERROR );                   \
	} else STRCPY( AIMMSMapiLastErrorMessage, text )

static MapiMessage AIMMSMapiMessage;

static char AIMMSNoteText[NOTE_SIZE];
static int  NoteLen;

static char AIMMSSubject [SUBJ_SIZE];

static char AIMMSMapiBuf[NOTE_SIZE];
static int  BufLen;

static MapiRecipDesc AIMMSMapiRecip[MAX_RECIP];
static int MaxRecip = 0;

static MapiFileDesc	 AIMMSMapiFiles[MAX_ATTCH];
static int MaxFile = 0;

__declspec(dllexport) void WINAPI AIMMSMapiSetErrorMode( int mode )
{
	AIMMSMapiErrorMode = mode;
}

__declspec(dllexport) void WINAPI AIMMSMapiLastError( _TCHAR *message )
{
	STRNCPY( message, AIMMSMapiLastErrorMessage, 256 );
}

__declspec(dllexport) int WINAPI AIMMSMapiNewMail( _TCHAR *subject )
{
	memset( &AIMMSMapiMessage, 0, sizeof( MapiMessage ) );
	
	memset( AIMMSNoteText, 0, sizeof( AIMMSNoteText ) );
	NoteLen = 0;
	
	STRNCPY_UNI_TO_ASC( AIMMSSubject, subject, SUBJ_SIZE - 1 );
	AIMMSSubject[SUBJ_SIZE - 1] = 0;
	
	AIMMSMapiMessage.lpszSubject  = AIMMSSubject;
	AIMMSMapiMessage.lpszNoteText = AIMMSNoteText;
	AIMMSMapiMessage.flFlags      = 0;
	
	AIMMSMapiMessage.lpRecips     = &AIMMSMapiRecip[0];
	MaxRecip = 0;
	
	AIMMSMapiMessage.lpFiles      = &AIMMSMapiFiles[0];
	MaxFile = 0;
	
	return 1;
}

__declspec(dllexport) int WINAPI AIMMSMapiAddText( _TCHAR *note_text )
{
	int len = STRLEN( note_text );
	
	if ( (NoteLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message text buffer size exceeded.") );
		return 0;
	}
	
	STRCPY_UNI_TO_ASC( AIMMSNoteText + NoteLen, note_text );
	NoteLen += len;
	
	return 1;
}

__declspec(dllexport) int WINAPI AIMMSMapiAddLine( _TCHAR *note_text )
{
	int len = STRLEN( note_text );
	
	if ( (NoteLen + len + 1) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message text buffer size exceeded.") );
		return 0;
	}
	
	STRCPY_UNI_TO_ASC( AIMMSNoteText + NoteLen, note_text );
	NoteLen += len;
	
	AIMMSNoteText[NoteLen++] = '\n';
	
	return 1;
}

__declspec(dllexport) int WINAPI AIMMSMapiAddParagraph( _TCHAR *note_text )
{
	int len = STRLEN( note_text );
	
	if ( (NoteLen + len + 2) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message text buffer size exceeded.") );
		return 0;
	}
	
	STRCPY_UNI_TO_ASC( AIMMSNoteText + NoteLen, note_text );
	NoteLen += len;
	
	AIMMSNoteText[NoteLen++] = '\n';
	AIMMSNoteText[NoteLen++] = '\n';
	
	return 1;
}

__declspec(dllexport) int WINAPI AIMMSMapiAddTextFromFile( _TCHAR *file )
{
	FILE *fp;
	_TCHAR buf[1025];
	int len, res; 
	
	if ( ! (fp = FOPEN( file, _T("r") )) ) {
		PASS_ERROR( _T("The specified MAPI include file cannot be opened.") );
		return 0;
	}
	
	while ( (len = fread( buf, 1, 1024, fp )) ) {
		buf[len] = 0;
		res = AIMMSMapiAddText( buf );
		if ( ! res ) return 0;
	}
	
	return 1;
}

int AIMMSMapiAddRecipient( int type, _TCHAR *recip_name, _TCHAR *recip_address )
{
	int len;
	
	if ( MaxRecip == MAX_RECIP ) {
		PASS_ERROR( _T("MAPI message has more than 1024 recipients.") );
		return 0;
	}
	
	memset( &AIMMSMapiRecip[MaxRecip], 0, sizeof( MapiRecipDesc ) );
	
	AIMMSMapiRecip[MaxRecip].ulRecipClass = type;
	
	len = STRLEN( recip_name );
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message recipients buffer size exceeded.") );
		return 0;
	}
	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen, recip_name );
	AIMMSMapiRecip[MaxRecip].lpszName    = AIMMSMapiBuf + BufLen;
	BufLen += len + 1;
	
	len = STRLEN( recip_address ) + 6;  /* Include "SMTP:" prefix and null termination" */
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message recipients buffer size exceeded.") );
		return 0;
	}
	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen, _T("SMTP:") );
	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen + 5, recip_address );
	AIMMSMapiRecip[MaxRecip].lpszAddress = AIMMSMapiBuf + BufLen;
	BufLen += len;
	
	MaxRecip ++;
	AIMMSMapiMessage.nRecipCount = MaxRecip;
	
	return 1;
}

__declspec(dllexport) int WINAPI AIMMSMapiAddRecipientTo( _TCHAR *recip_name, _TCHAR *recip_address )
{
	return AIMMSMapiAddRecipient( MAPI_TO, recip_name, recip_address );
}

__declspec(dllexport) int WINAPI AIMMSMapiAddRecipientCc( _TCHAR *recip_name, _TCHAR *recip_address )
{
	return AIMMSMapiAddRecipient( MAPI_CC, recip_name, recip_address );
}

__declspec(dllexport) int WINAPI AIMMSMapiAddRecipientBcc( _TCHAR *recip_name, _TCHAR *recip_address )
{
	return AIMMSMapiAddRecipient( MAPI_BCC, recip_name, recip_address );
}


__declspec(dllexport) int WINAPI AIMMSMapiAddAttachment( _TCHAR *path )
{
	int len;
	char absPath[_MAX_PATH], path_chr[_MAX_PATH];
	
	if ( MaxFile == MAX_ATTCH ) {
		PASS_ERROR( _T("MAPI message has more than 32 attachments.") );
		return 0;
	}
	
	memset( &AIMMSMapiFiles[MaxFile], 0, sizeof( MapiFileDesc ) );
	
	AIMMSMapiFiles[0].nPosition = -1;
	
	STRCPY_UNI_TO_ASC( path_chr, path );
	
    _fullpath( absPath, path_chr, _MAX_PATH );
	
	len = strlen( absPath );
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message attachment buffer size exceeded.") );
		return 0;
	}
	strcpy( AIMMSMapiBuf + BufLen, absPath );
	AIMMSMapiFiles[MaxFile].lpszPathName = AIMMSMapiBuf + BufLen;
	BufLen += len + 1;
	
	MaxFile ++;
	AIMMSMapiMessage.nFileCount = MaxFile;
	
	return 1;
}


typedef BOOL (WINAPI SETDLLDIRECTORY)(
	LPSTR dir
);
typedef SETDLLDIRECTORY FAR *LPSETDLLDIRECTORY;

static void ResetDllDirectory()
{
	LPSETDLLDIRECTORY fcn;
	HANDLE kernel32 = LoadLibrary("Kernel32.dll");
	if ( ! kernel32 ) return;

	fcn = (LPSETDLLDIRECTORY)GetProcAddress( kernel32, "SetDllDirectoryA" );
	if ( !fcn ) return ;
	fcn(0);
}

__declspec(dllexport) int WINAPI AIMMSMapiSendMail(int interactive)
{
	_TCHAR *error_message = 0;
	int res, flags = 0, err = 0;
	_TCHAR saved_directory[_MAX_PATH];
	
	if ( ! hlibMAPI ) {
		hlibMAPI = LoadLibrary(_T("mapi32.dll"));
		if ( !(lpfnMAPISendMail = (LPMAPISENDMAIL) GetProcAddress (hlibMAPI, "MAPISendMail")) ||
			!(lpfnMAPILogon    = (LPMAPILOGON) GetProcAddress( hlibMAPI, "MAPILogon"))
			) {
			PASS_ERROR( _T("Cannot load required DLL mapi32.dll. No message was sent") );
			FreeLibrary( hlibMAPI );
			return 0;
		}
	}
	
	GetCurrentDirectory( _MAX_PATH, saved_directory );

	//if ( ! hMAPISession ) {
	//	flags = MAPI_LOGON_UI;
	//	res   = (*lpfnMAPILogon)(0L, NULL, NULL, flags, 0L, (LPLHANDLE)&hMAPISession);
	//	
	//	if ( res != SUCCESS_SUCCESS ) {
	//		PASS_ERROR( _T("Cannot obtain valid MAPI session. No message was sent.") );
	//	}
	//}
	
	flags = 0;
	if ( interactive ) flags = MAPI_DIALOG;

	res = lpfnMAPISendMail( /*hMAPISession*/0, 0, &AIMMSMapiMessage, flags, 0 );

	// It turns out that some Outlook versions use SetDllDirectory to influence the DLL load 
	// search order.  Before returning we make sure to reset it to the state that AIMMS expects.
	ResetDllDirectory();

	SetCurrentDirectory( saved_directory );
	
	switch ( res ) {
	case MAPI_E_AMBIGUOUS_RECIPIENT: 
		PASS_ERROR( _T("A recipient was entered twice. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_ATTACHMENT_NOT_FOUND: 
		PASS_ERROR( _T("The specified attachment was not found. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_ATTACHMENT_OPEN_FAILURE: 
		PASS_ERROR( _T("The specified attachment could not be opened. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_BAD_RECIPTYPE: 
		PASS_ERROR( _T("The type of a recipient is wrong. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_FAILURE: 
		PASS_ERROR( _T("One or more unspecified errors occurred. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_INSUFFICIENT_MEMORY:
		PASS_ERROR( _T("There was insufficient memory to proceed. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_INVALID_RECIPS: 
		PASS_ERROR( _T("One or more recipients were invalid or did not resolve to any address.") );
		break;
	case MAPI_E_LOGIN_FAILURE: 
		PASS_ERROR( _T("There was no default logon, and the user failed to log on successfully when the logon dialog box was displayed. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_TEXT_TOO_LARGE: 
		PASS_ERROR( _T("The text in the message was too large. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_TOO_MANY_FILES: 
		PASS_ERROR( _T("There were too many file attachments. No message was sent.") ); 
		err = 1;
		break;
	case MAPI_E_TOO_MANY_RECIPIENTS: 
		PASS_ERROR( _T("There were too many recipients. No message was sent.") );
		err = 1;
		break;
	case MAPI_E_UNKNOWN_RECIPIENT:
		PASS_ERROR( _T("A recipient did not appear in the address list. No message was sent.") ); 
		err = 1;
		break;
	case MAPI_E_USER_ABORT: 
		PASS_ERROR( _T("The user canceled one of the dialog boxes. No message was sent.") ); 
		err = 1;
		break;
	}
	
	return (err) ? 0 : 1;
}


