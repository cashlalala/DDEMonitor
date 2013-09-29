#pragma once

#include <array>

#ifdef UNICODE
#define CP(TEXT) _T(TEXT)
#else
#define CP(TEXT) TEXT
#endif // DEBUG

#define DDEID_RISEFALL CP(".184") //���^
#define DDEID_STRIKEPC CP(".125") //�����
#define DDEID_VOLUME CP(".404") //����q
#define DDEID_BUYPC CP(".101") //�R��
#define DDEID_SELLPC CP(".102") //���
#define DDEID_TIME CP(".143") //�ɶ�
#define DDEID_CODE CP(".1018") //�N�X
#define DDEID_NAME CP(".2020") //�W��

#define DDENAME_RISEFALL CP("���^") //.184
#define DDENAME_STRIKEPC CP("�����") //.125
#define DDENAME_VOLUME CP("����q") //.404
#define DDENAME_BUYPC CP("�R��") //.101
#define DDENAME_SELLPC CP("���") //.102
#define DDENAME_TIME CP("�ɶ�") //.143
#define DDENAME_CODE CP("�N�X") //.1018
#define DDENAME_NAME CP("�W��") //.2020

#define JY_AMP_ID CP("M1JY&")
#define JY_AMP(DDEID) CP("M1JY&")DDEID

#define JY_AMP_RISEFALL JY_AMP(DDEID_RISEFALL) //���^
#define JY_AMP_STRIKEPC JY_AMP(DDEID_STRIKEPC) //�����
#define JY_AMP_VOLUME JY_AMP(DDEID_VOLUME) //����q
#define JY_AMP_BUYPC JY_AMP(DDEID_BUYPC) //�R��
#define JY_AMP_SELLPC JY_AMP(DDEID_SELLPC) //���
#define JY_AMP_TIME JY_AMP(DDEID_TIME) //�ɶ�
#define JY_AMP_CODE JY_AMP(DDEID_CODE) //�N�X
#define JY_AMP_NAME JY_AMP(DDEID_NAME) //�W��

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

