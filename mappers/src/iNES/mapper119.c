#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_MMC3.h"

static	void	Sync (void)
{
	u8 x, y;
	MMC3_SyncMirror();
	MMC3_SyncPRG(0x3F,0);
	for (x = 0; x < 8; x++)
	{
		y = MMC3_GetCHRBank(x);
		if (y & 0x40)
			EMU->SetCHR_RAM1(x,y & 0x07);
		else	EMU->SetCHR_ROM1(x,y & 0x3F);
	}
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
	MMC3_Destroy();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	iNES_InitROM();

	MMC3_Init(Sync);
}

CTMapperInfo	MapperInfo_119 =
{
	119,
	NULL,
	"TQROM (MMC3)",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};