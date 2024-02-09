#pragma once


// CViewOptions dialog

class CViewOptionsDlg : public DialogParentClass
{
	DECLARE_DYNAMIC(CViewOptionsDlg)

public:
	CViewOptionsDlg(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CViewOptionsDlg();

// Dialog Data
	enum { IDD = IDD_VIEW_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
