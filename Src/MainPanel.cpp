#include "stdafx.h"
#include "MainPanel.h"
#include "LDPCMan.h"
#include "AppEvents.h"

BEGIN_EVENT_TABLE(CMainPanel, CMainPanel_Base)
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

	m_listBoxWord->InsertColumn(0, "#", wxLIST_FORMAT_LEFT, 60 );
	m_listBoxWord->InsertColumn(1, "Informazione", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(2, "Parola di codice", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(3, "Ricevuto da canale", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(4, "Decodifica", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(5, "", wxLIST_FORMAT_LEFT, 20);

	m_btnDrawTanner->Enable(false);
	m_btnViewH->Enable(false);
	m_pnlSim->Enable(false);
	m_pnlFiller->Enable(false );

	
	m_bpAvviaSimulazione->SetBitmap(wxBitmap("IDP_PLAY", wxBITMAP_TYPE_PNG_RESOURCE));
	m_bpAvviaSimulazione->SetBitmapPosition(wxRIGHT);

	m_bpAvviaSimulazione->SetLabel("Avvia");

	m_pMan->SetAWGNVar(m_spnVarAWGN->GetValue());

	pImageList = new wxImageList(16, 16);
	pImageList->Add(wxBitmap("IDP_LEDS", wxBITMAP_TYPE_PNG_RESOURCE), wxBitmap("IDP_LEDS_MASK", wxBITMAP_TYPE_PNG_RESOURCE));
	m_listBoxWord->SetImageList(pImageList, wxIMAGE_LIST_SMALL);
	
	wxItemAttr ia;
	ia.SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString));
	m_listBoxWord->SetHeaderAttr(ia);
}


CMainPanel::~CMainPanel()
{
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
			for (int i = 0; i < m_Hdims.second; i++)
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
		if (strWord.Len() < m_Hdims.second)
		{
			for (int i = 0; i < m_Hdims.second - l; i++)
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

	m_gauge3->Pulse();
}

void CMainPanel::__OnPickBinImage(wxFileDirPickerEvent& event)
{
	//
}

void CMainPanel::__OnPickH(wxFileDirPickerEvent& event)
{
	m_Hdims = m_pMan->alist2cvMat( event.GetPath().ToStdString() );
	if (m_Hdims.first > 0)
	{
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
}

void CMainPanel::__OnWordChar(wxKeyEvent& event)
{
	if (((event.GetKeyCode() == '1')|| 
		(event.GetKeyCode() == '0'))
		&& (m_txtInsertWord->GetValue().Len()< m_Hdims.second) )
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

	for (int i = 0; i < m_listBoxWord->GetItemCount(); i++)
		sasWord->push_back(m_listBoxWord->GetItemText(i, 1));

	evt.SetClientData((void*)sasWord);
	m_pMan->QueueEvent(evt.Clone());

	m_pnlSimCfg->Enable(false);
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

void CMainPanel::GenerationFinished()
{
	for (int i = 0; i < m_asStringWords.Count(); i++)
	{
		int iItemIdx = m_listBoxWord->GetItemCount();
		m_listBoxWord->InsertItem(iItemIdx, "", -1);
		m_listBoxWord->SetItem(iItemIdx, 0, wxString::Format("%d", m_listBoxWord->GetItemCount()));
		m_listBoxWord->SetItem(iItemIdx, 1, m_asStringWords[i]);
	}

	m_listBoxWord->ScrollList(0, INT_MAX / 2);

	m_gauge3->SetValue(0);
	this->Layout();
	this->Refresh();
}

void CMainPanel::SimulationFinished()
{ 
	for (int i = 0; i < m_pMan->m_sasCurrWords.size(); i++)
	{
		m_listBoxWord->SetItem(i, 2, m_pMan->m_sasCurrWordsEnc[i]);
		m_listBoxWord->SetItem(i, 3, m_pMan->m_sasCurrWordsTx[i]);
		m_listBoxWord->SetItem(i, 4, m_pMan->m_sasCurrWordsDecode[i]);
		m_listBoxWord->SetItem(i, 5, "", (m_pMan->m_decodeOK[i]) ? 0:1);
	}

	m_gauge2->SetValue(0); 
	m_pnlSimCfg->Enable(true);

	m_pnlFiller->Enable(true);
	m_pnlChannel->Enable(true);
	m_pnlMatriceParita->Enable(true);

	this->Layout(); 
	this->Refresh();
}

wxThread::ExitCode CMainPanel::GenThread::Entry()
{
	m_pHandler->DoGenerate();
	return (wxThread::ExitCode)0;     // success
}
