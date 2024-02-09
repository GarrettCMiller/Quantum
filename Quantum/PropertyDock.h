#pragma once



// CPropertyDock

class CPropertyDock : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertyDock)

public:
	CPropertyDock() { }
	CPropertyDock(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertyDock(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropertyDock();

protected:
	DECLARE_MESSAGE_MAP()
};


