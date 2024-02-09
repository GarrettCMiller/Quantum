// OutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "OutputDlg.h"

Logger COutputDlg::m_Logger;
// COutputDlg dialog

IMPLEMENT_DYNAMIC(COutputDlg, DialogParentClass)

COutputDlg::COutputDlg(/*CWnd* pParent*/ /*=NULL*/)
	: DialogParentClass(COutputDlg::IDD/*, pParent*/)
	, m_strConsoleCmd(_T(""))
{

}

COutputDlg::~COutputDlg()
{
}

void COutputDlg::DoDataExchange(CDataExchange* pDX)
{
	DialogParentClass::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONSOLE_INPUT, m_strConsoleCmd);
	DDX_Control(pDX, IDC_CONSOLE_OUTPUT, m_Console);
}


BEGIN_MESSAGE_MAP(COutputDlg, DialogParentClass)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_CONSOLE_INPUT, &COutputDlg::OnEnChangeConsoleInput)
END_MESSAGE_MAP()


// COutputDlg message handlers

BOOL COutputDlg::OnInitDialog()
{
	DialogParentClass::OnInitDialog();

	CHARFORMAT2 format;
	
	ZeroMemory(&format, sizeof(CHARFORMAT2));

	format.cbSize = sizeof(CHARFORMAT2);
	format.dwMask = CFM_COLOR;
	format.crTextColor = 0x666666;

	m_Console.SetDefaultCharFormat(format);
	m_Console.SetBackgroundColor(false, 0x00000000);

	m_Logger.OpenNewLog("MFC", true);
	m_Logger./*OpenNewLogMFC*/AttachMFC(static_cast<CRichEditCtrl*>(GetDlgItem(IDC_CONSOLE_OUTPUT)));

	//m_Console.SendMessage(EM_SETEVENTMASK, ENM_UPDATE);

	m_Logger << "Initializing console dialog...\n";

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// void COutputDlg::OnEnUpdateConsoleOutput()
// {
// 	// TODO:  If this is a RICHEDIT control, the control will not
// 	// send this notification unless you override the CDialog::OnInitDialog()
// 	// function to send the EM_SETEVENTMASK message to the control
// 	// with the ENM_UPDATE flag ORed into the lParam mask.
// 	if (m_Console.GetModify())
// 	{
// 		m_Console.SetModify(false);
// 		m_Console.Invalidate(false);
// 	}
// }

void COutputDlg::OnOK()
{
	g_Output << "Console command: " << m_strConsoleCmd << "\n";

	m_strConsoleCmd = "";

	UpdateData(false);

	//CPropertyPage::OnOK();
}

void COutputDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CPropertyPage::OnPaint() for painting messages

#if 1
	static const char* keywords[][10] =
	{
		//bad keywords (red)
		{
			"fail", "failed", "error", "\0"
		},

		//good keyowrds (green)
		{
			"success", "successfully", "pass", "\0"
		},

		//value keywords
		{
			"true",	"false", "Null", "\0"
		},

		//GLib keywords
		{
			"Direct3D", "Direct3D9", "GLib",	"logger", "\0"
		},

		//local keywords
		{
			"particle",	"particles", "emitter",  "\0"
		},

		//windows keywords
		{
			"MFC",	"NONE",	"STUFF", "\0"
		},

				//"initializing"
	};

	static const DWORD keycolors[][2] =
	{
		//bad
		{0,		0x0000FF},

		//good
		{0,		0x00FF00},

		//value keywords
		{1,		0xAA0000,},

		//GLib
		{1,		0x00AAAA,},

		//local
		{0,		0x0000AA,},

		//windows
		{0,		0xDDDDDD}
	};

	FINDTEXTEX ftext;
	CHARFORMAT2 format;

	ZeroMemory(&format, sizeof(CHARFORMAT2));
	ZeroMemory(&ftext, sizeof(FINDTEXTEX));

	format.cbSize = sizeof(CHARFORMAT2);

	ftext.chrg.cpMin = 0;
	ftext.chrg.cpMax = -1;

	int pos = -1;

	for (int i = 0; i < 6; i++)
	{
		int j = -1;
		while (keywords[i][++j] != '\0')
		{
			ftext.chrg.cpMin = 0;
			m_Console.SetSel(0, 1);

			ftext.lpstrText = keywords[i][j];
			format.crTextColor = keycolors[i][1];

			while ((pos = m_Console.FindText(FR_WHOLEWORD | FR_DOWN, &ftext)) >= 0)
			{
				pos = m_Console.FindWordBreak(WB_NEXTBREAK, ftext.chrgText.cpMax);
				ftext.chrgText.cpMax = pos;

				//while ()

				if (keycolors[i][0] == 1)
				{
					format.dwMask = CFM_BOLD | CFM_COLOR;
					format.dwEffects = CFE_BOLD;
				}
				else
				{
					format.dwMask = CFM_COLOR;
					format.dwEffects = 0;
				}

				m_Console.SetSel(ftext.chrgText);
				m_Console.SetSelectionCharFormat(format);
				ftext.chrg.cpMin = pos + 1;
			}
		}
	}
#endif

	/*m_Console.LineScroll(1, 0);*/
}

void COutputDlg::OnEnChangeConsoleInput()
{
	UpdateData();
	if (m_strConsoleCmd.GetLength() > 0 && m_strConsoleCmd[m_strConsoleCmd.GetLength() - 1] == '\0')
		OnOK();
}

LRESULT COutputDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_PARENTNOTIFY)
		return CPropertyPage::WindowProc(message, wParam, lParam);

	return CPropertyPage::WindowProc(message, wParam, lParam);
}
