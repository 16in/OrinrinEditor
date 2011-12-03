/*! @file
	@brief �h���t�g�{�[�h�̊Ǘ����܂�
	���̃t�@�C���� DraughtBoard.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/30
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

/*
Safari�̃��[�f�B���O���X�g�݂����ȋ@�\�B
�G�t�F�N�g�����̂Ƃ��Ƃ��A�C�ɂȂ�AA���������o�^���Ă����A�E�N���Ƃ��ŌĂяo����悤�ɂ��Ă����B
�I������烊�Z�b�g�Ƃ��A�폜�Ƃ��B
MAA�����荞�ށA�N���b�v�{�[�h�����荞�ށA�Ƃ��ł���Ƃ��������B��荞�݂̓h���b�O�h���b�v�ł��Ȃ��H
�g�p���郄�c�����������́H�N���b�v�{�[�h�A���C���{�b�N�X�Ƃ��H
MLT/AST�ɃG�N�X�|�[�g�A�݂����Ȃ��Ƃ��o����Ɨǂ��B�@�\���̂̓I���������ł����BUseItLater�I�Ȃ��́B
���X�g�̒��g�̊m�F�ǂ����邩�B�Ȃ�Ƃ����ăT���l�C���\�����HPNG�ɂ��ĕ\���Ƃ��H
�T���l��128x128���炢�ŁB5x3�ɕ��ׂ�Ƃ��B�X�N���[���o�[���l��
���j���[�Ƀ|�b�v�A�b�v�͂ł��Ȃ��̂�
Ctrl+Space�ŃT���l�C���E�C���h�E�o�����������B�I�����Ďg����BMAA�̂ق��ł��Ȃ񂩏o���Ȃ��H
�E�N���ɂ̓{�[�h�ďo�Ƃ��A�I��͈͂��{�[�h�ɒǉ��Ƃ��H
���́FDraughtBoard�@�h���t�g�{�[�h�Ƃ���������

�{�[�h����̎d�l�E�G�a�^�ɑ}���A�N���b�y�{�[�h�A���C���{�b�L�X�����肩

�L�^�`���ǂ����邩�BSQLite��vector�H�@�T���l�[���C���[�W���ǂ����邩�ɂ���ĕς�邩

MAA�̃T���l�\���ɂ��g���Ȃ�A�X�N���[���o�[�l���B�ďo�L�[�o�C���h���l����BCtrl+T�Ƃ��H
*/

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

#define DRAUGHT_BOARD_CLASS	TEXT("DRAUGHT_BOARD")

//	�ꖇ�̃p�l���T�C�Y�E���������H
#define THM_WIDTH	128
#define THM_HEIGHT	128

//	�p�l���͂T���R�ɕ��ׂ�
#define TPNL_HORIZ	5
#define TPNL_VERTI	3
//-------------------------------------------------------------------------------------------------

#ifdef DRAUGHT_STYLE

typedef struct tagDRAUGHTITEM
{
	LPSTR	pcAaText;	//!<	AA������ESjis�Ŋm��
	INT		iMaxDot;	//!<	�����ő�h�b�g��
	INT		iLines;		//!<	�g�p�s��

	SIZE	stSize;		//!<	�s�N�Z���T�C�Y
	HDC		hThumbDC;	//!<	�T���l�C���p�f�o�C�X�R���e�L�X�g
	HBITMAP	hThumbBmp;	//!<	�T���l�C���p�r�b�g�}�b�v�n���h��
	HBITMAP	hOldBmp;	//!<	���A�p

} DRAUGHTITEM, *LPDRAUGHTITEM;
//-------------------------------------------------------------------------------------------------

extern HFONT	ghAaFont;		//	AA�p�t�H���g

static  ATOM	gDraughtAtom;	//!<	�E�C���h�E�N���X�A�g��
static  HWND	ghDraughtWnd;	//!<	���̃E�C���h�E�n���h��
static  HWND	ghDraughtTip;	//!<	�c�[���`�b�v

EXTERNED UINT	gdClickMode;	//!<	�A�C�e�����N���b�N�����Ƃ��̊�{����E�O�ʏ�}���@�P��`�}���@�Q���C���{�b�N�X�J���@�RUNI�N���b�v�@�SSJIS�N���b�v
//�N���b�y�{�[�h�փR�s�郂�[�h�̓R�s�[���[�h�X���b�v�ɏ]��

static HDC		ghNonItemDC;	//!<	�A�C�e�������̊G
static HBITMAP	ghNonItemBMP, ghOldBmp;	
static HPEN		ghLinePen;

static INT		giTarget;		//!<	�N���b�N�����A�C�e���ԍ��E�|�P�Ŗ���

static vector<DRAUGHTITEM>	gvcDrtItems;	//!<	
typedef vector<DRAUGHTITEM>::iterator	DRTI_ITR;	//!<	
//-------------------------------------------------------------------------------------------------

INT		DraughtTargetItemSet( LPPOINT );	//!<	
DOUBLE	DraughtAspectKeeping( LPSIZE, UINT );	//!<	
INT		DraughtItemDelete( CONST INT  );	//!<	
HRESULT	DraughtItemUse( INT );	//!<	

LRESULT CALLBACK DraughtProc( HWND, UINT, WPARAM, LPARAM );
VOID	Drt_OnCommand( HWND , INT, HWND, UINT );	//!<	
VOID	Drt_OnPaint( HWND );						//!<	
//VOID	Drt_OnSize( HWND , UINT, INT, INT );		//!<	
LRESULT	Drt_OnNotify( HWND , INT, LPNMHDR );		//!<	
VOID	Drt_OnLButtonUp( HWND, INT, INT, UINT );	//!<	
VOID	Drt_OnContextMenu( HWND, HWND, UINT, UINT );//!<	
VOID	Drt_OnDestroy( HWND );						//!<	
VOID	Drt_OnKillFocus( HWND, HWND );				//!<	

//-------------------------------------------------------------------------------------------------

/*!
	�e���|������AA��\������E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hPtWnd		���C�����E�C���h�E�n���h��
	@return	���ɂȂ�
*/
HRESULT DraughtInitialise( HINSTANCE hInstance, HWND hPtWnd )
{
	WNDCLASSEX	wcex;


	if( hInstance )
	{
		ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= DraughtProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= DRAUGHT_BOARD_CLASS;
		wcex.hIconSm		= NULL;

		gDraughtAtom = RegisterClassEx( &wcex );

		ghNonItemDC = NULL;

		//	�N���b�N����w�胍�[�h�E�f�t�H����̓��j�R�N���b�v
		gdClickMode = InitParamValue( INIT_LOAD, VL_DRAUGHT_MODE, 3 );
		//	Viewer�̏ꍇ�̓R�s�[���[�h�ɏ]��
	}
	else
	{
		if( ghNonItemDC )
		{
			SelectBitmap( ghNonItemDC, ghOldBmp );
			SelectPen( ghNonItemDC, GetStockPen(NULL_PEN) );
			DeleteObject( ghNonItemDC );
		}
		if( ghNonItemBMP  ){	DeleteBitmap( ghNonItemBMP );	}
		if( ghLinePen ){	DeletePen( ghLinePen  );	}

		DraughtItemDelete( -1 );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�\���p�E�C���h�E�����
*/
HWND DraughtWindowCreate( HINSTANCE hInstance, HWND hPtWnd )
{
	INT_PTR	iItems;
	INT		iRslt;
	HDC		hdc;

	RECT	wdRect, rect;

	if( !(hPtWnd ) )	//	�j�󂷂�E����Ȃ��H
	{


		return NULL;
	}

	//	�߂ɏo���Ă����炻�̂܂ܕԂ�
	if( ghDraughtWnd  ){	UpdateWindow( ghDraughtWnd );	return ghDraughtWnd;	}

	iItems = gvcDrtItems.size( );	//	���݌�

	GetWindowRect( hPtWnd, &wdRect );
	rect.left   = wdRect.left + 32;	//	�I�t�Z�b�g�l�ɓ��ɈӖ��͂Ȃ�
	rect.top    = wdRect.top  + 32;
	rect.right  = THM_WIDTH  * TPNL_HORIZ;
	rect.bottom = THM_HEIGHT * TPNL_VERTI;
	
	//	�E�C���h�E�쐬
	ghDraughtWnd = CreateWindowEx( WS_EX_CLIENTEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		DRAUGHT_BOARD_CLASS, TEXT("Draught Board"), WS_POPUP | WS_VISIBLE,
		rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hInstance, NULL );

	if( !(ghNonItemDC) )
	{
		//	�`��p�r�b�g�}�b�v�쐬
		hdc = GetDC( ghDraughtWnd );

		ghNonItemDC  = CreateCompatibleDC( hdc );
		ghNonItemBMP = CreateCompatibleBitmap( hdc, THM_WIDTH, THM_HEIGHT );

		ghOldBmp = SelectBitmap( ghNonItemDC, ghNonItemBMP );
		SelectFont( ghNonItemDC, ghAaFont );

		iRslt = PatBlt( ghNonItemDC, 0, 0, THM_WIDTH, THM_HEIGHT, WHITENESS );

		ReleaseDC( ghDraughtWnd, hdc );

		SetRect( &rect, 0, 0, THM_WIDTH, THM_HEIGHT );
		iRslt = DrawText( ghNonItemDC, TEXT("NO ITEM"), 7, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_NOCLIP | DT_SINGLELINE );

		SelectFont( ghNonItemDC, GetStockFont(DEFAULT_GUI_FONT) );

		ghLinePen = CreatePen( PS_SOLID, 1, 0 );
		SelectPen( ghNonItemDC, ghLinePen );
	}

	UpdateWindow( ghDraughtWnd );

	return ghDraughtWnd;
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
LRESULT CALLBACK DraughtProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	//	HANDLE_MSG( hWnd, WM_SIZE,        Drt_OnSize );	
		HANDLE_MSG( hWnd, WM_COMMAND,     Drt_OnCommand );	
	//	HANDLE_MSG( hWnd, WM_NOTIFY,      Drt_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_LBUTTONUP,   Drt_OnLButtonUp );
		HANDLE_MSG( hWnd, WM_PAINT,       Drt_OnPaint );
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Drt_OnContextMenu );
		HANDLE_MSG( hWnd, WM_DESTROY,     Drt_OnDestroy );
		HANDLE_MSG( hWnd, WM_KILLFOCUS,   Drt_OnKillFocus );

//		case WM_MOUSEWHEEL:	SendMessage( , WM_MOUSEWHEEL, wParam, lParam );	return 0;

//		case WM_CLOSE:	ShowWindow( ghDraughtWnd, SW_HIDE );	return 0;

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
VOID Drt_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	
	switch( id )
	{
		case IDM_DRAUGHT_INSERTEDIT:
		case IDM_DRAUGHT_INTERRUPTEDIT:
		case IDM_DRAUGHT_LAYERBOX:
		case IDM_DRAUGHT_UNICLIP:
		case IDM_DRAUGHT_SJISCLIP:	DraughtItemUse( id );	DestroyWindow( hWnd );	break;

		case IDM_DRAUGHT_DELETE:	DraughtItemDelete( giTarget );	InvalidateRect( hWnd , NULL, TRUE );	break;

		case IDM_DRAUGHT_EXPORT:	NotifyBalloonExist( TEXT("�������Ȃ̂ł��E�E�E"), TEXT("��������"), NIIF_ERROR );	break;

		case IDM_DRAUGHT_ALLDELETE:	DraughtItemDelete( -1 );	DestroyWindow( hWnd );	break;

		case IDM_DRAUGHT_CLOSE:		DestroyWindow( hWnd );	break;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Drt_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;
	UINT		x = 0, y = 0;
	INT_PTR		iItems;
	SIZE		stSize;

	DRTI_ITR	itItem;


	hdc = BeginPaint( hWnd, &ps );

	SetStretchBltMode( hdc, HALFTONE );

	iItems = gvcDrtItems.size( );

	itItem = gvcDrtItems.begin();
	for( y = 0; TPNL_VERTI > y; y++ )
	{
		for( x = 0; TPNL_HORIZ > x; x++ )
		{
			if( itItem != gvcDrtItems.end() )
			{
				stSize = itItem->stSize;
				DraughtAspectKeeping( &stSize, TRUE );

				StretchBlt( hdc, (x * THM_WIDTH), (y * THM_HEIGHT), stSize.cx, stSize.cy,	//	�R�s�[��c�b�A����w�x�A���A����
					itItem->hThumbDC, 0, 0, itItem->stSize.cx, itItem->stSize.cy,	//	�R�s�[���c�b�A����w�x�A���A����
					SRCCOPY );	//	���X�^�I�y���[�V�����R�[�h
					//	((x+1) * THM_WIDTH)	((y+1) * THM_HEIGHT)
				itItem++;
			}
			else
			{
				BitBlt( hdc, (x * THM_WIDTH), (y * THM_HEIGHT), THM_WIDTH, THM_HEIGHT, ghNonItemDC, 0, 0, SRCCOPY );
			}
		}
	}

	for( y = 1; TPNL_HORIZ > y; y++ )
	{
		MoveToEx( hdc, (y * THM_HEIGHT), 0, NULL );
		LineTo( hdc, (y * THM_HEIGHT), (THM_HEIGHT * TPNL_VERTI) );
	}

	for( x = 1; TPNL_VERTI > x; x++ )
	{
		MoveToEx( hdc, 0, (x * THM_WIDTH), NULL );
		LineTo(   hdc, (THM_WIDTH * TPNL_HORIZ), (x * THM_WIDTH) );
	}

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X���������ꍇ
*/
VOID Drt_OnKillFocus( HWND hWnd, HWND hwndNewFocus )
{
	DestroyWindow( hWnd );

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
LRESULT Drt_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{


	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̍��{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Drt_OnLButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	POINT	stPos;
	INT		id;

	TRACE( TEXT("LUP %d x %d"), x, y );	//	�N���C�����g���W

	stPos.x = x;
	stPos.y = y;

	giTarget = DraughtTargetItemSet( &stPos );

	switch( gdClickMode )
	{
		case  0:	id = IDM_DRAUGHT_INSERTEDIT;	break;
		case  1:	id = IDM_DRAUGHT_INTERRUPTEDIT;	break;
		case  2:	id = IDM_DRAUGHT_LAYERBOX;	break;
		default:
		case  3:	id = IDM_DRAUGHT_UNICLIP;	break;
		case  4:	id = IDM_DRAUGHT_SJISCLIP;	break;
	}

	FORWARD_WM_COMMAND( hWnd, id, ghDraughtWnd, 0, SendMessage );

	return;
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
VOID Drt_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;

	POINT	stPoint, stPos;

	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	TRACE( TEXT("CTX %d x %d"), stPoint.x, stPoint.y );

	stPos = stPoint;
	ScreenToClient( hWnd, &stPos );
	giTarget = DraughtTargetItemSet( &stPos );

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_DRAUGHT_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	dRslt = TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
	//	�I�������łO���|�P�H�ATPM_RETURNCMD����������A�I�������炻�̃��j���[�̂h�c��WM_COMMAND�����s
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Drt_OnDestroy( HWND hWnd )
{

	ghDraughtWnd = 0;

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�̑I��͈͂��炢������
	@param[in]	bSqSel	��`�ł��邩�ǂ���
*/
UINT DraughtItemAddFromSelect( UINT bSqSel )
{
	LPTSTR	ptString = NULL;
	UINT	cchSize, cbSize;
	LPPOINT	pstPos = NULL;

	UINT_PTR	i, j, iTexts;
	LONG	dMin = 0;
	INT		insDot, yLine, iLines = 0, dOffset;
	LPTSTR	ptSpace = NULL;

	LPSTR	pcArts;
	wstring	wsString;


	//	���������Ȃ̂�Unicode�Œ�
	cbSize = DocSelectTextGetAlloc( D_UNI | bSqSel, (LPVOID *)(&ptString), (bSqSel & D_SQUARE) ? &pstPos : NULL );

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	if( 0 >= cchSize )	return 0;	//	������Ȃ��Ȃ�Ȃɂ����Ȃ�

	//	�I�t�Z�b�g�ݒ肪�L��ꍇ�A���̕��𖄂߂�󔒂��K�v
	if( pstPos )	//	�ŏ��I�t�Z�b�g�l��T���āA���������[�ɂ���
	{
		dMin = pstPos[0].x;

		yLine = 0;
		for( i = 0; cchSize > i; i++ )
		{
			if( CC_CR == ptString[i] && CC_LF == ptString[i+1] )	//	���s�ł�������
			{
				//	�I�t�Z�b�g�ŏ���������
				if( dMin > pstPos[yLine].x ){	dMin =  pstPos[yLine].x;	}

				i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
				yLine++;	//	���s��������Focus�͎��̍s��
			}
		}
		//	���̎��_�ŁAyLine�͍s���ɂȂ��Ă�
		iLines = yLine;

		//	��s�ڂ̋󔒂�����đM�����͂��Ă���
		insDot = 0;
		dOffset = pstPos[0].x - dMin;
		ptSpace = DocPaddingSpaceUni( dOffset, NULL, NULL, NULL );
		//	�O���󔒂͖��������̂Ń��j�R�[�h�g���Ė�薳��
		StringCchLength( ptSpace, STRSAFE_MAX_CCH, &iTexts );
		for( j = 0; iTexts > j; j++ ){	wsString +=  ptSpace[j];	}
		FREE(ptSpace);
	}

	yLine = 0;	insDot = 0;
	for( i = 0; cchSize > i; i++ )
	{
		if( CC_CR == ptString[i] && CC_LF == ptString[i+1] )	//	���s�ł�������
		{
			wsString +=  wstring( TEXT("\r\n") );

			i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
			yLine++;	//	���s��������Focus�͎��̍s��

			//	�I�t�Z�b�g���̋󔒂����
			if( pstPos && (iLines > yLine) )
			{
				dOffset = pstPos[yLine].x - dMin;
				ptSpace = DocPaddingSpaceUni( dOffset, NULL, NULL, NULL );
				//	�O���󔒂͖��������̂Ń��j�R�[�h�g���Ė�薳��
				StringCchLength( ptSpace, STRSAFE_MAX_CCH, &iTexts );
				for( j = 0; iTexts > j; j++ ){	wsString +=  ptSpace[j];	}
				FREE(ptSpace);
			}
		}
		else if( CC_TAB ==  ptString[i] ){	/*	�^�u�͑}�����Ȃ�	*/	}
		else{	wsString += ptString[i];	}
	}

	FREE(ptString);
	FREE(pstPos);

	pcArts =  SjisEncodeAlloc( wsString.c_str() );

	DraughtItemAdding( pcArts );

	FREE(pcArts);

	return yLine;
}
//-------------------------------------------------------------------------------------------------

/*!
	AA�e�L�X�g���m�ۂ��Ď�荞��
	@param[in]	pcArts	�`�`�e�L�X�g
	@return		�ǉ���̃A�C�e������
*/
UINT DraughtItemAdding( LPSTR pcArts )
{
	UINT_PTR	cbSize, cchSize;
	LPTSTR		ptTextaa;
	DRAUGHTITEM	stItem;

	RECT	rect;
	INT_PTR	iItems;
	INT		iRslt, iYdot, iXdot, iLine;
	HDC		hdc;

	StringCchLengthA( pcArts, STRSAFE_MAX_CCH, &cbSize );

	stItem.pcAaText = (LPSTR)malloc( (cbSize + 1) );
	ZeroMemory( stItem.pcAaText, (cbSize + 1) );
	StringCchCopyA( stItem.pcAaText, (cbSize + 1), pcArts );

	ptTextaa = SjisDecodeAlloc( pcArts );
	StringCchLength( ptTextaa, STRSAFE_MAX_CCH, &cchSize );

	//	���`�`�̃T�C�Y�m�肪�K�v
	iXdot = TextViewSizeGet( ptTextaa, &iLine );
	iYdot = iLine * LINE_HEIGHT;

	stItem.iMaxDot = iXdot;
	stItem.iLines  = iLine;

	if( THM_WIDTH >  iXdot )	iXdot = THM_WIDTH;
	if( THM_HEIGHT > iYdot )	iYdot = THM_HEIGHT;

	stItem.stSize.cx = iXdot;
	stItem.stSize.cy = iYdot;

	SetRect( &rect, 0, 0, iXdot, iYdot );
	//	����܂�傫���Ȃ�A�������Ƃ��H

	//	�`��p�r�b�g�}�b�v�쐬
	hdc = GetDC( ghDraughtWnd );

	//	�T���l�pBMP�E����̓t���T�C�Y
	stItem.hThumbDC  = CreateCompatibleDC( hdc );
	stItem.hThumbBmp = CreateCompatibleBitmap( hdc, rect.right, rect.bottom );
	stItem.hOldBmp   = SelectBitmap( stItem.hThumbDC, stItem.hThumbBmp );
	SelectFont( stItem.hThumbDC, ghAaFont );

	ReleaseDC( ghDraughtWnd, hdc );

	iRslt = PatBlt( stItem.hThumbDC, 0, 0, rect.right, rect.bottom, WHITENESS );

	iRslt = DrawText( stItem.hThumbDC, ptTextaa, cchSize, &rect, DT_LEFT | DT_NOPREFIX | DT_NOCLIP | DT_WORDBREAK );

	SelectFont( stItem.hThumbDC, GetStockFont(DEFAULT_GUI_FONT) );

	gvcDrtItems.push_back( stItem );

	do	//	�݂͂����Ă���H
	{
		iItems = gvcDrtItems.size( );
		if( (TPNL_HORIZ * TPNL_VERTI) < iItems ){	DraughtItemDelete(  0 );	}

	}while( (TPNL_HORIZ * TPNL_VERTI) < iItems );

	FREE( ptTextaa );

	return iItems;
}
//-------------------------------------------------------------------------------------------------

/*!
	Target�A�C�e�����g���E�N���b�v�{�[�h�ցE���Ɏg�������Ƃ��́H
*/
HRESULT DraughtItemUse( INT id )
{
	INT_PTR		iItems, i;
	UINT_PTR	cbSize;
	UINT		dMode;
	DRTI_ITR	itItem;

	iItems = gvcDrtItems.size( );	//	���݌�
	if( 0 >= iItems )	return E_OUTOFMEMORY;

	for( i = 0, itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; i++, itItem++ )
	{
		if( giTarget == i )	//	�q�b�g
		{
			switch( id )
			{
				case IDM_DRAUGHT_INSERTEDIT:	dMode = MAA_INSERT;		break;
				case IDM_DRAUGHT_INTERRUPTEDIT:	dMode = MAA_INTERRUPT;	break;
				case IDM_DRAUGHT_LAYERBOX:		dMode = MAA_LAYERED;	break;
				default:
				case IDM_DRAUGHT_UNICLIP:		dMode = MAA_UNICLIP;	break;
				case IDM_DRAUGHT_SJISCLIP:		dMode = MAA_SJISCLIP;	break;
			}
			StringCchLengthA( itItem->pcAaText, STRSAFE_MAX_CCH, &cbSize );

			ViewMaaMaterialise( itItem->pcAaText, cbSize, dMode );
		}
	}

	return E_INVALIDARG;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ΏۃA�C�e���폜�EREDRAW�̖ʓ|�͕ʂŌ���
	@param[in]	iTarget	�Ώۂ̒ʂ��ԍ��E�}�C�i�X�Ȃ�S�폜
	@return	��O�폜�����@�O�폜�ł��񂩂���
*/
INT DraughtItemDelete( CONST INT iTarget )
{
	INT_PTR	iItems, i;
	INT		delCnt;
	DRTI_ITR	itItem;

	iItems = gvcDrtItems.size( );	//	���݌�
	if( 0 >= iItems )	return 0;

	delCnt = 0;
	for( i = 0, itItem = gvcDrtItems.begin(); gvcDrtItems.end() != itItem; i++, itItem++ )
	{
		if( iTarget == i || 0 > iTarget )
		{
			FREE( itItem->pcAaText );
			SelectBitmap( itItem->hThumbDC, itItem->hOldBmp );
			DeleteBitmap( itItem->hThumbBmp );
			DeleteDC( itItem->hThumbDC );
			delCnt++;

			if( iTarget == i )
			{
				gvcDrtItems.erase( itItem );
				return 1;
			}
		}
	}

	if( 0 > iTarget ){	gvcDrtItems.clear( );	}

	return delCnt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�N���C�����g���W���󂯎���āA�^�[�Q�b�g�A�C�e���ʂ��ԍ����o��
	@param[in]	ptPos	�N���C�����g���W���������\���̃|�C���^�[
	@return		���ォ��A���ɒʂ��ԍ��O�C���f�b�N�X�@�����Ł|�P
*/
INT DraughtTargetItemSet( LPPOINT pstPos )
{
	INT	ix, iy, number;

	ix = pstPos->x / THM_WIDTH;
	iy = pstPos->y / THM_HEIGHT;

	if( 0 > ix || TPNL_HORIZ <= ix || 0 > iy || TPNL_VERTI <= iy )	return -1;

	number = ix + iy * TPNL_HORIZ;

	TRACE( TEXT("TARGET %d"), number );

	return number;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c�����󂯎���āA�A�X�y�N�g����ێ����ăT���l�T�C�Y�ɏk������
	@param[in,out]	pstSize	�󂯎���āA�߂�
	@param[in]		bOrgRem	��O�T���l�T�C�Y��菬�����Ȃ�I���W�i���T�C�Y
	@return	DOUBLE	ZOOM��
*/
DOUBLE DraughtAspectKeeping( LPSIZE pstSize, UINT bOrgRem )
{
	INT	iOrgWid, iOrgHei;
	INT	iZomWid, iZomHei;
	DOUBLE	ddPercent, ddBuff;

	iOrgWid = pstSize->cx;
	iOrgHei = pstSize->cy;
	ddPercent = 1.0;

	if( iOrgWid >= iOrgHei )	//	�����`������
	{
		iZomWid = THM_WIDTH;

		if( THM_WIDTH == iOrgWid )	//	�s�^���Ȃ牽�����邱�ƂȂ�
		{
			iZomHei = iOrgHei;
		}
		else
		{
			ddPercent = (DOUBLE)THM_WIDTH / (DOUBLE)iOrgWid;
			if( bOrgRem && THM_WIDTH > iOrgWid )	//	�T�C�Y�ȉ��Ȃ�I���W�i���T�C�Y�ł�낵
			{
				iZomWid = iOrgWid;
				iZomHei = iOrgHei;
			}
			else
			{
				ddBuff  = (DOUBLE)iOrgHei * ddPercent;
				ddBuff += 0.5;	//	�l�̌ܓ�
				iZomHei = ddBuff;
			}
		}
	}
	else	//	�c��
	{
		iZomHei = THM_HEIGHT;

		if( THM_HEIGHT == iOrgHei )	//	�s�^���Ȃ牽�����邱�ƂȂ�
		{
			iZomWid = iOrgWid;
		}
		else
		{
			ddPercent = (DOUBLE)THM_HEIGHT / (DOUBLE)iOrgHei;
			if( bOrgRem && THM_HEIGHT > iOrgHei )	//	�T�C�Y�ȉ��Ȃ�I���W�i���T�C�Y�ł�낵
			{
				iZomWid = iOrgWid;
				iZomHei = iOrgHei;
			}
			else
			{
				ddBuff  = (DOUBLE)iOrgWid * ddPercent;
				ddBuff += 0.5;	//	�l�̌ܓ�
				iZomWid = ddBuff;
			}
		}
	}

	pstSize->cx = iZomWid;
	pstSize->cy = iZomHei;

	return ddPercent;
}
//-------------------------------------------------------------------------------------------------

#endif
