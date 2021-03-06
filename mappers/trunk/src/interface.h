/* Nintendulator Mapper Interface
 * Copyright (C) 2002-2011 QMT Productions
 *
 * $URL$
 * $Id$
 */

#pragma once

#ifndef	WIN32_LEAN_AND_MEAN
#define	WIN32_LEAN_AND_MEAN
#endif

/* Standard header files, used by all mappers */

#include	<windows.h>
#include	<tchar.h>

#pragma warning(disable:4100)	// "unreferenced formal parameter" - functions which don't use every parameter
#pragma warning(disable:4127)	// "conditional expression is constant" - do {} while (0)
#pragma warning(disable:4201)	// "nonstandard extension used : nameless struct/union" - used in lots of places
#pragma warning(disable:4244)	// "conversion from 'foo' to 'bar', possible loss of data" - I/O handlers all pass 'int' values and get crammed into bytes/shorts

#define	MSGBOX_FLAGS	(MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL)

/* Mapper Interface version (3.8) */

#ifdef	UNICODE
#define	CurrentMapperInterface	0x80030008
#else	/* !UNICODE */
#define	CurrentMapperInterface	0x00030008
#endif	/* UNICODE */

/* Integer types */

typedef	signed __int8		int8;
typedef	signed __int16		int16;
typedef	signed __int32		int32;
typedef	signed __int64		int64;

typedef	unsigned __int8		uint8;
typedef	unsigned __int16	uint16;
typedef	unsigned __int32	uint32;
typedef	unsigned __int64	uint64;

typedef	union
{
	struct
	{
		unsigned n0 : 4;
		unsigned n1 : 4;
	};
	uint8 b[1];
}	uint8_n;

typedef	union
{
	struct
	{
		unsigned n0 : 4;
		unsigned n1 : 4;
		unsigned n2 : 4;
		unsigned n3 : 4;
	};
	uint8 b[2];
	uint16 s[1];
}	uint16_n;

typedef	union
{
	struct
	{
		unsigned n0 : 4;
		unsigned n1 : 4;
		unsigned n2 : 4;
		unsigned n3 : 4;
		unsigned n4 : 4;
		unsigned n5 : 4;
		unsigned n6 : 4;
		unsigned n7 : 4;
	};
	uint8 b[4];
	uint16 s[2];
	uint32 l[1];
}	uint32_n;

#define b0 b[0]
#define b1 b[1]
#define b2 b[2]
#define b3 b[3]

#define s0 s[0]
#define s1 s[1]

#define l0 l[0]

/* Function types */

#define	MAPINT	__cdecl

typedef	void	(MAPINT *FCPUWrite)	(int Bank,int Addr,int Val);
typedef	int	(MAPINT *FCPURead)	(int Bank,int Addr);

typedef	void	(MAPINT *FPPUWrite)	(int Bank,int Addr,int Val);
typedef	int	(MAPINT *FPPURead)	(int Bank,int Addr);

typedef	void	(*FSync)	(void);	/* for Sync() callbacks */

/* Mapper Interface Structure - Pointers to data and functions within emulator */

struct	EmulatorInterface
{
/* Functions for managing read/write handlers */
	void		(MAPINT *SetCPUReadHandler)	(int,FCPURead);
	void		(MAPINT *SetCPUWriteHandler)	(int,FCPUWrite);
	FCPURead	(MAPINT *GetCPUReadHandler)	(int);
	FCPUWrite	(MAPINT *GetCPUWriteHandler)	(int);

	void		(MAPINT *SetPPUReadHandler)	(int,FPPURead);
	void		(MAPINT *SetPPUWriteHandler)	(int,FPPUWrite);
	FPPURead	(MAPINT *GetPPUReadHandler)	(int);
	FPPUWrite	(MAPINT *GetPPUWriteHandler)	(int);

/* Functions for mapping PRG */
	void		(MAPINT *SetPRG_ROM4)		(int,int);
	void		(MAPINT *SetPRG_ROM8)		(int,int);
	void		(MAPINT *SetPRG_ROM16)		(int,int);
	void		(MAPINT *SetPRG_ROM32)		(int,int);
	int		(MAPINT *GetPRG_ROM4)		(int);		/* -1 if no ROM mapped */
#define		GetPRG_ROM8(x)	GetPRG_ROM4(x) >> 1
#define		GetPRG_ROM16(x)	GetPRG_ROM4(x) >> 2
	void		(MAPINT *SetPRG_RAM4)		(int,int);
	void		(MAPINT *SetPRG_RAM8)		(int,int);
	void		(MAPINT *SetPRG_RAM16)		(int,int);
	void		(MAPINT *SetPRG_RAM32)		(int,int);
	int		(MAPINT *GetPRG_RAM4)		(int);		/* -1 if no RAM mapped */
#define		GetPRG_RAM8(x)	GetPRG_RAM4(x) >> 1

	unsigned char *	(MAPINT *GetPRG_Ptr4)		(int);
	void		(MAPINT *SetPRG_Ptr4)		(int,unsigned char *,BOOL);
	void		(MAPINT *SetPRG_OB4)		(int);		/* Open bus */

/* Functions for mapping CHR */
	void		(MAPINT *SetCHR_ROM1)		(int,int);
	void		(MAPINT *SetCHR_ROM2)		(int,int);
	void		(MAPINT *SetCHR_ROM4)		(int,int);
	void		(MAPINT *SetCHR_ROM8)		(int,int);
	int		(MAPINT *GetCHR_ROM1)		(int);		/* -1 if no ROM mapped */

	void		(MAPINT *SetCHR_RAM1)		(int,int);
	void		(MAPINT *SetCHR_RAM2)		(int,int);
	void		(MAPINT *SetCHR_RAM4)		(int,int);
	void		(MAPINT *SetCHR_RAM8)		(int,int);
	int		(MAPINT *GetCHR_RAM1)		(int);		/* -1 if no RAM mapped */

	void		(MAPINT *SetCHR_NT1)		(int,int);
	int		(MAPINT *GetCHR_NT1)		(int);		/* -1 if no nametable mapped */

	unsigned char *	(MAPINT *GetCHR_Ptr1)		(int);
	void		(MAPINT *SetCHR_Ptr1)		(int,unsigned char *,BOOL);
	void		(MAPINT *SetCHR_OB1)		(int);		/* Open bus */

/* Functions for controlling mirroring */
	void		(MAPINT *Mirror_H)		(void);
	void		(MAPINT *Mirror_V)		(void);
	void		(MAPINT *Mirror_4)		(void);
	void		(MAPINT *Mirror_S0)		(void);
	void		(MAPINT *Mirror_S1)		(void);
	void		(MAPINT *Mirror_Custom)	(int,int,int,int);

/* IRQ */
	void		(MAPINT *SetIRQ)		(int);		/* Sets the state of the /IRQ line */

/* Save RAM Handling */
	void		(MAPINT *Set_SRAMSize)		(int);		/* Sets the size of the SRAM (in bytes) */

/* Misc Callbacks */
	void		(MAPINT *DbgOut)		(TCHAR *,...);	/* Echo text to debug window */
	void		(MAPINT *StatusOut)		(TCHAR *,...);	/* Echo text on status bar */
/* Data fields */
	unsigned char *	OpenBus;			/* pointer to last value on the CPU data bus */
};

enum COMPAT_TYPE	{ COMPAT_NONE, COMPAT_PARTIAL, COMPAT_NEARLY, COMPAT_FULL, COMPAT_NUMTYPES };

/* Mapper Information structure - Contains pointers to mapper functions, sent to emulator on load mapper  */

enum RESET_TYPE	{ RESET_NONE, RESET_SOFT, RESET_HARD };

enum STATE_TYPE	{ STATE_SIZE, STATE_SAVE, STATE_LOAD };

enum CFG_TYPE	{ CFG_WINDOW, CFG_QUERY, CFG_CMD };

struct	MapperInfo
{
/* Mapper Information */
	void *		MapperId;
	TCHAR *		Description;
	COMPAT_TYPE	Compatibility;

/* Mapper Functions */
	BOOL		(MAPINT *Load)		(void);
	void		(MAPINT *Reset)	(RESET_TYPE);		/* ResetType */
	void		(MAPINT *Unload)	(void);
	void		(MAPINT *CPUCycle)	(void);
	void		(MAPINT *PPUCycle)	(int,int,int,int);	/* Address, Scanline, Cycle, IsRendering */
	int		(MAPINT *SaveLoad)	(STATE_TYPE,int,unsigned char *);	/* Mode, Offset, Data */
	int		(MAPINT *GenSound)	(int);			/* Cycles */
	unsigned char	(MAPINT *Config)	(CFG_TYPE,unsigned char);	/* Mode, Data */
};

#define	SAVELOAD_BYTE(mode,offset,data,value) \
do { \
	if (mode == STATE_SAVE) data[offset++] = value; \
	else if (mode == STATE_LOAD) value = data[offset++]; \
	else if (mode == STATE_SIZE) offset++; \
	else MessageBox(hWnd,_T("Invalid save/load type!"),_T(__FILE__),MB_OK); \
} while (0)
#define	SAVELOAD_WORD(mode,offset,data,value) \
do { \
	uint16_n sl_tmp; \
	if (mode == STATE_SAVE) { sl_tmp.s0 = value; data[offset++] = sl_tmp.b0; data[offset++] = sl_tmp.b1; } \
	else if (mode == STATE_LOAD) { sl_tmp.b0 = data[offset++]; sl_tmp.b1 = data[offset++]; value = sl_tmp.s0; } \
	else if (mode == STATE_SIZE) offset += 2; \
	else MessageBox(hWnd,_T("Invalid save/load type!"),_T(__FILE__),MB_OK); \
} while (0)
#define	SAVELOAD_LONG(mode,offset,data,value) \
do { \
	uint32_n sl_tmp; \
	if (mode == STATE_SAVE) { sl_tmp.l0 = value; data[offset++] = sl_tmp.b0; data[offset++] = sl_tmp.b1; data[offset++] = sl_tmp.b2; data[offset++] = sl_tmp.b3; } \
	else if (mode == STATE_LOAD) { sl_tmp.b0 = data[offset++]; sl_tmp.b1 = data[offset++]; sl_tmp.b2 = data[offset++]; sl_tmp.b3 = data[offset++]; value = sl_tmp.l0; } \
	else if (mode == STATE_SIZE) offset += 4; \
	else MessageBox(hWnd,_T("Invalid save/load type!"),_T(__FILE__),MB_OK); \
} while (0)

/* ROM Information Structure - Contains information about the ROM currently loaded */

enum ROM_TYPE	{ ROM_UNDEFINED, ROM_INES, ROM_UNIF, ROM_FDS, ROM_NSF, ROM_NUMTYPES };

struct	ROMInfo
{
	TCHAR *		Filename;
	ROM_TYPE	ROMType;
	union
	{
		struct
		{
			WORD	INES_MapperNum;
			BYTE	INES_Flags;	/* byte 6 flags in lower 4 bits, byte 7 flags in upper 4 bits */
			WORD	INES_PRGSize;	/* number of 16KB banks */
			WORD	INES_CHRSize;	/* number of 8KB banks */
			BYTE	INES_Version;	/* 1 for standard .NES, 2 Denotes presence of NES 2.0 data */
			BYTE	INES2_SubMapper;	/* NES 2.0 - submapper */
			BYTE	INES2_TVMode;	/* NES 2.0 - NTSC/PAL indicator */
			BYTE	INES2_PRGRAM;	/* NES 2.0 - PRG RAM counts, batteried and otherwise */
			BYTE	INES2_CHRRAM;	/* NES 2.0 - CHR RAM counts, batteried and otherwise */
			BYTE	INES2_VSDATA;	/* NES 2.0 - VS Unisystem information */
		};	/* INES */
		struct
		{
			char *	UNIF_BoardName;
			BYTE	UNIF_Mirroring;
			BYTE	UNIF_NTSCPAL;	/* 0 for NTSC, 1 for PAL, 2 for either */
			BOOL	UNIF_Battery;
			BYTE	UNIF_NumPRG;	/* number of PRG# blocks */
			BYTE	UNIF_NumCHR;	/* number of CHR# blocks */
			DWORD	UNIF_PRGSize[16];	/* size of each PRG block, in bytes */
			DWORD	UNIF_CHRSize[16];	/* size of each CHR block, in bytes */
		};	/* UNIF */
		struct
		{
			BYTE	FDS_NumSides;
		};	/* FDS */
		struct
		{
			BYTE	NSF_NumSongs;
			BYTE	NSF_InitSong;
			WORD	NSF_InitAddr;
			WORD	NSF_PlayAddr;
			char *	NSF_Title;
			char *	NSF_Artist;
			char *	NSF_Copyright;
			WORD	NSF_NTSCSpeed;	/* Number of microseconds between PLAY calls for NTSC */
			WORD	NSF_PALSpeed;	/* Number of microseconds between PLAY calls for PAL */
			BYTE	NSF_NTSCPAL;	/* 0 for NTSC, 1 for PAL, 2 for either */
			BYTE	NSF_SoundChips;
			BYTE	NSF_InitBanks[8];
			DWORD	NSF_DataSize;	/* total amount of (PRG) data present */
		};	/* NSF */
		struct
		{
			BYTE	reserved[256];
		};	/* reserved for additional file types */
	};
};

/* DLL Information Structure - Contains general information about the mapper DLL */

struct	DLLInfo
{
	TCHAR *		Description;
	int		Date;
	int		Version;
	const MapperInfo *	(MAPINT *LoadMapper)	(const ROMInfo *);
	void		(MAPINT *UnloadMapper)	(void);
};

extern	HWND			hWnd;
extern	HINSTANCE		hInstance;
extern	const EmulatorInterface	*EMU;
extern	const ROMInfo		*ROM;
