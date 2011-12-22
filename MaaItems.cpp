/*! @file
	@brief �`�`�ꗗ�̐�������܂�
	���̃t�@�C���� MaaItems.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/22
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
#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------

typedef struct tagVIEWORDER
{
	UINT	index;
	UINT	dHeight;
	UINT	dUpper;
	UINT	dDownr;

} VIEWORDER, *LPVIEWORDER;


typedef struct tagAATITLE
{
	UINT	number;
	TCHAR	atTitle[MAX_STRING];

} AATITLE, *LPAATITLE;
//-------------------------------------------------------------------------------------------------

#define TITLECBX_HEI	200

#define SBP_DIRECT		0xFF

static  HWND	ghItemsWnd;			//!<	���X�g�̃n���h��
static  HWND	ghToolTipWnd;		//!<	�c�[���`�b�v

static  HWND	ghComboxWnd;		//!<	���o���p�R���{�b�N�X

static WNDPROC	gpfOrgAaItemsProc;	//!<	
static WNDPROC	gpfOrgAaTitleCbxProc;	//!<	

static LPTSTR	gptTipBuffer;		//!<	

static INT		gixTopItem;			//!<	�ꗗ�̍ŏ��
static INT		gixMaxItem;			//!<	�A�C�e����

static  HWND	ghScrollWnd;		//!<	�X�N���[���o�[

static  LONG	gixNowSel;			//!<	�}�E�X�J�[�\��������Ƃ���̃C���f�b�N�X

#ifdef _ORRVW
EXTERNED HFONT	ghAaFont;			//!<	�\���p�̃t�H���g
#else
static HFONT	ghAaFont;			//!<	�\���p�̃t�H���g
#endif
EXTERNED HFONT	ghTipFont;			//!<	�c�[���`�b�v�p

static  HPEN	ghSepPen;			//!<	��؂���p�y��
static BOOLEAN	gbLineSep;			//!<	AA�̕����͐��ɂ���

extern  UINT	gbAAtipView;		//!<	��O�ŁA�`�`�c�[���`�b�v�\��

extern  HWND	ghSplitaWnd;		//!<	�X�v���b�g�o�[�n���h��

static vector<VIEWORDER>	gvcViewOrder;	//!<	
static vector<AATITLE>		gvcAaTitle;		//!<	
//-------------------------------------------------------------------------------------------------

LRESULT	Aai_OnNotify( HWND , INT, LPNMHDR );	//!<	
VOID	Aai_OnMouseMove( HWND, INT, INT, UINT );	//!<	
VOID	Aai_OnLButtonDown( HWND, BOOL, INT, INT, UINT );	//!<	
VOID	Aai_OnContextMenu( HWND, HWND, UINT, UINT );	//!<	
VOID	Aai_OnDropFiles( HWND , HDROP );	//!<	


HRESULT	AaItemsFavUpload( LPSTR, UINT );	//!<	
HRESULT	AaItemsFavDelete( LPSTR, UINT );	//!<	
UINT	AaItemsDoSelect( HWND, UINT );		//!<	

LRESULT	CALLBACK gpfAaItemsProc( HWND, UINT, WPARAM, LPARAM );	//	
LRESULT	CALLBACK gpfAaTitleCbxProc( HWND, UINT, WPARAM, LPARAM );	//	

#ifndef _ORRVW
INT_PTR	CALLBACK AaItemAddDlgProc( HWND, UINT, WPARAM, LPARAM );
#endif
//-------------------------------------------------------------------------------------------------

/*!
	�S�c���[�₨�C�Ƀ��X�g�̓��e��\������X�^�e�B�b�N�Ƃ����
	@param[in]	hWnd	�e�E�C���h�E�n���h���ENULL�Ȃ�j��
	@param[in]	hInst	�A�v���̎���
	@param[in]	ptRect	�N���C�A���g�̈�̃T�C�Y
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AaItemsInitialise( HWND hWnd, HINSTANCE hInst, LPRECT ptRect )
{
	TTTOOLINFO	stToolInfo;
	SCROLLINFO	stScrollInfo;
	INT		ttSize;
	RECT	rect;

	if( !(hWnd) )
	{
		free( gptTipBuffer );
		SetWindowFont( ghItemsWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );
		SetWindowFont( ghToolTipWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );
		DeleteFont( ghAaFont );
		DeleteFont( ghTipFont );
		DeletePen( ghSepPen );
		return S_FALSE;
	}

//�E�C���h�E�̃T�C�Y�́A���ƂŕύX�����ł���̂ŁA�����ł͌Œ�l�ō���Ă���

	ghSepPen  = CreatePen( PS_SOLID, 1, RGB(0xAA,0xAA,0xAA) );

	gbLineSep = InitParamValue( INIT_LOAD, VL_MAASEP_STYLE, 0 );


	gptTipBuffer = NULL;

	gixTopItem = 0;

	//	�c�[���`�b�v���
	ghToolTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hWnd, NULL, hInst, NULL );

	//	���o���R���{�b�N�X
	ghComboxWnd = CreateWindowEx( 0, WC_COMBOBOX, TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_NOINTEGRALHEIGHT, TREE_WIDTH + SPLITBAR_WIDTH, 0, ptRect->right - TREE_WIDTH - LSSCL_WIDTH, TITLECBX_HEI, hWnd, (HMENU)IDCB_AAITEMTITLE, hInst, NULL );
	GetClientRect( ghComboxWnd, &rect );

	//	�T�u�N���X��
	gpfOrgAaTitleCbxProc = SubclassWindow( ghComboxWnd, gpfAaTitleCbxProc );

	//	AA�ꗗ�̃X�^�e�B�b�N����E�I�[�i�[�h���[�ŕ`��
	ghItemsWnd = CreateWindowEx( WS_EX_CLIENTEDGE, WC_STATIC, TEXT(""), WS_VISIBLE | WS_CHILD | SS_OWNERDRAW | SS_NOTIFY, TREE_WIDTH + SPLITBAR_WIDTH, rect.bottom, ptRect->right - TREE_WIDTH - LSSCL_WIDTH, ptRect->bottom - rect.bottom, hWnd, (HMENU)IDSO_AAITEMS, hInst, NULL );

	DragAcceptFiles( ghItemsWnd, TRUE );

	//	�T�u�N���X��
	gpfOrgAaItemsProc = SubclassWindow( ghItemsWnd, gpfAaItemsProc );

	//	�ꗗ�̃X�N���[���o�[
	ghScrollWnd = CreateWindowEx( 0, WC_SCROLLBAR, TEXT("scroll"), WS_VISIBLE | WS_CHILD | SBS_VERT, ptRect->right - LSSCL_WIDTH, rect.bottom, LSSCL_WIDTH, ptRect->bottom - rect.bottom, hWnd, (HMENU)IDSB_LISTSCROLL, hInst, NULL );

	ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
	stScrollInfo.cbSize = sizeof(SCROLLINFO);
	stScrollInfo.fMask = SIF_DISABLENOSCROLL;
	SetScrollInfo( ghScrollWnd, SB_CTL, &stScrollInfo, TRUE );

	ghAaFont = CreateFont( FONTSZ_NORMAL, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("�l�r �o�S�V�b�N") );
	//	�t�H���g�̍���	���ϕ�	��������̕�����X���Ƃ̊p�x	�x�[�X���C����X���Ƃ̊p�x	�����̑���(0~1000�܂ŁE400=nomal)	�C�^���b�N��	�A���_�[���C��	�X�g���C�N�A�E�g	�����Z�b�g	�o�͐��x	�N���b�s���O���x	�o�͕i��	�Œ蕝���ϕ�	�t�H���g��
	SetWindowFont( ghItemsWnd, ghAaFont, TRUE );

	//	�������Ȃ��悤�ɁA9pt�������p�ӂ��Ă݂�
	ttSize = InitParamValue( INIT_LOAD, VL_MAATIP_SIZE, 16 );	//	�T�C�Y�m�F
	ghTipFont = CreateFont( (FONTSZ_REDUCE == ttSize) ? FONTSZ_REDUCE : FONTSZ_NORMAL, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("�l�r �o�S�V�b�N") );
	SetWindowFont( ghToolTipWnd, ghTipFont, TRUE );

	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghItemsWnd;
	stToolInfo.uId      = IDSO_AAITEMS;
	GetClientRect( ghItemsWnd, &stToolInfo.rect );
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghToolTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghToolTipWnd, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�



	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o���R���{�b�N�X�̃T�u�N���X�v���V�[�W��
	@param[in]	hWnd	���X�g�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�������ʂƂ�
*/
LRESULT	CALLBACK gpfAaTitleCbxProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_CHAR, Maa_OnChar );
		default:	break;
	}

	return CallWindowProc( gpfOrgAaTitleCbxProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	���o���R���{�b�N�X����ɂ���
*/
VOID AaTitleClear( VOID )
{
	while( ComboBox_GetCount( ghComboxWnd ) ){	ComboBox_DeleteString( ghComboxWnd, 0 );	}
	gvcAaTitle.clear( );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o���R���{�b�N�X�ɓ��e�ǉ�
	@param[in]	number	�ʂ��ԍ�
	@param[in]	ptTitle	���ꍞ�ޕ�����
	@return	�p�����������Ƃ̍��ڐ�
*/
INT AaTitleAddString( UINT number, LPSTR pcTitle )
{
	AATITLE	stTitle;
	LPTSTR	ptTitle;

	ptTitle = SjisDecodeAlloc( pcTitle );

	ComboBox_AddString( ghComboxWnd, ptTitle );

	ZeroMemory( &stTitle, sizeof(AATITLE) );
	stTitle.number = number;
	StringCchCopy( stTitle.atTitle, MAX_STRING, ptTitle );

	gvcAaTitle.push_back( stTitle );

	FREE(ptTitle);

	return ComboBox_GetCount( ghComboxWnd );
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���{�b�N�X�R�}���h����
	@param[in]	hWnd		�e�E�C���h�E�n���h��
	@param[in]	codeNotify	�ʒm���b�Z�[�W
*/
VOID AaTitleSelect( HWND hWnd, UINT codeNotify )
{
	INT	iSel;
	INT_PTR	iItems;

//	TRACE( TEXT("COMBOX[%u]"), codeNotify );

	if( CBN_SELCHANGE == codeNotify )	//	�I�����ύX���ꂽ
	{
		iSel = ComboBox_GetCurSel( ghComboxWnd );	//	�I�΂�Ă�̎����

		iItems = gvcAaTitle.size( );	//	�͂ݏo���m�F
		if( iItems <= iSel )	return;

		//	���o���̊Y������ʒu�փW�����v
		Aai_OnVScroll( hWnd, ghScrollWnd, SBP_DIRECT, gvcAaTitle.at( iSel ).number );
	}

	if( CBN_CLOSEUP == codeNotify )
	{
		SetFocus( ghItemsWnd );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	��ʃT�C�Y���ς�����̂ŃT�C�Y�ύX
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@param[in]	ptRect	�N���C�A���g�̈�
*/
VOID AaItemsResize( HWND hWnd, LPRECT ptRect )
{
	INT		dWidth, dLeft;
	RECT	sptRect, rect;
	TTTOOLINFO	stToolInfo;

	SplitBarPosGet( ghSplitaWnd, &sptRect );
	//	�g���^�u�o�[�̈ʒu�m��
//	MaaTabBarSizeGet( &tbRect );

	dWidth = ptRect->right - (sptRect.left + SPLITBAR_WIDTH) - LSSCL_WIDTH;
	dLeft  = ptRect->right - LSSCL_WIDTH;

	MoveWindow( ghComboxWnd, sptRect.left + SPLITBAR_WIDTH, ptRect->top, dWidth, TITLECBX_HEI, TRUE );
	GetClientRect( ghComboxWnd, &rect );

	MoveWindow( ghItemsWnd,  sptRect.left + SPLITBAR_WIDTH, ptRect->top + rect.bottom, dWidth, ptRect->bottom - rect.bottom, TRUE );
	MoveWindow( ghScrollWnd, dLeft, ptRect->top + rect.bottom, LSSCL_WIDTH, ptRect->bottom - rect.bottom, TRUE );

	//	�K�v�ȏ����������΂���
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize = sizeof(TTTOOLINFO);
	stToolInfo.hwnd   = ghItemsWnd;
	stToolInfo.uId    = IDSO_AAITEMS;
	GetClientRect( ghItemsWnd, &stToolInfo.rect );
	SendMessage( ghToolTipWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&stToolInfo );

	InvalidateRect( ghItemsWnd, NULL, TRUE );
	UpdateWindow( ghItemsWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	AA�\���X�^�e�B�b�N�̃T�u�N���X�v���V�[�W���E�c�[���`�b�v�̏����ɕK�v
	@param[in]	hWnd	���X�g�̃n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return		LRESULT	�������ʂƂ�
*/
LRESULT CALLBACK gpfAaItemsProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_CHAR,        Maa_OnChar );

		HANDLE_MSG( hWnd, WM_NOTIFY,      Aai_OnNotify );		//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_MOUSEMOVE,   Aai_OnMouseMove );	//	�}�E�X��������
		HANDLE_MSG( hWnd, WM_LBUTTONDOWN, Aai_OnLButtonDown );	//	�}�E�X���{�^�������ꂽ
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Aai_OnContextMenu );	//	�R���e�L�X�g���j���[����
		HANDLE_MSG( hWnd, WM_DROPFILES,   Aai_OnDropFiles );	//	�h���O���h���b�v�̎�t

		default:	break;
	}

	return CallWindowProc( gpfOrgAaItemsProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	AA�\���X�^�e�B�b�N�̃I�[�i�[�h���[�E�`��
	@param[in]	hWnd			�e�E�C���h�E�n���h��
	@param[in]	*pstDrawItem	�h���[���
	@return		�Ȃ�
*/
VOID AaItemsDrawItem( HWND hWnd, CONST DRAWITEMSTRUCT *pstDrawItem )
{
	UINT_PTR	rdLen;
	INT_PTR	rdLength;
	LPSTR	pcConts = NULL;
	LPTSTR	ptConStr = NULL;
	RECT	rect, drawRect;
	POINT	stPoint;

	VIEWORDER	stVwrder;

	INT		rdNextItem;
	LONG	rdDrawPxTop, rdBottom;	//	�`��̍ŏ�ʁA�`��̈�̍����l�`�w
	LONG	rdHeight, rdWidth;	//	�A�C�e���̍����E�`��̈�̕�

	HPEN	hOldPen;

	//	���ڂ̊O�ڎl�p�`�̎擾
	rect = pstDrawItem->rcItem;
	rdBottom = rect.bottom;
	rdWidth  = rect.right;
	rdDrawPxTop = 0;

	SetBkMode( pstDrawItem->hDC, TRANSPARENT );	//	�����`��͔w�i���߂Ŗ�I����

	FillRect( pstDrawItem->hDC, &rect, GetStockBrush(WHITE_BRUSH) );

	gvcViewOrder.clear();

	rdNextItem = gixTopItem;
	for( rdDrawPxTop = 0; rdBottom > rdDrawPxTop; rdNextItem++ )
	{
		pcConts = AacAsciiArtGet( rdNextItem );	//	�ꗗ�̃g�b�v���m�F
		if( !pcConts ){	break;	}

		stVwrder.index = rdNextItem;

		ptConStr = SjisDecodeAlloc( pcConts );
		StringCchLength( ptConStr, STRSAFE_MAX_CCH, &rdLen );
		rdLength = rdLen;

		free( pcConts );

		//	������ɍ��킹��RECT�m��
		DrawText( pstDrawItem->hDC, ptConStr, rdLength, &rect, DT_LEFT | DT_EDITCONTROL | DT_NOPREFIX | DT_CALCRECT );
		drawRect = rect;
		rdHeight = drawRect.bottom;
		drawRect.bottom += rdDrawPxTop;
		drawRect.top     = rdDrawPxTop;
		if( drawRect.right < rdWidth )	drawRect.right = rdWidth;

		stVwrder.dHeight = rdHeight;
		stVwrder.dUpper  = drawRect.top;
		stVwrder.dDownr  = drawRect.bottom;

		//	���ڂ��ƂɌ݂��Ⴂ�̐F�ɂ��邩�A��؂��������
		if( gbLineSep ){	FillRect( pstDrawItem->hDC, &drawRect, GetStockBrush(WHITE_BRUSH) );	}
		else
		{
			if( 1 & rdNextItem )	FillRect( pstDrawItem->hDC, &drawRect, GetStockBrush(LTGRAY_BRUSH) );
			else					FillRect( pstDrawItem->hDC, &drawRect, GetStockBrush(WHITE_BRUSH) );
		}

		DrawText( pstDrawItem->hDC, ptConStr, rdLength, &drawRect, DT_LEFT | DT_EDITCONTROL | DT_NOPREFIX );

		if( gbLineSep )
		{
			hOldPen = SelectPen( pstDrawItem->hDC, ghSepPen );
			MoveToEx( pstDrawItem->hDC, drawRect.left, drawRect.bottom-1, NULL );
			LineTo( pstDrawItem->hDC, drawRect.right, drawRect.bottom-1 );
			SelectPen( pstDrawItem->hDC, hOldPen );
		}

		gvcViewOrder.push_back( stVwrder );

		rdDrawPxTop += rdHeight;

		free( ptConStr );
	}
	InvalidateRect( ghScrollWnd, NULL, TRUE );
	UpdateWindow( ghScrollWnd );

	//	�J�[�\���ʒu�m�ۂ�����
	GetCursorPos( &stPoint );
	ScreenToClient( ghItemsWnd, &stPoint );
	Aai_OnMouseMove( hWnd, stPoint.x, stPoint.y, 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

#if 0

/*!
	�I�[�i�[�h���[�E�T�C�Y�v��
	@param[in]	hWnd			�e�E�C���h�E�n���h��
	@param[in]	pstMeasureItem	�T�C�Y���
	@return		HRESULT			�I����ԃR�[�h
*/
VOID AaItemsMeasureItem( HWND hWnd, LPMEASUREITEMSTRUCT pstMeasureItem )
{
	HDC		hDC;
	INT		rdLength, rdHeight;
	LPSTR	pcConts;
	RECT	stRect;

	//	���ڂ̕�����擾
	pcConts = AacAsciiArtGet( pstMeasureItem->itemID );
	if( !pcConts )	return;

	rdLength = strlen( pcConts );

	ListBox_GetItemRect( ghItemsWnd, pstMeasureItem->itemID, &stRect );

	hDC = GetDC( ghItemsWnd );
	DrawTextExA( hDC, pcConts, rdLength, &stRect, DT_LEFT | DT_EDITCONTROL | DT_NOPREFIX | DT_CALCRECT );
	ReleaseDC( ghItemsWnd, hDC );

	pstMeasureItem->itemHeight = (stRect.bottom - stRect.top);
	if( 256 <= pstMeasureItem->itemHeight )	pstMeasureItem->itemHeight = 255;

	free( pcConts );

	return;
}
//-------------------------------------------------------------------------------------------------
#endif


/*!
	�}�E�X���������Ƃ��̏���
	@param[in]	hWnd		�e�E�C���h�E�n���h��
	@param[in]	x			�N���C�A���g���W�w
	@param[in]	y			�N���C�A���g���W�x
	@param[in]	keyFlags	������Ă鑼�̃{�^��
	@return		�Ȃ�
*/
VOID Aai_OnMouseMove( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	TCHAR		atBuffer[MAX_STRING];
	UINT_PTR	i, max;
	LONG		iItem = -1, bottom;
	BOOLEAN		bReDraw = FALSE;

	//	���̂Ƃ��}�E�X�J�[�\�����ɂ���A�C�e����I�����Ă���

	if( !( gvcViewOrder.empty() ) )
	{
		max = gvcViewOrder.size();

		bottom = 0;
		for( i = 0; max > i; i++ )
		{
			bottom += gvcViewOrder.at( i ).dHeight;

			if( y < bottom ){	iItem = gvcViewOrder.at( i  ).index;	break;	}
		}
	}

	if( gixNowSel != iItem )	bReDraw = TRUE;
	gixNowSel = iItem;

	if( bReDraw && gbAAtipView )	SendMessage( ghToolTipWnd, TTM_UPDATE, 0, 0 );

	//	�f�o�b�O�p�E���Ƃŏ��������e�ύX
	StringCchPrintf( atBuffer, MAX_STRING, TEXT("(%d x %d) %d"), x, y, iItem );

	StatusBarMsgSet( 1, atBuffer );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̍��{�^�����_�E��(�_�u���N���b�R�j�����Ƃ��̏���
	@param[in]	hWnd			�e�E�C���h�E�n���h��
	@param[in]	fDoubleClick	�_�u���N���b�R���ꂽ�Ƃ��Ȃ��O�ƂȂ�
	@param[in]	x				�N���C�A���g���W�w
	@param[in]	y				�N���C�A���g���W�x
	@param[in]	keyFlags		������Ă鑼�̃{�^��
	@return		�Ȃ�
*/
VOID Aai_OnLButtonDown( HWND hWnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags )
{
	AaItemsDoSelect( hWnd, MAA_DEFAULT );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�N���[���̏���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	hwndCtl	�X�N���[���o�[�̃E�C���h�E�n���h��
	@param[in]	code	�����ԃR�[�h�E0xFF�����R�[�h�Ƃ��Ďg��
	@param[in]	pos		�܂݂̈ʒu
	@return		�����������e�Ƃ�
*/
VOID Aai_OnVScroll( HWND hWnd, HWND hwndCtl, UINT code, INT pos )
{
	INT	maePos;
	SCROLLINFO	stScrollInfo;

	if( ghScrollWnd != hwndCtl )	return;

	//	��Ԃ����₵��
	ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
	stScrollInfo.cbSize = sizeof(SCROLLINFO);
	stScrollInfo.fMask = SIF_ALL;
	GetScrollInfo( ghScrollWnd, SB_CTL, &stScrollInfo );

	maePos = gixTopItem;

	switch( code )	//	�X�N���[�������ɍ��킹�ē��e�����炷
	{
		case SB_TOP:
			gixTopItem = 0;
			break;

		case SB_LINEUP:
		case SB_PAGEUP:
			gixTopItem--;
			if( 0 > gixTopItem )	gixTopItem = 0;
			break;

		case SB_LINEDOWN:
		case SB_PAGEDOWN:
			gixTopItem++;
			if( gixMaxItem <=  gixTopItem ){	gixTopItem = gixMaxItem-1;	}
			break;

		case SB_BOTTOM:
			gixTopItem = gixMaxItem - 1;
			break;

		case SB_THUMBTRACK:
			gixTopItem = stScrollInfo.nTrackPos;
			break;

		case SBP_DIRECT:	//	���[�U�R�[�h����ɒǉ�
			gixTopItem = pos;
			break;

		default:	return;
	}
	//	�ω��Ȃ��Ȃ�Ȃɂ�����ł���
	if( maePos == gixTopItem )	return;

	InvalidateRect( ghItemsWnd, NULL, TRUE );
	UpdateWindow( ghItemsWnd  );	//	���A���^�C���`��ɕK�v

	stScrollInfo.fMask = SIF_POS;
	stScrollInfo.nPos  = gixTopItem;
	SetScrollInfo( ghScrollWnd, SB_CTL, &stScrollInfo, TRUE );

	TabMultipleTopMemory( gixTopItem );

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
LRESULT Aai_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	UINT_PTR		rdLength;
	LPSTR			pcConts = NULL;
	LPNMTTDISPINFO	pstDispInfo;

	if( TTN_GETDISPINFO ==  pstNmhdr->code )	//	�c�[���`�b�v�̓��e�̖₢���킹��������
	{
		pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

		if( !(gbAAtipView) )	//	��\���Ȃ牽�����Ȃ��ł���
		{
			ZeroMemory( &(pstDispInfo->szText), sizeof(pstDispInfo->szText) );
			pstDispInfo->lpszText = NULL;
			return 0;
		}

		FREE( gptTipBuffer );	//	NULL�t���[�ł����͖���

		pcConts = AacAsciiArtGet( gixNowSel );	//	�Y������C���f�b�N�XAA�����������Ă���
		if( !pcConts  ){	return 0;	}

		rdLength = strlen( pcConts );	//	������̒����擾

		//gptTipBuffer = (LPTSTR)malloc( sizeof(TCHAR) * (rdLength+1) );
		//ZeroMemory( gptTipBuffer, sizeof(TCHAR) * (rdLength+1) );
		//MultiByteToWideChar( CP_ACP, MB_COMPOSITE, pcConts, -1, gptTipBuffer, (rdLength+1) );

		gptTipBuffer = SjisDecodeAlloc( pcConts );

		pstDispInfo->lpszText = gptTipBuffer;

		free( pcConts );
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����i�E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndContext	�E�N���b�N�����q�E�C���h�E�n���h��
	@param[in]	xPos		�}�E�X�J�[�\���̃X�N���[���w���W
	@param[in]	yPos		�}�E�X�J�[�\���̃X�N���[���x���W
	@return		�Ȃ�
*/
VOID Aai_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	INT		dOpen;	//	�S�c���[�Ƃ��C�Ƀ��X�g�J���Ă��
	LPSTR	pcConts = NULL;
	UINT_PTR	rdLength;
	INT		sx, sy;

	dOpen = TreeFavWhichOpen(  );	//	�J���Ă�̂ŏ���������
	//	ACT_ALLTREE	ACT_FAVLIST

// _ORRVW
//	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_AAVIEW_POPUP) );

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_AALIST_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

#ifndef DRAUGHT_STYLE
	DeleteMenu( hSubMenu, IDM_DRAUGHT_ADDING, MF_BYCOMMAND );
#endif

	//	���C�Ƀ��X�g�̂݁A�폜��L���ɁA�ύX���邱�ƁE�W���Ŗ����ɂ��Ă���
	if( ACT_FAVLIST == dOpen )
	{	EnableMenuItem( hSubMenu, IDM_MAA_FAV_DELETE , MF_ENABLED );	}

	//	�c�[���`�b�v�̕\���E��\���̃g�S�D
	if( gbAAtipView ){	CheckMenuItem( hSubMenu, IDM_MAA_AATIP_TOGGLE, MF_CHECKED );	}

	//	�}���`���j�^���Ă�ƁA���W�l���}�C�i�X�ɂȂ邱�Ƃ�����B
	sx = (SHORT)xPos;
	sy = (SHORT)yPos;

	//	�t���O��TPM_RETURNCMD���w�肷��ƁAWM_COMMAND����΂Ȃ�
	dRslt = TrackPopupMenu( hSubMenu, TPM_RETURNCMD, sx, sy, 0, hWnd, NULL );	//	TPM_CENTERALIGN | TPM_VCENTERALIGN | 
	DestroyMenu( hMenu );
	switch( dRslt )
	{
		case IDM_MAA_FAV_DELETE:
			pcConts = AacAsciiArtGet( gixNowSel );	//	�Y������C���f�b�N�XAA�����������Ă���
			if( !pcConts ){	return;	}

			rdLength = strlen( pcConts );	//	������̒����擾
			AaItemsFavDelete( pcConts, rdLength );	//	�폜Commando���s
			FavContsRedrawRequest( hWnd );	//	�ĕ`�悵�Ȃ��Ⴞ��
			break;

#ifndef _ORRVW
		case IDM_MAA_INSERT_EDIT:		AaItemsDoSelect( hWnd, MAA_INSERT );	break;
		case IDM_MAA_INTERRUPT_EDIT:	AaItemsDoSelect( hWnd, MAA_INTERRUPT );	break;
		case IDM_MAA_SET_LAYERBOX:		AaItemsDoSelect( hWnd, MAA_LAYERED );	break;
#endif
		case IDM_MAA_CLIP_UNICODE:		AaItemsDoSelect( hWnd, MAA_UNICLIP );	break;
		case IDM_MAA_CLIP_SHIFTJIS:		AaItemsDoSelect( hWnd, MAA_SJISCLIP );	break;

#ifdef DRAUGHT_STYLE
		case IDM_DRAUGHT_ADDING:		AaItemsDoSelect( hWnd, MAA_DRAUGHT );	break;
#ifdef _ORRVW
		case IDM_DRAUGHT_OPEN:			Maa_OnCommand( hWnd, IDM_DRAUGHT_OPEN, NULL, 0 );	break;
#endif
#endif
		case IDM_MAA_AATIP_TOGGLE:
			gbAAtipView = gbAAtipView ? FALSE : TRUE;
			InitParamValue( INIT_SAVE, VL_MAATIP_VIEW, gbAAtipView );
			break;

		case IDM_MAA_SEP_STYLE_TOGGLE:
			gbLineSep = gbLineSep ? FALSE : TRUE;
			InitParamValue( INIT_SAVE, VL_MAASEP_STYLE, gbLineSep );
			InvalidateRect( ghItemsWnd, NULL, TRUE );
			break;


#ifdef DRAUGHT_STYLE
		case IDM_MAA_THUMBNAIL_OPEN:	Maa_OnCommand( hWnd , IDM_MAA_THUMBNAIL_OPEN, NULL, 0 );	break;
#endif
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h���b�O���h���b�v�̎󂯓���
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	hDrop	�h���b�s���I�u�W�F�N�g�n���h�D
*/
VOID Aai_OnDropFiles( HWND hWnd, HDROP hDrop )
{
	TCHAR	atFileName[MAX_PATH];
//	LPARAM	dNumber;

	ZeroMemory( atFileName, sizeof(atFileName) );

	DragQueryFile( hDrop, 0, atFileName, MAX_PATH );
	DragFinish( hDrop );

	TRACE( TEXT("MAA DROP[%s]"), atFileName );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	MLT�t�@�C�����������͂��C�ɓ��薼���󂯎���āA���e���X�^�e�B�b�N�ɓ����
	@param[in]	hWnd		�e�E�C���h�E�n���h��
	@param[in]	ptFileName	�t�@�C����
	@param[in]	type		�O(ACT_ALLTREE)�F�t�@�C���W�J�@�P(ACT_FAVLIST)�FSQL�W�J�@�Q�F���^�u���
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AaItemsDoShow( HWND hWnd, LPTSTR ptFileName, UINT type )
{
	SCROLLINFO	stScrollInfo;

	ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
	stScrollInfo.cbSize = sizeof(SCROLLINFO);

	//	�`��ʒu���ŏ�����ɂ��āA�ĕ`���Ԃɂ���
	gixTopItem = 0;

	//	�J��
	switch( type )
	{
		case ACT_ALLTREE:	gixMaxItem = AacAssembleFile( hWnd, ptFileName );	break;
		case ACT_FAVLIST:	gixMaxItem = AacAssembleSql( hWnd, ptFileName );	break;
		default:
			gixMaxItem = AacAssembleFile( hWnd, ptFileName );
			//	���X�g�������[�E�����ŕ`��ʒu�𕜌�����
			gixTopItem = TabMultipleTopMemory( -1 );
			break;
	}


	InvalidateRect( ghItemsWnd, NULL, TRUE );
	UpdateWindow( ghItemsWnd );

	if( 0 == gixMaxItem )
	{
		stScrollInfo.fMask = SIF_DISABLENOSCROLL;
		SetScrollInfo( ghScrollWnd, SB_CTL, &stScrollInfo, TRUE );
		return E_FAIL;
	}

	stScrollInfo.fMask = SIF_ALL;
	stScrollInfo.nMax  = gixMaxItem-1;
	stScrollInfo.nPos  = gixTopItem;
	stScrollInfo.nPage = 1;
	stScrollInfo.nTrackPos = 0;
	SetScrollInfo( ghScrollWnd, SB_CTL, &stScrollInfo, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�����`�`�����C�ɓ���r�p�k�ɂ����
	@param[in]	pcConts		AA�̕�����
	@param[in]	rdLength	�o�C�g��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AaItemsFavUpload( LPSTR pcConts, UINT rdLength )
{
	LPTSTR	ptBaseName;
	DWORD	dHash;

	ptBaseName = TreeBaseNameGet(  );	//	�ꗗ�x�[�X���擾����

	//	���̂`�`�̃n�b�V���l�����߂�
	HashData( (LPBYTE)pcConts, rdLength, (LPBYTE)(&(dHash)), 4 );

	//	���C�ɓ���ɋL�^����
	SqlFavUpload( ptBaseName, dHash, pcConts, rdLength );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C�ɓ��胊�X�g����폜����
	@param[in]	pcConts		AA�̕�����
	@param[in]	rdLength	�o�C�g��
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT AaItemsFavDelete( LPSTR pcConts, UINT rdLength )
{
	LPTSTR	ptBaseName;
	DWORD	dHash;

	ptBaseName = TreeBaseNameGet(  );	//	�ꗗ�x�[�X���擾����

	//	���̂`�`�̃n�b�V���l�����߂�
	HashData( (LPBYTE)pcConts, rdLength, (LPBYTE)(&(dHash)), 4 );

	//	���C�ɓ��肩��폜����
	SqlFavDelete( ptBaseName, dHash );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------




/*!
	�z�E�B�[����]�������̏�Ŕ���������
	@param[in]	hWnd	�e�E�C���h�E�n���h��
	@param[in]	hChdWnd	�}�E�X�J�[�\���́��ɂ������q�E�C���h�E
	@param[in]	zDelta	��]�ʁE���Ȃ������A���Ȃ牺�����ւ̃X�N���[���ƌ��Ȃ�
	@return		��O�����������@�O�֌W�Ȃ���
*/
UINT AaItemsIsUnderCursor( HWND hWnd, HWND hChdWnd, INT zDelta )
{
	UINT	dCode;

	if( ghItemsWnd != hChdWnd )	return 0;

	if( 0 < zDelta )		dCode = SB_LINEUP;
	else if( 0 > zDelta )	dCode = SB_LINEDOWN;
	else					dCode = SB_ENDSCROLL;

	FORWARD_WM_VSCROLL( hWnd, ghScrollWnd, dCode, 0, PostMessage );

	return 1;
}
//-------------------------------------------------------------------------------------------------


/*!
	���݃J�[�\�����ɂ���AA���g������
	@param[in]	hWnd	����AA�ꗗ�̃E�C���h�E�n���h��
	@param[in]	dMode	�g�p���[�h�������̓f�t�H��
	@return		UINT	��O�`�`�Ƃ����@�O�`�`��������
*/
UINT AaItemsDoSelect( HWND hWnd, UINT dMode )
{
	LPSTR		pcConts = NULL;
	UINT		uRslt;
	UINT_PTR	rdLength;

	pcConts = AacAsciiArtGet( gixNowSel );	//	�Y������C���f�b�N�XAA�����������Ă���
	if( !pcConts  ){	return 0;	}

	rdLength = strlen( pcConts );	//	������̒����擾

	uRslt = ViewMaaMaterialise( pcConts , rdLength, dMode );	//	�{�̂ɔ�΂�

	//	�����ł��C�ɓ���ɓ����
	AaItemsFavUpload( pcConts, rdLength );
	FavContsRedrawRequest( hWnd );

	free( pcConts );

	return 1;
}
//-------------------------------------------------------------------------------------------------


/*!
	�|�b�p�b�v�̕����T�C�Y�ύX�ƕ\����\����������
	@param[in]	ttSize	�����T�C�Y�E�P�Q���P�U�����Ȃ�
	@param[in]	bView	��O�\���@�O�\�����Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AaItemsTipSizeChange( INT ttSize, UINT bView )
{
	gbAAtipView = bView ? TRUE : FALSE;

	SetWindowFont( ghToolTipWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );
	DeleteFont( ghTipFont );
	//	���g���Ă郄�c�Ԃ��󂵂Ă���A�V�����̂����Ă�������
	ghTipFont = CreateFont( (FONTSZ_REDUCE == ttSize) ? FONTSZ_REDUCE : FONTSZ_NORMAL, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("�l�r �o�S�V�b�N") );
	SetWindowFont( ghToolTipWnd, ghTipFont, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


#ifndef _ORRVW


typedef struct tagITEMADDINFO
{
	LPTSTR	ptContent;			//!<	�{�����e
	TCHAR	atSep[MAX_PATH];	//!<	�Z�p���[�^���e
	INT		bType;				//!<	��OMLT�@�OAST

} ITEMADDINFO, *LPITEMADDINFO;
//--------------------------------------

/*!
	�c���[�̃t�@�C���ɃA�C�e���ǉ��E�ǉ��������t�@�C���̃p�X���󂯎��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	ptFile	�ǉ��������t�@�C���̃t���p�X
*/
HRESULT AacItemAdding( HWND hWnd, LPTSTR ptFile )
{
	HANDLE	hFile;

	LPTSTR		ptBuffer;//, ptExten;
	LPSTR		pcOutput;
//	TCHAR		atExBuf[10];
	CHAR		acCheck[6];
	DWORD		readed, wrote;
	UINT_PTR	cchSize, cchSep, cbSize;
	ITEMADDINFO	stIaInfo;	

	ZeroMemory( &stIaInfo, sizeof(ITEMADDINFO) );

	ZeroMemory( acCheck, sizeof(acCheck) );

	//	�g���q�m�F
	if( FileExtensionCheck( ptFile, TEXT(".ast") ) ){	stIaInfo.bType =  0;	}
	else{	stIaInfo.bType =  1;	}
	//ptExten = PathFindExtension( ptFile );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	//StringCchCopy( atExBuf, 10, ptExten );
	//CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ
	//stIaInfo.bType = StrCmp( atExBuf, TEXT(".ast") );	//	����AST�Ȃ�O�ɂȂ�

	if( DialogBoxParam( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MAA_IADD_DLG), hWnd, AaItemAddDlgProc, (LPARAM)(&stIaInfo) ) )
	{
		if( stIaInfo.ptContent )	//	���g���L���Ȃ珈������
		{
			hFile = CreateFile( ptFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			if( INVALID_HANDLE_VALUE != hFile )
			{
				//	�t�@�C����SJIS�^�ł���ƌ��Ȃ�
				SetFilePointer( hFile, -2, NULL, FILE_END );	//	�����̏󋵂��m�F
				ReadFile( hFile, acCheck, 6, &readed, NULL );	//	���̎��_�Ńt�@�C�������ɂ���
				SetFilePointer( hFile,  0, NULL, FILE_END );	//	�O�̂��ߖ���
				if( acCheck[0] != '\r' || acCheck[1] != '\n' )	//	���������s����Ȃ�������
				{
					acCheck[0] = '\r';	acCheck[1] = '\n';	acCheck[2] = NULL;
					WriteFile( hFile, acCheck, 2, &wrote, NULL );	//	���s�����Ă���
				}

				StringCchLength( stIaInfo.atSep, MAX_PATH, &cchSep );
				StringCchLength( stIaInfo.ptContent, STRSAFE_MAX_CCH, &cchSize );
				cchSize += (cchSep+2);
				ptBuffer = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
				ZeroMemory( ptBuffer, cchSize * sizeof(TCHAR) );

				StringCchPrintf( ptBuffer, cchSize, TEXT("%s%s\r\n"), stIaInfo.atSep, stIaInfo.ptContent );
				pcOutput = SjisEncodeAlloc( ptBuffer );
				cbSize = strlen( pcOutput );

				WriteFile( hFile , pcOutput, cbSize, &wrote, NULL );	//	���e��������

				SetEndOfFile( hFile );
				CloseHandle( hFile );
			}

			FREE(stIaInfo.ptContent);
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e���ǉ��̖ʓ|����_�C�����O�[
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK AaItemAddDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static LPITEMADDINFO	pstIaInfo;
	static LPTSTR	ptBuffer;
	UINT_PTR	cchSize;
	TCHAR	atName[MAX_PATH];
	INT		id;
	RECT	rect;

	switch( message )
	{
		case WM_INITDIALOG:
			pstIaInfo = (LPITEMADDINFO)(lParam);
			GetClientRect( hDlg, &rect );
			CreateWindowEx( 0, WC_BUTTON, TEXT("���̕�"),         WS_CHILD | WS_VISIBLE, 0, 0, 75, 23, hDlg, (HMENU)IDB_MAID_NOWPAGE, GetModuleHandle(NULL), NULL );
			CreateWindowEx( 0, WC_BUTTON, TEXT("�N���b�v�{�[�h"), WS_CHILD | WS_VISIBLE, 75, 0, 120, 23, hDlg, (HMENU)IDB_MAID_CLIPBOARD, GetModuleHandle(NULL), NULL ); 
			CreateWindowEx( 0, WC_EDIT,   TEXT(""),               WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 195, 0, rect.right-195-50, 23, hDlg, (HMENU)IDE_MAID_ITEMNAME, GetModuleHandle(NULL), NULL );
			CreateWindowEx( 0, WC_BUTTON, TEXT("�ǉ�"),           WS_CHILD | WS_VISIBLE, rect.right-50, 0, 50, 23, hDlg, (HMENU)IDB_MAID_ADDGO, GetModuleHandle(NULL), NULL );
			CreateWindowEx( 0, WC_EDIT,   TEXT(""),               WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_READONLY, 0, 23, rect.right, rect.bottom-23, hDlg, (HMENU)IDE_MAID_CONTENTS, GetModuleHandle(NULL), NULL );

			if( pstIaInfo->bType )
			{
				SetDlgItemText( hDlg, IDE_MAID_ITEMNAME, TEXT("���̂�AST�łȂ��Ǝg�p�ł��Ȃ��̂ł�") );
				EnableWindow( GetDlgItem(hDlg,IDE_MAID_ITEMNAME), FALSE );
				StringCchCopy( pstIaInfo->atSep, MAX_PATH, TEXT("[SPLIT]\r\n") );
			}

			//	�Ƃ肠�����N���b�v�{�[�h�̒��g���Ƃ�
			ptBuffer = DocClipboardDataGet( NULL );
			if( !(ptBuffer) ){	DocPageTextAllGetAlloc( D_UNI , (LPVOID *)(&ptBuffer) );	}
			//	�g���Ȃ��V�����m�Ȃ�A���̕ł̓��e�������Ă��ĕ\��
			SetDlgItemText( hDlg, IDE_MAID_CONTENTS, ptBuffer );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDCANCEL:
					FREE(ptBuffer);
					EndDialog(hDlg, 0 );
					return (INT_PTR)TRUE;

				case IDB_MAID_ADDGO:
					if( ptBuffer )
					{
						StringCchLength( ptBuffer, STRSAFE_MAX_CCH, &cchSize );
						cchSize += 2;
						pstIaInfo->ptContent = (LPTSTR)malloc( cchSize * sizeof(TCHAR) );
						StringCchCopy( pstIaInfo->ptContent, cchSize, ptBuffer );

						if( !(pstIaInfo->bType) )
						{
							GetDlgItemText( hDlg, IDE_MAID_ITEMNAME, atName, MAX_PATH );
							StringCchPrintf( pstIaInfo->atSep, MAX_PATH, TEXT("[AA][%s]\r\n"), atName );
						}
					}
					FREE(ptBuffer);
					EndDialog(hDlg, 1 );
					return (INT_PTR)TRUE;

				case IDB_MAID_CLIPBOARD:
					FREE(ptBuffer);
					ptBuffer = DocClipboardDataGet( NULL );
					SetDlgItemText( hDlg, IDE_MAID_CONTENTS, ptBuffer );
					return (INT_PTR)TRUE;

				case IDB_MAID_NOWPAGE:
					FREE(ptBuffer);
					DocPageTextAllGetAlloc( D_UNI , (LPVOID *)(&ptBuffer) );
					SetDlgItemText( hDlg, IDE_MAID_CONTENTS, ptBuffer );
					return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		case WM_SIZE:
			GetClientRect( hDlg, &rect );
			MoveWindow( GetDlgItem(hDlg,IDE_MAID_ITEMNAME), 195, 0, rect.right-195-50, 23, TRUE );
			MoveWindow( GetDlgItem(hDlg,IDB_MAID_ADDGO),    rect.right-50, 0, 50, 23, TRUE );
			MoveWindow( GetDlgItem(hDlg,IDE_MAID_CONTENTS), 0, 23, rect.right, rect.bottom-23, TRUE );
			break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------
	
	
#endif
