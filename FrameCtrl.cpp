/*! @file
	@brief �g�̖ʓ|����
	���̃t�@�C���� FrameCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/08
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

#define FRAMEINSERTBOX_CLASS	TEXT("FRAMEINSBOX_CLASS")
#define FIB_WIDTH	400
#define FIB_HEIGHT	200

#define TRANCE_COLOUR	RGB(0x66,0x77,0x88)
//-------------------------------------------------------------------------------------------------

typedef struct tagFRAMEINSINFO
{
	UINT	dRfItem;	//	�V��E��̃p�[�c��
	UINT	dLftRfDot;	//	����{��̃h�b�g���E���̉E�I�t�Z�b�g�J�n�n�_
	UINT	dMidLines;	//	�Ԃ̍s��
	UINT	dFlrItem;	//	���E���̃p�[�c��

	LPTSTR	ptRoof;		//	�V��̕�����
	LPTSTR	ptMiddle;	//	�Ԃ̕�����
	LPTSTR	ptFloor;	//	���̕�����

} FRMINSINFO, *LPFRMINSINFO;
//-------------------------------------------------------------------------------------------------

#define TB_ITEMS	14
static  TBBUTTON	gstFIBTBInfo[] = {
	{ 10,	IDM_FRAME_INS_DECIDE,	TBSTATE_ENABLED,	TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE,		0,	0  },	//	
	{  0,	0,						TBSTATE_ENABLED,	TBSTYLE_SEP,							0,	0  },
	{  0,	IDM_INSFRAME_ALPHA   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  1,	IDM_INSFRAME_BRAVO   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  2,	IDM_INSFRAME_CHARLIE ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  3,	IDM_INSFRAME_DELTA   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  4,	IDM_INSFRAME_ECHO    ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  5,	IDM_INSFRAME_FOXTROT ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  6,	IDM_INSFRAME_GOLF    ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  7,	IDM_INSFRAME_HOTEL   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  8,	IDM_INSFRAME_INDIA   ,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  9,	IDM_INSFRAME_JULIETTE,	TBSTATE_ENABLED,	TBSTYLE_CHECKGROUP | TBSTYLE_AUTOSIZE,	0,	0  },	//	
	{  0,	0,						TBSTATE_ENABLED,	TBSTYLE_SEP,							0,	0  },
	{ 11,	IDM_FRMINSBOX_QCLOSE,	TBSTATE_ENABLED,	TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,		0,	0  }	//	
};	//	
//-------------------------------------------------------------------------------------------------


#ifdef MULTI_FILE
extern FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
#define gstFile	(*gitFileIt)	//!<	�C�e���[�^���\���̂ƌ��Ȃ�
#else
extern ONEFILE	gstFile;			//!<	�t�@�C���P�ʁE�����t�@�C���ɂ͂ǂ�����đΉ����H
#endif
		//!<	�t�@�C���P�ʁE�����t�@�C���ɂ͂ǂ�����đΉ����H
extern INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  HWND	ghViewWnd;		//!<	�r���[�E�C���h�E�n���h��

extern INT		gdHideXdot;		//!<	���̉B�ꕔ��
extern INT		gdViewTopLine;	//!<	�\�����̍ŏ㕔�s�ԍ�

extern HFONT	ghAaFont;		//!<	AA�p�t�H���g

static INT		gNowSel;		//!<	�I�𒆂̘g�O�C���f�b�N�X

static TCHAR	gatFrameIni[MAX_PATH];	//!<	20110707	�g�p��INI�����E���₵��


static  ATOM		gFrmInsAtom;	//!<	
static  HWND		ghFrInbxWnd;
static  HWND		ghFIBtlbrWnd;
static HBRUSH		ghBgBrush;

static HIMAGELIST	ghFrameImgLst;

static POINT		gstViewOrigin;	//!<	�r���[�̍���E�C���h�E�ʒu�E
static POINT		gstOffset;		//!<	�r���[���ォ��́A�{�b�N�X�̑��Έʒu
static POINT		gstFrmSz;		//!<	�E�C���h�E�G�b�W����`��̈�܂ł̃I�t�Z�b�g
static INT			gdToolBarHei;	//!<	�c�[���o�[����

static  UINT		ghSelect;		//!<	�I�������g�ԍ��O�`�X
static BOOLEAN		gbQuickClose;	//!<	�\��t�����璼������

static FRMINSINFO	gstFrmInsInfo;	//!<	�\�����̃��c�̏��

extern HFONT		ghAaFont;		//	AA�p�t�H���g


FRAMEINFO	gstFrameInfo[FRAME_MAX];	//!<	�z��ŕK�v���m�ۂł�����
//-------------------------------------------------------------------------------------------------

INT_PTR	CALLBACK FrameEditDlgProc( HWND, UINT, WPARAM, LPARAM );

INT_PTR	Frm_OnInitDialog( HWND, HWND, LPARAM );
INT_PTR	Frm_OnCommand( HWND, INT, HWND, UINT );
INT_PTR	Frm_OnDrawItem( HWND, CONST LPDRAWITEMSTRUCT );
INT_PTR	Frm_OnNotify( HWND, INT, LPNMHDR );

HRESULT	InitFrameItem( UINT, UINT, LPFRAMEINFO );

HRESULT	FrameNameLoad( UINT, LPTSTR, UINT_PTR );

HRESULT	FramePartsUpdate( HWND, HWND, LPFRAMEITEM );

HRESULT	FrameDataGet( UINT, LPFRAMEINFO );
HRESULT	FrameInfoDisp( HWND );


HRESULT	FrameInsBoxInfoCheck( VOID );
INT		FrameInsBoxSizeGet( LPRECT );
VOID	FrameInsBoxFrmDraw( HDC );
VOID	FrameDrawItem( HDC, INT, LPTSTR );
HRESULT	FrameInsBoxDoInsert( HWND );

LRESULT	CALLBACK FrameInsProc( HWND, UINT, WPARAM, LPARAM );
VOID	Fib_OnPaint( HWND );
VOID	Fib_OnCommand( HWND, INT, HWND, UINT );
VOID	Fib_OnDestroy( HWND );
VOID	Fib_OnMoving( HWND, LPRECT );			//!<	
VOID	Fib_OnKey( HWND, UINT, BOOL, INT, UINT );	//!<	
BOOL	Fib_OnWindowPosChanging( HWND, LPWINDOWPOS );		//!<	
VOID	Fib_OnWindowPosChanged( HWND, const LPWINDOWPOS );	//!<	
//-------------------------------------------------------------------------------------------------

/*!
	�g�ݒ��INI�t�@�C�����m�ہE�A�v���N���シ���Ă΂��
	@param[in]	ptCurrent	��f�B���N�g��
	@param[in]	hInstance	�C���X�^���X�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameInitialise( LPTSTR ptCurrent, HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;
	UINT		resnum, ici;
	HBITMAP		hImg, hMsq;

	if( !(ptCurrent) || !(hInstance) )
	{
		if( ghFrInbxWnd ){	DestroyWindow( ghFrInbxWnd );	}
		if( ghBgBrush ){	DeleteBrush( ghBgBrush );	}

		if( ghFrameImgLst ){	ImageList_Destroy( ghFrameImgLst  );	}

		return S_OK;
	}

	StringCchCopy( gatFrameIni, MAX_PATH, ptCurrent );
	PathAppend( gatFrameIni, FRAME_INI_FILE );

//�g�}����
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= FrameInsProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= FRAMEINSERTBOX_CLASS;
	wcex.hIconSm		= NULL;

	gFrmInsAtom = RegisterClassEx( &wcex );

	ghBgBrush = CreateSolidBrush( TRANCE_COLOUR );

	ghFrInbxWnd = NULL;

	ghSelect = 0;

	gbQuickClose = TRUE;

	ZeroMemory( &gstViewOrigin, sizeof(POINT) );
	ZeroMemory( &gstFrmSz, sizeof(POINT) );
	gdToolBarHei = 0;

	//	�A�C�R��
	ghFrameImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 4, 1 );
	resnum = IDBMP_FRMINS_ALPHA;
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_FRMINS_SEL ) );
	for( ici = 0; 10 > ici; ici++ )
	{
		hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );
	}
	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_FRMINS_INSERT ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_FRMINS_INSERT ) );
	ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMP_REFRESH ) );
	hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( IDBMQ_REFRESH ) );
	ImageList_Add( ghFrameImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
	DeleteBitmap( hImg );	DeleteBitmap( hMsq );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g���̂̓��e�Ń��j���[��ύX
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameNameModifyMenu( HWND hWnd )
{
	HMENU	hMenu, hSubMenu;//, hBoxMenu;
	UINT	i;
	TCHAR	atBuffer[MAX_PATH], atName[MAX_STRING];

	//	���j���[�\���ς�����炱�����ύX�E�ǂ��ɂ��Ȃ��̂�
	hMenu = GetMenu( hWnd );
	hSubMenu = GetSubMenu( hMenu, 2 );

	//	���C���g�A�O�Ԃ̃A��
	//FrameNameLoad( 0, atName, MAX_STRING );
	//StringCchPrintf( atBuffer, MAX_PATH, TEXT("�g�F(&A)%s"), atName );
	//ModifyMenu( hSubMenu, IDM_INSFRAME_ALPHA, MF_BYCOMMAND | MFT_STRING, IDM_INSFRAME_ALPHA, atBuffer );

	//	�ǉ��g�E�P����O����
	for( i = 0; FRAME_MAX > i; i++ )
	{
		FrameNameLoad( i, atName, MAX_STRING );
		StringCchPrintf( atBuffer, MAX_PATH, TEXT("�g�F(&%c)%s"), 'A'+i, atName );
		ModifyMenu( hSubMenu, IDM_INSFRAME_ALPHA+i, MF_BYCOMMAND | MFT_STRING, IDM_INSFRAME_ALPHA+i, atBuffer );
		//	���j���[���\�[�X�ԍ��̘A�Ԃɒ���
	}

	DrawMenuBar( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�|�b�p�b�v���j���[�p�ɖ��O����������΂�����
	@param[in]	hPopMenu	�Ώۂ̃|�b�p�b�v���j���[�n���h��
	@param[in]	bMode		��O�A�N�Z�����[�^�t����@�O�t���Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameNameModifyPopUp( HMENU hPopMenu, UINT bMode )
{
	UINT	i, j;
	TCHAR	atBuffer[MAX_PATH], atName[MAX_STRING];

	for( i = 0, j = 1; FRAME_MAX > i; i++, j++ )
	{
		FrameNameLoad( i, atName, MAX_STRING );

		if( bMode )
		{
			if( 10 <= j )	j = 0;
			StringCchPrintf( atBuffer, MAX_PATH, TEXT("�g�F(&%c)%s"), '0'+j, atName );
		}
		else
		{
			StringCchPrintf( atBuffer, MAX_PATH, TEXT("�g�F%s"), atName );
		}
		ModifyMenu( hPopMenu, IDM_INSFRAME_ALPHA+i, MF_BYCOMMAND | MFT_STRING, IDM_INSFRAME_ALPHA+i, atBuffer );
		//	���j���[���\�[�X�ԍ��̘A�Ԃɒ���
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�̖��O�����������Ă���
	@param[in]	dNumber	�g�ԍ��O�C���f�b�N�X
	@param[out]	ptNamed	���O�����o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchSize	�o�b�t�@�̕������E�o�C�g����Ȃ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameNameLoad( UINT dNumber, LPTSTR ptNamed, UINT_PTR cchSize )
{
	TCHAR	atAppName[MIN_STRING];

	if( !(ptNamed) || 0 >= cchSize )	return E_INVALIDARG;

	//	�����APP�������
	StringCchPrintf( atAppName, MIN_STRING, TEXT("Frame%u"), dNumber );

	GetPrivateProfileString( atAppName, TEXT("Name"), atAppName, ptNamed, cchSize, gatFrameIni );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

//	20110707	�ꏊ�ς���
/*!
	�g���̃Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dNumber	�g�ԍ��O�C���f�b�N�X
	@param[in,out]	pstInfo	�������o�b�t�@��������ۑ����e�������肷��\���̂ۂ��񂽁`
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitFrameItem( UINT dMode, UINT dNumber, LPFRAMEINFO pstInfo )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];

	//	�����APP�������
	StringCchPrintf( atAppName, MIN_STRING, TEXT("Frame%u"), dNumber );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("Name"), atAppName, pstInfo->atFrameName, MAX_STRING, gatFrameIni );

		GetPrivateProfileString( atAppName, TEXT("Daybreak"), TEXT("��"), pstInfo->stDaybreak.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Morning"), TEXT("��"), pstInfo->stMorning.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Noon"), TEXT("��"), pstInfo->stNoon.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Afternoon"), TEXT("��"), pstInfo->stAfternoon.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Nightfall"), TEXT("��"), pstInfo->stNightfall.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Twilight"), TEXT("��"), pstInfo->stTwilight.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Midnight"), TEXT("��"), pstInfo->stMidnight.atParts, PARTS_CCH, gatFrameIni );
		GetPrivateProfileString( atAppName, TEXT("Dawn"), TEXT("��"), pstInfo->stDawn.atParts, PARTS_CCH, gatFrameIni );

		GetPrivateProfileString( atAppName, TEXT("LEFTOFFSET"), TEXT("0"), atBuff, MIN_STRING, gatFrameIni );
		pstInfo->dLeftOffset  = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("RIGHTOFFSET"), TEXT("0"), atBuff, MIN_STRING, gatFrameIni );
		pstInfo->dRightOffset = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( atAppName, TEXT("Name"), pstInfo->atFrameName, gatFrameIni );

		WritePrivateProfileString( atAppName, TEXT("Daybreak"), pstInfo->stDaybreak.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Morning"), pstInfo->stMorning.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Noon"), pstInfo->stNoon.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Afternoon"), pstInfo->stAfternoon.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Nightfall"), pstInfo->stNightfall.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Twilight"), pstInfo->stTwilight.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Midnight"), pstInfo->stMidnight.atParts, gatFrameIni );
		WritePrivateProfileString( atAppName, TEXT("Dawn"), pstInfo->stDawn.atParts, gatFrameIni );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dLeftOffset );
		WritePrivateProfileString( atAppName, TEXT("LEFTOFFSET"), atBuff, gatFrameIni );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dRightOffset );
		WritePrivateProfileString( atAppName, TEXT("RIGHTOFFSET"), atBuff, gatFrameIni );
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�ݒ�̃_�C�����O���J��
	@param[in]	hInst	�A�v���̎���
	@param[in]	hWnd	�{�̂̃E�C���h�E�n���h���ł���悤�ɂ��邱��
	@param[in]	dRsv	���g�p
	@return �I���R�[�h
*/
INT_PTR FrameEditDialogue( HINSTANCE hInst, HWND hWnd, UINT dRsv )
{
	INT_PTR	iRslt;

	gNowSel = 0;

	iRslt = DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_FRAME_EDIT_DLG), hWnd, FrameEditDlgProc, 0 );
	//	�������ʂɂ���ẮA�����Ń��j���[�̓��e����
	if( IDYES == iRslt ){	FrameNameModifyMenu( hWnd );	}

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�ݒ�_�C�����O�v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK FrameEditDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:	return Frm_OnInitDialog( hDlg, (HWND)(wParam), lParam );
		case WM_COMMAND:	return Frm_OnCommand( hDlg, (INT)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam) );
		case WM_DRAWITEM:	return Frm_OnDrawItem( hDlg, ((CONST LPDRAWITEMSTRUCT)(lParam)) );
		case WM_NOTIFY:		return Frm_OnNotify( hDlg, (INT)(wParam), (LPNMHDR)(lParam) );

	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O�N�����̏�����
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	hWndFocus	�Ȃ񂾂���
	@param[in]	lParam		�_�C�����O�I�[�|������Ƃ��ɌĂяo�������n�����l
	@return		���ɈӖ��͂Ȃ�
*/
INT_PTR Frm_OnInitDialog( HWND hDlg, HWND hWndFocus, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	i;

	//	�R���{�b�N�X�ɖ��O����Ƃ�
	hWorkWnd = GetDlgItem( hDlg, IDCB_BOX_NAME_SEL );

	for( i = 0; FRAME_MAX > i; i++ )	//	�o�b�t�@�Ɋm��
	{
		FrameDataGet( i, &(gstFrameInfo[i]) );
		//	INI�t�@�C��������������āA�R���{�b�N�X�ɖ��O�����ꂿ�Ⴄ
		ComboBox_AddString( hWorkWnd, gstFrameInfo[i].atFrameName );
	}

	ComboBox_SetCurSel( hWorkWnd, gNowSel );

	//	�p�[�c���������
	FrameInfoDisp( hDlg );

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�_�C�����O��COMMAND���b�Z�[�W�̎󂯎��
	@param[in]	hDlg		�_�C�����O�[�n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�������������񂩂�����
*/
INT_PTR Frm_OnCommand( HWND hDlg, INT id, HWND hWndCtl, UINT codeNotify )
{
	static BOOLEAN	cbNameMod = FALSE;	//	�_�C�����O�I���p�̍P�v�I�Ȃ���
	static BOOLEAN	cbNameChg = FALSE;	//	APPLY�p
	UINT	i;
	HWND	hCmboxWnd;
//	TCHAR	atName[MAX_STRING];

	switch( id )
	{
		default:	break;

		case IDCANCEL:
		case IDB_CANCEL:
			EndDialog( hDlg, IDCANCEL );
			return (INT_PTR)TRUE;

		case IDB_APPLY:
		case IDB_OK:
			hCmboxWnd = GetDlgItem( hDlg, IDCB_BOX_NAME_SEL );
			for( i = 0; FRAME_MAX > i; i++ )
			{
				InitFrameItem( INIT_SAVE, i, &(gstFrameInfo[i]) );
				if( cbNameChg )
				{
					ComboBox_DeleteString( hCmboxWnd, 0 );//�擪������
					ComboBox_AddString( hCmboxWnd, gstFrameInfo[i].atFrameName );//�����ɕt������
				}
			}
			ComboBox_SetCurSel( hCmboxWnd, gNowSel );
			cbNameChg = FALSE;
			if( IDB_OK ==  id ){	EndDialog( hDlg, cbNameMod ? IDYES : IDOK );	}
			return (INT_PTR)TRUE;

		case IDE_BOXP_MORNING:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stMorning) );	}		return (INT_PTR)TRUE;
		case IDE_BOXP_NOON:			if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stNoon) );	}		return (INT_PTR)TRUE;
		case IDE_BOXP_AFTERNOON:	if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stAfternoon) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_DAYBREAK:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stDaybreak) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_NIGHTFALL:	if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stNightfall) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_TWILIGHT:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stTwilight) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_MIDNIGHT:		if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stMidnight) );	}	return (INT_PTR)TRUE;
		case IDE_BOXP_DAWN:			if( EN_UPDATE == codeNotify ){	FramePartsUpdate( hDlg , hWndCtl, &(gstFrameInfo[gNowSel].stDawn) );	}		return (INT_PTR)TRUE;

		case IDS_FRAME_IMAGE:
			if( STN_DBLCLK == codeNotify )	//	�_�{�[�N���b�N���ꂽ
			{
				TRACE( TEXT("���ځ[�������") );
				InvalidateRect( hWndCtl, NULL, TRUE );
			}
			return (INT_PTR)TRUE;

		case IDB_BOXP_NAME_APPLY:	//	���̂�ύX����
			Edit_GetText( GetDlgItem(hDlg,IDE_BOXP_NAME_EDIT), gstFrameInfo[gNowSel].atFrameName, MAX_STRING );
			cbNameMod = TRUE;	cbNameChg = TRUE;
			return (INT_PTR)TRUE;

		case IDCB_BOX_NAME_SEL:
			if( CBN_SELCHANGE == codeNotify )	//	�I�����ύX���ꂽ
			{
				gNowSel = ComboBox_GetCurSel( hWndCtl );
				FrameInfoDisp( hDlg );
				InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );
			}
			return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�p�[�c���X�V���ꂽ��
	@param[in]	hDlg	�_�C�����Q�n���h�D
	@param[in]	hWndCtl	�Ώۂ̃p�[�cEDITBOX
	@param[in]	pstItem	�Ώۃp�[�c�̃f�[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FramePartsUpdate( HWND hDlg, HWND hWndCtl, LPFRAMEITEM pstItem )
{
	TCHAR	atBuffer[MIN_STRING];

	if( Edit_GetTextLength( hWndCtl ) )
	{
		Edit_GetText( hWndCtl, atBuffer, MIN_STRING );
		atBuffer[PARTS_CCH-1] = 0;
		StringCchCopy( pstItem->atParts, PARTS_CCH, atBuffer );
	}
	else	//	�������Ȃ�������A�S�p�󔒂ɂ����Ⴄ
	{
		StringCchCopy( pstItem->atParts, PARTS_CCH, TEXT("�@") );
	}

	//	�h�b�g���m�F����
	pstItem->dDot = ViewStringWidthGet( pstItem->atParts );

	//	���łɍĕ`��
	InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�[�i�[�h���[�̏����E�X�^�e�B�b�N�̃A��
	@param[in]	hDlg		�_�C�����Q�n���h�D
	@param[in]	pstDrawItem	�h���[���ւ̃|�C���^�[
	@return		�������������񂩂�����
*/
INT_PTR Frm_OnDrawItem( HWND hDlg, CONST LPDRAWITEMSTRUCT pstDrawItem )
{
	LPFRAMEINFO	pstInfo;

	HFONT	hFtOld;
	TCHAR	atStr[BIG_STRING];	//	����邩�H

	UINT	cchSize;
	INT	xMaxDot, i, xNoonLen, mzNoonCnt, xNightPos, xMidLen, mzMidCnt;
	INT	yPos = (LINE_HEIGHT/2);

	if( IDS_FRAME_IMAGE != pstDrawItem->CtlID ){	return (INT_PTR)FALSE;	}

	pstInfo = &(gstFrameInfo[gNowSel]);

	hFtOld = SelectFont( pstDrawItem->hDC, ghAaFont );

	FillRect( pstDrawItem->hDC, &(pstDrawItem->rcItem), GetSysColorBrush( COLOR_WINDOW ) );
	SetBkMode( pstDrawItem->hDC, TRANSPARENT );

	//	�g�`��̈���m��
	xMaxDot = pstDrawItem->rcItem.right - (SPACE_ZEN * 2);

	//	����̕�����
	xNoonLen  = xMaxDot - ( pstInfo->stMorning.dDot + pstInfo->stAfternoon.dDot );
	mzNoonCnt = xNoonLen  / pstInfo->stNoon.dDot;

	//	�����̕�����
	xMidLen  = xMaxDot - ( pstInfo->stTwilight.dDot + pstInfo->stDawn.dDot );
	mzMidCnt = xMidLen  / pstInfo->stMidnight.dDot;

	//	�V��̕`��
	ZeroMemory( atStr, sizeof(atStr) );
	StringCchCopy( atStr, BIG_STRING, pstInfo->stMorning.atParts );
	for( i = 0; mzNoonCnt > i; i++ ){	StringCchCat( atStr, BIG_STRING, pstInfo->stNoon.atParts );	}
	//	�����܂ł̃h�b�g���E�E��̕`��J�n�ʒu���E�̏c�̊J�n�ʒu
	xNightPos  = ViewStringWidthGet( atStr );
	xNightPos += pstInfo->dRightOffset;	//	�I�t�Z�b�g�����炵�Ă���
	//	�E�㕔���������Ă���
	StringCchCat( atStr, BIG_STRING, pstInfo->stAfternoon.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	ExtTextOut( pstDrawItem->hDC, SPACE_ZEN, yPos, 0, NULL, atStr, cchSize, NULL );

	yPos += LINE_HEIGHT;
	//	�����̕`��
	ZeroMemory( atStr, sizeof(atStr) );
	StringCchCopy( atStr, BIG_STRING, pstInfo->stDaybreak.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	ExtTextOut( pstDrawItem->hDC, SPACE_ZEN, yPos, 0, NULL, atStr, cchSize, NULL );

	//	�E���̕`��
	StringCchCopy( atStr, BIG_STRING, pstInfo->stNightfall.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	ExtTextOut( pstDrawItem->hDC, SPACE_ZEN + xNightPos, yPos, 0, NULL, atStr, cchSize, NULL );

	yPos += LINE_HEIGHT;
	//	�����̕`��
	ZeroMemory( atStr, sizeof(atStr) );
	StringCchCopy( atStr, BIG_STRING, pstInfo->stDaybreak.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	ExtTextOut( pstDrawItem->hDC, SPACE_ZEN, yPos, 0, NULL, atStr, cchSize, NULL );

	//	�E���̕`��
	StringCchCopy( atStr, BIG_STRING, pstInfo->stNightfall.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	ExtTextOut( pstDrawItem->hDC, SPACE_ZEN + xNightPos, yPos, 0, NULL, atStr, cchSize, NULL );


	yPos += LINE_HEIGHT;
	ZeroMemory( atStr, sizeof(atStr) );
	StringCchCopy( atStr, BIG_STRING, pstInfo->stDawn.atParts );
	for( i = 0; mzMidCnt > i; i++ ){	StringCchCat( atStr, BIG_STRING, pstInfo->stMidnight.atParts );	}
	StringCchCat( atStr, BIG_STRING, pstInfo->stTwilight.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	ExtTextOut( pstDrawItem->hDC, SPACE_ZEN, yPos, 0, NULL, atStr, cchSize, NULL );

	return (INT_PTR)TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hDlg		�_�C�����Q�n���h�D
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
INT_PTR Frm_OnNotify( HWND hDlg, INT idFrom, LPNMHDR pstNmhdr )
{
	INT	nmCode;
	TCHAR	atBuff[MIN_STRING];
	LPNMUPDOWN	pstUpDown;

	nmCode = pstNmhdr->code;

	//	����������f���^�����A�E�ŕ�
#if 0
	if( IDSP_LEFT_OFFSET == idFrom )
	{
		pstUpDown = (LPNMUPDOWN)pstNmhdr;
		if( UDN_DELTAPOS == nmCode )
		{
			TRACE( TEXT("%d %d"), pstUpDown->iPos, pstUpDown->iDelta );
			if( 0 < pstUpDown->iDelta ){		gstFrameInfo[gNowSel].dLeftOffset +=  1;	}
			else if( 0 >pstUpDown->iDelta ){	gstFrameInfo[gNowSel].dLeftOffset -=  1;	}
		}
		return (INT_PTR)TRUE;
	}
#endif
	if( IDSP_RIGHT_OFFSET == idFrom )
	{
		pstUpDown = (LPNMUPDOWN)pstNmhdr;
		if( UDN_DELTAPOS == nmCode )
		{
			TRACE( TEXT("%d %d"), pstUpDown->iPos, pstUpDown->iDelta );
			if( 0 < pstUpDown->iDelta ){		gstFrameInfo[gNowSel].dRightOffset -= 1;	}
			else if( 0 > pstUpDown->iDelta ){	gstFrameInfo[gNowSel].dRightOffset += 1;	}

			StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), gstFrameInfo[gNowSel].dRightOffset );
			Edit_SetText( GetDlgItem(hDlg,IDE_RIGHT_OFFSET), atBuff );

			InvalidateRect( GetDlgItem(hDlg,IDS_FRAME_IMAGE), NULL, TRUE );
		}

		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	����̘g�f�[�^��INI�t�@�C�����烍�[�h���ă������Ɋm�ہE�h�b�g�����v�Z
	@param[in]	dNumber		�g�ԍ�
	@param[in]	pstFrame	�g�f�[�^�����\���̂ւ̃|�C���^�`
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameDataGet( UINT dNumber, LPFRAMEINFO pstFrame )
{
	InitFrameItem( INIT_LOAD, dNumber, pstFrame );

	pstFrame->stDaybreak.dDot  = ViewStringWidthGet( pstFrame->stDaybreak.atParts );
	pstFrame->stMorning.dDot   = ViewStringWidthGet( pstFrame->stMorning.atParts );
	pstFrame->stNoon.dDot      = ViewStringWidthGet( pstFrame->stNoon.atParts );
	pstFrame->stAfternoon.dDot = ViewStringWidthGet( pstFrame->stAfternoon.atParts );
	pstFrame->stNightfall.dDot = ViewStringWidthGet( pstFrame->stNightfall.atParts );
	pstFrame->stTwilight.dDot  = ViewStringWidthGet( pstFrame->stTwilight.atParts );
	pstFrame->stMidnight.dDot  = ViewStringWidthGet( pstFrame->stMidnight.atParts );
	pstFrame->stDawn.dDot      = ViewStringWidthGet( pstFrame->stDawn.atParts );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�G�f�B�b�g�{�b�N�X�ɐݒ���e������
	@param[in]	hDlg	�_�C�����Q�n���h�D
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameInfoDisp( HWND hDlg )
{
	TCHAR	atBuff[MIN_STRING];

	//	���O�\��
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_NAME_EDIT), gstFrameInfo[gNowSel].atFrameName );

	//	�p�[�c���������
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_MORNING),   gstFrameInfo[gNowSel].stMorning.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_NOON),      gstFrameInfo[gNowSel].stNoon.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_AFTERNOON), gstFrameInfo[gNowSel].stAfternoon.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_DAYBREAK),  gstFrameInfo[gNowSel].stDaybreak.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_NIGHTFALL), gstFrameInfo[gNowSel].stNightfall.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_TWILIGHT),  gstFrameInfo[gNowSel].stTwilight.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_MIDNIGHT),  gstFrameInfo[gNowSel].stMidnight.atParts );
	Edit_SetText( GetDlgItem(hDlg,IDE_BOXP_DAWN),      gstFrameInfo[gNowSel].stDawn.atParts );
#if 0
	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"),   gstFrameInfo[gNowSel].dLeftOffset );
	Edit_SetText( GetDlgItem(hDlg,IDE_LEFT_OFFSET),    atBuff );
#endif
	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"),   gstFrameInfo[gNowSel].dRightOffset );
	Edit_SetText( GetDlgItem(hDlg,IDE_RIGHT_OFFSET),   atBuff );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g������
	@param[in]	dMode	���ꂽ���g�̔ԍ��O�C���f�b�N�X
	@param[in]	dStyle	��`���ǂ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFrameInsert( INT dMode, INT dStyle )
{
	UINT_PTR	iLines;
	INT_PTR		iLns;
	INT			iTop, iBtm, iInX, iEndot, iPadding, i, baseDot;
	INT			xNoonLen, mzNoonCnt, xMidLen, mzMidCnt, xNightPos;
	UINT		cchSize;
	TCHAR		atStr[BIG_STRING];	//	����邩�H
	LPTSTR		ptPadding;

	FRAMEINFO	stInfo;

	FrameDataGet( dMode, &stInfo );


	//	�y�[�W�S�̂̍s��
	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );

	//	�J�n�n�_����J�n	//	D_SQUARE
	iTop = gstFile.vcCont.at( gixFocusPage ).dSelLineTop;
	iBtm = gstFile.vcCont.at( gixFocusPage ).dSelLineBottom;
	if( 0 >  iTop ){	iTop = 0;	}
	if( 0 >  iBtm ){	iBtm = iLines - 1;	}

	iInX = DocLineParamGet( iBtm, NULL, NULL );
	if( 0 == iInX ){	 iBtm--;	}

	//	��`�I�𖳂��Ƃ݂Ȃ��E�Ă�������`�ɑΉ����Ē�����₱�����x�n�I

	ViewSelPageAll( -1 );	//	�͈͂Ƃ����̂ŉ������Ă���

	//	�I��͈͓��ł����Ƃ������h�b�g�����m�F
	baseDot = DocPageMaxDotGet( iTop, iBtm );
	//	�E�]�T�𑫂��Ă����B����͘g�̍��E���܂܂Ȃ��ʂł���
	baseDot += 16;	//	�l�͓K��

	iBtm++;	//	�V��̉��s������̂ŁA���̍s�̖��[�ł܂����s����Ώ��̕��m��

	DocCrLfAdd( 0 , iTop, TRUE );	//	�܂��V������邽�߂ɉ��s

	//�V��ǉ�
	//	����̕�����
	xNoonLen  = baseDot  + stInfo.stNoon.dDot;	//	��g���]�T����������
	mzNoonCnt = xNoonLen / stInfo.stNoon.dDot;

	//	�����̕�����
	xMidLen  = xNoonLen;
	if( stInfo.stNoon.dDot == stInfo.stMidnight.dDot )	//	�㉺�����v�Z�ł���Ȃ�
	{
		mzMidCnt = mzNoonCnt;
	}
	else
	{
		xMidLen += (stInfo.stMidnight.dDot / 2);
		mzMidCnt = xMidLen  / stInfo.stMidnight.dDot;
	}

	//	�V��̕`��
	ZeroMemory( atStr, sizeof(atStr) );
	StringCchCopy( atStr, BIG_STRING, stInfo.stMorning.atParts );
	for( i = 0; mzNoonCnt > i; i++ ){	StringCchCat( atStr, BIG_STRING, stInfo.stNoon.atParts );	}
	//	�����܂ł̃h�b�g���E�E��̕`��J�n�ʒu���E�̏c�̊J�n�ʒu
	xNightPos  = ViewStringWidthGet( atStr );
	xNightPos += stInfo.dRightOffset;	//	�I�t�Z�b�g�����炵�Ă���
	//	�E�㕔���������Ă���
	StringCchCat( atStr, BIG_STRING, stInfo.stAfternoon.atParts );
	//	�V�䏑��
	iInX = 0;
	DocInsertString( &iInX, &iTop, NULL, atStr, 0, FALSE );

	//���E���ǉ�
	for( i = iTop+1; iBtm >= i; i++ )
	{
		iInX = 0;	//	���ǉ�
		DocInsertString( &iInX, &i, NULL, stInfo.stDaybreak.atParts, 0, FALSE );
		iEndot = DocLineParamGet( i, NULL, NULL );
		iPadding  = xNightPos - iEndot;	//	�E�ǉ�
		ptPadding = DocPaddingSpaceWithPeriod( iPadding, NULL, NULL, NULL, TRUE );
		iInX = iEndot;
		DocInsertString( &iInX, &i, NULL, ptPadding, 0, FALSE );
		FREE(ptPadding);
		DocInsertString( &iInX, &i, NULL, stInfo.stNightfall.atParts, 0, FALSE );
	}

	//���}�����s
	iInX = DocLineParamGet( iBtm, NULL, NULL );
	DocCrLfAdd( iInX , iBtm, FALSE );
	iBtm++;

	//���ǉ�
	ZeroMemory( atStr, sizeof(atStr) );
	StringCchCopy( atStr, BIG_STRING, stInfo.stDawn.atParts );
	for( i = 0; mzMidCnt > i; i++ ){	StringCchCat( atStr, BIG_STRING, stInfo.stMidnight.atParts );	}
	StringCchCat( atStr, BIG_STRING, stInfo.stTwilight.atParts );
	StringCchLength( atStr, BIG_STRING, &cchSize );
	iInX = 0;
	DocInsertString( &iInX, &iBtm, NULL, atStr, 0, FALSE );

	ViewRedrawSetLine( iTop );
	for( i = iTop+1; iBtm > i; i++ )
	{
		DocBadSpaceCheck( i );	//	�o�b�h�󔒃`�F�L
		ViewRedrawSetLine( i );
	}
	//	���s���Ă邩��A����ȍ~�S���ĕ`��K�v
	iLns = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );	//	���ݍs���ĔF��
	for( ; iLns >  i; i++ ){	ViewRedrawSetLine(  i );	}
	ViewRedrawSetLine( ++i );	//	�O�̂���


	//	�ŏI�I�ȃL�����b�g�̈ʒu�����Z�b�g
	ViewPosResetCaret( iInX , iBtm );

	DocPageInfoRenew( -1, 1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------




/*!
	�}���p�E�C���h�E���
	@param[in]	hPrWnd	���C���̃E�C���h�E�n���h��
*/
HWND FrameInsBoxCreate( HINSTANCE hInst, HWND hPrWnd )
{
	INT			x, y;
	UINT		d;
	TCHAR		atBuffer[MAX_STRING];
	RECT		rect, vwRect;
//	TBADDBITMAP	stToolBmp;



	if( ghFrInbxWnd )
	{
		SetForegroundWindow( ghFrInbxWnd );
		return ghFrInbxWnd;
	}

	ZeroMemory( &gstFrmInsInfo, sizeof(FRMINSINFO) );

	//	�{�̃E�C���h�E
	ghFrInbxWnd = CreateWindowEx( WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		FRAMEINSERTBOX_CLASS, TEXT("�g�}���{�b�N�X"),
		WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU,
		0, 0, FIB_WIDTH, FIB_HEIGHT, NULL, NULL, hInst, NULL );
	SetLayeredWindowAttributes( ghFrInbxWnd, TRANCE_COLOUR, 0xFF, LWA_COLORKEY );
										//	TRANCE_COLOUR

	//	�c�[���o�[
	ghFIBtlbrWnd = CreateWindowEx( WS_EX_CLIENTEDGE, TOOLBARCLASSNAME, TEXT("fibtoolbar"),
		WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS,
		0, 0, 0, 0, ghFrInbxWnd, (HMENU)IDTB_FRMINSBOX_TOOLBAR, hInst, NULL );

	if( 0 == gdToolBarHei )	//	���l���擾�Ȃ�
	{
		GetWindowRect( ghFIBtlbrWnd, &rect );
		gdToolBarHei = rect.bottom - rect.top;

		gstFrmSz.x = 0;
		gstFrmSz.y = gdToolBarHei;
		ClientToScreen( ghFrInbxWnd, &gstFrmSz );
	}

	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghFIBtlbrWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//	�A�C�R���E�Ƃ肠�����Վ���
	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_STD_SMALL_COLOR;
	//SendMessage( ghFIBtlbrWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );

	SendMessage( ghFIBtlbrWnd, TB_SETIMAGELIST, 0, (LPARAM)ghFrameImgLst );

	SendMessage( ghFIBtlbrWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(16,16) );

	SendMessage( ghFIBtlbrWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );

	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�}������") );
	gstFIBTBInfo[0].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	for( d = 0; 10 > d; d++ )
	{
		FrameNameLoad( d, atBuffer, MAX_STRING );
		gstFIBTBInfo[d+2].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );
	}
	StringCchCopy( atBuffer, MAX_STRING, TEXT("�}�����������") );
	gstFIBTBInfo[13].iString = SendMessage( ghFIBtlbrWnd, TB_ADDSTRING, 0, (LPARAM)atBuffer );

	SendMessage( ghFIBtlbrWnd , TB_ADDBUTTONS, (WPARAM)TB_ITEMS, (LPARAM)&gstFIBTBInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghFIBtlbrWnd , TB_AUTOSIZE, 0, 0 );	//	�{�^���̃T�C�Y�ɍ��킹�ăc�[���o�[�����T�C�Y
	InvalidateRect( ghFIBtlbrWnd , NULL, TRUE );		//	�N���C�A���g�S�̂��ĕ`�悷�閽��

	//	������ԂƂ��ăA���t�@���`�F�L��Ԃ�
	SendMessage( ghFIBtlbrWnd, TB_CHECKBUTTON, IDM_INSFRAME_ALPHA, TRUE );
	ghSelect = 0;

	//	�������邩�ǂ���
	SendMessage( ghFIBtlbrWnd, TB_CHECKBUTTON, IDM_FRMINSBOX_QCLOSE, gbQuickClose );
	

	FrameInsBoxInfoCheck(  );	//	������ԃf�[�^�쐬


	//	�E�C���h�E�ʒu���m�肳����
	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;
	x = (vwRect.left + LINENUM_WID) - gstFrmSz.x;
	y = (vwRect.top  + RULER_AREA)  - gstFrmSz.y;
	TRACE( TEXT("Frame %d x %d"), x, y );

#ifdef _DEBUG
	SetWindowPos( ghFrInbxWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#else
	SetWindowPos( ghFrInbxWnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
#endif
	gstOffset.x = x - vwRect.left;
	gstOffset.y = y - vwRect.top;


	return ghFrInbxWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t���[���T�C�Y���m��
	@param[out]	pstRect	�t���[���T�C�Y������o�b�t�@
	@return		�����I�t�Z�b�g�i�c�[���o�[�̍����j
*/
INT FrameInsBoxSizeGet( LPRECT pstRect )
{
	RECT	rect;

	GetClientRect( ghFrInbxWnd, &rect );

	//	�N���C�����g��������A�c�[���o�[�������k��
	rect.bottom -= gdToolBarHei;

	*pstRect = rect;

	return gdToolBarHei;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g���ɍ��킹�ăf�[�^���
*/
HRESULT FrameInsBoxInfoCheck( VOID )
{
/*
��p������
����{�^�񒆁A�����܂ł̒������^�񒆂̉E�I�t�Z�b�g�J�n�n�_
�E��E������͂ݏo���Ȃ��悤�ɁA�����Ȃ��ő�����߂�
�����{�^�񒆁{�E���A�������ĕ��ׂ邾��
*/
	LPTSTR		ptBuffer;
	INT			dWid, dLftTopWid, dHei, iMidDot;
	UINT_PTR	cchLeft, cchCentre, cchRight, dCnt, d;
	RECT		stFrmRct;

	FRAMEINFO	stInfo;


	FrameInsBoxSizeGet( &stFrmRct );

	FREE( gstFrmInsInfo.ptRoof );
	FREE( gstFrmInsInfo.ptMiddle );
	FREE( gstFrmInsInfo.ptFloor );

	//	�g�p�[�c���m��
	FrameDataGet( ghSelect, &stInfo );

	//	��̕�
	dWid  = stFrmRct.right - stInfo.stMorning.dDot;	//	���������
	dWid -= stInfo.stAfternoon.dDot;	//	�E��������ƁA���MAX�h�b�g��
	//	��̃p�[�c���E�����_�؂�̂Ăł���
	gstFrmInsInfo.dRfItem = dWid / stInfo.stNoon.dDot;

	//	�e�p�[�c�̕������m�F
	StringCchLength( stInfo.stMorning.atParts, PARTS_CCH, &cchLeft );
	StringCchLength( stInfo.stNoon.atParts, PARTS_CCH, &cchCentre );
	StringCchLength( stInfo.stAfternoon.atParts, PARTS_CCH, &cchRight );
	//	������쐬
	dCnt = cchLeft + (cchCentre * gstFrmInsInfo.dRfItem) + cchRight + 2;
	gstFrmInsInfo.ptRoof = (LPTSTR)malloc( dCnt * sizeof(TCHAR) );
	ZeroMemory( gstFrmInsInfo.ptRoof, dCnt * sizeof(TCHAR) );
	StringCchCopy( gstFrmInsInfo.ptRoof, dCnt, stInfo.stMorning.atParts );
	for( d = 0; gstFrmInsInfo.dRfItem > d; d++ ){	StringCchCat( gstFrmInsInfo.ptRoof, dCnt, stInfo.stNoon.atParts );	}
	StringCchCat( gstFrmInsInfo.ptRoof, dCnt, stInfo.stAfternoon.atParts );


	//	����{��̃h�b�g���E�^�񒆂̕�
	dLftTopWid  = gstFrmInsInfo.dRfItem * stInfo.stNoon.dDot;
	gstFrmInsInfo.dLftRfDot = dLftTopWid + stInfo.stMorning.dDot;
	//	�^�񒆂̋󔒂̕�
	iMidDot  = gstFrmInsInfo.dLftRfDot - stInfo.stDaybreak.dDot;	//	���Ђ���
	iMidDot += stInfo.dRightOffset;	//	�E�I�t�Z�b�g����
	ptBuffer = DocPaddingSpaceMake( iMidDot );	//	�󔒍쐬
	//	�e�p�[�c�̕������m�F
	StringCchLength( stInfo.stDaybreak.atParts, PARTS_CCH, &cchLeft );
	StringCchLength( ptBuffer, STRSAFE_MAX_CCH, &cchCentre );	//	�������
	StringCchLength( stInfo.stNightfall.atParts, PARTS_CCH, &cchRight );
	//	������쐬
	dCnt = cchLeft + cchCentre + cchRight + 2;
	gstFrmInsInfo.ptMiddle = (LPTSTR)malloc( dCnt * sizeof(TCHAR) );
	ZeroMemory( gstFrmInsInfo.ptMiddle, dCnt * sizeof(TCHAR) );
	StringCchCopy( gstFrmInsInfo.ptMiddle, dCnt, stInfo.stDaybreak.atParts );
	StringCchCat( gstFrmInsInfo.ptMiddle, dCnt, ptBuffer );
	StringCchCat( gstFrmInsInfo.ptMiddle, dCnt, stInfo.stNightfall.atParts );
	FREE(ptBuffer);


	//	���̕�
	dWid  = stFrmRct.right - stInfo.stDawn.dDot;	//	����������
	dWid -= stInfo.stTwilight.dDot;	//	�E���������ƁA����MAX�h�b�g��
	//	���̃p�[�c���E�����_�؂�̂Ăł���
	gstFrmInsInfo.dFlrItem = dWid / stInfo.stMidnight.dDot;

	//	�e�p�[�c�̕������m�F
	StringCchLength( stInfo.stDawn.atParts, PARTS_CCH, &cchLeft );
	StringCchLength( stInfo.stMidnight.atParts, PARTS_CCH, &cchCentre );
	StringCchLength( stInfo.stTwilight.atParts, PARTS_CCH, &cchRight );
	//	������쐬
	dCnt = cchLeft + (cchCentre * gstFrmInsInfo.dFlrItem) + cchRight + 2;
	gstFrmInsInfo.ptFloor = (LPTSTR)malloc( dCnt * sizeof(TCHAR) );
	ZeroMemory( gstFrmInsInfo.ptFloor, dCnt * sizeof(TCHAR) );
	StringCchCopy( gstFrmInsInfo.ptFloor, dCnt, stInfo.stDawn.atParts );
	for( d = 0; gstFrmInsInfo.dFlrItem > d; d++ ){	StringCchCat( gstFrmInsInfo.ptFloor, dCnt, stInfo.stMidnight.atParts );	}
	StringCchCat( gstFrmInsInfo.ptFloor, dCnt, stInfo.stTwilight.atParts );


	//	�s��
	dHei =  stFrmRct.bottom - (LINE_HEIGHT * 2);	//	�c�h�b�g��
	if( 0 >= dHei ){	gstFrmInsInfo.dMidLines = 0;	}
	else{	gstFrmInsInfo.dMidLines = dHei / LINE_HEIGHT;	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�����s
	@param[in]	hWnd	�E�C���h�E�n���h��
*/
HRESULT FrameInsBoxDoInsert( HWND hWnd )
{
	LPTSTR		ptBuffer;
	UINT_PTR	cchRoof = 0, cchMidd = 0, cchFlor = 0, cchTotal;
	UINT		d;
	INT			iX, iY;
	HWND		hLyrWnd;
	RECT		rect;

	//	�}���p��������쐬
	StringCchLength( gstFrmInsInfo.ptRoof,   STRSAFE_MAX_CCH, &cchRoof );
	StringCchLength( gstFrmInsInfo.ptMiddle, STRSAFE_MAX_CCH, &cchMidd );
	StringCchLength( gstFrmInsInfo.ptFloor,  STRSAFE_MAX_CCH, &cchFlor );

	//	�e�s�ɉ��s�A�ŏI�s�ɂ͕s�v�A�k���^�[�~�l�[�^�p�̗]�T
	cchTotal = (cchRoof + 2) + (cchMidd + 2) * gstFrmInsInfo.dMidLines + cchFlor + 2;

	ptBuffer = (LPTSTR)malloc( cchTotal * sizeof(TCHAR) );
	ZeroMemory( ptBuffer, cchTotal * sizeof(TCHAR) );

	StringCchPrintf( ptBuffer, cchTotal, TEXT("%s\r\n"), gstFrmInsInfo.ptRoof );
	for( d = 0; gstFrmInsInfo.dMidLines > d; d++ )
	{
		StringCchCat( ptBuffer, cchTotal, gstFrmInsInfo.ptMiddle );
		StringCchCat( ptBuffer, cchTotal, TEXT("\r\n") );
	}
	StringCchCat( ptBuffer, cchTotal, gstFrmInsInfo.ptFloor );

	//	�}�������ɂ́A���C���{�b�N�X���\�������Ŏg��
	hLyrWnd = LayerBoxVisibalise( GetModuleHandle(NULL), ptBuffer, 0x10 );
	FREE(ptBuffer);
	//	���C���̈ʒu��ύX
	GetWindowRect( hWnd, &rect );
	LayerBoxPositionChange( hLyrWnd, (rect.left + gstFrmSz.x), (rect.top + gstFrmSz.y) );
	//	�󔒂�S�����ߎw��ɂ���
	LayerTransparentToggle( hLyrWnd, 1 );
	//	�㏑������
	LayerContentsImportable( hLyrWnd, IDM_LYB_OVERRIDE, &iX, &iY, D_INVISI );
	ViewPosResetCaret( iX, iY );	
	//	�I����������
	DestroyWindow( hLyrWnd );

	if( gbQuickClose )	DestroyWindow( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK FrameInsProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_PAINT,		Fib_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_KEYDOWN,	Fib_OnKey );
		HANDLE_MSG( hWnd, WM_COMMAND,	Fib_OnCommand );	
		HANDLE_MSG( hWnd, WM_DESTROY,	Fib_OnDestroy );	//	�I�����̏���
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGING, Fib_OnWindowPosChanging );
		HANDLE_MSG( hWnd, WM_WINDOWPOSCHANGED,  Fib_OnWindowPosChanged );

		case WM_MOVING:	Fib_OnMoving( hWnd, (LPRECT)lParam );	return 0;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Fib_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{

	switch( id )
	{
		case IDM_FRAME_INS_DECIDE:	FrameInsBoxDoInsert( hWnd );	return;

		case IDM_INSFRAME_ALPHA:	ghSelect = 0;	break;
		case IDM_INSFRAME_BRAVO:	ghSelect = 1;	break;
		case IDM_INSFRAME_CHARLIE:	ghSelect = 2;	break;
		case IDM_INSFRAME_DELTA:	ghSelect = 3;	break;
		case IDM_INSFRAME_ECHO:		ghSelect = 4;	break;
		case IDM_INSFRAME_FOXTROT:	ghSelect = 5;	break;
		case IDM_INSFRAME_GOLF:		ghSelect = 6;	break;
		case IDM_INSFRAME_HOTEL:	ghSelect = 7;	break;
		case IDM_INSFRAME_INDIA:	ghSelect = 8;	break;
		case IDM_INSFRAME_JULIETTE:	ghSelect = 9;	break;

		case IDM_FRMINSBOX_QCLOSE:	gbQuickClose = SendMessage( ghFIBtlbrWnd, TB_ISBUTTONCHECKED, IDM_FRMINSBOX_QCLOSE, 0 );	break;

		default:	return;
	}

	FrameInsBoxInfoCheck(  );
	InvalidateRect( hWnd, NULL, TRUE );

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�L�[�_�E���������E�L�[�{�[�h�ňړ��p
	@param[in]	hWnd	�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	vk		�����ꂽ�L�[�����z�L�[�R�[�h�ŗ���
	@param[in]	fDown	��O�_�E���@�O�A�b�v
	@param[in]	cRepeat	�A���I�T���񐔁E���ĂȂ��H
	@param[in]	flags	�L�[�t���O���낢��
	@return		����
*/
VOID Fib_OnKey( HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags )
{
	RECT	rect;

	GetWindowRect( hWnd, &rect );

	if( fDown )
	{
		switch( vk )
		{
			case VK_RIGHT:	TRACE( TEXT("�E") );	rect.left++;	break;
			case VK_LEFT:	TRACE( TEXT("��") );	rect.left--;	break;
			case VK_DOWN:	TRACE( TEXT("��") );	rect.top += LINE_HEIGHT;	break;
			case  VK_UP:	TRACE( TEXT("��") );	rect.top -= LINE_HEIGHT;	break;
			default:	return;
		}
	}

	SetWindowPos( hWnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	Fib_OnMoving( hWnd, &rect );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
*/
VOID Fib_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	RECT	rect;

	GetClientRect( hWnd, &rect );

	hdc = BeginPaint( hWnd, &ps );

	FillRect( hdc, &rect, ghBgBrush );

	//	������ĕ`��
	FrameInsBoxFrmDraw( hdc );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`��
	@param[in]	hDC	�`�悷��f�o�C�X�R���e�L�X�g
*/
VOID FrameInsBoxFrmDraw( HDC hDC )
{
	//UINT_PTR	cchSize;
	UINT	i;
	HFONT	hOldFnt;
	INT		topOst, iYpos;
	RECT	stFrmRct;

	FRAMEINFO	stInfo;


//	SetBkMode( hDC, OPAQUE );
	SetBkColor( hDC, ViewBackColourGet( NULL ) );	//	

	FrameDataGet( ghSelect, &stInfo );	//	�g�p�[�c���m��

	hOldFnt = SelectFont( hDC, ghAaFont );	//	�t�H���g��������

	topOst = FrameInsBoxSizeGet( &stFrmRct );	//	FRAME���Ă͂ߘg�̃T�C�Y

	//	�V��`��
	iYpos = topOst;
	FrameDrawItem( hDC, iYpos, gstFrmInsInfo.ptRoof );
	//StringCchLength( gstFrmInsInfo.ptRoof, STRSAFE_MAX_CCH, &cchSize );
	//ExtTextOut( hDC, 0, iYpos, 0, NULL, gstFrmInsInfo.ptRoof, cchSize, NULL );
	iYpos += LINE_HEIGHT;
	//	�ԕ`��
	//StringCchLength( gstFrmInsInfo.ptMiddle, STRSAFE_MAX_CCH, &cchSize );
	for( i = 0; gstFrmInsInfo.dMidLines > i; i++ )
	{
		FrameDrawItem( hDC, iYpos, gstFrmInsInfo.ptMiddle );
		//ExtTextOut( hDC, 0, iYpos, 0, NULL, gstFrmInsInfo.ptMiddle, cchSize, NULL );
		iYpos += LINE_HEIGHT;
	}
	//	���`��
	FrameDrawItem( hDC, iYpos, gstFrmInsInfo.ptFloor );
	//StringCchLength( gstFrmInsInfo.ptFloor, STRSAFE_MAX_CCH, &cchSize );
	//ExtTextOut( hDC, 0, iYpos, 0, NULL, gstFrmInsInfo.ptFloor, cchSize, NULL );



	SelectFont( hDC , hOldFnt );	//	�t�H���g�߂�

	return;
}
//-This way-----------------------------------------------------------------------------------------------

/*!
	��s���̕`��
	@param[in]	hDC		�`�悷��f�o�C�X�R���e�L�X�g
	@param[in]	iY		�`�悷�鍂���i���͏�ɂO�ł�낵�j
	@param[in]	ptLine	��s���̃f�[�^
*/
VOID FrameDrawItem( HDC hDC, INT iY, LPTSTR ptLine )
{
	UINT_PTR	cchSize, cl;
	UINT		iX, caret, len;
	INT			mRslt, mBase;
	LPTSTR		ptBgn;
	SIZE		stSize;

	StringCchLength( ptLine, STRSAFE_MAX_CCH, &cchSize );

	caret = 0;
	iX = 0;
	for( cl = 0; cchSize > cl; )
	{
		mRslt = iswspace(  ptLine[cl] );	//	�J�n�ʒu�̕����^�C�v�m�F
		ptBgn = &(ptLine[cl]);

		for( len= 0; cchSize > cl; len++, cl++ )
		{
			mBase = iswspace(  ptBgn[len] );	//	�����^�C�v���m�F���Ă���
			if( mRslt != mBase ){	break;	}	//	�^�C�v���ς������
		}
		GetTextExtentPoint32( hDC, ptBgn, len, &stSize );	//	�h�b�g���m�F

		if( mRslt ){	SetBkMode( hDC, TRANSPARENT );	}
		else{	SetBkMode( hDC, OPAQUE );	}

		ExtTextOut( hDC, iX, iY, 0, NULL, ptBgn, len, NULL );

		iX += stSize.cx;
	}

	return;
}
//-First Comes Rock-----------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��m�ۂ�����ʍ\���̃������Ƃ����I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Fib_OnDestroy( HWND hWnd )
{

	FREE( gstFrmInsInfo.ptRoof );
	FREE( gstFrmInsInfo.ptMiddle );
	FREE( gstFrmInsInfo.ptFloor );

	StatusBarSetText( SB_LAYER, TEXT("") );

	ghFrInbxWnd = NULL;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	��������Ă���Ƃ��ɔ����E�}�E�X�ŃE�C���h�E�h���b�O���Ƃ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstPos	���̏u�Ԃ̃X�N���[�����W
*/
VOID Fib_OnMoving( HWND hWnd, LPRECT pstPos )
{
	LONG	xEt, yEt, xLy, yLy, xSb, ySb;
	LONG	dLine, dRema;
	BOOLEAN	bMinus = FALSE;
	TCHAR	atBuffer[SUB_STRING];

	//	�t���[�����̍���X�N���[�����W
	xLy = pstPos->left + gstFrmSz.x;
	yLy = pstPos->top  + gstFrmSz.y;

	//	�r���[�̍���e�L�X�g�G���A�ʒu
	xEt = (gstViewOrigin.x + LINENUM_WID);
	yEt = (gstViewOrigin.y + RULER_AREA);
//	TRACE( TEXT("%d x %d"), xEt, yEt );

	//	�I�t�Z�b�g��
	xSb = xLy - xEt;	//	�w�͂��̂܂܃h�b�g��
	ySb = yLy - yEt;	//	�x���h�b�g�Ȃ̂ōs���ɂ��Ȃ��Ƃ����Ȃ�

	if( 0 > ySb ){	ySb *= -1;	bMinus = TRUE;	}	//	�}�C�i�X�␳
	//	�s���I�ȃ��m�����߂���Ă΂�
	dLine = ySb / LINE_HEIGHT;
	dRema = ySb % LINE_HEIGHT;
	if( (LINE_HEIGHT/2) < dRema ){	dLine++;	}
	if( bMinus ){	dLine *= -1;	}else{	dLine++;	}

	//	20110704	�����ł́A�܂��ʒu�̓X�N���[���̃Y�����l������ĂȂ�
	xSb += gdHideXdot;
	dLine += gdViewTopLine;

	StringCchPrintf( atBuffer, SUB_STRING, TEXT("Frame %d[dot] %d[line]"), xSb, dLine );
	StatusBarSetText( SB_LAYER, atBuffer );

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�B���h�E�̃T�C�Y�ύX����������O�ɑ����Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
	@return		����Message������������O
*/
BOOL Fib_OnWindowPosChanging( HWND hWnd, LPWINDOWPOS pstWpos )
{
	INT		clPosY, vwTopY, dSabun, dRem;
	BOOLEAN	bMinus = FALSE;
	RECT	vwRect;

	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOMOVE & pstWpos->flags )	return TRUE;

	clPosY = pstWpos->y + gstFrmSz.y;	//	�\���ʒu��TOP

	//	�\����������s�P�ʂɍ��킹��
	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;
	vwTopY = (vwRect.top  + RULER_AREA);

	dSabun = vwTopY - clPosY;
	if( 0 > dSabun ){	dSabun *= -1;	bMinus = TRUE;	}

	dRem = dSabun % LINE_HEIGHT;

	if( 0 == dRem ){	return TRUE;	}

	if( (LINE_HEIGHT/2) < dRem ){	dRem = dRem - LINE_HEIGHT;	}

	if( bMinus ){	dRem *=  -1;	}

	pstWpos->y += dRem;

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�B���h�E�̃T�C�Y�ύX�����������瑗���Ă���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	pstWpos	�V�����ʒu�Ƒ傫���������Ă�
*/
VOID Fib_OnWindowPosChanged( HWND hWnd, const LPWINDOWPOS pstWpos )
{
	RECT	vwRect;

	MoveWindow( ghFIBtlbrWnd, 0, 0, 0, 0, TRUE );	//	�c�[���o�[�͐��l�Ȃ��Ă�����ɍ��킹�Ă����

	FrameInsBoxInfoCheck(  );	//	�\�����ύX���čĕ`�悷��
	InvalidateRect( hWnd, NULL, TRUE );

	//	�ړ����Ȃ������Ƃ��͉������Ȃ��ł���
	if( SWP_NOMOVE & pstWpos->flags )	return;

	GetWindowRect( ghViewWnd, &vwRect );
	gstViewOrigin.x = vwRect.left;//�ʒu�L�^�E���������ς����̂���Ȃ�
	gstViewOrigin.y = vwRect.top;

	gstOffset.x = pstWpos->x - vwRect.left;
	gstOffset.y = pstWpos->y - vwRect.top;

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�r���[���ړ�����
	@param[in]	hWnd	�{�̃E�C���h�E�n���h���E���܂�Ӗ��͂Ȃ�
	@param[in]	state	����ԁE�ŏ����Ȃ�Ⴄ�R�g����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FrameMoveFromView( HWND hWnd, UINT state )
{
	RECT	vwRect = {0,0,0,0};
	POINT	lyPoint;

	if( !(ghFrInbxWnd) )	return S_FALSE;

	//	�ŏ������͔�\���ɂ���Ƃ�	SIZE_MINIMIZED

	if( SIZE_MINIMIZED != state )
	{
		GetWindowRect( ghViewWnd, &vwRect );
		gstViewOrigin.x = vwRect.left;//�ʒu�L�^
		gstViewOrigin.y = vwRect.top;
	}

	if( SIZE_MINIMIZED == state )
	{
		ShowWindow( ghFrInbxWnd, SW_HIDE );
	}
	else
	{
		lyPoint.x = gstOffset.x + vwRect.left;
		lyPoint.y = gstOffset.y + vwRect.top;
#ifdef _DEBUG
		SetWindowPos( ghFrInbxWnd, HWND_TOP, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#else
		SetWindowPos( ghFrInbxWnd, HWND_TOPMOST, lyPoint.x, lyPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
#endif
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------


