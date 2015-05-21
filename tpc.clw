; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tpc.h"
LastPage=0

ClassCount=17
Class1=CTpcApp
Class2=CTpcDoc
Class3=CTpcView
Class4=CMainFrame

ResourceCount=26
Resource1=IDD_INSERTARAGVDLG
Class5=CLeftView
Class6=CAboutDlg
Resource2=IDD_TIEMPOORDEN (Spanish (Modern))
Resource3=IDD_LOADDATA (Spanish (Modern))
Resource4=IDD_ABOUTBOX (Spanish (Modern))
Resource5=IDD_INSERTARAGVDLG (Spanish (Modern))
Class7=CTraceLevelDlg
Resource6=IDD_SELECT_AGV_HISTORY
Resource7=IDD_SELECT_AGV_HISTORY (Spanish (Modern))
Class8=CTestSRDlg
Resource8=IDD_TESTGETESTACION
Resource9=IDD_TIMEDLG (Spanish (Modern))
Class9=CTestEstacionDLG
Resource10=IDD_FTF_HISTORY
Class10=CAGVSelectDlg
Resource11=IDD_FTF_HISTORY (Spanish (Modern))
Resource12=IDD_TESTSR
Resource13=IDD_DIALOG_STOP_PLC
Class11=CInsertarAgvDlg
Resource14=IDD_TESTGENORD
Resource15=IDD_TESTGETESTACION (Spanish (Modern))
Resource16=IDD_TRACELEVEL (Spanish (Modern))
Class12=CSolicitarCargaBat
Resource17=IDD_CARGABAT (Spanish (Modern))
Class13=CTiempoOrdenDlg
Resource18=IDR_MAINFRAME (Spanish (Modern))
Resource19=IDD_TIEMPOORDEN
Class14=CPLCThread
Resource20=IDD_TRACELEVEL
Class15=CStartTime
Resource21=IDD_TIMEDLG
Resource22=IDD_TESTSR (Spanish (Modern))
Resource23=IDD_ABOUTBOX
Class16=CFTFHistoryDlg
Resource24=IDD_TESTGENORD (Spanish (Modern))
Resource25=IDD_CARGABAT
Class17=CDialogStopPlc
Resource26=IDR_MAINFRAME

[CLS:CTpcApp]
Type=0
HeaderFile=tpc.h
ImplementationFile=tpc.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CTpcApp

[CLS:CTpcDoc]
Type=0
HeaderFile=tpcDoc.h
ImplementationFile=tpcDoc.cpp
Filter=N

[CLS:CTpcView]
Type=0
HeaderFile=tpcView.h
ImplementationFile=tpcView.cpp
Filter=C
BaseClass=CListView
VirtualFilter=VWC
LastObject=ID_VER_FILTROTRACE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=IDD_AUTOTRAFFICCONTROL
BaseClass=CFrameWnd
VirtualFilter=fWC



[CLS:CLeftView]
Type=0
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp
Filter=T
LastObject=CLeftView

[CLS:CAboutDlg]
Type=0
HeaderFile=tpc.cpp
ImplementationFile=tpc.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_VER_FILTROTRACE
Command11=ID_SCROLL_LIST
Command12=IDD_TESTESTACION
Command13=IDD_TESTSR
Command14=IDD_TESTSR_Q
Command15=ID_SIMULATECOM
Command16=ID_GENERATESIMORDERS
Command17=IDS_INSERTARAGV
Command18=IDS_CARGABAT
Command19=ID_HERRAMIENTAS_TESTGENORDENES
Command20=ID_WRITEAGVHISTORY
Command21=IDS_READSITSTOP
Command22=IDD_TEST_STOP_PLC_COMS
Command23=IDD_AUTOTRAFFICCONTROL
Command24=ID_TIEMPOORDEN
Command25=ID_APP_ABOUT
CommandCount=25

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_VIEW_LARGEICON
Command8=ID_VIEW_SMALLICON
Command9=ID_VIEW_LIST
Command10=ID_VIEW_DETAILS
Command11=ID_APP_ABOUT
CommandCount=11

[DLG:IDR_MAINFRAME]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_APP_EXIT
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_VER_FILTROTRACE
Command11=ID_SCROLL_LIST
Command12=IDD_TESTESTACION
Command13=IDD_TESTSR
Command14=IDD_TESTSR_Q
Command15=ID_SIMULATECOM
Command16=ID_GENERATESIMORDERS
Command17=IDS_INSERTARAGV
Command18=IDS_CARGABAT
Command19=ID_HERRAMIENTAS_TESTGENORDENES
Command20=ID_WRITEAGVHISTORY
Command21=IDS_READSITSTOP
Command22=ID_TIEMPOORDEN
Command23=ID_APP_ABOUT
CommandCount=23

[TB:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_VIEW_LARGEICON
Command8=ID_VIEW_SMALLICON
Command9=ID_VIEW_LIST
Command10=ID_VIEW_DETAILS
Command11=ID_APP_ABOUT
CommandCount=11

[ACL:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[DLG:IDD_ABOUTBOX (Spanish (Modern))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_LOADDATA (Spanish (Modern))]
Type=1
Class=?
ControlCount=18
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_FILENAME,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_LOADFILE,button,1342242816
Control6=IDC_CURLINE,edit,1350631552
Control7=IDC_FICHERONMP,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_FICHEROAREA,edit,1350631552
Control10=IDC_BROWSEAREA,button,1342242816
Control11=IDC_LOADAREA,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_BROWSEFILE,button,1342242816
Control15=IDC_BROWSEFNMP,button,1342242816
Control16=IDC_SAVE,button,1342242816
Control17=IDC_LOAD,button,1342242816
Control18=IDC_STATIC,button,1342177287

[DLG:IDD_TRACELEVEL]
Type=1
Class=CTraceLevelDlg
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TRACE,button,1342242819
Control4=IDC_AVISO,button,1342242819
Control5=IDC_WARNING,button,1342242819
Control6=IDC_ERROR,button,1342242819
Control7=IDC_FATAL,button,1342242819
Control8=IDC_TRACE2,button,1342242819
Control9=IDC_AVISO2,button,1342242819
Control10=IDC_WARNING2,button,1342242819
Control11=IDC_ERROR2,button,1342242819
Control12=IDC_FATAL2,button,1342242819
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352

[CLS:CTraceLevelDlg]
Type=0
HeaderFile=TraceLevelDlg.h
ImplementationFile=TraceLevelDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_AVISO
VirtualFilter=dWC

[DLG:IDD_TRACELEVEL (Spanish (Modern))]
Type=1
Class=CTraceLevelDlg
ControlCount=14
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TRACE,button,1342242819
Control4=IDC_AVISO,button,1342242819
Control5=IDC_WARNING,button,1342242819
Control6=IDC_ERROR,button,1342242819
Control7=IDC_FATAL,button,1342242819
Control8=IDC_TRACE2,button,1342242819
Control9=IDC_AVISO2,button,1342242819
Control10=IDC_WARNING2,button,1342242819
Control11=IDC_ERROR2,button,1342242819
Control12=IDC_FATAL2,button,1342242819
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352

[DLG:IDD_TESTSR (Spanish (Modern))]
Type=1
Class=CTestSRDlg
ControlCount=26
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TIPOTEL,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350639744
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_EDIT4,edit,1350631552
Control9=IDC_EDIT5,edit,1350631552
Control10=IDC_EDIT6,edit,1350631552
Control11=IDC_EDIT7,edit,1350631552
Control12=IDC_EDIT8,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT9,edit,1350631552
Control15=IDC_EDIT10,edit,1350631552
Control16=IDC_EDIT11,edit,1350631552
Control17=IDC_EDIT12,edit,1350631552
Control18=IDC_EDIT13,edit,1350631552
Control19=IDC_EDIT14,edit,1350631552
Control20=IDC_EDIT15,edit,1350631552
Control21=IDC_EDIT16,edit,1350631552
Control22=IDC_EDIT17,edit,1350631552
Control23=IDC_EDIT18,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_EDIT19,edit,1350631552

[CLS:CTestSRDlg]
Type=0
HeaderFile=TestSRDlg.h
ImplementationFile=TestSRDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTestSRDlg

[DLG:IDD_TESTSR]
Type=1
Class=CTestSRDlg
ControlCount=26
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TIPOTEL,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350639744
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_EDIT4,edit,1350631552
Control9=IDC_EDIT5,edit,1350631552
Control10=IDC_EDIT6,edit,1350631552
Control11=IDC_EDIT7,edit,1350631552
Control12=IDC_EDIT8,edit,1350631552
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT9,edit,1350631552
Control15=IDC_EDIT10,edit,1350631552
Control16=IDC_EDIT11,edit,1350631552
Control17=IDC_EDIT12,edit,1350631552
Control18=IDC_EDIT13,edit,1350631552
Control19=IDC_EDIT14,edit,1350631552
Control20=IDC_EDIT15,edit,1350631552
Control21=IDC_EDIT16,edit,1350631552
Control22=IDC_EDIT17,edit,1350631552
Control23=IDC_EDIT18,edit,1350631552
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_EDIT19,edit,1350631552

[DLG:IDD_TESTGETESTACION]
Type=1
Class=CTestEstacionDLG
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PTOCOM,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_POSICION,edit,1350631552
Control6=IDC_LADO,edit,1350639744
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CODCLIND,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[CLS:CTestEstacionDLG]
Type=0
HeaderFile=TestEstacionDLG.h
ImplementationFile=TestEstacionDLG.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CTestEstacionDLG

[DLG:IDD_SELECT_AGV_HISTORY]
Type=1
Class=CAGVSelectDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_SELAGV,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CAGVSelectDlg]
Type=0
HeaderFile=AGVSelectDlg.h
ImplementationFile=AGVSelectDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAGVSelectDlg

[DLG:IDD_TESTGETESTACION (Spanish (Modern))]
Type=1
Class=CTestEstacionDLG
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PTOCOM,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_POSICION,edit,1350631552
Control6=IDC_LADO,edit,1350639744
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CODCLIND,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_SELECT_AGV_HISTORY (Spanish (Modern))]
Type=1
Class=CAGVSelectDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_SELAGV,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_INSERTARAGVDLG]
Type=1
Class=CInsertarAgvDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_NUMAGVINS,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDS_INSERTARAGVMP,edit,1350631552
Control6=IDC_POSINS,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[CLS:CInsertarAgvDlg]
Type=0
HeaderFile=InsertarAgvDlg.h
ImplementationFile=InsertarAgvDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_INSERTARAGVDLG (Spanish (Modern))]
Type=1
Class=CInsertarAgvDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_NUMAGVINS,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDS_INSERTARAGVMP,edit,1350631552
Control6=IDC_POSINS,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_CARGABAT (Spanish (Modern))]
Type=1
Class=CSolicitarCargaBat
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_NUMAGV,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_CARGARCHECK,button,1342242819
Control6=IDC_RADIO1,button,1342308361
Control7=IDC_RADIO2,button,1342177289
Control8=IDC_RADIO3,button,1342177289
Control9=IDC_RADIO4,button,1342177289
Control10=IDC_RADIO5,button,1342177289
Control11=IDC_RADIO6,button,1342177289
Control12=IDC_RADIO7,button,1342177289
Control13=IDC_RADIO8,button,1342177289

[DLG:IDD_TESTGENORD (Spanish (Modern))]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CODEST,edit,1350631552
Control4=IDC_TIPEST,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CSolicitarCargaBat]
Type=0
HeaderFile=SolicitarCargaBat.h
ImplementationFile=SolicitarCargaBat.cpp
BaseClass=CDialog
Filter=D
LastObject=CSolicitarCargaBat
VirtualFilter=dWC

[DLG:IDD_TIEMPOORDEN]
Type=1
Class=CTiempoOrdenDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PARTIDAORDEN,edit,1350631552
Control4=IDC_AUGMENTOTIEMPO,edit,1484849280
Control5=IDC_SLIDER1,msctls_trackbar32,1342242825
Control6=IDC_SLIDER2,msctls_trackbar32,1342242825
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_FRECUENCIAORDEN,edit,1484849280

[CLS:CTiempoOrdenDlg]
Type=0
HeaderFile=TiempoOrdenDlg.h
ImplementationFile=TiempoOrdenDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SLIDER1

[DLG:IDD_CARGABAT]
Type=1
Class=CSolicitarCargaBat
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_NUMAGV,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_CARGARCHECK,button,1342242819
Control6=IDC_RADIO1,button,1342308361
Control7=IDC_RADIO2,button,1342177289
Control8=IDC_RADIO3,button,1342177289
Control9=IDC_RADIO4,button,1342177289
Control10=IDC_RADIO5,button,1342177289
Control11=IDC_RADIO6,button,1342177289
Control12=IDC_RADIO7,button,1342177289
Control13=IDC_RADIO8,button,1342177289

[DLG:IDD_TESTGENORD]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CODEST,edit,1350631552
Control4=IDC_TIPEST,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CPLCThread]
Type=0
HeaderFile=PLCThread.h
ImplementationFile=PLCThread.cpp
BaseClass=CWinThread
Filter=N

[DLG:IDD_TIMEDLG]
Type=1
Class=CStartTime
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TIMEPICKER,SysDateTimePick32,1342242857
Control4=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242852
Control5=IDC_RADIO1,button,1342177289

[CLS:CStartTime]
Type=0
HeaderFile=StartTime.h
ImplementationFile=StartTime.cpp
BaseClass=CDialog
Filter=D
LastObject=CStartTime

[DLG:IDD_TIMEDLG (Spanish (Modern))]
Type=1
Class=CStartTime
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TIMEPICKER,SysDateTimePick32,1342242857
Control4=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242852
Control5=IDC_RADIO1,button,1342177289

[DLG:IDD_TIEMPOORDEN (Spanish (Modern))]
Type=1
Class=CTiempoOrdenDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PARTIDAORDEN,edit,1350631552
Control4=IDC_AUGMENTOTIEMPO,edit,1484849280
Control5=IDC_SLIDER1,msctls_trackbar32,1342242825
Control6=IDC_SLIDER2,msctls_trackbar32,1342242825
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_FRECUENCIAORDEN,edit,1484849280

[CLS:CFTFHistoryDlg]
Type=0
HeaderFile=FTFHistoryDlg.h
ImplementationFile=FTFHistoryDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFTFHistoryDlg

[DLG:IDD_FTF_HISTORY]
Type=1
Class=CFTFHistoryDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_FTFOHNEHIST,listbox,1352728843
Control4=IDC_LIST_FTFMITHIST,listbox,1352728843
Control5=IDC_STATIC,button,1342177287
Control6=IDC_HIST_KOLLISION,button,1342177289
Control7=IDC_HIST_DEADLOCK,button,1342177289
Control8=IDC_HIST_ALLGEM,button,1342177289
Control9=IDC_FTF_ZU_HIST,button,1342242816
Control10=IDC_FTF_NO_HIST,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_FTF_HISTORY (Spanish (Modern))]
Type=1
Class=CFTFHistoryDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_FTFOHNEHIST,listbox,1352728843
Control4=IDC_LIST_FTFMITHIST,listbox,1352728843
Control5=IDC_STATIC,button,1342177287
Control6=IDC_HIST_KOLLISION,button,1342177289
Control7=IDC_HIST_DEADLOCK,button,1342177289
Control8=IDC_HIST_ALLGEM,button,1342177289
Control9=IDC_FTF_ZU_HIST,button,1342242816
Control10=IDC_FTF_NO_HIST,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_STOP_PLC]
Type=1
Class=CDialogStopPlc
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352

[CLS:CDialogStopPlc]
Type=0
HeaderFile=DialogStopPlc.h
ImplementationFile=DialogStopPlc.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogStopPlc
VirtualFilter=dWC

