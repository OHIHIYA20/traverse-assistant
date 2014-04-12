// TraverseObsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traverse.h"
#include "TraverseObsDlg.h"
#include "SetupsDlg.h"

// CTraverseObsDlg dialog

IMPLEMENT_DYNAMIC(CTraverseObsDlg, CDialog)

CTraverseObsDlg::CTraverseObsDlg(XmlDetails &xml, TraverseObservation &tobs, CWnd* pParent /*=NULL*/)
	: CDialog(CTraverseObsDlg::IDD, pParent)
	, m_xml(xml)
	, m_tobs(tobs)
{

}

CTraverseObsDlg::~CTraverseObsDlg()
{
}

void CTraverseObsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TARGETEDIT, m_targetNameEdit);
	DDX_Control(pDX, IDC_HORZEDIT, m_horizontalEdit);
	DDX_Control(pDX, IDC_VERTEDIT, m_verticalEdit);
	DDX_Control(pDX, IDC_DISTEDIT, m_distanceEdit);
	DDX_Control(pDX, IDC_TARGETHEIGHTEDIT, m_targetHeightEdit);
	DDX_Control(pDX, IDC_TYPECOMBO, m_typeCombo);
}

BOOL CTraverseObsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_typeCombo.AddString(_T("Orientation"));
	m_typeCombo.AddString(_T("Forward"));
	m_typeCombo.AddString(_T("Backward"));
	m_typeCombo.AddString(_T("Network"));
	m_typeCombo.AddString(_T("Ignore"));

	FillControls(m_tobs);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CTraverseObsDlg, CDialog)
	ON_BN_CLICKED(IDC_LOADOBSBTN, &CTraverseObsDlg::OnBnClickedLoadobsbtn)
END_MESSAGE_MAP()


// CTraverseObsDlg message handlers

void CTraverseObsDlg::OnBnClickedLoadobsbtn()
{
	CSetupsDlg dlg(m_xml, this);
	if (IDOK == dlg.DoModal())
	{
		Observation obs = dlg.GetSingleSelectedObs();
		TraverseObservation tobs;
		tobs.obs = obs;
		FillControls(tobs);
	}
}

void CTraverseObsDlg::FillControls(const TraverseObservation &tobs)
{
	switch (tobs.type)
	{
		case TT_ORIENTATION: m_typeCombo.SetCurSel(0); break;
		case TT_FORWARD:     m_typeCombo.SetCurSel(1); break;
		case TT_BACKWARD:    m_typeCombo.SetCurSel(2); break;
		case TT_NETWORK:     m_typeCombo.SetCurSel(3); break;
		case TT_NONE:        m_typeCombo.SetCurSel(4); break;
	}

	CString value;
	m_targetNameEdit.SetWindowText(tobs.obs.targetName);
	value.Format(_T("%.4f"), tobs.obs.horizontalAngle);
	m_horizontalEdit.SetWindowText(value);
	value.Format(_T("%.4f"), tobs.obs.verticalAngle);
	m_verticalEdit.SetWindowText(value);
	value.Format(_T("%.3f"), tobs.obs.slopeDistance);
	m_distanceEdit.SetWindowText(value);
	value.Format(_T("%.3f"), tobs.obs.targetHeight);
	m_targetHeightEdit.SetWindowText(value);
}

