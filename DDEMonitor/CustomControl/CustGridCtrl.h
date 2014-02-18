#pragma once
#include "DataGrid\GridCtrl.h"
#include "..\Core\Formula.h"
#include <map>

class CCustGridCtrl :
	public CGridCtrl
{

public:
	static const UINT WM_RESET_SELECTED_CNT;

	Formula m_curFormual;

	DECLARE_DYNCREATE(CCustGridCtrl)
	friend class CGridCell;
	friend class CGridCellBase;
	friend class CCustChkCell;

	DECLARE_MESSAGE_MAP()

public:
	CCustGridCtrl();
	virtual ~CCustGridCtrl();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HRESULT OnResetSelectedCnt(WPARAM wparm, LPARAM lparm);
};

