#ifndef __MAINPANEL__
#define __MAINPANEL__
#pragma once

#include "MainPanel_Base.h"
class CLDPCMan;

class CMainPanel : public CMainPanel_Base
{
	// Costruttore(i) / distruttore
public:
	CMainPanel(wxWindow* pParent,
		CLDPCMan*			pMan,
		wxWindowID			WindowID = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long				lStyle = wxTAB_TRAVERSAL,
		const wxString& strName = wxPanelNameStr);
	~CMainPanel();

protected:
	void	__OnChannel(wxCommandEvent& event);
	void    __OnChangeSimulation(wxCommandEvent& event);
	void	__OnDrawTanner(wxCommandEvent& event);
	void	__OnInsertWord(wxCommandEvent& event);
	void    __OnPickH(wxFileDirPickerEvent& event);
	void	__OnWordChar(wxKeyEvent& event);
	void	__OnClearList(wxCommandEvent& event);
	void	__OnStartSimulation(wxCommandEvent& event);
	void	__OnSpinAWGN(wxSpinDoubleEvent& event);

	CLDPCMan*				m_pMan;
	std::pair<int, int>		m_Hdims;

private:
	wxDECLARE_NO_COPY_CLASS(CMainPanel);

	// Object that wants to respond to events
	DECLARE_EVENT_TABLE()
};
#endif