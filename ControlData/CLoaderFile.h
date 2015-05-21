#ifndef _CLOADERFILE_H_
#define _CLOADERFILE_H_


class CLoaderFile
{
public:
	bool Read				(bool& bVal);
	bool Read				(char& nVal);
	bool Read				(unsigned char& cVal);
	bool Read				(short& nVal);
	bool Read				(unsigned short& nVal);
	bool Read				(int& nVal);
	bool Read				(unsigned int& nVal);
	bool Read				(char* pChar,int nLen);
	bool Read				(CString &str,int nLen);
	bool Read				(unsigned int* pInt,int nInt);
	bool Read				(int* pInt,int nInt);
	bool SeekIdentString	(CString& strIdent);
	bool ReadCompactString	(CString& strCS);
	bool IsOpen();
	CLoaderFile();
	CLoaderFile(const char* pszFile);
	void Close();
	virtual ~CLoaderFile();

private:
	FILE* m_pFile;
};

#endif //_CLOADERFILE_H_
