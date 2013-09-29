
// DDEMonitorDlg.h : ���Y��
//

#pragma once

#include "DDE\DDEOperation.h"

#include "CustomControl\DataGrid\GridCtrl.h"

// CDDEMonitorDlg ��ܤ��
class CDDEMonitorDlg : public CDialogEx
{
// �غc
public:
	CDDEMonitorDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_DDEMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	DDE::CDDEOperation m_ddeOper; 

	// ���ͪ��T�������禡
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
