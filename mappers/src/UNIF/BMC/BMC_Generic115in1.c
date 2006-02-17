#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_Latch.h"

static	struct
{
	u8 Regs[4];
}	Mapper;

static	void	Sync (void)
{
	union
	{
		struct
		{
			unsigned CHRbank : 6;
			unsigned PRG16   : 1;
			unsigned PRGbank : 5;
			unsigned PRGsize : 1;
			unsigned Mir_HV  : 1;
			unsigned PRGchip : 1;
			unsigned         : 1;
		};
		u16 addr;
	}	M;
	M.addr = Latch.Addr.s0;
	EMU->SetCHR_ROM8(0,M.CHRbank);
	if (M.PRGsize)
	{
		EMU->SetPRG_ROM16(0x8,(M.PRGchip << 6) | (M.PRGbank << 1) | (M.PRG16));
		EMU->SetPRG_ROM16(0xC,(M.PRGchip << 6) | (M.PRGbank << 1) | (M.PRG16));
	}
	else	EMU->SetPRG_ROM32(0x8,(M.PRGchip << 5) | M.PRGbank);
	if (M.Mir_HV)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	u8 i;
	x = Latch_SaveLoad_A(mode,x,data);
	for (i = 0; i < 4; i++)
		SAVELOAD_BYTE(mode,x,data,Mapper.Regs[i])
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	int	_MAPINT	ReadRegs (int Bank, int Where)
{
	if (Where & 0x800)
		return Mapper.Regs[Where & 3];
	else	return 0;
}

static	void	_MAPINT	WriteRegs (int Bank, int Where, int What)
{
	if (Where & 0x800)
		Mapper.Regs[Where & 3] = What & 0xF;
}

static	void	_MAPINT	Shutdown (void)
{
	Latch_Destroy();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	EMU->SetCPUReadHandler(0x5,ReadRegs);
	EMU->SetCPUWriteHandler(0x5,WriteRegs);
	Latch_Init(Sync,IsHardReset,FALSE);
	if (IsHardReset)
	{
		Mapper.Regs[0] = Mapper.Regs[2] = 0xF;
		Mapper.Regs[1] = Mapper.Regs[3] = 0x0;
	}
}

CTMapperInfo	MapperInfo_BMC_Generic115in1 =
{
	"BMC-Generic115in1",
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