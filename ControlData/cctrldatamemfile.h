#ifndef _CCTRLDATAMEMFILE_H_
#define _CCTRLDATAMEMFILE_H_

#include  <stdlib.h>
#include  <stdio.h>



class CCtrlDataMemFile
{
public:
	bool Read(CString& str,int nLen);
	bool Read(bool& bVal);
	bool Read(unsigned char& cVal);
	bool Read(char& cVal);
	bool Read(unsigned short& nVal);
	bool Read(short& nVal);
	bool Read(unsigned int& nVal);
	bool Read(int& nVal);
	bool Read(unsigned int* pInt, unsigned int nSize, unsigned int nInt);
	
	UINT Read(void* pBuf, UINT nCount);
	CString ReadCompactString();
	bool SeekIdentString(const CString& strIdent);
	void SeekToBegin();
	bool Open(CString& strFileName);
	CCtrlDataMemFile();
	virtual ~CCtrlDataMemFile();
	

private:
	UINT  m_nPosition;
	UCHAR* m_pFileData;
	UINT  m_nFileLength;
};

#endif //_CCTRLDATAMEMFILE_H_
