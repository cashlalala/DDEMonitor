#pragma once
#include "DataGrid\GridCellCheck.h"
#include "..\Core\Formula.h"

class CCustChkCell :
	public CGridCellCheck
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CCustChkCell)

private:
	static SHORT m_nTotalAllowedCnt;
		
public:
	static const SHORT SC_MAX_ALLOWEDCNT;
	static const SHORT GetCheckedCnt();
	static const void ReSetCHeckedCnt();

public:
	CCustChkCell();
	virtual ~CCustChkCell();

	virtual void OnClick(CPoint PointCellRelative);
	
private:
	void SetPnt(AnalyzePnt& aPnt, ULONG ultime = 0, ULONG ulPrice = 0, int nRowOffset = 0, int nCol = 0);
};

