/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2011 QMT Productions
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
	MMC3::SyncMirror();
	MMC3::SyncPRG(0x3F, 0);
	MMC3::SyncWRAM();
	for (int i = 0; i < 8; i++)
	{
		uint8 bank = MMC3::GetCHRBank(i);
		if (bank & 0x80)
			EMU->SetCHR_RAM1(i, bank & 0x01);
		else	EMU->SetCHR_ROM1(i, bank & 0x7F);
	}
}

BOOL	MAPINT	Load (void)
{
	MMC3::Load(Sync);
	iNES_SetSRAM();
	return TRUE;
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	MMC3::Reset(ResetType);
}
void	MAPINT	Unload (void)
{
	MMC3::Unload();
}

uint8 MapperNum = 191;
} // namespace

const MapperInfo MapperInfo_191 =
{
	&MapperNum,
	_T("Pirate TQROM variant"),
	COMPAT_NEARLY,
	Load,
	Reset,
	Unload,
	NULL,
	MMC3::PPUCycle,
	MMC3::SaveLoad,
	NULL,
	NULL
};