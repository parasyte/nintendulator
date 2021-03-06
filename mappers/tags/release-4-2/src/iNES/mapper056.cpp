/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"

namespace
{
u8 PRGcontrol;
u8_n PRG[4];
u8 CHR[8];
u8 Mirror;
u16 IRQcounter;
u16_n IRQlatch;
u8 IRQenabled;

void	Sync (void)
{
	u8 x;
	EMU->SetPRG_RAM8(0x6, 0);
	for (x = 0; x < 3; x++)
		EMU->SetPRG_ROM8(8 | (x << 1), PRG[x].b0);
	EMU->SetPRG_ROM8(0xE, -1);
	if (Mirror)
		EMU->Mirror_V();
	else	EMU->Mirror_H();
	for (x = 0; x < 8; x++)
		EMU->SetCHR_ROM1(x, CHR[x]);
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	u8 i;
	SAVELOAD_BYTE(mode, x, data, PRGcontrol);
	for (i = 0; i < 4; i++)
		SAVELOAD_BYTE(mode, x, data, PRG[i].b0);
	for (i = 0; i < 8; i++)
		SAVELOAD_BYTE(mode, x, data, CHR[i]);
	SAVELOAD_BYTE(mode, x, data, Mirror);
	SAVELOAD_WORD(mode, x, data, IRQcounter);
	SAVELOAD_WORD(mode, x, data, IRQlatch.s0);
	SAVELOAD_BYTE(mode, x, data, IRQenabled);
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

void	MAPINT	CPUCycle (void)
{
	if (IRQenabled & 2)
	{
		if (IRQcounter == 0xFFFF)
		{
			IRQcounter = IRQlatch.s0;
			EMU->SetIRQ(0);
		}
		else	IRQcounter++;
	}
}

void	MAPINT	Write8 (int Bank, int Addr, int Val)
{
	IRQlatch.n0 = Val & 0xF;
}

void	MAPINT	Write9 (int Bank, int Addr, int Val)
{
	IRQlatch.n1 = Val & 0xF;
}

void	MAPINT	WriteA (int Bank, int Addr, int Val)
{
	IRQlatch.n2 = Val & 0xF;
}

void	MAPINT	WriteB (int Bank, int Addr, int Val)
{
	IRQlatch.n3 = Val & 0xF;
}

void	MAPINT	WriteC (int Bank, int Addr, int Val)
{
	IRQenabled = Val & 0x7;
	if (IRQenabled & 0x2)
		IRQcounter = IRQlatch.s0;
	EMU->SetIRQ(1);
}

void	MAPINT	WriteD (int Bank, int Addr, int Val)
{
	if (IRQenabled & 0x1)
		IRQenabled |= 0x2;
	else	IRQenabled &= ~0x2;
	EMU->SetIRQ(1);
}

void	MAPINT	WriteE (int Bank, int Addr, int Val)
{
	PRGcontrol = Val & 0xF;
}

void	MAPINT	WriteF (int Bank, int Addr, int Val)
{
	switch (PRGcontrol)
	{
	case 0x1:	PRG[0].n0 = Val & 0xF;	break;
	case 0x2:	PRG[1].n0 = Val & 0xF;	break;
	case 0x3:	PRG[2].n0 = Val & 0xF;	break;
	case 0x4:	PRG[3].n0 = Val & 0xF;	break;
	}
	switch (Addr & 0xC00)
	{
	case 0x000:	PRG[Addr & 3].n1 = (Val >> 4) & 1;	break;
	case 0x800:	Mirror = Val & 1;			break;
	case 0xC00:	CHR[Addr & 7] = Val;		break;
	}
	Sync();
}

void	MAPINT	Reset (RESET_TYPE ResetType)
{
	u8 x;

	EMU->SetCPUWriteHandler(0x8, Write8);
	EMU->SetCPUWriteHandler(0x9, Write9);
	EMU->SetCPUWriteHandler(0xA, WriteA);
	EMU->SetCPUWriteHandler(0xB, WriteB);
	EMU->SetCPUWriteHandler(0xC, WriteC);
	EMU->SetCPUWriteHandler(0xD, WriteD);
	EMU->SetCPUWriteHandler(0xE, WriteE);
	EMU->SetCPUWriteHandler(0xF, WriteF);

	if (ResetType == RESET_HARD)
	{
		for (x = 0; x < 4; x++)
			PRG[x].b0 = CHR[x | 0] = CHR[x | 4] = 0;
		PRGcontrol = Mirror = 0;
	}

	Sync();
}

u8 MapperNum = 56;
} // namespace

CTMapperInfo	MapperInfo_056 =
{
	&MapperNum,
	_T("SMB3 Pirate"),
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