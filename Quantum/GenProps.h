#pragma once
#include "afxcmn.h"


// CGenProps dialog

class CGenProps : public DialogParentClass//, public COleDropSource
{
	DECLARE_DYNAMIC(CGenProps)

public:
	CGenProps(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CGenProps();

// Dialog Data
	enum { IDD = IDD_PARTICLE_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL		m_bParticleEmitterActive;
	BOOL		m_bParticleEmitterRender;
	BOOL		m_bVisible;
	BOOL		m_bPaused;
	BOOL		m_bGraphHistory;

	float		m_fEmitRate;
	float		m_fSize;
	float		m_fEmitRadius;
	float		m_fMinSpawnAngleTheta;
	float		m_fMaxSpawnAngleTheta;
	float		m_fMinSpawnAnglePhi;
	float		m_fTimeMultiplier;
	float		m_fMaxSpawnAnglePhi;

	int			m_nMinLife;
	int			m_nMaxLife;
	int			m_EmitShape;
	int			m_nMaxParticles;
	int			m_nRandomSeed;
	int			m_nEmitDist;

	CString		m_strParticleID;

	CSliderCtrl m_sMaxParticles;
	CSliderCtrl m_sEmitRate;
	CSliderCtrl m_sSize;
	CSliderCtrl m_sMinLife;
	CSliderCtrl m_sMaxLife;
	CSliderCtrl m_sEmitRadius;
	CSliderCtrl m_sMinSpawnAngleTheta;
	CSliderCtrl m_sMaxSpawnAngleTheta;
	CSliderCtrl m_sMinSpawnAnglePhi;
	CSliderCtrl m_sMaxSpawnAnglePhi;

public:
	virtual BOOL OnInitDialog();
	void UpdateAllControls();

	virtual BOOL OnSetActive();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnNMCustomdrawPropsMaxParticlesSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsEmitRateSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsSizeSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsMinLifeSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsMaxLifeSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsEmitRadiusSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsMinSpawnAngleSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsMaxSpawnAngleSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsMinSpawnAnglePhiSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPropsMaxSpawnAnglePhiSlider(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnEnUpdatePropsRandomSeedEdit();
	afx_msg void OnEnUpdatePropsTimeMultiplierEdit();
	afx_msg void OnEnUpdatePropsMaxParticlesEdit();
	afx_msg void OnEnUpdatePropsEmitRateEdit();
	afx_msg void OnEnUpdatePropsSizeEdit();
	afx_msg void OnEnUpdatePropsMinLifeEdit();
	afx_msg void OnEnUpdatePropsMaxLifeEdit();
	afx_msg void OnEnUpdatePropsEmitRadiusEdit();
	afx_msg void OnEnUpdatePropsMinSpawnAngleEdit();
	afx_msg void OnEnUpdatePropsMaxSpawnAngleEdit();
	afx_msg void OnEnUpdatePropsParticleIDEdit();
	afx_msg void OnEnUpdatePropsMinSpawnAnglePhiEdit();
	afx_msg void OnEnUpdatePropsMaxSpawnAnglePhiEdit();

	afx_msg void OnBnClickedExplosion();
	afx_msg void OnBnClickedEmitterActive();
	afx_msg void OnBnClickedResetAll();
	afx_msg void OnBnClickedEmitterVisible();
	afx_msg void OnBnClickedEmitterPaused();
	afx_msg void OnBnClickedGraphHistory();
	afx_msg void OnBnClickedResetCurrentEmitter();

	afx_msg void OnCbnSelchangePropsSpawnShape();
	afx_msg void OnCbnSelchangeSpawnDist();
};

////////////////////////////////

//class CGenPropsPage : public CGenProps