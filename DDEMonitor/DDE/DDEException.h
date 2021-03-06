/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		DDEEXCEPTION.HPP
** COMPONENT:	Network & Comms Library
** DESCRIPTION:	The CDDEException class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef DDEEXCEPTION_HPP
#define DDEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "..\Core\Exception.h"

/******************************************************************************
**
** The exception type thrown by the DDE classes.
**
*******************************************************************************
*/
namespace DDE
{
	class CDDEException : public Core::Exception
	{
	public:
		//
		// Exception codes.
		//
		enum ErrCode
		{
			E_INIT_FAILED		= 10,	// Initialisation failed.
			E_REG_FAILED		= 11,	// Server registration failed.
			E_CONN_FAILED		= 12,	// Connect failed.
			E_STRING_FAILED		= 13,	// Failed to create string handle.
			E_REQUEST_FAILED	= 14,	// Failed to request an item.
			E_LINK_FAILED		= 15,	// Failed to create advise loop.
			E_ALLOC_FAILED		= 16,	// Failed to allocate data handle.
			E_QUERY_FAILED		= 17,	// Failed to query DDE servers.
			E_EXECUTE_FAILED	= 18,	// Failed to execute a command.
			E_POKE_FAILED		= 19,	// Failed to poke a value.
			E_UNINIT_FAILED		= 20,	// Failed to poke a value.
			E_DATA_FAILED		= 21,	// Failed to poke a value.
			E_DISCONN_FAILED    = 22
		};

		//
		// Constructors/Destructor.
		//
		CDDEException(int eErrCode, UINT nDDECode);
		virtual ~CDDEException() throw();

		//
		// Members.
		//
		UINT m_nDDECode;

		static CString GetErrorString(UINT nError);

		static DWORD GetLastError(DWORD dwInst);
	};
}


/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // DDEEXCEPTION_HPP
