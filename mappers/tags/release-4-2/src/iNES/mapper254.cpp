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
u16_n IRQcounter;
u8 PRG;
u8 CHR;
u8 Mirror;

void	Sync (void)
{
	EMU->SetPRG_ROM8(0x6, PRG);
	EMU->SetPRG_ROM32(0x8, 3);
	if (Mirror)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
	EMU->SetCHR_ROM8(0, CHR);
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	SAVELOAD_BYTE(mode, x, data, IRQenabled);
	SAVELOAD_WORD(mode, x, data, IRQcounter.s0);
	SAVELOAD_BYTE(mode, x, data, PRG);
	SAVELOAD_BYTE(mode, x, data, CHR);
	SAVELOAD_BYTE(mode, x, data, Mirror);
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

void	MAPINT	CPUCycle (void)
{
	if (IRQenabled)
	{
		IRQcounter.s0++;
		if (IRQcounter.s0 >= 24576)
			EMU->SetIRQ(0);
	}
}

void	MAPINT	Write8 (int Bank, int Addr, int Val)
{
	CHR = Val;
	Sync();
}

void	MAPINT	WriteEF (int Bank, int Addr, int Val)
{
	switch (Addr & 0x3)
	{
	case 0:	PRG = Val & 0x0F;
		Sync();				break;
	case 1:	Mirror = Val & 0x8;
		Sync();				break;
	case 2:	if (Val & 0x02)
			IRQenabled = 1;
		else
		{
			IRQenabled = 0;
			IRQcounter.s0 = 0;
			EMU->SetIRQ(1);
		}				break;
	case 3:					break;
	}
}

void	MAPINT	Reset (RESET_TYPE ResetType)
{
	int x;

	EMU->SetCPUWriteHandler(0x8, Write8);
	for (x = 0xE; x < 0x10; x++)
		EMU->SetCPUWriteHandler(x, WriteEF);

	if (ResetType == RESET_HARD)
	{
		PRG = Mirror = 0;
		IRQenabled = 0;
		IRQcounter.s0 = 0;
	}

	Sync();
}

u8 MapperNum = 254;
} // namespace

CTMapperInfo	MapperInfo_254 =
{
	&MapperNum,
	_T("Ai Senshi Nicol (Pirate)"),
	COMPAT_FULL,
	NULL,
	Reset,
	NULL,
	CPUCycle,
	NULL,
	SaveLoad,
	NULL,
	NULL
};