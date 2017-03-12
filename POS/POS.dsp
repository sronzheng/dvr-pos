# Microsoft Developer Studio Project File - Name="POS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=POS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "POS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "POS.mak" CFG="POS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "POS - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "POS - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "POS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../temp/POS/Release"
# PROP Intermediate_Dir "../temp/POS/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "POS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "POS_Dll_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# Begin Special Build Tool
TargetPath=\work\0314\0900\POS\temp\POS\Release\POS.dll
SOURCE="$(InputPath)"
PostBuild_Desc=***Copy OUTPUT File to Pgm DIR***
PostBuild_Cmds=copy $(TargetPath) ..\pgm\Release\POS.dll	copy ..\temp\POS\Release\POS.lib ..\pgm\Release\POS.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "POS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../temp/POS/Debug"
# PROP Intermediate_Dir "../temp/POS/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "POS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "POS_Dll_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
TargetPath=\work\0314\0900\POS\temp\POS\Debug\POS.dll
SOURCE="$(InputPath)"
PostBuild_Desc=***Copy OUTPUT File to Pgm DIR***
PostBuild_Cmds=copy $(TargetPath) ..\pgm\Debug\POS.dll	copy ..\temp\POS\Debug\POS.lib ..\pgm\Debug\POS.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "POS - Win32 Release"
# Name "POS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BinaryStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Casio61000SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Casio6100A0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\DresserWayne0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EagleVision0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EpsonCR0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EpsonHRK_ZA0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EpsonIStore0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Fujitsu1SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Fujitsu20000SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Gilbarco0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\NCR74500SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Pace0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Panda0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\POS.cpp
# End Source File
# Begin Source File

SOURCE=.\Receiver.cpp
# End Source File
# Begin Source File

SOURCE=.\RubyCR0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Sams40SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\Sharp0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StarTSP7000SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StreamReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\TVS0SParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BinaryStream.h
# End Source File
# Begin Source File

SOURCE=.\Casio61000SParser.h
# End Source File
# Begin Source File

SOURCE=.\Casio6100A0SParser.h
# End Source File
# Begin Source File

SOURCE=.\DresserWayne0SParser.h
# End Source File
# Begin Source File

SOURCE=.\EagleVision0SParser.h
# End Source File
# Begin Source File

SOURCE=.\EpsonCR0SParser.h
# End Source File
# Begin Source File

SOURCE=.\EpsonHRK_ZA0SParser.h
# End Source File
# Begin Source File

SOURCE=.\EpsonIStore0SParser.h
# End Source File
# Begin Source File

SOURCE=.\Fujitsu1SParser.h
# End Source File
# Begin Source File

SOURCE=.\Fujitsu20000SParser.h
# End Source File
# Begin Source File

SOURCE=.\Gilbarco0SParser.h
# End Source File
# Begin Source File

SOURCE=.\NCR74500SParser.h
# End Source File
# Begin Source File

SOURCE=.\Pace0SParser.h
# End Source File
# Begin Source File

SOURCE=.\Panda0SParser.h
# End Source File
# Begin Source File

SOURCE=.\Parser.h
# End Source File
# Begin Source File

SOURCE=.\ParserFactory.h
# End Source File
# Begin Source File

SOURCE=.\POS.H
# End Source File
# Begin Source File

SOURCE=.\POSStruct.h
# End Source File
# Begin Source File

SOURCE=.\Receiver.h
# End Source File
# Begin Source File

SOURCE=.\RubyCR0SParser.h
# End Source File
# Begin Source File

SOURCE=.\Sams40SParser.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\Sharp0SParser.h
# End Source File
# Begin Source File

SOURCE=.\StarTSP7000SParser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StreamReceiver.h
# End Source File
# Begin Source File

SOURCE=.\TVS0SParser.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
