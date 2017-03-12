; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTargetDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Target.h"

ClassCount=4
Class1=CTargetApp
Class2=CTargetDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TARGET_DIALOG
Resource4=IDD_CONFIGUREDLG_DIALOG
Resource5=IDD_ABOUTBOX (English (U.S.))
Class4=CConfigureDlg
Resource6=IDD_TARGET_DIALOG (English (U.S.))

[CLS:CTargetApp]
Type=0
HeaderFile=Target.h
ImplementationFile=Target.cpp
Filter=N

[CLS:CTargetDlg]
Type=0
HeaderFile=TargetDlg.h
ImplementationFile=TargetDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTargetDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=TargetDlg.h
ImplementationFile=TargetDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_TARGET_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CTargetDlg

[DLG:IDD_TARGET_DIALOG (English (U.S.))]
Type=1
Class=CTargetDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_TIP1,static,1342308352
Control3=IDC_TIP2,static,1342308352
Control4=IDC_CONFIGURE,button,1342242816
Control5=IDC_VIEW1,listbox,1352728833
Control6=IDC_VIEW2,listbox,1352728833
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CONFIGUREDLG_DIALOG]
Type=1
Class=CConfigureDlg
ControlCount=19
Control1=IDC_PROTOCOL,combobox,1344340227
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_PORT,combobox,1344340227
Control11=IDC_SPEED,combobox,1344340227
Control12=IDC_PARITY,combobox,1344340227
Control13=IDC_STOPBITS,combobox,1344340227
Control14=IDC_FLOWBITS,combobox,1344340227
Control15=IDC_CAMERAVIEW,combobox,1344340227
Control16=IDOK,button,1342242816
Control17=IDCANCEL,button,1342242816
Control18=IDC_XMLFILE,edit,1350631552
Control19=IDC_STOP,button,1342242816

[CLS:CConfigureDlg]
Type=0
HeaderFile=ConfigureDlg.h
ImplementationFile=ConfigureDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_FLOWBITS

