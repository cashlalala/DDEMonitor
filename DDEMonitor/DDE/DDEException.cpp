/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEEXCEPTION.CPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	CDDEException class definition.
**
*******************************************************************************
*/
#include "stdafx.h"
#include "DDEException.h"
#include "..\Core\StringUtil.h"

#include <ddeml.h>

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

namespace DDE
{
	CDDEException::CDDEException(int eErrCode, UINT nDDECode)
		: m_nDDECode(nDDECode)
	{
		// Get error code as #defined symbol.
		CString strErrDef = GetErrorString(m_nDDECode);

		// Convert error to string.
		switch(eErrCode)
		{
		case E_INIT_FAILED:
			m_details = Core::CStringUtil::Format(_T("Failed to initialise DDE: %s"), strErrDef);
			break;

		case E_CONN_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to connect to DDE server: %s"), strErrDef);
			break;

		case E_STRING_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to create DDE string: %s"), strErrDef);
			break;

		case E_REQUEST_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to request DDE item: %s"), strErrDef);
			break;

		case E_LINK_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to create DDE advise loop: %s"), strErrDef);
			break;

		case E_ALLOC_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to allocate DDE data handle: %s"), strErrDef);
			break;

		case E_QUERY_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to query DDE servers: %s"), strErrDef);
			break;

		case E_EXECUTE_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to execute a DDE command: %s"), strErrDef);
			break;

		case E_POKE_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to poke an item: %s"), strErrDef);
			break;

		case E_UNINIT_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to uninitialize DDE: %s"), strErrDef);
			break;

		case E_DATA_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to create DDE data: %s"), strErrDef);
			break;

		case E_DISCONN_FAILED:
			m_details = Core::CStringUtil::Format(TEXT("Failed to disconnect DDE server: %s"), strErrDef);
			break;

			// Shouldn't happen!
		default:
			ASSERT(FALSE);
			break;
		}
	}

	/******************************************************************************
	** Method:		Destructor.
	**
	** Description:	.
	**
	** Parameters:	None.
	**
	** Returns:		Nothing.
	**
	*******************************************************************************
	*/

	CDDEException::~CDDEException() throw()
	{
	}

	CString CDDEException::GetErrorString( UINT nError )
	{
		// Known error codes.
		switch (nError)
		{
		case DMLERR_NO_ERROR:				return TEXT("DMLERR_NO_ERROR");
		case DMLERR_ADVACKTIMEOUT:			return TEXT("DMLERR_ADVACKTIMEOUT");
		case DMLERR_BUSY:					return TEXT("DMLERR_BUSY");
		case DMLERR_DATAACKTIMEOUT:			return TEXT("DMLERR_DATAACKTIMEOUT");
		case DMLERR_DLL_NOT_INITIALIZED:	return TEXT("DMLERR_DLL_NOT_INITIALIZED");
		case DMLERR_DLL_USAGE:				return TEXT("DMLERR_DLL_USAGE");
		case DMLERR_EXECACKTIMEOUT:			return TEXT("DMLERR_EXECACKTIMEOUT");
		case DMLERR_INVALIDPARAMETER:		return TEXT("DMLERR_INVALIDPARAMETER");
		case DMLERR_LOW_MEMORY:				return TEXT("DMLERR_LOW_MEMORY");
		case DMLERR_MEMORY_ERROR:			return TEXT("DMLERR_MEMORY_ERROR");
		case DMLERR_NOTPROCESSED:			return TEXT("DMLERR_NOTPROCESSED");
		case DMLERR_NO_CONV_ESTABLISHED:	return TEXT("DMLERR_NO_CONV_ESTABLISHED");
		case DMLERR_POKEACKTIMEOUT:			return TEXT("DMLERR_POKEACKTIMEOUT");
		case DMLERR_POSTMSG_FAILED:			return TEXT("DMLERR_POSTMSG_FAILED");
		case DMLERR_REENTRANCY:				return TEXT("DMLERR_REENTRANCY");
		case DMLERR_SERVER_DIED:			return TEXT("DMLERR_SERVER_DIED");
		case DMLERR_SYS_ERROR:				return TEXT("DMLERR_SYS_ERROR");
		case DMLERR_UNADVACKTIMEOUT:		return TEXT("DMLERR_UNADVACKTIMEOUT");
		case DMLERR_UNFOUND_QUEUE_ID:		return TEXT("DMLERR_UNFOUND_QUEUE_ID");
		default:							ASSERT(FALSE);
		}

		// Unknown error code.
		CString strErr;
		strErr.Format(TEXT("0x%08X"), nError);
		return strErr;
	}

	DWORD CDDEException::GetLastError(DWORD dwInst)
	{
		return DdeGetLastError(dwInst);
	}

}

