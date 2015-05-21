// SiemensSR.cpp: implementation of the CSiemensSR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "msgMacro.h"
#include "SiemensSR.h"
#include "DBEstacion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiemensSR::CSiemensSR()
{
	m_SendBuf [0] = 'Q';	// Para el primer envío
	m_DataLen = 1;
	m_fConnected = false;
	m_fConnecting = false;
	m_fReceivePend = false;
	InitCommunication ();


}

CSiemensSR::~CSiemensSR()
{
	// Michael 22.04.2009
	if (m_AccessPoint != -1)
	{
		int result = ResetSR();
	}

}

/*****************************************************************************
	InitCommunication ()	
	PURPOSE: 
	initializes the communication between the PC and SIMATIC S5/S7,
	opens a connection to SIMATIC NET

	COMMENTS:
	The function "SetSR()" of the interface initializes the communication.
	The first parameter sets the accesspoint, that must be defined	
	with the program "PG/PC Control Panel (PG/PC Schnittstelle einstellen)".
	The second parameter is the handle of the main window.
	The third parameter defines the WM_USER message for messages from SIMATIC 
	NET. This message must be handled by the message loop function "SR_WndProc()".
******************************************************************************/
void CSiemensSR::InitCommunication()
{
	char cAccessPoint[MAX_ACCESSP];
	char cHostID[MAX_MAC];
	char partOfHostID[3];
	int i; 

	// Michael 21.04.2009 inicializar para resetear la conexión al salir
	m_AccessPoint = -1;

	// Michael 09.01.2002 Los posts del Siemens llegan a la ventana principal...

	m_hWnd = g_Container.m_pMainWnd->m_hWnd;


	/* set global variables for communication */
	strcpy(cAccessPoint, "CP_H1_1:");

	/* set remote IP address; use the format "xxx.xxx.xxx.xxx" */
	// Michael 25.05.2007 AGV9 strcpy(cHostID, "140.080.000.004");
	strcpy(cHostID, "140.080.000.009");

	/* set local and remote TSAP */
	strcpy((CHAR*)m_LocalTSAP, "PC_SEND");
	strcpy((CHAR*)m_RemoteTSAP, "RECEIVE");

	/* convert remote HostID (IP address) to use it in the Request Block */
	for(i=0; i<4; i++)
	{	
		strncpy(partOfHostID, &cHostID[4*i], 3);
		sscanf(partOfHostID, "%03d", &m_RemoteMAC[i]);
	}
	/* set last two bytes zero */
	m_RemoteMAC[4]=0;
	m_RemoteMAC[5]=0;

	/* length of data during data exchange in bytes */
	m_DataLen = 2;

	/* data, that should be exchanged (begin with 0, then increment to 99) */
	m_Data = 0;

	/* reference for an opened connection (not yet open) */
	m_OpenRef = 0;


	/* interface not initialized */
	// Michael 23.04.2009 does nothing - m_SrSet = FALSE;

	/* asyncronous communication, no blocking functions */
	SR_async = ASYNC;
	
	/* set the accesspoint for the PG/PC-Interface */
	m_AccessPoint = SetSR(cAccessPoint, m_hWnd, WM_USER_SIEMENS_SR);
	if (m_AccessPoint < 0)
	{
		MSGERROR1 ("Initialization Error: SetSR() returned with Error. GetLastError: %d", 
			GetLastError ());
	}
	// Michael 20.04.2009 TODO - m_SrSet is not used
	else
	{
		/* interface initialized */
		// Michael 23.04.2009 does nothing - m_SrSet = TRUE;
		// Michael 23.04.2009 Open the comm here
		/* indicator for a confirmed connection */
		m_fConnected = FALSE;
		m_fConnecting = TRUE;

		Process(DO_OPEN);
	}

	

	

}

/*****************************************************************************
    Process(WORD opcode) 

	PURPOSE: 
	processes interface functions, for instance "OpenRequest", 
	"ConnectionRequest", "SendData", ...
	If the interface function doesn´ t succeed, a messagebox displays the error.
******************************************************************************/

void CSiemensSR::Process(WORD opcode)
{
	WORD success = 0, 
	userID = 0;  /* userID not used */
	RB1 *rb_ptr = &m_ReqBlock;

	switch (opcode) 
	{	
		case DO_OPEN:
			/* open connection */
			success = SRMD_OpenReq(m_AccessPoint, userID, rb_ptr);
			break;
		case DO_CONNECT:
			/* send a connection request */
			success = SRMD_SendConnRequest(m_AccessPoint, m_OpenRef, userID, rb_ptr, 
				m_RemoteMAC, m_LocalTSAP, m_RemoteTSAP, 'f');
			break;
		case DO_RECEIVE:
			/* received data; save it */
			success = SRMD_ReceiveData(m_AccessPoint, m_OpenRef, userID, &m_ReqBlockRecv, 
				(BYTE *)&m_RecvBuf, 128 /* XXX m_DataLen */);
			// Michael 29.01.2002
			if (success == 0)
				m_fReceivePend = true;
			break;
		case DO_SEND_EOM:
			/* send data */
			success = SRMD_SendEomData(m_AccessPoint, m_OpenRef, userID, &m_ReqBlockSend, 
					(BYTE *)&m_SendBuf, m_DataLen);
			break;
		case DO_CLOSE:
			/* close connection */
			success = SRMD_CloseReq(m_AccessPoint, m_OpenRef, userID, rb_ptr);
			m_Data = 0;
			m_fConnected = FALSE;	// Michael 27.11.2001
			break;
		default:
			return;
	}

	if (success != 0)
	{
		MSGFATAL3 ("Process (): Error: %s returned with %d GetLastError :%d", GetProcessString(opcode), 
			success, GetLastError ());
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6,7,1, GetLastError ());

		// Michael 16.01.2002
		m_fConnected = false;
		m_fConnecting = false;
		::PostQuitMessage (0);


		// Michael 29.01.2002
//		switch (opcode) 
//		{	
//			case DO_OPEN:
//			case DO_CONNECT:
							
//				m_fConnected = TRUE;
//				m_fConnecting = FALSE;
//				Process(DO_SEND_EOM); // PC es activo - iniciar send al CP

				;
				// TODO Michael 12.10.2003	g_Container.m_fSiemensFatalError = true;
				// Michael 21.04.2009
//			success = SRMD_CloseReq(m_AccessPoint, m_OpenRef, userID, rb_ptr);
//			m_Data = 0;
//			m_fConnected = FALSE;	// Michael 27.11.2001
//				success = ResetSR();
//				if (success != 0)
//				{
//					MSGFATAL1 ("ResetSR() GetLastError :%d", GetLastError ());
//				}
//		}
//
		
//		}
	}

}



/*****************************************************************************
    FUNCTION: char *my_GetOpcodeString(WORD opcode) 

	PURPOSE: 
	Returns a string that indicates the current opcode.
	This function is used to make messages easy to understand.
******************************************************************************/

LPCTSTR CSiemensSR::GetOpcodeString(WORD opcode) 
{
	switch (opcode) 
	{	
		case OPEN_REQ:            return "Open Request";
		case SEND_CONN_REQ:       return "Send Connection Request";
		case AWAIT_CONN_REQ_TRAN: return "Await Connection Request";
		case SEND_EOM_DATA:       return "Send EOM Data";
		case RECEIVE_DATA:        return "Receive Data";
		case CLOSE_REQ:           return "Close Request";
		default:                  return "Opcode";
	}
}

/*****************************************************************************
    Response(RB1 *rb) 

	PURPOSE: 
	This function is called when the PC receives a message from a SIMATIC S5/S7
	(see function "SR_WndProc()").
	It checks the success respectively the failure of a request sent before and
	displays it in a messagebox.
******************************************************************************/

void CSiemensSR::Response(RB1 *rb)
{
	WORD response;
	SR_RBH *rb_H;
	static int pp = 0;

	if (rb == NULL)
		return;

	rb_H = (SR_RBH *) rb;
	
	/* check success/failure */
	response = rb_H->rb_response;
	if ((response % 2) == 0 || response == REQ_WAITING)
	{
		/* print out opcode and response */
		MSGERROR3 ("Error in \"%s\"\nrb_response = %s (%d)", 
			GetOpcodeString(rb_H->rb_opcode), GetResponseString(response), 
			response);

		CString strTemp;
		strTemp.Format (IDS_ERRSIEMENSSR, 
			GetOpcodeString(rb_H->rb_opcode),
			GetResponseString(response),
			response);

		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6,2,0,0,0,0,0,0,0,strTemp);

		switch (response)
		{
		case REM_ABORT:
		case ILLEGAL_REQ:
		// Michael 08.07.2008
		case UNKNOWN_REFERENCE:
		// Michael 08.07.2008 fin
			MSGFATAL ("Cerrando comunicacion con el PLC por Error");
			m_fConnected = FALSE;	// Michael 27.11.2001
		}

		// Michael 29.01.2002
		switch (rb_H->rb_opcode)
		{
		case SEND_CONN_REQ:
		case OPEN_REQ:
			// Si ha dado error en la conexión, no va más
			MSGFATAL ("Fatal Siemens Error");
			g_Container.m_fSiemensFatalError = true;
		case RECEIVE_DATA:
			MSGFATAL ("Response () : Anulando receive por error en recepción");
			m_fReceivePend = false; // para que se inicie otro cuando se envíe algo
		}


		return;
	}

	switch (rb_H->rb_opcode) 
	{	
		case OPEN_REQ: 
			/* save reference to the opened connection */
			if (rb_H->rb_response == OK_RESP)
			{
				m_OpenRef = rb->rb.open.open_reference;
				Process(DO_CONNECT);
			}
			return;
		case SEND_CONN_REQ: 
			m_fConnected = TRUE;
			m_fConnecting = FALSE;
			Process(DO_SEND_EOM); // PC es activo - iniciar send al CP
			return;
		case SEND_EOM_DATA: 
			MSGTRACE ("CSiemensSR::Response (): rb_opcode = SEND_EOM_DATA");
			// Michael 29.01.2002 Solo si no hay recieve pendiente
			if (!m_fReceivePend)
				Process(DO_RECEIVE);	
		
			return;
		case RECEIVE_DATA:
			MSGTRACE ("CSiemensSR::Response (): rb_opcode = RECEIVE_DATA");
			VerMensajeRecibido ();
			Process(DO_RECEIVE); // Preparar siguiente lectura
			MSGAVISO ("CSiemensSR::Response (): Siguiente Recepcion pedido");

			return; 
		case CLOSE_REQ: 
			m_OpenRef = 0;
			m_fConnected = FALSE;
			// Michael 22.11.2001 SendMessage(m_hWnd, WM_DESTROY, 0, 0);
			return;
	}

}

/*****************************************************************************
    GetResponseString(WORD resp) 

	PURPOSE: 
	Returns a string that indicates the current rb_response.
	This function is used to make messages easy to understand.
******************************************************************************/
LPCTSTR CSiemensSR::GetResponseString(WORD resp)
{
	switch (resp) 
	{	
		case OK_RESP:             return "OK_RESP";
		case OK_EOM_RESP:         return "OK_EOM_RESP";
		case OK_DECIDE_REQ_RESP:  return "OK_DECIDE_REQ_RESP";
		case OK_CLOSED_RESP:      return "OK_CLOSED_RESP";
		case OK_REJECT_CONN_RESP: return "OK_REJECT_CONN_RESP";
		case INVALID_REQ:         return "INVALID_REQ";
		case NO_RESOURCES:        return "NO_RESOURCES";
		case UNKNOWN_REFERENCE:   return "UNKNOWN_REFERENCE";
		case BUFFER_TOO_SHORT:    return "BUFFER_TOO_SHORT";
		case BUFFER_TOO_LONG:     return "BUFFER_TOO_LONG";
		case ILLEGAL_REQ:         return "ILLEGAL_REQ";
		case REM_ABORT:           return "REM_ABORT";
		case LOC_TIMEOUT:         return "LOC_TIMEOUT";
		case UNKNOWN_CONN_CLASS:  return "UNKNOWN_CONN_CLASS";
		case DUP_REQ:             return "DUP_REQ";
		case CONN_REJECT:         return "CONN_REJECT";
		case NEGOT_FAILED:        return "NEGOT_FAILED";
		case ILLEGAL_ADDRESS:     return "ILLEGAL_ADDRESS";
		case NETWORK_ERROR:       return "NETWORK_ERROR";
		case PROTOCOL_ERR:        return "PROTOCOL_ERR";
		case ILLEGAL_RB_LENGTH:   return "ILLEGAL_RB_LENGTH";
		default:                  return "unknown reponse";
	}

}

// Visualizar mensaje recibido
void CSiemensSR::VerMensajeRecibido()
{
	CString strTemp;
	strTemp.Format ("TipoTelegrama: %d Indumat1: %0x Indumat2: %0x EstMesa1: %x EstMesa2: %x EstPulmon: %x CapBuffer: %d OcuBuffer: %d OcuMesas: %x BufLen: %d",
		m_RecvBuf [0],	
		m_RecvBuf [1],	// Indumat1
		m_RecvBuf [2],	// Indumat2
		m_RecvBuf [3],	// Estmesa1
		m_RecvBuf [4],	// Estmesa2
		m_RecvBuf [5],	// EstPulmon
		m_RecvBuf [6],	// CapBuffer
		m_RecvBuf [7],	// OcuBuffer
		(m_RecvBuf [8] << 8) + m_RecvBuf [9],	// OcuMesas
		    m_ReqBlock.rb.va2.vc_buf0_length);

	MSGAVISO1 ("Mensaje Siemens: %s", strTemp);

	CDBEstacion estacion;

	// Michael 17.12.2009
	if (g_Container.m_fSimulateCom)
		return;


	switch (m_RecvBuf [0])
	{
	case 'C':
		estacion.UpdateCargadorPLC (m_RecvBuf);
		break;
	case 'E':
		estacion.UpdateEstacionPLC (m_RecvBuf);
		break;
		// Michael 10.06.2002
	case 'D':
		estacion.UpdateDefectoMesa (m_RecvBuf);
		break;
		// Michael 10.06.2002 Fin
	default:
		MSGERROR1 ("Mensaje tipo: %c recibido del PLC no interpretado",
			m_RecvBuf [0]);
	}

}

// Enviar un telegrama al PLC
void CSiemensSR::SendTelegrama(BYTE tipoTel, BYTE *ptrDatos, WORD len)
{
	if (!IsConnected ())
	{
		MSGERROR ("CSiemensSR::SendTelegrama () No Conectado");
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6,4,0);
		return; // Michael: XXX ver que hacer 27.11.2001
		// Process(DO_OPEN);
	}
	m_SendBuf [0] = tipoTel;
	memcpy (m_SendBuf + 1, ptrDatos, len);
	m_DataLen = len + 1;	// Len es tamaño datos solo
	Process (DO_SEND_EOM);

}

// Enviar el estado al PLC
void CSiemensSR::SendEstado(BYTE *ptrDatos)
{
	if (!IsConnected ())
	{
		MSGERROR ("CSiemensSR::SendEstado () No Conectado");
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6,4,0);
		return; // Michael: XXX ver que hacer 27.11.2001
		// Process(DO_OPEN);
	}
	memcpy (m_SendBuf, ptrDatos, MAXESTADOSENDPLC);
	m_DataLen = MAXESTADOSENDPLC;
	MSGTRACE("CSiemensSR::SendEstado(). Before Process");
	Process (DO_SEND_EOM);
	MSGTRACE("CSiemensSR::SendEstado(). After Process");
	delete ptrDatos;
}



// Get string asociado con el proceso
LPCTSTR CSiemensSR::GetProcessString(WORD opcode)
{
	switch (opcode) 
	{
		case DO_OPEN:           return "Do Open";
		case DO_CONNECT:        return "Do Connect";
		case DO_LISTEN:			return "Do Listen";
		case DO_SEND_EOM:       return "Do Send EOM";
		case DO_RECEIVE:        return "Do Receive";
		case DO_CLOSE:          return "Do Close";
		default:                
			{
			CString strTemp;
			strTemp.Format ("** Opcode: %d no interpretado **", opcode);
			return strTemp;
			}
	}

}

void CSiemensSR::SendClose()
{

	if (!IsConnected ())
	{
		MSGERROR ("CSiemensSR::SendClose () No Conectado");
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6,4,0);
		return; 
	}
	Process (DO_CLOSE);

}
