/*
Nintendulator - A Win32 NES emulator written in C.
Designed for maximum emulation accuracy.
Copyright (c) 2002  Quietust

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

#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>

#define	CONTROLLERS_MAXBUTTONS	32

struct tStdPort
{
	unsigned char	(*Read)		(struct tStdPort *);
	void		(*Write)	(struct tStdPort *,unsigned char);
	void		(*Config)	(struct tStdPort *,HWND);
	void		(*Unload)	(struct tStdPort *);
	void		(*Frame)	(struct tStdPort *,unsigned char);
	int		Type;
	int		Buttons[CONTROLLERS_MAXBUTTONS];
	int		NumButtons;
	int		DataLen;
	unsigned long	*Data;
	int		MovLen;
	unsigned char	*MovData;
};
enum	STDCONT_TYPE	{ STD_UNCONNECTED, STD_STDCONTROLLER, STD_ZAPPER, STD_ARKANOIDPADDLE, STD_POWERPAD, STD_FOURSCORE, STD_SNESCONTROLLER, STD_MAX };
void	StdPort_SetControllerType (struct tStdPort *,int);

struct tExpPort
{
	unsigned char	(*Read1)	(struct tExpPort *);
	unsigned char	(*Read2)	(struct tExpPort *);
	void		(*Write)	(struct tExpPort *,unsigned char);
	void		(*Config)	(struct tExpPort *,HWND);
	void		(*Unload)	(struct tExpPort *);
	void		(*Frame)	(struct tExpPort *,unsigned char);
	int		Type;
	int		Buttons[CONTROLLERS_MAXBUTTONS];
	int		NumButtons;
	int		DataLen;
	unsigned long	*Data;
	int		MovLen;
	unsigned char	*MovData;
};
enum	EXPCONT_TYPE	{ EXP_UNCONNECTED, EXP_FAMI4PLAY, EXP_ARKANOIDPADDLE, EXP_FAMILYBASICKEYBOARD, EXP_ALTKEYBOARD, EXP_FAMTRAINER, EXP_TABLET, EXP_MAX };
void	ExpPort_SetControllerType (struct tExpPort *,int);

#define	MOV_PLAY	0x01
#define	MOV_RECORD	0x02
#define	MOV_REVIEW	0x04

struct tControllers
{
	struct tStdPort Port1, Port2;
	struct tStdPort FSPort1, FSPort2, FSPort3, FSPort4;
	struct tExpPort ExpPort;


	int	NumDevices;
	BOOL	DeviceUsed[32];
	char	DeviceName[32][64];
	int	NumButtons[32], NumAxes[32];

	unsigned char	MovieMode;

	BYTE		KeyState[256];
	DIMOUSESTATE2	MouseState;
	DIJOYSTATE2	JoyState[30];

	LPDIRECTINPUT7		DirectInput;
	LPDIRECTINPUTDEVICE7	DIKeyboard;
	LPDIRECTINPUTDEVICE7	DIMouse;
	LPDIRECTINPUTDEVICE7	DIJoystick[30];
};
extern struct tControllers Controllers;

void	Controllers_OpenConfig (void);
void	Controllers_Init (void);
void	Controllers_Release (void);
void	Controllers_Write (unsigned char);
void	Controllers_SetDeviceUsed (void);
void	Controllers_Acquire (void);
void	Controllers_UnAcquire (void);
void	Controllers_UpdateInput (void);
void	Controllers_ConfigButton (int *,int,HWND,BOOL);

BOOL	Controllers_IsPressed (int);
void	Controllers_ParseConfigMessages (HWND,int,int *,int *,int *,UINT,WPARAM,LPARAM);

void	Controllers_ShowFrame (void);
void	Controllers_PlayMovie (BOOL);
void	Controllers_RecordMovie (BOOL);
void	Controllers_StopMovie (void);

#endif /* CONTROLLERS_H */