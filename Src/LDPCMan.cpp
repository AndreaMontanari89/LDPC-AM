#include "stdafx.h"
#include "LDPC-AM.h"

//#include "AppCfg.h"
#include "MainFrm.h"
#include "LDPCMan.h"
#include "AppEvents.h"
#include "Utils.h"
#include "TannerGraph.h"
#include "LDPC-codes-Lib.h"
#include "MainPanel.h"


float my_logf(float);

/* compute inverse error functions with maximum error of 2.35793 ulp */
float my_erfinvf(float a)
{
	float p, r, t;
	t = fmaf(a, 0.0f - a, 1.0f);
	t = my_logf(t);
	if (fabsf(t) > 6.125f) { // maximum ulp error = 2.35793
		p = 3.03697567e-10f; //  0x1.4deb44p-32 
		p = fmaf(p, t, 2.93243101e-8f); //  0x1.f7c9aep-26 
		p = fmaf(p, t, 1.22150334e-6f); //  0x1.47e512p-20 
		p = fmaf(p, t, 2.84108955e-5f); //  0x1.dca7dep-16 
		p = fmaf(p, t, 3.93552968e-4f); //  0x1.9cab92p-12 
		p = fmaf(p, t, 3.02698812e-3f); //  0x1.8cc0dep-9 
		p = fmaf(p, t, 4.83185798e-3f); //  0x1.3ca920p-8 
		p = fmaf(p, t, -2.64646143e-1f); // -0x1.0eff66p-2 
		p = fmaf(p, t, 8.40016484e-1f); //  0x1.ae16a4p-1 
	}
	else { // maximum ulp error = 2.35002
		p = 5.43877832e-9f;  //  0x1.75c000p-28 
		p = fmaf(p, t, 1.43285448e-7f); //  0x1.33b402p-23 
		p = fmaf(p, t, 1.22774793e-6f); //  0x1.499232p-20 
		p = fmaf(p, t, 1.12963626e-7f); //  0x1.e52cd2p-24 
		p = fmaf(p, t, -5.61530760e-5f); // -0x1.d70bd0p-15 
		p = fmaf(p, t, -1.47697632e-4f); // -0x1.35be90p-13 
		p = fmaf(p, t, 2.31468678e-3f); //  0x1.2f6400p-9 
		p = fmaf(p, t, 1.15392581e-2f); //  0x1.7a1e50p-7 
		p = fmaf(p, t, -2.32015476e-1f); // -0x1.db2aeep-3 
		p = fmaf(p, t, 8.86226892e-1f); //  0x1.c5bf88p-1 
	}
	r = a * p;
	return r;
}

/* compute natural logarithm with a maximum error of 0.85089 ulp */
float my_logf(float a)
{
	float i, m, r, s, t;
	int e;

	m = frexpf(a, &e);
	if (m < 0.666666667f) { // 0x1.555556p-1
		m = m + m;
		e = e - 1;
	}
	i = (float)e;
	/* m in [2/3, 4/3] */
	m = m - 1.0f;
	s = m * m;
	/* Compute log1p(m) for m in [-1/3, 1/3] */
	r = -0.130310059f;  // -0x1.0ae000p-3
	t = 0.140869141f;  //  0x1.208000p-3
	r = fmaf(r, s, -0.121484190f); // -0x1.f19968p-4
	t = fmaf(t, s, 0.139814854f); //  0x1.1e5740p-3
	r = fmaf(r, s, -0.166846052f); // -0x1.55b362p-3
	t = fmaf(t, s, 0.200120345f); //  0x1.99d8b2p-3
	r = fmaf(r, s, -0.249996200f); // -0x1.fffe02p-3
	r = fmaf(t, m, r);
	r = fmaf(r, m, 0.333331972f); //  0x1.5554fap-2
	r = fmaf(r, m, -0.500000000f); // -0x1.000000p-1
	r = fmaf(r, s, m);
	r = fmaf(i, 0.693147182f, r); //  0x1.62e430p-1 // log(2)
	if (!((a > 0.0f) && (a <= 3.40282346e+38f))) { // 0x1.fffffep+127
		r = a + a;  // silence NaNs if necessary
		if (a < 0.0f) r = NAN; //  NaN
		if (a == 0.0f) r = -INFINITY; // -Inf
	}
	return r;
}


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
	EVT_COMMAND(wxID_ANY, wxEVT_START_SIMULATION, CLDPCMan::__OnSimulate)	
	EVT_COMMAND(wxID_ANY, wxEVT_H_SELECTED, CLDPCMan::__OnHSelected)
	EVT_COMMAND(wxID_ANY, wxEVT_BIN_IMG_SELECTED, CLDPCMan::__OnImgSelected)
END_EVENT_TABLE()

void CLDPCMan::DoLoadImg()
{	
	wxString	strApp;
	int			idx = 0;
	m_sasCurrWords.Clear();
	m_ImgTX = cv::imread(m_strPathLoaded.ToStdString());
	if (m_ImgTX.channels() > 1)
		cv::cvtColor(m_ImgTX, m_ImgTX, cv::COLOR_RGB2GRAY);

	m_szImgLoaded = cv::Size(m_ImgTX.cols, m_ImgTX.rows);

	cv::threshold(m_ImgTX, m_ImgTX, 128, 255, cv::THRESH_BINARY);
	for (int r = 0; r < m_ImgTX.rows; r++)
		for (int c = 0; c < m_ImgTX.cols; c++)
		{
			if (m_ImgTX.at<uchar>(r, c) == 0)
				strApp += "0";
			else
				strApp += "1";

			idx++;
			if ((idx % (m_ParityCheck.cols - m_ParityCheck.rows)) == 0)
			{
				m_sasCurrWords.push_back(strApp);
				strApp.clear();
			}
		}

	int l = strApp.Len();
	if (l != 0)
	{
		m_iPadBitForImage = (m_ParityCheck.cols - m_ParityCheck.rows) - l;
		for (int i = 0; i < (m_ParityCheck.cols - m_ParityCheck.rows) - l; i++)
			strApp.Append("0");

		m_sasCurrWords.push_back(strApp);
	}	

	::wxGetApp().GetMainWnd()->GetMainPanel()->m_asStringWords = m_sasCurrWords;
	::wxGetApp().GetMainWnd()->GetMainPanel()->GenerationFinished();
	m_bLoadedImg = true;
}

void CLDPCMan::__OnImgSelected(wxCommandEvent& event)
{
	m_strPathLoaded = event.GetString().ToStdString();
	::wxGetApp().GetMainWnd()->GetMainPanel()->StartGeneration();
	m_pThread = new SimThread(this, 1);
	if (m_pThread->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Can't create the thread!");
		delete m_pThread;
		m_pThread = NULL;
	}	
}

void CLDPCMan::__OnHSelected(wxCommandEvent& event)
{
	wxString strPath = event.GetString();

	if (!wxDir::Exists(strPath.BeforeLast('\\') + "\\tmp\\"))
		wxMkDir(strPath.BeforeLast('\\') + "\\tmp\\");

	m_strFileNameApppchk = event.GetString().BeforeLast('\\') + "\\tmp\\H.pchk";
	m_strFileNameAppGen = event.GetString().BeforeLast('\\') + "\\tmp\\G.Gen";
	m_strFileNameColsOrder = event.GetString().BeforeLast('\\') + "\\tmp\\ColsOrder.txt";

	alist2pchk((char*)strPath.ToStdString().c_str(), (char*)m_strFileNameApppchk.ToStdString().c_str());
	make_gen((char*)m_strFileNameApppchk.ToStdString().c_str(), (char*)m_strFileNameAppGen.ToStdString().c_str(), (char*)m_strFileNameColsOrder.ToStdString().c_str(), (char*)"dense", NULL, NULL, NULL, NULL);

	//__ReorderParity(m_strFileNameColsOrder);
	//cvMat2alist(strPath.ToStdString());

	//alist2pchk((char*)strPath.ToStdString().c_str(), (char*)m_strFileNameApppchk.ToStdString().c_str());
	//make_gen((char*)m_strFileNameApppchk.ToStdString().c_str(), (char*)m_strFileNameAppGen.ToStdString().c_str(), (char*)m_strFileNameColsOrder.ToStdString().c_str(), (char*)"dense", NULL, NULL, NULL, NULL);
	
}

void CLDPCMan::__SwapParityCol(int c1, int c2)
{

	cv::Mat mc1 = m_ParityCheck.col(c1).clone();
	cv::Mat mc2 = m_ParityCheck.col(c2).clone();

	for (int i = 0; i < m_ParityCheck.rows; i++)
	{
		m_ParityCheck.at<float>(i, c1) = mc2.at<float>(i, 0);
		m_ParityCheck.at<float>(i, c2) = mc1.at<float>(i, 0);
	}
}


void CLDPCMan::DoSimulate()
{
	double dFrom = m_dBatchFrom;
	double dTo = m_dBatchTo;
	double dStep = m_dBatchStep;

	std::vector<std::pair<double, double>> v_statics;

	wxCommandEvent evt(wxEVT_SIMULATION_FINISHED);

	int iBatchIter = ((m_dBatchTo - m_dBatchFrom) / m_dBatchStep)+1;
	int iBatchIdx = 0;

	if (!m_bBatch) {

		if (m_iChannel == CH_AWGN) {
			dFrom = dTo = m_dAWGNVar;
		}
		if (m_iChannel == CH_BSC) {
			dFrom = dTo = m_dBSCPe;
		}		
	}


	do
	{
		iBatchIdx++;

		m_strFileNameInput = m_strFileNameApppchk.BeforeLast('\\') + "\\input.txt";
		m_strFileNameEnc = m_strFileNameApppchk.BeforeLast('\\') + "\\Encode.enc";
		m_strFileNameRec = m_strFileNameApppchk.BeforeLast('\\') + "\\Rec.rec";
		m_strFileNameDec = m_strFileNameApppchk.BeforeLast('\\') + "\\Dec.rec";
		m_strFileNameExt = m_strFileNameApppchk.BeforeLast('\\') + "\\Ext.rec";

		wxString	strChannel;
		int			idx = 0;
		double		dParam = dFrom;
		int			bits;

		if (m_bCodifica)
			bits = m_sasCurrWords.size() * m_ParityCheck.cols;
		else
			bits = m_sasCurrWords.size() * (m_ParityCheck.cols- m_ParityCheck.rows);

		int			errBits = 0;

		if (m_iChannel == CH_AWGN) {
			strChannel = "awgn";
		}
		if (m_iChannel == CH_BSC) {
			strChannel = "bsc";
		}

		if (wxFile::Exists(m_strFileNameInput))
			wxRemove(m_strFileNameInput);

		wxTextFile	tf(m_strFileNameInput);
		tf.Create();

		for (int i = 0; i < m_sasCurrWords.size(); i++)
			tf.AddLine(m_sasCurrWords[i]);
		tf.Write();
		tf.Close();

		if (m_bCodifica)
		{
			encode((char*)m_strFileNameApppchk.ToStdString().c_str(), (char*)m_strFileNameAppGen.ToStdString().c_str(), (char*)m_strFileNameInput.ToStdString().c_str(), (char*)m_strFileNameEnc.ToStdString().c_str());
			tf.Open(m_strFileNameEnc);
			idx = 0;
			m_sasCurrWordsEnc.Clear();
			for (wxString str = tf.GetFirstLine(); !tf.Eof(); str = tf.GetNextLine())
				m_sasCurrWordsEnc.push_back(str);

			tf.Close();
		}
		else
		{
			m_sasCurrWordsEnc = m_sasCurrWords;

			if (wxFile::Exists(m_strFileNameEnc))
				wxRemoveFile(m_strFileNameEnc);

			wxTextFile wxtfe(m_strFileNameEnc);
			wxtfe.Create();

			for (int i = 0; i < m_sasCurrWordsEnc.size(); i++)
				wxtfe.AddLine(m_sasCurrWordsEnc[i]);

			wxtfe.Write();
			wxtfe.Close();
		}

		srand(time(NULL));
		transmit((char*)m_strFileNameEnc.ToStdString().c_str(), (char*)m_strFileNameRec.ToStdString().c_str(), (char*)wxString::Format("%d", rand()).ToStdString().c_str(), (char*)strChannel.ToStdString().c_str(), (char*)wxString::Format("%.1f", dParam).ToStdString().c_str());

		tf.Open(m_strFileNameRec);
		m_sasCurrWordsTx.clear();
		for (wxString str = tf.GetFirstLine(); !tf.Eof(); str = tf.GetNextLine())
			m_sasCurrWordsTx.push_back(str);
		tf.Close();

		if (wxFile::Exists(m_strFileNameDec))
			wxRemoveFile(m_strFileNameDec);

		tf.Open(m_strFileNameRec);
		wxTextFile tf2(m_strFileNameDec);
		tf2.Create();
		wxString	cw;
		wxArrayString ascw;
		idx = 0;
		m_sasCurrWordsDecode.clear();
		m_decodeOK.clear();
		m_iTentativi.clear();
		static bool b = true;

		for (wxString str = tf.GetFirstLine(); !tf.Eof(); str = tf.GetNextLine())
		{
			if (m_bStopSim)
			{
				evt.SetString(wxString::Format("%d;%d;%.4f", bits, errBits, dFrom));
				wxGetApp().GetMainWnd()->GetMainPanel()->GetEventHandler()->QueueEvent(evt.Clone());			
				wxGetApp().GetMainWnd()->GetMainPanel()->BatchFinished();
				break;
			}

			std::vector<double> channel;

			if (m_iChannel == CH_AWGN)
				channel = DoubleVectorFromString(str);

			if (m_iChannel == CH_BSC)
				for (int c = 0; c < str.length(); c++)
					channel.push_back(wxAtof(str[c]));

			int iAtt = 1;
			if (m_bCodifica)
			{
				cw = m_mainGraph->Decode(channel, m_iChannel, dParam, m_ParityCheck, m_iMaxTentativi, &iAtt);
			}
			else
			{
				wxString strApp;
				for (int c = 0; c < channel.size(); c++)
				{
					if (channel[c] > 0.0)
						strApp += "1";
					else
						strApp += "0";
				}
				m_iTentativi.push_back(1);
				cw = strApp;
			}
			ascw.push_back(cw);
			tf2.AddLine(cw);
			m_iTentativi.push_back(iAtt);

			if (cw == m_sasCurrWordsEnc[idx])
				m_decodeOK.push_back(true);
			else
			{
				for (int ch = 0; ch < cw.length(); ch++)
					if (cw[ch] != m_sasCurrWordsEnc[idx][ch])
						errBits++;

				m_decodeOK.push_back(false);
			}
			idx++;

			wxGetApp().GetMainWnd()->GetMainPanel()->SetInfo(wxString::Format("Batch %d\\%d | Decodificata parola %d\\%d con %d iterazioni",iBatchIdx, iBatchIter, idx, (int)m_sasCurrWords.size(), iAtt));
		}

		if (m_bStopSim)
			break;

		tf.Close();
		tf2.Write();
		tf2.Close();

		if (m_bCodifica)
		{
			extract((char*)m_strFileNameAppGen.ToStdString().c_str(), (char*)m_strFileNameDec.ToStdString().c_str(), (char*)m_strFileNameExt.ToStdString().c_str());

			tf.Open(m_strFileNameExt);
			for (wxString str = tf.GetFirstLine(); !tf.Eof(); str = tf.GetNextLine())
			{
				m_sasCurrWordsDecode.push_back(str);
			}
			tf.Close();
		}
		else
		{
			for( int i=0;i< ascw.size(); i++)
				m_sasCurrWordsDecode.push_back(ascw[i]);
		}



		v_statics.push_back(std::pair<double, double>((double)(errBits) / (double)(bits), dFrom));


		if (m_bLoadedImg)
		{
			int r = 0;
			int c = 0;
			int i;
			int ch;
			int l;
			int b = 0;
			try
			{
				m_ImgRX = cv::Mat::zeros(m_szImgLoaded, CV_8UC1);
				m_ImgRX = cv::Scalar(255);
				unsigned char* puc = m_ImgRX.data;

				for (i = 0; i < m_sasCurrWordsDecode.size(); i++)
				{
					
					if (i == m_sasCurrWordsDecode.Count() - 1)
						l = m_sasCurrWordsDecode[i].Len() - m_iPadBitForImage;
					else
						l = m_sasCurrWordsDecode[i].Len();

					for (ch = 0; ch < l; ch++)
					{
						
						//if (m_sasCurrWordsDecode[i][ch] == '0') m_ImgRX.at<uchar>(r, c) = 0;
						/*c++;
						if ((c % m_szImgLoaded.width) == 0)
						{
							c = 0;
							r++;
						}*/
						if (m_sasCurrWordsDecode[i][ch] == '0') puc[b] = 0;
						b++;
					}
				}
			}
			catch (std::exception& e)
			{
				wxString g = e.what();
				g = g;
			}
		}
		evt.SetString(wxString::Format("%d;%d;%.4f", bits, errBits, dFrom));
		wxGetApp().GetMainWnd()->GetMainPanel()->GetEventHandler()->QueueEvent(evt.Clone());
		dFrom += dStep;
	}while ((dFrom < dTo) && (m_bBatch));

	if (m_bBatch)
	{

		wxString strFileName = wxDateTime::Now().Format("%d-%m-%Y_%H-%M-%S") + "_Batch.txt";
		wxTextFile wxtf(strFileName);
		wxtf.Create();

		if (m_iChannel == CH_AWGN)
			wxtf.AddLine("BER\tVar\tEb\\N0\tEb\\N0(dB)");
		else if (m_iChannel == CH_BSC)
			wxtf.AddLine("BER\tPe;");

		for( int i=0; i< v_statics.size(); i++)
			if((m_iChannel== CH_AWGN) && (v_statics[i].second != 0.0))
				wxtf.AddLine(wxString::Format("%.4f\t%.4f\t%.4f\t%.4f", v_statics[i].first, v_statics[i].second, 1.0/(2.0* v_statics[i].second), 10.0*log10(1.0 / (2.0 * v_statics[i].second))));
			else if ((m_iChannel == CH_BSC) && (v_statics[i].second != 0.0))
			{
				double xx = v_statics[i].second;
				double eb = pow(my_erfinvf((2 * xx) + 1), 2.0);
				wxtf.AddLine(wxString::Format("%.4f\t%.4f\t%.4f\t%.4f", v_statics[i].first, v_statics[i].second, eb, 10*log10(eb)));
			}

		wxtf.Write();
		wxtf.Close();
	}

	wxGetApp().GetMainWnd()->GetMainPanel()->BatchFinished();
}

void CLDPCMan::__OnSimulate(wxCommandEvent& event)
{
	wxArrayString* sas = (wxArrayString*)event.GetClientData();
	m_sasCurrWords.clear();
	m_sasCurrWords = *sas;
	delete sas;

	m_bBatch = event.GetInt();
	wxStringTokenizer strTokA;

	strTokA.SetString(event.GetString(), ";");
	m_dBatchFrom = wxAtof(strTokA.GetNextToken());
	m_dBatchTo = wxAtof(strTokA.GetNextToken());
	m_dBatchStep = wxAtof(strTokA.GetNextToken());

	m_pThread = new SimThread(this);
	if (m_pThread->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Can't create the thread!");
		delete m_pThread;
		m_pThread = NULL;
	}

}
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

	//switch (iTimerID)
	//{
	//default:
	//	break;
	//}
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
		//m_MainProcThread.m_hHandle = ::CreateThread(NULL,
		//	0,
		//	(LPTHREAD_START_ROUTINE)CLDPCMan::__MainProcessThreadStub,
		//	(void*)this,
		//	0,
		//	&m_MainProcThread.m_dwThreadId);

		m_mainGraph = nullptr;
		m_iChannel = CH_AWGN;
		m_iPadBitForImage = 0;
		m_bLoadedImg = false;
		m_bStopSim = false;

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

		if (m_mainGraph != nullptr)
		{
			delete m_mainGraph;
			m_mainGraph = nullptr;
		}

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

void CLDPCMan::cvMat2alist(std::string strFileName)
{
	int N = m_ParityCheck.cols;
	int M = m_ParityCheck.rows;

	std::vector<int> viSizeN;
	std::vector<int> viSizeM;

	if (wxFile::Exists(strFileName))
		wxRemoveFile(strFileName);

	wxTextFile tf(strFileName);
	tf.Create();

	tf.AddLine(wxString::Format("%d %d", N, M) );
	tf.Write();

	for (int r = 0; r < m_ParityCheck.rows; r++)
		viSizeN.push_back(cv::countNonZero(m_ParityCheck.row(r)));

	for (int c = 0; c < m_ParityCheck.cols; c++)
		viSizeM.push_back(cv::countNonZero(m_ParityCheck.col(c)));

	int iMaxN = *std::max_element(viSizeN.begin(), viSizeN.end());
	int iMaxM = *std::max_element(viSizeM.begin(), viSizeM.end());

	tf.AddLine(wxString::Format("%d %d", iMaxM, iMaxN));
	tf.Write();

	wxString app;
	for (int i = 0; i < viSizeM.size(); i++)
		app += wxString::Format("%d ", viSizeM[i]);

	tf.AddLine(app);
	tf.Write();

	app.clear();
	for( int i =0; i< viSizeN.size(); i++)
		app += wxString::Format("%d ", viSizeN[i]);
	
	tf.AddLine(app);
	tf.Write();
	app.Clear();

	for (int c = 0; c < m_ParityCheck.cols; c++)
	{
		for (int r = 0; r < m_ParityCheck.rows; r++)
		{
			if (m_ParityCheck.at<float>(r, c) != 0.0)
			{
				app += wxString::Format("%d ", r+1 );
			}
		}
		tf.AddLine(app);
		app.Clear();
	}
	
	tf.Write();
	app.Clear();

	for (int r = 0; r < m_ParityCheck.rows; r++)
	{
		for (int c = 0; c < m_ParityCheck.cols; c++)
		{
			if (m_ParityCheck.at<float>(r, c) != 0.0)
			{
				app += wxString::Format("%d ", c + 1);
			}
		}
		tf.AddLine(app);
		app.Clear();
	}

	tf.Write();
	tf.Close();
}

std::pair<int, int>	CLDPCMan::alist2cvMat(std::string strFileName)
{
	int N, M;
	std::pair<int, int>	 iRet = std::pair<int, int>(-1, -1);
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
		m_ParityCheck = cv::Mat::zeros(M, N, CV_32FC1);
		iRet = std::pair<int, int>(N, M);

		//
		for (uint i = 0; i < vMDirWeight.size(); i++)
		{
			strApp = tf.GetNextLine();			
			std::vector<int> vRow = IntVectorFromString(strApp);

			for( uint j = 0; j < vRow.size(); j++ )
				if(vRow[j] != 0)
					m_ParityCheck.at<float>(cv::Point(i, vRow[j]-1)) = 1.0;
		}

		// Costriusico il grafo di tanner
		if (m_mainGraph != nullptr)
			delete m_mainGraph;

		m_mainGraph = new CTannerGraph(m_ParityCheck);

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

	return iRet;
}

void CLDPCMan::DrawGraph()
{
	if (m_mainGraph != nullptr)
		m_mainGraph->Draw();
}

wxThread::ExitCode SimThread::Entry()
{
	if( m_ibeahv == 0)
		m_pHandler->DoSimulate();

	if (m_ibeahv == 1)
		m_pHandler->DoLoadImg();
	
	return (wxThread::ExitCode)0;     // success
}

void CLDPCMan::__ReorderParity( wxString cols_order_file)
{
	wxTextFile	tf;
	tf.Open(cols_order_file);
	std::vector<int> vi = IntVectorFromString(tf.GetFirstLine());
	tf.Close();

	//std::map<int, wxChar> map;
	//for (int i = 0; i < vi.size(); i++)
	//	map.insert(std::pair<int, wxChar>(vi[i], cw[i]));

	//wxString ss;
	//for (int i = 544; i < vi.size(); i++)
	//{
	//	std::map<int, wxChar>::iterator it = map.find(i);
	//	ss += it->second;
	//}

	bool swap = true;
	while (swap)
	{
		swap = false;
		for (int i = 0; i < vi.size() - 1; i++)
		{
			if (vi[i] > vi[i + 1])
			{
				int tmp = vi[i];
				vi[i] = vi[i + 1];
				vi[i + 1] = tmp;

				__SwapParityCol(i, i + 1);

				swap = true;
			}
		}
	}
}

wxString CLDPCMan::__ReorderWord(wxString cw, wxString cols_order_file)
{
	wxTextFile	tf;
	wxString	strRet=cw;
	tf.Open(cols_order_file);
	std::vector<int> vi = IntVectorFromString(tf.GetFirstLine());
	tf.Close();

	std::map<int, wxChar> map;
	for (int i = 0; i < vi.size(); i++)
		map.insert(std::pair<int, wxChar>(vi[i], cw[i]));

	wxString ss;
	for (int i = 544; i < vi.size(); i++)
	{
		std::map<int, wxChar>::iterator it = map.find(i);
		ss+=it->second;
	}

	bool swap = true;
	while (swap)
	{
		swap = false;
		for (int i = 0; i < cw.length()-1; i++)
		{
			if (vi[i] > vi[i + 1])
			{
				int tmp = vi[i];
				vi[i] = vi[i+1];
				vi[i+1] = tmp;

				wxChar ch = strRet[i];
				strRet[i] = strRet[i+1];
				strRet[i+1] = ch;

				swap = true;
			}
		}
	}
	
	return strRet;
}
