#include "..\stdafx.h"
#include "CAIDCommPoint.h"

CAIDCommPoint::CAIDCommPoint()
{
	m_nCp           = 0;
	m_nNCps         = 0;
	m_nCpIndex      = 0;
	m_nOccAGV       = 0;
	m_nOccStopAGV   = 0;
	m_nAGVTypes     = 0;
	m_nAGVTypesDest = 0;
	m_nDistIndex    = 0;
}

CAIDCommPoint::CAIDCommPoint(int nCp, int nCpIndex)
{
	m_nCp           = nCp;
	m_nNCps         = 0;
	m_nCpIndex      = nCpIndex;
	m_nOccAGV       = 0;
	m_nOccStopAGV   = 0;
	m_nAGVTypes     = 0;
	m_nAGVTypesDest = 0;
	m_nDistIndex    = 0;
}

CAIDCommPoint::~CAIDCommPoint()
{
}
