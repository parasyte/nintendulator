#ifndef	__MMC2_H__
#define	__MMC2_H__

#include	"..\interface.h"

typedef	struct	MMC2
{
	u8 PRG;
	u8 LatchState[2];
	u8 Latch0[2];
	u8 Latch1[2];
	u8 Mirror;
	void	(*Sync)	(void);
	FPPURead PPURead3, PPURead7;
}	TMMC2, *PMMC2;

void		MMC2_Init	(void (*)(void));
void		MMC2_Destroy	(void);
int	_MAPINT	MMC2_SaveLoad	(int,int,char *);
void		MMC2_SyncPRG	(int,int);
void		MMC2_SyncCHR	(void);
void		MMC2_SyncMirror	(void);
void	_MAPINT	MMC2_CPUWriteA	(int,int,int);
void	_MAPINT	MMC2_CPUWriteB	(int,int,int);
void	_MAPINT	MMC2_CPUWriteC	(int,int,int);
void	_MAPINT	MMC2_CPUWriteD	(int,int,int);
void	_MAPINT	MMC2_CPUWriteE	(int,int,int);
void	_MAPINT	MMC2_CPUWriteF	(int,int,int);
int	_MAPINT	MMC2_PPURead3	(int,int);
int	_MAPINT	MMC2_PPURead7	(int,int);

#endif	/* __MMC2_H__ */