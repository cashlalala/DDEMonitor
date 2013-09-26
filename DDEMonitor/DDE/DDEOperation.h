#pragma once

#include <ddeml.h>
#include <map>

namespace DDE
{
	class CDDEOperation
	{
	public:
		CDDEOperation(void);
		virtual ~CDDEOperation(void);

		void InitInstance(const CString& strInstName, DWORD dwCmd = APPCMD_CLIENTONLY);

		CString Connect(const CString& strInst, const CString& strSvr, const CString& strTopic, PCONVCONTEXT pConvContext);

		void DoTransaction(const CString& strConvId, const CString& strItem, UINT unFmt, UINT unType, DWORD dwTimeout);

		void UninitAll();

		CString CreateConvId(const CString& strInst, const CString& strSvr, const CString& strTopic);

	private:

		HSZ CreateStrHandle(DWORD dwInst, const CString& strTarget);

		void FreeStrHandle(DWORD dwInst, HSZ hStr);

		void FreeDataHandle(DWORD dwInst, HDDEDATA hData);

		void Disconnect(DWORD dwInst, HCONV hConv);


		//std::map<CString,DWORD>						m_mapInst;
		std::map<DWORD,std::map<CString,HSZ> >		m_mapServer;
		std::map<HSZ,std::map<CString,HSZ> >		m_mapTopic;

		std::map<CString,HCONV>                     m_mapConversation;
		
		void ResovleConvId(const CString& strConvId, CString& strInst, CString& strSvr, CString& strTopic);

		static HDDEDATA CALLBACK DdeCallback(
			_In_  UINT uType,
			_In_  UINT uFmt,
			_In_  HCONV hconv,
			_In_  HSZ hsz1,
			_In_  HSZ hsz2,
			_In_  HDDEDATA hdata,
			_In_  ULONG_PTR dwData1,
			_In_  ULONG_PTR dwData2
			);
	};
}



