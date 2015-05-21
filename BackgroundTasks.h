// BackgroundTasks.h: interface for the CBackgroundTasks class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDTASKS_H__CA435CF1_F80F_4198_B9F1_381B69A9C0F4__INCLUDED_)
#define AFX_BACKGROUNDTASKS_H__CA435CF1_F80F_4198_B9F1_381B69A9C0F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBackgroundTasks  
{
public:
	static void ArrancarThread();
	void run();
	CBackgroundTasks();
	virtual ~CBackgroundTasks();
	void * m_OraContext;	// Puntero al contexto para llamadas Oracle

private:
	HANDLE m_hTimers [2];
};

#endif // !defined(AFX_BACKGROUNDTASKS_H__CA435CF1_F80F_4198_B9F1_381B69A9C0F4__INCLUDED_)
