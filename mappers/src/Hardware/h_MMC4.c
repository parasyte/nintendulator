#include	"h_MMC4.h"

static	TMMC4	MMC4;

void	MMC4_Init (void (*Sync)(void))
{
	u8 x;
	for (x = 0; x < 2; x++)
	{
		MMC4.LatchState[x] = 0xFE;
		MMC4.Latch0[x] = 0;
		MMC4.Latch1[x] = 0;
	}
	EMU->SetCPUWriteHandler(0xA,MMC4_CPUWriteA);
	EMU->SetCPUWriteHandler(0xB,MMC4_CPUWriteB);
	EMU->SetCPUWriteHandler(0xC,MMC4_CPUWriteC);
	EMU->SetCPUWriteHandler(0xD,MMC4_CPUWriteD);
	EMU->SetCPUWriteHandler(0xE,MMC4_CPUWriteE);
	EMU->SetCPUWriteHandler(0xF,MMC4_CPUWriteF);
	MMC4.PPURead3 = EMU->GetPPUReadHandler(0x3);
	EMU->SetPPUReadHandler(0x3,MMC4_PPURead3);
	MMC4.PPURead7 = EMU->GetPPUReadHandler(0x7);
	EMU->SetPPUReadHandler(0x7,MMC4_PPURead7);
	(MMC4.Sync = Sync)();
}

void	MMC4_Destroy (void)
{
}

int	_MAPINT	MMC4_SaveLoad (int mode, int x, char *data)
{
	SAVELOAD_BYTE(mode,x,data,MMC4.Latch0[0])
	SAVELOAD_BYTE(mode,x,data,MMC4.Latch0[1])
	SAVELOAD_BYTE(mode,x,data,MMC4.Latch1[0])
	SAVELOAD_BYTE(mode,x,data,MMC4.Latch1[1])
	SAVELOAD_BYTE(mode,x,data,MMC4.LatchState[0])
	SAVELOAD_BYTE(mode,x,data,MMC4.LatchState[1])
	if (mode == STATE_LOAD)
		MMC4.Sync();
	return x;
}

void	MMC4_SyncPRG (int AND, int OR)
{
	EMU->SetPRG_ROM16(0x8,(MMC4.PRG & AND) | OR);
	EMU->SetPRG_ROM16(0xC,(0xFF & AND) | OR);
}

void	MMC4_SyncCHR (void)
{
	if (MMC4.LatchState[0])
		EMU->SetCHR_ROM4(0,MMC4.Latch0[1]);
	else 	EMU->SetCHR_ROM4(0,MMC4.Latch0[0]);
	if (MMC4.LatchState[1])
		EMU->SetCHR_ROM4(4,MMC4.Latch1[1]);
	else 	EMU->SetCHR_ROM4(4,MMC4.Latch1[0]);
}

void	MMC4_SyncMirror (void)
{
	if (MMC4.Mirror & 1)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
}

int	_MAPINT	MMC4_PPURead3 (int Bank, int Addr)
{
	int result = MMC4.PPURead3(Bank,Addr);
	register int addy = Addr & 0x3F8;
	if (addy == 0x3D8)
	{
		MMC4.LatchState[0] = 0;
		EMU->SetCHR_ROM4(0,MMC4.Latch0[0]);
	}
	if (addy == 0x3E8)
	{
		MMC4.LatchState[0] = 1;
		EMU->SetCHR_ROM4(0,MMC4.Latch0[1]);
	}
	return result;
}

int	_MAPINT	MMC4_PPURead7 (int Bank, int Addr)
{
	int result = MMC4.PPURead7(Bank,Addr);
	register int addy = Addr & 0x3F8;
	if (addy == 0x3D8)
	{
		MMC4.LatchState[1] = 0;
		EMU->SetCHR_ROM4(4,MMC4.Latch1[0]);
	}
	if (addy == 0x3E8)
	{
		MMC4.LatchState[1] = 1;
		EMU->SetCHR_ROM4(4,MMC4.Latch1[1]);
	}
	return result;
}

void	_MAPINT	MMC4_CPUWriteA (int Bank, int Where, int What)
{
	MMC4.PRG = What;
	MMC4.Sync();
}

void	_MAPINT	MMC4_CPUWriteB (int Bank, int Where, int What)
{
	MMC4.Latch0[0] = What;
	MMC4.Sync();
}

void	_MAPINT	MMC4_CPUWriteC (int Bank, int Where, int What)
{
	MMC4.Latch0[1] = What;
	MMC4.Sync();
}

void	_MAPINT	MMC4_CPUWriteD (int Bank, int Where, int What)
{
	MMC4.Latch1[0] = What;
	MMC4.Sync();
}

void	_MAPINT	MMC4_CPUWriteE (int Bank, int Where, int What)
{
	MMC4.Latch1[1] = What;
	MMC4.Sync();
}

void	_MAPINT	MMC4_CPUWriteF (int Bank, int Where, int What)
{
	MMC4.Mirror = What;
	MMC4.Sync();
}