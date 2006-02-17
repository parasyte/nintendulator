#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_MMC3.h"

static	struct
{
	u8 Cmd;
}	Mapper;

static	void	Sync (void)
{
	MMC3_SyncPRG(0x3F,0);
	MMC3_SyncCHR_ROM(0x1F,0);
}

static	void	_MAPINT	PPUCycle (int Addr, int Scanline, int Cycle, int IsRendering)
{
	if (MMC3_GetCHRBank((Addr >> 10) & 0x7) & 0x20)
		EMU->Mirror_S1();
	else	EMU->Mirror_S0();
	MMC3_PPUCycle(Addr,Scanline,Cycle,IsRendering);
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
	MMC3_Destroy();
}

static	void	_MAPINT	Write (int Bank, int Where, int What)
{
	MMC3_CPUWrite89(Bank,Where,What);
	if (Where & 1)
	{
		if (Mapper.Cmd < 6)
		{
			if (What & 0x20)
				EMU->Mirror_S1();
			else	EMU->Mirror_S0();
		}
	}
	else	Mapper.Cmd = What & 7;
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	iNES_InitROM();

	EMU->SetPRG_RAM8(0x6,0);
	EMU->Mirror_S0();

	MMC3_Init(Sync);

	EMU->SetCPUWriteHandler(0x8,Write);
	EMU->SetCPUWriteHandler(0x9,Write);
}

CTMapperInfo	MapperInfo_095 =
{
	95,
	NULL,
	"Dragon Buster (MMC3 variant)",
	COMPAT_PARTIAL,
	Reset,
	Shutdown,
	NULL,
	PPUCycle,
	MMC3_SaveLoad,
	NULL,
	NULL
};