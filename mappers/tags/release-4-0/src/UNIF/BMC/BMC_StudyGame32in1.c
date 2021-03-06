#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_Latch.h"

static	void	Sync (void)
{
	EMU->SetCHR_RAM8(0,0);
	EMU->SetPRG_RAM8(0x6,0);
	EMU->SetPRG_ROM32(0x8,Latch.Data & 0x1F);
}

static	void	_MAPINT	Load (void)
{
	Latch_Load(Sync,FALSE);
}
static	void	_MAPINT	Reset (RESET_TYPE ResetType)
{
	UNIF_SetMirroring(NULL);
	Latch_Reset(ResetType);
}
static	void	_MAPINT	Unload (void)
{
	Latch_Unload();
}

CTMapperInfo	MapperInfo_BMC_StudyGame32in1 =
{
	"BMC-StudyGame32in1",
	"Pirate multicart mapper",
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	NULL,
	Latch_SaveLoad_D,
	NULL,
	NULL
};