/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"h_MMC5.h"
#include	"Sound\s_MMC5.h"

namespace MMC5
{
u8 WRAMsize;
u8 PRGsize, CHRsize;
u8 WRAMprot[2];
u8 GfxMode;
u8 Mirror;
u8 PRG[5];
u16_n CHR_A[8], CHR_B[4];
u8 CHRhi, CHRmode;
u8 SplitMode, SplitScroll, SplitBank;
u8 IRQline, IRQenabled, IRQreads;
u8 Mul1, Mul2;
s16 LineCounter;
u8 SpriteMode;
u8 *NameTable0, *NameTable1, *ExRAM, *ExNameTable;
FCPUWrite _WritePPU;
FCPUWrite _CPUWrite6F;
FPPURead _PPURead[16];

//#define	MMC5_EXTENDED_VSPLIT	/* equivalent to selecting 'SL' mode on a cartridge rather than 'CL' mode */

// should ideally be const, but SetCHR_Ptr1() requires it to be writable
unsigned char DisabledEXRAM[0x400] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const	unsigned char	AttribBits[4] = {0x00,0x55,0xAA,0xFF};
const	unsigned char	AttribLoc[256] =
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
	0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
	0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
	0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F
};
const	unsigned char	AttribShift[128] =
{
	0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,
	4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6
};

BOOL	InSplitArea = FALSE;
int	VScroll;
int	TileCache, CurTile;
const	int	WRAMtable[MMC5WRAM_MAXOPTS][8] = {
	{-1,-1,-1,-1,-1,-1,-1,-1},
	{ 0, 0, 0, 0,-1,-1,-1,-1},
	{ 0, 1, 2, 3,-1,-1,-1,-1},
	{-1,-1,-1,-1, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 1, 1, 1, 1},
	{ 0, 1, 2, 3, 4, 4, 4, 4},
	{-1,-1,-1,-1, 0, 1, 2, 3},
	{ 0, 0, 0, 0, 1, 2, 3, 4},
	{ 0, 1, 2, 3, 4, 5, 6, 7},
};

void	SetPPUHandlers (void);

void	IRQ_Assert (void)
{
	if (IRQenabled & IRQreads & 0x80)
		EMU->SetIRQ(0);
	else if (MMC5sound::HaveIRQ())
		EMU->SetIRQ(0);
	else	EMU->SetIRQ(1);
}

void	Load (int _WRAMsize)
{
	EMU->Mirror_4();
	NameTable0	= EMU->GetCHR_Ptr1(0x8);
	NameTable1	= EMU->GetCHR_Ptr1(0x9);
	ExRAM		= EMU->GetCHR_Ptr1(0xA);
	ExNameTable	= EMU->GetCHR_Ptr1(0xB);

	WRAMsize = _WRAMsize;
	MMC5sound::Load();
}

void	Reset (RESET_TYPE ResetType)
{
	u8 x;

	_WritePPU = EMU->GetCPUWriteHandler(0x2);

	EMU->SetCPUWriteHandler(0x2, WritePPU);
	EMU->SetCPUWriteHandler(0x3, WritePPU);

	EMU->SetCPUReadHandler(0x5, CPURead5);
	EMU->SetCPUWriteHandler(0x5, CPUWrite5);
	_CPUWrite6F = EMU->GetCPUWriteHandler(0x8);
	for (x = 6; x <= 0xF; x++)
		EMU->SetCPUWriteHandler(x, CPUWrite6F);
	for (x = 0; x < 16; x++)
		_PPURead[x] = EMU->GetPPUReadHandler(x);
	
	PRGsize = 3;
	CHRsize = 0;
	
	IRQenabled = IRQreads = IRQline = 0;
	Mul1 = Mul2 = GfxMode = 0;
	
	SplitMode = SplitBank = SplitScroll = 0;
	SetPPUHandlers();

	for (x = 0; x < 8; x++)	CHR_A[x].s0 = x;
	for (x = 0; x < 4; x++)	CHR_B[x].s0 = x;
	for (x = 0; x < 2; x++)	WRAMprot[x] = 0;
	CHRhi = 0;

	for (x = 0; x < 5; x++)	PRG[x] = 0xFF;

	TileCache = -1;
	MMC5sound::Reset(ResetType);
	SyncPRG();
	SyncCHR(0);
	SyncMirror();
}

void	Unload (void)
{
	MMC5sound::Unload();
}

void	SetRAMSize (int _WRAMsize)
{
	WRAMsize = _WRAMsize;
}

int	GetRAMSize (void)
{
	return WRAMsize;
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	u8 i;
	SAVELOAD_BYTE(mode, x, data, WRAMsize);
	SAVELOAD_BYTE(mode, x, data, PRGsize);
	SAVELOAD_BYTE(mode, x, data, CHRsize);
	for (i = 0; i < 2; i++)
		SAVELOAD_BYTE(mode, x, data, WRAMprot[i]);
	SAVELOAD_BYTE(mode, x, data, GfxMode);
	SAVELOAD_BYTE(mode, x, data, Mirror);
	for (i = 0; i < 5; i++)
		SAVELOAD_BYTE(mode, x, data, PRG[i]);
	for (i = 0; i < 8; i++)
		SAVELOAD_WORD(mode, x, data, CHR_A[i].s0);
	for (i = 0; i < 4; i++)
		SAVELOAD_WORD(mode, x, data, CHR_B[i].s0);
	SAVELOAD_BYTE(mode, x, data, CHRhi);
	SAVELOAD_BYTE(mode, x, data, CHRmode);
	SAVELOAD_BYTE(mode, x, data, SplitMode);
	SAVELOAD_BYTE(mode, x, data, SplitScroll);
	SAVELOAD_BYTE(mode, x, data, SplitBank);
	SAVELOAD_BYTE(mode, x, data, IRQline);
	SAVELOAD_BYTE(mode, x, data, IRQenabled);
	SAVELOAD_BYTE(mode, x, data, IRQreads);
	SAVELOAD_BYTE(mode, x, data, Mul1);
	SAVELOAD_BYTE(mode, x, data, Mul2);
	SAVELOAD_WORD(mode, x, data, LineCounter);
	SAVELOAD_BYTE(mode, x, data, SpriteMode);
	x = MMC5sound::SaveLoad(mode, x, data);
	if (mode == STATE_LOAD)
	{
		SyncPRG();
		SyncCHR(-1);
		SyncMirror();
		SetPPUHandlers();
	}
	return x;
}

void	SetPRG (int Size, int Loc, int Bank)
{
	u8 x;
	if (Bank & 0x80)
	{
		if (Size == 8)		EMU->SetPRG_ROM8(Loc, Bank);
		else if (Size == 16)	EMU->SetPRG_ROM16(Loc, Bank >> 1);
		else if (Size == 32)	EMU->SetPRG_ROM32(Loc, Bank >> 2);
	}
	else
	{
		if (Size == 8)
		{
			if (WRAMtable[WRAMsize][Bank & 0x7] == -1)
				for (x = Loc; x < Loc + 2; x++)
					EMU->SetPRG_OB4(x);
			else	EMU->SetPRG_RAM8(Loc, WRAMtable[WRAMsize][Bank & 0x7]);
		}
		else if (Size == 16)
		{
			if (WRAMtable[WRAMsize][Bank & 0x6] == -1)
				for (x = Loc; x < Loc + 4; x++)
					EMU->SetPRG_OB4(x);
			else
			{
				EMU->SetPRG_RAM8(Loc + 0, WRAMtable[WRAMsize][(Bank & 0x6) | 0]);
				EMU->SetPRG_RAM8(Loc + 2, WRAMtable[WRAMsize][(Bank & 0x6) | 1]);
			}
		}
	}
}

void	SyncPRG (void)
{
		SetPRG( 8, 0x6, PRG[0] & 0x7F);
	switch (PRGsize)
	{
	case 0:	SetPRG(32, 0x8, PRG[4] | 0x80);	break;
	case 1:	SetPRG(16, 0x8, PRG[2]       );
		SetPRG(16, 0xC, PRG[4] | 0x80);	break;
	case 2:	SetPRG(16, 0x8, PRG[2]       );
		SetPRG( 8, 0xC, PRG[3]       );
		SetPRG( 8, 0xE, PRG[4] | 0x80);	break;
	case 3:	SetPRG( 8, 0x8, PRG[1]       );
		SetPRG( 8, 0xA, PRG[2]       );
		SetPRG( 8, 0xC, PRG[3]       );
		SetPRG( 8, 0xE, PRG[4] | 0x80);	break;
	}
}

void	SyncCHRA (void)
{
	switch (CHRsize)
	{
	case 0:	EMU->SetCHR_ROM8(0, CHR_A[7].s0);	break;
	case 1:	EMU->SetCHR_ROM4(0, CHR_A[3].s0);
		EMU->SetCHR_ROM4(4, CHR_A[7].s0);	break;
	case 2:	EMU->SetCHR_ROM2(0, CHR_A[1].s0);
		EMU->SetCHR_ROM2(2, CHR_A[3].s0);
		EMU->SetCHR_ROM2(4, CHR_A[5].s0);
		EMU->SetCHR_ROM2(6, CHR_A[7].s0);	break;
	case 3:	EMU->SetCHR_ROM1(0, CHR_A[0].s0);
		EMU->SetCHR_ROM1(1, CHR_A[1].s0);
		EMU->SetCHR_ROM1(2, CHR_A[2].s0);
		EMU->SetCHR_ROM1(3, CHR_A[3].s0);
		EMU->SetCHR_ROM1(4, CHR_A[4].s0);
		EMU->SetCHR_ROM1(5, CHR_A[5].s0);
		EMU->SetCHR_ROM1(6, CHR_A[6].s0);
		EMU->SetCHR_ROM1(7, CHR_A[7].s0);	break;
	}
}

void	SyncCHRB (void)
{
	switch (CHRsize)
	{
	case 0:	EMU->SetCHR_ROM8(0, CHR_B[3].s0);	break;
	case 1:	EMU->SetCHR_ROM4(0, CHR_B[3].s0);
		EMU->SetCHR_ROM4(4, CHR_B[3].s0);	break;
	case 2:	EMU->SetCHR_ROM2(0, CHR_B[1].s0);
		EMU->SetCHR_ROM2(2, CHR_B[3].s0);
		EMU->SetCHR_ROM2(4, CHR_B[1].s0);
		EMU->SetCHR_ROM2(6, CHR_B[3].s0);	break;
	case 3:	EMU->SetCHR_ROM1(0, CHR_B[0].s0);
		EMU->SetCHR_ROM1(1, CHR_B[1].s0);
		EMU->SetCHR_ROM1(2, CHR_B[2].s0);
		EMU->SetCHR_ROM1(3, CHR_B[3].s0);
		EMU->SetCHR_ROM1(4, CHR_B[0].s0);
		EMU->SetCHR_ROM1(5, CHR_B[1].s0);
		EMU->SetCHR_ROM1(6, CHR_B[2].s0);
		EMU->SetCHR_ROM1(7, CHR_B[3].s0);	break;
	}
}

void	SyncCHR (int mode)
{
	if ((TileCache != -1) || (InSplitArea))
		return;
	if ((SpriteMode) && (IRQreads & 0x01))
	{
		switch (mode)
		{
		case 0:	SyncCHRA();	break;
		case 1:	SyncCHRB();	break;
		}
	}
	else
	{
		if (CHRmode)
			SyncCHRB();
		else	SyncCHRA();
	}
}

void	SyncMirror (void)
{
	u8 mirror = Mirror, i;
	for (i = 0; i < 4; i++)
	{
		if ((mirror & 3) == 3)
		{
			EMU->SetCHR_Ptr1(0x8 | i, ExNameTable, FALSE);
			EMU->SetCHR_Ptr1(0xC | i, ExNameTable, FALSE);
		}
		else if (((mirror & 3) == 2) && (GfxMode & 0x2))
		{
			EMU->SetCHR_Ptr1(0x8 | i, DisabledEXRAM, FALSE);
			EMU->SetCHR_Ptr1(0xC | i, DisabledEXRAM, FALSE);
		}
		else
		{
			EMU->SetCHR_NT1(0x8 | i, mirror & 3);
			EMU->SetCHR_NT1(0xC | i, mirror & 3);
		}
		mirror >>= 2;
	}
}

void	MAPINT	WritePPU (int Bank, int Addr, int Val)
{
	_WritePPU(Bank, Addr, Val);
	switch (Addr & 7)
	{
	case 0:	SpriteMode = (Val & 0x20);	break;
	case 1:	if (!(Val & 0x18))
		{
			LineCounter = -2;
			IRQreads &= 0x80;
		}				break;
	}
}

int	MAPINT	CPURead5 (int Bank, int Addr)
{
	register int read = -1;
	switch (Addr & 0xF00)
	{
	case 0x000:
		read = MMC5sound::Read((Bank << 12) | Addr);		break;
	case 0x200:
		switch (Addr)
		{
		case 0x204:	read = IRQreads & 0xC0;
				IRQreads &= ~0x80;
				IRQ_Assert();				break;
		case 0x205:	read = ((Mul1 * Mul2) & 0x00FF) >> 0;	break;
		case 0x206:	read = ((Mul1 * Mul2) & 0xFF00) >> 8;	break;
		}							break;
	case 0xC00:
	case 0xD00:
	case 0xE00:
	case 0xF00:	if (GfxMode >= 2)
				read = ExRAM[Addr & 0x3FF];	break;
	}
	return read;
}
void	MAPINT	CPUWrite5 (int Bank, int Addr, int Val)
{
	switch (Addr & 0xF00)
	{
	case 0x000:	MMC5sound::Write((Bank << 12) | Addr, Val);	break;
	case 0x100:
		switch (Addr)
		{
		case 0x100:	PRGsize = Val & 3;
				SyncPRG();		break;
		case 0x101:	CHRsize = Val & 3;
				SyncCHR(-1);	break;
		case 0x102:	WRAMprot[0] = Val & 3;
							break;
		case 0x103:	WRAMprot[1] = Val & 3;
							break;
		case 0x104:	GfxMode = Val & 3;
				SetPPUHandlers();
				SyncMirror();	break;
		case 0x105:	Mirror = Val;
				SyncMirror();	break;
		case 0x106:	memset(ExNameTable, Val, 0x3C0);
							break;
		case 0x107:	memset(ExNameTable + 0x3C0, AttribBits[Val & 3], 0x40);
							break;
		case 0x113:
		case 0x114:
		case 0x115:
		case 0x116:
		case 0x117:	PRG[Addr - 0x113] = Val;
				SyncPRG();		break;
		case 0x120:
		case 0x121:
		case 0x122:
		case 0x123:
		case 0x124:
		case 0x125:
		case 0x126:
		case 0x127:	CHR_A[Addr & 0x7].b0 = Val;
				CHR_A[Addr & 0x7].b1 = CHRhi;
				CHRmode = 0;
				SyncCHR(-1);	break;
		case 0x128:
		case 0x129:
		case 0x12A:
		case 0x12B:	CHR_B[Addr & 0x3].b0 = Val;
				CHR_B[Addr & 0x3].b1 = CHRhi;
				CHRmode = 1;
				SyncCHR(-1);	break;
		case 0x130:	CHRhi = Val & 3;	break;
		}		break;
	case 0x200:
		switch (Addr)
		{
		case 0x200:	SplitMode = Val;
				SetPPUHandlers();	break;
		case 0x201:	SplitScroll = Val;	break;
		case 0x202:	SplitBank = Val;
				if (InSplitArea)
				{
					EMU->SetCHR_ROM4(0, SplitBank);
					EMU->SetCHR_ROM4(4, SplitBank);
				}			break;
		case 0x203:	IRQline = Val;	break;
		case 0x204:	IRQenabled = Val & 0x80;
				IRQ_Assert();
				break;
		case 0x205:	Mul1 = Val;	break;
		case 0x206:	Mul2 = Val;	break;
		}		break;
	case 0xC00:
	case 0xD00:
	case 0xE00:
	case 0xF00:	if (GfxMode != 3)
			{
				if ((GfxMode == 2) || (IRQreads & 0x40))
					ExRAM[Addr & 0x3FF] = Val;
				else	ExRAM[Addr & 0x3FF] = 0;
			}
							break;
	}
}

void	MAPINT	CPUWrite6F (int Bank, int Addr, int Val)
{
	if ((WRAMprot[0] == 2) && (WRAMprot[1] == 1))
		_CPUWrite6F(Bank, Addr, Val);
}

#ifdef	MMC5_EXTENDED_VSPLIT
int	MAPINT	PPUReadPT (int Bank, int Addr)
{
	if (!InSplitArea)
		return _PPURead[Bank](Bank, Addr);
	else	return _PPURead[Bank](Bank, (Addr & ~7) | (VScroll & 7));
}
#endif

static int extile = 0;
int	MAPINT	PPUReadNTSplitExt (int Bank, int Addr)
{
	if (CurTile >= 34)				// we only want to deal with background data
		return _PPURead[Bank](Bank, Addr);	// sprite fetches can go through
	if (InSplitArea)
	{
		static int splittile;
		if (Addr < 0x3C0)
		{	// custom nametable data
			splittile = ((VScroll & 0xF8) << 2) | (CurTile & 0x1F);
			return ExRAM[splittile];
		}	// custom attribute data
		else	return AttribBits[(ExRAM[0x3C0 | AttribLoc[splittile >> 2]] >> AttribShift[splittile & 0x7F]) & 3];
	}
	else
	{
		if (Addr < 0x3C0)
		{
			extile = Addr;
			return _PPURead[Bank](Bank, Addr);		// normal nametable data
		}
		else
		{
			if (TileCache != (ExRAM[extile] & 0x3F))
			{
				TileCache = (ExRAM[extile] & 0x3F) | (CHRhi << 6);
				EMU->SetCHR_ROM4(0, TileCache);
				EMU->SetCHR_ROM4(4, TileCache);
			}
			return AttribBits[ExRAM[extile] >> 6];	// custom attribute data
		}
	}
}
int	MAPINT	PPUReadNTSplit (int Bank, int Addr)
{
	if (CurTile >= 34)				// we only want to deal with background data
		return _PPURead[Bank](Bank, Addr);	// sprite fetches can go through
	if (InSplitArea)
	{
		static int splittile;
		if (Addr < 0x3C0)
		{	// custom nametable data
			splittile = ((VScroll & 0xF8) << 2) | (CurTile & 0x1F);
			return ExRAM[splittile];
		}	// custom attribute data
		else	return AttribBits[(ExRAM[0x3C0 | AttribLoc[splittile >> 2]] >> AttribShift[splittile & 0x7F]) & 3];
	}
	else	return _PPURead[Bank](Bank, Addr);	// weren't in split area, let it through
}
int	MAPINT	PPUReadNTExt (int Bank, int Addr)
{
	if (CurTile >= 34)				// we only want to deal with background data
		return _PPURead[Bank](Bank, Addr);	// sprite fetches can go through
	if (Addr < 0x3C0)
	{
		extile = Addr;
		if (TileCache != (ExRAM[extile] & 0x3F))
		{
			TileCache = (ExRAM[extile] & 0x3F) | (CHRhi << 6);
			EMU->SetCHR_ROM4(0, TileCache);
			EMU->SetCHR_ROM4(4, TileCache);
		}
		return _PPURead[Bank](Bank, Addr);		// normal nametable data
	}
	else	return AttribBits[ExRAM[extile] >> 6];	// custom attribute data
}

void	SetPPUHandlers (void)
{
	int x;
	if ((SplitMode & 0x80) && (GfxMode < 2))	// split mode
	{
#ifdef	MMC5_EXTENDED_VSPLIT
		for (x = 0; x < 8; x++)
			EMU->SetPPUReadHandler(x, PPUReadPT);
#endif
		if (GfxMode == 1)	
			for (x = 8; x < 0x10; x++)		// + exgfx
				EMU->SetPPUReadHandler(x, PPUReadNTSplitExt);
		else
			for (x = 8; x < 0x10; x++)		// split only
				EMU->SetPPUReadHandler(x, PPUReadNTSplit);
	}
	else if (GfxMode == 1)				// exgfx only
	{
#ifdef	MMC5_EXTENDED_VSPLIT
		for (x = 0; x < 8; x++)
			EMU->SetPPUReadHandler(x, _PPURead[x]);
#endif
		for (x = 8; x < 0x10; x++)
			EMU->SetPPUReadHandler(x, PPUReadNTExt);
	}
	else							// normal
	{
#ifdef	MMC5_EXTENDED_VSPLIT
		for (x = 0; x < 8; x++)
			EMU->SetPPUReadHandler(x, _PPURead[x]);
#endif
		for (x = 8; x < 0x10; x++)
			EMU->SetPPUReadHandler(x, _PPURead[x]);
	}
}

void	MAPINT	PPUCycle (int Addr, int Scanline, int Cycle, int IsRendering)
{
	if ((Scanline == 240) && (Cycle == 0))
	{
		InSplitArea = FALSE;
		TileCache = -1;
		LineCounter = -2;
		IRQreads &= 0x80;
		SyncCHR(-1);
	}
	if (!IsRendering)
		return;
	if (Cycle == 0)
	{
		if (Scanline == -1)
			IRQreads |= 0x01;
		if (Scanline == 0)
			IRQreads &= ~0x80;
		if (Scanline == 1)
			IRQreads |= 0x40;
		LineCounter++;
		if ((IRQline) && (LineCounter == IRQline))
			IRQreads |= 0x80;
		IRQ_Assert();
	}
	if (Cycle == 256)
	{
		TileCache = -1;
		SyncCHR(0);
	}
	else if (Cycle == 320)
	{
		CurTile = -1;
		if (Scanline == -1)
		{
			VScroll = SplitScroll;
			if (VScroll >= 240)
				VScroll -= 16;
		}
		else if (Scanline < 240)
			VScroll++;
		if (VScroll >= 240)
			VScroll -= 240;
		SyncCHR(1);
	}
	if ((!(Cycle & 7)) && (Cycle < 336))
	{
		CurTile++;
		if ((SplitMode & 0x80) && (GfxMode < 2))
		{
			if (SplitMode & 0x40)
			{
				if (CurTile == 0)
					InSplitArea = FALSE;
				else if (CurTile == (SplitMode & 0x1F))
				{
					InSplitArea = TRUE;
					EMU->SetCHR_ROM4(0, SplitBank);
					EMU->SetCHR_ROM4(4, SplitBank);
				}
				else if (CurTile == 34)
				{
					InSplitArea = FALSE;
					SyncCHR(0);
				}
			}
			else
			{
				if (CurTile == 0)
				{
					InSplitArea = TRUE;
					EMU->SetCHR_ROM4(0, SplitBank);
					EMU->SetCHR_ROM4(4, SplitBank);
				}
				else if (CurTile == (SplitMode & 0x1F))
				{
					InSplitArea = FALSE;
					if (GfxMode == 1)
						TileCache = -1;
					else	SyncCHR(1);
				}
			}
		}
		else	InSplitArea = FALSE;
	}
}

int	MAPINT	MapperSnd (int Cycles)
{
	int result = MMC5sound::Get(Cycles);
	IRQ_Assert();
	return result;
}
} // namespace MMC5