#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_MMC3.h"

static	struct
{
	u8 Reg1, Reg2;
}	Mapper;

static	void	Sync (void)
{
	MMC3_SyncMirror();
	MMC3_SyncPRG(0x3F,0);
	if (Mapper.Reg1 & 0x80)
		EMU->SetPRG_ROM16(0x8,Mapper.Reg1 & 0xF);
	MMC3_SyncCHR_ROM(0xFF,(Mapper.Reg2 & 0x01) << 8);
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	x = MMC3_SaveLoad(mode,x,data);
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg1)
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg2)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write (int Bank, int Where, int What)
{
	if (Where & 1)
		Mapper.Reg2 = What;
	else	Mapper.Reg1 = What;
	Sync();
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
	MMC3_Destroy();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	u8 x;
	iNES_InitROM();

	for (x = 0x6; x < 0x8; x++)
		EMU->SetCPUWriteHandler(0x6,Write);
	Mapper.Reg1 = Mapper.Reg2 = 0;
	MMC3_Init(Sync);
}

static	u8 MapperNum = 115;
CTMapperInfo	MapperInfo_115 =
{
	&MapperNum,
	"Mapper 115",
	COMPAT_NEARLY,
	Reset,
	Shutdown,
	NULL,
	MMC3_PPUCycle,
	SaveLoad,
	NULL,
	NULL
};