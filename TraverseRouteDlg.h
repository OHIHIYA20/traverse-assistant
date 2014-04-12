#pragma once
#include "afxwin.h"


// CTraverseRouteDlg dialog

class CTraverseRouteDlg : public CDialog
{
	DECLARE_DYNAMIC(CTraverseRouteDlg)

	CString &m_route;

public:
	CTraverseRouteDlg(CString &route, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTraverseRouteDlg();

// Dialog Data
	enum { IDD = IDD_TRAVERSEROUTEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
private:
	CEdit m_routeEdit;
public:
	virtual BOOL OnInitDialog();
};
