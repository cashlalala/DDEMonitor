
// DDEMonitorDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "DDEMonitor.h"
#include "DDEMonitorDlg.h"
#include "afxdialogex.h"
#include "DDE\DDEException.h"
#include "DDE\DDEKernel.h"
#include "DDE\DDEItemsHelper.h"

#include <algorithm>

#pragma comment (lib,"Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DDE;

#define MMSDDSINST _T("MMS")
#define EQU_RANGE_100MS 1000000

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDDEMonitorDlg 對話方塊



CDDEMonitorDlg::CDDEMonitorDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CDDEMonitorDlg::IDD, pParent)
, m_strServer(_T(""))
, m_strTopic(_T(""))
, m_strItem(_T(""))
, m_strItemName(_T(""))
, m_nInterval(1)
, m_dwTimerStatistic(0)
, m_eventStatistic(TRUE, TRUE)
, m_unResolution(10)
, m_dwTimerId(0)
, m_ullTimeLBound(0)
, m_ullTimeUBound(0)
, m_ullLastUpdate(0)
, m_bBeginFlag(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (auto& item : m_ulStatAry)
		item = 0;
}

void CDDEMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strServer);
	DDX_Text(pDX, IDC_EDIT2, m_strTopic);
	DDX_Text(pDX, IDC_CB_ITEM, m_strItem);
	DDX_Control(pDX, IDC_GRID, m_ctrlGridDDEFunc);
	DDX_Control(pDX, IDC_GRID_OUTPUT, m_ctrlGridOutput);
	DDX_Text(pDX, IDC_CB_ITEMNAME, m_strItemName);
	DDX_Control(pDX, IDC_BUTTON_ADVISE, m_btnAdvise);
	DDX_Control(pDX, IDC_CB_ITEM, m_ctrolItem);
	DDX_Control(pDX, IDC_CB_ITEMNAME, m_ctrolItemName);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_Remove, m_btnRmv);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nInterval);
	DDV_MinMaxInt(pDX, m_nInterval, 1, 120);
	DDX_Control(pDX, IDC_SLIDER_INTERVAL, m_ctrlSliderInterval);
	DDX_Control(pDX, IDC_BUTTON_ADDALLNAME, m_btnAddAll);
}

BEGIN_MESSAGE_MAP(CDDEMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CONN, &CDDEMonitorDlg::OnBnClickedButtonConn)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDDEMonitorDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_Remove, &CDDEMonitorDlg::OnBnClickedButtonRemove)
	ON_NOTIFY(NM_CLICK, IDC_GRID, &CDDEMonitorDlg::OnDDEFuncGridClick)
	ON_BN_CLICKED(IDC_BUTTON_ADVISE, &CDDEMonitorDlg::OnBnClickedButtonAdvise)
	ON_MESSAGE(WM_UPDATE_DATA, &CDDEMonitorDlg::OnUpdateOutput)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_INTERVAL, &CDDEMonitorDlg::OnNMCustomdrawSliderInterval)
	ON_EN_KILLFOCUS(IDC_EDIT_INTERVAL, &CDDEMonitorDlg::OnEnKillfocusEditInterval)
	ON_BN_CLICKED(IDC_BUTTON_ADDALLNAME, &CDDEMonitorDlg::OnBnClickedButtonAddallname)
END_MESSAGE_MAP()


// CDDEMonitorDlg 訊息處理常式

BOOL CDDEMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	timeBeginPeriod(m_unResolution);

	m_ctrlSliderInterval.SetRange(1, 120);
	m_ctrlSliderInterval.SetPageSize(300);
	m_ctrlSliderInterval.SetPos(1);

	m_ctrlGridDDEFunc.SetEditable(FALSE);
	m_ctrlGridDDEFunc.EnableDragAndDrop(TRUE);
	m_ctrlGridDDEFunc.SetColumnCount(5);
	m_ctrlGridDDEFunc.SetFixedRowCount(1);
	m_ctrlGridDDEFunc.SetRowCount(1);

	GV_ITEM item;

	item.mask = GVIF_TEXT;
	item.row = 0;
	item.col = 0;
	item.strText = _T("Item DDE Id");
	m_ctrlGridDDEFunc.SetItem(&item);

	item.row = 0;
	item.col = 1;
	item.strText = _T("Item Name");
	m_ctrlGridDDEFunc.SetItem(&item);

	m_ctrlGridDDEFunc.Invalidate();

	m_ctrlGridOutput.SetEditable(FALSE);
	m_ctrlGridOutput.EnableDragAndDrop(FALSE);
	m_ctrlGridOutput.SetRowCount(2);
	m_ctrlGridOutput.SetColumnCount(STATISTIC_ITEM_STRINGID_COUNT);
	m_ctrlGridOutput.SetFixedRowCount(1);

	m_ctrolItem.AddString(JY_AMP_ID);

	for (const auto& strId : DDE_ITEM_STRINGID_TABLE)
	{
		CString strItemName;
		strItemName.LoadString(strId);
		m_ctrolItemName.AddString(strItemName);
	}

	item.col = 0;
	for (const auto& strId : STATISTIC_ITEM_STRINGID_TABLE)
	{
		item.row = 0;
		item.strText.LoadString(strId);
		m_ctrlGridOutput.SetItem(&item);
		++item.col;
	}

	m_ctrlGridOutput.Invalidate();

	try
	{
		m_ddeOper.InitInstance(MMSDDSINST, 0);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"), e.twhat());
	}

	m_strServer = _T("MMSDDE");
	m_strTopic = _T("FUSA");

	UpdateData(FALSE);


	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CDDEMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CDDEMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CDDEMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDDEMonitorDlg::OnClose()
{
	try
	{
		m_ddeOper.UninitAll();
	}
	catch (DDE::CDDEException& e)
	{
		TRACE(_T("%s\n"), e.twhat());
	}

	timeEndPeriod(m_unResolution);
	CDialogEx::OnClose();
}


void CDDEMonitorDlg::OnBnClickedButtonConn()
{
	UpdateData(TRUE);
	try
	{
		m_strConvId = m_ddeOper.Connect(MMSDDSINST, m_strServer, m_strTopic, NULL);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"), e.twhat());
		AfxMessageBox(e.twhat(), MB_OK);
	}

}

void CDDEMonitorDlg::OnBnClickedButtonAdd()
{
	UpdateData(TRUE);
	if (m_strItem.IsEmpty() || m_strItemName.IsEmpty())
		return;

	CString strId = CDDEItemsHelper::GetIDFromName(m_strItemName);
	CString strItemId = m_strItem + strId;
	if (m_mapCurItem.end() == m_mapCurItem.find(strItemId))
	{
		m_mapCurItem[strItemId] = m_strItemName;
		m_ctrlGridDDEFunc.InsertRow(strItemId);
		m_ctrlGridDDEFunc.SetItemText(m_ctrlGridDDEFunc.GetRowCount() - 1, 1, m_strItemName);
	}
	m_ctrlGridDDEFunc.Invalidate();
}


void CDDEMonitorDlg::OnBnClickedButtonRemove()
{
	UpdateData(TRUE);

	if (m_strItem.IsEmpty() || m_strItemName.IsEmpty())
		return;

	for (int i = 1; i < m_ctrlGridDDEFunc.GetRowCount(); ++i)
	{
		TRACE(L"Current item [%s,%s]...\n", m_ctrlGridDDEFunc.GetItemText(i, 0), m_ctrlGridDDEFunc.GetItemText(i, 1));
		CString& strItemId = m_ctrlGridDDEFunc.GetItemText(i, 0);
		if (strItemId.Find(m_strItem) != -1
			&& m_strItemName == m_ctrlGridDDEFunc.GetItemText(i, 1))
		{
			CStringMap::iterator itItem = m_mapCurItem.find(strItemId);
			m_mapCurItem.erase(itItem);
			m_ctrlGridDDEFunc.DeleteRow(i);
			m_ctrlGridDDEFunc.Invalidate();

			m_strItem = _T("");
			m_strItemName = _T("");
			break;
		}
	}

	UpdateData(FALSE);
}

void CDDEMonitorDlg::OnBnClickedButtonAdvise()
{
	UpdateData(TRUE);

	TCHAR lptszText[50];
	memset(lptszText, 0x0, sizeof(TCHAR)* 50);
	m_btnAdvise.GetWindowText(lptszText, 48);

	bool bIsStart = (_tcscmp(lptszText, _T("Start Advise")) == 0) ? true : false;

	try
	{
		if (bIsStart)
		{
			GetSystemTimeAsFileTime(&m_tLastUpdateTime);
			memcpy(&m_ullLastUpdate, &m_tLastUpdateTime, sizeof(m_ullLastUpdate));
			m_ullTimeUBound = m_nInterval * 10000000 + EQU_RANGE_100MS;
			m_ullTimeLBound = m_nInterval * 10000000 - EQU_RANGE_100MS;
			//m_dwTimerId = timeSetEvent(500,100,TimerCounter,PtrToUlong(this),
			//	TIME_PERIODIC|TIME_CALLBACK_FUNCTION|TIME_KILL_SYNCHRONOUS);
			//if (!m_dwTimerId)
			//{
			//	AfxMessageBox(_T("Create Timer Fail!"),MB_OK);
			//	return;
			//}

			m_btnAdvise.SetWindowText(_T("Stop Advise"));

			//init the data of first row 
			m_lstOutput.push_back(CStringMap());
			CStringMap& mapFirstItem = m_lstOutput.front();
			int i = 0;
			for (auto itemPair : m_mapCurItem)
			{
				CString strItemResult = m_ddeOper.Request(m_strConvId, itemPair.first, CF_TEXT);

				mapFirstItem[itemPair.second] = strItemResult;

				CString strTempTime;
				strTempTime.LoadString(IDS_DATETIME);
				if (itemPair.second == strTempTime) {
					m_ulStatAry[STATISTIC_TIME_IDX] = _tcstoul(strTempTime, NULL, 10);
				}

				//init the first statistical data
				CString strTempStrikePC;
				strTempStrikePC.LoadString(IDS_STRIKEPRICE);
				if (itemPair.second == strTempStrikePC)
				{
					m_ulStatAry[STATISTIC_OPENPC_IDX] = _tcstoul(strItemResult, NULL, 10);
					m_ulStatAry[STATISTIC_CLOSEPC_IDX] = m_ulStatAry[STATISTIC_OPENPC_IDX];
					m_ulStatAry[STATISTIC_HIGHPC_IDX] = m_ulStatAry[STATISTIC_OPENPC_IDX];
					m_ulStatAry[STATISTIC_LOWPC_IDX] = m_ulStatAry[STATISTIC_OPENPC_IDX];
				}
			}

			TRACE(_T("Init value: [%lu, %lu, %lu, %lu, %lu, %lu]\n"),
				m_ulStatAry[STATISTIC_TIME_IDX], m_ulStatAry[STATISTIC_OPENPC_IDX],
				m_ulStatAry[STATISTIC_HIGHPC_IDX], m_ulStatAry[STATISTIC_LOWPC_IDX],
				m_ulStatAry[STATISTIC_CLOSEPC_IDX], m_ulStatAry[STATISTIC_VOL_IDX]);
			//start to advise
			for (auto itemPair : m_mapCurItem)
			{
				m_ddeOper.DoTransaction(m_strConvId, itemPair.first, CF_TEXT, XTYP_ADVSTART);
			}
		}
		else
		{
			//if (m_dwTimerId)
			//{
			//	timeKillEvent(m_dwTimerId);
			//	m_dwTimerId  = 0;
			//}
			m_btnAdvise.SetWindowText(_T("Start Advise"));
			for (auto itemPair : m_mapCurItem)
			{
				m_ddeOper.DoTransaction(m_strConvId, itemPair.first, CF_TEXT, XTYP_ADVSTOP);
			}
		}

		DisableCtrl(!bIsStart);

	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"), e.twhat());
	}

	UpdateData(FALSE);
}

void CDDEMonitorDlg::OnBnClickedButtonAddallname()
{
	UpdateData(TRUE);

	if (m_strItem.IsEmpty())
		return;

	for (const auto& nRCID : DDE_ITEM_STRINGID_TABLE)
	{
		CString strItemName;
		strItemName.LoadString(nRCID);

		CString strItemId = m_strItem + CDDEItemsHelper::GetIDFromRCID(nRCID);
		if (m_mapCurItem.end() == m_mapCurItem.find(strItemId))
		{
			m_mapCurItem[strItemId] = strItemName;
			m_ctrlGridDDEFunc.InsertRow(strItemId);
			m_ctrlGridDDEFunc.SetItemText(m_ctrlGridDDEFunc.GetRowCount() - 1, 1, strItemName);
		}
	}

	m_ctrlGridDDEFunc.Invalidate();
}

void CDDEMonitorDlg::OnDDEFuncGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	if (pItem->iRow != 0) // the header line
	{
		UpdateData(TRUE);
		int nIdx = 0;
		m_strItem = m_ctrlGridDDEFunc.GetItemText(pItem->iRow, 0).Tokenize(_T("."), nIdx);
		m_strItemName = m_ctrlGridDDEFunc.GetItemText(pItem->iRow, 1);
		UpdateData(FALSE);
	}
}


LRESULT CDDEMonitorDlg::OnUpdateOutput(WPARAM wParam, LPARAM lParam)
{
	if (!lParam || !wParam) return E_FAIL;

	TCHAR* pData = (TCHAR*)lParam;
	TypDDEItem* pItem = (TypDDEItem*)wParam;
	static ULONG ulLastVol = 0;
	static ULONG lnLastTime = m_ulStatAry[STATISTIC_TIME_IDX];

	do
	{
		if (IDS_TIME == CDDEItemsHelper::GetRCIDFromID(pItem->strItem))
		{
			lnLastTime = m_ulStatAry[STATISTIC_TIME_IDX];
			m_ulStatAry[STATISTIC_TIME_IDX] = _tcstoul(pData, NULL, 10);
			const ULONG& lnNow = m_ulStatAry[STATISTIC_TIME_IDX];

			//if (lnLastTime == lnNow) {
			//	if (m_bBeginFlag && m_mapDelayOpen.count(lnNow) == m_mapDelayOpen.size())
			//		break; //keep accumulating
			//}

			if (1 == m_nInterval) {
				if (lnNow / 100 - lnLastTime / 100 == m_nInterval)//flush
				{
//					m_bBeginFlag = true;
//#ifdef _DEBUG
//					for (const auto& par : m_mapDelayOpen) {
//						TRACE(_T("%s : %s\n"),par.first,par.second);
//					}
//#endif
//					m_mapDelayOpen.clear();

					m_ctrlGridOutput.InsertRow(_T(""), 1);

					CString strTempValue;
					strTempValue.Format(_T("%lu"), lnNow / 100);
					m_ctrlGridOutput.SetItemText(1, 0, strTempValue);

					if (m_mapOpenClose.begin() != m_mapOpenClose.end()) {
						m_ctrlGridOutput.SetItemText(1, 1, (*m_mapOpenClose.begin()).second);
						TRACE(_T("open price: [%s]\n"), (*m_mapOpenClose.begin()).second);
					}
					else {
						strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_OPENPC_IDX]);
						m_ctrlGridOutput.SetItemText(1, 1, strTempValue);
					}

					auto minmaxPair = std::minmax_element(m_mapOpenClose.begin(), m_mapOpenClose.end(),
						[](const std::pair<ULONG, CString>& a, const std::pair<ULONG, CString>& b)->bool {
						return (_tcstoul(a.second, NULL, 10) < _tcstoul(b.second, NULL, 10)) ? true : false;
					});

					m_ulStatAry[STATISTIC_HIGHPC_IDX] = _tcstoul((*minmaxPair.second).second, NULL, 10);
					m_ctrlGridOutput.SetItemText(1, 2, (*minmaxPair.second).second);

					m_ulStatAry[STATISTIC_LOWPC_IDX] = _tcstoul((*minmaxPair.first).second, NULL, 10);
					m_ctrlGridOutput.SetItemText(1, 3, (*minmaxPair.first).second);

					if (m_mapOpenClose.rbegin() != m_mapOpenClose.rend()) {
						m_ctrlGridOutput.SetItemText(1, 4, (*m_mapOpenClose.rbegin()).second);
						TRACE(_T("close price: [%s]\n"), (*m_mapOpenClose.rbegin()).second);
					}
					else {
						strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_CLOSEPC_IDX]);
						m_ctrlGridOutput.SetItemText(1, 4, strTempValue);
					}

					LONG lDiff = m_ulStatAry[STATISTIC_VOL_IDX] - ulLastVol;
					strTempValue.Format(_T("%lu(%s%d)"), m_ulStatAry[STATISTIC_VOL_IDX], ((lDiff >= 0) ? L"+" : L"-"), ((lDiff >= 0) ? lDiff : -lDiff));
					m_ctrlGridOutput.SetItemText(1, 5, strTempValue);
					ulLastVol = m_ulStatAry[STATISTIC_VOL_IDX];

					m_ctrlGridOutput.Invalidate();
					m_mapOpenClose.clear();
				}
				//else
				//	m_bBeginFlag = false;
			}
		}

		//update statistical data
		if (IDS_STRIKEPRICE == CDDEItemsHelper::GetRCIDFromID(pItem->strItem))
		{
			m_mapOpenClose.insert(std::make_pair(m_ulStatAry[STATISTIC_TIME_IDX], pData));
			//if (m_bBeginFlag)
			//	m_mapDelayOpen.insert(std::make_pair(m_ulStatAry[STATISTIC_TIME_IDX], pData));
		}

		if (IDS_VOLUME == CDDEItemsHelper::GetRCIDFromID(pItem->strItem))
		{
			m_ulStatAry[STATISTIC_VOL_IDX] = _tcstoul(pData, NULL, 10);
		}

	} while (false);

	TRACE(_T("Current change: [%lu, %lu, %lu, %lu, %lu, %lu]\n"),
		m_ulStatAry[STATISTIC_TIME_IDX], m_ulStatAry[STATISTIC_OPENPC_IDX],
		m_ulStatAry[STATISTIC_HIGHPC_IDX], m_ulStatAry[STATISTIC_LOWPC_IDX],
		m_ulStatAry[STATISTIC_CLOSEPC_IDX], m_ulStatAry[STATISTIC_VOL_IDX]);

	delete[] pData;
	delete pItem;

	return 0;
}


void CDDEMonitorDlg::OnNMCustomdrawSliderInterval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	m_nInterval = m_ctrlSliderInterval.GetPos();

	*pResult = 0;

	UpdateData(FALSE);
}


void CDDEMonitorDlg::OnEnKillfocusEditInterval()
{
	UpdateData(TRUE);
	m_ctrlSliderInterval.SetPos(m_nInterval);
	UpdateData(FALSE);
}


void CDDEMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CDialogEx::OnTimer(nIDEvent);

	m_eventStatistic.ResetEvent();

	//push into output
	int i = 0;
	m_ctrlGridOutput.InsertRow(_T(""), 1);
	CString strTempValue;

	strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_OPENPC_IDX]);
	m_ctrlGridOutput.SetItemText(1, 1, strTempValue);
	//update current open to previous close
	m_ulStatAry[STATISTIC_OPENPC_IDX] = m_ulStatAry[STATISTIC_CLOSEPC_IDX];

	strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_HIGHPC_IDX]);
	m_ctrlGridOutput.SetItemText(1, 2, strTempValue);
	m_ulStatAry[STATISTIC_HIGHPC_IDX] = m_ulStatAry[STATISTIC_OPENPC_IDX];

	strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_LOWPC_IDX]);
	m_ctrlGridOutput.SetItemText(1, 3, strTempValue);
	m_ulStatAry[STATISTIC_LOWPC_IDX] = m_ulStatAry[STATISTIC_OPENPC_IDX];

	strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_CLOSEPC_IDX]);
	m_ctrlGridOutput.SetItemText(1, 4, strTempValue);

	strTempValue.Format(_T("%lu"), m_ulStatAry[STATISTIC_VOL_IDX]);
	m_ctrlGridOutput.SetItemText(1, 5, strTempValue);
	m_ulStatAry[STATISTIC_VOL_IDX] = 0;

	m_eventStatistic.SetEvent();

	m_ctrlGridOutput.Invalidate();
}

void CDDEMonitorDlg::TimerCounter(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	FILETIME tCurrentTime;

	GetSystemTimeAsFileTime(&tCurrentTime);

	CDDEMonitorDlg* pDlg = (CDDEMonitorDlg*)dwUser;

	ULONGLONG ullCur;
	memcpy(&ullCur, &tCurrentTime, sizeof(ULONGLONG));

	ULONGLONG dwDiff = ullCur - pDlg->m_ullLastUpdate;
	TRACE(L"[%llu]  [%llu] [%llu]\n", pDlg->m_ullTimeLBound, dwDiff, pDlg->m_ullTimeUBound);
	if (dwDiff <= pDlg->m_ullTimeUBound && dwDiff >= pDlg->m_ullTimeLBound)
	{
		int i = 0;
		pDlg->m_ctrlGridOutput.InsertRow(_T(""), 1);
		CString strTempValue;

		strTempValue.Format(_T("%lu"), pDlg->m_ulStatAry[STATISTIC_OPENPC_IDX]);
		pDlg->m_ctrlGridOutput.SetItemText(1, 1, strTempValue);
		//update current open to previous close
		pDlg->m_ulStatAry[STATISTIC_OPENPC_IDX] = pDlg->m_ulStatAry[STATISTIC_CLOSEPC_IDX];

		strTempValue.Format(_T("%lu"), pDlg->m_ulStatAry[STATISTIC_HIGHPC_IDX]);
		pDlg->m_ctrlGridOutput.SetItemText(1, 2, strTempValue);
		pDlg->m_ulStatAry[STATISTIC_HIGHPC_IDX] = pDlg->m_ulStatAry[STATISTIC_OPENPC_IDX];

		strTempValue.Format(_T("%lu"), pDlg->m_ulStatAry[STATISTIC_LOWPC_IDX]);
		pDlg->m_ctrlGridOutput.SetItemText(1, 3, strTempValue);
		pDlg->m_ulStatAry[STATISTIC_LOWPC_IDX] = pDlg->m_ulStatAry[STATISTIC_OPENPC_IDX];

		strTempValue.Format(_T("%lu"), pDlg->m_ulStatAry[STATISTIC_CLOSEPC_IDX]);
		pDlg->m_ctrlGridOutput.SetItemText(1, 4, strTempValue);

		strTempValue.Format(_T("%lu"), pDlg->m_ulStatAry[STATISTIC_VOL_IDX]);
		pDlg->m_ctrlGridOutput.SetItemText(1, 5, strTempValue);
		pDlg->m_ulStatAry[STATISTIC_VOL_IDX] = 0;

		pDlg->m_ctrlGridOutput.Invalidate();

		pDlg->m_tLastUpdateTime = tCurrentTime;
		memcpy(&pDlg->m_ullLastUpdate, &pDlg->m_tLastUpdateTime, sizeof(ULONGLONG));
	}
}


void CDDEMonitorDlg::DisableCtrl(bool bIsStart)
{
	m_ctrolItem.EnableWindow(bIsStart);
	m_ctrolItemName.EnableWindow(bIsStart);
	m_btnAdd.EnableWindow(bIsStart);
	m_btnRmv.EnableWindow(bIsStart);
	m_ctrlSliderInterval.EnableWindow(bIsStart);
	m_btnAddAll.EnableWindow(bIsStart);
}
