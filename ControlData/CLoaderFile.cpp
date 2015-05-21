/* Include files */
#include "..\stdafx.h"
#include  <stdlib.h>
#include  <stdio.h>

#include "CLoaderFile.h"

#include "..\tpc.h"
#include "..\msgMacro.h"

CLoaderFile::CLoaderFile(const char* pszFile)
{
	errno_t errNo = fopen_s(&m_pFile, pszFile, "rb");
}

CLoaderFile::CLoaderFile()
{
	m_pFile = NULL;
}

CLoaderFile::~CLoaderFile()
{
}

bool CLoaderFile::IsOpen()
{
	return (m_pFile != NULL);
}
void CLoaderFile::Close()
{
	if(m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

bool CLoaderFile::SeekIdentString(CString& strIdent)
{
	bool bRet = false;
	CString strSeek = strIdent + "###";
	CString strFound;
	fseek(m_pFile,0,SEEK_SET);
	bRet = ReadCompactString(strFound);
	if(bRet)
	{
		bool bFound = false;
		char c;
		while(bRet && !bFound)
		{
			bRet = Read(c);
			if(bRet)
			{
				if(c == '#')
				{
					char ac[2];
					bRet = Read(ac,2);
					if(bRet)
					{
						if((ac[0] == '#') && (ac[1] == '#'))
						{
							//Kandidat gefunden:
							int nLen = 0;
							bRet = Read(nLen);
							if(bRet && (nLen == (int)strSeek.GetLength()))
							{
								strFound.Empty();
								bRet = Read(strFound,nLen);
								bFound = bRet && (strFound == strSeek);
							}
						}
					}
					else bRet = false;
				}
			}
			else bRet = false;			
		}
	}
	return bRet;
}

bool CLoaderFile::ReadCompactString(CString& strCS)
{
	int nLen = 0;
	bool bRet = Read(nLen);
	if(bRet)
	{
		char* pChar = new char[nLen + 1];
		if(nLen > 0)
		{
			bRet = Read(pChar,nLen);
			*(pChar + nLen) = 0;
		}
		else
		{
			*pChar = 0;
			
		}
		if(bRet)
		{
			strCS = pChar;
		}
		
		//printf("\nReadCompactString: Len = %d",nLen);
		//printf("(%s)",pChar);
	}
	return bRet;
}

bool CLoaderFile::Read(bool& bVal)
{
	int nByte = fread(&bVal,sizeof(bool),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(char& cVal)
{
	int nByte = fread(&cVal,sizeof(char),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(unsigned char& cVal)
{
	int nByte = fread(&cVal,sizeof(unsigned char),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(short& nVal)
{
	int nByte = fread(&nVal,sizeof(short),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(unsigned short& nVal)
{
	int nByte = fread(&nVal,sizeof(unsigned short),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(int& nVal)
{
	int nByte = fread(&nVal,sizeof(int),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(unsigned int& nVal)
{
	int nByte = fread(&nVal,sizeof(unsigned int),1,m_pFile);
	return(nByte != 0);
}

bool CLoaderFile::Read(char* pChar,int nLen)
{
	int nByte = fread(pChar,1,nLen,m_pFile);
	*(pChar + nByte) = 0;
	return (nByte == nLen);
}

bool CLoaderFile::Read(CString& str,int nLen)
{
	char* pChar = new char[nLen+1];
	bool bRet = Read(pChar,nLen);
	str = pChar;
	delete [] pChar;
	return bRet;
}

bool CLoaderFile::Read(unsigned int* pInt,int nInt)
{
	int nElem = fread(pInt,sizeof(unsigned int),nInt,m_pFile);
	return (nElem == nInt);
}

bool CLoaderFile::Read(int* pInt,int nInt)
{
	int nElem = fread(pInt,sizeof(int),nInt,m_pFile);
	return (nElem == nInt);
}
