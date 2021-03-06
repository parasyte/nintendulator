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
static	void	Sync_SUROM (void)
{
	MMC1_SyncMirror();
	MMC1_SyncPRG(0xF,MMC1_GetCHRBankLo() & 0x10);
	MMC1_SyncCHR_RAM(0x01,0);
	MMC1_SyncWRAM();
}

static	void	_MAPINT	Load_SAROM (void)
{
	UNIF_SetSRAM(8192);
	MMC1_Load(Sync_SAROM);
}
static	void	_MAPINT	Load_SBROM (void)
{
	MMC1_Load(Sync_SBROM);
}
static	void	_MAPINT	Load_SCROM (void)
{
	MMC1_Load(Sync_SCROM);
}
static	void	_MAPINT	Load_SEROM (void)
{
	MMC1_Load(Sync_SEROM);
}
static	void	_MAPINT	Load_SGROM (void)
{
	MMC1_Load(Sync_SGROM);
}
static	void	_MAPINT	Load_SKROM (void)
{
	UNIF_SetSRAM(8192);
	MMC1_Load(Sync_SKROM);
}
static	void	_MAPINT	Load_SLROM (void)
{
	MMC1_Load(Sync_SLROM);
}
static	void	_MAPINT	Load_SL1ROM (void)
{
	MMC1_Load(Sync_SL1ROM);
}
static	void	_MAPINT	Load_SNROM (void)
{
	UNIF_SetSRAM(8192);
	MMC1_Load(Sync_SNROM);
}
static	void	_MAPINT	Load_SOROM (void)
{
	UNIF_SetSRAM(8192);
	MMC1_Load(Sync_SOROM);
}
static	void	_MAPINT	Load_SUROM (void)
{
	UNIF_SetSRAM(8192);
	MMC1_Load(Sync_SUROM);
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	MMC1_Reset(ResetType);
}
static	void	_MAPINT	Unload (void)
{
	MMC1_Unload();
}


CTMapperInfo	MapperInfo_NES_SAROM =
{
	"NES-SAROM",
	"MMC1 with SRAM",
	COMPAT_FULL,
	Load_SAROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SBROM =
{
	"NES-SBROM",
	"MMC1",
	COMPAT_FULL,
	Load_SBROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SCROM =
{
	"NES-SCROM",
	"MMC1",
	COMPAT_FULL,
	Load_SCROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SEROM =
{
	"NES-SEROM",
	"MMC1",
	COMPAT_FULL,
	Load_SEROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SGROM =
{
	"NES-SGROM",
	"MMC1 with CHR-RAM",
	COMPAT_FULL,
	Load_SGROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SKROM =
{
	"NES-SKROM",
	"MMC1 with CHR-ROM and SRAM",
	COMPAT_FULL,
	Load_SKROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SLROM =
{
	"NES-SLROM",
	"MMC1 with CHR-ROM",
	COMPAT_FULL,
	Load_SLROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SL1ROM =
{
	"NES-SL1ROM",
	"MMC1",
	COMPAT_FULL,
	Load_SL1ROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SNROM =
{
	"NES-SNROM",
	"MMC1 with CHR-RAM and SRAM",
	COMPAT_FULL,
	Load_SNROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SOROM =
{
	"NES-SOROM",
	"MMC1 with 16KB SRAM",
	COMPAT_FULL,
	Load_SOROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};
CTMapperInfo	MapperInfo_NES_SUROM =
{
	"NES-SUROM",
	"MMC1 with 512KB PRG",
	COMPAT_FULL,
	Load_SNROM,
	Reset,
	Unload,
	NULL,
	NULL,
	MMC1_SaveLoad,
	NULL,
	NULL
};