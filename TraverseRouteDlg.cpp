// TraverseRouteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traverse.h"
#include "TraverseRouteDlg.h"


// CTraverseRouteDlg dialog

IMPLEMENT_DYNAMIC(CTraverseRouteDlg, CDialog)

CTraverseRouteDlg::CTraverseRouteDlg(CString &route, CWnd* pParent /*=NULL*/)
	: CDialog(CTraverseRouteDlg::IDD, pParent)
	, m_route(route)
{

}

CTraverseRouteDlg::~CTraverseRouteDlg()
{
}

void CTraverseRouteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_routeEdit);
}


BEGIN_MESSAGE_MAP(CTraverseRouteDlg, CDialog)
END_MESSAGE_MAP()


// CTraverseRouteDlg message handlers

void CTraverseRouteDlg::OnOK()
{
	m_routeEdit.GetWindowText(m_route);

	CDialog::OnOK();
}

BOOL CTraverseRouteDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_routeEdit.SetWindowText(m_route);
	m_routeEdit.SetSel(0, -1); // todo: get the editbox ready for immediate typing.

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
