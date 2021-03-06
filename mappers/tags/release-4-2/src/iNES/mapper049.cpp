/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_MMC3.h"

namespace
{
u8 Reg;

void	Sync (void)
{
	MMC3::SyncMirror();
	if (Reg & 1)
		MMC3::SyncPRG(0xF, (Reg & 0xC0) >> 2);
	else	EMU->SetPRG_ROM32(0x8, (Reg & 0x30) >> 4);
	MMC3::SyncCHR_ROM(0x7F, (Reg & 0xC0) << 1);
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	x = MMC3::SaveLoad(mode, x, data);
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

void	MAPINT	Load (void)
{
	MMC3::Load(Sync);
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	u8 x;
	MMC3::Reset(ResetType);
	for (x = 0x6; x < 0x8; x++)
		EMU->SetCPUWriteHandler(x, Write);
	if (ResetType == RESET_HARD)
	{
		Reg = 0;
		Sync();
	}
}
void	MAPINT	Unload (void)
{
	MMC3::Unload();
}

u8 MapperNum = 49;
} // namespace

CTMapperInfo	MapperInfo_049 =
{
	&MapperNum,
	_T("1993 Super HiK 4-in-1 (MMC3)"),
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	MMC3::PPUCycle,
	SaveLoad,
	NULL,
	NULL
};