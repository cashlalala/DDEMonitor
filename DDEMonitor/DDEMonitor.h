
// DDEMonitor.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CDDEMonitorApp:
// �аѾ\��@�����O�� DDEMonitor.cpp
//

class CDDEMonitorApp : public CWinApp
{
public:
	CDDEMonitorApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CDDEMonitorApp theApp;