#pragma once
#include "afxcmn.h"


// CColorDlg dialog

class CColorDlg : public DialogParentClass
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(/*CWnd* pParent = NULL*/);   // standard constructor
	virtual ~CColorDlg();

// Dialog Data
	enum { IDD = IDD_PARTICLE_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BYTE m_Red;
	BYTE m_Green;
	BYTE m_Blue;
	BYTE m_Alpha;

	int m_FinalRed;
	int m_FinalGreen;
	int m_FinalBlue;
	int m_FinalAlpha;

	int	m_ActiveColorMode;

	Float	m_fColorMultiplier;

	CSliderCtrl m_sliderRed;
	CSliderCtrl m_sliderGreen;
	CSliderCtrl m_sliderBlue;
	CSliderCtrl m_sliderAlpha;

	CSliderCtrl m_sliderFinalRed;
	CSliderCtrl m_sliderFinalGreen;
	CSliderCtrl m_sliderFinalBlue;
	CSliderCtrl m_sliderFinalAlpha;

public:
	virtual BOOL OnInitDialog();
	void UpdateAllControls();

	afx_msg void OnNMCustomdrawColorRedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawColorGreenSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawColorBlueSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawColorAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMReleasedcaptureColorRedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureColorGreenSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureColorBlueSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureColorAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnUpdateColorRedEdit();
	afx_msg void OnEnUpdateColorGreenEdit();
	afx_msg void OnEnUpdateColorBlueEdit();
	afx_msg void OnEnUpdateColorAlphaEdit();

	afx_msg void OnEnUpdateColorMultiplierEdit();

	afx_msg void OnNMCustomdrawDecayRedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDecayGreenSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDecayBlueSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawDecayAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnCbnSelchangeActiveColorMode();
public:
	BOOL m_bColorBlend;
public:
	afx_msg void OnBnClickedColorBlend();
public:
	afx_msg void OnEnUpdateDecayRedEdit();
public:
	afx_msg void OnEnUpdateDecayGreenEdit();
public:
	afx_msg void OnEnUpdateDecayBlueEdit();
public:
	afx_msg void OnEnUpdateDecayAlphaEdit();
public:
	afx_msg void OnBnClickedRandomizeColor();
public:
	BOOL m_bRandomizeColor;
};
