#pragma once
#include "afxcmn.h"


// CParticleSystemsDlg dialog

class CParticleSystemsDlg : public DialogParentClass
{
	DECLARE_DYNAMIC(CParticleSystemsDlg)

public:
	CParticleSystemsDlg(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CParticleSystemsDlg();

// Dialog Data
	enum { IDD = IDD_PARTICLE_SYSTEMS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CImageList list;
	CTreeCtrl m_ParticleSystems;
public:
	afx_msg void OnBnClickedNewSystem();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedDeleteAll();
	afx_msg void OnTvnSelchangedParticleSystemTree(NMHDR *pNMHDR, LRESULT *pResult);
};
