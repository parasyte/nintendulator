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

#ifndef STATES_H
#define STATES_H

#define	STATES_VERSION	"0950"

struct tStates
{
	char BaseFilename[256];
	BOOL NeedSave, NeedLoad;
	int SelSlot;
};

extern	struct	tStates	States;

void	States_Init (void);
void	States_SetFilename (char *);
void	States_SetSlot (int Slot);
int	States_SaveData (FILE *);
BOOL	States_LoadData (FILE *, int);
void	States_SaveState (void);
void	States_LoadState (void);

#endif /* STATES_H */