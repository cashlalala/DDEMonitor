#pragma once

#include <array>

#ifdef UNICODE
#define CP(TEXT) _T(TEXT)
#else
#define CP(TEXT) TEXT
#endif // DEBUG

#define DDEID_RISEFALL CP(".184") //漲跌
#define DDEID_STRIKEPC CP(".125") //成交價
#define DDEID_VOLUME CP(".404") //成交量
#define DDEID_BUYPC CP(".101") //買價
#define DDEID_SELLPC CP(".102") //賣價
#define DDEID_TIME CP(".143") //時間
#define DDEID_CODE CP(".1018") //代碼
#define DDEID_NAME CP(".2020") //名稱

#define DDENAME_RISEFALL CP("漲跌") //.184
#define DDENAME_STRIKEPC CP("成交價") //.125
#define DDENAME_VOLUME CP("成交量") //.404
#define DDENAME_BUYPC CP("買價") //.101
#define DDENAME_SELLPC CP("賣價") //.102
#define DDENAME_TIME CP("時間") //.143
#define DDENAME_CODE CP("代碼") //.1018
#define DDENAME_NAME CP("名稱") //.2020

#define JY_AMP_ID CP("M1JY&")
#define JY_AMP(DDEID) CP("M1JY&")DDEID

#define JY_AMP_RISEFALL JY_AMP(DDEID_RISEFALL) //漲跌
#define JY_AMP_STRIKEPC JY_AMP(DDEID_STRIKEPC) //成交價
#define JY_AMP_VOLUME JY_AMP(DDEID_VOLUME) //成交量
#define JY_AMP_BUYPC JY_AMP(DDEID_BUYPC) //買價
#define JY_AMP_SELLPC JY_AMP(DDEID_SELLPC) //賣價
#define JY_AMP_TIME JY_AMP(DDEID_TIME) //時間
#define JY_AMP_CODE JY_AMP(DDEID_CODE) //代碼
#define JY_AMP_NAME JY_AMP(DDEID_NAME) //名稱

#define DDE_ITEM_STRINGID_COUNT 8

namespace DDE
{
	extern const std::array<int,DDE_ITEM_STRINGID_COUNT> DDE_ITEM_STRINGID_TABLE;

	class CDDEItemsHelper
	{
	public:
		static CString GetNameFromID(const CString& strId);
		static CString GetIDFromName(const CString& strName);
	};
}

