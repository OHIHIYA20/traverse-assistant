// SetupsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traverse.h"
#include "SetupsDlg.h"
#include "ObservationsDlg.h"

// CSetupsDlg dialog

IMPLEMENT_DYNAMIC(CSetupsDlg, CDialog)

CSetupsDlg::CSetupsDlg(XmlDetails &xml, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupsDlg::IDD, pParent)
	, m_xml(xml)
{
}

CSetupsDlg::~CSetupsDlg()
{
}

void CSetupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETUPSLIST, m_setupsList);
}

void CSetupsDlg::LoadFromSurveyXml()
{
	if (m_xml.surveyXml == NULL)
	{
		if (m_xml.xmlDocument.LoadFile(CW2A(m_xml.filename)))
		{
			const TiXmlElement *root = m_xml.xmlDocument.RootElement();
			if (root)
				m_xml.surveyXml = root->FirstChildElement("Survey");
		}
		else
		{
		LPCSTR error = m_xml.xmlDocument.ErrorDesc();
		MessageBox(CA2W(error), _T("Error"));
		}
	}

	if (m_xml.surveyXml)
	{
		int nSetups = 0;
		const TiXmlElement *setup = m_xml.surveyXml->FirstChildElement("InstrumentSetup");
		while (setup)
		{
			const char *sz1 = setup->Attribute("id");
			const char *sz2 = setup->Attribute("stationName");
			const char *sz3 = setup->Attribute("instrumentHeight");
			const char *sz4 = setup->Attribute("orientationAzimuth");
			InsertItem(nSetups, CA2W(sz1), CA2W(sz2), CA2W(sz3), CA2W(sz4));

			setup = setup->NextSiblingElement("InstrumentSetup");
			++nSetups;
		}
	}
}


void CSetupsDlg::InsertItem(int item, LPCTSTR szid, LPCTSTR szname, LPCTSTR szheight, LPCTSTR szorientation)
{
	CString id(szid);
	CString name(szname);
	CString height(szheight);
	CString orientation(szorientation);

	LVITEM sItem = {LVIF_TEXT, item, 0, 0, 0, name.GetBuffer(0)};
	m_setupsList.InsertItem(&sItem);
	name.ReleaseBuffer();

	sItem.iSubItem = 1;
	sItem.pszText = id.GetBuffer(0);
	m_setupsList.SetItem(&sItem);
	id.ReleaseBuffer();

	sItem.iSubItem = 2;
	sItem.pszText = height.GetBuffer(0);
	m_setupsList.SetItem(&sItem);
	height.ReleaseBuffer();

	sItem.iSubItem = 3;
	sItem.pszText = orientation.GetBuffer(0);
	m_setupsList.SetItem(&sItem);
	orientation.ReleaseBuffer();
}

Observation CSetupsDlg::GetSingleSelectedObs() const
{
	return m_singleSelectedObs;
}


BEGIN_MESSAGE_MAP(CSetupsDlg, CDialog)
//	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CSetupsDlg::OnHdnItemdblclickSetupslist)
ON_NOTIFY(NM_DBLCLK, IDC_SETUPSLIST, &CSetupsDlg::OnNMDblclkSetupslist)
END_MESSAGE_MAP()


BOOL CSetupsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_setupsList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect rect;
	m_setupsList.GetWindowRect(&rect);
	int columnWidth = rect.Width() / 4;

	m_setupsList.InsertColumn(0, _T("Name"),     LVCFMT_LEFT,  columnWidth);
	m_setupsList.InsertColumn(1, _T("ID"),      LVCFMT_LEFT,  columnWidth);
	m_setupsList.InsertColumn(2, _T("Instr. Height"),     LVCFMT_LEFT,  columnWidth);
	m_setupsList.InsertColumn(3, _T("Orientation"), LVCFMT_LEFT,  columnWidth);

	LoadFromSurveyXml();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//void CSetupsDlg::OnHdnItemdblclickSetupslist(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CSetupsDlg::OnNMDblclkSetupslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int item = pNMItemActivate->iItem;
	CString setupName = m_setupsList.GetItemText(item, 0);
	CString setupId = m_setupsList.GetItemText(item, 1);

	CObservationsDlg dlg(m_xml, setupName, setupId, true, this);
	if (IDOK == dlg.DoModal())
	{
		bool ok = false;
		m_singleSelectedObs = dlg.GetSingleSelectedObs(ok);
		*pResult = 0;
		if (ok)
			CDialog::OnOK();
	}

	*pResult = 0;
}
