#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_MMC2.h"

static	void	Sync (void)
{
	MMC2_SyncPRG(0xF,0);
	MMC2_SyncCHR();
	MMC2_SyncMirror();
}

static	void	_MAPINT	Load (void)
{
	MMC2_Load(Sync);
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_SetMirroring();
	MMC2_Reset(ResetType);
}
static	void	_MAPINT	Unload (void)
{
	MMC2_Unload();
}

static	u8 MapperNum = 9;
CTMapperInfo	MapperInfo_009 =
{
	&MapperNum,
	"MMC2",
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC2_SaveLoad,
	NULL,
	NULL
};