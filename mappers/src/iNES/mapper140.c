#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 Latch;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetCHR_ROM8 (  0,(Mapper.Latch >> 0) & 0xF);
	EMU->SetPRG_ROM32(0x8,(Mapper.Latch >> 4) & 0x3);
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.Latch)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write (int Bank, int Where, int What)
{
	Mapper.Latch = What;
	Sync();
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	iNES_InitROM();

	EMU->SetCPUWriteHandler(0x6,Write);
	EMU->SetCPUWriteHandler(0x7,Write);

	Mapper.Latch = 0;

	Sync();
}

CTMapperInfo	MapperInfo_140 =
{
	140,
	NULL,
	"Mapper 140 (Bio Senshi Dan)",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};