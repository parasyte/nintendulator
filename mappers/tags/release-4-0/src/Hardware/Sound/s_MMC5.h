#ifndef	S_MMC5_H
#define	S_MMC5_H

void		MMC5sound_Load		(void);
void		MMC5sound_Reset		(RESET_TYPE);
void		MMC5sound_Unload	(void);
void		MMC5sound_Write		(int,int);
int		MMC5sound_Read		(int);
int	_MAPINT	MMC5sound_Get		(int);
int	_MAPINT	MMC5sound_SaveLoad	(STATE_TYPE,int,unsigned char *);

#endif	/* S_MMC5_H */