========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : tpc
========================================================================

Alignment must be set to byte for comunications with BuB

/////////////////////////////////////////////////////////////////////////////

Timeouts for communication with AGV:

120 ms STX
170 ms ETX
(In Indumat system the 120 Ms start is from the start of sending the polling telegram)


Look at eVerification CTeleDlg::VerifyAck(CString& str) for messages to send
back to BuB.

See texte\\ack_msg.txt for messages


21.12.2001:

code in following function modified to allow SitGo in any instance (when simulating)

unsigned char GetEABFahrverbot(TypEABFzStrukt tAFz,
							   TypEABFzStrukt tVFz)


   if(uRet /* Michael 21.12.2001 XXX && !bFVSit */)
   {
	   // Fahrverbot nicht aus Behandlung der Situation-Liste
		uRet = tFESituation.CheckSet(CFzBedingung(*pAFzDat),
									 CFzBedingung(*pVFzDat));
		if(!uRet) uFeFvGrundEAB = 200;
   }

SitGo.Txt modified accordingly.

-- 
vpuntodescanso modified in test database - quitar condicion de automan...
	corregir bug de que mesa mirar cuando es estación de entrada
	-> Generar desde designer

See other database structure modifications.

Arranque 15.01.2002 11:20 approx

Agv 5 has order "half" loaded in T3.1. Doesn't get the order again...


Crash for deadlock ???


Aprox 12:20: Agv 5 en T5.2 no carga orden... ?


14:45 approx: Deadlock at Krones...

Agv 5 en T1.b2 no carga palets 16:00


18.01.2002

Deadlock en proceso:

-- UpdateEstacion en un proceso
-- UpdateEstadoOrden en otro...