#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"

static	void	Sync (void)
{
	EMU->SetPRG_ROM16(0x8,Latch.Data & 0x7);
	EMU->SetPRG_ROM16(0xC,-1);
	EMU->SetCHR_ROM8(0,(Latch.Data & 0xF0) >> 4);
	if (Latch.Data & 0x8)
		((ROM->INES_Flags & 0x08) ? EMU->Mirror_V : EMU->Mirror_S1)();
	else	((ROM->INES_Flags & 0x08) ? EMU->Mirror_H : EMU->Mirror_S0)();
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

CTMapperInfo	MapperInfo_078 =
{
	78,
	NULL,
	"Irem 74161/32",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	Latch_SaveLoad_D,
	NULL,
	NULL
};