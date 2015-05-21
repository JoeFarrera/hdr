// DBEstacion.h: interface for the CDBEstacion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBESTACION_H__FC20EE55_A06A_4219_96A1_75B0A8B252DA__INCLUDED_)
#define AFX_DBESTACION_H__FC20EE55_A06A_4219_96A1_75B0A8B252DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Macros para interpretar datos
#define ERRORMESA(f) ((f) & BIT3)
#define PRESENCIAPALET(f) ((f) & BIT1)
#define MESAOPERATIVA(f) ((f) & BIT2)
#define FINPRODUCCION(f) ((f) & BIT4)
#define ESTACIONENTRADA m_EstadoEstacion & BIT1
#define ESTACIONSALIDA m_EstadoEstacion & BIT2
#define ESTACIONMANUAL m_EstadoEstacion & BIT3
#define ESTACIONAUTOMATICO	m_EstadoEstacion & BIT4



typedef struct 
{
 char codclind [8];
 int ptocom;       
 int posicion;     
 char lado [2];    
 int levelest;     
 char prepal [2];  
 char mesaop [2];  
 int errmesa;      
 char finprod [2]; 
 char tipest [2];  
 char codestpul [8];
} TEstAgv;

typedef struct
{
	char codestpul [7 + 1];
    char descrip [40 + 1];
    char tipestpul [1 + 1];
    char senper [1 + 1];
    char tipmesa [1 + 1];
    char tippal [1 + 1];
    char tippalenv [1 + 1];
    int cappul;
    int puncriped;
    char codtren [3 + 1];
    char estest [1 + 1];
    char estpul [1 + 1];
    char codart [10 + 1];
    int palped; 
    char finsum [1 + 1];
    char sumcon [1 + 1];
    char codpullno [7 + 1];
    char evacuar [1 + 1];
	char automan [1 + 1];
    char intcamb [1 + 1];
    int ocupul;
    char senact [1 + 1];
    int palsum;
    int resagv;
    char senagv [1 + 1];
    int palpedrad;
} TEstacion;


class CDBEstacion  
{
public:
	T_sqlca sqlca;
	T_oraca oraca;
	void UpdateDefectoMesa (BYTE *bMensaje);
	static void UpdateBuBDescansos();
	static void InitArrayCargadores (UINT * ptrArray);
	static void SetPuntosEspera();
	static void GetAgvEnCargador();
	static bool IsCargado (UINT ptoCom, bool* todaviaCargando);
	static bool GetCargadorLibre (UINT mpAgv, UINT * ptoCom);
	UINT GetCarga (LPCTSTR codord);
	UINT GetDescarga (LPCTSTR codord);
	UINT GetDescargaLleno(LPCTSTR codord);
	void TestGeneracionOrden (LPCTSTR ptrCodest, LPCTSTR tipEst);
	void UpdateCargadorPLC (BYTE *bMensaje);
	bool GetEstAgv (LPCTSTR codestpul);
	LPCTSTR GetCodEstPul();
	bool SentidoDelante();
	LPCTSTR GetClIND();
	UINT GetNumeroMesas();
	void UpdateEstacionPLC (BYTE * bMensaje);
	bool GetEstAgv (int ptocom, int posicion, int lado);
	//void InsertarEstacion (UINT index);
	CDBEstacion();
	virtual ~CDBEstacion();
	TEstAgv * m_pEstAgv1;	// Puntero a Primera estación del AGV
	TEstAgv * m_pEstAgv2;	// Puntero a Segunda estación del AGV
	BYTE m_CapPulmon;
	BYTE m_OcuPulmon;
	LPCTSTR m_CodPulVac;
	UCHAR m_Automan;
	UCHAR m_Senact;
	UCHAR m_Prepal;
	UCHAR m_Mesaop;
	BYTE m_Errmesa;
	UCHAR m_Finprod;
	BYTE m_Resagv;
	UCHAR m_Tipest;


private:
	void InsertarHistOcuEst();
	void SetOrdenEstacion();
	void UpdateEstAgv(TEstAgv * ptrEstAgv);
	UINT m_tipMesa;			// 1: Mesa Simple 2: Mesa doble
	bool m_fGotDatos;		// Indicar que ya hay datos
	TEstacion * m_pEstacion;	// Puntero a Estación
	void UpdateEstacion();
	BYTE m_EstadoMesa2;
	BYTE m_EstadoMesa1;
	BYTE m_EstadoEstacion;
	void InsertOrUpdateEstacionAgv(int index,
									int ptocom,
									int posicion,
									char lado,
									LPCTSTR ptrCodest,
									LPCTSTR ptrCodestInd);
protected:
};

#endif // !defined(AFX_DBESTACION_H__FC20EE55_A06A_4219_96A1_75B0A8B252DA__INCLUDED_)
