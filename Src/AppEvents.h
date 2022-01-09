#ifndef __APPEVENTS_H__
#define __APPEVENTS_H__
#pragma once


//
// Definizione timer utente
//
enum _AppTimer
{
	TID_Example = ID_LAST_RESOURCE + 1,
};

DECLARE_LOCAL_EVENT_TYPE(wxEVT_APP_INIT_LDPC_OBJ, wxID_ANY )				// Evento per inizializzazione oggetto CLDPCMan

#endif	// __APPEVENTS_H__
