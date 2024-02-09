// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
public:
	//afx_msg void OnOpenImage();
	afx_msg void OnSaveState();
	afx_msg void OnLoadState();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

