/* Nintendulator Mapper DLLs
 * Copyright (C) 2002-2010 QMT Productions
 *
 * $URL$
 * $Id$
 */

#include	"..\DLL\d_iNES.h"
#include	"..\Hardware\h_Latch.h"
#include	"resource.h"

namespace
{
u8 Jumper;
HWND ConfigWindow;
u8 ConfigCmd;
u8 JumperData[0x1000];

void	Sync (void)
{
	EMU->SetCHR_ROM8(0, Latch::Addr.s0 & 0x07);
	EMU->SetPRG_ROM32(0x8, (Latch::Addr.s0 & 0x70) >> 4);
	if (Latch::Addr.s0 & 0x08)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
	if (Latch::Addr.s0 & 0x100)
	{
		u8 x;
		memset(JumperData, Jumper, 0x1000);
		for (x = 0x8; x < 0x10; x++)
			EMU->SetPRG_Ptr4(x, JumperData, FALSE);
	}
}

int	MAPINT	SaveLoad (STATE_TYPE mode, int x, unsigned char *data)
{
	x = Latch::SaveLoad_A(mode, x, data);
	SAVELOAD_BYTE(mode, x, data, Jumper);
	if (mode == STATE_LOAD)
		Sync();
	return x;
}

LRESULT CALLBACK ConfigProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			CheckDlgButton(hDlg, IDC_MAPPER59_J0, (Jumper & 0x01) ? BST_CHECKED : BST_UNCHECKED);
			CheckDlgButton(hDlg, IDC_MAPPER59_J1, (Jumper & 0x02) ? BST_CHECKED : BST_UNCHECKED);
			return FALSE;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDOK:
				ConfigCmd = 0x80;
				if (IsDlgButtonChecked(hDlg, IDC_MAPPER59_J0) == BST_CHECKED)
					ConfigCmd |= 0x01;
				if (IsDlgButtonChecked(hDlg, IDC_MAPPER59_J1) == BST_CHECKED)
					ConfigCmd |= 0x02;
				MessageBox(hDlg, _T("Please perform a SOFT RESET for this to take effect!"), _T("INES.DLL"), MB_OK);
			case IDCANCEL:
				DestroyWindow(hDlg);
				ConfigWindow = NULL;
				return TRUE;		break;
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hDlg);
			ConfigWindow = NULL;
			return TRUE;		break;
	}
	return FALSE;
}

unsigned char	MAPINT	Config (CFG_TYPE mode, unsigned char data)
{
	switch (mode)
	{
	case CFG_WINDOW:
		if (data)
		{
			if (ConfigWindow)
				break;
			ConfigWindow = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAPPER59), hWnd, (DLGPROC)ConfigProc);
			SetWindowPos(ConfigWindow, hWnd, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
		}
		else	return FALSE;
		break;
	case CFG_QUERY:
		return ConfigCmd;
		break;
	case CFG_CMD:
		if (data & 0x80)
		{
			Jumper = data & 0x3;
			Sync();
		}
		ConfigCmd = 0;
		break;
	}
	return 0;
}

void	MAPINT	Load (void)
{
	Latch::Load(Sync, FALSE);
	ConfigWindow = NULL;
}
void	MAPINT	Reset (RESET_TYPE ResetType)
{
	Latch::Reset(ResetType);
	ConfigCmd = 0;
}
void	MAPINT	Unload (void)
{
	Latch::Unload();
	if (ConfigWindow)
		DestroyWindow(ConfigWindow);
}

u8 MapperNum = 59;
} // namespace

CTMapperInfo	MapperInfo_059 =
{
	&MapperNum,
	_T("T3H53"),
	COMPAT_FULL,
	Load,
	Reset,
	Unload,
	NULL,
	NULL,
	SaveLoad,
	NULL,
	Config
};