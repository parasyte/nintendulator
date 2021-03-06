/*
Nintendulator - A Win32 NES emulator written in C.
Designed for maximum emulation accuracy.
Copyright (c) 2002  Quietust

Based on NinthStar, a portable Win32 NES Emulator written in C++
Copyright (C) 2000  David de Regt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

For a copy of the GNU General Public License, go to:
http://www.gnu.org/copyleft/gpl.html#SEC1
*/

#ifndef NES_H
#define NES_H

#include "MapperInterface.h"
#include "resource.h"
#include <windows.h>

struct tNES
{
	int SRAM_Size;

	int PRGMask, CHRMask;

	BOOL ROMLoaded;
	BOOL Stop, Running, Scanline;
	BOOL GameGenie;
	BOOL SoundEnabled;
	BOOL AutoRun;
	BOOL FrameStep, GotStep;
	BOOL HasMenu;
};
extern	struct tNES NES;

#define	MAX_PRGROM_MASK	0x7FF
#define	MAX_PRGRAM_MASK	0xF
#define	MAX_CHRROM_MASK	0xFFF
#define	MAX_CHRRAM_MASK	0x1F
extern	unsigned char PRG_ROM[MAX_PRGROM_MASK+1][0x1000];	/* 8192 KB */
extern	unsigned char PRG_RAM[MAX_PRGRAM_MASK+1][0x1000];	/*   64 KB */
extern	unsigned char CHR_ROM[MAX_CHRROM_MASK+1][0x400];	/* 4096 KB */
extern	unsigned char CHR_RAM[MAX_CHRRAM_MASK+1][0x400];	/*   32 KB */

void	NES_Init (void);
void	NES_Release (void);
void	NES_OpenFile (char *);
void	NES_CloseFile (void);
int	NES_FDSSave (FILE *);
int	NES_FDSLoad (FILE *);
void	NES_SaveSRAM (void);
void	NES_LoadSRAM (void);
const char *	NES_OpenFileiNES (char *);
const char *	NES_OpenFileUNIF (char *);
const char *	NES_OpenFileFDS (char *);
const char *	NES_OpenFileNSF (char *);
void	NES_SetCPUMode (int);
void	NES_Reset (RESET_TYPE);

void	NES_Start (BOOL);
void	NES_Stop (void);

void	NES_UpdateInterface (void);
void	NES_LoadSettings (void);
void	NES_SaveSettings (void);
void	NES_Repaint (void);
void	NES_MapperConfig (void);

#endif /* NES_H */