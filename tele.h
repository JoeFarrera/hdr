#ifndef __CTELE_H
	#define __CTELE_H


class CTele
{
public:
	unsigned short nTeleLaenge;
	unsigned short nRetStatus;
	unsigned short nOnOff;
	unsigned char  nPcNr;
	unsigned char  nData[MAX_TELE_LAENGE-TELE_HEADER];
};

#endif
