#pragma once

#define MMSDDSINST _T("MMS")

#define WM_UPDATE_DATA WM_USER+1

extern std::map<CString,DWORD>	InstanceMap;

struct TypDDEItem {
	CString strSvr;
	CString strTopic;
	CString strItem;
};