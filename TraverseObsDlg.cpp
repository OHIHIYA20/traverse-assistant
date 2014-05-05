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
		case TT_Orientation: m_typeCombo.SetCurSel(0); break;
		case TT_Forward:     m_typeCombo.SetCurSel(1); break;
		case TT_Backward:    m_typeCombo.SetCurSel(2); break;
		case TT_Network:     m_typeCombo.SetCurSel(3); break;
		case TT_None:        m_typeCombo.SetCurSel(4); break;
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


void CTraverseObsDlg::OnOK()
{
	switch (m_typeCombo.GetCurSel())
	{
		case 0: m_tobs.type = TT_Orientation; break;
		case 1: m_tobs.type = TT_Forward; break;
		case 2: m_tobs.type = TT_Backward; break;
		case 3: m_tobs.type = TT_Network; break;
		case 4: m_tobs.type = TT_None; break;
	}

	CString value;
	m_targetNameEdit.GetWindowText(m_tobs.obs.targetName);
	m_horizontalEdit.GetWindowText(value);
	m_tobs.obs.horizontalAngle = _tstof(value.GetString());
	m_verticalEdit.GetWindowText(value);
	m_tobs.obs.verticalAngle = _tstof(value.GetString());
	m_distanceEdit.GetWindowText(value);
	m_tobs.obs.slopeDistance = _tstof(value.GetString());
	m_targetHeightEdit.GetWindowText(value);
	m_tobs.obs.targetHeight = _tstof(value.GetString());

	CDialog::OnOK();
}
