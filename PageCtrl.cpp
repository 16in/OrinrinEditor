/*! @file
	@brief �t�@�C���̃y�[�W�P�ʂ��Ǘ����܂�
	���̃t�@�C���� PageCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/20
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

//	TODO:	�e�ł̏ڍׂ�\���o����悤�ɂ���
//	TODO:	�I��ł̂ݕۑ��Ƃ��A�Ȃ񂩂���ȋ@�\�ق���


#define PAGELIST_CLASS	TEXT("PAGE_LIST")
#define PL_WIDTH	110
#define PL_HEIGHT	300
//-------------------------------------------------------------------------------------------------

//	�z�C�z�C���L���Ă����̂��炤��

extern FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
#define gstFile	(*gitFileIt)	//!<	�C�e���[�^���\���̂ƌ��Ȃ�

extern INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X
extern INT		gixDropPage;	//!<	�����z�b�g�ԍ�
//-------------------------------------------------------------------------------------------------

static HINSTANCE	ghInst;		//!<	���̃A�v���̎����l

static  ATOM	gPageAtom;		//!<	�N���X�A�g��
static  HWND	ghPageWnd;		//!<	���̃E�C���h�E�̃n���h��

static  HWND	ghToolWnd;		//!<	�c�[���o�[
static  HWND	ghPageListWnd;	//!<	�y�[�W���X�g�r���[�n���h��

static  HWND	ghPageTipWnd;	//!<	�c�[���`�b�v�n���h��
static HFONT	ghPgTipFont;	//!<	�c�[���`�b�v�p
static LPTSTR	gptPgTipBuf;	//!<	

static INT		gixMouseSel;	//!<	�}�E�X�J�[�\�����̃A��

static BOOLEAN	gbPgTipView;	//!<	�Ńc�[���e�B�b�v�\��ON/OFF

static WNDPROC	gpfOrigPageViewProc;	//!<	
static WNDPROC	gpfOrigPageToolProc;

static HIMAGELIST	ghPgLstImgLst;

extern INT	gbTmpltDock;		//	�ŁE��s�e���v���̃h�b�L���O


extern  HWND	ghMainSplitWnd;	//!<	���C���̃X�v���b�g�o�[�n���h��
extern  LONG	grdSplitPos;	//!<	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g
//-------------------------------------------------------------------------------------------------

//	�c�[���o�[�E�V�K�쐬�Ƃ�
#define PGTB_ITEMS	9
static TBBUTTON gstPgTlBarInfo[] = {
/*�V�K*/{  0,	IDM_PAGEL_ADD,		TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*�}��*/{  1,	IDM_PAGEL_INSERT,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*����*/{  2,	IDM_PAGEL_DUPLICATE,TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*�폜*/{  3,	IDM_PAGEL_DELETE,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*����*/{  4,	IDM_PAGEL_COMBINE,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*���*/{  5,	IDM_PAGEL_UPFLOW,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*����*/{  6,	IDM_PAGEL_DOWNSINK,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*����*/{  7,	IDM_PAGEL_RENAME,	TBSTATE_ENABLED | TBSTATE_WRAP,	BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 },	//	
/*�X�V*/{  8,	IDM_PAGEL_DETAIL,	TBSTATE_WRAP,					BTNS_BUTTON | BTNS_AUTOSIZE,	0,	0 } 	//	
};	//	���e�ύX������A�c�[���o�[������̐ݒ�Ƃ����ύX�Z��

//-------------------------------------------------------------------------------------------------


LRESULT	CALLBACK PageListProc( HWND, UINT, WPARAM, LPARAM );
VOID	Plt_OnCommand( HWND, INT, HWND, UINT );
VOID	Plt_OnSize( HWND, UINT, INT, INT );
LRESULT	Plt_OnNotify( HWND, INT, LPNMHDR );
VOID	Plt_OnContextMenu( HWND, HWND, UINT, UINT );

LRESULT	PageListNotify( HWND, LPNMLISTVIEW );
HRESULT	PageListNameChange( INT );
HRESULT	PageListSpinning( HWND, INT, INT );
HRESULT	PageListViewRewrite( INT );
HRESULT	PageListDuplicate( HWND, INT );
HRESULT PageListCombine( HWND, INT );

HRESULT	PageListJump( INT );

LRESULT	CALLBACK gpfPageViewProc( HWND, UINT, WPARAM, LPARAM );
LRESULT	Plv_OnNotify( HWND , INT, LPNMHDR );	//!<	
VOID	Plv_OnMouseMove( HWND, INT, INT, UINT );	//!<	

LRESULT	CALLBACK gpfPageToolProc( HWND, UINT, WPARAM, LPARAM );

INT_PTR	CALLBACK PageNameDlgProc( HWND, UINT, WPARAM, LPARAM );
//-------------------------------------------------------------------------------------------------


/*!
	�y�[�W�p�肷�Ƃт�`�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�e�E�C���h�E�̃n���h��
	@param[in]	pstFrame	���C���N���C�����g�̈�
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND PageListInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame )
{

	LVCOLUMN	stLvColm;
//	TBADDBITMAP	stToolBmp;
	TTTOOLINFO	stToolInfo;
	RECT		tbRect;
//	LONG_PTR	tbixStd;
	DWORD		dwExStyle, dwStyle;
	TCHAR		atBuff[MAX_STRING];
	HWND		hPrWnd;

	UINT		ici, resnum;
	HBITMAP		hImg, hMsq;
	INT			spPos;

	LOGFONT	stFont;

	WNDCLASSEX	wcex;
	RECT	wdRect, clRect, rect;

	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= PageListProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_PGLVPOPUPMENU);
	wcex.lpszClassName	= PAGELIST_CLASS;
	wcex.hIconSm		= NULL;

	gPageAtom = RegisterClassEx( &wcex );

	ghInst = hInstance;

	gixMouseSel = -1;

	gbPgTipView = InitParamValue( INIT_LOAD, VL_PAGETIP_VIEW, 1 );

	InitWindowPos( INIT_LOAD, WDP_PLIST, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
		rect.left   = wdRect.left - PL_WIDTH;
		rect.top    = wdRect.top;
		rect.right  = PL_WIDTH;
		rect.bottom = PL_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_PLIST, &rect );	//	�N�����ۑ�
	}

	if( gbTmpltDock )	//	���[���E�B���h�[�Ƀh�b�L������
	{
		spPos = grdSplitPos - SPLITBAR_WIDTH;	//	�E����̃I�t�Z�b�g

		hPrWnd    = hParentWnd;
		dwExStyle = 0;
		dwStyle   = WS_CHILD | WS_VISIBLE;
		rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
		rect.left  = rect.right - spPos;
		rect.right = PLIST_DOCK;
		rect.bottom >>= 1;
	}
	else
	{
		dwExStyle = WS_EX_TOOLWINDOW;
		if( InitWindowTopMost( INIT_LOAD, WDP_PLIST , 0 ) ){	dwExStyle |=  WS_EX_TOPMOST;	}
		dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE;
		hPrWnd = NULL;
	}
	ghPageWnd = CreateWindowEx( dwExStyle, PAGELIST_CLASS, TEXT("Page List"), dwStyle,
		rect.left, rect.top, rect.right, rect.bottom, hPrWnd, NULL, hInstance, NULL);


	GetClientRect( ghPageWnd, &clRect );

	ghToolWnd = CreateWindowEx( 0, TOOLBARCLASSNAME, TEXT("pagetoolbar"),
		WS_CHILD | WS_VISIBLE | CCS_NORESIZE | CCS_LEFT | CCS_NODIVIDER | TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS,// | TBSTYLE_WRAPABLE,
		0, 0, 0, 0, ghPageWnd, (HMENU)IDTB_PAGE_TOOLBAR, hInstance, NULL);
	//	�����c�[���`�b�v�X�^�C����ǉ�
	SendMessage( ghToolWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_MIXEDBUTTONS );

	//stToolBmp.hInst = HINST_COMMCTRL;
	//stToolBmp.nID   = IDB_STD_SMALL_COLOR;
	//tbixStd = SendMessage( ghToolWnd, TB_ADDBITMAP, 0, (LPARAM)&stToolBmp );
	ghPgLstImgLst = ImageList_Create( 16, 16, ILC_COLOR24 | ILC_MASK, 9, 1 );
	resnum = IDBMPQ_PAGE_TB_FIRST;
	for( ici = 0; 9 > ici; ici++ )
	{
		hImg = LoadBitmap( hInstance, MAKEINTRESOURCE( (resnum++) ) );
		hMsq = LoadBitmap( hInstance, MAKEINTRESOURCE( (resnum++) ) );
		ImageList_Add( ghPgLstImgLst, hImg, hMsq );	//	�C���[�W���X�g�ɃC���[�W��ǉ�
		DeleteBitmap( hImg );	DeleteBitmap( hMsq );
	}
	SendMessage( ghToolWnd, TB_SETIMAGELIST, 0, (LPARAM)ghPgLstImgLst );

	SendMessage( ghToolWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0 );
	//	�c�[���`�b�v�������ݒ�E�{�^���e�L�X�g���c�[���`�b�v�ɂȂ�
	StringCchCopy( atBuff, MAX_STRING, TEXT("�����ɐV�K�쐬\r\nAlt + Shift + I ") );	gstPgTlBarInfo[ 0].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�I��ł̎��ɑ}��\r\nAlt + I") );			gstPgTlBarInfo[ 1].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�I��ł𕡐�\r\nAlt + C") );				gstPgTlBarInfo[ 2].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�I��ł��폜\r\nAlt + D") );				gstPgTlBarInfo[ 3].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("���̕łƓ���\r\nAlt + G") );				gstPgTlBarInfo[ 4].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�ł���ֈړ�\r\nAlt + U") );				gstPgTlBarInfo[ 5].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�ł����ֈړ�\r\nAlt + J") );				gstPgTlBarInfo[ 6].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�Ŗ��̂̕ύX\r\nAlt + N") );				gstPgTlBarInfo[ 7].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );
	StringCchCopy( atBuff, MAX_STRING, TEXT("�ŐV�̏��ɍX�V") );						gstPgTlBarInfo[ 8].iString = SendMessage( ghToolWnd, TB_ADDSTRING, 0, (LPARAM)atBuff );

	SendMessage( ghToolWnd, TB_SETROWS, MAKEWPARAM(PGTB_ITEMS,TRUE), (LPARAM)(&tbRect) );

	SendMessage( ghToolWnd, TB_ADDBUTTONS, (WPARAM)PGTB_ITEMS, (LPARAM)&gstPgTlBarInfo );	//	�c�[���o�[�Ƀ{�^����}��

	SendMessage( ghToolWnd, TB_GETITEMRECT, 0, (LPARAM)(&tbRect) );
	MoveWindow( ghToolWnd, 0, 0, tbRect.right, rect.bottom, TRUE );
	InvalidateRect( ghToolWnd , NULL, TRUE );	//	�N���C�����g�S�̂��ĕ`�悷��

//	�T�u�N���X��
	gpfOrigPageToolProc = SubclassWindow( ghToolWnd, gpfPageToolProc );

	tbRect.bottom  = rect.bottom;
	tbRect.left    = 0;
	tbRect.top     = 0;

//���X�g�r���[
	ghPageListWnd = CreateWindowEx( 0, WC_LISTVIEW, TEXT("pagelist"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LVS_REPORT | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
		tbRect.right, clRect.top, clRect.right - tbRect.right, clRect.bottom, ghPageWnd,
		(HMENU)IDLV_PAGELISTVIEW, hInstance, NULL );
	ListView_SetExtendedListViewStyle( ghPageListWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	gpfOrigPageViewProc = SubclassWindow( ghPageListWnd, gpfPageViewProc );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt = LVCFMT_LEFT;
	stLvColm.pszText = TEXT("No");		stLvColm.cx =  28;	stLvColm.iSubItem = 0;	ListView_InsertColumn( ghPageListWnd, 0, &stLvColm );
	stLvColm.pszText = TEXT("��");		stLvColm.cx =  67;	stLvColm.iSubItem = 1;	ListView_InsertColumn( ghPageListWnd, 1, &stLvColm );
	stLvColm.pszText = TEXT("Byte");	stLvColm.cx =  45;	stLvColm.iSubItem = 2;	ListView_InsertColumn( ghPageListWnd, 2, &stLvColm );
	stLvColm.pszText = TEXT("Line");	stLvColm.cx =  45;	stLvColm.iSubItem = 3;	ListView_InsertColumn( ghPageListWnd, 3, &stLvColm );

//�c�[���`�b�v
	ghPageTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghPageWnd, NULL, hInstance, NULL );


	ViewingFontGet( &stFont );
	stFont.lfHeight = FONTSZ_REDUCE;
	ghPgTipFont = CreateFontIndirect( &stFont );
//	ghPgTipFont = CreateFont( FONTSZ_REDUCE, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("�l�r �o�S�V�b�N") );
	SetWindowFont( ghPageTipWnd, ghPgTipFont, TRUE );

	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghPageListWnd;
	stToolInfo.uId      = IDLV_PAGELISTVIEW;
	GetClientRect( ghPageListWnd, &stToolInfo.rect );
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghPageTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghPageTipWnd, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�

//	�c�[���`�b�v�̃|�b�v�f�B���C�́ATTM_SETDELAYTIME �Œ����o����
//	wParam	TTDT_INITIAL	�\���܂ł̎���
//	lParam	The LOWORD specifies the delay time, in milliseconds. The HIWORD must be zero.
//	SendMessage( ghPageTipWnd, TTM_SETDELAYTIME, TTDT_INITIAL, MAKELPARAM(2222,0) );
	//	���ʖ���

	ShowWindow( ghPageWnd, SW_SHOW );
	UpdateWindow( ghPageWnd );

	return ghPageWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�[���o�[�T�u�N���X
	WindowsXP�ŁA�c�[���o�[�̃{�^����Ń}�E�X�̍��{�^�����������܂܉E�{�^���������ƁA
	����ȍ~�̃}�E�X����𐳏�Ɏ󂯕t���Ȃ��Ȃ�B����̑΍�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
*/
LRESULT CALLBACK gpfPageToolProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			if( SendMessage(hWnd, TB_GETHOTITEM, 0, 0) >= 0 ){	ReleaseCapture(   );	}
			return 0;
	}

	return CallWindowProc( gpfOrigPageToolProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK PageListProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_COMMAND,     Plt_OnCommand );	
		HANDLE_MSG( hWnd, WM_SIZE,        Plt_OnSize );	
		HANDLE_MSG( hWnd, WM_NOTIFY,      Plt_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Plt_OnContextMenu );

		case WM_DESTROY:
			SetWindowFont( ghPageTipWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );
			DeleteFont( ghPgTipFont );
			FREE( gptPgTipBuf );
			ImageList_Destroy( ghPgLstImgLst );
			return 0;

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
VOID Plt_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	INT	iPage, iItem, mRslt, iDiff;
	LONG_PTR	rdExStyle;

	switch( id )
	{
		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_PAGE,  1 );	return;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_PAGE, -1 );	return;

		case IDM_PAGEL_ADD:	//	�����V�K�쐬�͂��ł��L��
			iPage = DocPageCreate( -1 );
			PageListInsert( iPage );	//	�y�[�W���X�g�r���[�ɒǉ�
			DocPageChange( iPage );
			DocModifyContent( TRUE );
			DocFileBackup( hWnd );	//	�o�b�N�A�b�v���Ă���
			ViewFocusSet(  );	//	�t�H�[�J�X�߂�
			return;

		case IDM_TOPMOST_TOGGLE:	//	�펞�őS�ʂƒʏ�E�C���h�E�̃g�O��
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_PLIST, 0 );
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_PLIST, 1 );
			}
			return;

		case IDM_PAGEL_AATIP_TOGGLE:
			gbPgTipView = gbPgTipView ? FALSE : TRUE;
			InitParamValue( INIT_SAVE, VL_PAGETIP_VIEW, gbPgTipView );
			return;

		default:	break;
	}

	//	�I������Ă鍀�ڂ��m��
	iItem = ListView_GetNextItem( ghPageListWnd, -1, LVNI_ALL | LVNI_SELECTED );

	//	�I������Ă郂�m���Ȃ��Ɩ��Ӗ�
	if( 0 >  iItem ){	iItem = gixFocusPage;	}
	//	���I���Ȃ�A���ݕł��I������Ă��郂�m�ƌ��Ȃ�

	switch( id )
	{
		case IDM_PAGE_PREV:	//	�O�̕łֈړ�
			iDiff = iItem - 1;
			PageListJump( iDiff );
			return;

		case IDM_PAGE_NEXT:	//	���̕łֈړ�
			iDiff = iItem + 1;
			PageListJump( iDiff );
			return;
		//	20120110	�ňړ�������ύX�L��ɂȂ����Ⴄ�̏C��

		case IDM_PAGEL_INSERT:	//	�C�ӈʒu�V�K�쐬
			iPage = DocPageCreate( iItem );
			PageListInsert( iPage );	//	�y�[�W���X�g�r���[�ɒǉ�
			DocPageChange( iPage );
			DocFileBackup( hWnd );	//	�o�b�N�A�b�v���Ă���
			break;

		case IDM_PAGEL_RENAME:	//	�Ŗ��̕ύX
			if( FAILED( PageListNameChange( iItem ) ) ){	 return;	}
			break;

		case IDM_PAGEL_DELETE:	//	���̕ł��폜
			//	�m�F�����
			mRslt = MessageBoxCheckBox( hWnd, ghInst, 2 );
			if( IDYES == mRslt ){	DocPageDelete( iItem  );	}
			break;

		case IDM_PAGEL_DIVIDE:	//	�����̓r���[�̃��j���[���낤
			break;

		case IDM_PAGEL_COMBINE:	//	����
			//	�m�F�����
			mRslt = MessageBoxCheckBox( hWnd, ghInst, 0 );
			if( IDYES == mRslt ){	PageListCombine( hWnd , iItem );	}
			break;

		case IDM_PAGEL_UPFLOW:	//	���ړ�
			PageListSpinning( hWnd, iItem, 1 );
			break;

		case IDM_PAGEL_DOWNSINK:	//	���ړ�
			PageListSpinning( hWnd, iItem, -1 );
			break;

		case IDM_PAGEL_DUPLICATE:	//	�ŕ���
			PageListDuplicate( hWnd, iItem );
			break;

		case IDM_PAGEL_DETAIL:
			TRACE( TEXT("�ŐV�̏��ɍX�V") );
			return;

		default:	TRACE( TEXT("������[%d]"), id );	return;
	}

	//	�X�V�ɂȂ��͂������ɖ߂�
	DocModifyContent( TRUE );

	ViewFocusSet(  );	//	�t�H�[�J�X�߂�

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�T�C�Y�ύX���ꂽ
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	state	�Ȃɂ��̏��
	@param[in]	cx		�ύX���ꂽ�N���C�����g��
	@param[in]	cy		�ύX���ꂽ�N���C�����g����
*/
VOID Plt_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	RECT	tbRect;
	TTTOOLINFO	stToolInfo;

	tbRect.right = 0;
	if( ghToolWnd )
	{
		SendMessage( ghToolWnd, TB_GETITEMRECT, 0, (LPARAM)(&tbRect) );
		MoveWindow( ghToolWnd, 0, 0, tbRect.right, cy, TRUE );
	}

	MoveWindow( ghPageListWnd, tbRect.right, 0, cx - tbRect.right, cy, TRUE );

	//	�K�v�ȏ����������΂���
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize = sizeof(TTTOOLINFO);
	stToolInfo.hwnd   = ghPageListWnd;
	stToolInfo.uId    = IDLV_PAGELISTVIEW;
	GetClientRect( ghPageListWnd, &stToolInfo.rect );
	SendMessage( ghPageTipWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&stToolInfo );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Plt_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	//	�y�[�W���X�g�r���[
	if( IDLV_PAGELISTVIEW == idFrom ){	return PageListNotify( hWnd, (LPNMLISTVIEW)pstNmhdr );	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		����
*/
VOID Plt_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	INT		iCount, iItem;
	BOOLEAN	bSel;
	LONG_PTR	rdExStyle;

	POINT	stPoint;

	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	bSel = FALSE;
	iCount  = ListView_GetItemCount( ghPageListWnd );
	iItem = ListView_GetNextItem( ghPageListWnd, -1, LVNI_ALL | LVNI_SELECTED);
	if( 0 <= iItem )	bSel = TRUE;


	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_PGLVPOPUPMENU) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	//	�ł��P�����Ȃ��Ȃ�A�폜�𖳌���
	if( 1 >= iCount ){	EnableMenuItem( hSubMenu, IDM_PAGEL_DELETE, MF_GRAYED );	}

	if( gbTmpltDock )
	{
		EnableMenuItem( hSubMenu, IDM_TOPMOST_TOGGLE, MF_GRAYED );
	}
	else
	{
		//	�őO�ʂł��邩
		rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
		if( WS_EX_TOPMOST & rdExStyle ){	CheckMenuItem( hSubMenu , IDM_TOPMOST_TOGGLE, MF_BYCOMMAND | MF_CHECKED );	}
	}

	//	�|�b�p�b�v�\���L�邩�H
	if( gbPgTipView ){	CheckMenuItem( hSubMenu, IDM_PAGEL_AATIP_TOGGLE, MF_CHECKED );	}

	//	���I���Ȃ�A�I�����ĂȂ��Ǝg���Ȃ��@�\�𖳌���
	if( !(bSel) )
	{
		EnableMenuItem( hSubMenu, IDM_PAGEL_INSERT, MF_GRAYED );	//	�C�Ӎ쐬
		EnableMenuItem( hSubMenu, IDM_PAGEL_DELETE, MF_GRAYED );	//	�폜
		EnableMenuItem( hSubMenu, IDM_PAGEL_COMBINE, MF_GRAYED );	//	����
		EnableMenuItem( hSubMenu, IDM_PAGEL_UPFLOW, MF_GRAYED );	//	����
		EnableMenuItem( hSubMenu, IDM_PAGEL_DOWNSINK, MF_GRAYED );	//	���~
		EnableMenuItem( hSubMenu, IDM_PAGEL_DUPLICATE, MF_GRAYED );	//	����
		EnableMenuItem( hSubMenu, IDM_PAGEL_RENAME, MF_GRAYED );	//	����
	}

	dRslt = TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
	//	�I�������łO���|�P�H�A�I�������炻�̃��j���[�̂h�c��WM_COMMAND�����s
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/* !
	�e�E�C���h�E���ړ�������傫���ς������
	@param[in]	hPrntWnd	�e�E�C���h�E�n���h��
	@param[in]	pstFrame	�N���C�A���g�T�C�Y
*/
VOID PageListResize( HWND hPrntWnd, LPRECT pstFrame )
{
	RECT	rect;
//gbTmpltDock

	rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
	rect.left  = rect.right - (grdSplitPos - SPLITBAR_WIDTH);
	rect.right = (grdSplitPos - SPLITBAR_WIDTH);
	rect.bottom >>= 1;

	SetWindowPos( ghPageWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�т�[��NOTIFY���b�Z�[�W����
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@param[in]	pstLv	�ǉ����\���̂ւ̃|�C���^�[
	@return		LRESULT	�Ȃ񂩂��낢������
*/
LRESULT PageListNotify( HWND hWnd, LPNMLISTVIEW pstLv )
{
	HWND	hLvWnd;
	INT		iCount, iItem, nmCode;//, iPage;

	DWORD	lvClmn;
	INT		lvLine;
	LPNMLVCUSTOMDRAW	pstDraw;

	hLvWnd = pstLv->hdr.hwndFrom;
	nmCode = pstLv->hdr.code;
	//	�Ȃ�炩�̃A�N�V�����̋N������ROW�ʒu���Q�b�g���� 
	iCount = ListView_GetItemCount( hLvWnd );
	iItem  = pstLv->iItem;

	//	���ʂ̃N���b�N�ɂ���
	if( NM_CLICK == nmCode )
	{
		if( 0 <= iItem )	//	�Y���y�[�W�ւ̈ړ�
		{
			TRACE( TEXT("�y�[�W�I��[%d]"), iItem );
			DocPageChange( iItem );
		}
	}

	//	�_�u���N���b�N
	if( NM_DBLCLK == nmCode )
	{
		if( 0 <= iItem )	//	�Ŗ���DIALOGUE���j���L
		{
			if( SUCCEEDED( PageListNameChange( iItem ) ) )
			{
				DocModifyContent( TRUE );
			}
		}
	}


//�J�X�^���h���[�E�T�u�N���X�̒����Ə�肭�����Ȃ��EWhy?
	if( NM_CUSTOMDRAW == nmCode )
	{
		pstDraw = (LPNMLVCUSTOMDRAW)pstLv;

		//	�����ň����������e��Ԃ��B�����鏉��o�^
		if( pstDraw->nmcd.dwDrawStage == CDDS_PREPAINT ){		return CDRF_NOTIFYSUBITEMDRAW;	}
		if( pstDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT ){	return CDRF_NOTIFYSUBITEMDRAW;	}

		if( pstDraw->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT|CDDS_SUBITEM) )//(CDDS_ITEMPREPAINT|CDDS_SUBITEM)
		{
			lvLine = pstDraw->nmcd.dwItemSpec;	//	�s
			lvClmn = pstDraw->iSubItem;			//	�J����

			//	�ĕ`�悹���Ƃ��������ɕς��
			if( 2 == lvClmn )
			{
				if( PAGE_BYTE_MAX < gstFile.vcCont.at( lvLine ).dByteSz )
					pstDraw->clrTextBk = 0x000000FF;
				else
					pstDraw->clrTextBk = 0xFF000000;
			}
			else
			{
				pstDraw->clrTextBk = 0xFF000000;	//	����Ńf�t�H�F�w��
			}

			return CDRF_NEWFONT;
		}
	}

	return 0;	//	�ʏ�͂O
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�r���[�N����
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT PageListClear( VOID )
{
	ListView_DeleteAllItems( ghPageListWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă���œ��e��ύX
	@param[in]	dPage	�J���łO�C���f�b�N�X�E�\���͂P�C���f�b�N�X�Ȃ̂Œ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListViewChange( INT dPage )
{
	//	�t�H�[�J�X�y�[�W�́A�����ɗ���O�ɕύX���Ă�������

	LONG	iItem;

	iItem = ListView_GetItemCount( ghPageListWnd );
	if( iItem <= dPage )	return E_OUTOFMEMORY;

	//	�I����Ԃ�ύX
	ListView_SetItemState( ghPageListWnd, dPage, LVIS_SELECTED, LVIS_SELECTED );

	//	�r���[����������
	ViewEditReset(  );

	//	�����ŊJ�����ł𓊉��z�b�g�L�[�ԍ��ɃZ�b�g
	gixDropPage = dPage;

//	ViewFocusSet(  );	//	�����ł̓t�H�[�J�X���Ȃ��ق����ǂ���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̈ʒu�Ƀy�[�W��ǉ�
	@param[in]	iBefore	�|�P�Ȃ疖���ǉ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListInsert( INT iBefore )
{
	//	���X�g�r���[�̓r���}�����Ăł��������H
	UINT	iItem, i;
	TCHAR	atBuffer[MIN_STRING];
	LVITEM	stLvi;

	iItem = ListView_GetItemCount( ghPageListWnd );

	//	�Ƃ肠�������[�ɒǉ��E�r���ǉ��͌��

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask  = LVIF_TEXT;

	//	�ꗗ�ԍ��͏��increment�ł����̂��H

	ZeroMemory( atBuffer, sizeof(atBuffer) );

	if( 0 > iBefore )	//	�����ǉ��Ȃ��s���₹�΂���
	{
		stLvi.iItem = iItem;

		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), iItem + 1 );
		stLvi.pszText  = atBuffer;
	}
	else
	{
		stLvi.iItem = iBefore;
		stLvi.pszText  = TEXT("");
	}

	stLvi.iSubItem = 0;
	ListView_InsertItem( ghPageListWnd, &stLvi );

	stLvi.pszText  = TEXT("");
	stLvi.iSubItem =  1;
	ListView_SetItem( ghPageListWnd, &stLvi );

	stLvi.pszText = TEXT("0");	//	byte
	stLvi.iSubItem =  2;
	ListView_SetItem( ghPageListWnd, &stLvi );

	stLvi.pszText = TEXT("1");	//	line
	stLvi.iSubItem =  3;
	ListView_SetItem( ghPageListWnd, &stLvi );


	if( 0 <= iBefore )
	{
		//	�A�ԐU�蒼��
		iItem = ListView_GetItemCount( ghPageListWnd );
		for( i = iBefore; iItem > i; i++ )
		{
			StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), i + 1 );
			ListView_SetItemText( ghPageListWnd, i, 0, atBuffer );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ń��X�g���č\������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListBuild( LPVOID pVoid )
{
	INT	i;
	PAGE_ITR	itPage;
	LVITEM	stLvi;
	TCHAR	atBuffer[MIN_STRING];

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask  = LVIF_TEXT;

	i = 0;
	for( itPage = gstFile.vcCont.begin(); itPage != gstFile.vcCont.end(); itPage++ )
	{
		stLvi.iItem    = i;
		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), i + 1 );
		stLvi.pszText  = atBuffer;
		stLvi.iSubItem = 0;
		ListView_InsertItem( ghPageListWnd, &stLvi );

		stLvi.pszText  = itPage->atPageName;
		stLvi.iSubItem =  1;
		ListView_SetItem( ghPageListWnd, &stLvi );

		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), itPage->dByteSz );
		stLvi.pszText  = atBuffer;
		stLvi.iSubItem =  2;
		ListView_SetItem( ghPageListWnd, &stLvi );

		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), itPage->vcPage.size() );
		stLvi.iSubItem =  3;
		ListView_SetItem( ghPageListWnd, &stLvi );

		i++;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�΂ꂽ�ł��㉺�ړ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	iPage	�ړ�������Ŕԍ�
	@param[in]	bDir	���F��ց@���F����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListSpinning( HWND hWnd, INT iPage, INT bDir )
{
	INT	iItem, i = 0;
	PAGE_ITR	itPage, itSwap;

	iItem = ListView_GetItemCount( ghPageListWnd );

	if( 0 == bDir ){	return E_INVALIDARG;	}	//	���܂�Ӗ��͂Ȃ�

	//	����ȏな�ɂ����Ȃ��Ȃ牽�����Ȃ�
	if( (0 == iPage) && (0 < bDir) ){	return  E_ABORT;	}

	//	����ȏに�ɃC�P�i�C�Ȃ牽�����Ȃ�
	if( (iItem <= (iPage+1)) && (0 > bDir) ){	return  E_ABORT;	}

	TRACE( TEXT("�ňړ�����[%d]"), iPage );

	//	�X���ʒu�܂ŃC�e���[�^�������Ă���
	itPage = gstFile.vcCont.begin(  );
	//for( i = 0; iPage > i; i++ ){	itPage++;	}
	advance( itPage, iPage );

	//	�X���b�v�Ώ�
	if( 0 <  bDir ){	itSwap = itPage - 1;	i = iPage - 1;	}	//	���Ɉړ��Ȃ璼�O�̂�ƌ���
	if( 0 >  bDir ){	itSwap = itPage + 1;	i = iPage + 1;	}	//	���Ȃ璼��̂ƃ`�F���W�Q�b�^�[

	iter_swap( itPage, itSwap );	//	�X���b�s���O�I

	//	�\�����e����ւ��āA�I�������Ă���
	PageListViewRewrite( iPage );
	PageListViewRewrite( i );

	//	�I����Ԃ�ύX
	ListView_SetItemState( ghPageListWnd, i, LVIS_SELECTED, LVIS_SELECTED );

	DocPageChange( i );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ꗗ�̎w��̕ł��폜����
	@param[in]	iPage	�T�N�b���Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListDelete( INT iPage )
{
	UINT	iItem, i;
	TCHAR	atBuffer[MIN_STRING];

	ListView_DeleteItem( ghPageListWnd, iPage );
	//	Delete������A���X�g�r���[�͎����ŋl�߂���

	//	�A�ԐU�蒼��
	iItem = ListView_GetItemCount( ghPageListWnd );
	for( i = 0; iItem > i; i++ )
	{
		StringCchPrintf( atBuffer, MIN_STRING, TEXT("%u"), i + 1 );
		ListView_SetItemText( ghPageListWnd, i, 0, atBuffer );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�e�ł̏󋵁E�����͂��̂����\���̂ɂ����ق�����������
	@param[in]	dPage	�Ŕԍ�
	@param[in]	dByte	�o�C�g��
	@param[in]	dLine	�s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListInfoSet( INT dPage, INT dByte, INT dLine )
{
	INT		iPage;
	TCHAR	atBuffer[MIN_STRING];

	iPage = ListView_GetItemCount( ghPageListWnd );
	if( iPage <= dPage )	return E_OUTOFMEMORY;

	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dByte );	//	byte
	ListView_SetItemText( ghPageListWnd, dPage, 2, atBuffer );

	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dLine );	//	line
	ListView_SetItemText( ghPageListWnd, dPage, 3, atBuffer );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	����ł̏���������������
	@param[in]	dPage	�Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListViewRewrite( INT dPage )
{
	UINT_PTR	dLines;
	UINT		dBytes;
	INT			iPage;
	TCHAR	atBuffer[MIN_STRING];

	iPage = ListView_GetItemCount( ghPageListWnd );
	if( iPage <= dPage )	return E_OUTOFMEMORY;

	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dPage + 1 );
	ListView_SetItemText( ghPageListWnd, dPage, 0, atBuffer );

	ListView_SetItemText( ghPageListWnd, dPage, 1, gstFile.vcCont.at( dPage ).atPageName );

	dBytes = gstFile.vcCont.at( dPage ).dByteSz;
	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dBytes );	//	byte
	ListView_SetItemText( ghPageListWnd, dPage, 2, atBuffer );

	dLines = gstFile.vcCont.at( dPage ).vcPage.size( );
	StringCchPrintf( atBuffer, MIN_STRING, TEXT("%d"), dLines );	//	line
	ListView_SetItemText( ghPageListWnd, dPage, 3, atBuffer );



	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X�ł��ړ�
	@param[in]	iDiff	�ړ���E�͈͊O��������Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListJump( INT iDiff )
{
	INT	iItem;

	if( 0 >  iDiff )	return E_OUTOFMEMORY;

	iItem = ListView_GetItemCount( ghPageListWnd );
	if( iItem <= iDiff )	return E_OUTOFMEMORY;

	ListView_EnsureVisible( ghPageListWnd, iDiff, FALSE );

	DocPageChange( iDiff );

	ViewFocusSet(  );	//	�t�H�[�J�X�߂�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���̕ύX�_�C�����O�{�b�N�X�̃��Z�[�W�n���h�������Ă΂�
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK PageNameDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static INT cdPage;
//	TCHAR	atBuffer[MAX_PATH];

	switch( message )
	{
		case WM_INITDIALOG:
			cdPage = lParam;
			Edit_SetText( GetDlgItem(hDlg,IDE_PAGENAME), gstFile.vcCont.at( cdPage ).atPageName );
			SetFocus( GetDlgItem(hDlg,IDE_PAGENAME) );
			return (INT_PTR)FALSE;

		case WM_COMMAND:
			if( IDOK == LOWORD(wParam) )
			{
				Edit_GetText( GetDlgItem(hDlg,IDE_PAGENAME), gstFile.vcCont.at( cdPage ).atPageName, SUB_STRING );
				EndDialog( hDlg, IDOK );
				return (INT_PTR)TRUE;
			}

			if( IDCANCEL == LOWORD(wParam) )
			{
				EndDialog( hDlg, IDCANCEL );
				return (INT_PTR)TRUE;
			}

			break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ŗ��O�ύX�_�C�����O�Ƃ�
	@param[in]	dPage	�Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListNameChange( INT dPage )
{
	INT_PTR	iRslt;

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PAGE_NAME_DLG), ghPageWnd, PageNameDlgProc, dPage );
	if( IDOK == iRslt )
	{
		PageListNameSet( dPage, gstFile.vcCont.at( dPage ).atPageName );
		return S_OK;
	}

	return E_ABORT;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ŗ��O���Z�b�g
	@param[in]	dPage	�Ŕԍ�
	@param[in]	ptName	�Ŗ���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT PageListNameSet( INT dPage, LPTSTR ptName )
{
	INT		iPage;
	LVITEM	stLvi;

	iPage = ListView_GetItemCount( ghPageListWnd );
	if( iPage <= dPage )	return E_OUTOFMEMORY;

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask     = LVIF_TEXT;
	stLvi.iItem    = dPage;
	stLvi.pszText  = ptName;
	stLvi.iSubItem =  1;	//	���O
	ListView_SetItem( ghPageListWnd, &stLvi );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���O�̕t���Ă���ł����邩
	@return	INT	��O���O�t�����������@�O�Ȃ�����
*/
INT PageListIsNamed( FILES_ITR itFile )
{
	UINT_PTR	iPage, i;

	iPage = itFile->vcCont.size(  );
	for( i = 0; iPage > i; i++ )
	{
		if( 0 != itFile->vcCont.at( i ).atPageName[0] ){	return TRUE;	}
	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X�ł̓��e���A���̕ł�����ăR�s�[����
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	iNowPage	�t�H�[�J�X���Ă��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT PageListDuplicate( HWND hWnd, INT iNowPage )
{
	INT		iNewPage;
//	INT_PTR	iNext, iTotal;
	LINE_ITR	itLine;

	TRACE( TEXT("�ŕ���") );

	//iTotal = gstFile.vcCont.size(  );
	//iNext = iNowPage + 1;	//	���̕�
	//if( iTotal <= iNext ){	iNext =  -1;	}	//	�S�ł�葽���Ȃ疖�[�w��

	iNewPage = DocPageCreate( iNowPage );	//	�V��
	PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�

	//	��̈�s������Ă�̂ŁA�폜���Ă���
	gstFile.vcCont.at( iNewPage ).vcPage.clear(  );

	std::copy(	gstFile.vcCont.at( iNowPage ).vcPage.begin(),
				gstFile.vcCont.at( iNowPage ).vcPage.end(),
				back_inserter( gstFile.vcCont.at( iNewPage ).vcPage ) );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X�ł̎��̕łƓ�������
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	iNowPage	�t�H�[�J�X���Ă��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT PageListCombine( HWND hWnd, INT iNowPage )
{
	INT		iNext;
	INT_PTR	iTotal;
	ONELINE	stLine;
	LINE_ITR	itLine;

	ZeroONELINE( &stLine );

	TRACE( TEXT("�œ���") );

	iTotal = gstFile.vcCont.size(  );

	//	�Ő�������Ȃ��Ȃ�i�j�����Ȃ�
	if( 1 >= iTotal )	return E_ACCESSDENIED;


	iNext = iNowPage + 1;	//	���̕�
	if( iTotal <= iNext ){	return E_OUTOFMEMORY;	}	//	���[�łȂ牽�����Ȃ�

	//	��؂�Ƃ��ĉ��s�����
	gstFile.vcCont.at( iNowPage ).vcPage.push_back( stLine );

	//	���̕ł̑S�̂��R�s�[�����Ⴄ
	std::copy(	gstFile.vcCont.at( iNext ).vcPage.begin(),
				gstFile.vcCont.at( iNext ).vcPage.end(),
				back_inserter( gstFile.vcCont.at( iNowPage ).vcPage ) );

	SqnFreeAll( &(gstFile.vcCont.at( iNowPage ).stUndoLog) );	//	�A���h�D���O�폜

	DocPageDelete( iNext  );	//	���̕ł͍폜�����Ⴄ

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����X�g�r���[�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
*/
LRESULT CALLBACK gpfPageViewProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
//	INT	id;

	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_NOTIFY,    Plv_OnNotify  );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_MOUSEMOVE, Plv_OnMouseMove );	
		HANDLE_MSG( hWnd, WM_COMMAND,   Plt_OnCommand );	
	}

	return CallWindowProc( gpfOrigPageViewProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X���������Ƃ��̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			�N���C�A���g���W�w
	@param[in]	y			�N���C�A���g���W�x
	@param[in]	keyFlags	������Ă鑼�̃{�^��
	@return		�Ȃ�
*/
VOID Plv_OnMouseMove( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	LVHITTESTINFO	stHitInfo;
	INT	iItem;
	BOOLEAN	bReDraw = FALSE;

	//	���̂Ƃ��}�E�X�J�[�\�����ɂ���A�C�e����I�����Ă���

	ZeroMemory( &stHitInfo, sizeof(LVHITTESTINFO) );
	stHitInfo.pt.x = 1;	//	�������d�v�Ȃ̂ł����͓K���ł���
	stHitInfo.pt.y = y;

	iItem = ListView_HitTest( hWnd, &stHitInfo );
	if( gixMouseSel != iItem )	bReDraw = TRUE;
	gixMouseSel = iItem;

	if( bReDraw )	SendMessage( ghPageTipWnd, TTM_UPDATE, 0, 0 );

//	TRACE( TEXT("PLV MM %d,%d,%d"), iItem, stHitInfo.iItem, stHitInfo.iSubItem );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Plv_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	INT				dBytes;
	UINT_PTR		rdLength;
	LPNMTTDISPINFO	pstDispInfo;


	if( TTN_GETDISPINFO == pstNmhdr->code )	//	�c�[���`�b�v�̓��e�̖₢���킹��������
	{
		pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

		ZeroMemory( &(pstDispInfo->szText), sizeof(pstDispInfo->szText) );
		pstDispInfo->lpszText = NULL;

		FREE( gptPgTipBuf );

		if( !(gbPgTipView) ){	return 0;	}	//	��\���Ȃ牽�����Ȃ��ł���

		if( 0 > gixMouseSel ){	return 0;	}

		//	�Y���y�[�W�����������
		dBytes = DocAllTextGetAlloc( gixMouseSel, D_UNI, (LPVOID *)(&gptPgTipBuf), gitFileIt );

		StringCchLength( gptPgTipBuf, STRSAFE_MAX_CCH, &rdLength );
		if( 2 <= rdLength )
		{
			//	���[�ɗ]�v�ȉ��s������̂ŏ����Ă���
			gptPgTipBuf[rdLength-1] = NULL;
			gptPgTipBuf[rdLength-2] = NULL;
			rdLength -= 2;
		}

		pstDispInfo->lpszText = gptPgTipBuf;

		return 0;
	}

//	TRACE( TEXT("%u"), pstNmhdr->code );
	//	�����Ȃ������瑱����H
	return CallWindowProc( gpfOrigPageViewProc, hWnd, WM_NOTIFY, (WPARAM)idFrom, (LPARAM)pstNmhdr );

	//	�������[�v���ĂȂ����A���v��
}
//-------------------------------------------------------------------------------------------------


