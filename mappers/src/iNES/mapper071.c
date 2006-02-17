#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 PRG;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetPRG_ROM16(0x8,Mapper.PRG);
	EMU->SetPRG_ROM16(0xC,-1);
	EMU->SetCHR_RAM8(0,0);
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.PRG)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	WriteCDEF (int Bank, int Where, int What)
{
	Mapper.PRG = What;
	Sync();
}

static	void	_MAPINT	Write9 (int Bank, int Where, int What)
{
	if (What & 0x10)
		EMU->Mirror_S1();
	else	EMU->Mirror_S0();
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	iNES_InitROM();

	EMU->SetCPUWriteHandler(0x9,Write9);	// Fire Hawk needs this
	EMU->SetCPUWriteHandler(0xC,WriteCDEF);
	EMU->SetCPUWriteHandler(0xD,WriteCDEF);
	EMU->SetCPUWriteHandler(0xE,WriteCDEF);
	EMU->SetCPUWriteHandler(0xF,WriteCDEF);

	Mapper.PRG = 0;

	Sync();
}

CTMapperInfo	MapperInfo_071 =
{
	71,
	NULL,
	"Camerica (partial)",
	COMPAT_PARTIAL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};