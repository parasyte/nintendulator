#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"

static	struct
{
	u8 Game;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetPRG_ROM32(0x8,((Mapper.Game & 0x0F) << 1) | (Latch.Data & 0x01));
	EMU->SetCHR_ROM8(0,((Mapper.Game & 0xF0) >> 1) | ((Latch.Data & 0x70) >> 4));
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	x = Latch_SaveLoad_D(mode,x,data);
	SAVELOAD_BYTE(mode,x,data,Mapper.Game)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write (int Bank, int Where, int What)
{
	Mapper.Game = What;
	Sync();
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
	Latch_Destroy();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	u8 x;
	iNES_InitROM();

	for (x = 0x6; x < 0x8; x++)
		EMU->SetCPUWriteHandler(x,Write);
	Mapper.Game = 0;
	Latch_Init(Sync,IsHardReset,FALSE);
}

CTMapperInfo	MapperInfo_046 =
{
	46,
	NULL,
	"GameStation/RumbleStation",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};