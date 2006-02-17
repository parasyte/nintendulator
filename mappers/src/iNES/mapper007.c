#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"

static	void	Sync (void)
{
	EMU->SetPRG_ROM32(0x8,Latch.Data & 0x07);
	EMU->SetCHR_RAM8(0,0);
	if (Latch.Data & 0x10)
		EMU->Mirror_S1();
	else	EMU->Mirror_S0();
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
	Latch_Destroy();
}

static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_InitROM();
	Latch_Init(ResetType,Sync,FALSE);
}

static	u8 MapperNum = 7;
CTMapperInfo	MapperInfo_007 =
{
	&MapperNum,
	"AOROM",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	Latch_SaveLoad_D,
	NULL,
	NULL
};
