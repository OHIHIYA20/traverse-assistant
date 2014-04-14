
// TraverseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Traverse.h"
#include "TraverseDlg.h"
#include "TraverseRouteDlg.h"
#include "TraverseObsDlg.h"
#include "ObservationsDlg.h"
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

	m_xml.filename = _T("C:\\Src-scratch\\Traverse\\Debug\\data\\j153.sqlite.xml");
}

void CTraverseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROUTELISTBOX, m_stationNamesListBox);
	DDX_Control(pDX, IDC_LEGOBSLAB, m_legObsLab);
	DDX_Control(pDX, IDC_LEGOBSLIST, m_obsList);
}

BEGIN_MESSAGE_MAP(CTraverseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DEFINEROUTEBTN, &CTraverseDlg::OnBnClickedDefineroutebtn)
	ON_BN_CLICKED(IDC_ADDLEGOBSBTN, &CTraverseDlg::OnBnClickedAddlegobsbtn)
	ON_LBN_SELCHANGE(IDC_ROUTELISTBOX, &CTraverseDlg::OnLbnSelchangeRoutelistbox)
	ON_BN_CLICKED(IDC_LOADOBSBTN, &CTraverseDlg::OnBnClickedLoadobsbtn)
	ON_NOTIFY(NM_DBLCLK, IDC_LEGOBSLIST, &CTraverseDlg::OnNMDblclkLegobslist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LEGOBSLIST, &CTraverseDlg::OnLvnKeydownLegobslist)
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

	m_obsList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CRect rect;
	m_obsList.GetWindowRect(&rect);
	int columnWidth = rect.Width() / 5;

	m_obsList.InsertColumn(0, _T("Type"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(1, _T("Target"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(2, _T("Horizontal"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(3, _T("Reduced"), LVCFMT_LEFT,  columnWidth);
	m_obsList.InsertColumn(4, _T("Target Height"), LVCFMT_LEFT,  columnWidth);

	return TRUE;  // return TRUE  unless you set the focus to a control
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
	CString route;
	for (int i = 0; i < m_stationNamesListBox.GetCount(); ++i)
	{
		CString legName;
		m_stationNamesListBox.GetText(i, legName);

		route.Append(legName);
		route.AppendChar(_T(' '));
	}

	CTraverseRouteDlg dlg(route, this);
	if (IDOK == dlg.DoModal())
	{
		CStringArray stationNames;
		SplitStringAtSpaces(route, stationNames);

		m_stationNamesListBox.ResetContent();
		for (INT_PTR i = 0; i < stationNames.GetSize(); ++i)
			m_stationNamesListBox.AddString(stationNames.GetAt(i));

		if (stationNames.GetSize() > 0)
		{
			m_stationNamesListBox.SetCurSel(0);
			OnLbnSelchangeRoutelistbox();
		}
	}
}

void CTraverseDlg::OnBnClickedAddlegobsbtn()
{
	int nLeg = m_stationNamesListBox.GetCurSel();
	CString stationName;
	m_stationNamesListBox.GetText(nLeg, stationName);

	TraverseObservation tobs;
	tobs.routeSequence = nLeg;

	CTraverseObsDlg dlg(m_xml, tobs, this);
	if (IDOK == dlg.DoModal())
	{
		m_legs.Add(tobs);
		RefreshList();
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

void CTraverseDlg::OnLbnSelchangeRoutelistbox()
{
	int selection = m_stationNamesListBox.GetCurSel();
	CString stationName;
	m_stationNamesListBox.GetText(selection, stationName);

	CString stationNumber = _T("1");

	CString caption;
	caption.Format(_T("Observations @ %s (%s):"), stationName, stationNumber);
	m_legObsLab.SetWindowText(caption);

	RefreshList();
}

void CTraverseDlg::OnBnClickedLoadobsbtn()
{
	int nLeg = m_stationNamesListBox.GetCurSel();
	if (nLeg == LB_ERR)
		return;

	CString stationName;
	m_stationNamesListBox.GetText(nLeg, stationName);

	CString stationNumber = _T("1");

	CObservationsDlg dlg(m_xml, stationName, stationNumber, false, this);
	if (IDOK == dlg.DoModal())
	{
		CArray<Observation> obs;
		dlg.GetMultipleSelectedObs(obs);

		for (INT_PTR i = 0; i < obs.GetCount(); ++i)
		{
			Observation o = obs.GetAt(i);
			ETraverseType type = GetObservationType(o, stationName);

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

ETraverseType CTraverseDlg::GetObservationType(const Observation &obs, CString stationName) const
{
	int nLeg = -1;
	int nStations = m_stationNamesListBox.GetCount();

	CStringArray stations;
	for (int i = 0; i < nStations; ++i)
	{
		CString legName;
		m_stationNamesListBox.GetText(i, legName);
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
	int nLeg = m_stationNamesListBox.GetCurSel();
	if (nLeg == LB_ERR)
		return;

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

void CTraverseDlg::AppendObservationToList(const TraverseObservation &obs, INT_PTR nLeg)
{
	_TCHAR sztype[2] = {0, 0};
	switch (obs.type)
	{
		case TT_ORIENTATION: sztype[0] = _T('O'); break;
		case TT_FORWARD:     sztype[0] = _T('F'); break;
		case TT_BACKWARD:    sztype[0] = _T('B'); break;
		case TT_NETWORK:     sztype[0] = _T('N'); break;
		case TT_NONE:        sztype[0] = _T('-'); break;
	}

	CString horizontal, distance, height;
	CString target = obs.obs.targetName;
	horizontal.Format(_T("%.4f"), obs.obs.horizontalAngle);
	distance.Format(_T("%.3f"), obs.obs.slopeDistance);
	height.Format(_T("%.3f"), obs.obs.targetHeight);

	int nItem = m_obsList.GetItemCount();

	LVITEM sItem = {LVIF_TEXT, nItem, 0, 0, 0, sztype};
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
