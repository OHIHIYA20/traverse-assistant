#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Structures.h"

// CObservationsDlg dialog

class CObservationsDlg : public CDialog
{
	DECLARE_DYNAMIC(CObservationsDlg)

private:
	void LoadFromSurveyXml();
	void InsertItem(int item, LPCTSTR target, LPCTSTR description, LPCTSTR horizontal, LPCTSTR vertical, LPCTSTR distance, LPCTSTR height, LPCTSTR unique);
	Observation GetObservationAt(int iItem) const;

public:
	CObservationsDlg(XmlDetails &xml, CString setupName, CString setupId, bool singleSelection, CWnd* pParent = NULL);   // standard constructor
	virtual ~CObservationsDlg();
	Observation GetSingleSelectedObs(bool &ok) const;
	bool GetMultipleSelectedObs(CArray<Observation> &obs) const;


// Dialog Data
	enum { IDD = IDD_OBSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_obsListLab;
	CListCtrl m_obsList;

	bool m_singleSelection;
	XmlDetails &m_xml;
	CString m_setupName;
	CString m_setupId;
	CArray<Observation> m_selectedObservations;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnNMDblclkObslist(NMHDR *pNMHDR, LRESULT *pResult);
};
