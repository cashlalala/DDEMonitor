#pragma once

#include <windef.h>

typedef enum {
	HIGH = 0,
	LOW,
	HORIZ
} EnFType;

typedef enum {
	DOWN = -1,
	HORIZON = 0,
	UP = 1
} EnSide;

typedef struct _AnalyzePnt{
	_AnalyzePnt() : nRowOffset(0), nCol(0), ulTime(0), ulPrice(0){};
	ULONG ulTime;
	ULONG ulPrice;
	int nRowOffset;
	int nCol;
} AnalyzePnt;

//a pair of begin time and end time as an ID of a formula
typedef std::pair<AnalyzePnt, AnalyzePnt> FormulaId;

class Formula {
public:
	Formula() : m_dSlope(0), m_dConst(0) {};

	Formula(const AnalyzePnt& p1, const AnalyzePnt& p2) :
		m_Id(p1,p2),
		m_dSlope(double(p2.ulPrice - p1.ulPrice) / (p2.ulTime - p1.ulTime)),
		m_dConst(-double(m_dSlope*p1.ulTime + p1.ulPrice))
	{};
	~Formula(){};

	EnSide CheckSide(const AnalyzePnt& anaPnt) {
		double dAns = m_dSlope * anaPnt.ulTime + anaPnt.ulPrice;
		return (dAns > 0) ? EnSide::UP :
			(dAns == 0) ? EnSide::HORIZON :
			EnSide::DOWN;
	};

	EnFType m_enFType;
	FormulaId m_Id;
	COLORREF crBk;

	void Calculate() {
		m_dSlope = double(m_Id.second.ulPrice - m_Id.first.ulPrice) / (m_Id.second.ulTime - m_Id.first.ulTime);
		m_dConst = -double(m_dSlope*m_Id.first.ulTime + m_Id.first.ulPrice);
	};

private:

	double m_dSlope;
	double m_dConst;
};

