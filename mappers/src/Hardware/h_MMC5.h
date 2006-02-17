#ifndef	__MMC5_H__
#define	__MMC5_H__

#include	"..\interface.h"

#define	MMC5WRAM_0KB_0KB	0
#define	MMC5WRAM_8KB_0KB	1
#define	MMC5WRAM_32KB_0KB	2
#define	MMC5WRAM_0KB_8KB	3
#define	MMC5WRAM_8KB_8KB	4
#define	MMC5WRAM_32KB_8KB	5
#define	MMC5WRAM_0KB_32KB	6
#define	MMC5WRAM_8KB_32KB	7
#define	MMC5WRAM_32KB_32KB	8
#define	MMC5WRAM_MAXOPTS	9

typedef	struct	MMC5
{
	int WRAMtable[MMC5WRAM_MAXOPTS][8];
	u8 PRGsize, CHRsize, WRAMsize;
	u8 IRQenabled, IRQreads, IRQline;
	u8 Mirror;
	u8 Mul1, Mul2, GfxMode;
	u8 CHR_A[8], CHR_B[4];
	u8 PRG[5];
	u8 WRAMprot[2];
	u8 SplitMode;
	u8 SplitBank;
	u8 SplitScroll;
	u8 *NameTable0, *NameTable1, *ExRAM, *ExNameTable;
	u8 TileCache;
	u8 DrawStatus, DrawBank, CurTile;
	FCPUWrite CPUWrite6F;
	FPPURead PPURead[16];
}	TMMC5, *PMMC5;

extern	TMMC5	MMC5;

void		MMC5_Init	(void);
void		MMC5_Destroy	(void);
int	_MAPINT	MMC5_SaveLoad	(int,int,char *);
void		MMC5_SetPRG	(int,int,int);
void		MMC5_SyncPRG	(void);
void		MMC5_SyncCHR	(void);
void		MMC5_SyncCHRA	(void);
void		MMC5_SyncCHRB	(void);
void		MMC5_SyncMirror	(void);
int	_MAPINT	MMC5_CPURead5	(int,int);
void	_MAPINT	MMC5_CPUWrite5	(int,int,int);
void	_MAPINT	MMC5_CPUWrite6F	(int,int,int);
void	_MAPINT	MMC5_PPUCycle	(int,int,int,int);
int	_MAPINT	MMC5_MapperSnd	(int);

#endif	/* __MMC5_H__ */