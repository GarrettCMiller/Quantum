#pragma once


// CVectorProps dialog

class CVectorProps : public CDialog
{
	DECLARE_DYNAMIC(CVectorProps)

public:
	CVectorProps(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVectorProps();

// Dialog Data
	enum { IDD = IDD_VECTOR_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
