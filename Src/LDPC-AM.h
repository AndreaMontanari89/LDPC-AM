#ifndef __LDPC_AM_H__
#define __LDPC_AM_H__
#pragma once

#ifndef __STDAFX_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "Resource.h"
#include "AppEvents.h"
class CMainFrame;

//
// Dichiaraz. costanti, tipi, strutture, oggetti, etc. necessari per la dichiarazione della classe o usati molto frequentemente
//

//
// CxppIPM_App - Dichiarazione oggetto applicazione xppIPM
//
class CLDPCApp : public wxApp
{
	// Costruttore(i) / distruttore

	//
	// Enum, strutture, classi interne all'oggetto
	//
public:
	enum _AppExitReason												// Identif. causa uscita dall'applicazione
	{
		AER_NormalExit,
		AER_AppInitFailure,
	};

	class CCmdArgs													// Opzioni impostate da riga di comando
	{
	public:
		CCmdArgs()
		{
			m_strConfigFile.Clear();
		};
		wxString m_strConfigFile;
	};


	//
	// Funzioni
	//
public:

//	const CAppCfg*		GetAppCfg( void ) const						{ return( m_pAppCfg ); };
	wxString			GetAppCfgFilename( void )					{ return( m_strAppCfgPathName ); };
	const CCmdArgs*		GetCmdArgs( void ) const					{ return( &m_CmdArgs ); };
	const wxString&		GetCurrWrkDir( void ) const					{ return( m_strCurrWrkDir ); };
	CMainFrame*			GetMainWnd( void )							{ return( (CMainFrame*) GetTopWindow() ); };
	int64_t				GetTimestamp( void );
	double				GetTimestampMSec( void );
	int64_t				GetTimestampUSec( void );
	int					MakeFileNameAbs( wxString* pstrFile );
	int					MakeFileNameAbsFromDir( wxString* pstrFile, wxString pstrBaseDir );
	void				GetFileVers(wxString* pstrFileVer, int* piFileVer);
	int					MakePathAbs( wxString* pstrPath );
	long				MessageBox(	wxString	strMessage,
									long		lStyle = wxOK | wxCENTRE,
									wxWindow*	pParent = NULL,
									wxString	strCaption = wxT( "" ),
									wxString	strBtn1Msg = wxT( "" ),
									wxString	strBtn2Msg = wxT( "" ),
									wxString	strBtn3Msg = wxT( "" )
									);
	int					OnExit( void );
	bool				OnInit( void );
	//
	// Variabili
	//
	// Variabili 'standard' comuni a tutte le applicazioni
protected:
	int					m_iAppExitReason;							// Identif. causa uscita dall'applicazione
	wxString			m_strExecPath;								// Pathname eseguibile ( es.: C:\MyApp\MyAppW32UD.exe )
	wxString			m_strAppName;								// Nome applicazione ( es.: MyAppW32UD.exe )
	wxArrayString		m_strCmdArgs;								// Array argomenti sulla riga di comando
	CCmdArgs			m_CmdArgs;									// Opzioni impostate da riga di comando
	wxString			m_strCurrWrkDir;							// Directory di lavoro corrente ( termina con \ )
	wxString			m_strAppCfgPathName;						// Pathname file config. applicazione
//	CAppCfg*			m_pAppCfg;									// Punt. alla configurazione applicazione
	LONGLONG			m_llHighResolSysTimerFrequency;				// Frequenza del timer di sistema ad alta risoluzione
	int					m_iFileVers[4];								// Versione file ( applicazione )
private:
	// Object that wants to respond to events
	DECLARE_EVENT_TABLE()
};


//
// Create a forward declaration of the wxGetApp function implemented by IMPLEMENT_APP
//
// Senza questa istruzione ::wxGetApp() non viene definita
DECLARE_APP(CLDPCApp)


//
// Funzione:
//		double CxppIPM_App::GetTimestampMSec( void )
//
// Descrizione:
//		Lettura timestamp convertito in millisecondi
//
// Parametri:
//		Vedi codice
//
// Output:
//		Vedi codice
//
__forceinline double CLDPCApp::GetTimestampMSec( void )
{
	LARGE_INTEGER liTime;

	QueryPerformanceCounter( &liTime );

	return( (double) liTime.QuadPart / (double) m_llHighResolSysTimerFrequency * 1000.0 );
}
#endif
