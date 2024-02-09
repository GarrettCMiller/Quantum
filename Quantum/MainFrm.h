// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ChildView.h"

#include "OutputDlg.h"

#include "MovementDlg.h"
#include "ColorDlg.h"
#include "GenProps.h"
#include "AppearanceDlg.h"

#include "StatisticsDlg.h"

#include "ViewOptions.h"

#include "PhysProps.h"

#include "ParticleSystemsDlg.h"

#include "PropertyDock.h"

#include "VectorProps.h"
#include "WaveProperties.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView    m_wndView;

	bool			m_bGenPropsDlg;
	bool			m_bColorDlg;
	bool			m_bMovementDlg;

public:
	//Util Dialogs
	CPropertyDock	m_PropsDock;
	CPropertyDock	m_UtilDock;

	COutputDlg		m_OutputDlg;
	CStatisticsDlg	m_StatsDlg;

	//Prop Dialogs
	CMovementDlg	m_MovementDlg;
	CColorDlg		m_ColorDlg;
	CGenProps		m_GenPropsDlg;
	CAppearanceDlg	m_AppearanceDlg;
	CPhysPropsDlg	m_PhysPropsDlg;

	//Misc Dialogs
	CViewOptionsDlg	m_ViewOptionsDlg;
	CParticleSystemsDlg	m_ParticleSystemsDlg;

	CVectorProps	m_VectorPropsDlg;
	CWaveProperties	m_WavePropsDlg;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnOpenImage();

	void SetDlgProps();
	void CreateUtilityDock();

	afx_msg void OnViewGeneraloptions();
	afx_msg void OnUpdateViewGeneraloptions(CCmdUI *pCmdUI);
	afx_msg void OnViewMovementoptions();
	afx_msg void OnUpdateViewMovementoptions(CCmdUI *pCmdUI);
	afx_msg void OnViewColoroptions();
	afx_msg void OnUpdateViewColoroptions(CCmdUI *pCmdUI);
	afx_msg void OnViewAppearanceoptions();
	afx_msg void OnUpdateViewAppearanceoptions(CCmdUI *pCmdUI);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


