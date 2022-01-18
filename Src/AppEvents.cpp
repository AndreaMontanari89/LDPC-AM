#include "stdafx.h"
#include "LDPC-AM.h"

//
// Eventi di segnalazione derivati da wxCommandEvent
//
DEFINE_EVENT_TYPE(wxEVT_APP_INIT_LDPC_OBJ)						// Evento per inizializzazione oggetto CLDPCMan
DEFINE_EVENT_TYPE(wxEVT_START_SIMULATION)
DEFINE_EVENT_TYPE(wxEVT_H_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_BIN_IMG_SELECTED)
