/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_Latch.h"

namespace
{
void	Sync_GNROM (void)
{
	EMU->SetPRG_ROM32(0x8, (Latch::Data >> 4) & 0x3);
	EMU->SetCHR_ROM8(0, (Latch::Data >> 0) & 0x3);
}

void	MAPINT	Load_GNROM (void)
{
	Latch::Load(Sync_GNROM, TRUE);
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	Latch::Reset(ResetType);
	UNIF_SetMirroring(NULL);
}
void	MAPINT	Unload (void)
{
	Latch::Unload();
}
} // namespace

CTMapperInfo	MapperInfo_NES_GNROM =
{
	"NES-GNROM",
	_T("Standard 32KB PRG/8KB CHR switch"),
	COMPAT_FULL,
	Load_GNROM,
	Reset,
	Unload,
	NULL,
	NULL,
	Latch::SaveLoad_D,
	NULL,
	NULL
};