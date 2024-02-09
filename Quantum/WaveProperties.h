#pragma once


// CWaveProperties dialog

class CWaveProperties : public CDialog
{
	DECLARE_DYNAMIC(CWaveProperties)

public:
	CWaveProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaveProperties();

// Dialog Data
	enum { IDD = IDD_WAVE_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	float m_fFrequency;
	float m_fBase;
	float m_fAmplitude;
public:
	afx_msg void OnBnClickedApply();
public:
	virtual BOOL OnInitDialog();
};
