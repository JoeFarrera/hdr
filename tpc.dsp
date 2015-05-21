# Microsoft Developer Studio Project File - Name="tpc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=tpc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tpc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tpc.mak" CFG="tpc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tpc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "tpc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tpc - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /I ".\TrafficManager" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0a /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "tpc - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /I ".\TrafficManager" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0a /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/tpc_2009.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "tpc - Win32 Release"
# Name "tpc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Agv.cpp
# End Source File
# Begin Source File

SOURCE=.\AGVSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Alarma.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\AutomaticTrafficControl.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundTasks.cpp
# End Source File
# Begin Source File

SOURCE=.\Block.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\BlockTableAGVTypePair.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Container.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\CrashDistance.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\CrashDistanceArray.cpp
# End Source File
# Begin Source File

SOURCE=.\DBCrono.cpp
# End Source File
# Begin Source File

SOURCE=.\DBEstacion.cpp
# End Source File
# Begin Source File

SOURCE=.\DBEstadistica.cpp
# End Source File
# Begin Source File

SOURCE=.\DBEstadoPLC.cpp
# End Source File
# Begin Source File

SOURCE=.\DBGestorOrden.cpp
# End Source File
# Begin Source File

SOURCE=.\DBUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Destination.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogStopPlc.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\DistanceReqWayTele.cpp
# End Source File
# Begin Source File

SOURCE=.\EstacionBuB.cpp
# End Source File
# Begin Source File

SOURCE=.\FTFHistoryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FzBedingung.cpp
# End Source File
# Begin Source File

SOURCE=.\FzSituation.cpp
# End Source File
# Begin Source File

SOURCE=.\GestorAGV.cpp
# End Source File
# Begin Source File

SOURCE=.\GestorEstacionBuB.cpp
# End Source File
# Begin Source File

SOURCE=.\GestorOrden.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertarAgvDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorCliente.cpp
# End Source File
# Begin Source File

SOURCE=.\Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Orden.cpp
# End Source File
# Begin Source File

SOURCE=.\PLCThread.cpp
# End Source File
# Begin Source File

SOURCE=.\PuntoDescanso.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialCom.cpp
# End Source File
# Begin Source File

SOURCE=.\SiemensSR.cpp
# End Source File
# Begin Source File

SOURCE=.\SituationSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SolicitarCargaBat.cpp
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\SIEMENS\SIMATIC.NET\sr\include\SR_util.cpp"
# End Source File
# Begin Source File

SOURCE=.\StartTime.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Tabsetfv.cpp
# End Source File
# Begin Source File

SOURCE=.\TCPServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Telegrama.cpp
# End Source File
# Begin Source File

SOURCE=.\TestEstacionDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\TestGenOrdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TestSRDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TiempoOrdenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\tpc.cpp
# End Source File
# Begin Source File

SOURCE=.\tpc.rc
# End Source File
# Begin Source File

SOURCE=.\tpcDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\tpcView.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceLevelDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Agv.h
# End Source File
# Begin Source File

SOURCE=.\AGVSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Alarma.h
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\AutomaticTrafficControl.h
# End Source File
# Begin Source File

SOURCE=.\BackgroundTasks.h
# End Source File
# Begin Source File

SOURCE=.\Block.h
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\BlockTableAGVTypePair.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\Container.h
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\CrashDistance.h
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\CrashDistanceArray.h
# End Source File
# Begin Source File

SOURCE=.\DBCrono.h
# End Source File
# Begin Source File

SOURCE=.\DBEstacion.h
# End Source File
# Begin Source File

SOURCE=.\DBEstadistica.h
# End Source File
# Begin Source File

SOURCE=.\DBEstadoPLC.h
# End Source File
# Begin Source File

SOURCE=.\DBGestorOrden.h
# End Source File
# Begin Source File

SOURCE=.\DBUtil.h
# End Source File
# Begin Source File

SOURCE=.\Destination.h
# End Source File
# Begin Source File

SOURCE=.\DialogStopPlc.h
# End Source File
# Begin Source File

SOURCE=.\TrafficManager\DistanceReqWayTele.h
# End Source File
# Begin Source File

SOURCE=.\EstacionBuB.h
# End Source File
# Begin Source File

SOURCE=.\FTFHistoryDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\hdr_es\ANLAG_SW\STEUER\ZDV\FZ_TELE.H
# End Source File
# Begin Source File

SOURCE=.\FzBedingung.h
# End Source File
# Begin Source File

SOURCE=.\FzSituation.h
# End Source File
# Begin Source File

SOURCE=.\GestorAGV.h
# End Source File
# Begin Source File

SOURCE=.\GestorEstacionBuB.h
# End Source File
# Begin Source File

SOURCE=.\GestorOrden.h
# End Source File
# Begin Source File

SOURCE=.\InsertarAgvDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\hdr_es\ANLAG_SW\common\KONSTANT.H
# End Source File
# Begin Source File

SOURCE=.\LeftView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MonitorCliente.h
# End Source File
# Begin Source File

SOURCE=.\Msg.h
# End Source File
# Begin Source File

SOURCE=.\msgMacro.h
# End Source File
# Begin Source File

SOURCE=.\MyStackWalker.h
# End Source File
# Begin Source File

SOURCE=.\Orden.h
# End Source File
# Begin Source File

SOURCE=.\PLCThread.h
# End Source File
# Begin Source File

SOURCE=.\PuntoDescanso.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SerialCom.h
# End Source File
# Begin Source File

SOURCE=.\SiemensSR.h
# End Source File
# Begin Source File

SOURCE=.\SituationSet.h
# End Source File
# Begin Source File

SOURCE=.\SolicitarCargaBat.h
# End Source File
# Begin Source File

SOURCE=..\..\SIEMENS\SIMATIC.NET\SR.nt\Include\SR.h
# End Source File
# Begin Source File

SOURCE=..\..\SIEMENS\SIMATIC.NET\SR.nt\Include\SR_util.H
# End Source File
# Begin Source File

SOURCE=.\StackWalker.h
# End Source File
# Begin Source File

SOURCE=.\StartTime.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TABSETFV.H
# End Source File
# Begin Source File

SOURCE=.\tele.h
# End Source File
# Begin Source File

SOURCE=.\Telegrama.h
# End Source File
# Begin Source File

SOURCE=.\TestEstacionDLG.h
# End Source File
# Begin Source File

SOURCE=.\TestGenOrdDlg.h
# End Source File
# Begin Source File

SOURCE=.\TestSRDlg.h
# End Source File
# Begin Source File

SOURCE=.\TiempoOrdenDlg.h
# End Source File
# Begin Source File

SOURCE=.\tpc.h
# End Source File
# Begin Source File

SOURCE=.\tpcDoc.h
# End Source File
# Begin Source File

SOURCE=.\tpcView.h
# End Source File
# Begin Source File

SOURCE=.\TraceLevelDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\hdr_es\ANLAG_SW\STEUER\INCLUDE\TYPEDEF.H
# End Source File
# Begin Source File

SOURCE=.\V_BLOCK.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tpc.ico
# End Source File
# Begin Source File

SOURCE=.\res\tpc.rc2
# End Source File
# Begin Source File

SOURCE=.\res\tpcDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\A1_EP_FB.INC
# End Source File
# Begin Source File

SOURCE=.\A1_EP_MP.A86
# End Source File
# Begin Source File

SOURCE=.\A1_FZEIT.P86
# End Source File
# Begin Source File

SOURCE=.\A1_KDNAM.C86
# End Source File
# Begin Source File

SOURCE=.\A1_MPNMP.INC
# End Source File
# Begin Source File

SOURCE=.\A1_MPQV.INC
# End Source File
# Begin Source File

SOURCE=.\A1_MPSTR.A86
# End Source File
# Begin Source File

SOURCE=.\A1_Z_TYP.C86
# End Source File
# Begin Source File

SOURCE=.\A1_Z_VER.INC
# End Source File
# Begin Source File

SOURCE=.\A1_ZIEL.P86
# End Source File
# Begin Source File

SOURCE=.\A1_ZT_V.C86
# End Source File
# Begin Source File

SOURCE=.\copykontrastfiles.bat
# End Source File
# Begin Source File

SOURCE=.\DBCrono.pc
# End Source File
# Begin Source File

SOURCE=.\DBEstacion.pc
# End Source File
# Begin Source File

SOURCE=.\DBEstadistica.pc
# End Source File
# Begin Source File

SOURCE=.\DBEstadoPLC.pc
# End Source File
# Begin Source File

SOURCE=.\DBGestorOrden.pc
# End Source File
# Begin Source File

SOURCE=.\DBUtil.pc
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE="..\..\Program Files\SIEMENS\SIMATIC.NET\sr\lib\s7_sr.lib"
# End Source File
# Begin Source File

SOURCE=..\..\oracle\ora92\precomp\lib\orasql9.lib
# End Source File
# End Target
# End Project
