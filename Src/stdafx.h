#ifndef __STDAFX_H__
#define __STDAFX_H__
#pragma once

//
// Controllo presenza UNICODE
//
#if !defined( _UNICODE )
#error "UNICODE not defined"
#endif
#include <yvals.h>
#include <stdint.h>													// Visual Studio 2010 include stdint.h
#include <inttypes.h>
#include <limits>
								

// File header supplementari da aggiungere in base alle funzionalita' implementate nell'applicazione
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// For all others, include the necessary headers (this file is usually all you need
// because it includes almost all "standard" wxWidgets headers )
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif


#include <wx/event.h>
#include <wx/socket.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>
#include <wx/rawbmp.h>
#include <wx/taskbar.h>
#include <wx/bookctrl.h>
#include <wx/listctrl.h>
#include "wx/imaglist.h"
#include <wx/aboutdlg.h>
#include "wx/dcbuffer.h"
#include "wx/dcgraph.h"
#include "wx/xrc/xmlres.h"
#include "wx/dir.h"

#include <sys/timeb.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <Wincrypt.h>

#include <list>
#include <map>
#include <string>
#include <vector>

#include <opencv2/world.hpp>
#include <opencv2/highgui.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

#endif