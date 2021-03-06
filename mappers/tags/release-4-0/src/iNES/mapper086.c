#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 Reg;
}	Mapper;

static	void	Sync (void)
{
	EMU->SetPRG_ROM32(0x8,(Mapper.Reg & 0x30) >> 4);
	EMU->SetCHR_ROM8(0,(Mapper.Reg & 0x03) | ((Mapper.Reg & 0x40) >> 4));
}

static	int	_MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	WriteBank (int Bank, int Addr, int Val)
{
	Mapper.Reg = Val;
	Sync();
}

static	void	_MAPINT	WriteSpeech (int Bank, int Addr, int Val)
{
	if ((Val & 0x30) != 0x20)
		return;
	switch (Val & 0xF)
	{
	case 0x0:EMU->StatusOut("\"Strike!\"");	break;
	case 0x1:EMU->StatusOut("\"Ball!\"");	break;
	case 0x2:EMU->StatusOut("\"Out!\"");	break;
	case 0x3:EMU->StatusOut("'?' (3)");	break;
	case 0x4:EMU->StatusOut("'?' (4)");	break;
	case 0x5:EMU->StatusOut("\"Foul!\"");	break;
	case 0x6:EMU->StatusOut("'?' (6)");	break;
	case 0x7:EMU->StatusOut("\"You're out!\"");	break;
	case 0x8:EMU->StatusOut("\"Play ball!\"");	break;
	case 0x9:EMU->StatusOut("\"Ball 4!\"");	break;
	case 0xA:EMU->StatusOut("'?' (A)");	break;
	case 0xB:EMU->StatusOut("'?' (B)");	break;
	case 0xC:EMU->StatusOut("'?' (C)");	break;
	case 0xD:EMU->StatusOut("'?' (D)");	break;
	case 0xE:EMU->StatusOut("*crack*");	break;
	case 0xF:EMU->StatusOut("*cheer*");	break;
	}
}

static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	iNES_SetMirroring();

	EMU->SetCPUWriteHandler(0x6,WriteBank);
	EMU->SetCPUWriteHandler(0x7,WriteSpeech);

	if (ResetType == RESET_HARD)
		Mapper.Reg = 0;

	Sync();
}

static	u8 MapperNum = 86;
CTMapperInfo	MapperInfo_086 =
{
	&MapperNum,
	"Mapper 86 (Jaleco)",
	COMPAT_FULL,
	NULL,
	Reset,
	NULL,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};