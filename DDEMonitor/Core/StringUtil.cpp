#include "stdafx.h"
#include "StringUtil.h"

#include <string>

namespace Core
{
	CStringUtil::CStringUtil(void)
	{
	}


	CStringUtil::~CStringUtil(void)
	{
	}

	CString CStringUtil::Format( const TCHAR* lptszFmt, ... )
	{
		int size = 100;
		std::wstring str;
		va_list ap;
		while (1) {
			str.resize(size);
			va_start(ap, lptszFmt);
			int n = _vsnwprintf((TCHAR*)str.c_str(), size, lptszFmt, ap);
			va_end(ap);
			if (n > -1 && n < size) {
				str.resize(n);
				return CString(str.c_str());
			}
			if (n > -1)
				size = n + 1;
			else
				size *= 2;
		}
		return CString(str.c_str());
	}

}

