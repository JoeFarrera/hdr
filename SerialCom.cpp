// SerialCom.cpp: implementation of the CSerialCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "SerialCom.h"
#include "Telegrama.h"
#include "Container.h"
#include "Alarma.h"
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialCom::CSerialCom()
{
	m_uErrorCount = 0;

	m_strTeleRecibir.Empty ();
	
	// otros variables se inicializan en OpenConnection y SetupConnection
	
	m_bPort = AfxGetApp ()->GetProfileInt ("Settings", "AGVComPort", 0);
	if (0 == m_bPort)	
	{
		CString strTemp;
		strTemp.LoadString (IDS_NOAGVCOMPORTSET);
		AfxMessageBox (strTemp);
		AfxGetApp ()->WriteProfileInt ("Settings", "AGVComPort", 1);
		m_bPort = 1;
	}
	
	m_dwBaudRate = CBR_9600;
	m_bByteSize = 8;
	m_bParity = NOPARITY;
	m_bStopBits = ONESTOPBIT;
	
	
	// Asociar event con lectura
    m_osRead.hEvent = CreateEvent(NULL,    // no security
		TRUE,    // explicit reset req
									FALSE,   // initial event reset
									NULL ) ; // no name
	ASSERT (m_osRead.hEvent != NULL);
	
	m_osWrite.hEvent = CreateEvent(NULL,    // no security
									TRUE,    // explicit reset req
									FALSE,   // initial event reset
									NULL ) ; // no name
	ASSERT (m_osWrite.hEvent != NULL);

	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;

	OpenConnection ();
	
}

CSerialCom::~CSerialCom()
{
	CloseConnection ();

}

// Abrir la conexión con el puerto de comunicaciones
bool CSerialCom::OpenConnection()
{

   bool       fRetVal;
   CString	  strComPrefix;
   CString	  strPort;

   COMMTIMEOUTS  CommTimeOuts;

	// TRACE("NOT Opening coms for %s\n", strPort);

	// M2015 return true;

   // load the COM prefix string and append port number

   ASSERT (strComPrefix.LoadString(IDS_COMPREFIX));

   strPort.Format (_T("%s%d"), strComPrefix, m_bPort);

   // open COM device
   TRACE("strPort is: %s\n", strPort);

   if (( m_hComDev =
      CreateFile( strPort, GENERIC_READ | GENERIC_WRITE,
                  0,                    // exclusive access
                  NULL,                 // no security attrs
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL |
                  FILE_FLAG_OVERLAPPED, //, // overlapped I/O
                  NULL )) == (HANDLE) -1 )
	  {
	   CString strLastError;
	   CString strError;
	   strLastError.Format (_T("%d"), GetLastError ());
	   AfxFormatString2 (strError, IDS_COMPORTERROR, strPort, strLastError);
	   AfxMessageBox(strError, MB_ICONSTOP);
	   return ( FALSE ) ;
	  }
	
      // get any early read notifications 

      SetCommMask(m_hComDev, EV_RXCHAR ) ;

      // setup device buffers

      SetupComm(m_hComDev, 4096, 4096 ) ;

      // purge any information in the buffer

      PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT |
                                      PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

      // set up for overlapped I/O

      CommTimeOuts.ReadIntervalTimeout = 100;
      CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
      CommTimeOuts.ReadTotalTimeoutConstant = 0; 
      // CBR_9600 is approximately 1byte/ms. For our purposes, allow
      // double the expected time per character for a fudge factor.
      CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/m_dwBaudRate ;
      CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
      SetCommTimeouts(m_hComDev, &CommTimeOuts ) ;



   
   // XXX is this necessary.... ?
   fRetVal = SetupConnection( ) ;
   EscapeCommFunction(m_hComDev, SETDTR); 

/*
   if (fRetVal)
   {
      m_fConnected = TRUE ;

      // Create a secondary thread
      // to watch for an event.

      if (NULL == (hCommWatchThread =
                      CreateThread( (LPSECURITY_ATTRIBUTES) NULL,
                                    0,
                                    (LPTHREAD_START_ROUTINE) CommWatchProc,
                                    (LPVOID) this,
                                    0, &dwThreadID )))
      {
         CONNECTED( npTTYInfo ) = FALSE ;
         CloseHandle( COMDEV( npTTYInfo ) ) ;
         fRetVal = FALSE ;
      }
      else
      {
         THREADID( npTTYInfo ) = dwThreadID ;
         HTHREAD( npTTYInfo ) = hCommWatchThread ;

         // assert DTR

         EscapeCommFunction(m_hCom, SETDTR ) ;

      }
   }
   else
   {
      CONNECTED( npTTYInfo ) = FALSE ;
      CloseHandle( COMDEV( npTTYInfo ) ) ;
   }

   // restore cursor

   SetCursor( hOldCursor ) ;
   */


   fRetVal = TRUE;
   return ( fRetVal ) ;

} // end of OpenConnection()




bool CSerialCom::SetupConnection()
{
   bool       fRetVal;
   BYTE       bSet;
   DCB        dcb;

   
   dcb.DCBlength = sizeof( DCB ) ;

   GetCommState(m_hComDev, &dcb ) ;

   dcb.BaudRate = m_dwBaudRate;
   dcb.ByteSize = m_bByteSize;
   dcb.Parity = m_bParity;
   dcb.StopBits = m_bStopBits;

   // setup hardware flow control

   bSet = FC_XONXOFF & FC_DTRDSR;
   dcb.fOutxDsrFlow = bSet;

   if (bSet)
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
   else
      dcb.fDtrControl = DTR_CONTROL_ENABLE;

   bSet = FC_XONXOFF & FC_RTSCTS;
   dcb.fOutxCtsFlow = bSet;

   if (bSet)
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
   else
      dcb.fRtsControl = RTS_CONTROL_ENABLE;


   // setup software flow control

   bSet = (BYTE) ((FC_XONXOFF & FC_XONXOFF) != 0);

   dcb.fInX = dcb.fOutX = bSet;
   dcb.XonChar = ASCII_XON;
   dcb.XoffChar = ASCII_XOFF;
   dcb.XonLim = 100;
   dcb.XoffLim = 100;

   // other various settings

   dcb.fBinary = TRUE;
   dcb.fParity = TRUE;

   fRetVal = (SetCommState(m_hComDev, &dcb) != 0);

   return (fRetVal);

}

// Leer un bloque de datos del puerto de comunicaciones
int CSerialCom::ReadCommBlock(LPSTR lpszBlock, int nMaxLength)
{
   BOOL       fReadStat ;
   COMSTAT    ComStat ;
   DWORD      dwErrorFlags;
   DWORD      dwLength;
   DWORD      dwError;


   

   ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);

   // Leer solo lo que hay en la cola
   dwLength = min((DWORD)nMaxLength, ComStat.cbInQue) ;

   if (dwLength > 0)
   {

      fReadStat = ReadFile(m_hComDev,
							lpszBlock,
							nMaxLength,
							&dwLength,
							&m_osRead) ;
      if (!fReadStat)
      {
         if (GetLastError() == ERROR_IO_PENDING)
         {
            TRACE("ReadCommBlock IO Pending\n");
            // We have to wait for read to complete.
            // This function will timeout according to the
            // CommTimeOuts.ReadTotalTimeoutConstant variable
            // Every time it times out, check for port errors
            while(!GetOverlappedResult(m_hComDev,
										&m_osRead, 
										&dwLength, 
										TRUE ))
            {
               dwError = GetLastError();
               if(dwError == ERROR_IO_INCOMPLETE)
                  // normal result if not finished
                  continue;
               else
               {
                  // an error occurred, try to recover
                  TRACE ("ReadCommBlock ** ERROR **-%u\n", dwError ) ;
                  ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
                  if ((dwErrorFlags > 0))
                  {
					  CString strError;
					  strError.Format ("ReadCommBlock ** ERROR **-%u",
						  dwErrorFlags);
	                  AfxMessageBox (strError, MB_ICONSTOP);
                  }
                  break;
               }

            }

	      }
         else
         {
            // some other error occurred
            dwLength = 0 ;
            ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
            if (dwErrorFlags > 0)
            {
					  CString strError;
					  strError.Format ("ReadCommBlock ** ERROR **-%u",
						  dwErrorFlags);
	                  AfxMessageBox (strError, MB_ICONSTOP);
            }
         }
      }
   }

   return ( dwLength ) ;

} // end of ReadCommBlock()


bool CSerialCom::WriteCommBlock(LPCTSTR lpByte, DWORD dwBytesToWrite)
{
   BOOL        fWriteStat;
   DWORD       dwBytesWritten;
   DWORD       dwErrorFlags;
   DWORD   	   dwError;
   DWORD       dwBytesSent=0;
   COMSTAT     ComStat;

if (g_Container.m_fSimulateCom)
   return (TRUE);

   fWriteStat = WriteFile (m_hComDev, 
							lpByte, 
							dwBytesToWrite,
							&dwBytesWritten, 
							&m_osWrite);

   // Note that normally the code will not execute the following
   // because the driver caches write operations. Small I/O requests
   // (up to several thousand bytes) will normally be accepted
   // immediately and WriteFile will return true even though an
   // overlapped operation was specified

   if (!fWriteStat)
   {
      if(GetLastError() == ERROR_IO_PENDING)
      {
         // We should wait for the completion of the write operation
         // so we know if it worked or not

         // This is only one way to do this. It might be beneficial to
         // place the write operation in a separate thread
         // so that blocking on completion will not negatively
         // affect the responsiveness of the UI

         // If the write takes too long to complete, this
         // function will timeout according to the
         // CommTimeOuts.WriteTotalTimeoutMultiplier variable.
         // This code logs the timeout but does not retry
         // the write.

         while(!GetOverlappedResult(m_hComDev,
									&m_osWrite, 
									&dwBytesWritten, 
									TRUE ))
         {
            dwError = GetLastError();
            if(dwError == ERROR_IO_INCOMPLETE)
            {
               // normal result if not finished
               dwBytesSent += dwBytesWritten;
               continue;
            }
            else
            {
               // an error occurred, try to recover
               TRACE ("WriteCommBlock ** ERROR **-%u\n", dwError ) ;
			
               ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
               if ((dwErrorFlags > 0))
               {
                TRACE ("WriteCommBlock ** ERROR FLAGS **-%u\n", dwErrorFlags ) ;
			   }
               break;
            }
         }

         dwBytesSent += dwBytesWritten;

         if( dwBytesSent != dwBytesToWrite )
              TRACE ("Probable Write Timeout: Total of %ld bytes sent\n", dwBytesSent) ;
         else
		 {
			 if (!FlushFileBuffers (m_hComDev))
			 {
				 dwError = GetLastError ();
				 TRACE ("Flush file buffers error GetLastError %d", dwError);
			 }
			 else
				TRACE ("%ld bytes written\n", dwBytesSent);
		 }

      }
      else
      {
         // some other error occurred
		 TRACE ("LastError is: %d\n", GetLastError ());
         ClearCommError (m_hComDev, &dwErrorFlags, &ComStat);
         if (dwErrorFlags > 0)
         {
              TRACE ("WriteCommBlock ** ERROR FLAGS **-%u\n", dwErrorFlags ) ;
		 }
         return ( FALSE );
      }
   }
   return ( TRUE ) ;

} // end of WriteCommBlock()



bool CSerialCom::CloseConnection()
{
	   // set connected flag to FALSE

   m_fConnected = FALSE ;

   // disable event notification and wait for thread
   // to halt

   SetCommMask(m_hComDev, 0);

   // drop DTR

   EscapeCommFunction(m_hComDev, CLRDTR);

   // purge any outstanding reads/writes and close device handle

   PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT |
                                   PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
   CloseHandle(m_hComDev) ;

   return ( TRUE ) ;


}

bool CSerialCom::SendTelegrama(LPCTSTR telegrama)
{
	bool fRetVal;
	CString strErrorCheck;
	CTelegrama::GetErrorCheck (strErrorCheck, telegrama);

	m_strTeleEnviar.Format (_T("%c%s%s%c"), STX, telegrama, strErrorCheck, ETX);
	fRetVal = WriteCommBlock (m_strTeleEnviar, m_strTeleEnviar.GetLength ());
	MSGTRACE1 ("> %s", m_strTeleEnviar);
	return (fRetVal);
}

DWORD CSerialCom::ReceiveTelegrama(CString &telegrama)
{
	int i;
	int nLength;
	bool fLectura, fStart;
	TCHAR szIn [MAXBLOCK];
	DWORD dwEvtMask = 0;
	DWORD dwResult = 0;

if (g_Container.m_fSimulateCom)
   return (WAIT_OBJECT_0);
	
	// Esperar a que haya telegrama a leer
	TRACE (_T("ReceiveTelegrama: Waiting for read event\n"));
	telegrama.Empty ();
	// Note: This sets the hEvent member of osRead to not signalled


	if (!WaitCommEvent(m_hComDev, &dwEvtMask, &m_osRead))
	{
		dwResult = GetLastError ();
		if (dwResult == ERROR_IO_PENDING)
		{
				dwResult = WaitCommEvent(m_hComDev, &dwEvtMask, &m_osRead);
				if (dwResult == 0)
					dwResult = GetLastError ();
				TRACE (_T("ReceiveTelegrama(): IO_PENDING - waiting\n"));
				// Esperar telegrama entrante o time out
				dwResult = WaitForSingleObject (m_osRead.hEvent, 300);
				if (dwResult != WAIT_OBJECT_0)
				{
					// La peticion de lectura debe ser anulada
					TRACE (_T("ReceiveTelegrama: Time Out...\n"));
					CancelIo (m_hComDev);
					dwResult = WAIT_TIMEOUT;
					m_uErrorCount ++; 
					if (m_uErrorCount == 100)
					{
						
						CAlarma * ptrAlarma = new CAlarma;
						ptrAlarma->AvisarAlarma (4, 1, 0);

						CloseConnection ();
						OpenConnection ();
						m_uErrorCount = 0;
						m_fConnected = true;
					}
				}
				else
				{
					TRACE (_T("ReceiveTelegrama: Hay Event\n"));
					m_uErrorCount = 0;

				}
		}
		else
		{
			// hay otro problema
			TRACE (_T("ReceiveTelegrama: GetLastError is %d\n"), dwResult);
			// XXX Ver como tratar
			dwResult = WAIT_FAILED;
		}
		
		
		
	}
	else
	{
		dwResult = WAIT_OBJECT_0;
		m_uErrorCount = 0;

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			dwResult = WAIT_OBJECT_0;
		}
		else
		{
			TRACE (_T("ReceiveTelegrama: Event no es EV_RXCHAR\n"));
			dwResult = WAIT_FAILED;
		}
	}

	
	if (dwResult == WAIT_OBJECT_0)
	{
		// Mientras no se haya leido un mensaje entero
		for (fLectura = FALSE; telegrama.GetLength () < MAXBLOCK && !fLectura;) 
		{
			
			memset (szIn, ' ', MAXBLOCK);
			if ((nLength = ReadCommBlock(szIn, MAXBLOCK )) || m_strTeleRecibir.GetLength ())
			{
				if (nLength)
				{
					// Guardar el telegrama leido en el buffer de telegramas entrantes
					szIn [nLength] = '\0';
					MSGTRACE2 ("Leido del puerto nLength:%d '%s'\n", 
						nLength,
						szIn);
					m_strTeleRecibir += szIn;	
				}
				
				nLength = m_strTeleRecibir.GetLength ();
				for (i = 0, fStart = 0; i < nLength && !fLectura; i++)
				{
					switch (m_strTeleRecibir [0])
					{
					case STX:
						if (fStart)
						{
							TRACE ("ReceiveTelegrama () Recibido STX en mensaje del port\n");
						}
						else
						{
							fStart = TRUE;
						}
						break;
					case ETX:
						if (!fStart)
						{
							TRACE ("ReceiveTelegrama () Recibido ETX sin registrar mensaje del port\n");
						}
						else
						{
							TRACE ("ReceiveTelegrama () ETX - end mensaje: telegrama:'%s'\n", telegrama); 
							fLectura = TRUE;
						}
						break;
					default:
						telegrama.Insert (telegrama.GetLength (), m_strTeleRecibir [0]);
						break;
					} /* case */	
					m_strTeleRecibir.Delete (0);
					
				} // for i
			} // if nLength
			else
				break;
		} // for
		
		if (fLectura)
		{
			// ver que el error check es correcto
			CString strECin;
			CString strECcalc;
			int nLen;
			strECin = telegrama.Right (2);
			nLen = telegrama.GetLength ();
			telegrama.Delete (nLen - 2, 2);	// El telegrama está "pelado"
			CTelegrama::GetErrorCheck (strECcalc, telegrama);
			if (strECcalc != strECin)
			{
				MSGERROR ("ReceiveTelegrama (): ErrorCheck erroneo\n");
				fLectura = false;
				dwResult = WAIT_FAILED; // Michael 29.10.2001
				// XXX habra que repetir el telegrama
			}
		}
		else
		{
			MSGERROR ("Lectura Erronea en ReceiveTelegrama");
			dwResult = WAIT_FAILED;
		}
	}
	
	return (dwResult);
}

void CSerialCom::ResetReceiveBuffer()
{
	m_strTeleRecibir.Empty ();

}
