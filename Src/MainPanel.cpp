#include "stdafx.h"
#include "MainPanel.h"
#include "LDPCMan.h"
#include "AppEvents.h"

BEGIN_EVENT_TABLE(CMainPanel, CMainPanel_Base)
	EVT_COMMAND(wxID_ANY, wxEVT_SIMULATION_FINISHED, CMainPanel::__OnSimulationFinished)
END_EVENT_TABLE()

CMainPanel::CMainPanel(wxWindow* pParent,
	CLDPCMan*			pMan,
	wxWindowID			WindowID	/* = wxID_ANY */,
	const wxPoint& pos			/* = wxDefaultPosition */,
	const wxSize& size		/* = wxDefaultSize */,
	long				lStyle		/* = wxTAB_TRAVERSAL */,
	const wxString& strName		/* = wxPanelNameStr */)
	: CMainPanel_Base(pParent, WindowID, pos, size, lStyle)
{
	wxImageList* pImageList;

	m_pMan = pMan;

	m_lblMatTitle->SetLabel("Matrice di controllo parità");
	m_lblChTitle->SetLabel("Proprietà canale");
	m_lblNrBitParity->SetLabel("Numero bit di parità");

	m_rdAWGN->SetValue(true);
	m_rdRandom->SetValue(true);

	m_bmpChannel->SetBitmap(wxBitmap("IDP_AWGN", wxBITMAP_TYPE_PNG_RESOURCE));
	m_spnPeBSC->Enable(false);
	m_txtInsertWord->Enable(false);

	pImageList = new wxImageList(16, 16);
	pImageList->Add(wxBitmap("IDP_LEDS", wxBITMAP_TYPE_PNG_RESOURCE), wxBitmap("IDP_LEDS_MASK", wxBITMAP_TYPE_PNG_RESOURCE));
	m_listBoxWord->SetImageList(pImageList, wxIMAGE_LIST_SMALL);

	m_listBoxWord->InsertColumn(0, "", wxLIST_FORMAT_LEFT, 0);
	m_listBoxWord->InsertColumn(1, "#", wxLIST_FORMAT_LEFT, 60 );
	m_listBoxWord->InsertColumn(2, "Informazione", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(3, "Parola di codice", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(4, "Ricevuto da canale", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(5, "Decodifica", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(6, "", wxLIST_FORMAT_LEFT, 20);
	m_listBoxWord->InsertColumn(7, "Iter.", wxLIST_FORMAT_LEFT, 60);

	m_btnDrawTanner->Enable(false);
	m_btnViewH->Enable(false);
	m_pnlSim->Enable(false);
	m_pnlFiller->Enable(false );

	
	m_bpAvviaSimulazione->SetBitmap(wxBitmap("IDP_PLAY", wxBITMAP_TYPE_PNG_RESOURCE));
	m_bpAvviaSimulazione->SetBitmapPosition(wxRIGHT);

	m_bpAvviaSimulazione->SetLabel("Avvia");

	m_pMan->SetAWGNVar(m_spnVarAWGN->GetValue());

	wxItemAttr ia;
	ia.SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));
	m_listBoxWord->SetHeaderAttr(ia);

	m_pMan->m_iMaxTentativi = m_spnAttmpt->GetValue();

	m_listResult->InsertColumn(0, "#", wxLIST_FORMAT_LEFT, 60);
	m_listResult->InsertColumn(1, "BER", wxLIST_FORMAT_LEFT, 90);
	m_listResult->InsertColumn(2, "Tipo canale", wxLIST_FORMAT_LEFT, 120);
	m_listResult->InsertColumn(3, "Parametro", wxLIST_FORMAT_LEFT, 100);
	m_listResult->InsertColumn(4, "k\\n", wxLIST_FORMAT_LEFT, 60);
	m_listResult->InsertColumn(5, "k", wxLIST_FORMAT_LEFT, 60);
	m_listResult->InsertColumn(6, "n", wxLIST_FORMAT_LEFT, 60);
	m_listResult->InsertColumn(7, "m", wxLIST_FORMAT_LEFT, 60);
	m_listResult->InsertColumn(8, "Bits", wxLIST_FORMAT_LEFT, 150);
	m_listResult->SetHeaderAttr(ia);
}


CMainPanel::~CMainPanel()
{
}

void CMainPanel::__OnCleanRes(wxCommandEvent& event)
{
	m_listResult->DeleteAllItems();
}

void CMainPanel::__OnChannel(wxCommandEvent& event)
{
	if (m_rdAWGN->GetValue())
	{
		m_spnVarAWGN->Enable(true);
		m_spnPeBSC->Enable(false);
		m_bmpChannel->SetBitmap(wxBitmap("IDP_AWGN", wxBITMAP_TYPE_PNG_RESOURCE));
		m_bmpChannel->Center();
		m_pMan->SetChannel(CLDPCMan::CH_AWGN);
	}
	else
	{
		m_spnVarAWGN->Enable(false);
		m_spnPeBSC->Enable(true);
		m_bmpChannel->SetBitmap(wxBitmap("IDP_BSC", wxBITMAP_TYPE_PNG_RESOURCE));
		m_bmpChannel->Center();
		m_pMan->SetChannel(CLDPCMan::CH_BSC);
	}
	this->Layout();
}

void CMainPanel::__OnChangeSimulation(wxCommandEvent& event)
{
	if (m_rdRandom->GetValue())
	{
		m_txtInsertWord->Enable(false);
	}
	else
	{
		m_txtInsertWord->Enable(true);
	}
	this->Layout();
}

void CMainPanel::__OnDrawTanner(wxCommandEvent& event)
{
	m_pMan->DrawGraph();
}

void CMainPanel::DoGenerate()
{
	int  iRep = m_spnRep->GetValue();
	bool bRand = m_rdRandom->GetValue();
	wxString		strWord = wxEmptyString;
	m_asStringWords.clear();

	if (bRand)
	{
		for (int i = 0; i < iRep; i++)
		{
			for (int i = 0; i < (m_Hdims.first-m_Hdims.second); i++)
			{
				LARGE_INTEGER liTime;
				QueryPerformanceCounter(&liTime);

				std::srand(liTime.LowPart);
				wxChar c = '1';
				int x = std::rand();
				if ((x % 2) == 0)
					c = '0';

				strWord += c;
			}
			m_asStringWords.push_back(strWord);

			strWord.Clear();
		}
	}
	else
	{
		strWord = m_txtInsertWord->GetValue();
		int l = strWord.Len();
		if (strWord.Len() < (m_Hdims.first - m_Hdims.second))
		{
			for (int i = 0; i < (m_Hdims.first - m_Hdims.second) - l; i++)
				strWord.Prepend("0");
		}

		for (int i = 0; i < iRep; i++)
			m_asStringWords.push_back(strWord);
	}
	GenerationFinished();
}

void CMainPanel::__OnInsertWord(wxCommandEvent& event)
{
	m_pThread = new GenThread(this);

	if (m_pThread->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Can't create the thread!");
		delete m_pThread;
		m_pThread = NULL;
	}

	m_pMan->m_bLoadedImg = false;
	m_gauge3->Pulse();
}

void CMainPanel::__OnPickBinImage(wxFileDirPickerEvent& event)
{
	wxCommandEvent evt(wxEVT_BIN_IMG_SELECTED);
	evt.SetString(event.GetPath());
	m_pMan->QueueEvent(evt.Clone());
}

void CMainPanel::__OnPickH(wxFileDirPickerEvent& event)
{
	m_Hdims = m_pMan->alist2cvMat( event.GetPath().ToStdString() );
	if (m_Hdims.first > 0)
	{
		wxCommandEvent e;
		__OnClearList(e);

		m_txtN->SetLabel(wxString::Format("%d", m_Hdims.first));
		m_txtM->SetLabel(wxString::Format("%d", m_Hdims.second));
		m_txtK->SetLabel(wxString::Format("%d", m_Hdims.first - m_Hdims.second));
		m_txtKN->SetLabel(wxString::Format("%.2f", (double)(m_Hdims.first - m_Hdims.second) / (double)(m_Hdims.first)));

		m_btnDrawTanner->Enable(true);
		m_btnViewH->Enable(true);
		m_pnlSim->Enable(true);		
		m_pnlFiller->Enable(true);

		wxCommandEvent evt(wxEVT_H_SELECTED);
		evt.SetString(event.GetPath());
		m_pMan->QueueEvent(evt.Clone());
	}
	else
	{
		
	}
}

void CMainPanel::__OnNonCod(wxCommandEvent& event)
{
	if (m_chkNonCod->GetValue())
	{
		m_pnlSim->Enable(true);
		m_pnlFiller->Enable(true);
	}
	else
	{
		m_pnlSim->Enable(false);
		m_pnlFiller->Enable(false);
	}
}

void CMainPanel::__OnStopSimulation(wxCommandEvent& event)
{ 
	m_pMan->m_bStopSim = true;
}

void CMainPanel::__OnViewH(wxCommandEvent& event)
{
	int iDesiredWidth = 500;
	cv::Mat H = m_pMan->GetH();
	double dScale = H.cols / (double)iDesiredWidth;

	cv::resize(H, H, cv::Size(), 1.0 / dScale, 1.0 / dScale, cv::INTER_NEAREST);
	cv::imshow("Matrice H", H);
}

void CMainPanel::__OnClearList(wxCommandEvent& event)
{
	m_listBoxWord->DeleteAllItems();
	m_pMan->m_bLoadedImg = false;
}

void CMainPanel::__OnWordChar(wxKeyEvent& event)
{
	if (((event.GetKeyCode() == '1')|| 
		(event.GetKeyCode() == '0'))
		&& (m_txtInsertWord->GetValue().Len() < (m_Hdims.first - m_Hdims.second)) )
		event.Skip();

	if (((event.GetKeyCode() == WXK_LEFT) ||
		(event.GetKeyCode() == WXK_RIGHT) ||
		(event.GetKeyCode() == WXK_NUMPAD_DELETE) ||
		(event.GetKeyCode() == WXK_CANCEL) ||
		(event.GetKeyCode() == WXK_DELETE) ||
		(event.GetKeyCode() == WXK_BACK) ))
		event.Skip();
}

void CMainPanel::__OnStartSimulation(wxCommandEvent& event)
{				
	wxCommandEvent evt(wxEVT_START_SIMULATION);
	wxArrayString* sasWord = new wxArrayString();

	m_pMan->m_bStopSim = false;

	for (int i = 0; i < m_listBoxWord->GetItemCount(); i++)
		sasWord->push_back(m_listBoxWord->GetItemText(i, 2));

	evt.SetClientData((void*)sasWord);
	evt.SetInt(m_chkBatch->GetValue());
	evt.SetString(wxString::Format("%.4f;%.4f;%.4f;", m_spnNFrom->GetValue(), m_spnNTo->GetValue(), m_spnNStep->GetValue() ));
	m_pMan->QueueEvent(evt.Clone());

	m_bpAvviaSimulazione->Enable(false);
	m_gauge2->Pulse();

	m_pnlFiller->Enable(false);
	m_pnlChannel->Enable(false);
	m_pnlMatriceParita->Enable(false);

	this->Layout();

	event.Skip(false);
}

void CMainPanel::__OnSpinAWGN(wxSpinDoubleEvent& event)
{
	m_pMan->SetAWGNVar(m_spnVarAWGN->GetValue());
	wxCommandEvent evt;
	__OnStartSimulation(evt);
}

void CMainPanel::__OnSpinBSC(wxSpinDoubleEvent& event)
{
	m_pMan->SetBSCPe(m_spnPeBSC->GetValue());
	wxCommandEvent evt;
	__OnStartSimulation(evt);
}

void CMainPanel::StartGeneration()
{
	m_gauge3->Pulse();
}

void CMainPanel::GenerationFinished()
{
	for (int i = 0; i < m_asStringWords.Count(); i++)
	{
		int iItemIdx = m_listBoxWord->GetItemCount();
		m_listBoxWord->InsertItem(iItemIdx, "", -1);
		m_listBoxWord->SetItem(iItemIdx, 1, wxString::Format("%d", m_listBoxWord->GetItemCount()));
		m_listBoxWord->SetItem(iItemIdx, 2, m_asStringWords[i]);
	}

	m_listBoxWord->ScrollList(0, INT_MAX / 2);

	m_gauge3->SetValue(0);
	this->Layout();
	this->Refresh();
}

void CMainPanel::BatchFinished()
{
	m_gauge2->SetValue(0);
	m_bpAvviaSimulazione->Enable(true);

	m_pnlFiller->Enable(true);
	m_pnlChannel->Enable(true);
	m_pnlMatriceParita->Enable(true);

	this->Layout();
	this->Refresh();
}

void CMainPanel::SimulationFinished(int bits, int errbits, double channel_par)
{
	for (int i = 0; i < m_pMan->m_sasCurrWords.size(); i++)
	{
		if (i < m_pMan->m_sasCurrWordsEnc.size())
			m_listBoxWord->SetItem(i, 3, m_pMan->m_sasCurrWordsEnc[i]);
		if (i < m_pMan->m_sasCurrWordsTx.size())
			m_listBoxWord->SetItem(i, 4, m_pMan->m_sasCurrWordsTx[i]);
		if (i < m_pMan->m_sasCurrWordsDecode.size())
			m_listBoxWord->SetItem(i, 5, m_pMan->m_sasCurrWordsDecode[i]);
		if (i < m_pMan->m_decodeOK.size())
			m_listBoxWord->SetItem(i, 6, "", (m_pMan->m_decodeOK[i]) ? 0 : 1);
		if (i < m_pMan->m_iTentativi.size())
			m_listBoxWord->SetItem(i, 7, wxString::Format("%d", m_pMan->m_iTentativi[i]));
	}




	wxString channel = m_rdAWGN->GetValue() ? "AWGN" : "BSC";
	wxString schannel_par = wxString::Format("%.4f", channel_par);
	int iItemIdx = m_listResult->GetItemCount();
	m_listResult->InsertItem(iItemIdx, "", -1);
	m_listResult->SetItem(iItemIdx, 0, wxString::Format("%d", m_listResult->GetItemCount()));
	m_listResult->SetItem(iItemIdx, 1, wxString::Format("%.2e", (double)errbits / (double)bits));
	m_listResult->SetItem(iItemIdx, 2, channel);
	m_listResult->SetItem(iItemIdx, 3, schannel_par);
	m_listResult->SetItem(iItemIdx, 4, wxString::Format("%.2f", (double)(m_pMan->GetH().cols - m_pMan->GetH().rows) / (double)m_pMan->GetH().cols));
	m_listResult->SetItem(iItemIdx, 5, wxString::Format("%d", m_pMan->GetH().cols - m_pMan->GetH().rows));
	m_listResult->SetItem(iItemIdx, 6, wxString::Format("%d", m_pMan->GetH().cols));
	m_listResult->SetItem(iItemIdx, 7, wxString::Format("%d", m_pMan->GetH().rows));
	m_listResult->SetItem(iItemIdx, 8, wxString::Format("%d", bits));

	m_listResult->ScrollList(0, INT_MAX / 2);

	wxCommandEvent e;
	__OnViewImgs(e);
}

wxThread::ExitCode CMainPanel::GenThread::Entry()
{
	m_pHandler->DoGenerate();
	return (wxThread::ExitCode)0;     // success
}

void CMainPanel::__OnViewImgs(wxCommandEvent& event)
{
	if((m_pMan->m_ImgTX.cols> 0) && (m_pMan->m_ImgRX.cols > 0))
	{
		int iDesiredWidth = 500;
		double dScale = m_pMan->m_ImgTX.cols / (double)iDesiredWidth;

		cv::resize(m_pMan->m_ImgTX, m_pMan->m_ImgTX, cv::Size(), 1.0 / dScale, 1.0 / dScale, cv::INTER_NEAREST);
		cv::imshow("Immagine trasmessa", m_pMan->m_ImgTX);

		dScale = m_pMan->m_ImgRX.cols / (double)iDesiredWidth;
		cv::resize(m_pMan->m_ImgRX, m_pMan->m_ImgRX, cv::Size(), 1.0 / dScale, 1.0 / dScale, cv::INTER_NEAREST);
		cv::imshow("Immagine ricevuta", m_pMan->m_ImgRX);
	}

}

void CMainPanel::__OnSpnAttempt(wxSpinEvent& event)
{
	m_pMan->m_iMaxTentativi = m_spnAttmpt->GetValue();
}

void CMainPanel::__OnSimulationFinished(wxCommandEvent& event)
{
	wxStringTokenizer strTok;
	strTok.SetString( event.GetString(), ";");
	wxString s = event.GetString();
	int g = wxAtoi(strTok.GetNextToken());
	int h = wxAtoi(strTok.GetNextToken());
	double f = wxAtof(strTok.GetNextToken());
	SimulationFinished(g,h,f);
}