#pragma once
#include "afxwin.h"


// CAppearanceDlg dialog

class CAppearanceDlg : public DialogParentClass
{
	DECLARE_DYNAMIC(CAppearanceDlg)

public:
	CAppearanceDlg(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CAppearanceDlg();

// Dialog Data
	enum { IDD = IDD_PARTICLE_APPEARANCE };

	CListBox		m_lbTextures;

	CString			m_strTexturePath;
	CString			m_strCurrentTexture;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	bool AddTextureString(String path);

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBrowseTexture();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedAddTexture();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedApplySelTex();
	afx_msg void OnBnClickedDeleteSelTex();
};
