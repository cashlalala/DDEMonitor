#include "stdafx.h"
#include "DataGrid\GridCell.h"
#include "DataGrid\GridCtrl.h"
#include "CustChkCell.h"


IMPLEMENT_DYNCREATE(CCustChkCell, CGridCell)

const SHORT CCustChkCell::SC_MAX_ALLOWEDCNT = 2;

SHORT CCustChkCell::m_nTotalAllowedCnt = 0;

CCustChkCell::CCustChkCell()
{
}


CCustChkCell::~CCustChkCell()
{
}

void CCustChkCell::OnClick(CPoint PointCellRelative)
{
	if (m_nTotalAllowedCnt == SC_MAX_ALLOWEDCNT && !GetCheck()) {
		MessageBox(AfxGetMainWnd()->GetSafeHwnd(), _T("You can check only two boxes at one time!"), _T("Info"), MB_OK);
		return;
	}

	m_nTotalAllowedCnt = m_nTotalAllowedCnt + (GetCheck() ? -1 : 1);

	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	//PointCellRelative += m_Rect.TopLeft();

	// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
	// uncheck if the user clicked in the box
	//if (GetCheckPlacement().PtInRect(PointCellRelative))
	//{
		m_bChecked = !m_bChecked;
		GetGrid()->InvalidateRect(m_Rect);
	//}
}

