
// TraverseDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Structures.h"
#include "afxcmn.h"

// CTraverseDlg dialog
class CTraverseDlg : public CDialog
{
	void SplitStringAtSpaces(const CString &value, CStringArray &tokens) const;
	ETraverseType GetObservationType(const Observation &obs, CString stationName) const;
	void AppendObservationToList(const TraverseObservation &obs);
	TraverseObservation GetObservationAtListItem(int nItem) const;

// Construction
public:
	CTraverseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TRAVERSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDefineroutebtn();
	afx_msg void OnBnClickedAddlegobsbtn();
private:
	CListBox m_stationNamesListBox;
public:
	afx_msg void OnLbnSelchangeRoutelistbox();
private:
	XmlDetails m_xml;

	CStatic m_legObsLab;
public:
	afx_msg void OnBnClickedLoadobsbtn();
private:
	CListCtrl m_obsList;
public:
	afx_msg void OnNMDblclkLegobslist(NMHDR *pNMHDR, LRESULT *pResult);
};
