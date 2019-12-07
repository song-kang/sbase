; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPas_diskinfoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "pas_diskinfo.h"

ClassCount=3
Class1=CPas_diskinfoApp
Class2=CPas_diskinfoDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PAS_DISKINFO_DIALOG

[CLS:CPas_diskinfoApp]
Type=0
HeaderFile=pas_diskinfo.h
ImplementationFile=pas_diskinfo.cpp
Filter=N

[CLS:CPas_diskinfoDlg]
Type=0
HeaderFile=pas_diskinfoDlg.h
ImplementationFile=pas_diskinfoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_INFO

[CLS:CAboutDlg]
Type=0
HeaderFile=pas_diskinfoDlg.h
ImplementationFile=pas_diskinfoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PAS_DISKINFO_DIALOG]
Type=1
Class=CPas_diskinfoDlg
ControlCount=1
Control1=IDC_EDIT_INFO,edit,1350637700

