#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"

static	void	Sync (void)
{
	EMU->SetCHR_RAM4(0,0);
	EMU->SetCHR_RAM4(4,Latch.Data & 0x03);
	EMU->SetPRG_ROM32(0x8,0);
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
	Latch_Destroy();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	iNES_InitROM();
	Latch_Init(Sync,IsHardReset,FALSE);
}

CTMapperInfo	MapperInfo_013 =
{
	13,
	NULL,
	"CPROM",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	Latch_SaveLoad_D,
	NULL,
	NULL
};