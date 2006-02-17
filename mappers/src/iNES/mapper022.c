#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 PRG[2];
	u8 CHR[8];
	u8 Mirror;
}	Mapper;

static	void	Sync (void)
{
	u8 x;
	EMU->SetPRG_ROM8(0x8,Mapper.PRG[0]);
	EMU->SetPRG_ROM8(0xA,Mapper.PRG[1]);
	EMU->SetPRG_ROM16(0xC,-1);
	for (x = 0; x < 8; x++)
		EMU->SetCHR_ROM1(x,Mapper.CHR[x]);
	switch (Mapper.Mirror & 3)
	{
	case 0:	EMU->Mirror_V();		break;
	case 1:	EMU->Mirror_H();		break;
	case 2:	EMU->Mirror_S1();	break;
	case 3:	EMU->Mirror_S0();	break;
	}
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	u8 i;
	for (i = 0; i < 2; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.PRG[i])
	for (i = 0; i < 8; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.CHR[i])
	SAVELOAD_BYTE(mode,x,data,Mapper.Mirror)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write8 (int Bank, int Where, int What)
{
	Mapper.PRG[0] = What;
	Sync();
}

static	void	_MAPINT	Write9 (int Bank, int Where, int What)
{
	Mapper.Mirror = What;
	Sync();
}

static	void	_MAPINT	WriteA (int Bank, int Where, int What)
{
	Mapper.PRG[1] = What;
	Sync();
}

static	void	_MAPINT	WriteB (int Bank, int Where, int What)
{
	Mapper.CHR[0 | (Where & 1)] = What >> 1;
	Sync();
}

static	void	_MAPINT	WriteC (int Bank, int Where, int What)
{
	Mapper.CHR[2 | (Where & 1)] = What >> 1;
	Sync();
}

static	void	_MAPINT	WriteD (int Bank, int Where, int What)
{
	Mapper.CHR[4 | (Where & 1)] = What >> 1;
	Sync();
}

static	void	_MAPINT	WriteE (int Bank, int Where, int What)
{
	Mapper.CHR[6 | (Where & 1)] = What >> 1;
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

	EMU->SetCPUWriteHandler(0x8,Write8);
	EMU->SetCPUWriteHandler(0x9,Write9);
	EMU->SetCPUWriteHandler(0xA,WriteA);
	EMU->SetCPUWriteHandler(0xB,WriteB);
	EMU->SetCPUWriteHandler(0xC,WriteC);
	EMU->SetCPUWriteHandler(0xD,WriteD);
	EMU->SetCPUWriteHandler(0xE,WriteE);

	Mapper.PRG[0] = 0;	Mapper.PRG[1] = 1;
	for (x = 0; x < 8; x++)
		Mapper.CHR[x] = x;
	Mapper.Mirror = 0;
	Sync();
}

CTMapperInfo	MapperInfo_022 =
{
	22,
	NULL,
	"Konami VRC2 Type A",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};