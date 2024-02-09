#pragma once


// CPhysPropsDlg dialog

class CPhysPropsDlg : public DialogParentClass	//CPropertyPage
{
	DECLARE_DYNAMIC(CPhysPropsDlg)

public:
	CPhysPropsDlg();
	virtual ~CPhysPropsDlg();

// Dialog Data
	enum { IDD = IDD_PARTICLE_PHYSICS };

public:
	BOOL		m_bRandomizeMass;
	BOOL		m_bRandomizeCharge;
	BOOL		m_bCollisions;

	BOOL		m_bApplyGravity;
	BOOL		m_bApplyEMForce;

protected:

	void UpdateAllControls();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedRandomizeMass();
	afx_msg void OnBnClickedRandomizeCharge();
	afx_msg void OnBnClickedClearHistory();
	afx_msg void OnBnClickedCollisions();

	afx_msg void OnBnClickedApplyGravity();
	afx_msg void OnBnClickedApplyEMForce();
	virtual BOOL OnInitDialog();
};
