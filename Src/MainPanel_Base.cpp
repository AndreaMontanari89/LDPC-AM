///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MainPanel_Base.h"

///////////////////////////////////////////////////////////////////////////

CMainPanel_Base::CMainPanel_Base( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bHeadSizer;
	bHeadSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pnlMatriceParita = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_lblMatTitle = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("Matrice di controllo parità"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMatTitle->Wrap( -1 );
	m_lblMatTitle->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer3->Add( m_lblMatTitle, 0, wxALL, 5 );

	m_staticText5 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("File in formato alist (*.alist)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer3->Add( m_staticText5, 0, wxALL, 5 );

	m_filePicker1 = new wxFilePickerCtrl( m_pnlMatriceParita, wxID_ANY, wxEmptyString, wxT("Seleziona una matrice"), wxT("*alist"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN );
	m_filePicker1->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_filePicker1->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_filePicker1->SetMinSize( wxSize( 480,-1 ) );

	bSizer3->Add( m_filePicker1, 0, wxALL, 5 );

	m_staticline1 = new wxStaticLine( m_pnlMatriceParita, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText6 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("Numero di bit di informazione"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_txtK = new wxTextCtrl( m_pnlMatriceParita, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_txtK->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_txtK->SetMaxSize( wxSize( 50,-1 ) );

	fgSizer1->Add( m_txtK, 0, wxALL, 5 );

	m_staticText10 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("k"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText7 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("Lunghezza parola di codice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	m_staticText7->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_txtN = new wxTextCtrl( m_pnlMatriceParita, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_txtN->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_txtN->SetMaxSize( wxSize( 50,-1 ) );

	fgSizer1->Add( m_txtN, 0, wxALL, 5 );

	m_staticText11 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	m_staticText11->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lblNrBitParity = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("Numero bit di parità"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblNrBitParity->Wrap( -1 );
	m_lblNrBitParity->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer1->Add( m_lblNrBitParity, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_txtM = new wxTextCtrl( m_pnlMatriceParita, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_txtM->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_txtM->SetMaxSize( wxSize( 50,-1 ) );

	fgSizer1->Add( m_txtM, 0, wxALL, 5 );

	m_staticText12 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("m"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText81 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("Code Ratio (Rc)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	m_staticText81->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText81, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_txtKN = new wxTextCtrl( m_pnlMatriceParita, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_txtKN->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_txtKN->SetMaxSize( wxSize( 50,-1 ) );

	fgSizer1->Add( m_txtKN, 0, wxALL, 5 );

	m_staticText121 = new wxStaticText( m_pnlMatriceParita, wxID_ANY, wxT("k/n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	m_staticText121->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	fgSizer1->Add( m_staticText121, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer7->Add( fgSizer1, 0, 0, 5 );


	bSizer7->Add( 20, 0, 0, wxEXPAND, 5 );

	m_btnDrawTanner = new wxButton( m_pnlMatriceParita, wxID_ANY, wxT("Disegna \n grafo \ndi Tanner"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnDrawTanner->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer7->Add( m_btnDrawTanner, 0, wxALL|wxEXPAND, 5 );

	m_btnViewH = new wxButton( m_pnlMatriceParita, wxID_ANY, wxT("Visualizza\nMatrice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnViewH->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer7->Add( m_btnViewH, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( bSizer7, 0, 0, 0 );


	m_pnlMatriceParita->SetSizer( bSizer3 );
	m_pnlMatriceParita->Layout();
	bSizer3->Fit( m_pnlMatriceParita );
	bHeadSizer->Add( m_pnlMatriceParita, 0, wxEXPAND | wxALL, 0 );

	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bHeadSizer->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_pnlChannel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_lblChTitle = new wxStaticText( m_pnlChannel, wxID_ANY, wxT("Proprietà canale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChTitle->Wrap( -1 );
	m_lblChTitle->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer9->Add( m_lblChTitle, 0, wxALL, 5 );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	m_rdAWGN = new wxRadioButton( m_pnlChannel, wxID_ANY, wxT("AWGN"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rdAWGN->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_rdAWGN, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText25 = new wxStaticText( m_pnlChannel, wxID_ANY, wxT("Varianza"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	m_staticText25->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer12->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spnVarAWGN = new wxSpinCtrlDouble( m_pnlChannel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS, 0, 10, 0.000000, 0.01 );
	m_spnVarAWGN->SetDigits( 2 );
	bSizer12->Add( m_spnVarAWGN, 0, wxALL, 5 );


	bSizer11->Add( bSizer12, 1, wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer10->Add( bSizer11, 1, 0, 5 );

	m_staticline5 = new wxStaticLine( m_pnlChannel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer10->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );

	m_rdBSC = new wxRadioButton( m_pnlChannel, wxID_ANY, wxT("BSC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rdBSC->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer111->Add( m_rdBSC, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText251 = new wxStaticText( m_pnlChannel, wxID_ANY, wxT("Pe"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText251->Wrap( -1 );
	m_staticText251->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer121->Add( m_staticText251, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spnPeBSC = new wxSpinCtrlDouble( m_pnlChannel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxSP_ARROW_KEYS, 0, 1, 0.000000, 0.1 );
	m_spnPeBSC->SetDigits( 4 );
	bSizer121->Add( m_spnPeBSC, 0, wxALL, 5 );


	bSizer111->Add( bSizer121, 1, wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer10->Add( bSizer111, 1, wxALIGN_CENTER_VERTICAL, 5 );


	bSizer9->Add( bSizer10, 0, wxEXPAND, 5 );

	m_staticline3 = new wxStaticLine( m_pnlChannel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer9->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );

	m_bmpChannel = new wxStaticBitmap( m_pnlChannel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_bmpChannel, 0, wxALL|wxEXPAND, 5 );


	m_pnlChannel->SetSizer( bSizer9 );
	m_pnlChannel->Layout();
	bSizer9->Fit( m_pnlChannel );
	bHeadSizer->Add( m_pnlChannel, 0, wxALL|wxEXPAND, 0 );

	m_staticline16 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bHeadSizer->Add( m_staticline16, 0, wxEXPAND | wxALL, 5 );

	m_pnlFiller = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer141;
	bSizer141 = new wxBoxSizer( wxVERTICAL );

	m_lblChTitle1 = new wxStaticText( m_pnlFiller, wxID_ANY, wxT("Riempimento simulazione"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblChTitle1->Wrap( -1 );
	m_lblChTitle1->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer141->Add( m_lblChTitle1, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_rdRandom = new wxRadioButton( m_pnlFiller, wxID_ANY, wxT("Genera random"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rdRandom->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer21->Add( m_rdRandom, 0, wxALL, 5 );


	fgSizer21->Add( 0, 0, 1, wxEXPAND, 5 );

	m_rdInsertWord = new wxRadioButton( m_pnlFiller, wxID_ANY, wxT("Inserisci parola"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rdInsertWord->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer21->Add( m_rdInsertWord, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_txtInsertWord = new wxTextCtrl( m_pnlFiller, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtInsertWord->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	fgSizer21->Add( m_txtInsertWord, 0, wxALL, 5 );


	bSizer141->Add( fgSizer21, 0, wxEXPAND, 5 );

	m_staticline611 = new wxStaticLine( m_pnlFiller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer141->Add( m_staticline611, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxHORIZONTAL );

	m_staticText1411 = new wxStaticText( m_pnlFiller, wxID_ANY, wxT("Ripeti"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1411->Wrap( -1 );
	m_staticText1411->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer1511->Add( m_staticText1411, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spnRep = new wxSpinCtrl( m_pnlFiller, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 0 );
	m_spnRep->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer1511->Add( m_spnRep, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer141->Add( bSizer1511, 0, wxEXPAND, 5 );

	m_staticline71 = new wxStaticLine( m_pnlFiller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer141->Add( m_staticline71, 0, wxEXPAND | wxALL, 5 );

	m_pnlCtrlGen = new wxPanel( m_pnlFiller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer241;
	bSizer241 = new wxBoxSizer( wxHORIZONTAL );

	m_btnIsertWord = new wxButton( m_pnlCtrlGen, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnIsertWord->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer241->Add( m_btnIsertWord, 0, wxALL, 5 );

	m_button31 = new wxButton( m_pnlCtrlGen, wxID_ANY, wxT("Pulisci"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button31->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer241->Add( m_button31, 0, wxALL, 5 );


	m_pnlCtrlGen->SetSizer( bSizer241 );
	m_pnlCtrlGen->Layout();
	bSizer241->Fit( m_pnlCtrlGen );
	bSizer141->Add( m_pnlCtrlGen, 0, wxEXPAND | wxALL, 5 );

	m_staticline711 = new wxStaticLine( m_pnlFiller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer141->Add( m_staticline711, 0, wxEXPAND | wxALL, 5 );

	m_staticText51 = new wxStaticText( m_pnlFiller, wxID_ANY, wxT("Usa un'immagine"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	m_staticText51->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer141->Add( m_staticText51, 0, wxALL, 5 );

	m_filePicker2 = new wxFilePickerCtrl( m_pnlFiller, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_filePicker2->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_filePicker2->SetMinSize( wxSize( 450,-1 ) );

	bSizer141->Add( m_filePicker2, 0, wxALL, 5 );

	m_staticline7111 = new wxStaticLine( m_pnlFiller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer141->Add( m_staticline7111, 0, wxEXPAND | wxALL, 5 );

	m_pnlLoadGen = new wxPanel( m_pnlFiller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	m_gauge3 = new wxGauge( m_pnlLoadGen, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,-1 ), wxGA_HORIZONTAL );
	m_gauge3->SetValue( 0 );
	bSizer22->Add( m_gauge3, 1, wxALL|wxEXPAND, 5 );


	m_pnlLoadGen->SetSizer( bSizer22 );
	m_pnlLoadGen->Layout();
	bSizer22->Fit( m_pnlLoadGen );
	bSizer141->Add( m_pnlLoadGen, 1, wxEXPAND | wxALL, 0 );


	m_pnlFiller->SetSizer( bSizer141 );
	m_pnlFiller->Layout();
	bSizer141->Fit( m_pnlFiller );
	bHeadSizer->Add( m_pnlFiller, 1, wxEXPAND | wxALL, 5 );


	bSizer21->Add( bHeadSizer, 0, wxEXPAND, 5 );

	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer21->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSimSizer;
	bSimSizer = new wxBoxSizer( wxVERTICAL );

	m_pnlSim = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_pnlSim->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );

	m_lblMatTitle1 = new wxStaticText( m_pnlSim, wxID_ANY, wxT("Simulazione"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMatTitle1->Wrap( -1 );
	m_lblMatTitle1->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer25->Add( m_lblMatTitle1, 0, wxALL, 5 );

	m_pnlCfg = new wxPanel( m_pnlSim, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );

	m_pnlSimCfg = new wxPanel( m_pnlCfg, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );

	m_bpAvviaSimulazione = new wxButton( m_pnlSimCfg, wxID_ANY, wxT("Avvia"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_bpAvviaSimulazione, 0, wxALL, 5 );

	m_bpAvviaSimulazione1 = new wxButton( m_pnlSimCfg, wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_bpAvviaSimulazione1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer20->Add( 50, 0, 0, wxEXPAND, 5 );

	m_btnShowImages = new wxButton( m_pnlSimCfg, wxID_ANY, wxT("Visualizza imm."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_btnShowImages, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer20->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText19 = new wxStaticText( m_pnlSimCfg, wxID_ANY, wxT("Iterazioni massime per decodifica:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	m_staticText19->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer20->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_spnAttmpt = new wxSpinCtrl( m_pnlSimCfg, wxID_ANY, wxT("50"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 50 );
	m_spnAttmpt->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer20->Add( m_spnAttmpt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_pnlSimCfg->SetSizer( bSizer20 );
	m_pnlSimCfg->Layout();
	bSizer20->Fit( m_pnlSimCfg );
	bSizer26->Add( m_pnlSimCfg, 1, wxEXPAND | wxALL, 0 );

	m_gauge2 = new wxGauge( m_pnlCfg, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,-1 ), wxGA_HORIZONTAL );
	m_gauge2->SetValue( 0 );
	bSizer26->Add( m_gauge2, 0, wxALL|wxEXPAND, 5 );

	m_txtinfo = new wxStaticText( m_pnlCfg, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtinfo->Wrap( -1 );
	m_txtinfo->SetFont( wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer26->Add( m_txtinfo, 0, wxALL, 5 );

	m_listBoxWord = new wxListCtrl( m_pnlCfg, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bSizer26->Add( m_listBoxWord, 0, wxALL|wxEXPAND, 5 );


	m_pnlCfg->SetSizer( bSizer26 );
	m_pnlCfg->Layout();
	bSizer26->Fit( m_pnlCfg );
	bSizer25->Add( m_pnlCfg, 0, wxEXPAND | wxALL, 5 );


	m_pnlSim->SetSizer( bSizer25 );
	m_pnlSim->Layout();
	bSizer25->Fit( m_pnlSim );
	bSimSizer->Add( m_pnlSim, 0, wxEXPAND | wxALL, 0 );

	m_staticline62 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSimSizer->Add( m_staticline62, 0, wxEXPAND | wxALL, 5 );


	bSizer21->Add( bSimSizer, 0, wxEXPAND, 5 );

	wxBoxSizer* bSimSizer1;
	bSimSizer1 = new wxBoxSizer( wxVERTICAL );

	m_pnlRes = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer251;
	bSizer251 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer211;
	bSizer211 = new wxBoxSizer( wxHORIZONTAL );

	m_lblMatTitle11 = new wxStaticText( m_pnlRes, wxID_ANY, wxT("Risultati"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lblMatTitle11->Wrap( -1 );
	m_lblMatTitle11->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer211->Add( m_lblMatTitle11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_btnPulisciRisultato = new wxButton( m_pnlRes, wxID_ANY, wxT("Pulisci"), wxDefaultPosition, wxDefaultSize, 0 );
	m_btnPulisciRisultato->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer211->Add( m_btnPulisciRisultato, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer251->Add( bSizer211, 1, wxEXPAND, 5 );

	m_listResult = new wxListCtrl( m_pnlRes, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_listResult->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer251->Add( m_listResult, 1, wxALL|wxEXPAND, 5 );


	m_pnlRes->SetSizer( bSizer251 );
	m_pnlRes->Layout();
	bSizer251->Fit( m_pnlRes );
	bSimSizer1->Add( m_pnlRes, 0, wxEXPAND | wxALL, 0 );


	bSizer21->Add( bSimSizer1, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer21 );
	this->Layout();

	// Connect Events
	m_filePicker1->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( CMainPanel_Base::__OnPickH ), NULL, this );
	m_btnDrawTanner->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnDrawTanner ), NULL, this );
	m_btnViewH->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnViewH ), NULL, this );
	m_rdAWGN->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChannel ), NULL, this );
	m_spnVarAWGN->Connect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( CMainPanel_Base::__OnSpinAWGN ), NULL, this );
	m_rdBSC->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChannel ), NULL, this );
	m_spnPeBSC->Connect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( CMainPanel_Base::__OnSpinBSC ), NULL, this );
	m_rdRandom->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChangeSimulation ), NULL, this );
	m_rdInsertWord->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChangeSimulation ), NULL, this );
	m_txtInsertWord->Connect( wxEVT_CHAR, wxKeyEventHandler( CMainPanel_Base::__OnWordChar ), NULL, this );
	m_btnIsertWord->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnInsertWord ), NULL, this );
	m_button31->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnClearList ), NULL, this );
	m_filePicker2->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( CMainPanel_Base::__OnPickBinImage ), NULL, this );
	m_bpAvviaSimulazione->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnStartSimulation ), NULL, this );
	m_bpAvviaSimulazione1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnStopSimulation ), NULL, this );
	m_btnShowImages->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnViewImgs ), NULL, this );
	m_spnAttmpt->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CMainPanel_Base::__OnSpnAttempt ), NULL, this );
	m_btnPulisciRisultato->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnCleanRes ), NULL, this );
}

CMainPanel_Base::~CMainPanel_Base()
{
	// Disconnect Events
	m_filePicker1->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( CMainPanel_Base::__OnPickH ), NULL, this );
	m_btnDrawTanner->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnDrawTanner ), NULL, this );
	m_btnViewH->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnViewH ), NULL, this );
	m_rdAWGN->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChannel ), NULL, this );
	m_spnVarAWGN->Disconnect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( CMainPanel_Base::__OnSpinAWGN ), NULL, this );
	m_rdBSC->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChannel ), NULL, this );
	m_spnPeBSC->Disconnect( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxSpinDoubleEventHandler( CMainPanel_Base::__OnSpinBSC ), NULL, this );
	m_rdRandom->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChangeSimulation ), NULL, this );
	m_rdInsertWord->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( CMainPanel_Base::__OnChangeSimulation ), NULL, this );
	m_txtInsertWord->Disconnect( wxEVT_CHAR, wxKeyEventHandler( CMainPanel_Base::__OnWordChar ), NULL, this );
	m_btnIsertWord->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnInsertWord ), NULL, this );
	m_button31->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnClearList ), NULL, this );
	m_filePicker2->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( CMainPanel_Base::__OnPickBinImage ), NULL, this );
	m_bpAvviaSimulazione->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnStartSimulation ), NULL, this );
	m_bpAvviaSimulazione1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnStopSimulation ), NULL, this );
	m_btnShowImages->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnViewImgs ), NULL, this );
	m_spnAttmpt->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CMainPanel_Base::__OnSpnAttempt ), NULL, this );
	m_btnPulisciRisultato->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainPanel_Base::__OnCleanRes ), NULL, this );

}
