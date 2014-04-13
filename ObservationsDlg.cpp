// ObservationsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traverse.h"
#include "ObservationsDlg.h"


// CObservationsDlg dialog

IMPLEMENT_DYNAMIC(CObservationsDlg, CDialog)

CObservationsDlg::CObservationsDlg(XmlDetails &xml, CString setupName, CString setupNumber, bool singleSelection, CWnd* pParent /*=NULL*/)
	: CDialog(CObservationsDlg::IDD, pParent)
	, m_singleSelection(singleSelection)
	, m_xml(xml)
	, m_setupName(setupName)
	, m_setupNumber(setupNumber)
{

}

CObservationsDlg::~CObservationsDlg()
{
}

void CObservationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBSLISTLAB, m_obsListLab);
	DDX_Control(pDX, IDC_OBSLIST, m_obsList);
}

void CObservationsDlg::LoadFromSurveyXml()
{
	if (m_xml.surveyXml == NULL)
	{
		if (m_xml.xmlDocument.LoadFile(CW2A(m_xml.filename)))
		{
			const TiXmlElement *root = m_xml.xmlDocument.RootElement();
			if (root)
				m_xml.surveyXml = root->FirstChildElement("Survey");
		}
	}

	CString searchName = m_setupName + _T('@') + m_setupNumber;

	if (m_xml.surveyXml)
	{
		const TiXmlElement *group = m_xml.surveyXml->FirstChildElement("ObservationGroup");
		if (group)
		{
			int nItem = 0;
			const TiXmlElement *obs = group->FirstChildElement("ReducedObservation");
			while (obs)
			{
				if (searchName == CA2W(obs->Attribute("setupID")))
				{
					const char *sz1 = obs->Attribute("name");
					const char *sz2 = obs->Attribute("desc");
					const char *sz3 = obs->Attribute("horizAngle");
					const char *sz4 = obs->Attribute("zenithAngle");
					const char *sz5 = obs->Attribute("slopeDistance");
					const char *sz6 = obs->Attribute("targetHeight");
					const char *sz7 = obs->Attribute("targetSetupID");

					InsertItem(nItem, CA2W(sz1), CA2W(sz2), CA2W(sz3), CA2W(sz4), CA2W(sz5), CA2W(sz6), CA2W(sz7));
					++nItem;
				}
				obs = obs->NextSiblingElement("ReducedObservation");
			}
		}
	}
}


void CObservationsDlg::InsertItem(int item, LPCTSTR sztarget, LPCTSTR szdescription, LPCTSTR szhorizontal, LPCTSTR szvertical, LPCTSTR szdistance, LPCTSTR szheight, LPCTSTR szunique)
{
	CString target(sztarget);
	CString description;
	if (szdescription)
		description.SetString(szdescription);
	CString horizontal(szhorizontal);
	CString vertical(szvertical);
	CString distance(szdistance);
	CString height(szheight);
	int uniqueId = -1;
	if (szunique)
		uniqueId = _tstoi(szunique);

	LVITEM sItem = {LVIF_TEXT, item, 0, 0, 0, target.GetBuffer(0)};
	int nItem = m_obsList.InsertItem(&sItem);
	target.ReleaseBuffer();

	sItem.iSubItem = 1;
	sItem.pszText = description.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	description.ReleaseBuffer();

	sItem.iSubItem = 2;
	sItem.pszText = horizontal.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	horizontal.ReleaseBuffer();

	sItem.iSubItem = 3;
	sItem.pszText = vertical.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	vertical.ReleaseBuffer();

	sItem.iSubItem = 4;
	sItem.pszText = distance.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	distance.ReleaseBuffer();

	sItem.iSubItem = 5;
	sItem.pszText = height.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	height.ReleaseBuffer();

	m_obsList.SetItemData(nItem, uniqueId);
}

Observation CObservationsDlg::GetSingleSelectedObs(bool &ok) const
{
	ok = m_selectedObservations.GetCount() == 1;
	return m_selectedObservations.GetAt(0);
}

bool CObservationsDlg::GetMultipleSelectedObs(CArray<Observation> &obs) const
{
	obs.RemoveAll();
	obs.Copy(m_selectedObservations);
	return obs.GetCount() > 0;
}



BEGIN_MESSAGE_MAP(CObservationsDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_OBSLIST, &CObservationsDlg::OnNMDblclkObslist)
END_MESSAGE_MAP()


// CObservationsDlg message handlers

BOOL CObservationsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString caption;
	caption.Format(_T("Observations @ %s (%s):"), m_setupName, m_setupNumber);
	m_obsListLab.SetWindowText(caption);

	if (m_singleSelection)
		m_obsList.ModifyStyle(0, LVS_SINGLESEL);
	else
		m_obsList.ModifyStyle(LVS_SINGLESEL, 0);

	m_obsList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect rect;
	m_obsList.GetWindowRect(&rect);
	int columnWidth = rect.Width() / 6;

	m_obsList.InsertColumn(0, _T("Target"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(1, _T("Description"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(2, _T("Horizontal"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(3, _T("Vertical"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(4, _T("Slope Dist"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(5, _T("Instr. Height"), LVCFMT_LEFT,  columnWidth);

	LoadFromSurveyXml();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CObservationsDlg::OnOK()
{
	if (m_singleSelection)
	{
	}
	else
	{
		m_selectedObservations.RemoveAll();

		POSITION pos = m_obsList.GetFirstSelectedItemPosition();
		if (pos)
		{
			while (pos)
			{
				int nItem = m_obsList.GetNextSelectedItem(pos);
				m_selectedObservations.Add(GetObservationAt(nItem));
			}
		}
		else
			return;
	}

	CDialog::OnOK();
}

void CObservationsDlg::OnNMDblclkObslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (m_singleSelection)
	{
		m_selectedObservations.RemoveAll();
		m_selectedObservations.Add(GetObservationAt(pNMItemActivate->iItem));
		*pResult = 0;
		CDialog::OnOK();
	}
	
	*pResult = 0;
}


Observation CObservationsDlg::GetObservationAt(int iItem) const
{
	int targetID = (int)m_obsList.GetItemData(iItem);
	char szTargetID[32];
	sprintf_s(szTargetID, "%d", targetID);

	if (m_xml.surveyXml == NULL)
	{
		if (m_xml.xmlDocument.LoadFile(CW2A(m_xml.filename)))
		{
			const TiXmlElement *root = m_xml.xmlDocument.RootElement();
			if (root)
				m_xml.surveyXml = root->FirstChildElement("Survey");
		}
	}

	CString searchName = m_setupName + _T('@') + m_setupNumber;

	if (m_xml.surveyXml)
	{
		const TiXmlElement *group = m_xml.surveyXml->FirstChildElement("ObservationGroup");
		if (group)
		{
			int nItem = 0;
			const TiXmlElement *obs = group->FirstChildElement("ReducedObservation");
			while (obs)
			{
				if (0 == strcmp(szTargetID, obs->Attribute("targetSetupID")))
				{
					const char *sz1 = obs->Attribute("name");
					const char *sz2 = obs->Attribute("desc");
					const char *sz3 = obs->Attribute("horizAngle");
					const char *sz4 = obs->Attribute("zenithAngle");
					const char *sz5 = obs->Attribute("slopeDistance");
					const char *sz6 = obs->Attribute("targetHeight");

					Observation o;
					o.targetName = CA2W(sz1);
					o.description = CA2W(sz2);
					o.horizontalAngle = atof(sz3);
					o.verticalAngle = atof(sz4);
					o.slopeDistance = atof(sz5);
					o.targetHeight = atof(sz6);
					return o;
				}
				obs = obs->NextSiblingElement("ReducedObservation");
			}
		}
	}

	Observation o;
	return o;
}
