#include	"..\..\DLL\d_UNIF.h"

struct
{
	u8 Data;
}	Mapper;

static	void	Sync (void)
{
	if (Mapper.Data & 0x0E)
	{
		EMU->SetPRG_ROM16(0x8,0x0F);
		EMU->SetPRG_ROM16(0xC,Mapper.Data & 0x0F);
	}
	else	EMU->SetPRG_ROM32(0x8,0);
	EMU->SetCHR_ROM8(0,Mapper.Data & 0x0F);
}

int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.Data)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write (int Bank, int Where, int What)
{
	Mapper.Data = EMU->GetCPUReadHandler(Bank)(Bank,Where);
	Sync();
}

static	void	_MAPINT	Shutdown (void)
{
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	u8 x;
	for (x = 0x8; x <= 0xF; x++)
		EMU->SetCPUWriteHandler(x,Write);
	Mapper.Data = 0;
	UNIF_SetMirroring(NULL);
	Sync();
}

CTMapperInfo	MapperInfo_BMC_Generic15in1 =
{
	"BMC-Generic15in1",
	"Pirate multicart mapper",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};