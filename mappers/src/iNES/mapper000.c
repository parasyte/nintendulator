#include	"..\DLL\d_iNES.h"

static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_InitROM();

	EMU->SetPRG_ROM32(0x8,0);
	if (ROM->INES_CHRSize)
		EMU->SetCHR_ROM8(0,0);
	else	EMU->SetCHR_RAM8(0,0);
}

static	u8 MapperNum = 0;
CTMapperInfo	MapperInfo_000 =
{
	&MapperNum,
	"NROM",
	COMPAT_FULL,
	Reset,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};