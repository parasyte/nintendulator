#include	"..\DLL\d_iNES.h"

static	struct
{
	FCPURead Read;
	u8 Reg1, Reg2, Reg3;
}	Mapper;

static	void	Sync (void)
{
	if (Mapper.Reg1 & 0x80)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
	if (Mapper.Reg1 & 0x40)
	{
		EMU->SetPRG_ROM32(0x8,(Mapper.Reg1 & 0x0E) | (Mapper.Reg2 & 0x01));
		EMU->SetCHR_ROM8(0,((Mapper.Reg1 & 0x0E) << 2) | ((Mapper.Reg2 & 0x70) >> 4));
	}
	else
	{
		EMU->SetPRG_ROM32(0x8,Mapper.Reg1 & 0x0F);
		EMU->SetCHR_ROM8(0,((Mapper.Reg1 & 0x0F) << 2) | ((Mapper.Reg2 & 0x30) >> 4));
	}
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg1)
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg2)
	SAVELOAD_BYTE(mode,x,data,Mapper.Reg3)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	Write (int Bank, int Where, int What)
{
	switch (Where & 0xFF8)
	{
	case 0xF80:
	case 0xF88:
	case 0xF90:
	case 0xF98:	if (Mapper.Reg1)	break;
				Mapper.Reg1 = What;
			Sync();			break;
	case 0xFA0:
	case 0xFA8:
	case 0xFB0:
	case 0xFB8:	break;
	case 0xFC0:
	case 0xFC8:
	case 0xFD0:
	case 0xFD8:	if (Mapper.Reg3)	break;
				Mapper.Reg3 = What;
			Sync();			break;
	case 0xFE0:	break;
	case 0xFE8:
	case 0xFF0:	Mapper.Reg2 = What;
			Sync();			break;
	case 0xFF8:	break;
	}
}

static	int	_MAPINT	Read (int Bank, int Where)
{
	int What = Mapper.Read(Bank,Where);
	if ((Where & 0xF80) == 0xF80)
		Write(Bank,Where,What);
	return What;
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	iNES_InitROM();

	Mapper.Read = EMU->GetCPUReadHandler(0xF);
	EMU->SetCPUReadHandler(0xF,Read);
	EMU->SetCPUWriteHandler(0xF,Write);

	Mapper.Reg1 = Mapper.Reg2 = Mapper.Reg3 = 0;
}

CTMapperInfo	MapperInfo_234 =
{
	234,
	NULL,
	"Maxi 15",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};