#include "stdafx.h"
#include "LDPC-AM.h"
#include "MainFrm.h"
#include "Version.h"

//
// Make the application class known to wxWidgets for dynamic construction
//
IMPLEMENT_APP(CLDPCApp)

//
// CLDPCApp event table
//
BEGIN_EVENT_TABLE(CLDPCApp, wxApp )
END_EVENT_TABLE()

bool CLDPCApp::OnInit( void )
{
	bool				bFuncRetVal = false;
	wxString			strErrMsg( wxT( "" ) );
	long				lErrMsgDlgStyle = wxOK | wxICON_HAND | wxCENTRE;
	wxString			strTmp, strTmp1, strCmdParam;
	wxString			strMsgsTxtPathName;
	wxFileName			fnFileName;
	LARGE_INTEGER		liTime;
	int					iParamIdx;
	CMainFrame*			pMainFrame;

	do
	{

		//
		// Inizializz. variabili oggetto
		//
		m_iAppExitReason = AER_AppInitFailure;						// Identif. causa uscita dall'applicazione
		m_strAppCfgPathName.Clear();								// Pathname file config. applicazione
//		m_pAppCfg = NULL;											// Punt. alla configurazione applicazione
		
		m_llHighResolSysTimerFrequency = -1;						// Frequenza del timer di sistema ad alta risoluzione
		if ( QueryPerformanceFrequency( &liTime ) )
				m_llHighResolSysTimerFrequency = liTime.QuadPart;
		else
			m_llHighResolSysTimerFrequency = -1;					// Timer non disponibile

		// Inizializzazione del generatore di numeri casuali
		srand( (unsigned) time( NULL ) );

		// Inizializzazione gestori wxImageHandler per salvataggio immagini wxImage in formato JPEG e BMP
		{
			wxImageHandler* pImageHandler;

			// Nota: saranno distrutti automaticamnte a fine applicazione
			pImageHandler = new wxBMPHandler();
			wxImage::AddHandler( pImageHandler );

			pImageHandler = new wxJPEGHandler();
			wxImage::AddHandler( pImageHandler );
		}

		//
		// Ricava il path, il nome dell'applicazione e il pathname di default del file di configurazione
		//
		{
			wxStandardPaths& sp = wxStandardPaths::Get();

			// Pathname eseguibile ( es.: C:\Develop\...\Debug\MyAppW32UD.exe o C:\MyApp\MyAppW32UD.exe )
			m_strExecPath = sp.GetExecutablePath();
			fnFileName.Assign( m_strExecPath );

			// Nome applicazione ( es.: MyAppW32UD.exe ) - GetAppName() equivale a fnExeName.GetName()
			m_strAppName = GetAppName();

			// Directory di lavoro corrente ( es.: C:\MyApp\ )
			fnFileName.AssignDir( ::wxGetCwd() );
			fnFileName.Normalize( wxPATH_NORM_ALL  );
			m_strCurrWrkDir = fnFileName.GetFullPath();

			// Costruisci il pathname di default del file di configurazione, ma senza estensione
			m_strAppCfgPathName.Printf( wxT( "%sxppIPM" ), m_strCurrWrkDir );
		}

		//
		// Lettura, analisi e salvataggio parametri su riga di comando
		//
		m_strCmdArgs.Empty();
		if ( argc > 1 )
			m_strCmdArgs.Alloc( argc - 1 );

 		for ( iParamIdx = 1; iParamIdx < argc; ++iParamIdx )
		{
			strCmdParam = wxString( argv[iParamIdx] );
			m_strCmdArgs.Add( strCmdParam );						// Salva il parametro

			//
			// Controllo switch ( i parametri che iniziano per '/' o '-' )
			//
			if ( ( strCmdParam[0] == (wxChar) '-' || strCmdParam[0] == (wxChar) '/' ) && strCmdParam.Len() > 1 )
			{
				// Esempio lettura parametri
				//if ( !strCmdParam.Mid( 1 ).CmpNoCase( wxT( "ShowWnd" ) ) )
				//{
				//	// Flag visualizz. finestra applicazione
				//	m_CmdArgs.m_bShowWnd = true;
				//}
			}

			//
			// Spazio per salvataggio parametri ( serve un contatore di parametri )
			//
		}

		// Uscita con errore nel caso di parametro non valido
		if ( iParamIdx < argc )
			break;

		// Versione file
		m_iFileVers[0] = FILE_VERSION_MAJOR;
		m_iFileVers[1] = FILE_VERSION_MINOR;
		m_iFileVers[2] = FILE_VERSION_REVISION;
		m_iFileVers[3] = FILE_VERSION_BUILD;

		//
		// Controllo esistenza file di configurazione
		//
		if ( m_CmdArgs.m_strConfigFile.IsEmpty() )
		{
			// File di configurazione di default ( xml o ini )
			fnFileName.Assign( m_strAppCfgPathName + wxT( ".xml" ) );
			m_strAppCfgPathName = fnFileName.GetFullPath();
			if ( !fnFileName.FileExists() )
			{
				fnFileName.SetExt( wxT( "ini" ) );
				m_strAppCfgPathName = fnFileName.GetFullPath();
				if ( !fnFileName.FileExists() )
				{
					//strErrMsg.Printf( wxT( "File \"%s\" not found." ), m_strAppCfgPathName );
					//break;
				}
			}
		}
		else
		{
			// File di configurazione specificato come argomento della riga di comando
			strTmp = m_CmdArgs.m_strConfigFile;
			MakeFileNameAbs( &strTmp );
			fnFileName.Assign( strTmp );
			m_strAppCfgPathName = fnFileName.GetFullPath();
			if ( !fnFileName.FileExists() )
			{
				strErrMsg.Printf( wxT( "File \"%s\" not found." ), m_strAppCfgPathName );
				break;
			}
		}

		//
		// Caricamento configurazione applicazione
		//
//		m_pAppCfg = new CAppCfg;
//		m_pAppCfg->Set( m_strAppCfgPathName, wxT( "xppIPM_Main" ) );
//		if ( ( iRC = m_pAppCfg->Read() ) != ErrOk )
//		{
//			strErrMsg.Printf( wxT( "Error %d loading configuration file \"%s\"" ), iRC, m_strAppCfgPathName );
//			break;
//		}
		//
		// Creazione e visualizza la mainframe dell'applicazione
		//

		// Creazione e visualizza la mainframe dell'applicazione
		pMainFrame = new CMainFrame( "Simulazione LDPC", wxDefaultPosition, wxDefaultSize );

		// Informa l'applicazione che pMainFrame e' la finestra principale ( opzionale )
		SetTopWindow( pMainFrame );


		// Visualizza la finestra principale dell'applicazione
		//
		pMainFrame->Show( true );

		// Inizializzazione terminata correttamente
		m_iAppExitReason = AER_NormalExit;
		bFuncRetVal = true;
	}
	while ( 0 );

	//
	// Si sono verificati degli errori nella inizializzazione dell'applicazione ?
	//
	if ( !bFuncRetVal )
	{
		// In caso di errori in inizializzazione, la OnExit non viene chiamata, quindi la
		// richiamiamo manualmente ( vedi OnExit() ) per il parziale cleanup
		assert( m_iAppExitReason == AER_AppInitFailure );
		this->OnExit();

		// Visualizza eventuali messaggi di errore
		if ( !strErrMsg.IsEmpty() )
			MessageBox( strErrMsg, lErrMsgDlgStyle );
	}

    return( bFuncRetVal );
}


//
// Funzione:
//		int CLDPCApp::OnExit( void )
//
// Descrizione:
//		Evento richiamato sull'uscita dall'applicazione
//
// Parametri:
//		Nessuno
//
// Output:
//		Vedi documentazione wxWidgets
//
int CLDPCApp::OnExit( void )
{
	// Distruzione oggetto configurazione applicazione
	//if ( m_pAppCfg )
	//{
		// Spazio per salvataggio dati configurazione

		// Deallocazione oggetto configurazione applicazione
	//	delete m_pAppCfg;
	//}

	// Cleanup gestori wxImageHandler per salvataggio immagini wxImage
	wxImage::CleanUpHandlers();

	// Determina la causa dell'uscita dall'applicazione
	if ( m_iAppExitReason == AER_AppInitFailure )
	{
		// In caso di errori in inizializzazione applicazione, questa funzione OnExit() viene chiamata
		// direttamente dalla OnInit() e NON deve essere eseguita la wxApp::OnExit().
		return( 0 );
	}
	else
	{
		// Uscita normale dall'applicazione
		return( wxApp::OnExit() );
	}
}



//
// Funzione:
//		int CLDPCApp::MakeFileNameAbs( wxString* pstrFile )
//
// Descrizione:
//		Funzione per rendere assoluto un nome file relativo, accodandolo al path dell'applicazione
//
// Parametri:
//		wxString*	pstrPath				Punt. al nome file da rendere assoluto
//
// Output:
//		ErrOk se la funzione e' terminata correttamente, altrimenti il codice dell'errore avvenuto
//
int CLDPCApp::MakeFileNameAbs( wxString* pstrFile )
{
	int			iFuncRetVal = 1;
	wxFileName	fnFileName;

	do
	{
		// Controllo parametro
		if ( !pstrFile )
			break;
		if ( pstrFile->IsEmpty() )
			break;
		iFuncRetVal = 0;
		fnFileName.Assign( *pstrFile );

		// Esci se il nome file specificato dal parametro non e' relativo
		if ( fnFileName.IsAbsolute() )
			break;

		// Accoda il nome file relativo alla directory di lavoro dell'applicazione
		fnFileName.Normalize( wxPATH_NORM_ALL, m_strCurrWrkDir );

		// Esporta il pathname assoluto ed esci
		*pstrFile = fnFileName.GetFullPath();
	}
	while ( 0 );

	return( iFuncRetVal );
}


//
// Funzione:
//		int CLDPCApp::MakeFileNameAbs( wxString* pstrFile )
//
// Descrizione:
//		Funzione per rendere assoluto un nome file relativo, accodandolo al path della directory strBaseDir
//
// Parametri:
//		wxString*	pstrPath				Punt. al nome file da rendere assoluto
//
//		wxString strBaseDir					Path della directory a cui si accoda il file
//
// Output:
//		ErrOk se la funzione e' terminata correttamente, altrimenti il codice dell'errore avvenuto
//
int CLDPCApp::MakeFileNameAbsFromDir( wxString* pstrFile,  wxString strBaseDir )
{
	int			iFuncRetVal = 1;
	wxFileName	fnFileName;

	do
	{
		// Controllo parametro
		if ( !pstrFile )
			break;
		if ( pstrFile->IsEmpty() )
			break;
		iFuncRetVal = 0;
		fnFileName.Assign( *pstrFile );

		// Esci se il nome file specificato dal parametro non e' relativo
		if ( fnFileName.IsAbsolute() )
			break;

		// Accoda il nome file relativo alla directory di lavoro dell'applicazione
		fnFileName.Normalize( wxPATH_NORM_ALL, strBaseDir );

		// Esporta il pathname assoluto ed esci
		*pstrFile = fnFileName.GetFullPath();
	}
	while ( 0 );

	return( iFuncRetVal );
}


//
// Funzione:
//		int CLDPCApp::MakePathAbs( wxString* pstrPath )
//
// Descrizione:
//		Funzione per rendere assoluto un path relativo, accodandolo alla directory di lavoro dell'applicazione
//
// Parametri:
//		wxCString*	pstrPath				Punt. al path da rendere assoluto
//
// Output:
//		ErrOk se la funzione e' terminata correttamente, altrimenti il codice dell'errore avvenuto
//
int CLDPCApp::MakePathAbs( wxString* pstrPath )
{
	int			iFuncRetVal = 1;
	wxFileName	fnFileName;

	do
	{
		// Controllo parametro
		if ( !pstrPath )
			break;
		iFuncRetVal = 0;

		if ( pstrPath->IsEmpty() )
			fnFileName.AssignDir( m_strCurrWrkDir );
		else
		{
			fnFileName.AssignDir( *pstrPath );

			// Se il path specificato dal parametro e' relativo, accodalo alla directory di lavoro dell'applicazione
			if ( !fnFileName.IsAbsolute() )
				fnFileName.Normalize( wxPATH_NORM_ALL, m_strCurrWrkDir );
		}

		// Esporta il path assoluto ed esci
		*pstrPath = fnFileName.GetPath( wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR );
	}
	while ( 0 );

	return( iFuncRetVal );
}

// Funzione:
//		long CLDPCApp::MessageBox( .... )
//
// Descrizione:
//		Funzione sostitutiva della AfxMessageBox()
//
// Parametri:
//		Vedi codice
//
// Output:
//		Vedi codice
//
long CLDPCApp::MessageBox(	wxString	strMessage,
								long		lStyle /* = wxOK | wxCENTRE */,
								wxWindow*	pParent /* = NULL */,
								wxString	strCaption /* = wxT( "" ) */,
								wxString	strBtn1Msg /* = wxT( "" ) */,
								wxString	strBtn2Msg /* = wxT( "" ) */,
								wxString	strBtn3Msg /* = wxT( "" ) */ )
{
	wxString strDlgCaption;

	// Composizione titolo dialog box
	strDlgCaption = GetAppName();
	if ( !strCaption.IsEmpty() )
		strDlgCaption += wxT( " - " ) + strCaption;

	//DF Spazio per modifica testi pulsanti

	wxMessageDialog	Dlg( pParent, strMessage, strDlgCaption, lStyle );

	return( Dlg.ShowModal() );
}

void CLDPCApp::GetFileVers(wxString* pstrFileVer, int* piFileVer)
{
	if (pstrFileVer)
		pstrFileVer->Printf(wxT("%d.%d.%d.%d%s"), m_iFileVers[0], m_iFileVers[1], m_iFileVers[2], m_iFileVers[3], "" );

	if (piFileVer)
	{
		for (int i = 0; i < 4; ++i)
			*piFileVer++ = m_iFileVers[i];
	}
}