#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_MMC3.h"

static	struct
{
	u8 Reg;
}	Mapper;

static	void	Sync (void)
{
	MMC3_SyncMirror();
	MMC3_SyncPRG(0xF,Mapper.Reg << 4);
	MMC3_SyncCHR_ROM(0x7F,Mapper.Reg << 7);
}

static	int	_MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	x = MMC3_SaveLoad(mode,x,data);
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write (int Bank, int Addr, int Val)
{
	Mapper.Reg = Val & 1;
	Sync();
}

static	void	_MAPINT	Shutdown (void)
{
	MMC3_Destroy();
}

static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_InitROM();

	EMU->SetCPUWriteHandler(0x6,Write);
	EMU->SetCPUWriteHandler(0x7,Write);

	Mapper.Reg = 0;
	MMC3_Init(ResetType,Sync);
}

static	u8 MapperNum = 47;
CTMapperInfo	MapperInfo_047 =
{
	&MapperNum,
	"Super Spike & Nintendo World Cup Soccer (MMC3)",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	MMC3_PPUCycle,
	SaveLoad,
	NULL,
	NULL
};