#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_Latch.h"

static	void	Sync (void)
{
	EMU->SetCHR_ROM8(0,Latch.Addr.b0 & 0x07);
	if (Latch.Addr.b0 & 0x40)
	{
		EMU->SetPRG_ROM16(0x8,(Latch.Addr.b0 & 0x38) >> 3);
		EMU->SetPRG_ROM16(0xC,(Latch.Addr.b0 & 0x38) >> 3);
	}
	else	EMU->SetPRG_ROM32(0x8,(Latch.Addr.b0 & 0x30) >> 4);
	if (Latch.Addr.b0 & 0x80)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
}

static	void	_MAPINT	Load (void)
{
	Latch_Load(Sync,FALSE);
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	Latch_Reset(ResetType);
}
static	void	_MAPINT	Unload (void)
{
	Latch_Unload();
}

CTMapperInfo	MapperInfo_BMC_SV01 =
{
	"BMC-SV01",
	"Pirate multicart mapper",
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	NULL,
	Latch_SaveLoad_AL,
	NULL,
	NULL
};