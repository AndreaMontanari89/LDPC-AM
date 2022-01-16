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

	m_pMan = pMan;

	m_lblMatTitle->SetLabel("Matrice di controllo parità");
	m_lblChTitle->SetLabel("Proprietà canale");
	m_lblNrBitParity->SetLabel("Numero bit di parità");

	m_rdAWGN->SetValue(true);
	m_rdRandom->SetValue(true);

	m_bmpChannel->SetBitmap(wxBitmap("IDP_AWGN", wxBITMAP_TYPE_PNG_RESOURCE));
	m_spnPeBSC->Enable(false);
	m_txtInsertWord->Enable(false);

	m_listBoxWord->InsertColumn(0, "#", wxLIST_FORMAT_CENTER, 50 );
	m_listBoxWord->InsertColumn(1, "Informazione", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(2, "Parola di codice", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(3, "Ricevuto da canale", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(4, "Decodifica", wxLIST_FORMAT_LEFT, 300);
	m_listBoxWord->InsertColumn(5, "Check", wxLIST_FORMAT_CENTER, 70);

	m_btnDrawTanner->Enable(false);
	m_btnViewH->Enable(false);
	m_pnlSim->Enable(false);
	m_pnlFiller->Enable(false );

	
	m_bpAvviaSimulazione->SetBitmap(wxBitmap("IDP_PLAY", wxBITMAP_TYPE_PNG_RESOURCE));
	m_bpAvviaSimulazione->SetBitmapPosition(wxRIGHT);

	m_bpAvviaSimulazione->SetLabel("Avvia");

	m_pMan->SetAWGNVar(m_spnVarAWGN->GetValue());
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
	}
	else
	{
		m_spnVarAWGN->Enable(false);
		m_spnPeBSC->Enable(true);
		m_bmpChannel->SetBitmap(wxBitmap("IDP_BSC", wxBITMAP_TYPE_PNG_RESOURCE));
		m_bmpChannel->Center();
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

void CMainPanel::__OnInsertWord(wxCommandEvent& event)
{
	int  iRep = m_spnRep->GetValue();
	bool bRand = m_rdRandom->GetValue();
	wxString		strWord = wxEmptyString;
	wxArrayString	asStringWords;

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
				if ((x%2)==0)
					c = '0';

				strWord += c;
			}
			asStringWords.push_back(strWord);

			strWord.Clear();
		}
	}
	else
	{
		strWord = m_txtInsertWord->GetValue();
		int l = strWord.Len();
		if (strWord.Len() < m_Hdims.second)
		{
			for (int i = 0; i < m_Hdims.first -l; i++)
				strWord.Prepend("0");
		}

		for (int i = 0; i < iRep; i++)
			asStringWords.push_back(strWord);
	}

	for (int i = 0; i < asStringWords.Count(); i++)
	{
		int iItemIdx = m_listBoxWord->GetItemCount();
		m_listBoxWord->InsertItem(iItemIdx, "");
		m_listBoxWord->SetItem(iItemIdx, 0, wxString::Format("%d", m_listBoxWord->GetItemCount()));
		m_listBoxWord->SetItem(iItemIdx, 1, asStringWords[i]);
	}

	m_listBoxWord->ScrollList(0, INT_MAX/2);

}

void CMainPanel::__OnPickH(wxFileDirPickerEvent& event)
{
	m_Hdims = m_pMan->alist2cvMat( event.GetPath().ToStdString() );
	if (m_Hdims.first > 0)
	{
		m_txtN->SetLabel(wxString::Format("%d", m_Hdims.first));
		m_txtM->SetLabel(wxString::Format("%d", m_Hdims.second));
		m_txtK->SetLabel(wxString::Format("%d", m_Hdims.first - m_Hdims.second));
		m_txtKN->SetLabel(wxString::Format("%.1f", (double)(m_Hdims.first - m_Hdims.second) / (double)(m_Hdims.first)));

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
	evt.SetClientData((void*)m_listBoxWord);
	m_pMan->QueueEvent(evt.Clone());
}

void CMainPanel::__OnSpinAWGN(wxSpinDoubleEvent& event)
{
	m_pMan->SetAWGNVar(m_spnVarAWGN->GetValue());
}