
// DDEMonitorDlg.h : 標頭檔
//

#pragma once

#include "DDE\DDEOperation.h"

#include "CustomControl\DataGrid\GridCtrl.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <array>
#include <Mmsystem.h>

#include "afxwin.h"
#include "Core\StringUtil.h"
#include "afxcmn.h"
#include "DDE\DDEItemsHelper.h"

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
	
	//variables

	CString m_strServer;
	CString m_strTopic;
	CString m_strItem;
	CString m_strConvId;
	CString m_strItemName;

	int m_nInterval;

	UINT m_unResolution;
	MMRESULT m_dwTimerId;

	std::array<ULONG,STATISTIC_ITEM_STRINGID_COUNT> m_ulStatAry;

	FILETIME m_tLastUpdateTime;
	ULONGLONG	m_ullLastUpdate;
	ULONGLONG	 m_ullTimeLBound;
	ULONGLONG	 m_ullTimeUBound;

	//controls 
	CComboBox m_ctrolItem;
	CComboBox m_ctrolItemName;

	CGridCtrl m_ctrlGridDDEFunc;
	CGridCtrl m_ctrlGridOutput;

	CButton m_btnAdd;
	CButton m_btnRmv;
	CButton m_btnAddAll;
	CButton m_btnAdvise;

	CSliderCtrl m_ctrlSliderInterval;

	//models
	CStringMap m_mapCurItem;
	std::vector<CStringMap> m_lstOutput;

	//handle
	DWORD m_dwTimerStatistic;
	CEvent m_eventStatistic;
	
private:
	static void CALLBACK CDDEMonitorDlg::TimerCounter(UINT uTimerID, 
		UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

	void DisableCtrl( bool bIsStart );

	std::multimap<ULONG, CString> m_mapOpenClose;
	std::multimap<ULONG, CString> m_mapDelayOpen;

	bool m_bBeginFlag;

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonConn();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnDDEFuncGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	
	afx_msg void OnBnClickedButtonAdvise();

	afx_msg LRESULT OnUpdateOutput( WPARAM wParam,LPARAM lParam);

	afx_msg void OnNMCustomdrawSliderInterval(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditInterval();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonAddallname();
};
