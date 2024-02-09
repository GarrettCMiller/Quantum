// PropertyDock.cpp : implementation file
//

#include "stdafx.h"
#include "Quantum.h"
#include "PropertyDock.h"


// CPropertyDock

IMPLEMENT_DYNAMIC(CPropertyDock, CPropertySheet)

CPropertyDock::CPropertyDock(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CPropertyDock::CPropertyDock(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CPropertyDock::~CPropertyDock()
{
}


BEGIN_MESSAGE_MAP(CPropertyDock, CPropertySheet)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////