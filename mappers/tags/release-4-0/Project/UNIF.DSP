# Microsoft Developer Studio Project File - Name="UNIF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UNIF - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UNIF.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UNIF.MAK" CFG="UNIF - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UNIF - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UNIF - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Mappers/UNIF", RDAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UNIF - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "..\obj_unif"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNIF_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Gr /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNIF_EXPORTS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386

!ELSEIF  "$(CFG)" == "UNIF - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin"
# PROP Intermediate_Dir "..\obj_unif"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNIF_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /Gr /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNIF_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "UNIF - Win32 Release"
# Name "UNIF - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Group "Hardware Source Files"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\Hardware\h_FME7.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_Latch.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC1.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC2.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC3.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC4.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC5.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC6.c
# End Source File
# End Group
# Begin Group "Sound Source Files"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\Hardware\Sound\s_FME7.c
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\Sound\s_MMC5.c
# End Source File
# End Group
# Begin Group "Mapper Source Files"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Group "NES"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_AxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_BxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_CxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\NES\NES_DxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_ExROM.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\NES\NES_FxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_GxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_HxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_JxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_MxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_NROM.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\NES\NES_PxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_SxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_TxROM.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Nes\NES_UxROM.c
# End Source File
# End Group
# Begin Group "BTL"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\Unif\Btl\BTL_MARIO1_MALEE2.c
# End Source File
# End Group
# Begin Group "BMC"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_1991SuperHiK7in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_1992Ballgames11in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic115in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic15in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic20in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic20in1A.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic42in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic52in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Generic76in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_GoldenGame150in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Mario7in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_NovelDiamond9999999in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Reset4in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_StudyGame32in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Super1Min1.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\BMC\BMC_Super24in1SC03.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Super700in1.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_Supervision16in1.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\BMC\BMC_SV01.c
# End Source File
# Begin Source File

SOURCE=..\src\Unif\Bmc\BMC_T3H53.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\BMC\BMC_WS.c
# End Source File
# End Group
# Begin Group "UNL"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\UNIF\UNL\UNL_8237.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\UNL\UNL_Dripgame.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\UNL\UNL_H2288.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\UNL\UNL_Sachen.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\UNL\UNL_Sachen_74LS374N.c
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\UNL\UNL_Sachen_8259.c
# End Source File
# End Group
# End Group
# Begin Group "DLL Source Files"

# PROP Default_Filter "c;cpp;cxx;tli"
# Begin Source File

SOURCE=..\src\Dll\d_UNIF.c
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Hardware Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\Hardware\h_FME7.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_Latch.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC1.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC2.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC3.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC4.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC5.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\h_MMC6.h
# End Source File
# End Group
# Begin Group "Sound Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\Hardware\Sound\s_FME7.h
# End Source File
# Begin Source File

SOURCE=..\src\Hardware\Sound\s_MMC5.h
# End Source File
# End Group
# Begin Group "DLL Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\Dll\d_UNIF.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\interface.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\UNIF\resource.h
# End Source File
# Begin Source File

SOURCE=..\src\UNIF\UNIF.rc
# End Source File
# End Group
# End Target
# End Project
