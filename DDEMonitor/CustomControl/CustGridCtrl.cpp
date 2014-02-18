#include "stdafx.h"
#include "CustGridCtrl.h"
#include "CustChkCell.h"


IMPLEMENT_DYNCREATE(CCustGridCtrl, CGridCtrl)

BEGIN_MESSAGE_MAP(CCustGridCtrl, CGridCtrl)
	ON_MESSAGE(WM_RESET_SELECTED_CNT, OnResetSelectedCnt)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

const UINT CCustGridCtrl::WM_RESET_SELECTED_CNT = WM_USER + 100;

CCustGridCtrl::CCustGridCtrl()
{
}


CCustGridCtrl::~CCustGridCtrl()
{
}


void CCustGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CGridCtrl::OnLButtonUp(nFlags, point);

	const int& nCurRow = GetFocusCell().row;
	const int& nCurCol = GetFocusCell().col;
	const SHORT& nChkedCnt = CCustChkCell::GetCheckedCnt();

	CCustChkCell* pChkCell = dynamic_cast<CCustChkCell*>(GetCell(nCurRow, nCurCol));
	if (pChkCell) {
		//BOOL bIsChecked = pChkCell->GetCheck();
		//CGridCell* pCell = (CGridCell*)GetCell(nCurRow, 0); //time;
		//ULONG ulTime = _tcstoul(pCell->GetText(), NULL, 10);

		//pCell = (CGridCell*)GetCell(nCurRow, 2); //hi price;
		//ULONG ulHiPc = _tcstoul(pCell->GetText(), NULL, 10);

		//pCell = (CGridCell*)GetCell(nCurRow, 3); //lo price;
		//ULONG ulLoPc = _tcstoul(pCell->GetText(), NULL, 10);


		TRACE(_T("Dump current ID [%lu:%lu][%lu:%lu]\n"), m_curFormual.m_Id.first.ulTime, m_curFormual.m_Id.first.ulPrice
			, m_curFormual.m_Id.second.ulTime, m_curFormual.m_Id.second.ulPrice);
	}
}

HRESULT CCustGridCtrl::OnResetSelectedCnt(WPARAM wparm, LPARAM lparm)
{
	int nRowCnt = GetRowCount();
	CCustChkCell* pChkCell = (CCustChkCell*)GetCell(nRowCnt - m_curFormual.m_Id.first.nRowOffset - 1, m_curFormual.m_Id.first.nCol);
	pChkCell->SetCheck(FALSE);
	pChkCell = (CCustChkCell*)GetCell(nRowCnt - m_curFormual.m_Id.second.nRowOffset - 1, m_curFormual.m_Id.second.nCol);
	pChkCell->SetCheck(FALSE);
	Invalidate();

	CCustChkCell::ReSetCHeckedCnt();
	m_curFormual = Formula();
	return S_OK;
}
