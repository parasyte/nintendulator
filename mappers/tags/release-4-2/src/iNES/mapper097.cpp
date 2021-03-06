/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"

namespace
{
void	Sync (void)
{
	EMU->SetPRG_ROM16(0x8, -1);
	EMU->SetPRG_ROM16(0xC, Latch::Data & 0xF);
	EMU->SetCHR_RAM8(0, 0);
	switch ((Latch::Data >> 6) & 0x3)
	{
	case 0:	/* EMU->Mirror_S0();
		/* uncertain */		break;
	case 1:	EMU->Mirror_H();	break;
	case 2:	EMU->Mirror_V();	break;
	case 3:	/* EMU->Mirror_S1();
		/* uncertain */		break;
	}
}

void	MAPINT	Load (void)
{
	Latch::Load(Sync, FALSE);
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	Latch::Reset(ResetType);
}
void	MAPINT	Unload (void)
{
	Latch::Unload();
}

u8 MapperNum = 97;
} // namespace

CTMapperInfo	MapperInfo_097 =
{
	&MapperNum,
	_T("Kid Niki (J)"),
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	NULL,
	Latch::SaveLoad_D,
	NULL,
	NULL
};