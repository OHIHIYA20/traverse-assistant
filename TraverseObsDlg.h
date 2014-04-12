#pragma once
#include "afxwin.h"
#include "Structures.h"

// CTraverseObsDlg dialog

class CTraverseObsDlg : public CDialog
{
	DECLARE_DYNAMIC(CTraverseObsDlg)

	void FillControls(const TraverseObservation &observation);

public:
	CTraverseObsDlg(XmlDetails &xml, TraverseObservation &tobs, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTraverseObsDlg();

// Dialog Data
	enum { IDD = IDD_TRAVERSEOBSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadobsbtn();
private:
	XmlDetails &m_xml;
	TraverseObservation &m_tobs;

	CEdit m_targetNameEdit;
	CEdit m_horizontalEdit;
	CEdit m_verticalEdit;
	CEdit m_distanceEdit;
	CEdit m_targetHeightEdit;
	CComboBox m_typeCombo;
public:
	virtual BOOL OnInitDialog();
};
