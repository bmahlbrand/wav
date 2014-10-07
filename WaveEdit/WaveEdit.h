
// WaveEdit.h : main header file for the WaveEdit application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "WaveFile.h"


// CWaveEditApp:
// See WaveEdit.cpp for the implementation of this class
//

class CWaveEditApp : public CWinAppEx
{
public:
	CWaveEditApp();
	~CWaveEditApp();
	
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWaveEditApp theApp;
