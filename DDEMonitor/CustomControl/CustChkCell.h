#pragma once
#include "DataGrid\GridCellCheck.h"

class CCustChkCell :
	public CGridCellCheck
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CCustChkCell)

private:
	static SHORT m_nTotalAllowedCnt;
	static const SHORT SC_MAX_ALLOWEDCNT;

public:
	CCustChkCell();
	virtual ~CCustChkCell();

	virtual void OnClick(CPoint PointCellRelative);

};

