#include "stdafx.h"
#include "LDPC-AM.h"

#include "MainFrm.h"
#include "MainPanel.h"
#include "Version.h"
#include "LDPCMan.h"


//////////////////////////////////////////////////////////////////////////////
//
// Costruttore(i) classe CMainFrame
//
CMainFrame::CMainFrame( const wxString& strTitle, const wxPoint& pos, const wxSize& size )
{
	//
	// Inizializzazione oggetto
	//
	m_pCLDPCMan = NULL;												// Punt. al gestore xppIPM


	wxSize size1( 1500, 950 );
// Rifare con controllo sulla dimensione video come fatto da holechecker

	long	lStyle = wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE;

	// Creazione finestra in due step ( il costruttore non richiama quello della classe base ma chiama solo la create )
	Create( NULL, IDW_MAINFRAME, strTitle, pos, size1, lStyle );
}


//
// Distruttore classe CMainFrame
//
CMainFrame::~CMainFrame()
{
	//
	// Deallocazione oggetti creati dinamicamente
	//
}


//
// CMainFrame event table
//
BEGIN_EVENT_TABLE( CMainFrame, wxFrame )
	EVT_WINDOW_CREATE( CMainFrame::__OnWindowCreate )
	EVT_CLOSE( CMainFrame::__OnWindowClose )
	EVT_ICONIZE( CMainFrame::__OnIconized )
	EVT_MENU(IDM_APP_EXIT, CMainFrame::__OnAppExit )
	EVT_MENU(IDM_APP_ABOUT, CMainFrame::__OnAppAbout )
END_EVENT_TABLE()


//
// Funzione:
//		void CMainFrame::__OnWindowCreate( wxWindowCreateEvent& Event )
//
// Descrizione:
//		EVT_WINDOW_CREATE event handler
//
// Parametri:
//		wxWindowCreateEvent	Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
void CMainFrame::__OnWindowCreate( wxWindowCreateEvent& Event )
{
	// L'evento puo' essere ricevuto anche durante la creazione delle finestre figlie
	if ( Event.GetWindow() == this )
	{
		int				iInitError = 0;
		//const CAppCfg*	pAppCfg = ::wxGetApp().GetAppCfg();
		wxString		strTmp;
		wxBoxSizer*		pTopSizer;

		do
		{
			//
			// Inizializz. generale
			//
			m_ui32ObjState = 0L;
			// Creazione sizer principale
			pTopSizer = new wxBoxSizer( wxVERTICAL );
			SetSizer( pTopSizer );


			// Carica icona applicazione
			SetIcon( wxIcon("IDR_APP_MAIN_ICON", wxBITMAP_TYPE_ICO_RESOURCE) );

			// Creazione status bar
			CreateStatusBar( 2 );

			// Centra la finestra a video
			CentreOnScreen();

			m_pCLDPCMan = new CLDPCMan( this );
			if ( ( iInitError = m_pCLDPCMan->InitObj() ) != 0 )
				break;

			m_pMainPanel = new CMainPanel(this, m_pCLDPCMan);
			pTopSizer->Add(m_pMainPanel, wxSizerFlags(1).Border().Expand());

			// Funzione terminata correttamente
			iInitError = 0;
		}
		while ( 0 );

		// Se si sono verificati degli errori termina l'applicazione
		if ( iInitError != 0 )
			__CloseWindow( NULL, true );
	}
}


//
// Funzione:
//		void CMainFrame::__OnWindowClose( wxCloseEvent& Event )
//
// Descrizione:
//		EVT_WINDOW_CLOSE event handler
//
// Parametri:
//		wxCloseEvent		Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
void CMainFrame::__OnWindowClose( wxCloseEvent& Event )
{
	__CloseWindow( &Event, false );
}


//
// Funzione:
//		void CMainFrame::__CloseWindow( wxCloseEvent* pEvent, bool bForcedClose )
//
// Descrizione:
//		Chusura finestra principale dell'applicazione
//
// Parametri:
//		wxCloseEvent*	pEvent				Punt. ai dati evento; puo' essere NULL
//		bool			bForcedClose		Flag chiusura forzata
//
// Output:
//		Nessuno
//
void CMainFrame::__CloseWindow( wxCloseEvent* pEvent, bool bForcedClose )
{
	bool	bExitAllowed = false;
	bool	bForcedExit = false;

	do
	{
		// Chiusura forzata ?
		if ( bForcedClose )
		{
			bExitAllowed = bForcedExit = true;
			break;
		}

		// Spazio per altri controlli

		// Permetti l'uscita dall'applicazione
		bExitAllowed = true;
	}
	while ( 0 );

	do
	{
		// Uscita dall'applicazione non permessa, se possibile blocca la procedura di chiusura
		if ( !bExitAllowed )
		{
			if ( pEvent->CanVeto() )
			{
				// Notify the calling code that we didn't delete the frame.
				pEvent->Veto();
				break;
			}
			else
				bForcedExit = true;
		}

		//
		// Uscita dall'applicazione
		//
		// Oggetto già in chiusura 
		//
		if( m_ui32ObjState & OS_CLOSING )
			break;

		m_ui32ObjState |= OS_CLOSING;

		// Distruzione gestore xppIPM
		if ( m_pCLDPCMan )
		{
			m_pCLDPCMan->CleanupObj();
			delete m_pCLDPCMan;
			m_pCLDPCMan = NULL;
		}

		// Spazio per eventuale salvataggio dati

		// Distruzione finestra
		this->Destroy();
	}
	while ( 0 );
}


//
// Funzione:
//		void CMainFrame::__OnIconized( wxIconizeEvent& Event )
//
// Descrizione:
//		Evento richiamato quando la finestra deve essere ridotta a icona o ripristinata
//
// Parametri:
//		wxIconizeEvent&		Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
void CMainFrame::__OnIconized( wxIconizeEvent& Event )
{
	// Normalmente questo evento non e' gestito, ma in questa applicazione invece di ridurre a icona
	// la finestra lasciandone il pulsante sulla taskbar, la finestra viene nascosta e ne viene
	// solo lasciata l'icona nella parte dx della taskbar
	//if ( Event.IsIconized() )
	//	Show( false );
	//else
	//	Show( true );

	//// Termina la gestione dell'evento
	////
	//// If true, the event processing system continues searching for a further handler function for this event,
	//// even though it has been processed already in the current handler
	//Event.Skip( true );
}


//
// Funzione:
//		void CMainFrame::__OnAppExit( wxCommandEvent& WXUNUSED( Event ) )
//
// Descrizione:
//		IDM_APP_EXIT menu command handler
//
//		Comando per uscire dall'applicazione
//
// Parametri:
//		wxCommandEvent&		Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
void CMainFrame::__OnAppExit( wxCommandEvent& WXUNUSED( Event ) )
{
    Close( false );
}


//
// Funzione:
//		void CMainFrame::__OnAppAbout( wxCommandEvent& WXUNUSED( Event ) )
//
// Descrizione:
//		IDM_ABOUT menu command handler
//
//		Comando per visualizz. dialog "Informazioni sull'applicazione"
//
// Parametri:
//		wxCommandEvent&		Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
// Note:
//		Vedi InitAboutInfoAll() in samples\dialogs\dialogs.cpp come esempio per
//		l'utilizzo delle varie opzioni disponibili
//
void CMainFrame::__OnAppAbout( wxCommandEvent& WXUNUSED( Event ) )
{
    wxAboutDialogInfo	DlgInfo;
	wxString			strTmp, strFileVersion;

    DlgInfo.SetName(VER_PRODUCTNAME_STR);

	::wxGetApp().GetFileVers(&strFileVersion, NULL);
	strTmp.Printf( wxT( "Release %s" ), strFileVersion );
    DlgInfo.SetVersion( strTmp, strTmp );

    DlgInfo.SetDescription( wxT( "LDPC-AM Simulator\n\n" ) );

	DlgInfo.SetCopyright( wxT( "Copyright (C) Andrea Montanari.\n" ) );

	wxAboutBox( DlgInfo, this );
}