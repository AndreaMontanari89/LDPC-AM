///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/spinctrl.h>
#include <wx/statbmp.h>
#include <wx/gauge.h>
#include <wx/listctrl.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CMainPanel_Base
///////////////////////////////////////////////////////////////////////////////
class CMainPanel_Base : public wxPanel
{
	private:

	protected:
		wxPanel* m_pnlMatriceParita;
		wxStaticText* m_staticText5;
		wxFilePickerCtrl* m_filePicker1;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_txtK;
		wxStaticText* m_staticText10;
		wxStaticText* m_staticText7;
		wxTextCtrl* m_txtN;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_txtM;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText81;
		wxTextCtrl* m_txtKN;
		wxStaticText* m_staticText121;
		wxButton* m_btnDrawTanner;
		wxButton* m_btnViewH;
		wxStaticLine* m_staticline2;
		wxPanel* m_pnlChannel;
		wxRadioButton* m_rdAWGN;
		wxStaticText* m_staticText25;
		wxSpinCtrlDouble* m_spnVarAWGN;
		wxStaticLine* m_staticline5;
		wxRadioButton* m_rdBSC;
		wxStaticText* m_staticText251;
		wxSpinCtrlDouble* m_spnPeBSC;
		wxStaticLine* m_staticline3;
		wxStaticBitmap* m_bmpChannel;
		wxStaticLine* m_staticline16;
		wxPanel* m_pnlFiller;
		wxRadioButton* m_rdRandom;
		wxRadioButton* m_rdInsertWord;
		wxTextCtrl* m_txtInsertWord;
		wxStaticLine* m_staticline611;
		wxStaticText* m_staticText1411;
		wxSpinCtrl* m_spnRep;
		wxStaticLine* m_staticline71;
		wxPanel* m_pnlCtrlGen;
		wxButton* m_btnIsertWord;
		wxButton* m_button31;
		wxStaticLine* m_staticline711;
		wxStaticText* m_staticText51;
		wxFilePickerCtrl* m_filePicker2;
		wxStaticLine* m_staticline7111;
		wxPanel* m_pnlLoadGen;
		wxStaticLine* m_staticline6;
		wxPanel* m_pnlSim;
		wxPanel* m_pnlCfg;
		wxPanel* m_pnlSimCfg;
		wxButton* m_bpAvviaSimulazione;
		wxButton* m_bpAvviaSimulazione1;
		wxButton* m_btnShowImages;
		wxStaticText* m_staticText19;
		wxSpinCtrl* m_spnAttmpt;
		wxGauge* m_gauge2;
		wxStaticText* m_txtinfo;
		wxListCtrl* m_listBoxWord;
		wxStaticLine* m_staticline62;
		wxPanel* m_pnlRes;
		wxButton* m_btnPulisciRisultato;
		wxListCtrl* m_listResult;

		// Virtual event handlers, override them in your derived class
		virtual void __OnPickH( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void __OnDrawTanner( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnViewH( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnChannel( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnSpinAWGN( wxSpinDoubleEvent& event ) { event.Skip(); }
		virtual void __OnSpinBSC( wxSpinDoubleEvent& event ) { event.Skip(); }
		virtual void __OnChangeSimulation( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnWordChar( wxKeyEvent& event ) { event.Skip(); }
		virtual void __OnInsertWord( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnClearList( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnPickBinImage( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void __OnStartSimulation( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnStopSimulation( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnViewImgs( wxCommandEvent& event ) { event.Skip(); }
		virtual void __OnSpnAttempt( wxSpinEvent& event ) { event.Skip(); }
		virtual void __OnCleanRes( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxStaticText* m_lblMatTitle;
		wxStaticText* m_lblNrBitParity;
		wxStaticText* m_lblChTitle;
		wxStaticText* m_lblChTitle1;
		wxGauge* m_gauge3;
		wxStaticText* m_lblMatTitle1;
		wxStaticText* m_lblMatTitle11;

		CMainPanel_Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1271,862 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );

		~CMainPanel_Base();

};

