
// DDEMonitorDlg.h : 標頭檔
//

#pragma once

#include "DDE\DDEOperation.h"

#include "CustomControl\DataGrid\GridCtrl.h"

#include <vector>
#include <map>
#include "afxwin.h"

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
	CString m_strItemName;

	CGridCtrl m_ctrlGridDDEFunc;
	CGridCtrl m_ctrlGridOutput;

	CButton m_btnAdvise;

	std::map<CString,CString> m_mapCurItem;

	std::vector<std::map<CString,CString> > m_lstOutput;

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonConn();
	afx_msg void OnBnClickedButtonGetitem();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButtonCancelAdvice();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnDDEFuncGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	
	afx_msg void OnBnClickedButtonAdvise();
	
};
