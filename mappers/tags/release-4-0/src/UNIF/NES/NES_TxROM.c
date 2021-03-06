#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_MMC3.h"

static	void	Sync_TFROM (void)
{
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_ROM(0x3F,0);
	UNIF_SetMirroring(MMC3_SyncMirror);
}
static	void	Sync_TGROM (void)
{
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_RAM(0x7,0);
	MMC3_SyncMirror();
}
static	void	Sync_TKROM (void)
{
	MMC3_SyncWRAM();
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_ROM(0xFF,0);
	MMC3_SyncMirror();
}
static	void	Sync_TLROM (void)
{
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_ROM(0xFF,0);
	MMC3_SyncMirror();
}
static	void	Sync_TR1ROM (void)
{
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_ROM(0x3F,0);
	EMU->Mirror_4();
}
static	void	Sync_TSROM (void)
{
	MMC3_SyncWRAM();
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_ROM(0xFF,0);
	MMC3_SyncMirror();
}

static	void	_MAPINT	Load_TFROM (void)
{
	MMC3_Load(Sync_TFROM);
}
static	void	_MAPINT	Load_TGROM (void)
{
	MMC3_Load(Sync_TGROM);
}
static	void	_MAPINT	Load_TKROM (void)
{
	UNIF_SetSRAM(8192);
	MMC3_Load(Sync_TKROM);
}
static	void	_MAPINT	Load_TLROM (void)
{
	MMC3_Load(Sync_TLROM);
}
static	void	_MAPINT	Load_TR1ROM (void)
{
	MMC3_Load(Sync_TR1ROM);
}
static	void	_MAPINT	Load_TSROM (void)
{
	MMC3_Load(Sync_TSROM);
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	MMC3_Reset(ResetType);
}
static	void	_MAPINT	Unload (void)
{
	MMC3_Unload();
}

CTMapperInfo	MapperInfo_NES_TFROM =
{
	"NES-TFROM",
	"MMC3 with optional hardwired mirroring",
	COMPAT_FULL,
	Load_TFROM,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_TGROM =
{
	"NES-TGROM",
	"MMC3 with CHR-RAM",
	COMPAT_FULL,
	Load_TGROM,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_TKROM =
{
	"NES-TKROM",
	"MMC3 with CHR-ROM and SRAM",
	COMPAT_FULL,
	Load_TKROM,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_TLROM =
{
	"NES-TLROM",
	"MMC3 with CHR-ROM",
	COMPAT_FULL,
	Load_TLROM,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_TR1ROM =
{
	"NES-TR1ROM",
	"MMC3 with 4-screen VRAM",
	COMPAT_FULL,
	Load_TR1ROM,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_TSROM =
{
	"NES-TSROM",
	"MMC3 with WRAM",
	COMPAT_FULL,
	Load_TSROM,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};