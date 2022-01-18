#ifndef __APPEVENTS_H__
#define __APPEVENTS_H__
#pragma once
#include "Resource.h"

//
// Definizione timer utente
//
enum _AppTimer
{
	TID_Example = ID_LAST_RESOURCE + 1,
};

DECLARE_LOCAL_EVENT_TYPE(wxEVT_APP_INIT_LDPC_OBJ, wxID_ANY )				// Evento per inizializzazione oggetto CLDPCMan
DECLARE_LOCAL_EVENT_TYPE(wxEVT_START_SIMULATION, wxID_ANY)
DECLARE_LOCAL_EVENT_TYPE(wxEVT_H_SELECTED, wxID_ANY)	
DECLARE_LOCAL_EVENT_TYPE(wxEVT_BIN_IMG_SELECTED, wxID_ANY)
#endif	// __APPEVENTS_H__
