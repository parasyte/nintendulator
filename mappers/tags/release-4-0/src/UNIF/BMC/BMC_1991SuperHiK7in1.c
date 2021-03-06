#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_MMC3.h"

static	struct
{
	u8 WhichGame;
}	Mapper;

static	void	Sync (void)
{
	MMC3_SyncMirror();
	MMC3_SyncPRG((Mapper.WhichGame == 6) ? 0x1F : 0x0F,Mapper.WhichGame << 4);
	MMC3_SyncCHR_ROM((Mapper.WhichGame == 6) ? 0xFF : 0x7F,Mapper.WhichGame << 7);
	MMC3_SyncWRAM();
}

static	int	_MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	x = MMC3_SaveLoad(mode,x,data);
	SAVELOAD_BYTE(mode,x,data,Mapper.WhichGame)
	return x;
}

static	void	_MAPINT	WriteAB (int Bank, int Addr, int Val)
{
	switch (Addr & 1)
	{
	case 0:	MMC3_CPUWriteAB(Bank,Addr,Val);	break;
	case 1:	Mapper.WhichGame = Val & 0x07;
		if (Mapper.WhichGame == 7)
			Mapper.WhichGame = 6;
		Sync();				break;
	}
}

static	void	_MAPINT	Load (void)
{
	MMC3_Load(Sync);
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	if (ResetType == RESET_HARD)
		Mapper.WhichGame = 0;
	MMC3_Reset(ResetType);
	EMU->SetCPUWriteHandler(0xA,WriteAB);
	EMU->SetCPUWriteHandler(0xB,WriteAB);
}
static	void	_MAPINT	Unload (void)
{
	MMC3_Unload();
}

CTMapperInfo	MapperInfo_BMC_1991SuperHiK7in1 =
{
	"BMC-1991SuperHiK7in1",
	"Pirate multicart mapper",
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	MMC3_PPUCycle,
	SaveLoad,
	NULL,
	NULL
};