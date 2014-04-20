
// TraverseDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Structures.h"
#include "afxcmn.h"

// CTraverseDlg dialog
class CTraverseDlg : public CDialog
{
	void EnableWindow(UINT nID, BOOL enabled);
	void SplitStringAtSpaces(const CString &value, CStringArray &tokens) const;
	ETraverseType GetObservationType(const Observation &obs, CString stationName) const;
	void SortList();
	void RefreshList();
	void RefreshStations();
	void AppendObservationToList(const TraverseObservation &obs, INT_PTR nLeg);
	TraverseObservation GetObservationAtListItem(int nItem) const;
	OccupiedStation GetStationAtRouteItem(int nLeg) const;

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
	afx_msg void OnBnClickedLoadobsbtn();
	afx_msg void OnNMDblclkLegobslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeRoutelistbox();
private:
	XmlDetails m_xml;
	CArray<TraverseObservation> m_legs;
	CArray<OccupiedStation> m_stations;

	CListCtrl m_routeList;
	CListCtrl m_obsList;
	CStatic m_legObsLab;
	CImageList m_routeImageList;
	CImageList m_obsImageList;
public:
	afx_msg void OnLvnKeydownLegobslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileOpen();
	afx_msg void OnLvnItemchangedRoutelist(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_routeBtn;
	};
