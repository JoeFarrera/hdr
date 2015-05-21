#include "..\stdafx.h"
#include "CCtrlDataMemFile.h"





CCtrlDataMemFile::CCtrlDataMemFile()
{
	m_nFileLength = 0;
	m_nPosition   = 0;
	m_pFileData		= NULL;
}

CCtrlDataMemFile::~CCtrlDataMemFile()
{
	if(m_pFileData != NULL)
	{
		delete [] m_pFileData;
	}
}

bool CCtrlDataMemFile::Open(CString &strFileName)
{
	bool bRet(false);
	FILE* pMyFile;

	errno_t errNo = fopen_s(&pMyFile, strFileName,"rb");

	if (pMyFile != NULL)
	{
		fseek(pMyFile,0,SEEK_END);
		m_nFileLength = ftell(pMyFile);
		fseek(pMyFile,0,SEEK_SET);
		m_pFileData       = new UCHAR[m_nFileLength];
		bRet = (m_pFileData != NULL) ? true : false;
		if(bRet)
		{
			size_t nBytes = fread(m_pFileData,1,m_nFileLength,pMyFile);
			bRet = (nBytes == m_nFileLength);
		}
		fclose(pMyFile);
	}
	return bRet;
}

CString CCtrlDataMemFile::ReadCompactString()
{
	CString strRet;
	int     nLen;

	if(Read(&nLen,sizeof(int)))
	{
		char* pChar   = new char[nLen+1];
		*(pChar+nLen) = 0;
	
		nLen          = Read(pChar,nLen);
		*(pChar+nLen) = 0;
		strRet        = CString(pChar);
		delete [] pChar;
	}
	return strRet;
}

bool CCtrlDataMemFile::SeekIdentString(const CString& strIdent)
{
	int nLen,
	    nFoundLen;
	int nError = 0;
	bool bRet   = true;
	bool bFound = false;
	CString strSeek = strIdent + "###";
	
	CString strFound;
	
	SeekToBegin();
	strFound = ReadCompactString();
	if(!strFound.IsEmpty())
	{
		nError++;
		char c;
		while(bRet && !bFound)
		{
			bRet = Read(c);
			if(bRet)
			{
				if(c == '#')
				{
					char ac[2];
					bRet = (Read(ac,2) == 2);
					if(bRet)
					{
						if((ac[0] == '#') && (ac[1] == '#'))
						{
							//Kandidat gefunden:
							nLen = 0;
							bRet = Read(nLen);
							if(bRet && (nLen == (int)strSeek.GetLength()))
							{
								strFound.Empty();
								
								nFoundLen = nLen;
								bRet = Read(strFound,nLen);
								bFound = bRet && (strFound == strSeek);
							}
							if(!bRet)
							{
								nError++;
							}
						}
					}
					else
					{
						bRet = false;
						nError++;
					}
				}
			}
			else
			{
				bRet = false;		
				nError++;
			}	
		}
	}
	return bFound;
}

void CCtrlDataMemFile::SeekToBegin()
{
	m_nPosition = 0;
}

UINT CCtrlDataMemFile::Read(void *pBuf, UINT nCount)
{
	UINT nRet = 0;
	if((m_nPosition + nCount) <= m_nFileLength)
	{
		memcpy(pBuf,m_pFileData + m_nPosition,nCount);
		m_nPosition += nCount;
		nRet = nCount;
	}
	return nRet;
}


bool CCtrlDataMemFile::Read(bool& bVal)
{
	int nByte = Read(&bVal,sizeof(bool));
	return(nByte == sizeof(bool));
}

bool CCtrlDataMemFile::Read(unsigned char& cVal)
{
	int nByte = Read(&cVal,sizeof(unsigned char));
	return(nByte == sizeof(unsigned char));
}

bool CCtrlDataMemFile::Read(char& cVal)
{
	int nByte = Read(&cVal,sizeof(char));
	return (nByte == sizeof(char));
}

bool CCtrlDataMemFile::Read(unsigned short& nVal)
{
	int nByte = Read(&nVal,sizeof(unsigned short));
	return (nByte == sizeof(unsigned short));
}

bool CCtrlDataMemFile::Read(short& nVal)
{
	int nByte = Read(&nVal,sizeof(short));
	return (nByte == sizeof(short));
}

bool CCtrlDataMemFile::Read(int& nVal)
{
	int nByte = Read(&nVal,sizeof(int));
	return (nByte == sizeof(int));
}

bool CCtrlDataMemFile::Read(unsigned int& nVal)
{
	int nByte = Read(&nVal,sizeof(unsigned int));
	return (nByte == sizeof(unsigned int));
}

bool CCtrlDataMemFile::Read(unsigned int* pInt, unsigned int nSize, unsigned int nInt)
{
	int nByte = Read(pInt,nSize * nInt);
	return (nByte == (int)(nSize * nInt));
}

bool CCtrlDataMemFile::Read(CString& str,int nLen)
{
	char* pChar = new char[nLen+1];
	bool bRet = (Read(pChar,nLen) == nLen);
	*(pChar + nLen) = 0;
	str = CString(pChar);
	delete [] pChar;
	return bRet;
}
