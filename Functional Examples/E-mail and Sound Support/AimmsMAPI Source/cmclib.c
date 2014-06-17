/*
 *   Interface on top of the Common Messaging Calls (CMC) API, to create and send e-mail
 *   messages from within e.g. the AIMMS environment. 
 *
 *   The accompanying AIMMS project imports the functions exported here.
 *
 *   Feel free to modify the source to suit your own needs.
 */ 

#include <windows.h>
#include <xcmc.h>
#include <xcmcext.h>
#include <xcmcmsxt.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>         // must be included before aimmsapi.h
#include "uniaimms.h"
#include "aimmsapi.h"

#define DLL_EXPORT(type) __declspec(dllexport) type WINAPI

#define NOTE_SIZE (1<<16)
#define SUBJ_SIZE (1<<8)
#define MAX_RECIP (1<<10)
#define MAX_ATTCH (1<<5)

static HINSTANCE      hlibMAPI = 0;

typedef CMC_return_code (WINAPI *fnCMCLogon)(
    CMC_string              service,
    CMC_string              user,
    CMC_string              password,
    CMC_object_identifier   character_set,
    CMC_ui_id               ui_id,
    CMC_uint16              caller_cmc_version,
    CMC_flags               logon_flags,
    CMC_session_id FAR      *session,
    CMC_extension FAR       *logon_extensions
);

static fnCMCLogon lpfnCMCLogon = NULL;

typedef CMC_return_code (WINAPI *fnCMCSendMail)(
    CMC_session_id          session,
    CMC_message FAR         *message,
    CMC_flags               send_flags,
    CMC_ui_id               ui_id,
    CMC_extension FAR       *send_extensions
);
static fnCMCSendMail lpfnCMCSendMail = NULL;

static CMC_session_id CMCSession = 0;

static int  AIMMSMapiErrorMode = 1;
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

static CMC_message AIMMSMapiMessage;

static char AIMMSNoteText[NOTE_SIZE];
static int  NoteLen;

static char AIMMSSubject [SUBJ_SIZE];

static char AIMMSMapiBuf[NOTE_SIZE];
static int  BufLen;

static CMC_recipient AIMMSMapiRecip[MAX_RECIP];
static int MaxRecip = 0;

static CMC_attachment AIMMSMapiFiles[MAX_ATTCH];
static int MaxFile = 0;

DLL_EXPORT(void) AIMMSMapiSetErrorMode( int mode )
{
	AIMMSMapiErrorMode = mode;
}

DLL_EXPORT(void) AIMMSMapiLastError( _TCHAR *message )
{
	STRNCPY( message, AIMMSMapiLastErrorMessage, 256 );
}

DLL_EXPORT(int) AIMMSMapiNewMail( _TCHAR *subject )
{
	memset( &AIMMSMapiMessage, 0, sizeof( CMC_message ) );
	
	memset( AIMMSNoteText, 0, sizeof( AIMMSNoteText ) );
	NoteLen = 0;
	
	STRNCPY_UNI_TO_ASC( AIMMSSubject, subject, SUBJ_SIZE - 1 );
	AIMMSSubject[SUBJ_SIZE - 1] = 0;
	
	AIMMSMapiMessage.subject       = AIMMSSubject;
	AIMMSMapiMessage.text_note     = AIMMSNoteText;
	AIMMSMapiMessage.message_flags = 0;
	
	AIMMSMapiMessage.recipients    = &AIMMSMapiRecip[0];
	MaxRecip = 0;
	
	AIMMSMapiMessage.attachments   = &AIMMSMapiFiles[0];
	MaxFile = 0;
	
	return 1;
}

DLL_EXPORT(int) AIMMSMapiAddText( _TCHAR *note_text )
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

DLL_EXPORT(int) AIMMSMapiAddLine( _TCHAR *note_text )
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

DLL_EXPORT(int) AIMMSMapiAddParagraph( _TCHAR *note_text )
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

DLL_EXPORT(int) AIMMSMapiAddTextFromFile( char *file )
{
	FILE *fp;
	_TCHAR buf[1025];
	int len, res; 
	
	if ( ! (fp = fopen( file, "r" )) ) {
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

int AIMMSMapiAddRecipient( int role, _TCHAR *recip_name, _TCHAR *recip_address )
{
	int len;
	
	if ( MaxRecip == MAX_RECIP ) {
		PASS_ERROR( _T("MAPI message has more than 1024 recipients.") );
		return 0;
	}
	
	memset( &AIMMSMapiRecip[MaxRecip], 0, sizeof( CMC_recipient ) );
	
	AIMMSMapiRecip[MaxRecip].role = role;
	AIMMSMapiRecip[MaxRecip].name_type = CMC_TYPE_INDIVIDUAL;

	len = STRLEN( recip_name );
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message recipients buffer size exceeded.") );
		return 0;
	}
	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen, recip_name );
	AIMMSMapiRecip[MaxRecip].name = AIMMSMapiBuf + BufLen;
	BufLen += len + 1;
	
	len = STRLEN( recip_address ) + 6;  /* Include "SMTP:" prefix and null termination" */
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message recipients buffer size exceeded.") );
		return 0;
	}
	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen, _T("SMTP:") );
	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen + 5, recip_address );
	AIMMSMapiRecip[MaxRecip].address = AIMMSMapiBuf + BufLen;
	BufLen += len;
	
	// This recipient becomes the last one, the previous one must be 
	// unflagged as the last recipient.

	if ( MaxRecip ) AIMMSMapiRecip[MaxRecip-1].recip_flags = 0; 
	AIMMSMapiRecip[MaxRecip].recip_flags = CMC_RECIP_LAST_ELEMENT; 

	MaxRecip ++;
	
	return 1;
}

DLL_EXPORT(int) AIMMSMapiAddRecipientTo( _TCHAR *recip_name, _TCHAR *recip_address )
{
	return AIMMSMapiAddRecipient( CMC_ROLE_TO, recip_name, recip_address );
}

DLL_EXPORT(int) AIMMSMapiAddRecipientCc( _TCHAR *recip_name, _TCHAR *recip_address )
{
	return AIMMSMapiAddRecipient( CMC_ROLE_CC, recip_name, recip_address );
}

DLL_EXPORT(int) AIMMSMapiAddRecipientBcc( _TCHAR *recip_name, _TCHAR *recip_address )
{
	return AIMMSMapiAddRecipient( CMC_ROLE_BCC, recip_name, recip_address );
}

DLL_EXPORT(int) AIMMSMapiAddAttachment( _TCHAR *path )
{
	int len;
	char absPath[_MAX_PATH], path_chr[_MAX_PATH];
	_TCHAR *file;
	
	if ( MaxFile == MAX_ATTCH ) {
		PASS_ERROR( _T("MAPI message has more than 32 attachments.") );
		return 0;
	}
	
	memset( &AIMMSMapiFiles[MaxFile], 0, sizeof( CMC_attachment ) );

	// Construct the full path to the attachment and store it in
	// the attach_filename field
	
	STRCPY_UNI_TO_ASC( path_chr, path );

    _fullpath( absPath, path_chr, _MAX_PATH );
	
	len = strlen( absPath );
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message attachment buffer size exceeded.") );
		return 0;
	}

	strcpy( AIMMSMapiBuf + BufLen, absPath );
	AIMMSMapiFiles[MaxFile].attach_filename = AIMMSMapiBuf + BufLen;
	BufLen += len + 1;
	
	// Construct the short filename (without directories) to the attachment
	// and store it in the attach_title field.

	file = path + STRLEN(path);
	while ( *file != _T('\\') && *file != _T('/') && file != path ) file--;

	len = STRLEN( file );
	if ( (BufLen + len) >= NOTE_SIZE ) {
		PASS_ERROR( _T("MAPI message attachment buffer size exceeded.") );
		return 0;
	}

	STRCPY_UNI_TO_ASC( AIMMSMapiBuf + BufLen, file );
	AIMMSMapiFiles[MaxFile].attach_title = AIMMSMapiBuf + BufLen;
	BufLen += len + 1;

	// This attachment becomes the last one, the previous one must be 
	// unflagged as the last attachment.

	if ( MaxFile ) AIMMSMapiFiles[MaxFile-1].attach_flags = 0;
	AIMMSMapiFiles[MaxFile].attach_flags = CMC_ATT_LAST_ELEMENT; 

	MaxFile ++;
	
	return 1;
}

DLL_EXPORT(int) AIMMSMapiSendMail(int interactive)
{
	_TCHAR *error_message = 0;
	int flags = 0, err = 0;
	_TCHAR saved_directory[_MAX_PATH];
	CMC_return_code res;

	if ( ! hlibMAPI ) {
		hlibMAPI = LoadLibrary(_T("mapi32.dll"));
		if ( !(lpfnCMCSendMail = (fnCMCSendMail) GetProcAddress (hlibMAPI, "cmc_send")) ||
			 !(lpfnCMCLogon    = (fnCMCLogon) GetProcAddress( hlibMAPI, "cmc_logon"))
			) {
			PASS_ERROR( _T("Cannot load required DLL mapi32.dll. No message was sent") );
			FreeLibrary( hlibMAPI );
			return 0;
		}
	}
	
	/* 
	 * Sending an e-mail message may screw up the current directory
	 * Hence we save it and restore it afterwards
	 */

	GetCurrentDirectory( _MAX_PATH, saved_directory );
	
	if ( ! CMCSession ) {
		res = lpfnCMCLogon(
                 NULL,                   /* Default service           */
                 NULL,                   /* Prompt for user name      */
                 NULL,                   /* Prompt for password       */
                 NULL,                   /* Default character set     */
                 0,                      /* Default UI ID             */
                 CMC_VERSION,            /* Version 1 CMC calls       */
                 CMC_LOGON_UI_ALLOWED |  /* Full logon UI             */
                 CMC_ERROR_UI_ALLOWED,   /* Use UI to display errors. */
                 &CMCSession,            /* Returned session ID       */
                 NULL);                  /* No extensions             */
		
		if ( res != CMC_SUCCESS ) {
			PASS_ERROR( _T("Cannot obtain valid MAPI session. No message was sent.") );
			return 0;
		}
	}
	
	flags = 0;
	if ( interactive ) flags = CMC_SEND_UI_REQUESTED;
	
	res = lpfnCMCSendMail( 
		     CMCSession,					/* Session ID - set with logon call   */
		     &AIMMSMapiMessage,				/* Message structure                  */
		     flags | CMC_ERROR_UI_ALLOWED,	/* Send flags                         */
		     0,								/* No UI will be used.                */
		     NULL);							/* No extensions                      */
	
	/* Restore the saved working directory */
	SetCurrentDirectory( saved_directory );
	
	switch ( res ) {
	case CMC_E_AMBIGUOUS_RECIPIENT: 
		PASS_ERROR( _T("The recipient name was ambiguous. Multiple matches were found. No message was sent") );
		err = 1;
		break;
	case CMC_E_ATTACHMENT_NOT_FOUND: 
		PASS_ERROR( _T("The specified attachment was not found as specified. No message was sent.") );
		err = 1;
		break;
	case CMC_E_ATTACHMENT_OPEN_FAILURE: 
		PASS_ERROR( _T("The specified attachment was found but could not be opened, or the attachment file could not be created. No message was sent.") );
		err = 1;
		break;
	case CMC_E_ATTACHMENT_READ_FAILURE: 
		PASS_ERROR( _T("The specified attachment was found and opened, but there was an error reading it. No message was sent.") );
		err = 1;
		break;
	case CMC_E_ATTACHMENT_WRITE_FAILURE: 
		PASS_ERROR( _T("The attachment file was created successfully, but there was an error writing it. No message was sent.") );
		err = 1;
		break;
	case CMC_E_COUNTED_STRING_UNSUPPORTED: 
		PASS_ERROR( _T("The current implementation does not support the counted-string type. No message was sent.") );
		err = 1;
		break;
	case CMC_E_FAILURE: 
		PASS_ERROR( _T("There was a general failure that does not fit the description of any other return value. No message was sent.") );
		err = 1;
		break;
	case CMC_E_INSUFFICIENT_MEMORY:
		PASS_ERROR( _T("Insufficient memory was available to complete the requested operation. No message was sent.") );
		err = 1;
		break;
	case CMC_E_INVALID_FLAG: 
		PASS_ERROR( _T("A flag set using a flags parameter was invalid. No message was sent.") );
		err = 1;
		break;
	case CMC_E_INVALID_MESSAGE_PARAMETER: 
		PASS_ERROR( _T("One of the parameters in the message was invalid. No message was sent.") );
		err = 1;
		break;
	case CMC_E_INVALID_PARAMETER: 
		PASS_ERROR( _T("A function parameter was invalid. No message was sent.") );
		err = 1;
		break;
	case CMC_E_INVALID_SESSION_ID: 
		PASS_ERROR( _T("The specified session handle is invalid or no longer valid, for example, after logging off. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_INVALID_UI_ID: 
		PASS_ERROR( _T("The specified user-interface identifier is invalid or no longer valid. No message was sent.") );
		err = 1;
		break;
	case CMC_E_LOGON_FAILURE:
		PASS_ERROR( _T("The service, user name, or password specified were invalid, so logon cannot be completed. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_RECIPIENT_NOT_FOUND: 
		PASS_ERROR( _T("One or more of the specified recipients were not found. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_TEXT_TOO_LARGE: 
		PASS_ERROR( _T("The size of the text string passed to the current implementation is too large. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_TOO_MANY_FILES: 
		PASS_ERROR( _T("The current implementation cannot support the number of files specified. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_TOO_MANY_RECIPIENTS: 
		PASS_ERROR( _T("The current implementation cannot support the number of recipients specified. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_UNSUPPORTED_DATA_EXT: 
		PASS_ERROR( _T("The current implementation does not support the data extension requested. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_UNSUPPORTED_FLAG: 
		PASS_ERROR( _T("The current implementation does not support the flag requested. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_UNSUPPORTED_FUNCTION_EXT: 
		PASS_ERROR( _T("The current implementation does not support the function extension requested. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_USER_CANCEL: 
		PASS_ERROR( _T("The operation was canceled by the user. No message was sent.") ); 
		err = 1;
		break;
	case CMC_E_USER_NOT_LOGGED_ON: 
		PASS_ERROR( _T("The user was not logged on and the CMC_LOGON_UI_ALLOWED flag was not set. No message was sent.") ); 
		err = 1;
		break;
	}
	
	return (err) ? 0 : 1;
}


