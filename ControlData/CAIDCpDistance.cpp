#include "..\stdafx.h"
#include "CAIDCpDistance.h"

CAIDCpDistance::CAIDCpDistance()
{
}

CAIDCpDistance::CAIDCpDistance(int nNCp,unsigned int nDistIndex,unsigned short nOccAGV)
{
	m_nNCp       = nNCp;
	m_nDistIndex = nDistIndex;
	m_nOccAGV    = nOccAGV;
}

CAIDCpDistance::~CAIDCpDistance()
{
}
