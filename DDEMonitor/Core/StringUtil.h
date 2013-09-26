#pragma once

namespace Core
{

	struct CStringHash {
		size_t operator()(const CString& k) const 
		{
			unsigned long hash = 5381;
			int c =0;

			WCHAR* str = (WCHAR*) k.GetString();

			while (c = *str++)
			{
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
			}	

			return hash;
		}
	};

	struct CStringEqul {
		bool operator()(const CString& lhs, const CString& rhs) const 
		{
			return lhs == rhs;
		}
	};

	class CStringUtil
	{
	public:
		static CString Format(const TCHAR* lptszFmt, ...);

	private:
		CStringUtil(void);
		~CStringUtil(void);
	};
}


