#include "..\stdafx.h"
#include "CManBlockAreaObject.h"

CManBlockAreaObject::CManBlockAreaObject()
{
	m_nCp  = 0;
	m_nNCp = 0;
}

CManBlockAreaObject::CManBlockAreaObject(int nCp,int nNCp)
{
	m_nCp  = nCp;
	m_nNCp = nNCp;
}

CManBlockAreaObject::~CManBlockAreaObject()
{
}
