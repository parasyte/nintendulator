#include	"..\..\DLL\d_UNIF.h"

static	struct
{
	u8 Game;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetCHR_ROM8(0,Mapper.Game);
	EMU->SetPRG_ROM16(0x8,Mapper.Game);
	EMU->SetPRG_ROM16(0xC,Mapper.Game);
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.Game)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	if (IsHardReset)
		Mapper.Game = 0;
	else	Mapper.Game++;
	Sync();
	UNIF_SetMirroring(NULL);
}

CTMapperInfo	MapperInfo_BMC_Reset4in1 =
{
	-1,
	"BMC-Reset4in1",
	"BMC-Reset4in1",
	COMPAT_FULL,
	Reset,
	NULL,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};