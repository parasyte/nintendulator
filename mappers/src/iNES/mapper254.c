#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 IRQenabled;
	u16_n IRQcounter;
	u8 PRG;
	u8 CHR;
	u8 Mirror;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetPRG_ROM8(0x6,Mapper.PRG);
	EMU->SetPRG_ROM32(0x8,3);
	if (Mapper.Mirror)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
	EMU->SetCHR_ROM8(0,Mapper.CHR);
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.IRQenabled)
	SAVELOAD_WORD(mode,x,data,Mapper.IRQcounter.s0)
	SAVELOAD_BYTE(mode,x,data,Mapper.PRG)
	SAVELOAD_BYTE(mode,x,data,Mapper.CHR)
	SAVELOAD_BYTE(mode,x,data,Mapper.Mirror)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	CPUCycle (void)
{
	if (Mapper.IRQenabled)
	{
		Mapper.IRQcounter.s0++;
		if (Mapper.IRQcounter.s0 >= 24576)
			EMU->SetIRQ(0);
	}
}

static	void	_MAPINT	Write8 (int Bank, int Where, int What)
{
	Mapper.CHR = What;
	Sync();
}

static	void	_MAPINT	WriteEF (int Bank, int Where, int What)
{
	switch (Where & 0x3)
	{
	case 0:	Mapper.PRG = What & 0x0F;
		Sync();				break;
	case 1:	Mapper.Mirror = What & 0x8;
		Sync();				break;
	case 2:	if (What & 0x02)
			Mapper.IRQenabled = 1;
		else
		{
			Mapper.IRQenabled = 0;
			Mapper.IRQcounter.s0 = 0;
			EMU->SetIRQ(1);
		}				break;
	case 3:					break;
	}
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
	for (x = 0xE; x < 0x10; x++)
		EMU->SetCPUWriteHandler(x,WriteEF);

	Mapper.PRG = Mapper.Mirror = 0;
	Mapper.IRQenabled = 0;
	Mapper.IRQcounter.s0 = 0;

	Sync();
}

CTMapperInfo	MapperInfo_254 =
{
	254,
	NULL,
	"Ai Senshi Nicol (Pirate)",
	COMPAT_FULL,
	Reset,
	Shutdown,
	CPUCycle,
	NULL,
	SaveLoad,
	NULL,
	NULL
};