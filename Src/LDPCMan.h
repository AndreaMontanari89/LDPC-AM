#ifndef __IPM_MAN_H__
#define __IPM_MAN_H__
#pragma once

#include "xppCircularQueue.h"

//class CAppCfg;
class CMainFrame;
//
// Classe gestore xppIPM
//
class CLDPCMan : public wxEvtHandler
{
	// Costruttore(i) / distruttore
public:
	CLDPCMan(CMainFrame* pParent);
	~CLDPCMan();

	//
	// Enum, strutture, classi interne all'oggetto
	//
public:
	enum _ObjState													// Stato oggetto
	{
		OS_INITIALIZING = 0x00000001,					// Inizializzazione oggetto in corso
		OS_INITIALIZED = 0x00000002,					// Oggetto inizializzato
		OS_CLEANINGUP = 0x00000004,						// Cleanup oggetto in corso
		OS_RESET_MODULE_PENDING = 0x00000008,			// Reset modulo pendente
		OS_RESETTING_MODULE = 0x00000010,				// Reset modulo in corso
	};

	enum __InitCleanupObjMode										// Modalita' di inizializzazione oggetto
	{
		ICOM_Standard,
		ICOM_ResetModuleReqFromMCU,
	};

	enum __MainProcThreadQueuedEventID								// Eventi gestiti dal thread principale di elaborazione
	{
		MPTEQI_Timeout = 0,											// Timeout ( solo uso interno )
		MPTEQI_MCU_Msg,												// Messaggio ricevuto dall'unita' di controllo moduli
		MPTEQI_NewAcquiredFrame,									// Avvenuta acquisizione di un nuovo frame
		MPTEQI_KeyboardInput,										// Avvenuta inserimento di dati da tastiera
		MPTEQI_CheckScan,											// Controllo scansioni
	};

	//
	// Funzioni
	//
public:
	int						CleanupObj(void);
	int						InitObj(void);

	void					OnInitObj(wxCommandEvent& Event);
	void					OnKeyboardInput(wxString strInput);
	int						PostMainProcThreadEvent(int iEvent);

	int						alist2cvMat(std::string strFileName, cv::Mat& pMat	);

protected:
	void				__OnTimer(wxTimerEvent& Event);
	void				__CleanupObj(int iCleanupObjMode);
	int					__InitObj(int iInitObjMode);
	void 				__HandleCommandKeyboardInput(const wxString& strInputCommand);
	void				__HandleKeyboardInput(wxString strKeyboardInput);
	static DWORD WINAPI	__MainProcessThreadStub(void* pParam);
	DWORD				__MainProcessThread(void* pParam);
	uint32_t			GetObjState(void);
	int					__SetObjState(uint32_t ui32NewObjState);



public:

	//
	// Variabili
	//
protected:
	CMainFrame* m_pParent;								// Punt. all'oggetto genitore
//	const CAppCfg* m_pAppCfg;								// Punt. alla configurazione applicazione
	uint32_t				m_ui32ObjState;							// Stato oggetto; vedi enum _ObjState
	wxCriticalSection		m_csObjStateMutex;						// Critical section per l'accesso esclusivo allo stato oggetto
	std::list<wxString>		m_KeyboardInput;						// Lista dati inseriti da tastiera
	wxCriticalSection		m_csKeyboardInputMutex;					// Critical section per l'accesso esclusivo a m_KeyboardInput

	struct __MainProcessThread										// Dati thread principale di elaborazione
	{
		HANDLE		m_hHandle;										// Handle thread
		DWORD		m_dwThreadId;									// Id thread
		HANDLE		m_hQueuedEventEvent;							// Evento 'evento in coda eventi'
		HANDLE		m_hQuitThreadEvent;								// Evento per terminare il thread

		CxppCircolarQueue<int>	m_EventQueue;						// Coda eventi da gestire
		enum { m_iEventQueueSize = 2048 };
		wxCriticalSection		m_csEventQueueMutex;				// Critical section per l'accesso esclusivo a m_EventQueue

	} m_MainProcThread;

private:
	// Object that wants to respond to events
	DECLARE_EVENT_TABLE()
};

//
// Funzione:
//		uint32_t CLDPCMan::GetObjState( void )
//		int CLDPCMan::__SetObjState( uint32_t ui32NewObjState )
//
// Descrizione:
//		Funzioni per lettura / scrittura stato oggetto
//
// Parametri:
//		Vedi codice
//
// Output:
//		Vedi codice
//
inline uint32_t CLDPCMan::GetObjState(void)
{
	uint32_t				ui32State;
	wxCriticalSectionLocker	Locker(m_csObjStateMutex);

	ui32State = m_ui32ObjState;

	return(ui32State);
}

inline int CLDPCMan::__SetObjState(uint32_t ui32NewObjState)
{
	int						iFuncRetVal = 0;
	wxCriticalSectionLocker	Locker(m_csObjStateMutex);

	do
	{
		// Impostazione nuovo stato oggetto
		m_ui32ObjState = ui32NewObjState;

		// Funzione terminata correttamente
		iFuncRetVal = 0;
	} while (0);

	return(iFuncRetVal);
}

#endif	// __IPM_MAN_H__
