// VectorProps.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "VectorProps.h"


// CVectorProps dialog

IMPLEMENT_DYNAMIC(CVectorProps, CDialog)

CVectorProps::CVectorProps(CWnd* pParent /*=NULL*/)
	: CDialog(CVectorProps::IDD, pParent)
{

}

CVectorProps::~CVectorProps()
{
}

void CVectorProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVectorProps, CDialog)
END_MESSAGE_MAP()


// CVectorProps message handlers
