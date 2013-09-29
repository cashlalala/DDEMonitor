
// DDEMonitorDlg.cpp : ��@��
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

// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
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


// CDDEMonitorDlg ��ܤ��



CDDEMonitorDlg::CDDEMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDDEMonitorDlg::IDD, pParent)
	, m_strServer(_T(""))
	, m_strTopic(_T(""))
	, m_strItem(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDDEMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strServer);
	DDX_Text(pDX, IDC_EDIT2, m_strTopic);
	DDX_Text(pDX, IDC_EDIT3, m_strItem);
}

BEGIN_MESSAGE_MAP(CDDEMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CONN, &CDDEMonitorDlg::OnBnClickedButtonConn)
	ON_BN_CLICKED(IDC_BUTTON_GETITEM, &CDDEMonitorDlg::OnBnClickedButtonGetitem)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDDEMonitorDlg::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDDEMonitorDlg �T���B�z�`��

BOOL CDDEMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w
	try
	{
		m_ddeOper.InitInstance(MMSDDSINST,0);
		m_strServer = _T("DdemlSvr");
		m_strTopic = _T("MyTopic");
		m_strItem = _T("MyItem");

		UpdateData(FALSE);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}
	

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CDDEMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
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
		HDDEDATA hData = m_ddeOper.DoTransaction(m_strConvId,m_strItem,CF_TEXT,XTYP_ADVSTART|XTYPF_ACKREQ,5000);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}
}


void CDDEMonitorDlg::OnBnClickedButtonCancel()
{
	try
	{
		HDDEDATA hData = m_ddeOper.DoTransaction(m_strConvId,m_strItem,CF_TEXT,XTYP_ADVSTOP,5000);
	}
	catch (CDDEException& e)
	{
		TRACE(_T("%s\n"),e.twhat());
	}
	
}
