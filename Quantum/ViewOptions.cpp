// ViewOptions.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "ViewOptions.h"


// CViewOptions dialog

IMPLEMENT_DYNAMIC(CViewOptionsDlg, DialogParentClass)

CViewOptionsDlg::CViewOptionsDlg(/*CWnd* pParent*/ /*=NULL*/)
	: DialogParentClass(CViewOptionsDlg::IDD/*, pParent*/)
{

}

CViewOptionsDlg::~CViewOptionsDlg()
{
}

void CViewOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	DialogParentClass::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewOptionsDlg, DialogParentClass)
END_MESSAGE_MAP()


// CViewOptions message handlers
