#pragma once

namespace Core
{

	class CStringUtil
	{
	public:
		static CString Format(const TCHAR* lptszFmt, ...);

	private:
		CStringUtil(void);
		~CStringUtil(void);
	};
}


