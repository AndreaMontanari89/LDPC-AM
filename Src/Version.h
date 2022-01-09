//
// Nome file:	Version.h
//
// Descrizione:	Versione applicazione / libreria
//
// Storia:
// Data		Autore	Commento
// ---------------------------------------------------------------------------
// 22-10-14	SR		Prima stesura
//
#ifndef __VERSION_H__
#define __VERSION_H__
#pragma once

//
// Versione compilatore
//
#if ( _MSC_VER >= 1900 )
#define VER_COMPILER_STR			"VC15"
#else
#define VER_COMPILER_STR			"VC9"
#endif

//
// Versione applicazione / libreria
//
// Le nuove versioni compatibili a livello binario possono mantenere
// i valori correnti di VERSION_MAJOR e VERSION_MINOR
//
// Come promemoria si include la seguente spiegazione tratta da
//		http://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html
//
/*
	3.6. Incompatible Libraries

	When a new version of a library is binary-incompatible with the old one the soname needs to change.
	In C, there are four basic reasons that a library would cease to be binary compatible:
		1)	The behavior of a function changes so that it no longer meets its original specification,
		2)	Exported data items change (exception: adding optional items to the ends of structures is okay,
			as long as those structures are only allocated within the library).
		3)	An exported function is removed.
		4)	The interface of an exported function changes.

	If you can avoid these reasons, you can keep your libraries binary-compatible.
	Said another way, you can keep your Application Binary Interface (ABI) compatible if you avoid such changes.
	For example, you might want to add new functions but not delete the old ones. You can add items to structures but only
	if you can make sure that old programs won't be sensitive to such changes by adding items only to the end of the structure,
	only allowing the library (and not the application) to allocate the structure, making the extra items optional (or having
	the library fill them in), and so on. Watch out - you probably can't expand structures if users are using them in arrays.

	For C++ (and other languages supporting compiled-in templates and/or compiled dispatched methods), the situation is trickier.
	All of the above issues apply, plus many more issues. The reason is that some information is implemented ``under the covers''
	in the compiled code, resulting in dependencies that may not be obvious if you don't know how C++ is typically implemented.
	Strictly speaking, they aren't ``new'' issues, it's just that compiled C++ code invokes them in ways that may be surprising to you.
	The following is a (probably incomplete) list of things that you cannot do in C++ and retain binary compatibility,
	as reported by Troll Tech's Technical FAQ:

		1)	add reimplementations of virtual functions (unless it it safe for older binaries to call the original implementation),
			because the compiler evaluates SuperClass::virtualFunction() calls at compile-time (not link-time).
		2)	add or remove virtual member functions, because this would change the size and layout of the vtbl of every subclass.
		3)	change the type of any data members or move any data members that can be accessed via inline member functions.
		4)	change the class hierarchy, except to add new leaves.
		5)	add or remove private data members, because this would change the size and layout of every subclass.
		6)	remove public or protected member functions unless they are inline.
		7)	make a public or protected member function inline.
		8)	change what an inline function does, unless the old version continues working.
		9)	change the access rights (i.e. public, protected or private) of a member function in a portable program,
			because some compilers mangle the access rights into the function name.

*/

#define FILE_VERSION_MAJOR			2
#define FILE_VERSION_MINOR			1
#define FILE_VERSION_REVISION		2
#define FILE_VERSION_BUILD			4

#define FILE_VERSION_BUILD_DATE		"2021-12-10"
#define FILE_VERSION_BUILD_TIME		"13:00:00"

#define PRODUCT_VERSION_MAJOR		FILE_VERSION_MAJOR
#define PRODUCT_VERSION_MINOR		FILE_VERSION_MINOR
#define PRODUCT_VERSION_REVISION	FILE_VERSION_REVISION
#define PRODUCT_VERSION_BUILD		FILE_VERSION_BUILD

//
// Commento, descrizione applicazione e copyright
//
#define VER_COMMENT					"File built on " VER_FILE_VERSION_STR " - " FILE_VERSION_BUILD_DATE " - " FILE_VERSION_BUILD_TIME

#define VER_FILE_DESCRIPTION_STR	"LDPC-AM Simulator - " VER_COMPILER_STR
#define VER_PRODUCTNAME_STR			"LDPC"

#define VER_COPYRIGHT_STR			"Copyright (C) Andrea Montanari."


//
// Controllo coerenza identificatore versione e variabili progetto
//
#if ( defined( P_VER_MAJOR ) && FILE_VERSION_MAJOR == P_VER_MAJOR )
#error "Incoerenza identificatore versione / variabili progetto"
#endif
#if ( defined( P_VER_MINOR ) && FILE_VERSION_MINOR == P_VER_MINOR )
#error "Incoerenza identificatore versione / variabili progetto"
#endif


// Macro utilizzate per esportare la versione come stringhe ASCII
#define VER_STR_EXPAND(tok) #tok
#define VER_STREXPAND(tok) VER_STR_EXPAND(tok)

#define VER_FILE_VERSION			FILE_VERSION_MAJOR, FILE_VERSION_MINOR, FILE_VERSION_REVISION, FILE_VERSION_BUILD
#define VER_FILE_VERSION_STR		VER_STREXPAND(FILE_VERSION_MAJOR)        \
									"." VER_STREXPAND(FILE_VERSION_MINOR)    \
									"." VER_STREXPAND(FILE_VERSION_REVISION) \
									"." VER_STREXPAND(FILE_VERSION_BUILD)

#define VER_PRODUCT_VERSION			PRODUCT_VERSION_MAJOR, PRODUCT_VERSION_MINOR, PRODUCT_VERSION_REVISION, PRODUCT_VERSION_BUILD
#define VER_PRODUCT_VERSION_STR		VER_STREXPAND(PRODUCT_VERSION_MAJOR)        \
									"." VER_STREXPAND(PRODUCT_VERSION_MINOR)    \
									"." VER_STREXPAND(PRODUCT_VERSION_REVISION) \
									"." VER_STREXPAND(PRODUCT_VERSION_BUILD)

#if defined( _WIN32 ) || defined( _WIN64 )
#define VER_ORIGINAL_FILENAME_STR	VER_PRODUCTNAME_STR ".exe"
#else
#define VER_ORIGINAL_FILENAME_STR	VER_PRODUCTNAME_STR
#endif	// defined( _WIN32 ) || defined( _WIN64 )
#define VER_INTERNAL_NAME_STR		VER_PRODUCTNAME_STR

#if defined( _WIN32 ) || defined( _WIN64 )

#define VER_FILE_FLAGS_MASK			VS_FFI_FILEFLAGSMASK
#ifdef _DEBUG
	#define VER_DEBUG				VS_FF_DEBUG
	#define VER_PRIVATE_BUILD		VS_FF_PRIVATEBUILD
	#define VER_PRE_RELEASE			( VS_FF_PRERELEASE | VS_FF_SPECIALBUILD )
#else
	#define VER_DEBUG				0
	#define VER_PRIVATE_BUILD		0
	#define VER_PRE_RELEASE			0
#endif
#define VER_FILE_FLAGS				( VER_DEBUG | VER_PRIVATE_BUILD | VER_PRE_RELEASE )

#define VER_FILE_OS					VOS_NT_WINDOWS32
#define VER_FILE_TYPE				VFT_APP							// VFT_APP, VFT_DLL, VFT_STATIC_LIB o VFT_UNKNOWN
#define VER_FILE_SUBTYPE			0

#endif	// defined( _WIN32 ) || defined( _WIN64 )

#endif // __VERSION_H__
