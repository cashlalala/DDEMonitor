#include "stdafx.h"
#include "DataGrid\GridCell.h"
#include "DataGrid\GridCtrl.h"
#include "CustChkCell.h"
#include "CustGridCtrl.h"



IMPLEMENT_DYNCREATE(CCustChkCell, CGridCell)

const SHORT CCustChkCell::SC_MAX_ALLOWEDCNT = 2;

SHORT CCustChkCell::m_nTotalAllowedCnt = 0;

//BEGIN_MESSAGE_MAP(CCustChkCell, CGridCell)
//END_MESSAGE_MAP()

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

	CCustGridCtrl* pGrid = dynamic_cast<CCustGridCtrl*>(GetGrid());
	if (nullptr != pGrid) {
		CCellID cellId = pGrid->GetFocusCell();
		ULONG ulOffset = pGrid->GetRowCount() - 1 - cellId.row;
		ULONG ulTime = _tcstoul(pGrid->GetCell(cellId.row, 0)->GetText(), 0, 10);
		ULONG ulHiPc = _tcstoul(pGrid->GetCell(cellId.row, 2)->GetText(), 0, 10);
		ULONG ulLoPc = _tcstoul(pGrid->GetCell(cellId.row, 3)->GetText(), 0, 10);
		Formula& formaula = pGrid->m_curFormual;

		//some kind of state machine
		if (GetCheck()) {
			if (m_nTotalAllowedCnt == 2) { // eliminate 2 to 1 
				if (formaula.m_Id.first.ulTime == ulTime) {
					SetPnt(formaula.m_Id.first);
				}
				else /*if (formaula.m_Id.second.ulPrice == ulTime)*/ {
					SetPnt(formaula.m_Id.second);
				}
			}
			else if (m_nTotalAllowedCnt == 1) { // eliminate 1 to 0
				if (formaula.m_Id.first.ulTime == ulTime) {
					SetPnt(formaula.m_Id.first);
				}
				else /*if (formaula.m_Id.second.ulPrice == ulTime)*/ {
					SetPnt(formaula.m_Id.second);
				}
			}
			else {
				TRACE(_T("No matching point\n"));
			}
			m_nTotalAllowedCnt -= 1;
		}
		else {
			if (m_nTotalAllowedCnt == 1) {// add 1 to 2
				if (formaula.m_Id.first.ulTime != 0) {
					SetPnt(formaula.m_Id.second, ulTime, ulHiPc, ulOffset, cellId.col);
				}
				else /*if (formaula.m_Id.second.ulPrice == ulTime)*/ {
					SetPnt(formaula.m_Id.first, ulTime, ulHiPc, ulOffset, cellId.col);
				}
			}
			else if (m_nTotalAllowedCnt == 0) { // add 0 to 1
				SetPnt(formaula.m_Id.first, ulTime, ulHiPc, ulOffset, cellId.col);
			}
			else {
				TRACE(_T("No matching point\n"));
			}
			m_nTotalAllowedCnt += 1;
		}
	}

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

const SHORT CCustChkCell::GetCheckedCnt()
{
	return m_nTotalAllowedCnt;
}

void CCustChkCell::SetPnt(AnalyzePnt& aPnt, ULONG ultime, ULONG ulPrice, int nRowOffset, int nCol) {
	aPnt.ulTime = ultime;
	aPnt.ulPrice = ulPrice;
	aPnt.nRowOffset = nRowOffset;
	aPnt.nCol = nCol;
}

const void CCustChkCell::ReSetCHeckedCnt()
{
	m_nTotalAllowedCnt = 0;
}
