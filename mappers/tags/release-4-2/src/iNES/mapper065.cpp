/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"

namespace
{
u8 IRQenabled;
u16 IRQcounter;
u16_n IRQlatch;
u8 PRG[4];
u8 CHR[8];
u8 Mirror;

void	Sync (void)
{
	u8 x;
	EMU->SetPRG_RAM8(0x6, 0);
	for (x = 0; x < 4; x++)
		EMU->SetPRG_ROM8(8 | (x << 1), PRG[x]);
	for (x = 0; x < 8; x++)
		EMU->SetCHR_ROM1(x, CHR[x]);
	if (Mirror & 0x40)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	u8 i;
	SAVELOAD_BYTE(mode, x, data, IRQenabled);
	SAVELOAD_WORD(mode, x, data, IRQcounter);
	SAVELOAD_WORD(mode, x, data, IRQlatch.s0);
	for (i = 0; i < 3; i++)
		SAVELOAD_BYTE(mode, x, data, PRG[i]);
	for (i = 0; i < 8; i++)
		SAVELOAD_BYTE(mode, x, data, CHR[i]);
	SAVELOAD_BYTE(mode, x, data, Mirror);
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

void	MAPINT	CPUCycle (void)
{
	if ((IRQenabled) && (!--IRQcounter))
	{
		EMU->SetIRQ(0);
		IRQenabled = 0;
	}
}

void	MAPINT	Write8 (int Bank, int Addr, int Val)
{
	switch (Addr)
	{
	case 0:	PRG[0] = Val;
		Sync();			break;
	}
}

void	MAPINT	Write9 (int Bank, int Addr, int Val)
{
	switch (Addr)
	{
	case 0:	Mirror = Val;
		Sync();			break;
	case 3:	IRQenabled = Val & 0x80;
		EMU->SetIRQ(1);				break;
	case 4:	IRQcounter = IRQlatch.s0;
		EMU->SetIRQ(1);				break;
	case 5:	IRQlatch.b1 = Val;		break;
	case 6:	IRQlatch.b0 = Val;		break;
	}
}

void	MAPINT	WriteA (int Bank, int Addr, int Val)
{
	switch (Addr)
	{
	case 0:	PRG[1] = Val;
		Sync();			break;
	}
}

void	MAPINT	WriteB (int Bank, int Addr, int Val)
{
	switch (Addr)
	{
	case 0:	CHR[0] = Val;	break;
	case 1:	CHR[1] = Val;	break;
	case 2:	CHR[2] = Val;	break;
	case 3:	CHR[3] = Val;	break;
	case 4:	CHR[4] = Val;	break;
	case 5:	CHR[5] = Val;	break;
	case 6:	CHR[6] = Val;	break;
	case 7:	CHR[7] = Val;	break;
	}
	Sync();
}

void	MAPINT	WriteC (int Bank, int Addr, int Val)
{
	switch (Addr)
	{
	case 0:	PRG[2] = Val;
		Sync();			break;
	}
}

void	MAPINT	Load (void)
{
	iNES_SetSRAM();
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	u8 x;

	EMU->SetCPUWriteHandler(0x8, Write8);
	EMU->SetCPUWriteHandler(0x9, Write9);
	EMU->SetCPUWriteHandler(0xA, WriteA);
	EMU->SetCPUWriteHandler(0xB, WriteB);
	EMU->SetCPUWriteHandler(0xC, WriteC);

	if (ResetType == RESET_HARD)
	{
		PRG[0] = 0;	PRG[1] = 1;	PRG[2] = 0xFE;	PRG[3] = 0xFF;
		for (x = 0; x < 8; x++)
			CHR[x] = x;
		IRQenabled = 0;
		IRQcounter = IRQlatch.s0 = 0;
	}

	Sync();
}

u8 MapperNum = 65;
} // namespace

CTMapperInfo	MapperInfo_065 =
{
	&MapperNum,
	_T("Irem H-3001"),
	COMPAT_FULL,
	Load,
	Reset,
	NULL,
	CPUCycle,
	NULL,
	SaveLoad,
	NULL,
	NULL
};