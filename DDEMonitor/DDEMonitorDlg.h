
// DDEMonitorDlg.h : 標頭檔
//

#pragma once

#include "DDE\DDEOperation.h"

#include "CustomControl\DataGrid\GridCtrl.h"

#include <vector>
#include <map>
#include <unordered_map>
#include "afxwin.h"
#include "Core\StringUtil.h"

typedef std::unordered_map<CString,CString,Core::CStringHash,Core::CStringEqul> CStringMap;

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

	//controls 
	CComboBox m_ctrolItem;
	CComboBox m_ctrolItemName;

	CGridCtrl m_ctrlGridDDEFunc;
	CGridCtrl m_ctrlGridOutput;

	CButton m_btnAdd;
	CButton m_btnRmv;

	CButton m_btnAdvise;

	//model
	CStringMap m_mapCurItem;
	std::vector<CStringMap> m_lstOutput;

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonConn();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnDDEFuncGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	
	afx_msg void OnBnClickedButtonAdvise();
	afx_msg LRESULT OnUpdateOutput( WPARAM wParam,LPARAM lParam);

	};
