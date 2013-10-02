#include "stdafx.h"
#include "DDEOperation.h"
#include "DDEException.h"
#include "DDEKernel.h"

std::map<CString,DWORD> InstanceMap;

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
		switch (uType)
		{
		case XTYP_ADVDATA:
			{
				char lpszData[100];
				memset(lpszData,0x0,100);
				DWORD dwLen = DdeGetData(hdata,(LPBYTE)lpszData,99,0);
				CA2W wstrData(lpszData);

				TCHAR* pData = new TCHAR[dwLen+2];
				memset(pData,_T('\0'),sizeof(TCHAR)* (dwLen+2));
				if (!pData) break;
				
				_tcscpy_s(pData,dwLen+1,wstrData);

				TypDDEItem* pItem = new TypDDEItem;
				if (!pItem)
				{
					delete[] pData;
					break;
				}
				DWORD& dwInst = InstanceMap[MMSDDSINST];

				dwLen = DdeQueryString(dwInst,hsz1,0,0,CP_WINUNICODE);
				DdeQueryString(dwInst,hsz1,pItem->strTopic.GetBuffer(dwLen+2),dwLen+1,CP_WINUNICODE);
				pItem->strTopic.ReleaseBuffer();

				dwLen = DdeQueryString(dwInst,hsz2,0,0,CP_WINUNICODE);
				DdeQueryString(dwInst,hsz2,pItem->strItem.GetBuffer(dwLen+2),dwLen+1,CP_WINUNICODE);
				pItem->strItem.ReleaseBuffer();

				CWnd* pWnd = AfxGetMainWnd();
				::PostMessage(pWnd->m_hWnd,WM_UPDATE_DATA,(WPARAM)pItem,(LPARAM)pData);
				TRACE(L"DDE OP -> Topic[%s], Item[%s]:",pItem->strTopic,pItem->strItem);
				TRACE(L"Data[%s]\n",lpszData);
				break;
			}
		case XTYP_ADVREQ:
			TRACE(_T("XTYP_ADVREQ\n"));
			break;

		default:
			break;
		}
		return 0;
	}

	void CDDEOperation::InitInstance( const CString& strInstName, DWORD dwCmd /*= APPCMD_CLIENTONLY*/ )
	{
		if (InstanceMap.find(strInstName)==InstanceMap.end())
		{
			InstanceMap[strInstName] = 0;
			UINT unResult = DdeInitialize(&InstanceMap[strInstName],CDDEOperation::DdeCallback,dwCmd,NULL);
			if (unResult!=DMLERR_NO_ERROR)
				throw CDDEException(CDDEException::E_INIT_FAILED,unResult);
		}
	}

	void CDDEOperation::UninitAll()
	{
		BOOL bResult = TRUE;

		for (auto& hInstPair : InstanceMap)
		{
			for (auto& hConvPair : m_mapConversation)
			{
				TRACE(_T("Disconnecting Server [%s]...\n"),hConvPair.first);
				Disconnect(hInstPair.second,hConvPair.second);
			}

			for (auto& hServerPair : m_mapServer[hInstPair.second])
			{
				for (auto& hTopicPair : m_mapTopic[hServerPair.second])
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
		const DWORD& hInst = InstanceMap[strInst];

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

	CString CDDEOperation::DoTransaction( const CString& strConvId, const CString& strItem, UINT unFmt, UINT unType, DWORD dwTimeout/*=DDE_DEFAULT_TIMEOUT*/ )
	{
		const HCONV hConv = m_mapConversation[strConvId];
		CString strInst, strSvr, strTopic;
		ResovleConvId(strConvId,strInst,strSvr,strTopic);

		HSZ hSzItem = CreateStrHandle(InstanceMap[strInst],strItem);

		HDDEDATA hData = DdeClientTransaction(NULL,NULL,hConv,hSzItem,unFmt,unType,dwTimeout,NULL);

		FreeStrHandle(InstanceMap[strInst],hSzItem);

		if (!hData)
			throw CDDEException(CDDEException::E_DATA_FAILED,CDDEException::GetLastError(InstanceMap[strInst]));

		CString strReturn(L"");
		if (unType&XCLASS_DATA)
		{
			char lpszData[100];
			memset(lpszData,0x0,100);
			DWORD dwResult = DdeGetData(hData,(LPBYTE)lpszData,99,0);
			TRACE("GetData in Transaction: %s\n",lpszData);
			FreeDataHandle(InstanceMap[strInst],hData);
			strReturn = CA2W(lpszData);
		}
		TRACE(_T("[%s][%s][%d]:[%s]\n"),strConvId,strItem,unType,strReturn);
		return strReturn;
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

	CString CDDEOperation::Request( const CString& strConvId, const CString& strItem, UINT unFmt, DWORD dwTimeout /*= DDE_DEFAULT_TIMEOUT*/ )
	{
		return DoTransaction(strConvId, strItem, unFmt, XTYP_REQUEST ,dwTimeout);
	}


}
