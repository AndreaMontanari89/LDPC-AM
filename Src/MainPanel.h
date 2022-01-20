#ifndef __MAINPANEL__
#define __MAINPANEL__
#pragma once

#include "MainPanel_Base.h"
class CLDPCMan;


class CMainPanel : public CMainPanel_Base
{
	class GenThread : public wxThread
	{
	public:
		GenThread(CMainPanel* handler)
			: wxThread(wxTHREAD_DETACHED)
		{
			m_pHandler = handler;
		}
		~GenThread() {};
	protected:
		virtual ExitCode Entry();
		CMainPanel* m_pHandler;
	};

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
	void    __OnPickBinImage(wxFileDirPickerEvent& event);
	void	__OnWordChar(wxKeyEvent& event);
	void	__OnClearList(wxCommandEvent& event);
	void	__OnStartSimulation(wxCommandEvent& event);
	void	__OnSpinAWGN(wxSpinDoubleEvent& event);
	void	__OnSpinBSC(wxSpinDoubleEvent& event);
	void	__OnViewH(wxCommandEvent& event);
	void	__OnViewImgs(wxCommandEvent& event);
	void	__OnStopSimulation(wxCommandEvent& event);
	void	__OnSpnAttempt(wxSpinEvent& event);
	void	__OnCleanRes(wxCommandEvent& event);
	void	__OnSimulationFinished(wxCommandEvent& event);

public:
	void	BatchFinished();
	void	SimulationFinished( int bits, int errbits, double channel_par);
	void	GenerationFinished();
	void	DoGenerate();
	void	StartGeneration();
	void	SetInfo(wxString info) { m_txtinfo->SetLabel(info); };

	CLDPCMan*				m_pMan;
	std::pair<int, int>		m_Hdims;
	GenThread*				m_pThread;

	wxArrayString			m_asStringWords;

private:
	wxDECLARE_NO_COPY_CLASS(CMainPanel);

	// Object that wants to respond to events
	DECLARE_EVENT_TABLE()
};
#endif