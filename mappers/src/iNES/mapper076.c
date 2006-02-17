#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 Cmd;
	u8 PRG[3];
	u8 CHR[4];
	u8 Mirror;
}	Mapper;

static	void	Sync (void)
{
	u8 x;
	EMU->SetPRG_ROM8(0x8,Mapper.PRG[0]);
	EMU->SetPRG_ROM8(0xA,Mapper.PRG[1]);
	EMU->SetPRG_ROM8(0xC,Mapper.PRG[2]);
	EMU->SetPRG_ROM8(0xE,-1);
	for (x = 0; x < 4; x++)
		EMU->SetCHR_ROM2(x << 1,Mapper.CHR[x]);
	if (Mapper.Mirror & 1)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	u8 i;
	SAVELOAD_BYTE(mode,x,data,Mapper.Mirror)
	for (i = 0; i < 3; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.PRG[i])
	for (i = 0; i < 4; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.CHR[i])
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write89 (int Bank, int Where, int What)
{
	switch (Where & 0x1)
	{
	case 0:	Mapper.Cmd = What;	break;
	case 1:	switch (Mapper.Cmd & 0x07)
		{
		case 2:	Mapper.CHR[0] = What;	break;
		case 3:	Mapper.CHR[1] = What;	break;
		case 4:	Mapper.CHR[2] = What;	break;
		case 5:	Mapper.CHR[3] = What;	break;
		case 6:	if (Mapper.Cmd & 0x40)
				Mapper.PRG[2] = What;
			else	Mapper.PRG[0] = What;
						break;
		case 7:	Mapper.PRG[1] = What;	break;
		}			break;
	}
	Sync();
}

static	void	_MAPINT	WriteAB (int Bank, int Where, int What)
{
	switch (Where & 0x1)
	{
	case 0:	Mapper.Mirror = What;	break;
	case 1:				break;
	}
	Sync();
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	u8 x;

	iNES_InitROM();

	EMU->SetCPUWriteHandler(0x8,Write89);
	EMU->SetCPUWriteHandler(0x9,Write89);
	EMU->SetCPUWriteHandler(0xA,WriteAB);
	EMU->SetCPUWriteHandler(0xB,WriteAB);

	Mapper.PRG[0] = 0;
	Mapper.PRG[1] = 1;
	Mapper.PRG[2] = -2;
	for (x = 0; x < 4; x++)
		Mapper.CHR[x] = 0;
	Mapper.Cmd = 0;

	Sync();
}

CTMapperInfo	MapperInfo_076 =
{
	76,
	NULL,
	"Mapper 76",
	COMPAT_PARTIAL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};