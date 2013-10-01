#include "stdafx.h"
#include "DDEItemsHelper.h"
#include "..\resource.h"

namespace DDE
{
	const std::array<int,DDE_ITEM_STRINGID_COUNT> DDE_ITEM_STRINGID_TABLE= 
	{
	IDS_TIME,
	IDS_CODE,
	IDS_NAME,
	IDS_RISEFALL,
	IDS_STRIKEPRICE,
	IDS_BUYPRICE,
	IDS_SELLPRICE,
	IDS_VOLUME
	};

	const std::array<int,STATISTIC_ITEM_STRINGID_COUNT> STATISTIC_ITEM_STRINGID_TABLE= 
	{
		IDS_DATETIME,
		IDS_OPEN_PRICE,
		IDS_HIGHEST_PRICE,
		IDS_LOWEST_PRICE,
		IDS_CLOSE_PRICE,
		IDS_VOLUME
	};

	CString CDDEItemsHelper::GetNameFromID( const CString& strId )
	{
		int nRcId = GetRCIDFromID(strId);
		CString strName; 
		strName.LoadString(nRcId);
		return strName;
	}

	CString CDDEItemsHelper::GetIDFromName( const CString& strName )
	{
		CString strId;
		for (const auto& stringId : DDE_ITEM_STRINGID_TABLE)
		{
			strId.LoadString(stringId);
			if (strId==strName)
			{
				switch (stringId)
				{
				case IDS_NAME:
					return DDEID_NAME;
				case IDS_RISEFALL:
					return DDEID_RISEFALL;
				case IDS_SELLPRICE:
					return DDEID_SELLPC;
				case IDS_STRIKEPRICE:
					return DDEID_STRIKEPC;
				case IDS_BUYPRICE:
					return DDEID_BUYPC;
				case IDS_CODE:
					return DDEID_CODE;
				case IDS_TIME:
					return DDEID_TIME;
				case IDS_VOLUME:
					return DDEID_VOLUME;
				default:
					break;
				}
			}
		}
		return CP("");
	}

	int CDDEItemsHelper::GetRCIDFromID( const CString& strId )
	{
		return (strId.Right(4)==DDEID_RISEFALL)? IDS_RISEFALL :
			(strId.Right(4)==DDEID_STRIKEPC)? IDS_STRIKEPRICE :
			(strId.Right(4)==DDEID_VOLUME)? IDS_VOLUME :
			(strId.Right(4)==DDEID_BUYPC)? IDS_BUYPRICE :
			(strId.Right(4)==DDEID_SELLPC)? IDS_SELLPRICE :
			(strId.Right(4)==DDEID_TIME)? IDS_TIME :
			(strId.Right(5)==DDEID_CODE)? IDS_CODE :
			(strId.Right(5)==DDEID_NAME)? IDS_NAME :
			0;
	}

}
