#include "stdafx.h"
#include "LDPC-AM.h"

//#include "AppCfg.h"
#include "MainFrm.h"
#include "LDPCMan.h"
#include "AppEvents.h"
#include "Utils.h"
#include "TannerGraph.h"
#include "LDPC-codes-Lib.h"


//////////////////////////////////////////////////////////////////////////////
//
// Costruttore(i) classe CLDPCMan
//
CLDPCMan::CLDPCMan(CMainFrame* pParent)
{
	//
	// Inizializzazione oggetto
	//
	m_pParent = pParent;											// Punt. all'oggetto genitore
	//m_pAppCfg = ::wxGetApp().GetAppCfg();							// Punt. alla configurazione applicazione
	m_ui32ObjState = 0;												// Stato oggetto; vedi enum _ObjState
	// Tutte le restanti inizializzazioni sono effettuate nella __InitObj()
}


//
// Distruttore classe CLDPCMan
//
CLDPCMan::~CLDPCMan()
{
	// Pulizia automatica oggetto nel caso in cui non sia stata chiamata la CleanupObj()
	CleanupObj();
}


//
// CLDPCMan event table
//
BEGIN_EVENT_TABLE(CLDPCMan, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, wxEVT_APP_INIT_LDPC_OBJ, CLDPCMan::OnInitObj)
END_EVENT_TABLE()

//
// Funzione:
//		void CMCU_Itf::__OnTimer( wxTimerEvent& Event )
//
// Descrizione:
//		Gestore eventi timer
//
// Parametri:
//		wxTimerEvent&		Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
void CLDPCMan::__OnTimer(wxTimerEvent& Event)
{
	int	iTimerID = Event.GetTimer().GetId();

	switch (iTimerID)
	{
	default:
		break;
	}
}

//
// Funzione:
//		int CLDPCMan::InitObj( void )
//
// Descrizione:
//		Inizializzazione oggetto
//
// Parametri:
//		Nessuno
//
// Output:
//		ErrOk se la funzione e' terminata correttamente, altrimenti il codice dell'errore avvenuto
//
int CLDPCMan::InitObj(void)
{
	int	iFuncRetVal = 1;

	do
	{
		// Controllo stato oggetto
		if (GetObjState())
			break;

		// Post evento inizializzazione oggetto
		wxCommandEvent Event(wxEVT_APP_INIT_LDPC_OBJ, wxID_ANY);
		QueueEvent(Event.Clone());

		// Imposta il nuovo stato oggetto ed esci correttamente
		__SetObjState(OS_INITIALIZING);
		iFuncRetVal = 0;
	} while (0);

	return(iFuncRetVal);
}


//
// Funzione:
//		void CLDPCMan::OnInitObj( wxCommandEvent& Event )
//
// Descrizione:
//		wxEVT_APP_INIT_XPPIPM_OBJ event handler
//
// Parametri:
//		wxCommandEvent		Event			Riferim. ai dati evento
//
// Output:
//		Nessuno
//
void CLDPCMan::OnInitObj(wxCommandEvent& WXUNUSED(Event))
{
	do
	{
		// Controllo stato oggetto
		if (GetObjState() != OS_INITIALIZING)
			break;

		// Inizializzazione oggetto
		if (__InitObj(ICOM_Standard) != 0)
			break;
		// Imposta il nuovo stato oggetto
		__SetObjState(OS_INITIALIZED);

	} while (0);
}


//
// Funzione:
//		int CLDPCMan::__InitObj( int iInitObjMode )
//
// Descrizione:
//		Inizializzazione oggetto
//
// Parametri:
//		int			iInitObjMode			Modalita' di inizializzazione oggetto; vedi enum __InitCleanupObjMode
//
// Output:
//		Nessuno
//
int CLDPCMan::__InitObj(int iInitObjMode)
{
	int			iFuncRetVal = 0;
	wxString	strTmp;

	do
	{
		//
		// Inizializzazione generale variabili oggetto prima di qualsiasi altro test
		//

		m_KeyboardInput.clear();									// Lista dati inseriti da tastiera

		// Dati thread principale di elaborazione
		m_MainProcThread.m_hHandle = NULL;							// Handle thread
		m_MainProcThread.m_dwThreadId = 0;							// Id thread
		m_MainProcThread.m_hQueuedEventEvent = NULL;				// Evento 'evento in coda eventi'
		m_MainProcThread.m_hQuitThreadEvent = NULL;					// Evento per terminare il thread
		m_MainProcThread.m_EventQueue.clear();						// Coda eventi da gestire

		//
		// Inizializz. variabili varie
		//

		//
		// Creazione thread principale di elaborazione
		//
		// Inizializzazione coda eventi da gestire
		m_MainProcThread.m_EventQueue.reserve(m_MainProcThread.m_iEventQueueSize);

		// Evento 'evento in coda eventi' ( con reset manuale )
		m_MainProcThread.m_hQueuedEventEvent = ::CreateEvent(NULL, true, false, NULL);

		// Evento per terminare il thread
		m_MainProcThread.m_hQuitThreadEvent = ::CreateEvent(NULL, false, false, NULL);

		// Creazione thread principale di elaborazione
		m_MainProcThread.m_hHandle = ::CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)CLDPCMan::__MainProcessThreadStub,
			(void*)this,
			0,
			&m_MainProcThread.m_dwThreadId);

		cv::Mat x;

		// Creazione matriche di controllo di parità tramite alist file
		alist2cvMat("C:\\Develop\\LDPC-Codes\\test\\H.alist.txt", x);

		int block_size = x.cols;
		int word_size = x.cols-x.rows;

		/*Parte codifica e trasmissione*/

		// Creazione matriche di controlo di parità tramite alist file
		alist2pchk((char*)"C:\\Develop\\LDPC-Codes\\test\\H.alist.txt", (char*)"C:\\Develop\\LDPC-Codes\\test\\H.pchk");
		//Creazione della matrice generatrice
		make_gen((char*)"C:\\Develop\\LDPC-Codes\\test\\H.pchk", (char*)"C:\\Develop\\LDPC-Codes\\test\\G.Gen", (char*)"dense", NULL, NULL, NULL, NULL );
		//Codifica del file di input
		encode((char*)"C:\\Develop\\LDPC-Codes\\test\\H.pchk", (char*)"C:\\Develop\\LDPC-Codes\\test\\G.Gen", (char*)"C:\\Develop\\LDPC-Codes\\test\\input2.txt", (char*)"C:\\Develop\\LDPC-Codes\\test\\enc");
		//Trasmissione sul canale
		transmit((char*)"C:\\Develop\\LDPC-Codes\\test\\enc", (char*)"C:\\Develop\\LDPC-Codes\\test\\rec", (char*)"1", (char*)"awgn", (char*)"0.8");

		CTannerGraph*	mainGraph = new CTannerGraph(x);
		wxArrayString	wxasWords;
		wxArrayString	wxasRX;

		if (!wxFile::Exists("C:\\Develop\\LDPC-Codes\\test\\rec"))
			break;

		wxTextFile	tf("C:\\Develop\\LDPC-Codes\\test\\rec");
		wxString	strApp;
		wxString	cw;
		tf.Open();

		strApp = tf.GetFirstLine();

		if (strApp != wxEmptyString)
		{
			std::vector<double> channel = DoubleVectorFromString(strApp);
			cw = mainGraph->Decode(channel, 0.8, x, 1000 );

			wxasWords.push_back(cw);
			wxasRX.push_back(cw.Right(word_size));

			do
			{
				strApp = tf.GetNextLine();

				if (strApp == wxEmptyString)
					break;

				std::vector<double> channel = DoubleVectorFromString(strApp);
				cw = mainGraph->Decode(channel, 0.8, x, 1000);

				wxasWords.push_back(cw);
				wxasRX.push_back(cw.Right(word_size));

			} while (1);
		}

		delete mainGraph;

		// Funzione terminata correttamente
		iFuncRetVal = 0;
	} while (0);

	return(iFuncRetVal);
}

//
// Funzione:
//		int CLDPCMan::CleanupObj( void )
//
// Descrizione:
//		Cleanup oggetto
//
// Parametri:
//		Nessuno
//
// Output:
//		ErrOk se la funzione e' terminata correttamente, altrimenti il codice dell'errore avvenuto
//
int CLDPCMan::CleanupObj(void)
{
	int			iFuncRetVal = 1;
	uint32_t	ui32CurrObjState;

	do
	{
		// Controllo stato oggetto
		if ((ui32CurrObjState = GetObjState()) == 0)
			break;
		__SetObjState(ui32CurrObjState | OS_CLEANINGUP);

		// Cleanup oggetto
		__CleanupObj(ICOM_Standard);

		// Imposta il nuovo stato oggetto ed esci correttamente
		__SetObjState(0);
		iFuncRetVal = 0;
	} while (0);

	return(iFuncRetVal);
}


//
// Funzione:
//		void CLDPCMan::__CleanupObj( int iCleanupObjMode )
//
// Descrizione:
//		Cleanup oggetto
//
// Parametri:
//		int			iCleanupObjMode			Modalita' di cleanup oggetto; vedi enum __InitCleanupObjMode
//
// Output:
//		Nessuno
//
void CLDPCMan::__CleanupObj(int iCleanupObjMode)
{

	do
	{

			// Blocco try catch per evitare runtime error che farebbe apparire dialog e blocca il Restart.
			// Causo volutamente un crash che provoca il restart
			//
	

				//
				// Termina il thread principale di elaborazione
				//
				if (m_MainProcThread.m_hHandle && m_MainProcThread.m_hQuitThreadEvent)
				{
					::SetEvent(m_MainProcThread.m_hQuitThreadEvent);
					::WaitForSingleObject(m_MainProcThread.m_hHandle, INFINITE);
					::CloseHandle(m_MainProcThread.m_hHandle);
					::CloseHandle(m_MainProcThread.m_hQueuedEventEvent);
					::CloseHandle(m_MainProcThread.m_hQuitThreadEvent);

					m_MainProcThread.m_EventQueue.clear();					// Coda eventi da gestire
				}
				m_MainProcThread.m_hHandle = NULL;							// Handle thread
				m_MainProcThread.m_dwThreadId = 0;							// Id thread
				m_MainProcThread.m_hQueuedEventEvent = NULL;				// Evento 'evento in coda eventi'
				m_MainProcThread.m_hQuitThreadEvent = NULL;					// Evento per terminare il thread

				//
				// Lista dati inseriti da tastiera
				//
				m_KeyboardInput.clear();

	} while (0);
}




//
// Funzione:
//		void CLDPCMan::__HandleKeyboardInput( wxString strKeyboardInput )
//
// Descrizione:
//		Gestione dati inseriti da tastiera
//
// Parametri:
//		wxString	strKeyboardInput		Stringa dati inseriti
//
// Output:
//		Nessuno
//
// Note:
//		Funzione richiamata dal thread principale di elaborazione
//
void CLDPCMan::__HandleKeyboardInput(wxString strKeyboardInput)
{
	do
	{
		// Controllo parametro
		strKeyboardInput.Trim(false).Trim(true);
		if (strKeyboardInput.IsEmpty())
			break;

		// Debug
		// APP_TRACE( TL_Min, NULL, NULL, __FUNCTION__, 0, "HandleKeyboardInput: %s", const_cast<char*> ( (const char*) strKeyboardInput.mb_str( wxConvUTF8 ) ) );

		// Controllo flag gestione dati inseriti da tastiera in modalita' comando o carattere singolo
		__HandleCommandKeyboardInput(strKeyboardInput);
	} while (0);
}


//
// Funzione:
//		void CLDPCMan::__HandleCommandKeyboardInput( const wxString &strInputCommand )
//
// Descrizione:
//		Gestione comando inserito da tastiera
//
// Parametri:
//		wxString	strInputCommand				Comando inserito
//
// Output:
//		Nessuno
//
void CLDPCMan::__HandleCommandKeyboardInput(const wxString& strInputCommand)
{
	int				iSrcIdx, iDstIdx, iStartIdx, iEndIdx;
	bool			bQuotationMark;
	wxArrayString	strCmdArg;
	wxString		strTmp, strTmp1;
	int				iArgCntr = 0;
	bool			bHandledCommand = true;
	bool			bCmdErr = false;

	wxString		strCommand = strInputCommand;

	do
	{
		// Controllo parametro
		strCommand.Trim(false).Trim(true);
		if (strCommand.IsEmpty())
			break;

		//
		// Compattazione spazi tra gli argomenti del comando tranne quelli tra apici
		// ( mantenuto l'algoritmo per compatibilita' )
		//
		bQuotationMark = false;
		iSrcIdx = iDstIdx = 0;
		while (iSrcIdx < (int)strCommand.Len())
		{
			if (strCommand[iDstIdx] == (wxChar)'"')
				bQuotationMark = !bQuotationMark;

			if (bQuotationMark)
				strCommand.SetChar(iDstIdx++, strCommand[iSrcIdx]);
			else
			{
				if (strCommand[iSrcIdx] == (wxChar)' ')
				{
					if (strCommand[iDstIdx - 1] != (wxChar)' ')
						strCommand.SetChar(iDstIdx++, strCommand[iSrcIdx]);
				}
				else
					strCommand.SetChar(iDstIdx++, strCommand[iSrcIdx]);
			}

			++iSrcIdx;
		}
		if (strCommand[iDstIdx - 1] == (wxChar)' ')
			--iDstIdx;
		strCommand.Truncate(iDstIdx);

		//
		// Suddivisione stringa strCommand in argomenti ( mantenuto algoritmo per compatibilita' )
		//
		bQuotationMark = false;
		strCmdArg.Empty();

		for (iStartIdx = iEndIdx = 0; iEndIdx < (int)strCommand.Len(); ++iEndIdx)
		{
			if (!bQuotationMark)
			{
				if (strCommand[iEndIdx] == (wxChar)' ')
				{
					strCmdArg.Add(strCommand.Mid(iStartIdx, iEndIdx - iStartIdx));
					iStartIdx = ++iEndIdx;
				}
			}

			if (iEndIdx < (int)strCommand.Len())
				if (strCommand[iEndIdx] == (wxChar)'"')
					bQuotationMark = !bQuotationMark;
		}
		strCmdArg.Add(strCommand.Mid(iStartIdx, iEndIdx - iStartIdx));
		iArgCntr = strCmdArg.Count();

		// Echo comando
		strTmp.Printf(wxT("%s\n"), strCommand);
//		m_pParent->ConsolePrintf(strTmp);

		//
		// Analisi comando(i)
		//
		do
		{
			if (strCmdArg[0].CmpNoCase(wxT("Quit")) == 0 || strCmdArg[0].CmpNoCase(wxT("Kill")) == 0)
			{
				//
				// Uscita applicazione
				//
				// Simula la pressione della voce di menu' "Uscita applicazione"
				wxCommandEvent Event(wxEVT_COMMAND_MENU_SELECTED, IDM_APP_EXIT);
				::wxGetApp().GetMainWnd()->GetEventHandler()->QueueEvent(Event.Clone());
				break;
			}

			if (strCmdArg[0].CmpNoCase(wxT("Help")) == 0 || strCmdArg[0].CmpNoCase(wxT("?")) == 0)
			{
				//m_pParent->ConsolePrintf(wxT("---------------------------------------\n"));
				//m_pParent->ConsolePrintf(wxT("Quit, Kill    - Quit\n"));
				//m_pParent->ConsolePrintf(wxT("Ver           - Version\n"));
				//m_pParent->ConsolePrintf(wxT("Help, ?       - Help\n"));
				//m_pParent->ConsolePrintf(wxT("\n"));

				break;
			}


		bHandledCommand = false;
	} while (0);

	//
	// Comando sconosciuto
	//
	if (!bHandledCommand && !strCmdArg[0].IsEmpty())
	{
		strTmp.Printf(wxT("Unknown Command: %s\n"), strCmdArg[0]);
		//m_pParent->ConsolePrintf(strTmp);
	}

	//
	// Prompt per il prossimo comando
	//
	//m_pParent->ConsolePrintf(wxT("Cmd>"));
}
while (0);
}


//
// Funzione:
//		void CLDPCMan::__MainProcessThread( void* pParam )
//
// Descrizione:
//		Funzione main del thread principale di elaborazione
//
// Parametri:
//		Vedi codice
//
// Output:
//		Vedi codice
//
DWORD WINAPI CLDPCMan::__MainProcessThreadStub(void* pParam)
{
	return(((CLDPCMan*)pParam)->__MainProcessThread(pParam));
}

DWORD CLDPCMan::__MainProcessThread(void* pParam)
{
	UNREFERENCED_PARAMETER(pParam);

	enum __WakeEvents
	{
		WE_QuitThread = 0,
		WE_QueuedEvent,

		WE_Counter
	};

	HANDLE		hWakeEvents[WE_Counter];
	DWORD		dWakeTimeout = 5;
	DWORD		dwWakeEvent;
	int			iWakeEvent, iQueuedEvent;
	wxString	strKeyboardInput;

	//
	// Imposta gli eventi che possono risvegliare il thread
	//
	hWakeEvents[WE_QuitThread] = m_MainProcThread.m_hQuitThreadEvent;		// Evento per terminare il thread
	hWakeEvents[WE_QueuedEvent] = m_MainProcThread.m_hQueuedEventEvent;		// Evento 'evento in coda eventi' ( con reset manuale )

	try
	{
		//
		// Loop per gestione eventi
		//
		while (true)
		{
			// Attesa evento(i)
			dwWakeEvent = ::WaitForMultipleObjects(WE_Counter, hWakeEvents, false, dWakeTimeout);

			try
			{
				// Determina quale evento ha svegliato il thread
				if (dwWakeEvent >= WAIT_OBJECT_0 && dwWakeEvent < WAIT_OBJECT_0 + WE_Counter)
				{
					iWakeEvent = dwWakeEvent - WAIT_OBJECT_0;
					if (iWakeEvent == WE_QuitThread)
					{
						//
						// Evento per terminare il thread
						//
						break;
					}
					else if (iWakeEvent == WE_QueuedEvent)
					{
						//
						// Evento 'evento in coda eventi'
						//
						// Ottieni l'accesso esclusivo alla coda eventi gestiti dal thread principale di elaborazione
						m_MainProcThread.m_csEventQueueMutex.Enter();

						// Preleva l'evento dalla coda eventi
						if (!m_MainProcThread.m_EventQueue.empty())
						{
							iQueuedEvent = m_MainProcThread.m_EventQueue.front();
							m_MainProcThread.m_EventQueue.pop();

							// Se la coda non e' vuota, lascia m_MainProcThread.m_hWakeUpEvent nello stato di 'segnalato'
							// in modo che al prossimo loop sia subito gestito il prossimo evento in coda
							if (m_MainProcThread.m_EventQueue.empty())
								::ResetEvent(m_MainProcThread.m_hQueuedEventEvent);
						}
						else
						{
							// Non deve mai succedere
							wxASSERT(false);
							iQueuedEvent = MPTEQI_Timeout;
							::ResetEvent(m_MainProcThread.m_hQueuedEventEvent);
						}

						// Rilascia l'accesso esclusivo alla coda eventi
						m_MainProcThread.m_csEventQueueMutex.Leave();
					}
					else
						wxASSERT(false);
				}
				else if (dwWakeEvent == WAIT_TIMEOUT)
				{
					// Uscita dalla wait per timeout
					iQueuedEvent = MPTEQI_Timeout;
				}
				else
				{
					wxASSERT(false);										// WAIT_FAILED, WAIT_ABANDONED_0, ...
					break;
				}
			}
			catch (...)
			{
				throw 0;
			}

			if (iQueuedEvent == MPTEQI_KeyboardInput)
			{
				try
				{
					//
					// Avvenuta inserimento di dati da tastiera
					//
					// Preleva e gestisci il dato piu' vecchio presente in lista
					m_csKeyboardInputMutex.Enter();
					if (!m_KeyboardInput.empty())
					{
						strKeyboardInput = m_KeyboardInput.front();
						m_KeyboardInput.pop_front();
					}
					else
					{
						strKeyboardInput.Clear();
						wxASSERT(false);									// Non deve mai succedere
					}
					m_csKeyboardInputMutex.Leave();

					__HandleKeyboardInput(strKeyboardInput);
				}
				catch (...)
				{
					throw 3;
				}
			}
			else
			{
				// Avvenuta uscita dalla wait per timeout
				wxASSERT(iQueuedEvent == MPTEQI_Timeout);
			}
		}
	}
	catch (...)
	{
	}

	return((DWORD)0);
}


//
// Funzione:
//		void CLDPCMan::OnKeyboardInput( wxString strInput )
//
// Descrizione:
//		Evento ricezione dati inseriti da tastiera
//
// Parametri:
//		wxString	strInput				Stringa dati inseriti
//
// Output:
//		Nessuno
//
void CLDPCMan::OnKeyboardInput(wxString strInput)
{
	uint32_t	ui32CurrObjState;
	int			iRC;

	do
	{
		// Controlla stato oggetto per prevenire l'inserimento di dati durante la cleanup dell'oggetto
		if (!((ui32CurrObjState = GetObjState()) & OS_INITIALIZED))
			break;
		if (ui32CurrObjState & (OS_CLEANINGUP | OS_RESET_MODULE_PENDING | OS_RESETTING_MODULE))
			break;

		if (!strInput.IsEmpty())
		{
			// Inserisci la stringa ricevuta nella lista dati inseriti da tastiera
			m_csKeyboardInputMutex.Enter();

			m_KeyboardInput.push_back(strInput);

			// Notifica al thread principale di elaborazione l'avvenuto inserimento di dati da tastiera
			if ((iRC = PostMainProcThreadEvent(CLDPCMan::MPTEQI_KeyboardInput)) != 0)
			{

				// Errore in notifica -> rimuove l'elemento in lista
				m_KeyboardInput.pop_back();
			}

			m_csKeyboardInputMutex.Leave();
		}
	} while (0);
}

//
// Funzione:
//		int CLDPCMan::PostMainProcThreadEvent( int iEvent )
//
// Descrizione:
//		Inserisci un nuovo evento nella coda eventi gestiti dal thread principale di elaborazione
//
// Parametri:
//		int			iEvent					Identif. evento; vedi enum __MainProcThreadQueuedEventID
//
// Output:
//		ErrOk se la funzione e' terminata correttamente, altrimenti il codice dell'errore avvenuto
//
int CLDPCMan::PostMainProcThreadEvent(int iEvent)
{
	int			iFuncRetVal = 1;
	uint32_t	ui32CurrObjState;

	do
	{
		// Controlla lo stato dell'oggetto per prevenire l'inserimento di eventi da parte di altri
		// thread durante la cleanup o il reset dell'oggetto
		if (!((ui32CurrObjState = GetObjState()) & OS_INITIALIZED))
			break;
		if (ui32CurrObjState & (OS_CLEANINGUP | OS_RESET_MODULE_PENDING | OS_RESETTING_MODULE))
			break;

		// Ottieni l'accesso esclusivo alla coda eventi gestiti dal thread principale di elaborazione
		m_MainProcThread.m_csEventQueueMutex.Enter();

		// Verifica se la coda e' piena
		if (m_MainProcThread.m_EventQueue.size() < m_MainProcThread.m_EventQueue.max_size())
		{
			// Inserimento del nuovo evento in coda eventi
			m_MainProcThread.m_EventQueue.push(iEvent);

			// Segnala la presenza di un nuovo evento
			::SetEvent(m_MainProcThread.m_hQueuedEventEvent);

			// Funzione terminata correttamente
			iFuncRetVal = 0;
		}
		else
			iFuncRetVal = 1;

		// Rilascia l'accesso esclusivo alla coda eventi
		m_MainProcThread.m_csEventQueueMutex.Leave();
	} while (0);

	return(iFuncRetVal);
}

int	CLDPCMan::alist2cvMat(std::string strFileName, cv::Mat& pMat)
{
	int N, M;
	int iRetCode = 1;	
	std::vector<int>	vMDirWeight;
	std::vector<int>	vNDirWeight;

	do
	{
		if (!wxFile::Exists(strFileName))
			break;

		wxTextFile	tf(strFileName);
		wxString	strApp;
		wxString	strAccumulator = wxEmptyString;

		tf.Open();
		strApp = tf.GetFirstLine();
		wxSscanf(strApp,"%d %d", &N, &M);

		// Linea col massimo dei pase lung le direzioni M, N per ora non mi serve
		tf.GetNextLine();

		// Pesi nella direzione M
		strApp = tf.GetNextLine();		
		vMDirWeight = IntVectorFromString(strApp);

		// Pesi nella direzione N
		strApp = tf.GetNextLine();
		vNDirWeight = IntVectorFromString(strApp);

		// Azzermento matrice H
		pMat = cv::Mat::zeros(M, N, CV_32FC1);

		//
		for (uint i = 0; i < vMDirWeight.size(); i++)
		{
			strApp = tf.GetNextLine();			
			std::vector<int> vRow = IntVectorFromString(strApp);

			for( uint j = 0; j < vRow.size(); j++ )
				if(vRow[j] != 0)
					pMat.at<float>(cv::Point(i, vRow[j]-1)) = 1.0;
		}

		cv::imwrite("H.bmp", pMat);
#ifdef MATLAB
		using namespace matlab::engine;

		wxString strstr = "H=[";
		for (int r = 0; r < pMat.rows; r++)
		{
			strstr += "";
			for (int c = 0; c < pMat.cols; c++)
			{
				strstr += wxString::Format("%d ", (int)(pMat.at<int32_t>(r, c)));
			}
			strstr.erase(strstr.Len() - 1);
			strstr += ";";
		}
		strstr.erase(strstr.Len() - 1);
		strstr += "];";

		// Start MATLAB engine synchronously
		//std::vector<String> s = findMATLAB();

		std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();
		matlab::data::ArrayFactory factory;		

		// Pass vector containing 2 scalar args in vector    
		std::vector<matlab::data::Array> args({

			factory.createArray({ (unsigned long long)pMat.rows,(unsigned long long)pMat.cols }, (const int32_t*)pMat.datastart, (const int32_t*)pMat.dataend)
		});

		// Call MATLAB function and return result
		matlab::data::Array result;
		try
		{
			matlab::data::ArrayDimensions ard = args[0].getDimensions();
			result = matlabPtr->feval(u"make_gen", args);
			matlab::data::Array field = matlabPtr->getProperty(result, "x");

			int32_t* p = new int32_t(sizeof(int32_t)*field.getNumberOfElements());
			cv::Mat X = cv::Mat(ard[0], ard[1], CV_32SC1);
			for (int h = 0; h < ard[0]; h++)
			{
				for (int k = 0; k < ard[1]; k++)
				{
					int32_t ee = (field[h][k]);
					X.at<int32_t>(h, k) = ee;
				}
			}
			
			cv::imwrite("X.bmp", X);
		}
		catch (matlab::Exception& e)
		{
			wxString g = e.what();
			g = g;
		}
#endif

	} while (0);

	return iRetCode;
}
