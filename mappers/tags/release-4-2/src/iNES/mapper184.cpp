/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"

namespace
{
u8 Reg;

void	Sync (void)
{
	EMU->SetPRG_ROM16(0x8, 0);
	EMU->SetPRG_ROM16(0xC, -1);
	EMU->SetCHR_ROM4(0, ((Reg & 0x02) << 0) | ((Reg & 0x04) >> 2) | ((Reg & 0x01) << 2));
	EMU->SetCHR_ROM4(4, ((Reg & 0x20) >> 4) | ((Reg & 0x10) >> 1));
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	SAVELOAD_BYTE(mode, x, data, Reg);
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

void	MAPINT	Write (int Bank, int Addr, int Val)
{
	Reg = Val;
	Sync();
}

void	MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_SetMirroring();

	EMU->SetCPUWriteHandler(0x6, Write);
	EMU->SetCPUWriteHandler(0x7, Write);

	if (ResetType == RESET_HARD)
		Reg = 0;

	Sync();
}

u8 MapperNum = 184;
} // namespace

CTMapperInfo	MapperInfo_184 =
{
	&MapperNum,
	_T("Mapper 184 (Atlantis no Nazo)"),
	COMPAT_FULL,
	NULL,
	Reset,
	NULL,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};