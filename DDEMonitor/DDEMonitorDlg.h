
// DDEMonitorDlg.h : 標頭檔
//

#pragma once

#include "DDE\DDEOperation.h"

#include "CustomControl\DataGrid\GridCtrl.h"

// CDDEMonitorDlg 對話方塊
class CDDEMonitorDlg : public CDialogEx
{
// 建構
public:
	CDDEMonitorDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_DDEMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	DDE::CDDEOperation m_ddeOper; 

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	
	CString m_strServer;
	CString m_strTopic;
	CString m_strItem;
	CString m_strConvId;

	CGridCtrl m_ctrlGridDDEFunc;
	CGridCtrl m_ctrlGridOutput;

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonConn();
	afx_msg void OnBnClickedButtonGetitem();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButtonCancelAdvice();
};
