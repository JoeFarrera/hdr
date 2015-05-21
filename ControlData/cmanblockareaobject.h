#ifndef _CMANBLOCKAREAOBJECT_H_
#define _CMANBLOCKAREAOBJECT_H_


class CManBlockAreaObject
{
public:
	int GetCp (){return  m_nCp;}
	int GetNCp(){return m_nNCp;}
	CManBlockAreaObject();
	CManBlockAreaObject(int nCp,int nNCp);
	virtual ~CManBlockAreaObject();
	
private:
	//Variable:
	int			m_nCp;
	int			m_nNCp;
};

typedef CArray<CManBlockAreaObject, CManBlockAreaObject&> CManBlockAreaObjectArray;
typedef CList <CManBlockAreaObject, CManBlockAreaObject& > CManBlockAreaObjectList;

#endif //_CMANBLOCKAREAOBJECT_H_
