#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 PRG[2];
	u8_n CHR[8];
	u8 Mirror;
}	Mapper;

static	void	Sync (void)
{
	u8 x;
	EMU->SetPRG_RAM8(0x6,0);
	EMU->SetPRG_ROM8(0x8,Mapper.PRG[0]);
	EMU->SetPRG_ROM8(0xA,Mapper.PRG[1]);
	EMU->SetPRG_ROM16(0xC,0xF);
	for (x = 0; x < 8; x++)
		EMU->SetCHR_ROM1(x,Mapper.CHR[x].b0 >> 1);
	if (Mapper.Mirror & 0x1)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
}

static	int	_MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	u8 i;
	for (i = 0; i < 2; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.PRG[i])
	for (i = 0; i < 8; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.CHR[i].b0)
	SAVELOAD_BYTE(mode,x,data,Mapper.Mirror)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write8 (int Bank, int Addr, int Val)
{
	Mapper.PRG[0] = Val & 0x1F;
	Sync();
}

static	void	_MAPINT	Write9 (int Bank, int Addr, int Val)
{
	Mapper.Mirror = Val & 0xF;
	Sync();
}

static	void	_MAPINT	WriteA (int Bank, int Addr, int Val)
{
	Mapper.PRG[1] = Val & 0x1F;
	Sync();
}

static	void	_MAPINT	WriteB (int Bank, int Addr, int Val)
{
	switch (Addr & 3)
	{
	case 0:	Mapper.CHR[0].n0 = Val & 0xF;	break;
	case 1:	Mapper.CHR[1].n0 = Val & 0xF;	break;
	case 2:	Mapper.CHR[0].n1 = Val & 0xF;	break;
	case 3:	Mapper.CHR[1].n1 = Val & 0xF;	break;
	}
	Sync();
}

static	void	_MAPINT	WriteC (int Bank, int Addr, int Val)
{
	switch (Addr & 3)
	{
	case 0:	Mapper.CHR[2].n0 = Val & 0xF;	break;
	case 1:	Mapper.CHR[3].n0 = Val & 0xF;	break;
	case 2:	Mapper.CHR[2].n1 = Val & 0xF;	break;
	case 3:	Mapper.CHR[3].n1 = Val & 0xF;	break;
	}
	Sync();
}

static	void	_MAPINT	WriteD (int Bank, int Addr, int Val)
{
	switch (Addr & 3)
	{
	case 0:	Mapper.CHR[4].n0 = Val & 0xF;	break;
	case 1:	Mapper.CHR[5].n0 = Val & 0xF;	break;
	case 2:	Mapper.CHR[4].n1 = Val & 0xF;	break;
	case 3:	Mapper.CHR[5].n1 = Val & 0xF;	break;
	}
	Sync();
}

static	void	_MAPINT	WriteE (int Bank, int Addr, int Val)
{
	switch (Addr & 3)
	{
	case 0:	Mapper.CHR[6].n0 = Val & 0xF;	break;
	case 1:	Mapper.CHR[7].n0 = Val & 0xF;	break;
	case 2:	Mapper.CHR[6].n1 = Val & 0xF;	break;
	case 3:	Mapper.CHR[7].n1 = Val & 0xF;	break;
	}
	Sync();
}

static	void	_MAPINT	Load (void)
{
	iNES_SetSRAM();
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	u8 x;
	EMU->SetCPUWriteHandler(0x8,Write8);
	EMU->SetCPUWriteHandler(0x9,Write9);
	EMU->SetCPUWriteHandler(0xA,WriteA);
	EMU->SetCPUWriteHandler(0xB,WriteB);
	EMU->SetCPUWriteHandler(0xC,WriteC);
	EMU->SetCPUWriteHandler(0xD,WriteD);
	EMU->SetCPUWriteHandler(0xE,WriteE);

	if (ResetType == RESET_HARD)
	{
		Mapper.PRG[0] = 0;	Mapper.PRG[1] = 1;
		for (x = 0; x < 8; x++)
			Mapper.CHR[x].b0 = x;
		Mapper.Mirror = 0;
	}
	Sync();
}

static	u8 MapperNum = 22;
CTMapperInfo	MapperInfo_022 =
{
	&MapperNum,
	"Konami VRC2/VRC4",
	COMPAT_PARTIAL,
	Load,
	Reset,
	NULL,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};