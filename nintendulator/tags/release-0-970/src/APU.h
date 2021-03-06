/* Nintendulator - Win32 NES emulator written in C++
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#ifndef	APU_H
#define APU_H

#ifndef	NSFPLAYER
#define DIRECTSOUND_VERSION 0x0700
#include <mmsystem.h>
#include <dsound.h>
#endif	/* !NSFPLAYER */

namespace APU
{
extern short	*buffer;

#ifdef	NSFPLAYER
extern	short	sample_pos;
extern	BOOL	sample_ok;
#endif	/* NSFPLAYER */

namespace DPCM
{
	void	Fetch (void);
}

void	Init		(void);
void	Create		(void);
void	Release		(void);
#ifndef	NSFPLAYER
int	Save		(FILE *);
int	Load		(FILE *);
void	SoundOFF	(void);
void	SoundON		(void);
#endif	/* !NSFPLAYER */
void	Reset		(void);
#ifndef	NSFPLAYER
void	Config		(HWND);
#endif	/* !NSFPLAYER */
void	Run		(void);
void	SetFPS		(int);
void	WriteReg	(int, unsigned char);
unsigned char	Read4015	(void);

} // namespace APU
#endif	/* !APU_H */
