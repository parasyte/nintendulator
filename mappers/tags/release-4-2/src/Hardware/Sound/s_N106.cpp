/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\..\interface.h"
#include	"s_N106.h"

// Namco 106
namespace N106sound
{
u8 regs[0x80];
u8 chans;
u8 addr;
u8 inc;
struct	N106chan
{
	u8 freql, freqm, freqh;
	u32 freq;
	u8 len;
	u8 addr;
	u8 volume;
	u8 CurP;
	u8 CurA;
	s32 LCtr;

	int	GenerateWave (int Cycles)
	{
		s32 _freq;
		if (!freq)
			return 0;
		_freq = (0xF0000 * chans) / freq;
		LCtr += Cycles;
		while (LCtr > _freq)
		{
			u8 _addr;
			CurA++;
			while (CurA >= len)
				CurA -= len;
			_addr = addr + CurA;
			CurP = regs[_addr >> 1];
			if (_addr & 1)
				CurP >>= 4;
			else	CurP &= 0xF;
			LCtr -= _freq;
		}
		return (CurP - 0x8) * volume;
	}
	void	Write (int addr, int Val)
	{
		switch (addr & 0x7)
		{
		case 0:	freql = Val;
			freq = freql | (freqm << 8) | (freqh << 16);
			break;
		case 2:	freqm = Val;
			freq = freql | (freqm << 8) | (freqh << 16);
			break;
		case 4:	freqh = Val & 3;
			freq = freql | (freqm << 8) | (freqh << 16);
			if (len != 0x20 - (Val & 0x1C))
			{
				len = 0x20 - (Val & 0x1C);
				CurA = 0;
			}
			break;
		case 6:	addr = Val;
			break;
		case 7:	volume = Val & 0xF;
			break;
		}
	}
} Ch[8];

void	Load (void)
{
	ZeroMemory(Ch, sizeof(Ch));
	ZeroMemory(regs, sizeof(regs));
	chans = 0;
	addr = 0;
	inc = 0;
}

void	Reset (RESET_TYPE ResetType)
{
	int i;
	for (i = 0; i < 8; i++)
		Ch[i].len = 0x10;
	chans = 8;
	inc = 0x80;
}

void	Unload (void)
{
}

void	Write (int Addr, int Val)
{
	switch (Addr & 0xF800)
	{
	case 0xF800:
		addr = Val & 0x7F;
		inc = Val & 0x80;
		break;
	case 0x4800:
		regs[addr] = Val;
		if (addr & 0x40)
		{
			Ch[(addr & 0x38) >> 3].Write(addr & 0x7, Val);
			if (addr == 0x3F)
				chans = 1 + ((Val >> 4) & 0x7);
		}
		if (inc)
		{
			addr++;
			addr &= 0x7F;
		}
		break;
	}
}

int	Read (int Addr)
{
	int data = regs[addr];

	if (inc)
	{
		addr++;
		addr &= 0x7F;
	}

	return data;
}

int	MAPINT	Get (int Cycles)
{
	int out = 0;
	int i;
	for (i = 8 - chans; i < 8; i++)
		out += Ch[i].GenerateWave(Cycles);
	return out << 5;
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	int i;
	switch (mode)
	{
	case STATE_SAVE:
		for (i = 0; i < 0x80; i++)
			data[x++] = regs[i];
		data[x++] = addr | inc;
		break;
	case STATE_LOAD:
		Write(0xF800, 0x80);
		for (i = 0; i < 0x80; i++)
			Write(0x4800, data[x++]);
		Write(0xF800, data[x++]);
		break;
	case STATE_SIZE:
		x += 0x81;
		break;
	}
	for (i = 0; i < 8; i++)
	{
		SAVELOAD_BYTE(mode, x, data, Ch[i].CurP);
		SAVELOAD_BYTE(mode, x, data, Ch[i].CurA);
		SAVELOAD_LONG(mode, x, data, Ch[i].LCtr);
	}
	return x;
}
} // namespace N106sound