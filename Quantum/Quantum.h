// Quantum.h : main header file for the Quantum application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "DynamicArray.h"

class CMainFrame;

// CQuantumApp:
// See Quantum.cpp for the implementation of this class
//
class CThread;

class CQuantumApp : public CWinApp
{
public:
	CQuantumApp();
	~CQuantumApp();

	CMainFrame*		m_pFrame;

	EmitterList		m_Emitters;

	static Bool		m_bActive;
	static Bool		m_bQuit;
	static Bool		m_bInLoop;

	static CThread*	m_pThread;

	static Bool		m_bDraggingTabs;
	static CWnd*	m_pDraggingTab;

	static DWORD MainUpdateLoop(LPVOID data);
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
public:
	virtual int Run();
};

extern CQuantumApp theApp;