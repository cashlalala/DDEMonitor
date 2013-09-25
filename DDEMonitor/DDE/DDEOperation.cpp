#include "stdafx.h"
#include "DDEOperation.h"
#include "DDEException.h"

namespace DDE
{

	CDDEOperation::CDDEOperation(void)
	{
	}


	CDDEOperation::~CDDEOperation(void)
	{

	}

	HDDEDATA CALLBACK CDDEOperation::DdeCallback( 
		_In_ UINT uType, 
		_In_ UINT uFmt, 
		_In_ HCONV hconv, 
		_In_ HSZ hsz1, 
		_In_ HSZ hsz2, 
		_In_ HDDEDATA hdata, 
		_In_ ULONG_PTR dwData1, 
		_In_ ULONG_PTR dwData2 )
	{

		char lpszData[100];
		memset(lpszData,0x0,100);

		switch (uType)
		{
		case XTYP_ADVDATA:
			{
				DWORD dwResult = DdeGetData(hdata,(LPBYTE)lpszData,99,0);
				//CString strLog = CA2W(lpszData);
				TRACE("XTYP_ADVDATA %s\n",lpszData);
				break;
			}
		default:
			break;
		}
		return 0;
	}

	void CDDEOperation::InitInstance( const CString& strInstName, DWORD dwCmd /*= APPCMD_CLIENTONLY*/ )
	{
		if (m_mapInst.find(strInstName)==m_mapInst.end())
		{
			m_mapInst[strInstName] = 0;
			UINT unResult = DdeInitialize(&m_mapInst[strInstName],CDDEOperation::DdeCallback,dwCmd,NULL);
			if (unResult!=DMLERR_NO_ERROR)
				throw CDDEException(CDDEException::E_INIT_FAILED,unResult);
		}
	}

	void CDDEOperation::UninitAll()
	{
		BOOL bResult = TRUE;

		for (auto hInstPair : m_mapInst)
		{
			for (auto hConvPair : m_mapConversation)
			{
				TRACE(_T("Disconnecting Server [%s]...\n"),hConvPair.first);
				Disconnect(hInstPair.second,hConvPair.second);
			}

			for (auto hServerPair : m_mapServer[hInstPair.second])
			{
				for (auto hTopicPair : m_mapTopic[hServerPair.second])
				{
					TRACE(_T("Freeing Topic String [%s]...\n"),hTopicPair.first);
					FreeStrHandle(hInstPair.second,hTopicPair.second);
				}
				TRACE(_T("Freeing Server String [%s]...\n"),hServerPair.first);
				FreeStrHandle(hInstPair.second,hServerPair.second);
			}
			TRACE(_T("Uninit DDE instance [%s]...\n"),hInstPair.first);
			bResult &= DdeUninitialize(hInstPair.second);
			if (!bResult)
				throw CDDEException(CDDEException::E_UNINIT_FAILED,CDDEException::GetLastError(hInstPair.second));
		}
	}

	void CDDEOperation::FreeStrHandle( DWORD dwInst,HSZ hStr )
	{
		if (!DdeFreeStringHandle(dwInst,hStr))
			throw CDDEException(CDDEException::E_STRING_FAILED,CDDEException::GetLastError(dwInst));
	}

	void CDDEOperation::FreeDataHandle( DWORD dwInst,HDDEDATA hData )
	{
		if (!DdeFreeDataHandle(hData))
			throw CDDEException(CDDEException::E_DATA_FAILED,CDDEException::GetLastError(dwInst));
	}

	CString CDDEOperation::Connect( const CString& strInst, const CString& strSvr, const CString& strTopic, PCONVCONTEXT pConvContext )
	{
		const DWORD& hInst = m_mapInst[strInst];

		if (m_mapServer[hInst].find(strSvr)==m_mapServer[hInst].end())
			m_mapServer[hInst][strSvr] = CreateStrHandle(hInst,strSvr);
		const HSZ& hSvr = m_mapServer[hInst][strSvr];

		if (m_mapTopic[hSvr].find(strTopic)==m_mapTopic[hSvr].end())
			m_mapTopic[hSvr][strTopic] = CreateStrHandle(hInst,strTopic);
		const HSZ& hTopic = m_mapTopic[hSvr][strTopic];

		HCONV hConv = DdeConnect(hInst,hSvr,hTopic,(PCONVCONTEXT)NULL);
		if (!hConv)
			throw CDDEException(CDDEException::E_CONN_FAILED,CDDEException::GetLastError(hInst));

		CString strId = CreateConvId(strInst,strSvr,strTopic);
		m_mapConversation[strId] = hConv;
		return strId;
	}

	CString CDDEOperation::CreateConvId( const CString& strInst, const CString& strSvr, const CString& strTopic )
	{
		CString strId;
		strId.Format(_T("%s:%s:%s"),strInst,strSvr,strTopic);
		return strId;
	}

	void CDDEOperation::Disconnect( DWORD dwInst, HCONV hConv )
	{
		if (!DdeDisconnect(hConv))
			throw CDDEException(CDDEException::E_DISCONN_FAILED,CDDEException::GetLastError(dwInst));
	}

	void CDDEOperation::DoTransaction( const CString& strConvId, const CString& strItem, UINT unFmt, UINT unType, DWORD dwTimeout )
	{
		const HCONV hConv = m_mapConversation[strConvId];
		CString strInst, strSvr, strTopic;
		ResovleConvId(strConvId,strInst,strSvr,strTopic);

		HSZ hSzItem = CreateStrHandle(m_mapInst[strInst],strItem);

		HDDEDATA hData = DdeClientTransaction(NULL,NULL,hConv,hSzItem,unFmt,unType,dwTimeout,NULL);

		FreeStrHandle(m_mapInst[strInst],hSzItem);

		if (!hData)
			throw CDDEException(CDDEException::E_DATA_FAILED,CDDEException::GetLastError(m_mapInst[strInst]));

		if (unType&XCLASS_DATA)
		{
			char lpszData[100];
			memset(lpszData,0x0,100);
			DWORD dwResult = DdeGetData(hData,(LPBYTE)lpszData,99,0);
			TRACE("GetData in Transaction: %s\n",lpszData);
			FreeDataHandle(m_mapInst[strInst],hData);
		}
		
	}

	HSZ CDDEOperation::CreateStrHandle( DWORD dwInst, const CString& strTarget )
	{
		HSZ hSz = DdeCreateStringHandle(dwInst, strTarget, CP_WINUNICODE);
		if (!hSz)
			throw CDDEException(CDDEException::E_STRING_FAILED,CDDEException::GetLastError(dwInst));
		return hSz;
	}

	void CDDEOperation::ResovleConvId( const CString& strConvId, CString& strInst, CString& strSvr, CString& strTopic )
	{
		int nIdx = 0;
		strInst = strConvId.Tokenize(_T(":"),nIdx);
		strSvr = strConvId.Tokenize(_T(":"),nIdx);
		strTopic = strConvId.Tokenize(_T(":"),nIdx);

	}


}
