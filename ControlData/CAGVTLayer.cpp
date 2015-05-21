#include "..\stdafx.h"
#include "CAGVTLayer.h"

CAGVTLayer::CAGVTLayer()
{
}

CAGVTLayer::CAGVTLayer(unsigned char nLayer)
{
	m_nLayer = nLayer;
}

CAGVTLayer::~CAGVTLayer()
{
}

void CAGVTLayer::LinkToPhysicalStation(int                   nDest,
									   char                  cSide,
									   short                 nPos,
									   CAIDStationArray&     arrAIDStations,
									   CCustomerStationList& listCustomerStations)
{
	bool bRet = true;
	CString strDummy;
	CCustomerStation tCS(nDest,cSide,(unsigned char)nPos,m_nLayer,strDummy);
	POSITION pos = listCustomerStations.Find(tCS);

	if(pos != NULL)
	{
		tCS = listCustomerStations.GetAt(pos);
		m_nPSIndex = tCS.FindAIDStation(arrAIDStations);
	}
	else
	{
		bRet = false;
	}
}
