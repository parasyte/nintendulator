#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 PRG;
	u8 CHR[4];
	u8 Mirror;
	u16_n IRQcounter;
	u8 IRQenabled;
	u8 IRQpos;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetPRG_ROM16(0x8,Mapper.PRG);
	EMU->SetPRG_ROM16(0xC,-1);

	EMU->SetCHR_ROM2(0,Mapper.CHR[0]);
	EMU->SetCHR_ROM2(2,Mapper.CHR[1]);
	EMU->SetCHR_ROM2(4,Mapper.CHR[2]);
	EMU->SetCHR_ROM2(6,Mapper.CHR[3]);

	switch (Mapper.Mirror & 3)
	{
	case 0:	EMU->Mirror_V();	break;
	case 1:	EMU->Mirror_H();	break;
	case 2:	EMU->Mirror_S0();	break;
	case 3:	EMU->Mirror_S1();	break;
	}
}

static	int	_MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	u8 i;
	SAVELOAD_WORD(mode,x,data,Mapper.IRQcounter.s0)
	SAVELOAD_BYTE(mode,x,data,Mapper.IRQenabled)
	SAVELOAD_BYTE(mode,x,data,Mapper.IRQpos)
	SAVELOAD_BYTE(mode,x,data,Mapper.PRG)
	for (i = 0; i < 4; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.CHR[i])
	SAVELOAD_BYTE(mode,x,data,Mapper.Mirror)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	CPUCycle (void)
{
	if (Mapper.IRQenabled)
	{
		if (!Mapper.IRQcounter.s0)
		{
			Mapper.IRQenabled = 0;
			Mapper.IRQcounter.s0 = 0xFFFF;
			EMU->SetIRQ(0);
		}
		else	Mapper.IRQcounter.s0--;
	}
}

static	void	_MAPINT	Write8 (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.CHR[0] = Val;
		Sync();
	}
}

static	void	_MAPINT	Write9 (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.CHR[1] = Val;
		Sync();
	}
}

static	void	_MAPINT	WriteA (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.CHR[2] = Val;
		Sync();
	}
}

static	void	_MAPINT	WriteB (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.CHR[3] = Val;
		Sync();
	}
}

static	void	_MAPINT	WriteC (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		if (Mapper.IRQpos)
			Mapper.IRQcounter.b0 = Val;
		else	Mapper.IRQcounter.b1 = Val;
		Mapper.IRQpos ^= 1;
	}
}

static	void	_MAPINT	WriteD (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.IRQpos = 0;
		Mapper.IRQenabled = (Val & 0x10) >> 4;
		EMU->SetIRQ(1);
	}
}

static	void	_MAPINT	WriteE (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.Mirror = Val;
		Sync();
	}
}

static	void	_MAPINT	WriteF (int Bank, int Addr, int Val)
{
	if (Addr & 0x800)
	{
		Mapper.PRG = Val;
		Sync();
	}
}

static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	EMU->SetCPUWriteHandler(0x8,Write8);
	EMU->SetCPUWriteHandler(0x9,Write9);
	EMU->SetCPUWriteHandler(0xA,WriteA);
	EMU->SetCPUWriteHandler(0xB,WriteB);
	EMU->SetCPUWriteHandler(0xC,WriteC);
	EMU->SetCPUWriteHandler(0xD,WriteD);
	EMU->SetCPUWriteHandler(0xE,WriteE);
	EMU->SetCPUWriteHandler(0xF,WriteF);

	if (ResetType == RESET_HARD)
	{
		Mapper.PRG = 0;
		Mapper.CHR[0] = 0;	Mapper.CHR[1] = 0;	Mapper.CHR[2] = 0;	Mapper.CHR[3] = 0;
		Mapper.Mirror = 0;
		Mapper.IRQcounter.s0 = 0;
		Mapper.IRQenabled = Mapper.IRQpos = 0;
	}

	Sync();
}

static	u8 MapperNum = 67;
CTMapperInfo	MapperInfo_067 =
{
	&MapperNum,
	_T("Sunsoft Mapper #3"),
	COMPAT_FULL,
	NULL,
	Reset,
	NULL,
	CPUCycle,
	NULL,
	SaveLoad,
	NULL,
	NULL
};
