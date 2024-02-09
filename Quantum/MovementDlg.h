#pragma once
#include "afxcmn.h"


// CMovementDlg dialog

class CMovementDlg : public DialogParentClass
{
	DECLARE_DYNAMIC(CMovementDlg)

public:
	CMovementDlg(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CMovementDlg();

// Dialog Data
	enum { IDD = IDD_PARTICLE_MOVEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BOOL		m_bApplyWind;
	BOOL		m_bRadialVelocityWave;
	BOOL		m_bThetaVelocityWave;
	BOOL		m_bPhiVelocityWave;

	float		m_fPolarPickupRange;
	
	float		m_fVelocityX;
	float		m_fVelocityY;
	float		m_fVelocityZ;

	float		m_fVelocityR;
	float		m_fVelocityA;
	float		m_fVelocityPhi;

	float		m_fAccelX;
	float		m_fAccelY;
	float		m_fAccelZ;
	
	float		m_fAccelR;
	float		m_fAccelA;
	float		m_fAccelPhi;

	CSliderCtrl	m_sPolarPickupRange;
	
	CSliderCtrl	m_sVelocityX;
	CSliderCtrl	m_sVelocityY;
	CSliderCtrl m_sVelocityZ;
	
	CSliderCtrl	m_sVelocityR;
	CSliderCtrl	m_sVelocityA;
	CSliderCtrl m_sVelocityPhi;
	
	CSliderCtrl	m_sAccelX;
	CSliderCtrl	m_sAccelY;
	CSliderCtrl m_sAccelZ;
	
	CSliderCtrl	m_sAccelR;
	CSliderCtrl	m_sAccelA;

	CSliderCtrl	m_sAccelPhi;

public:
	virtual BOOL OnInitDialog();
	void UpdateAllControls();

	afx_msg void OnNMCustomdrawVelXSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawVelYSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawVelZSlider(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMCustomdrawVelPolarPickupRangeSlider(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMCustomdrawVelRadSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawVelAngSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawVelPhiSlider(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMCustomdrawAccXSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAccYSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAccZSlider(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMCustomdrawAccRadSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAccAngSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAccAngSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnEnUpdateVelXEdit();
	afx_msg void OnEnUpdateVelYEdit();
	afx_msg void OnEnUpdateVelZEdit();
	
	afx_msg void OnEnUpdateVelPolarPickupRangeEdit();
	
	afx_msg void OnEnUpdateVelRadEdit();
	afx_msg void OnEnUpdateVelAngEdit();
	afx_msg void OnEnUpdateVelPhiEdit();
	
	afx_msg void OnEnUpdateAccXEdit();
	afx_msg void OnEnUpdateAccYEdit();
	afx_msg void OnEnUpdateAccZEdit();
	
	afx_msg void OnEnUpdateAccRadEdit();
	afx_msg void OnEnUpdateAccAngEdit();
	afx_msg void OnEnUpdateAccAngEdit2();

	afx_msg void OnBnClickedApplyWind();
	afx_msg void OnBnClickedWindOptions();
	afx_msg void OnBnClickedGravityOptions();

	afx_msg void OnBnClickedVelAngWave();
	afx_msg void OnBnClickedVelRadWave();
	afx_msg void OnBnClickedVelAngWaveParams();
	afx_msg void OnBnClickedVelRadWaveParams();
	afx_msg void OnBnClickedVelPhiWave();
};
