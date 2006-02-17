#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_MMC1.h"

static	void	Sync_SAROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0x7,0);
	MMC1_SyncCHR_ROM(0x0F,0);
	MMC1_SyncWRAM();
}
static	void	Sync_SBROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0x7,0);
	MMC1_SyncCHR_ROM(0x0F,0);
}
static	void	Sync_SCROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0x7,0);
	MMC1_SyncCHR_ROM(0x1F,0);
}
static	void	Sync_SEROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0x1,0);
	MMC1_SyncCHR_ROM(0x0F,0);
}
static	void	Sync_SGROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0xF,0);
	MMC1_SyncCHR_RAM(0x1,0);
}
static	void	Sync_SKROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0xF,0);
	MMC1_SyncCHR_ROM(0x1F,0);
	MMC1_SyncWRAM();
}
static	void	Sync_SLROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0xF,0);
	MMC1_SyncCHR_ROM(0x1F,0);
}
static	void	Sync_SL1ROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0x7,0);
	MMC1_SyncCHR_ROM(0x1F,0);
}
static	void	Sync_SNROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0xF,0);
	MMC1_SyncCHR_RAM(0x01,0);
	MMC1_SyncWRAM();
}
static	void	Sync_SOROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0xF,0);
	MMC1_SyncCHR_RAM(0x01,0);
	if (MMC1_GetWRAMEnabled())
		EMU->SetPRG_RAM8(0x6,((MMC1_GetCHRBankLo() & 0x10) >> 4) ^ 1);
	else
	{
		EMU->SetPRG_OB4(0x6);
		EMU->SetPRG_OB4(0x7);
	}
}

static	void	_MAPINT	Shutdown (void)
{
	UNIF_SaveSRAM();
	MMC1_Destroy();
}

static	void	_MAPINT	Reset_SAROM (int IsHardReset)
{
	UNIF_InitSRAM(8192);
	MMC1_Init(Sync_SAROM);
}
static	void	_MAPINT	Reset_SBROM (int IsHardReset)
{
	MMC1_Init(Sync_SBROM);
}
static	void	_MAPINT	Reset_SCROM (int IsHardReset)
{
	MMC1_Init(Sync_SCROM);
}
static	void	_MAPINT	Reset_SEROM (int IsHardReset)
{
	MMC1_Init(Sync_SEROM);
}
static	void	_MAPINT	Reset_SGROM (int IsHardReset)
{
	MMC1_Init(Sync_SGROM);
}
static	void	_MAPINT	Reset_SKROM (int IsHardReset)
{
	UNIF_InitSRAM(8192);
	MMC1_Init(Sync_SKROM);
}
static	void	_MAPINT	Reset_SLROM (int IsHardReset)
{
	MMC1_Init(Sync_SLROM);
}
static	void	_MAPINT	Reset_SL1ROM (int IsHardReset)
{
	MMC1_Init(Sync_SL1ROM);
}

static	void	_MAPINT	Reset_SNROM (int IsHardReset)
{
	UNIF_InitSRAM(8192);
	MMC1_Init(Sync_SNROM);
}
static	void	_MAPINT	Reset_SOROM (int IsHardReset)
{
	UNIF_InitSRAM(8192);
	MMC1_Init(Sync_SOROM);
}

CTMapperInfo	MapperInfo_NES_SAROM =
{
	-1,
	"NES-SAROM",
	"NES-SAROM",
	COMPAT_FULL,
	Reset_SAROM,
	Shutdown,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SBROM =
{
	-1,
	"NES-SBROM",
	"NES-SBROM",
	COMPAT_FULL,
	Reset_SBROM,
	NULL,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SCROM =
{
	-1,
	"NES-SCROM",
	"NES-SCROM",
	COMPAT_FULL,
	Reset_SCROM,
	NULL,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SEROM =
{
	-1,
	"NES-SEROM",
	"NES-SEROM",
	COMPAT_FULL,
	Reset_SEROM,
	NULL,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SGROM =
{
	-1,
	"NES-SGROM",
	"NES-SGROM",
	COMPAT_FULL,
	Reset_SGROM,
	NULL,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SKROM =
{
	-1,
	"NES-SKROM",
	"NES-SKROM",
	COMPAT_FULL,
	Reset_SKROM,
	Shutdown,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SLROM =
{
	-1,
	"NES-SLROM",
	"NES-SLROM",
	COMPAT_FULL,
	Reset_SLROM,
	NULL,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SL1ROM =
{
	-1,
	"NES-SL1ROM",
	"NES-SL1ROM",
	COMPAT_FULL,
	Reset_SL1ROM,
	NULL,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SNROM =
{
	-1,
	"NES-SNROM",
	"NES-SNROM",
	COMPAT_FULL,
	Reset_SNROM,
	Shutdown,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SOROM =
{
	-1,
	"NES-SOROM",
	"NES-SOROM",
	COMPAT_FULL,
	Reset_SOROM,
	Shutdown,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};