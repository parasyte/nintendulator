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

#ifndef __APU_H__
#define __APU_H__

#include <windows.h>
#ifndef NSFPLAYER
#include <mmsystem.h>

#define DIRECTSOUND_VERSION 0x0700
#include <dsound.h>
#endif

struct	tAPU
{
	int			Cycles;
	int			BufPos;
#ifndef NSFPLAYER
	unsigned long		last_pos;
	BOOL			isEnabled;

	LPDIRECTSOUND		DirectSound;
	LPDIRECTSOUNDBUFFER	PrimaryBuffer;
	LPDIRECTSOUNDBUFFER	Buffer;

	short			*buffer;
	BYTE			Regs[0x17];
#endif

	unsigned char		WantFPS;
	unsigned long		MHz;
	unsigned long		QuarterFrameLen;
#ifndef NSFPLAYER
	unsigned long		LockSize;
#endif
};
extern struct tAPU APU;

void	APU_Init		(void);
void	APU_Create		(void);
void	APU_Release		(void);
#ifndef NSFPLAYER
void	APU_GetSoundRegisters	(unsigned char *);
void	APU_SetSoundRegisters	(unsigned char *);
void	APU_SoundOFF		(void);
void	APU_SoundON		(void);
#endif
void	APU_Reset		(void);
#ifndef NSFPLAYER
void	APU_Config		(HWND);
#endif
void	APU_Run			(void);
void	APU_SetFPS		(int);
void	APU_WriteReg		(int,unsigned char);
unsigned char	APU_Read4015	(void);

#ifdef NSFPLAYER
extern	short	sample_pos;
extern	BOOL	sample_ok;
#endif

#endif