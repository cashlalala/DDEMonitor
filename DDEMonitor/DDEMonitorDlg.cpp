
// DDEMonitorDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "DDEMonitor.h"
#include "DDEMonitorDlg.h"
#include "afxdialogex.h"
#include "DDE\DDEException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DDE;

#define MMSDDSINST _T("MMS")

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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDDEMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strServer);
	DDX_Text(pDX, IDC_EDIT2, m_strTopic);
	DDX_Text(pDX, IDC_EDIT3, m_strItem);
	DDX_Control(pDX, IDC_GRID, m_ctrlGridDDEFunc);
	DDX_Control(pDX, IDC_GRID_OUTPUT, m_ctrlGridOutput);	
	DDX_Text(pDX, IDC_EDIT_ITEMNAME, m_strItemName);
	DDX_Control(pDX, IDC_BUTTON_ADVISE, m_btnAdvise);
}

BEGIN_MESSAGE_MAP(CDDEMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CONN, &CDDEMonitorDlg::OnBnClickedButtonConn)
	ON_BN_CLICKED(IDC_BUTTON_GETITEM, &CDDEMonitorDlg::OnBnClickedButtonGetitem)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDDEMonitorDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_Remove, &CDDEMonitorDlg::OnBnClickedButtonRemove)
	ON_NOTIFY(NM_CLICK, IDC_GRID, &CDDEMonitorDlg::OnDDEFuncGridClick)
	ON_BN_CLICKED(IDC_BUTTON_ADVISE, &CDDEMonitorDlg::OnBnClickedButtonAdvise)
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
	m_ctrlGridDDEFunc.SetEditable(TRUE);
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

	item.mask = GVIF_TEXT;
	item.row = 0;
	item.col = 1;
	item.strText = _T("Item Name");
	m_ctrlGridDDEFunc.SetItem(&item);

	m_mapCurItem[_T("M1JY&.184")] = _T("漲跌");
	m_mapCurItem[_T("M1JY&.125")] = _T("成交價");
	m_mapCurItem[_T("M1JY&.101")] = _T("買價");
	m_mapCurItem[_T("M1JY&.102")] = _T("賣價");
	m_mapCurItem[_T("M1JY&.404")] = _T("成交量");

	m_ctrlGridDDEFunc.InsertRow(_T("M1JY&.184"));
	m_ctrlGridDDEFunc.SetItemText(1,1,_T("漲跌"));
	m_ctrlGridDDEFunc.InsertRow(_T("M1JY&.125"));
	m_ctrlGridDDEFunc.SetItemText(2,1,_T("成交價"));
	m_ctrlGridDDEFunc.InsertRow(_T("M1JY&.101"));
	m_ctrlGridDDEFunc.SetItemText(3,1,_T("買價"));
	m_ctrlGridDDEFunc.InsertRow(_T("M1JY&.102"));
	m_ctrlGridDDEFunc.SetItemText(4,1,_T("賣價"));
	m_ctrlGridDDEFunc.InsertRow(_T("M1JY&.404"));
	m_ctrlGridDDEFunc.SetItemText(5,1,_T("成交量"));

	m_ctrlGridDDEFunc.Invalidate();

	m_ctrlGridOutput.SetEditable(TRUE);
	m_ctrlGridOutput.EnableDragAndDrop(TRUE);
	m_ctrlGridOutput.SetRowCount(1);
	m_ctrlGridOutput.SetColumnCount(10);
	m_ctrlGridOutput.SetFixedRowCount(1);
	m_ctrlGridOutput.SetFixedColumnCount(1);

	try
	{
		m_ddeOper.InitInstance(MMSDDSINST,0);
		m_strServer = _T("MMSDDE");
		m_strTopic = _T("FUSA");
		m_strItem = _T("M1BPZ3&.143");

		UpdateData(FALSE);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}


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
		TRACE(_T("%s\n"),e.twhat());
	}
	

	CDialogEx::OnClose();
}


void CDDEMonitorDlg::OnBnClickedButtonConn()
{
	UpdateData(TRUE);
	try
	{
		m_strConvId = m_ddeOper.Connect(MMSDDSINST,m_strServer,m_strTopic,NULL);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}
	catch (std::exception& e)
	{
		CString strWhat = CA2W(e.what());
		TRACE(_T("%s\n"),strWhat);
	}
	
}


void CDDEMonitorDlg::OnBnClickedButtonGetitem()
{
	UpdateData(TRUE);

	try
	{
		m_ddeOper.DoTransaction(m_strConvId,m_strItem,CF_TEXT,XTYP_ADVSTART|XTYPF_ACKREQ,5000);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}
}


void CDDEMonitorDlg::OnBnClickedButtonCancelAdvice()
{
	try
	{
		m_ddeOper.DoTransaction(m_strConvId,m_strItem,CF_TEXT,XTYP_ADVSTOP,5000);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}
}


void CDDEMonitorDlg::OnBnClickedButtonAdd()
{
	UpdateData(TRUE);
	if (m_mapCurItem.end()==m_mapCurItem.find(m_strItem))
	{
		m_mapCurItem[m_strItem] = m_strItemName;
		m_ctrlGridDDEFunc.InsertRow(m_strItem);	
	}
	m_ctrlGridDDEFunc.SetItemText(m_ctrlGridDDEFunc.GetRowCount()-1,1,m_strItemName);
	m_ctrlGridDDEFunc.Invalidate();
}


void CDDEMonitorDlg::OnBnClickedButtonRemove()
{
	UpdateData(TRUE);

	for (int i=1;i<m_ctrlGridDDEFunc.GetRowCount();++i)
	{
		TRACE(L"Current item %s...\n",m_ctrlGridDDEFunc.GetItemText(i,0));
		if (m_strItem == m_ctrlGridDDEFunc.GetItemText(i,0)
			&& m_strItemName == m_ctrlGridDDEFunc.GetItemText(i,1))
		{
			std::map<CString,CString>::iterator itItem = m_mapCurItem.find(m_strItem);
			m_mapCurItem.erase(itItem);
			m_ctrlGridDDEFunc.DeleteRow(i);
			m_ctrlGridDDEFunc.Invalidate();
			break;
		}
	}

	m_strItem = _T("");
	m_strItemName = _T("");

	UpdateData(FALSE);
}

void CDDEMonitorDlg::OnDDEFuncGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	TRACE(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);

	if (pItem->iRow!=0) // the header line
	{
		m_strItem = m_ctrlGridDDEFunc.GetItemText(pItem->iRow, 0);
		m_strItemName = m_ctrlGridDDEFunc.GetItemText(pItem->iRow, 1);
		UpdateData(FALSE);
	}
}


void CDDEMonitorDlg::OnBnClickedButtonAdvise()
{
	UpdateData(TRUE);

	TCHAR lptszText[50];
	memset(lptszText,0x0,sizeof(TCHAR)*50);
	m_btnAdvise.GetWindowText(lptszText,48);

	bool bIsStart = (_tcscmp(lptszText,_T("Start Advise"))==0)? true : false;

	try
	{
		if (bIsStart)
		{
			m_btnAdvise.SetWindowText(_T("Stop Advise"));
			for (auto itemPair : m_mapCurItem)
			{
				m_ddeOper.DoTransaction(m_strConvId,itemPair.first,CF_TEXT,XTYP_REQUEST,60000);

				m_ddeOper.DoTransaction(m_strConvId,itemPair.first,CF_TEXT,XTYP_ADVSTART,5000);
			}
		}
		else
		{
			m_btnAdvise.SetWindowText(_T("Start Advise"));
			for (auto itemPair : m_mapCurItem)
			{
				m_ddeOper.DoTransaction(m_strConvId,itemPair.first,CF_TEXT,XTYP_ADVSTOP,5000);
			}
		}
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}

}
