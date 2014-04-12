#pragma once
#include "afxcmn.h"
#include "Structures.h"

// CSetupsDlg dialog

class CSetupsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupsDlg)

private:
	void LoadFromSurveyXml();
	void InsertItem(int item, LPCTSTR id, LPCTSTR name, LPCTSTR height, LPCTSTR orientation);

public:
	CSetupsDlg(XmlDetails &xml, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupsDlg();
	Observation GetSingleSelectedObs() const;

// Dialog Data
	enum { IDD = IDD_SETUPSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	XmlDetails &m_xml;
	CListCtrl m_setupsList;
	Observation m_singleSelectedObs;

public:
//	afx_msg void OnHdnItemdblclickSetupslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkSetupslist(NMHDR *pNMHDR, LRESULT *pResult);
};
