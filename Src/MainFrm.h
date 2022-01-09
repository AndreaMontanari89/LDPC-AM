#ifndef __MAINFRM_H__
#define __MAINFRM_H__
#pragma once

class CLDPCMan;

//
// Classe finestra principale dell'applicazione
//
class CMainFrame : public wxFrame
{
	// Costruttore(i) / distruttore
public:
    CMainFrame( const wxString& strTitle, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );
    ~CMainFrame();

	//
	// Enum, strutture, classi interne all'oggetto
	//
	enum _MainBookCtrlPageID										// Identif. pagine book-ctrl principale
	{
		MBCPI_Counter												// Inserire eventuali altri identificatori sopra questa riga
	};

	enum _ObjState													// Stato oggetto
	{												
		OS_CLOSING						= 0x00000001,				// MainFrm in chiusura
	};

	//
	// Funzioni
	//
public:
	CLDPCMan*		GetCLDPCMan( void )								{ return( m_pCLDPCMan); };

protected:
	void			__CloseWindow( wxCloseEvent* pEvent, bool bForcedClose );
	void			__OnAppAbout( wxCommandEvent& Event );
	void			__OnAppExit( wxCommandEvent& Event );
	void			__OnIconized( wxIconizeEvent& Event );
	void			__OnWindowClose( wxCloseEvent& Event );
	void			__OnWindowCreate( wxWindowCreateEvent& Event );

	//
	// Variabili
	//
protected:

	CLDPCMan*			m_pCLDPCMan;								// Punt. al gestore modulo

	uint32_t			m_ui32ObjState;								// Stato oggetto vedi enum CMainFrm::_ObjState

//DF//DF	CDbgView*			m_pDbgView[1];									// Punt. alle finestre per la visualizz. immagini di debug
//DF//DF	int					m_iNumDbgView;									// Num. di finestre presenti in m_pDbgView

private:
	// Object that wants to respond to events
	DECLARE_EVENT_TABLE()
};
#endif