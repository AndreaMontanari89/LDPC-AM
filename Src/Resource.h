//
// Nome file:	Resource.h
//
// Descrizione:	File header identificatori risorse
//
// Storia:
// Data		Autore	Commento
// ---------------------------------------------------------------------------
// 22-10-14	SR		Prima stesura
//
#ifndef __RESOURCE_H__
#define __RESOURCE_H__
#pragma once

enum _ResourceID
{
	// Identificatori icone, bitmap, etc.
	IDR_APP_MAIN_ICON = 128,
	IDB_MSG_LOG_IMAGELIST,

	// Identificatori socket

	// L'intervallo da ID_LAST_RESOURCE a wxID_LOWEST ( attualmente 4999 ) e' stato riservato
	// per gli identificatori eventi ( timer, etc. )
	ID_LAST_RESOURCE = 2000,

	// Identificatori aventi valori uguali a quelli gia' presenti tra wxID_LOWEST e wxID_HIGHEST
	IDM_APP_EXIT = wxID_EXIT,
	IDM_APP_ABOUT = wxID_ABOUT,

	// Identificatori finestre e controlli child
	IDW_MAINFRAME = wxID_HIGHEST + 1,
};

#endif	// __RESOURCE_H__
