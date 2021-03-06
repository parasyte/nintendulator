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
void	Sync (void)
{
	u8 x, y;
	MMC3::SyncMirror();
	MMC3::SyncPRG(0x3F, 0);
	MMC3::SyncWRAM();
	for (x = 0; x < 8; x++)
	{
		y = MMC3::GetCHRBank(x);
		if (y & 0x40)
			EMU->SetCHR_RAM1(x, y & 0x07);
		else	EMU->SetCHR_ROM1(x, y & 0x3F);
	}
}

void	MAPINT	Load (void)
{
	MMC3::Load(Sync);
	iNES_SetSRAM();
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	MMC3::Reset(ResetType);
}
void	MAPINT	Unload (void)
{
	MMC3::Unload();
}

u8 MapperNum = 119;
} // namespace

CTMapperInfo	MapperInfo_119 =
{
	&MapperNum,
	_T("TQROM (MMC3)"),
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	MMC3::PPUCycle,
	MMC3::SaveLoad,
	NULL,
	NULL
};