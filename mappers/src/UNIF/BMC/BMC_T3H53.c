#include	"..\..\DLL\d_UNIF.h"
#include	"..\..\Hardware\h_Latch.h"
#include	"..\resource.h"

static	struct
{
	u8 Jumper;
	HWND ConfigWindow;
	u8 ConfigCmd;
}	Mapper;

static	void	Sync (void)
{
	u8 x;
	EMU->SetCHR_ROM8(0,Latch.Addr.s0 & 0x07);
	if (Latch.Addr.b0 & 0x80)
	{
		EMU->SetPRG_ROM16(0x8,(Latch.Addr.s0 & 0x70) >> 4);
		EMU->SetPRG_ROM16(0xC,(Latch.Addr.s0 & 0x70) >> 4);
	}
	else	EMU->SetPRG_ROM32(0x8,(Latch.Addr.s0 & 0x60) >> 5);
	if (Latch.Addr.s0 & 0x08)
		EMU->Mirror_H();
	else	EMU->Mirror_V();
	if (Latch.Addr.s0 & 0x100)
		for (x = 0x8; x < 0x10; x++)
		{
			EMU->SetPRG_RAM4(x,x-8);
			memset(EMU->GetPRG_Ptr4(x),Mapper.Jumper,0x1000);
		}
}

static	LRESULT CALLBACK ConfigProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			CheckDlgButton(hDlg,IDC_BMC_T3H53_J0,(Mapper.Jumper & 0x01) ? BST_CHECKED : BST_UNCHECKED);
			CheckDlgButton(hDlg,IDC_BMC_T3H53_J1,(Mapper.Jumper & 0x02) ? BST_CHECKED : BST_UNCHECKED);
			return FALSE;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDOK:
				Mapper.ConfigCmd = 0x80;
				if (IsDlgButtonChecked(hDlg,IDC_BMC_T3H53_J0) == BST_CHECKED)
					Mapper.ConfigCmd |= 0x01;
				if (IsDlgButtonChecked(hDlg,IDC_BMC_T3H53_J1) == BST_CHECKED)
					Mapper.ConfigCmd |= 0x02;
			case IDCANCEL:
				DestroyWindow(hDlg);
				Mapper.ConfigWindow = NULL;
				return TRUE;		break;
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hDlg);
			Mapper.ConfigWindow = NULL;
			return TRUE;		break;
	}
	return FALSE;
}

static	unsigned char	_MAPINT	Config (CFG_TYPE mode, unsigned char data)
{
	switch (mode)
	{
	case CFG_WINDOW:
		if (data)
		{
			if (Mapper.ConfigWindow)
				break;
			Mapper.ConfigWindow = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_BMC_T3H53),hWnd,(DLGPROC)ConfigProc);
			SetWindowPos(Mapper.ConfigWindow,hWnd,0,0,0,0,SWP_SHOWWINDOW | SWP_NOSIZE);
		}
		else	return TRUE;
		break;
	case CFG_QUERY:
		return Mapper.ConfigCmd;
		break;
	case CFG_CMD:
		if (Mapper.ConfigCmd & 0x80)
		{
			Mapper.Jumper = Mapper.ConfigCmd & 0x03;
			Sync();
			MessageBox(hWnd,"Please perform a SOFT RESET for this to take effect!","UNIF.DLL",MB_OK);
		}
		Mapper.ConfigCmd = 0;
		break;
	}
	return 0;
}

static	void	_MAPINT	Shutdown (void)
{
	Latch_Destroy();
	if (Mapper.ConfigWindow)
		DestroyWindow(Mapper.ConfigWindow);
	Mapper.ConfigWindow = NULL;
}

static	void	_MAPINT	Reset (int IsHardReset)
{
	if (IsHardReset)
		Mapper.Jumper = 0;

	Mapper.ConfigWindow = NULL;
	Mapper.ConfigCmd = 0;

	Latch_Init(Sync,IsHardReset,FALSE);
}

CTMapperInfo	MapperInfo_BMC_T3H53 =
{
	"BMC-T3H53",
	"Pirate multicart mapper with dipswitches",
	COMPAT_FULL,
	Reset,
	Shutdown,
	NULL,
	NULL,
	Latch_SaveLoad_A,
	NULL,
	Config
};