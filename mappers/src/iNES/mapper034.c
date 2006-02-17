#include	"..\DLL\d_iNES.h"

static	struct
{
	u8 BNROM_PRG;
	u8 NINA_PRG;
	u8 NINA_CHR[2];
	u8 Mode;
	FCPUWrite Write7;
}	Mapper;

static	void	Sync (void)
{
	switch (Mapper.Mode)
	{
	case 0:	EMU->SetPRG_ROM32(0x8,-1);
		EMU->SetCHR_RAM8(0,0);
		break;
	case 1:	EMU->SetPRG_RAM8(0x6,0);
		EMU->SetPRG_ROM32(0x8,Mapper.NINA_PRG);
		EMU->SetCHR_ROM4(0,Mapper.NINA_CHR[0]);
		EMU->SetCHR_ROM4(4,Mapper.NINA_CHR[1]);
		break;
	case 2:	EMU->SetPRG_ROM32(0x8,Mapper.BNROM_PRG);
		EMU->SetCHR_RAM8(0,0);
		break;
	}
}

static	void	SetMode (int Mode)
{
	if (Mapper.Mode)
	{
		Sync();
		if (Mapper.Mode != Mode)
			EMU->DbgOut("WTF? Mapper 34 attempting to use both NINA-01 and NES-BNROM behaviour!");
		return;
	}
	Mapper.Mode = Mode;
	Sync();
#ifdef	DEBUG
	if (Mode == 1)
		EMU->StatusOut("Mapper 34 locked to NINA-01 subset");
	else if (Mode == 2)
		EMU->StatusOut("Mapper 34 locked to NES-BNROM subset");
#endif
}

static	int	_MAPINT	SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,Mapper.BNROM_PRG)
	SAVELOAD_BYTE(mode,x,data,Mapper.NINA_PRG)
	SAVELOAD_BYTE(mode,x,data,Mapper.NINA_CHR[0])
	SAVELOAD_BYTE(mode,x,data,Mapper.NINA_CHR[1])
	SAVELOAD_BYTE(mode,x,data,Mapper.Mode)
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

static	void	_MAPINT	WriteNINA (int Bank, int Where, int What)
{
	Mapper.Write7(Bank,Where,What);
	switch (Where)
	{
	case 0xFFD:	Mapper.NINA_PRG = What;
			SetMode(1);	break;
	case 0xFFE:	Mapper.NINA_CHR[0] = What;
			SetMode(1);	break;
	case 0xFFF:	Mapper.NINA_CHR[1] = What;
			SetMode(1);	break;
	}
}

static	void	_MAPINT	WriteBNROM (int Bank, int Where, int What)
{
	Mapper.BNROM_PRG = What;
	SetMode(2);
}

static	void	_MAPINT	Shutdown (void)
{
	iNES_UnloadROM();
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	u8 x;
	iNES_InitROM();

	Mapper.Write7 = EMU->GetCPUWriteHandler(0x7);
	EMU->SetCPUWriteHandler(0x7,WriteNINA);

	for (x = 0x8; x < 0x10; x++)
		EMU->SetCPUWriteHandler(x,WriteBNROM);

	Mapper.BNROM_PRG = 0;
	Mapper.NINA_PRG = 0;
	Mapper.NINA_CHR[0] = 0;
	Mapper.NINA_CHR[1] = 1;
	Mapper.Mode = 0;
	Sync();
}

CTMapperInfo	MapperInfo_034 =
{
	34,
	NULL,
	"BNROM/Nina-01",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	NULL
};