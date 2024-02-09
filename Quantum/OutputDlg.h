#pragma once

#include "afxcmn.h"
#include "resource.h"
#include "stdafx.h"

// COutputDlg dialog

class COutputDlg : public DialogParentClass
{
	DECLARE_DYNAMIC(COutputDlg)

public:
	COutputDlg(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~COutputDlg();

	static Logger	m_Logger;

// Dialog Data
	enum { IDD = IDD_OUTPUT_WINDOW };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strConsoleCmd;
	virtual BOOL OnInitDialog();

	CRichEditCtrl m_Console;

	virtual void OnOK();
	afx_msg void OnPaint();
public:
	afx_msg void OnEnChangeConsoleInput();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};