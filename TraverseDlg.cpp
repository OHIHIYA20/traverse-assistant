
// TraverseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traverse.h"
#include "TraverseDlg.h"
#include "TraverseRouteDlg.h"
#include "TraverseObsDlg.h"
#include "ObservationsDlg.h"
#include "SetupsDlg.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTraverseDlg dialog




CTraverseDlg::CTraverseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraverseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTraverseDlg::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Control(pDX, IDC_ROUTELIST, m_routeList);
DDX_Control(pDX, IDC_LEGOBSLAB, m_legObsLab);
DDX_Control(pDX, IDC_LEGOBSLIST, m_obsList);
DDX_Control(pDX, IDC_DEFINEROUTEBTN, m_routeBtn);
	}

BEGIN_MESSAGE_MAP(CTraverseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DEFINEROUTEBTN, &CTraverseDlg::OnBnClickedDefineroutebtn)
	ON_BN_CLICKED(IDC_ADDLEGOBSBTN, &CTraverseDlg::OnBnClickedAddlegobsbtn)
	ON_BN_CLICKED(IDC_LOADOBSBTN, &CTraverseDlg::OnBnClickedLoadobsbtn)
	ON_NOTIFY(NM_DBLCLK, IDC_LEGOBSLIST, &CTraverseDlg::OnNMDblclkLegobslist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LEGOBSLIST, &CTraverseDlg::OnLvnKeydownLegobslist)
	ON_COMMAND(ID_FILE_OPEN, &CTraverseDlg::OnFileOpen)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ROUTELIST, &CTraverseDlg::OnLvnItemchangedRoutelist)
END_MESSAGE_MAP()


// CTraverseDlg message handlers

BOOL CTraverseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	EnableWindow(IDC_DEFINEROUTEBTN, FALSE);
	EnableWindow(IDC_ADDLEGOBSBTN, FALSE);
	EnableWindow(IDC_LOADOBSBTN, FALSE);

	m_routeBtn.SetWindowText(_T("Add start..."));

	m_obsList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_routeList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect rect1;
	m_routeList.GetWindowRect(&rect1);
	int columnWidth1 = rect1.Width() / 2;

	m_routeList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, rect1.Width() * 2 / 3);
	m_routeList.InsertColumn(1, _T("No."), LVCFMT_LEFT, rect1.Width() * 1 / 3);

	CRect rect2;
	m_obsList.GetWindowRect(&rect2);
	int columnWidth2 = rect2.Width() / 5;

	m_obsList.InsertColumn(0, _T("Type"), LVCFMT_LEFT,  columnWidth2);
	m_obsList.InsertColumn(1, _T("Target"), LVCFMT_LEFT,  columnWidth2);
	m_obsList.InsertColumn(2, _T("Horizontal"), LVCFMT_LEFT,  columnWidth2);
	m_obsList.InsertColumn(3, _T("Reduced"), LVCFMT_LEFT,  columnWidth2);
	m_obsList.InsertColumn(4, _T("Target Height"), LVCFMT_LEFT,  columnWidth2);

	m_routeImageList.Create(10, 10, ILC_COLOR8, 2, 1);
	m_routeImageList.Add(theApp.LoadIcon(IDI_TERMINAL));
	m_routeImageList.Add(theApp.LoadIcon(IDI_STATION));
	m_routeList.SetImageList(&m_routeImageList, LVSIL_SMALL);

	m_obsImageList.Create(16, 16, ILC_COLOR8, 5, 1);
	m_obsImageList.Add(theApp.LoadIcon(IDI_DOT_BLUE));
	m_obsImageList.Add(theApp.LoadIcon(IDI_DOT_GREEN));
	m_obsImageList.Add(theApp.LoadIcon(IDI_DOT_ORANGE));
	m_obsImageList.Add(theApp.LoadIcon(IDI_DOT_PINK));
	m_obsImageList.Add(theApp.LoadIcon(IDI_DOT_GRAY));
	m_obsList.SetImageList(&m_obsImageList, LVSIL_SMALL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTraverseDlg::EnableWindow(UINT nID, BOOL enabled)
	{
	CWnd *button = GetDlgItem(nID);
	if (button)
		button->EnableWindow(enabled);
	}

void CTraverseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTraverseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTraverseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTraverseDlg::OnBnClickedDefineroutebtn()
	{
	EnableWindow(IDC_ADDLEGOBSBTN, FALSE);
	EnableWindow(IDC_LOADOBSBTN, FALSE);

	CSetupsDlg dlg(m_xml, this);
	if (IDOK == dlg.DoModal())
		{
		OccupiedStation nextStation = dlg.GetSelectedSetup();
		m_stations.Add(nextStation);
		RefreshStations();

		m_routeBtn.SetWindowText(_T("Add next/end..."));
		}
	}

void CTraverseDlg::OnBnClickedAddlegobsbtn()
{
	POSITION pos = m_routeList.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nLeg = m_routeList.GetNextSelectedItem(pos);
		CString stationName = m_routeList.GetItemText(nLeg, 0);

		TraverseObservation tobs;
		tobs.routeSequence = nLeg;

		CTraverseObsDlg dlg(m_xml, tobs, this);
		if (IDOK == dlg.DoModal())
		{
			m_legs.Add(tobs);
			RefreshList();
		}
	}
}

void CTraverseDlg::SplitStringAtSpaces(const CString &value, CStringArray &tokens) const
{
	int start = 0;
	while (true)
	{
		int end = value.Find(_T(' '), start);
		if (end == -1)
		{
			CString token = value.Mid(start);
			token.Trim();
			if (!token.IsEmpty())
				tokens.Add(token);
			break;
		}
		else
		{
			CString token = value.Mid(start, end - start);
			token.Trim();
			if (!token.IsEmpty())
				tokens.Add(token);
			start = end + 1;
		}
	}
}

void CTraverseDlg::OnLvnItemchangedRoutelist(NMHDR *pNMHDR, LRESULT *pResult)
	{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVNI_SELECTED))
	{
		OccupiedStation s = GetStationAtRouteItem(pNMLV->iItem);

		CString caption;
		caption.Format(_T("Observations at %s (%d):"), s.setupName, s.setupNumber);
		m_legObsLab.SetWindowText(caption);

		EnableWindow(IDC_ADDLEGOBSBTN, TRUE);
		EnableWindow(IDC_LOADOBSBTN, TRUE);

		RefreshList();
	}

	*pResult = 0;
}

void CTraverseDlg::OnBnClickedLoadobsbtn()
{
	POSITION pos = m_routeList.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nLeg = m_routeList.GetNextSelectedItem(pos);
		OccupiedStation station = GetStationAtRouteItem(nLeg);

		CObservationsDlg dlg(m_xml, station, false, this);
		if (IDOK == dlg.DoModal())
		{
			CArray<Observation> obs;
			dlg.GetMultipleSelectedObs(obs);

			for (INT_PTR i = 0; i < obs.GetCount(); ++i)
			{
				Observation o = obs.GetAt(i);
				ETraverseType type = GetObservationType(o, station.setupName);

				TraverseObservation tobs;
				tobs.routeSequence = nLeg;
				tobs.type = type;
				tobs.obs = o;

				m_legs.Add(tobs);
			}

			SortList();
			RefreshList();
		}
	}
}

ETraverseType CTraverseDlg::GetObservationType(const Observation &obs, CString stationName) const
{
	int nLeg = -1;
	int nStations = m_routeList.GetItemCount();

	CStringArray stations;
	for (int i = 0; i < nStations; ++i)
	{
		CString legName = m_routeList.GetItemText(i, 0);
		stations.Add(legName);

		if (legName == stationName)
			nLeg = i;
	}
	
	if (nLeg == -1)
		return TT_NONE;

	CString previousStation, nextStation;
	if (nLeg > 0)
		previousStation = stations.GetAt(nLeg - 1);
	if (nLeg < nStations - 1)
		nextStation = stations.GetAt(nLeg + 1);

	if (obs.targetName == previousStation)
		return TT_BACKWARD;
	else if (obs.targetName == nextStation)
		return TT_FORWARD;
	else /* if targetName point exists in db */
		return TT_ORIENTATION;

	return TT_NONE;
}

void CTraverseDlg::RefreshList()
{
	POSITION pos = m_routeList.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nLeg = m_routeList.GetNextSelectedItem(pos);

		m_obsList.DeleteAllItems();

		for (INT_PTR i = 0; i < m_legs.GetCount(); ++i)
		{
			TraverseObservation tobs = m_legs.GetAt(i);
			if (tobs.routeSequence == nLeg)
			{
				AppendObservationToList(tobs, i);
			}
		}
	}
}

void CTraverseDlg::RefreshStations()
	{
	m_routeList.DeleteAllItems();

	int iItem = 0;

	for (INT_PTR i = 0; i < m_stations.GetCount(); ++i)
		{
		iItem = (int)i;

		OccupiedStation station = m_stations.GetAt(i);
		bool isTerminal = (i == 0 || i == m_stations.GetCount() - 1);

		LVITEM sItem = {LVIF_TEXT | LVIF_IMAGE, iItem, 0, 0, 0, station.setupName.GetBuffer(0)};
		sItem.iImage = isTerminal ? 0 : 1;
		m_routeList.InsertItem(&sItem);
		station.setupName.ReleaseBuffer();

		CString number;
		number.Format(_T("%d"), station.setupNumber);

		sItem.iSubItem = 1;
		sItem.pszText = number.GetBuffer(0);
		m_routeList.SetItem(&sItem);
		number.ReleaseBuffer();
		}

	m_routeList.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
	m_routeList.SetSelectionMark(iItem);
	m_routeList.EnsureVisible(iItem, FALSE);
	}

void CTraverseDlg::AppendObservationToList(const TraverseObservation &obs, INT_PTR nLeg)
{
	_TCHAR sztype[2] = {0, 0};
	int iImage = 0;
	switch (obs.type)
	{
		case TT_ORIENTATION: sztype[0] = _T('O'); iImage = 0; break;
		case TT_FORWARD:     sztype[0] = _T('F'); iImage = 1; break;
		case TT_BACKWARD:    sztype[0] = _T('B'); iImage = 2; break;
		case TT_NETWORK:     sztype[0] = _T('N'); iImage = 3; break;
		case TT_NONE:        sztype[0] = _T('-'); iImage = 4; break;
	}

	CString horizontal, distance, height;
	CString target = obs.obs.targetName;
	horizontal.Format(_T("%.4f"), obs.obs.horizontalAngle);
	distance.Format(_T("%.3f"), obs.obs.slopeDistance);
	height.Format(_T("%.3f"), obs.obs.targetHeight);

	int nItem = m_obsList.GetItemCount();

	LVITEM sItem = {LVIF_TEXT | LVIF_IMAGE, nItem, 0, 0, 0, sztype, 2, iImage};
	nItem = m_obsList.InsertItem(&sItem);

	sItem.iSubItem = 1;
	sItem.pszText = target.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	target.ReleaseBuffer();

	sItem.iSubItem = 2;
	sItem.pszText = horizontal.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	horizontal.ReleaseBuffer();

	sItem.iSubItem = 3;
	sItem.pszText = distance.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	distance.ReleaseBuffer();

	sItem.iSubItem = 4;
	sItem.pszText = height.GetBuffer(0);
	m_obsList.SetItem(&sItem);
	height.ReleaseBuffer();

	m_obsList.SetItemData(nItem, nLeg);
}

TraverseObservation CTraverseDlg::GetObservationAtListItem(int nItem) const
{
	TraverseObservation tobs;
	if (nItem < m_obsList.GetItemCount())
	{
		CString type = m_obsList.GetItemText(nItem, 0);
		CString target = m_obsList.GetItemText(nItem, 1);
		CString horizontal = m_obsList.GetItemText(nItem, 2);
		CString distance = m_obsList.GetItemText(nItem, 3);
		CString height = m_obsList.GetItemText(nItem, 4);

		tobs.obs.targetName = target;
		tobs.obs.horizontalAngle = _tstof(horizontal.GetString());
		tobs.obs.verticalAngle = 90.0;
		tobs.obs.slopeDistance = _tstof(distance.GetString());
		tobs.obs.targetHeight = _tstof(height.GetString());

		switch (type.GetAt(0))
		{
			case _T('O'): tobs.type = TT_ORIENTATION; break;
			case _T('F'): tobs.type = TT_FORWARD; break;
			case _T('B'): tobs.type = TT_BACKWARD; break;
			case _T('N'): tobs.type = TT_NETWORK; break;
			case _T('-'): tobs.type = TT_NONE; break;
		}
	}
	return tobs;
}

void CTraverseDlg::OnNMDblclkLegobslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	TraverseObservation tobs = GetObservationAtListItem(pNMItemActivate->iItem);
	CTraverseObsDlg dlg(m_xml, tobs, this);
	dlg.DoModal();

	*pResult = 0;
}

void CTraverseDlg::OnLvnKeydownLegobslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	switch (pLVKeyDow->wVKey)
	{
	case VK_DELETE:
		{
		CArray<INT_PTR> victims;

		POSITION pos = m_obsList.GetFirstSelectedItemPosition();
		if (pos)
		{
			while (pos)
			{
				int nItem = m_obsList.GetNextSelectedItem(pos);
				INT_PTR legIndex = (INT_PTR)m_obsList.GetItemData(nItem);
				victims.Add(legIndex);
			}
		}

		if (!victims.IsEmpty())
		{
			CString msg;
			msg.Format(_T("Are you sure you want to delete %d observation(s)?"), victims.GetCount());

			if (IDYES == MessageBox(msg, _T("Confirm"), MB_YESNO | MB_ICONWARNING))
			{
				for (INT_PTR i = victims.GetCount() - 1; i >= 0; --i)
				{
					INT_PTR legIndex = victims.GetAt(i);
					m_legs.RemoveAt(legIndex);
				}

				RefreshList();
			}
		}
		break;
		}
	}

	*pResult = 0;
}

void CTraverseDlg::SortList()
{
	std::sort(m_legs.GetData(), m_legs.GetData() + m_legs.GetSize(), TraverseObservation::SortByType);
}

void CTraverseDlg::OnFileOpen()
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = _T("LandXML Files (*.xml)\0*.xml\0Civil Designer fieldbook (*.csv)\0*.csv\0\0");

	if(dlg.DoModal() == IDOK)
		{
		bool initialised = !m_legs.IsEmpty() || m_routeList.GetItemCount() > 0;
		if (initialised)
			{
			CString msg = _T("Opening a new file will cause you to lose your current calculation data.\n") \
				_T("Are you sure you want to continue?");
			if (IDNO == MessageBox(msg, _T("Confirm"), MB_YESNO | MB_ICONQUESTION))
				return;
			}

		m_xml.Reset();

		if (dlg.m_ofn.nFilterIndex == 1)
			{
			m_xml.filename = dlg.GetPathName();
			EnableWindow(IDC_DEFINEROUTEBTN, TRUE);
			}
		else
			{
			MessageBox(_T("Currently unsupported filetype"), _T("Error"), MB_OK | MB_ICONEXCLAMATION);
			return;
			}

		m_legs.RemoveAll();
		RefreshList();
		m_routeList.DeleteAllItems();
		}
}

OccupiedStation CTraverseDlg::GetStationAtRouteItem(int nLeg) const
	{
	OccupiedStation ret;
	ret.setupName = m_routeList.GetItemText(nLeg, 0);
	ret.setupNumber = _tstoi(m_routeList.GetItemText(nLeg, 1).GetString());
	return ret;
	}