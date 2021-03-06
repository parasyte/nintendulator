/*
Nintendulator - A Win32 NES emulator written in C.
Designed for maximum emulation accuracy.
Copyright (c) 2002-2006  QMT Productions

Based on NinthStar, a portable Win32 NES Emulator written in C++
Copyright (C) 2000  David de Regt

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

For a copy of the GNU General Public License, go to:
http://www.gnu.org/copyleft/gpl.html#SEC1
*/

#include "stdafx.h"
#include "Nintendulator.h"
#include "MapperInterface.h"
#include "NES.h"
#include "PPU.h"
#include "CPU.h"
#include "GFX.h"
#include "Debugger.h"

struct	tPPU	PPU;
unsigned char	PPU_VRAM[0x4][0x400];
unsigned short	DrawArray[256*240];

/*#define	SHORQ	/* Enable ShoRQ(tm) technology */

const	unsigned char	ReverseCHR[256] =
{
	0x00,0x80,0x40,0xC0,0x20,0xA0,0x60,0xE0,0x10,0x90,0x50,0xD0,0x30,0xB0,0x70,0xF0,
	0x08,0x88,0x48,0xC8,0x28,0xA8,0x68,0xE8,0x18,0x98,0x58,0xD8,0x38,0xB8,0x78,0xF8,
	0x04,0x84,0x44,0xC4,0x24,0xA4,0x64,0xE4,0x14,0x94,0x54,0xD4,0x34,0xB4,0x74,0xF4,
	0x0C,0x8C,0x4C,0xCC,0x2C,0xAC,0x6C,0xEC,0x1C,0x9C,0x5C,0xDC,0x3C,0xBC,0x7C,0xFC,
	0x02,0x82,0x42,0xC2,0x22,0xA2,0x62,0xE2,0x12,0x92,0x52,0xD2,0x32,0xB2,0x72,0xF2,
	0x0A,0x8A,0x4A,0xCA,0x2A,0xAA,0x6A,0xEA,0x1A,0x9A,0x5A,0xDA,0x3A,0xBA,0x7A,0xFA,
	0x06,0x86,0x46,0xC6,0x26,0xA6,0x66,0xE6,0x16,0x96,0x56,0xD6,0x36,0xB6,0x76,0xF6,
	0x0E,0x8E,0x4E,0xCE,0x2E,0xAE,0x6E,0xEE,0x1E,0x9E,0x5E,0xDE,0x3E,0xBE,0x7E,0xFE,
	0x01,0x81,0x41,0xC1,0x21,0xA1,0x61,0xE1,0x11,0x91,0x51,0xD1,0x31,0xB1,0x71,0xF1,
	0x09,0x89,0x49,0xC9,0x29,0xA9,0x69,0xE9,0x19,0x99,0x59,0xD9,0x39,0xB9,0x79,0xF9,
	0x05,0x85,0x45,0xC5,0x25,0xA5,0x65,0xE5,0x15,0x95,0x55,0xD5,0x35,0xB5,0x75,0xF5,
	0x0D,0x8D,0x4D,0xCD,0x2D,0xAD,0x6D,0xED,0x1D,0x9D,0x5D,0xDD,0x3D,0xBD,0x7D,0xFD,
	0x03,0x83,0x43,0xC3,0x23,0xA3,0x63,0xE3,0x13,0x93,0x53,0xD3,0x33,0xB3,0x73,0xF3,
	0x0B,0x8B,0x4B,0xCB,0x2B,0xAB,0x6B,0xEB,0x1B,0x9B,0x5B,0xDB,0x3B,0xBB,0x7B,0xFB,
	0x07,0x87,0x47,0xC7,0x27,0xA7,0x67,0xE7,0x17,0x97,0x57,0xD7,0x37,0xB7,0x77,0xF7,
	0x0F,0x8F,0x4F,0xCF,0x2F,0xAF,0x6F,0xEF,0x1F,0x9F,0x5F,0xDF,0x3F,0xBF,0x7F,0xFF
};

const	unsigned long	CHRLoBit[16] =
{
	0x00000000,0x00000001,0x00000100,0x00000101,0x00010000,0x00010001,0x00010100,0x00010101,
	0x01000000,0x01000001,0x01000100,0x01000101,0x01010000,0x01010001,0x01010100,0x01010101
};
const	unsigned long	CHRHiBit[16] =
{
	0x00000000,0x00000002,0x00000200,0x00000202,0x00020000,0x00020002,0x00020200,0x00020202,
	0x02000000,0x02000002,0x02000200,0x02000202,0x02020000,0x02020002,0x02020200,0x02020202
};

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
const	unsigned long	AttribBits[4] =
{
	0x00000000,0x04040404,0x08080808,0x0C0C0C0C
};

const	unsigned char	AttribShift[128] =
{
	0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,0,0,2,2,
	4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6,4,4,6,6
};

unsigned char	PPU_OpenBus[0x400] =
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

static	void	(_MAPINT *PPU_PPUCycle)		(int,int,int,int);
static	void	_MAPINT	PPU_NoPPUCycle		(int Addr, int Scanline, int Cycle, int IsRendering)	{ }

int	_MAPINT	PPU_BusRead (int Bank, int Addr)
{
/*	if (!PPU.Readable[Bank])
		return Addr & 0xFF;
*/	return PPU.CHRPointer[Bank][Addr];
}

void	_MAPINT	PPU_BusWriteCHR (int Bank, int Addr, int Val)
{
	if (!PPU.Writable[Bank])
		return;
#ifdef ENABLE_DEBUGGER
	Debugger.PatChanged = TRUE;
#endif	/* ENABLE_DEBUGGER */
	PPU.CHRPointer[Bank][Addr] = Val;
}

void	_MAPINT	PPU_BusWriteNT (int Bank, int Addr, int Val)
{
	if (!PPU.Writable[Bank])
		return;
#ifdef ENABLE_DEBUGGER
	Debugger.NTabChanged = TRUE;
#endif	/* ENABLE_DEBUGGER */
	PPU.CHRPointer[Bank][Addr] = Val;
}

void	PPU_Reset (void)
{
	PPU.SprAddr = 0;
	PPU.IsRendering = FALSE;
	PPU.OnScreen = FALSE;
	PPU.IOAddr = 0;
	PPU.IOVal = 0;
	PPU.IOMode = 0;
	PPU.Clockticks = 0;
	PPU.PALsubticks = 0;
	PPU.SLnum = 241;
	PPU_GetHandlers();
}
void	PPU_GetHandlers (void)
{
	if ((MI) && (MI->PPUCycle))
		PPU_PPUCycle = MI->PPUCycle;
	else	PPU_PPUCycle = PPU_NoPPUCycle;
	PPU_GetGFXPtr();
}
__inline static	void	DiscoverSprites (void)
{
	int SprHeight = 7 | ((PPU.Reg2000 & 0x20) >> 2);
	int SL = PPU.SLnum;
	int spt;
	PPU.SprCount = 0;
	PPU.Spr0InLine = FALSE;
	if (!PPU.IsRendering)
		return;
	for (spt = 0; spt < 32; spt += 4)
		PPU.SprBuff[spt+1] = 0xFF;	/* pre-init sprite buffer tile indices */
	for (spt = 0; spt < 256; spt += 4)
	{
		unsigned char off;
		if (spt < 8)
			off = spt + (PPU.SprAddr & 0xF8);
		else	off = spt;
		if ((SL - PPU.Sprite[off]) & ~SprHeight)
			continue;
		if (PPU.SprCount == 32)
		{
			PPU.Reg2002 |= 0x20;
			break;
		}
		PPU.SprBuff[PPU.SprCount] = SL - PPU.Sprite[off];
		PPU.SprBuff[PPU.SprCount+1] = PPU.Sprite[off | 1];
		PPU.SprBuff[PPU.SprCount+2] = PPU.Sprite[off | 2];
		PPU.SprBuff[PPU.SprCount+3] = PPU.Sprite[off | 3];
		PPU.SprCount += 4;
		if (!spt)
			PPU.Spr0InLine = TRUE;
	}
}
/*
static	unsigned char	SpritePtr;
__inline static void	ProcessSprites (void)
{
	if (PPU.Clockticks < 64)
	{
		static int sprpos;
		if (PPU.Clockticks == 0)
			sprpos = 0;
		PPU.SprBuff[SpritePtr = sprpos] = 0xFF;
		if (PPU.Clockticks & 1)
			sprpos++;
	}
	else if (PPU.Clockticks < 256)
	{
		static int sprpos, sprnum, sprsub, sprtmp, sprstate;
		if (PPU.Clockticks == 64)
		{
			sprpos = 0;
			sprnum = 0;
			sprsub = 0;
			sprstate = 0;
		}
		switch (sprstate)
		{
		case 0:	// evaluate current Y coordinate
			if (PPU.Clockticks & 1)
			{
				if (sprpos < 0x40)
					PPU.SprBuff[SpritePtr = sprpos] = sprtmp;
				if ((sprtmp >= PPU.SLnum) && (sprtmp < PPU.SLnum + 7 + ((PPU.Reg2000 & 0x20) >> 2)))
				{
					sprstate = 1;	// sprite is in range, fetch the rest of it
					sprsub = 1;
					sprpos++;
				}
				else
				{
					//
					if (++sprnum == 64)
						sprstate = 4;
					else if (sprpos == 32)
						sprstate = 2;
					else	sprstate = 0;
					//
				}
			}
			else	sprtmp = PPU.Sprite[SpritePtr = sprnum << 2];
			break;
		case 1:	// Y-coordinate is in range, copy remaining bytes
			if (PPU.Clockticks & 1)
			{
				PPU.SprBuff[SpritePtr = sprpos++] = sprtmp;
				if (sprsub++ == 3)
				{
					//
					if (++sprnum == 64)
						sprstate = 4;
					else if (sprpos == 32)
						sprstate = 2;
					else	sprstate = 0;
					//
				}
			}
			else	sprtmp = PPU.Sprite[SpritePtr = (sprnum << 2) | sprsub];
			break;
		case 2:	// exactly 8 sprites detected, go through 'weird' evaluation
			if (PPU.Clockticks & 1)
			{
				SpritePtr = 0;	// failed write
				if ((sprtmp >= PPU.SLnum) && (sprtmp < PPU.SLnum + 7 + ((PPU.Reg2000 & 0x20) >> 2)))
				{	// 9th sprite found "in range"
					sprstate = 3;
					sprsub = 1;
					sprpos = 1;	// set counter
					PPU.Reg2002 |= 0x20;	// set sprite overflow flag
				}
				else
				{
					sprsub = (sprsub + 1) & 3;
					//
					if (++sprnum == 64)
						sprstate = 4;
					//
				}
			}
			else	sprtmp = PPU.Sprite[SpritePtr = (sprnum << 2) | sprsub];
			break;
		case 3:	// 9th sprite detected, fetch next 3 bytes
			if (PPU.Clockticks & 1)
			{
				SpritePtr = sprpos;
				sprsub = (sprsub + 1) & 3;
				if (!sprsub)
					sprnum = (sprnum + 1) & 63;
				if (sprpos++ == 3)
				{
					if (sprsub == 3)
						sprnum = (sprnum + 1) & 63;
					sprstate = 4;
				}
			}
			else	sprtmp = PPU.Sprite[SpritePtr = (sprnum << 2) | sprsub];
			break;
		case 4:	// no more sprites to evaluate, thrash until HBLANK
			if (PPU.Clockticks & 1)
			{
				SpritePtr = 0;
				sprnum = (sprnum + 1) & 63;
			}
			else	sprtmp = PPU.Sprite[SpritePtr = sprnum << 2];
			break;
		}
	}
	else if (PPU.Clockticks < 320)
	{
		if ((PPU.Clockticks & 7) < 4)
			SpritePtr = (PPU.Clockticks & 3) | (((PPU.Clockticks - 256) & 0x38) >> 1);
		else	SpritePtr = (((PPU.Clockticks - 256) & 0x38) >> 1) | 3;
	}
	else
	{
		SpritePtr = 0;
	}

//*** Cycles 0-63: Secondary OAM (32-byte buffer for current sprites on scanline) is initialized to $FF - attempting to read $2004 will return $FF
//*** Cycles 64-255: Sprite evaluation
//* On even cycles, data is read from (primary) OAM
//* On odd cycles, data is written to secondary OAM (unless writes are inhibited, in which case it will read the value in secondary OAM instead)
//1. Starting at n = 0, read a sprite's Y-coordinate (OAM[n][0], copying it to the next open slot in secondary OAM (unless 8 sprites have been found, in which case the write is ignored).
//1a. If Y-coordinate is in range, copy remaining bytes of sprite data (OAM[n][1] thru OAM[n][3]) into secondary OAM.
//2. Increment n
//2a. If n has overflowed back to zero (all 64 sprites evaluated), go to 4
//2b. If less than 8 sprites have been found, go to 1
//2c. If exactly 8 sprites have been found, disable writes to secondary OAM
//3. Starting at m = 0, evaluate OAM[n][m] as a Y-coordinate.
//3a. If the value is in range, set the sprite overflow flag in $2002 and read the next 3 entries of OAM (incrementing 'm' after each byte and incrementing 'n' when 'm' overflows); if m = 3, increment n
//3b. If the value is not in range, increment n AND m (without carry). If n overflows to 0, go to 4; otherwise go to 3
//4. Attempt (and fail) to copy OAM[n][0] into the next free slot in secondary OAM, and increment n (repeat until HBLANK is reached)
//*** Cycles 256-319: Sprite fetches (8 sprites total, 8 cycles per sprite)
//1-4: Read the Y-coordinate, tile number, attributes, and X-coordinate of the selected sprite
//5-8: Read the X-coordinate of the selected sprite 4 times.
//* On the first empty sprite slot, read the Y-coordinate of sprite #63 followed by $FF for the remaining 7 cycles
//* On all subsequent empty sprite slots, read $FF for all 8 reads
//*** Cycles 320-340: Background render pipeline initialization
//* Read the first byte in secondary OAM (the Y-coordinate of the first sprite found, sprite #63 if no sprites were found)
}
*/
void	PPU_GetGFXPtr (void)
{
	PPU.GfxData = DrawArray;
}

void	PPU_PowerOn()
{
	PPU.Reg2000 = 0;
	PPU.Reg2001 = 0;
	PPU.ColorEmphasis = 0;
	PPU.GrayScale = 0x3F;
	PPU.VRAMAddr = PPU.IntReg = 0;
	PPU.IntX = 0;
	PPU.ppuLatch = 0;
	PPU.Reg2002 = 0;
	PPU.buf2007 = 0;
	PPU.HVTog = TRUE;
	PPU.ShortSL = TRUE;
	ZeroMemory(PPU_VRAM,sizeof(PPU_VRAM));
	memset(PPU.Palette,0x3F,sizeof(PPU.Palette));
	memset(PPU.Sprite,0xFF,sizeof(PPU.Sprite));
	PPU_GetHandlers();
}

int	PPU_Save (FILE *out)
{
	int clen = 0;
	unsigned short tps;
	fwrite(PPU_VRAM,1,0x1000,out);	clen += 0x1000;	//	NTAR	uint8[0x1000]	4 KB of name/attribute table RAM
	fwrite(PPU.Sprite,1,0x100,out);	clen += 0x100;	//	SPRA	uint8[0x100]	256 bytes of sprite RAM
	fwrite(PPU.Palette,1,0x20,out);	clen += 0x20;	//	PRAM	uint8[0x20]	32 bytes of palette index RAM
	fwrite(&PPU.Reg2000,1,1,out);	clen++;		//	R2000	uint8		Last value written to $2000
	fwrite(&PPU.Reg2001,1,1,out);	clen++;		//	R2001	uint8		Last value written to $2001
	fwrite(&PPU.Reg2002,1,1,out);	clen++;		//	R2002	uint8		Current contents of $2002
	fwrite(&PPU.SprAddr,1,1,out);	clen++;		//	SPADR	uint8		SPR-RAM Address ($2003)

	fwrite(&PPU.IntX,1,1,out);	clen++;		//	XOFF	uint8		Tile X-offset.

	fwrite(&PPU.HVTog,1,1,out);	clen++;		//	VTOG	uint8		Toggle used by $2005 and $2006.
	tps = (unsigned short)PPU.VRAMAddr;
	fwrite(&tps,2,1,out);		clen += 2;	//	RADD	uint16		VRAM Address
	tps = (unsigned short)PPU.IntReg;
	fwrite(&tps,2,1,out);		clen += 2;	//	TADD	uint16		VRAM Address Latch
	fwrite(&PPU.buf2007,1,1,out);	clen++;		//	VBUF	uint8		VRAM Read Buffer
	fwrite(&PPU.ppuLatch,1,1,out);	clen++;		//	PGEN	uint8		PPU "general" latch

	tps = (unsigned short)PPU.Clockticks | (PPU.PALsubticks << 12);
	fwrite(&tps,2,1,out);		clen += 2;	//	TICKS	uint16		Clock Ticks (0..340) with PAL subticks stored in upper 4 bits
	tps = (unsigned short)PPU.SLnum;
	fwrite(&tps,2,1,out);		clen += 2;	//	SLNUM	uint16		Scanline number
	fwrite(&PPU.ShortSL,1,1,out);	clen++;		//	SHORT	uint8		Short frame (last scanline 1 clock tick shorter)

	tps = (unsigned short)PPU.IOAddr;
	fwrite(&tps,2,1,out);		clen += 2;	//	IOADD	uint16		External I/O Address
	fwrite(&PPU.IOVal,1,1,out);	clen++;		//	IOVAL	uint8		External I/O Value
	fwrite(&PPU.IOMode,1,1,out);	clen++;		//	IOMOD	uint8		External I/O Mode/Counter

	fwrite(&PPU.IsPAL,1,1,out);	clen++;		//	NTSCP	uint8		0 for NTSC, 1 for PAL
	return clen;
}

int	PPU_Load (FILE *in)
{
	int clen = 0;
	unsigned short tps;
	fread(PPU_VRAM,1,0x1000,in);	clen += 0x1000;	//	NTAR	uint8[0x1000]	4 KB of name/attribute table RAM
	fread(PPU.Sprite,1,0x100,in);	clen += 0x100;	//	SPRA	uint8[0x100]	256 bytes of sprite RAM
	fread(PPU.Palette,1,0x20,in);	clen += 0x20;	//	PRAM	uint8[0x20]	32 bytes of palette index RAM
	fread(&PPU.Reg2000,1,1,in);	clen++;		//	R2000	uint8		Last value written to $2000
	fread(&PPU.Reg2001,1,1,in);	clen++;		//	R2001	uint8		Last value written to $2001
	fread(&PPU.Reg2002,1,1,in);	clen++;		//	R2002	uint8		Current contents of $2002
	fread(&PPU.SprAddr,1,1,in);	clen++;		//	SPADR	uint8		SPR-RAM Address ($2003)

	fread(&PPU.IntX,1,1,in);	clen++;		//	XOFF	uint8		Tile X-offset.

	fread(&PPU.HVTog,1,1,in);	clen++;		//	VTOG	uint8		Toggle used by $2005 and $2006.
	fread(&tps,2,1,in);		clen += 2;	//	RADD	uint16		VRAM Address
	PPU.VRAMAddr = tps;
	fread(&tps,2,1,in);		clen += 2;	//	TADD	uint16		VRAM Address Latch
	PPU.IntReg = tps;
	fread(&PPU.buf2007,1,1,in);	clen++;		//	VBUF	uint8		VRAM Read Buffer
	fread(&PPU.ppuLatch,1,1,in);	clen++;		//	PGEN	uint8		PPU "general" latch.

	fread(&tps,2,1,in);		clen += 2;	//	TICKS	uint16		Clock Ticks (0..340) with PAL subticks stored in upper 4 bits
	PPU.Clockticks = tps & 0xFFF;
	PPU.PALsubticks = tps >> 12;

	fread(&tps,2,1,in);		clen += 2;	//	SLNUM	uint16		Scanline number
	PPU.SLnum = tps;
	fread(&PPU.ShortSL,1,1,in);	clen++;		//	SHORT	uint8		Short frame (last scanline 1 clock tick shorter)

	fread(&tps,2,1,in);		clen += 2;	//	IOADD	uint16		External I/O Address
	PPU.IOAddr = tps;
	fread(&PPU.IOVal,1,1,in);	clen++;		//	IOVAL	uint8		External I/O Value
	fread(&PPU.IOMode,1,1,in);	clen++;		//	IOMOD	uint8		External I/O Mode/Counter

	fread(&PPU.IsPAL,1,1,in);	clen++;		//	NTSCP	uint8		0 for NTSC, 1 for PAL

	PPU.IsRendering = PPU.OnScreen = FALSE;
	PPU.ColorEmphasis = (PPU.Reg2001 & 0xE0) << 1;
	PPU.GrayScale = (PPU.Reg2001 & 0x01) ? 0x30 : 0x3F;
	NES_SetCPUMode(PPU.IsPAL);
	return clen;
}

static int EndSLTicks = 341;
static unsigned long PatAddr;
static unsigned char RenderData[4];

__inline static	void	RunNoSkip (int NumTicks)
{
	register unsigned long TL;
	register unsigned char TC;

	register int SprNum;
	register unsigned char *CurTileData;

	register int i, y;
	for (i = 0; i < NumTicks; i++)
	{
		PPU.Clockticks++;
		if (PPU.Clockticks == 256)
		{
			if (PPU.SLnum < 240)
			{
				DiscoverSprites();
				ZeroMemory(PPU.TileData,sizeof(PPU.TileData));
			}
			if (PPU.SLnum == -1)
			{
				if ((PPU.ShortSL) && (PPU.IsRendering) && (!PPU.IsPAL))
					EndSLTicks = 340;
				else	EndSLTicks = 341;
			}
			else	EndSLTicks = 341;
		}
		else if (PPU.Clockticks == 304)
		{
			if ((PPU.IsRendering) && (PPU.SLnum == -1))
				PPU.VRAMAddr = PPU.IntReg;
		}
		else if (PPU.Clockticks == EndSLTicks)
		{
			PPU.Clockticks = 0;
			PPU.SLnum++;
			NES.Scanline = TRUE;
			if (PPU.SLnum < 240)
				PPU.OnScreen = TRUE;
			else if (PPU.SLnum == 240)
				PPU.IsRendering = PPU.OnScreen = FALSE;
			else if (PPU.SLnum == 241)
			{
				PPU_GetGFXPtr();
				GFX_DrawScreen();
				PPU.Reg2002 |= 0x80;
				if (PPU.Reg2000 & 0x80)
					CPU.WantNMI = TRUE;
				PPU.SprAddr = 0;
			}
			else if (PPU.SLnum == PPU.SLEndFrame - 1)
			{
				PPU.SLnum = -1;
				PPU.ShortSL = !PPU.ShortSL;
				PPU.Reg2002 = 0;
				if (PPU.Reg2001 & 0x18)
					PPU.IsRendering = TRUE;
			}
		}
		if (PPU.IsRendering)
		{
//			ProcessSprites();
			if (PPU.Clockticks & 1)
			{
				if (PPU.IOMode)
				{
					RenderData[(PPU.Clockticks >> 1) & 3] = rand() & 0xFF;
					if (PPU.IOMode == 2)
						PPU.WriteHandler[PPU.RenderAddr >> 10](PPU.RenderAddr >> 10,PPU.RenderAddr & 0x3FF,PPU.IOVal = RenderData[(PPU.Clockticks >> 1) & 3]);
				}
				else if (PPU.ReadHandler[PPU.RenderAddr >> 10] == PPU_BusRead)
					RenderData[(PPU.Clockticks >> 1) & 3] = PPU.CHRPointer[PPU.RenderAddr >> 10][PPU.RenderAddr & 0x3FF];
				else	RenderData[(PPU.Clockticks >> 1) & 3] = PPU.ReadHandler[PPU.RenderAddr >> 10](PPU.RenderAddr >> 10,PPU.RenderAddr & 0x3FF);
			}
			switch (PPU.Clockticks)
			{
			/* BEGIN BACKGROUND */
			case   0:	case   8:	case  16:	case  24:	case  32:	case  40:	case  48:	case  56:
			case  64:	case  72:	case  80:	case  88:	case  96:	case 104:	case 112:	case 120:
			case 128:	case 136:	case 144:	case 152:	case 160:	case 168:	case 176:	case 184:
			case 192:	case 200:	case 208:	case 216:	case 224:	case 232:	case 240:	case 248:
			case 320:	case 328:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
				break;
			case   1:	case   9:	case  17:	case  25:	case  33:	case  41:	case  49:	case  57:
			case  65:	case  73:	case  81:	case  89:	case  97:	case 105:	case 113:	case 121:
			case 129:	case 137:	case 145:	case 153:	case 161:	case 169:	case 177:	case 185:
			case 193:	case 201:	case 209:	case 217:	case 225:	case 233:	case 241:	case 249:
			case 321:	case 329:
				PatAddr = (RenderData[0] << 4) | (PPU.VRAMAddr >> 12) | ((PPU.Reg2000 & 0x10) << 8);
				break;
			case   2:	case  10:	case  18:	case  26:	case  34:	case  42:	case  50:	case  58:
			case  66:	case  74:	case  82:	case  90:	case  98:	case 106:	case 114:	case 122:
			case 130:	case 138:	case 146:	case 154:	case 162:	case 170:	case 178:	case 186:
			case 194:	case 202:	case 210:	case 218:	case 226:	case 234:	case 242:	case 250:
			case 322:	case 330:
				PPU.RenderAddr = 0x23C0 | (PPU.VRAMAddr & 0xC00) | AttribLoc[(PPU.VRAMAddr >> 2) & 0xFF];
				break;
			case   3:	case  11:	case  19:	case  27:	case  35:	case  43:	case  51:	case  59:
			case  67:	case  75:	case  83:	case  91:	case  99:	case 107:	case 115:	case 123:
			case 131:	case 139:	case 147:	case 155:	case 163:	case 171:	case 179:	case 187:
			case 195:	case 203:	case 211:	case 219:	case 227:	case 235:	case 243:
				CurTileData = &PPU.TileData[PPU.Clockticks + 13];
				TL = AttribBits[(RenderData[1] >> AttribShift[PPU.VRAMAddr & 0x7F]) & 3];
				((unsigned long *)CurTileData)[0] = TL;
				((unsigned long *)CurTileData)[1] = TL;
				if ((PPU.VRAMAddr & 0x1F) == 0x1F)
					PPU.VRAMAddr ^= 0x41F;
				else	PPU.VRAMAddr++;
				break;
			case 251:
				CurTileData = &PPU.TileData[PPU.Clockticks + 13];
				TL = AttribBits[(RenderData[1] >> AttribShift[PPU.VRAMAddr & 0x7F]) & 3];
				((unsigned long *)CurTileData)[0] = TL;
				((unsigned long *)CurTileData)[1] = TL;
				if ((PPU.VRAMAddr & 0x1F) == 0x1F)
					PPU.VRAMAddr ^= 0x41F;
				else	PPU.VRAMAddr++;
				if ((PPU.VRAMAddr & 0x7000) == 0x7000)
				{
					register int YScroll = PPU.VRAMAddr & 0x3E0;
					PPU.VRAMAddr &= 0xFFF;
					if (YScroll == 0x3A0)
						PPU.VRAMAddr ^= 0xBA0;
					else if (YScroll == 0x3E0)
						PPU.VRAMAddr ^= 0x3E0;
					else	PPU.VRAMAddr += 0x20;
				}
				else	PPU.VRAMAddr += 0x1000;
				break;
			case 323:	case 331:
				CurTileData = &PPU.TileData[PPU.Clockticks - 323];
				TL = AttribBits[(RenderData[1] >> AttribShift[PPU.VRAMAddr & 0x7F]) & 3];
				((unsigned long *)CurTileData)[0] = TL;
				((unsigned long *)CurTileData)[1] = TL;
				if ((PPU.VRAMAddr & 0x1F) == 0x1F)
					PPU.VRAMAddr ^= 0x41F;
				else	PPU.VRAMAddr++;
				break;
			case   4:	case  12:	case  20:	case  28:	case  36:	case  44:	case  52:	case  60:
			case  68:	case  76:	case  84:	case  92:	case 100:	case 108:	case 116:	case 124:
			case 132:	case 140:	case 148:	case 156:	case 164:	case 172:	case 180:	case 188:
			case 196:	case 204:	case 212:	case 220:	case 228:	case 236:	case 244:	case 252:
			case 324:	case 332:
				PPU.RenderAddr = PatAddr;
				break;
			case   5:	case  13:	case  21:	case  29:	case  37:	case  45:	case  53:	case  61:
			case  69:	case  77:	case  85:	case  93:	case 101:	case 109:	case 117:	case 125:
			case 133:	case 141:	case 149:	case 157:	case 165:	case 173:	case 181:	case 189:
			case 197:	case 205:	case 213:	case 221:	case 229:	case 237:	case 245:	case 253:
				TC = ReverseCHR[RenderData[2]];
				CurTileData = &PPU.TileData[PPU.Clockticks + 11];
				((unsigned long *)CurTileData)[0] |= CHRLoBit[TC & 0xF];
				((unsigned long *)CurTileData)[1] |= CHRLoBit[TC >> 4];
				break;
			case 325:	case 333:
				TC = ReverseCHR[RenderData[2]];
				CurTileData = &PPU.TileData[PPU.Clockticks - 325];
				((unsigned long *)CurTileData)[0] |= CHRLoBit[TC & 0xF];
				((unsigned long *)CurTileData)[1] |= CHRLoBit[TC >> 4];
				break;
			case   6:	case  14:	case  22:	case  30:	case  38:	case  46:	case  54:	case  62:
			case  70:	case  78:	case  86:	case  94:	case 102:	case 110:	case 118:	case 126:
			case 134:	case 142:	case 150:	case 158:	case 166:	case 174:	case 182:	case 190:
			case 198:	case 206:	case 214:	case 222:	case 230:	case 238:	case 246:	case 254:
			case 326:	case 334:
				PPU.RenderAddr = PatAddr | 8;
				break;
			case   7:	case  15:	case  23:	case  31:	case  39:	case  47:	case  55:	case  63:
			case  71:	case  79:	case  87:	case  95:	case 103:	case 111:	case 119:	case 127:
			case 135:	case 143:	case 151:	case 159:	case 167:	case 175:	case 183:	case 191:
			case 199:	case 207:	case 215:	case 223:	case 231:	case 239:	case 247:	case 255:
				TC = ReverseCHR[RenderData[3]];
				CurTileData = &PPU.TileData[PPU.Clockticks + 9];
				((unsigned long *)CurTileData)[0] |= CHRHiBit[TC & 0xF];
				((unsigned long *)CurTileData)[1] |= CHRHiBit[TC >> 4];
				break;
			case 327:	case 335:
				TC = ReverseCHR[RenderData[3]];
				CurTileData = &PPU.TileData[PPU.Clockticks - 327];
				((unsigned long *)CurTileData)[0] |= CHRHiBit[TC & 0xF];
				((unsigned long *)CurTileData)[1] |= CHRHiBit[TC >> 4];
				break;
				/* END BACKGROUND */
				/* BEGIN SPRITES */
			case 256:	case 264:	case 272:	case 280:	case 288:	case 296:	case 304:	case 312:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
				break;
			case 257:
				PPU.VRAMAddr &= ~0x41F;
				PPU.VRAMAddr |= PPU.IntReg & 0x41F;
					case 265:	case 273:	case 281:	case 289:	case 297:	case 305:	case 313:
				break;
			case 258:	case 266:	case 274:	case 282:	case 290:	case 298:	case 306:	case 314:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
				break;
			case 259:	case 267:	case 275:	case 283:	case 291:	case 299:	case 307:	case 315:
				SprNum = (PPU.Clockticks >> 1) & 0x1C;
				TC = PPU.SprBuff[SprNum | 1];
				if (PPU.Reg2000 & 0x20)
					PatAddr = ((TC & 0xFE) << 4) | ((TC & 0x01) << 12) | ((PPU.SprBuff[SprNum] & 7) ^ ((PPU.SprBuff[SprNum | 2] & 0x80) ? 0x17 : 0x00) ^ ((PPU.SprBuff[SprNum] & 0x8) << 1));
				else	PatAddr = (TC << 4) | (PPU.SprBuff[SprNum] ^ ((PPU.SprBuff[SprNum | 2] & 0x80) ? 0x7 : 0x0)) | ((PPU.Reg2000 & 0x08) << 9);
				break;
			case 260:	case 268:	case 276:	case 284:	case 292:	case 300:	case 308:	case 316:
				PPU.RenderAddr = PatAddr;
				break;
			case 261:	case 269:	case 277:	case 285:	case 293:	case 301:	case 309:	case 317:
				SprNum = (PPU.Clockticks >> 1) & 0x1E;
				if (PPU.SprBuff[SprNum] & 0x40)
					TC = RenderData[2];
				else	TC = ReverseCHR[RenderData[2]];
				TL = AttribBits[PPU.SprBuff[SprNum] & 0x3];
				CurTileData = PPU.SprData[SprNum >> 2];
				((unsigned long *)CurTileData)[0] = CHRLoBit[TC & 0xF] | TL;
				((unsigned long *)CurTileData)[1] = CHRLoBit[TC >> 4] | TL;
				break;
			case 262:	case 270:	case 278:	case 286:	case 294:	case 302:	case 310:	case 318:
				PPU.RenderAddr = PatAddr | 8;
				break;
			case 263:	case 271:	case 279:	case 287:	case 295:	case 303:	case 311:	case 319:
				SprNum = (PPU.Clockticks >> 1) & 0x1E;
				if (PPU.SprBuff[SprNum] & 0x40)
					TC = RenderData[3];
				else	TC = ReverseCHR[RenderData[3]];
				CurTileData = PPU.SprData[SprNum >> 2];
				((unsigned long *)CurTileData)[0] |= CHRHiBit[TC & 0xF];
				((unsigned long *)CurTileData)[1] |= CHRHiBit[TC >> 4];
				break;
				/* END SPRITES */
			case 336:	case 338:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
			case 337:	case 339:
				break;
			case 340:
				break;
			}
			if (!(PPU.Clockticks & 1))
			{
				PPU_PPUCycle(PPU.RenderAddr,PPU.SLnum,PPU.Clockticks,1);
				if (PPU.IOMode == 2)
					PPU.WriteHandler[PPU.RenderAddr >> 10](PPU.RenderAddr >> 10,PPU.RenderAddr & 0x3FF,PPU.RenderAddr & 0xFF);
			}
		}
		if (PPU.IOMode)
		{
			unsigned short addr = (unsigned short)(PPU.IOAddr & 0x3FFF);
			if ((PPU.IOMode >= 5) && (!PPU.IsRendering))
				PPU_PPUCycle(addr,PPU.SLnum,PPU.Clockticks,PPU.IsRendering);
			else if (PPU.IOMode == 2)
			{
				if (!PPU.IsRendering)
					PPU.WriteHandler[addr >> 10](addr >> 10,addr & 0x3FF,PPU.IOVal);
			}
			else if (PPU.IOMode == 1)
			{
				PPU.IOMode++;
				if (!PPU.IsRendering)
				{
					if (PPU.ReadHandler[addr >> 10] == PPU_BusRead)
						PPU.buf2007 = PPU.CHRPointer[addr >> 10][addr & 0x3FF];
					else	PPU.buf2007 = PPU.ReadHandler[addr >> 10](addr >> 10,addr & 0x3FF);
				}
			}
			PPU.IOMode -= 2;
		}
		if (!PPU.IsRendering && !PPU.IOMode)
			PPU_PPUCycle(PPU.VRAMAddr,PPU.SLnum,PPU.Clockticks,0);
		if ((PPU.Clockticks < 256) && (PPU.OnScreen))
		{
			register int PalIndex;
			if ((PPU.Reg2001 & 0x08) && ((PPU.Clockticks >= 8) || (PPU.Reg2001 & 0x02)))
				TC = PPU.TileData[PPU.Clockticks + PPU.IntX];
			else	TC = 0;
			if ((PPU.Reg2001 & 0x10) && ((PPU.Clockticks >= 8) || (PPU.Reg2001 & 0x04)))
				for (y = 0; y < PPU.SprCount; y += 4)
				{
					register int SprPixel = PPU.Clockticks - PPU.SprBuff[y | 3];
					register unsigned char SprDat;
					if (SprPixel & ~7)
						continue;
					SprDat = PPU.SprData[y >> 2][SprPixel];
					if (SprDat & 0x3)
					{
						if ((PPU.Spr0InLine) && (y == 0) && (TC & 0x3) && (PPU.Clockticks < 255))
						{
							PPU.Reg2002 |= 0x40;	/* Sprite 0 hit */
							PPU.Spr0InLine = FALSE;
						}
						if (!((TC & 0x3) && (PPU.SprBuff[y | 2] & 0x20)))
							TC = SprDat | 0x10;
						break;
					}
				}
			if (TC & 0x3)
				PalIndex = PPU.Palette[TC & 0x1F];
			else	PalIndex = PPU.Palette[0];
			if ((!PPU.IsRendering) && ((PPU.VRAMAddr & 0x3F00) == 0x3F00))
				PalIndex = PPU.Palette[PPU.VRAMAddr & 0x1F];
			PalIndex &= PPU.GrayScale;
			PalIndex |= PPU.ColorEmphasis;
#ifdef	SHORQ
			if (CPU.WantIRQ)
				PalIndex ^= 0x80;
#endif /* SHORQ */
			*PPU.GfxData = PalIndex;
			PPU.GfxData++;
		}
	}
}
__inline static	void	RunSkip (int NumTicks)
{
	register unsigned char TC;

	register int SprNum;
	register unsigned char *CurTileData;

	register int i;
	for (i = 0; i < NumTicks; i++)
	{
		PPU.Clockticks++;
		if (PPU.Clockticks == 256)
		{
			if (PPU.SLnum < 240)
			{
				DiscoverSprites();
				if (PPU.Spr0InLine)
					ZeroMemory(PPU.TileData,sizeof(PPU.TileData));
			}
			if (PPU.SLnum == -1)
			{
				if ((PPU.ShortSL) && (!PPU.IsPAL))
					EndSLTicks = 340;
				else	EndSLTicks = 341;
			}
			else	EndSLTicks = 341;
		}
		else if (PPU.Clockticks == 304)
		{
			if ((PPU.IsRendering) && (PPU.SLnum == -1))
				PPU.VRAMAddr = PPU.IntReg;
		}
		else if (PPU.Clockticks == EndSLTicks)
		{
			PPU.Clockticks = 0;
			PPU.SLnum++;
			NES.Scanline = TRUE;
			if (PPU.SLnum < 240)
				PPU.OnScreen = TRUE;
			else if (PPU.SLnum == 240)
				PPU.IsRendering = PPU.OnScreen = FALSE;
			else if (PPU.SLnum == 241)
			{
				PPU_GetGFXPtr();
				GFX_DrawScreen();
				PPU.Reg2002 |= 0x80;
				if (PPU.Reg2000 & 0x80)
					CPU.WantNMI = TRUE;
				PPU.SprAddr = 0;
			}
			else if (PPU.SLnum == PPU.SLEndFrame - 1)
			{
				PPU.SLnum = -1;
				PPU.ShortSL = !PPU.ShortSL;
				PPU.Reg2002 = 0;
				if (PPU.Reg2001 & 0x18)
					PPU.IsRendering = TRUE;
			}
		}
		if (PPU.IsRendering)
		{
//			ProcessSprites();
			if (PPU.Clockticks & 1)
			{
				if (PPU.IOMode)
				{
					RenderData[(PPU.Clockticks >> 1) & 3] = rand() & 0xFF;
					if (PPU.IOMode == 2)
						PPU.WriteHandler[PPU.RenderAddr >> 10](PPU.RenderAddr >> 10,PPU.RenderAddr & 0x3FF,PPU.IOVal = RenderData[(PPU.Clockticks >> 1) & 3]);
				}
				else if (PPU.ReadHandler[PPU.RenderAddr >> 10] == PPU_BusRead)
					RenderData[(PPU.Clockticks >> 1) & 3] = PPU.CHRPointer[PPU.RenderAddr >> 10][PPU.RenderAddr & 0x3FF];
				else	RenderData[(PPU.Clockticks >> 1) & 3] = PPU.ReadHandler[PPU.RenderAddr >> 10](PPU.RenderAddr >> 10,PPU.RenderAddr & 0x3FF);
			}
			switch (PPU.Clockticks)
			{
			/* BEGIN BACKGROUND */
			case   0:	case   8:	case  16:	case  24:	case  32:	case  40:	case  48:	case  56:
			case  64:	case  72:	case  80:	case  88:	case  96:	case 104:	case 112:	case 120:
			case 128:	case 136:	case 144:	case 152:	case 160:	case 168:	case 176:	case 184:
			case 192:	case 200:	case 208:	case 216:	case 224:	case 232:	case 240:	case 248:
			case 320:	case 328:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
				break;
			case   1:	case   9:	case  17:	case  25:	case  33:	case  41:	case  49:	case  57:
			case  65:	case  73:	case  81:	case  89:	case  97:	case 105:	case 113:	case 121:
			case 129:	case 137:	case 145:	case 153:	case 161:	case 169:	case 177:	case 185:
			case 193:	case 201:	case 209:	case 217:	case 225:	case 233:	case 241:	case 249:
			case 321:	case 329:
				PatAddr = (RenderData[0] << 4) | (PPU.VRAMAddr >> 12) | ((PPU.Reg2000 & 0x10) << 8);
				break;
			case   2:	case  10:	case  18:	case  26:	case  34:	case  42:	case  50:	case  58:
			case  66:	case  74:	case  82:	case  90:	case  98:	case 106:	case 114:	case 122:
			case 130:	case 138:	case 146:	case 154:	case 162:	case 170:	case 178:	case 186:
			case 194:	case 202:	case 210:	case 218:	case 226:	case 234:	case 242:	case 250:
			case 322:	case 330:
				PPU.RenderAddr = 0x23C0 | (PPU.VRAMAddr & 0xC00) | AttribLoc[(PPU.VRAMAddr >> 2) & 0xFF];
				break;
			case   3:	case  11:	case  19:	case  27:	case  35:	case  43:	case  51:	case  59:
			case  67:	case  75:	case  83:	case  91:	case  99:	case 107:	case 115:	case 123:
			case 131:	case 139:	case 147:	case 155:	case 163:	case 171:	case 179:	case 187:
			case 195:	case 203:	case 211:	case 219:	case 227:	case 235:	case 243:
			case 323:	case 331:
				if ((PPU.VRAMAddr & 0x1F) == 0x1F)
					PPU.VRAMAddr ^= 0x41F;
				else	PPU.VRAMAddr++;
				break;
			case 251:
				if ((PPU.VRAMAddr & 0x1F) == 0x1F)
					PPU.VRAMAddr ^= 0x41F;
				else	PPU.VRAMAddr++;
				if ((PPU.VRAMAddr & 0x7000) == 0x7000)
				{
					register int YScroll = PPU.VRAMAddr & 0x3E0;
					PPU.VRAMAddr &= 0xFFF;
					if (YScroll == 0x3A0)
						PPU.VRAMAddr ^= 0xBA0;
					else if (YScroll == 0x3E0)
						PPU.VRAMAddr ^= 0x3E0;
					else	PPU.VRAMAddr += 0x20;
				}
				else	PPU.VRAMAddr += 0x1000;
				break;
			case   4:	case  12:	case  20:	case  28:	case  36:	case  44:	case  52:	case  60:
			case  68:	case  76:	case  84:	case  92:	case 100:	case 108:	case 116:	case 124:
			case 132:	case 140:	case 148:	case 156:	case 164:	case 172:	case 180:	case 188:
			case 196:	case 204:	case 212:	case 220:	case 228:	case 236:	case 244:	case 252:
			case 324:	case 332:
				PPU.RenderAddr = PatAddr;
				break;
			case   5:	case  13:	case  21:	case  29:	case  37:	case  45:	case  53:	case  61:
			case  69:	case  77:	case  85:	case  93:	case 101:	case 109:	case 117:	case 125:
			case 133:	case 141:	case 149:	case 157:	case 165:	case 173:	case 181:	case 189:
			case 197:	case 205:	case 213:	case 221:	case 229:	case 237:	case 245:	case 253:
				if (PPU.Spr0InLine)
				{
					TC = ReverseCHR[RenderData[2]];
					CurTileData = &PPU.TileData[PPU.Clockticks + 11];
					((unsigned long *)CurTileData)[0] |= CHRLoBit[TC & 0xF];
					((unsigned long *)CurTileData)[1] |= CHRLoBit[TC >> 4];
				}
				break;
			case 325:	case 333:
				if (PPU.Spr0InLine)
				{
					TC = ReverseCHR[RenderData[2]];
					CurTileData = &PPU.TileData[PPU.Clockticks - 325];
					((unsigned long *)CurTileData)[0] |= CHRLoBit[TC & 0xF];
					((unsigned long *)CurTileData)[1] |= CHRLoBit[TC >> 4];
				}
				break;
			case   6:	case  14:	case  22:	case  30:	case  38:	case  46:	case  54:	case  62:
			case  70:	case  78:	case  86:	case  94:	case 102:	case 110:	case 118:	case 126:
			case 134:	case 142:	case 150:	case 158:	case 166:	case 174:	case 182:	case 190:
			case 198:	case 206:	case 214:	case 222:	case 230:	case 238:	case 246:	case 254:
			case 326:	case 334:
				PPU.RenderAddr = PatAddr | 8;
				break;
			case   7:	case  15:	case  23:	case  31:	case  39:	case  47:	case  55:	case  63:
			case  71:	case  79:	case  87:	case  95:	case 103:	case 111:	case 119:	case 127:
			case 135:	case 143:	case 151:	case 159:	case 167:	case 175:	case 183:	case 191:
			case 199:	case 207:	case 215:	case 223:	case 231:	case 239:	case 247:	case 255:
				if (PPU.Spr0InLine)
				{
					TC = ReverseCHR[RenderData[3]];
					CurTileData = &PPU.TileData[PPU.Clockticks + 9];
					((unsigned long *)CurTileData)[0] |= CHRHiBit[TC & 0xF];
					((unsigned long *)CurTileData)[1] |= CHRHiBit[TC >> 4];
				}
				break;
			case 327:	case 335:
				if (PPU.Spr0InLine)
				{
					TC = ReverseCHR[RenderData[3]];
					CurTileData = &PPU.TileData[PPU.Clockticks - 327];
					((unsigned long *)CurTileData)[0] |= CHRHiBit[TC & 0xF];
					((unsigned long *)CurTileData)[1] |= CHRHiBit[TC >> 4];
				}
				break;
				/* END BACKGROUND */
				/* BEGIN SPRITES */
			case 256:	case 264:	case 272:	case 280:	case 288:	case 296:	case 304:	case 312:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
				break;
			case 257:
				PPU.VRAMAddr &= ~0x41F;
				PPU.VRAMAddr |= PPU.IntReg & 0x41F;
					case 265:	case 273:	case 281:	case 289:	case 297:	case 305:	case 313:
				break;
			case 258:	case 266:	case 274:	case 282:	case 290:	case 298:	case 306:	case 314:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
				break;
			case 259:	case 267:	case 275:	case 283:	case 291:	case 299:	case 307:	case 315:
				SprNum = (PPU.Clockticks >> 1) & 0x1C;
				TC = PPU.SprBuff[SprNum | 1];
				if (PPU.Reg2000 & 0x20)
					PatAddr = ((TC & 0xFE) << 4) | ((TC & 0x01) << 12) | ((PPU.SprBuff[SprNum] & 7) ^ ((PPU.SprBuff[SprNum | 2] & 0x80) ? 0x17 : 0x00) ^ ((PPU.SprBuff[SprNum] & 0x8) << 1));
				else	PatAddr = (TC << 4) | (PPU.SprBuff[SprNum] ^ ((PPU.SprBuff[SprNum | 2] & 0x80) ? 0x7 : 0x0)) | ((PPU.Reg2000 & 0x08) << 9);
				break;
			case 260:	case 268:	case 276:	case 284:	case 292:	case 300:	case 308:	case 316:
				PPU.RenderAddr = PatAddr;
				break;
			case 261:
				if (PPU.Spr0InLine)
				{
					if (PPU.SprBuff[2] & 0x40)
						TC = RenderData[2];
					else	TC = ReverseCHR[RenderData[2]];
					((unsigned long *)PPU.SprData[0])[0] = CHRLoBit[TC & 0xF];
					((unsigned long *)PPU.SprData[0])[1] = CHRLoBit[TC >> 4];
				}
				break;
			case 269:	case 277:	case 285:	case 293:	case 301:	case 309:	case 317:
				break;
			case 262:	case 270:	case 278:	case 286:	case 294:	case 302:	case 310:	case 318:
				PPU.RenderAddr = PatAddr | 8;
				break;
			case 263:
				if (PPU.Spr0InLine)
				{
					SprNum = (PPU.Clockticks >> 1) & 0x1E;
					if (PPU.SprBuff[2] & 0x40)
						TC = RenderData[3];
					else	TC = ReverseCHR[RenderData[3]];
					((unsigned long *)PPU.SprData[0])[0] |= CHRHiBit[TC & 0xF];
					((unsigned long *)PPU.SprData[0])[1] |= CHRHiBit[TC >> 4];
				}
				break;
			case 271:	case 279:	case 287:	case 295:	case 303:	case 311:	case 319:
				break;
				/* END SPRITES */
			case 336:	case 338:
				PPU.RenderAddr = 0x2000 | (PPU.VRAMAddr & 0xFFF);
			case 337:	case 339:
				break;
			case 340:
				break;
			}
			if (!(PPU.Clockticks & 1))
			{
				PPU_PPUCycle(PPU.RenderAddr,PPU.SLnum,PPU.Clockticks,1);
				if (PPU.IOMode == 2)
					PPU.WriteHandler[PPU.RenderAddr >> 10](PPU.RenderAddr >> 10,PPU.RenderAddr & 0x3FF,PPU.RenderAddr & 0xFF);
			}
		}
		if (PPU.IOMode)
		{
			unsigned short addr = (unsigned short)(PPU.IOAddr & 0x3FFF);
			if ((PPU.IOMode >= 5) && (!PPU.IsRendering))
				PPU_PPUCycle(addr,PPU.SLnum,PPU.Clockticks,PPU.IsRendering);
			else if (PPU.IOMode == 2)
			{
				if (!PPU.IsRendering)
					PPU.WriteHandler[addr >> 10](addr >> 10,addr & 0x3FF,PPU.IOVal);
			}
			else if (PPU.IOMode == 1)
			{
				PPU.IOMode++;
				if (!PPU.IsRendering)
				{
					if (PPU.ReadHandler[addr >> 10] == PPU_BusRead)
						PPU.buf2007 = PPU.CHRPointer[addr >> 10][addr & 0x3FF];
					else	PPU.buf2007 = PPU.ReadHandler[addr >> 10](addr >> 10,addr & 0x3FF);
				}
			}
			PPU.IOMode -= 2;
		}
		if (!PPU.IsRendering && !PPU.IOMode)
			PPU_PPUCycle(PPU.VRAMAddr,PPU.SLnum,PPU.Clockticks,0);
		if ((PPU.Spr0InLine) && (PPU.Clockticks < 255) && (PPU.OnScreen) && ((PPU.Reg2001 & 0x18) == 0x18) && ((PPU.Clockticks >= 8) || ((PPU.Reg2001 & 0x06) == 0x06)))
		{
			register int SprPixel = PPU.Clockticks - PPU.SprBuff[3];
			if (!(SprPixel & ~7) && (PPU.SprData[0][SprPixel] & 0x3) && (PPU.TileData[PPU.Clockticks + PPU.IntX] & 0x3))
				PPU.Reg2002 |= 0x40;	/* Sprite 0 hit */
		}
	}
}

void	PPU_Run (void)
{
	if (PPU.IsPAL)
	{
		register int cycles = 3;
		if (++PPU.PALsubticks == 5)
		{
			PPU.PALsubticks = 0;
			cycles = 4;
		}
		if (GFX.FPSCnt < GFX.FSkip)
			RunSkip(cycles);
		else	RunNoSkip(cycles);
	}
	else
	{
		if (GFX.FPSCnt < GFX.FSkip)
			RunSkip(3);
		else	RunNoSkip(3);
	}
}

static	int	__fastcall	Read01356 (void)
{
	return PPU.ppuLatch;
}

static	int	__fastcall	Read2 (void)
{
	register unsigned char tmp;
	PPU.HVTog = TRUE;
	tmp = PPU.Reg2002 | (PPU.ppuLatch & 0x1F);
	if (tmp & 0x80)
		PPU.Reg2002 &= 0x60;
	return PPU.ppuLatch = tmp;
}

static	int	__fastcall	Read4 (void)
{
	if (PPU.IsRendering)
	{
		if (PPU.Clockticks < 64)
			PPU.ppuLatch = 0xFF;
		else if (PPU.Clockticks < 192)
			PPU.ppuLatch = PPU.Sprite[((PPU.Clockticks - 64) << 1) & 0xFC];
		else if (PPU.Clockticks < 256)
			PPU.ppuLatch = (PPU.Clockticks & 1) ? PPU.Sprite[0xFC] : PPU.Sprite[((PPU.Clockticks - 192) << 1) & 0xFC];
		else if (PPU.Clockticks < 320)
			PPU.ppuLatch = 0xFF;
		else	PPU.ppuLatch = PPU.Sprite[0];
	}
	else	PPU.ppuLatch = PPU.Sprite[PPU.SprAddr];
	return PPU.ppuLatch;
}

static	int	__fastcall	Read7 (void)
{
	PPU.IOAddr = PPU.VRAMAddr & 0x3FFF;
	PPU.IOMode = 5;
	if (PPU.Reg2000 & 0x04)
		PPU.VRAMAddr += 32;
	else	PPU.VRAMAddr++;
	PPU.VRAMAddr &= 0x7FFF;
	if ((PPU.IOAddr & 0x3F00) == 0x3F00)
	{
		if (PPU.Reg2001 & 0x01)
			return PPU.ppuLatch = PPU.Palette[PPU.IOAddr & 0x1F] & 0x30;
		else	return PPU.ppuLatch = PPU.Palette[PPU.IOAddr & 0x1F];
	}
	else	return PPU.ppuLatch = PPU.buf2007;
}

int	_MAPINT	PPU_IntRead (int Bank, int Addr)
{
	static int (__fastcall *funcs[8])(void) = {Read01356,Read01356,Read2,Read01356,Read4,Read01356,Read01356,Read7};
	return funcs[Addr & 7]();
}

static	void	__fastcall	Write0 (int Val)
{
	if ((Val & 0x80) && !(PPU.Reg2000 & 0x80) && (PPU.Reg2002 & 0x80))
		CPU.WantNMI = TRUE;
	PPU.Reg2000 = Val;
	PPU.IntReg &= 0x73FF;
	PPU.IntReg |= (Val & 3) << 10;
#ifdef ENABLE_DEBUGGER
	Debugger.NTabChanged = TRUE;
#endif	/* ENABLE_DEBUGGER */
}

static	void	__fastcall	Write1 (int Val)
{
	PPU.Reg2001 = Val;
	if ((Val & 0x18) && (PPU.SLnum < 240))
		PPU.IsRendering = TRUE;
	else	PPU.IsRendering = FALSE;
	PPU.ColorEmphasis = (Val & 0xE0) << 1;
	PPU.GrayScale = (Val & 0x01) ? 0x30 : 0x3F;
}

static	void	__fastcall	Write2 (int Val)
{
}

static	void	__fastcall	Write3 (int Val)
{
	PPU.SprAddr = Val;
}

static	void	__fastcall	Write4 (int Val)
{
	if (PPU.IsRendering)
		Val = 0xFF;
	if ((PPU.SprAddr & 0x03) == 0x02)
		Val &= 0xE3;
	PPU.Sprite[PPU.SprAddr++] = Val;
}

static	void	__fastcall	Write5 (int Val)
{
	if (PPU.HVTog)
	{
		PPU.IntReg &= 0x7FE0;
		PPU.IntReg |= (Val & 0xF8) >> 3;
		PPU.IntX = Val & 7;
	}
	else
	{
		PPU.IntReg &= 0x0C1F;
		PPU.IntReg |= (Val & 0x07) << 12;
		PPU.IntReg |= (Val & 0xF8) << 2;
	}
	PPU.HVTog = !PPU.HVTog;
}

static	void	__fastcall	Write6 (int Val)
{
	if (PPU.HVTog)
	{
		PPU.IntReg &= 0x00FF;
		PPU.IntReg |= (Val & 0x3F) << 8;
	}
	else
	{
		PPU.IntReg &= 0x7F00;
		PPU.IntReg |= Val;
		PPU.VRAMAddr = PPU.IntReg;
	}
	PPU.HVTog = !PPU.HVTog;
}

static	void	__fastcall	Write7 (int Val)
{
	if ((PPU.VRAMAddr & 0x3F00) == 0x3F00)
	{
		register unsigned char Addr = (unsigned char)PPU.VRAMAddr & 0x1F;
		register unsigned Val = Val & 0x3F;
#ifdef ENABLE_DEBUGGER
		Debugger.PalChanged = TRUE;
#endif	/* ENABLE_DEBUGGER */
		PPU.Palette[Addr] = Val;
		if (!(Addr & 0x3))
			PPU.Palette[Addr ^ 0x10] = Val;
	}
	else
	{
		PPU.IOAddr = PPU.VRAMAddr & 0x3FFF;
		PPU.IOVal = Val;
		PPU.IOMode = 6;
	}
	if (PPU.Reg2000 & 0x04)
		PPU.VRAMAddr += 32;
	else	PPU.VRAMAddr++;
	PPU.VRAMAddr &= 0x7FFF;
}

void	_MAPINT	PPU_IntWrite (int Bank, int Addr, int Val)
{
	static void (__fastcall *funcs[8])(int) = {Write0,Write1,Write2,Write3,Write4,Write5,Write6,Write7};
	PPU.ppuLatch = Val;
	funcs[Addr & 7](Val);
}
