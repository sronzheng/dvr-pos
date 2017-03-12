# Microsoft Developer Studio Project File - Name="POSTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=POSTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "POSTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "POSTest.mak" CFG="POSTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "POSTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "POSTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "POSTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../temp/POSTest/Release"
# PROP Intermediate_Dir "../temp/POSTest/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../cppunit/include" /I "../pos" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_UNIT_TEST_" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cppunit.lib /nologo /subsystem:console /machine:I386 /libpath:"../cppunit/lib"
# Begin Special Build Tool
TargetPath=\work\0314\0900\POS\temp\POSTest\Release\POSTest.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Unit Testing ...
PostBuild_Cmds=$(TargetPath)
# End Special Build Tool

!ELSEIF  "$(CFG)" == "POSTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../temp/POSTest/Debug"
# PROP Intermediate_Dir "../temp/POSTest/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppunit/include" /I "../pos" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_UNIT_TEST_" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cppunitd.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../cppunit/lib"
# Begin Special Build Tool
TargetPath=\work\0314\0900\POS\temp\POSTest\Debug\POSTest.exe
SOURCE="$(InputPath)"
PostBuild_Desc=Unit Testing ...
PostBuild_Cmds=$(TargetPath)
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "POSTest - Win32 Release"
# Name "POSTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\pos\BinaryStream.cpp
# End Source File
# Begin Source File

SOURCE=.\BinaryStreamTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Casio61000SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Casio61000SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Casio6100A0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Casio6100A0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\DresserWayne0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\DresserWayne0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\EagleVision0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EagleVision0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\EpsonCR0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EpsonCR0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\EpsonHRK_ZA0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EpsonHRK_ZA0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\EpsonIStore0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\EpsonIStore0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Fujitsu1SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Fujitsu1SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Fujitsu20000SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Fujitsu20000SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Gilbarco0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Gilbarco0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\NCR74500SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\NCR74500SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Pace0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Pace0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Panda0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Panda0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\ParserFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserFactoryTest.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserStub.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserTest.cpp
# End Source File
# Begin Source File

SOURCE=.\POSTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Receiver.cpp
# End Source File
# Begin Source File

SOURCE=.\ReceiverStub.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\RubyCR0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\RubyCR0SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Sams40SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Sams40SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\Sharp0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SharpParserTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\StarTSP7000SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StarTSP7000SParserTest.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\pos\StreamReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\StreamReceiverTest.cpp
# End Source File
# Begin Source File

SOURCE=..\pos\TVS0SParser.cpp
# End Source File
# Begin Source File

SOURCE=.\TVS0SParserTest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\POS\BinaryStream.h
# End Source File
# Begin Source File

SOURCE=.\BinaryStreamTest.h
# End Source File
# Begin Source File

SOURCE=.\Casio61000SParserTest.h
# End Source File
# Begin Source File

SOURCE=..\pos\Casio6100A0SParser.h
# End Source File
# Begin Source File

SOURCE=.\Casio6100A0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\DresserWayne0SParserTest.h
# End Source File
# Begin Source File

SOURCE=..\POS\EagleVision0SParser.h
# End Source File
# Begin Source File

SOURCE=.\EagleVision0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\EpsonCR0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\EpsonHRK_ZA0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\EpsonIStore0SParserTest.h
# End Source File
# Begin Source File

SOURCE=..\pos\Fujitsu1SParser.h
# End Source File
# Begin Source File

SOURCE=.\Fujitsu1SParserTest.h
# End Source File
# Begin Source File

SOURCE=..\pos\Fujitsu20000SParser.h
# End Source File
# Begin Source File

SOURCE=.\Fujitsu20000SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\Gilbarco0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\NCR74500SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\Pace0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\Panda0SParserTest.h
# End Source File
# Begin Source File

SOURCE=..\POS\Parser.h
# End Source File
# Begin Source File

SOURCE=.\ParserFactoryTest.h
# End Source File
# Begin Source File

SOURCE=.\ParserStub.h
# End Source File
# Begin Source File

SOURCE=.\ParserTest.h
# End Source File
# Begin Source File

SOURCE=..\pos\Receiver.h
# End Source File
# Begin Source File

SOURCE=.\ReceiverStub.h
# End Source File
# Begin Source File

SOURCE=.\RubyCR0SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\Sams40SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\SharpParserTest.h
# End Source File
# Begin Source File

SOURCE=.\StarTSP7000SParserTest.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StreamReceiverTest.h
# End Source File
# Begin Source File

SOURCE=.\TVS0SParserTest.h
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
