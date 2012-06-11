/*! @file
	@brief ����̃c�[���`�b�v�̐���
	���̃t�@�C���� HoverTip.cpp �ł��B
	@author	SikigamiHNQ
	@date	2012/06/08
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 - 2012 Orinrin/SikigamiHNQ

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


/*
�c�[���`�b�v�̃T�u�N���X�ł��Ȃ����H

�c�[���`�b�v�r���[�����ɂ�
��{�I�Ƀ}�E�X���[�u�Ŕ�������͂�
����p�̃n���h�����邩�H
TrackMouseEvent WM_MOUSEHOVER WM_MOUSELEAVE ������
WM_MOUSEHOVER��������A���X�g�r���[�Ƃ��̃n���h���n���ăc�[���`�b�v�|�b�p�b�v���R�[������B
�R�[�����ꂽ��A���̃n���h���ɑ΂���DISPINFO�I��message�𑗂�
�������̓R�[���o�b�N�������w�肷��Ƃ��B
���e�Q�b�g������A�쐬���ĕ\���B�P�̂������蓾�Ȃ��̂ŁA
�O�̂���������j�󂷂�悤�ɁB
��\���̂܂�WM_MOUSELEAVE��������L�����Z�����H

�t���[�e�B���O�E�C���h�E�����Ă����ɕ\���E�h���t�g�{�[�h�Ɠ����悤��
�\���g�̐e�̓f�X�N�g�b�v�������ł����B
�}�E�X�N���b�N�ŕ���E�R�O�b�o�߂ŕ���
��pixel�̃}�E�X���[�u�����m���ĕ���

�|�b�v�A�b�v�̃T�C�Y�́A������Ō��߂�B�����Q�������炢�ŁB
�w�i�ƕ����F�� GetSysColor �łƂ��

SystemParametersInfo  SPI_GETMOUSEHOVERTIME  SPI_GETMOUSEHOVERWIDTH �Œ����o����

*/

/*
�o���邾�����e�͉B������
�g�p���ł́AWM_MOUSEHOVER �̃L���b�`����B�����|�C���g�̍��W�󂯎��
���e�̌ďo�́A�R�[���o�b�N���邩�H�\�����e�̕�����|�C���^���󂯎��
�J���͂������ł��΂����H

�`�b�v���̂̓X�^�e�B�b�N�ŃI�[�i�[�h���[
*/
#ifdef USE_HOVERTIP

//#error MOUSEHOVER��肩��



#define HOVER_TIPS_CLASS	TEXT("HOVER_TIPS")
//-------------------------------------------------------------------------------------------------

static  ATOM	gTipAtom;	//!<	�E�C���h�E�N���X�A�g��
static  HWND	ghTipWnd;	//!<	�z�o�[�`�b�v�̃E�C���h�E�n���h��
static HFONT	ghTipFont;	//!<	�c�[���`�b�v�p

static LPTSTR	gptContent;	//!<	�\�����e
static RECT		gstContSize;//!<	�\���傫��
//-------------------------------------------------------------------------------------------------



LRESULT	CALLBACK HoverTipProc( HWND, UINT, WPARAM, LPARAM );	//!<	
VOID	Htp_OnPaint( HWND );						//!<	
VOID	htp_OnTimer( HWND, UINT );					//!<	
VOID	Htp_OnKillFocus( HWND, HWND );				//!<	
VOID	Htp_OnLButtonUp( HWND, INT, INT, UINT );	//!<	
VOID	Htp_OnMButtonUp( HWND, INT, INT, UINT );	//!<	
VOID	Htp_OnRButtonUp( HWND, INT, INT, UINT );	//!<	
VOID	Htp_OnMouseMove( HWND, INT, INT, UINT );	//!<	
//-------------------------------------------------------------------------------------------------

/*!
	������
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hPtWnd		���C�����E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT HoverTipInitialise( HINSTANCE hInstance, HWND hPtWnd )
{
	LOGFONT	stFont;
	WNDCLASSEX	wcex;

	if( hInstance )
	{
		gptContent = NULL;

		//	�\���`�b�v�E�C���h�E�N���X�쐬
		ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= HoverTipProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_INFOBK+1);	//	�c�[���`�b�v�R���g���[���̔w�i�F
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= HOVER_TIPS_CLASS;
		wcex.hIconSm		= NULL;

		gTipAtom = RegisterClassEx( &wcex );
	
		//	�\���`�b�v�E�C���h�E�쐬 | WS_EX_TOPMOST
		ghTipWnd = CreateWindowEx( WS_EX_TOOLWINDOW,
			HOVER_TIPS_CLASS, TEXT("InfoTip"), WS_POPUP | WS_BORDER, 0, 0, 15, 15, NULL, NULL, hInstance, NULL );
		//	�ŏ��͔�\��

		//	�\���t�H���g�쐬
		ViewingFontGet( &stFont );
		stFont.lfHeight = FONTSZ_REDUCE;
		ghTipFont = CreateFontIndirect( &stFont );
	}
	else
	{
		DeleteFont( ghTipFont );

		DestroyWindow( ghTipWnd );

		FREE( gptContent );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	MouseHover�̓o�^����
	@param[in]	hTgtWnd	�`�F�b�N�Ώۂ̃E�C���h�E�̃n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT HoverTipResist( HWND hTgtWnd )
{
	//	Hover�Œ�ELEAVE�͎g��Ȃ���

	TRACKMOUSEEVENT	stTrackMsEv;

	TRACE( TEXT("MOUSE HOVER RESIST") );

	ZeroMemory( &stTrackMsEv, sizeof(TRACKMOUSEEVENT) );
	stTrackMsEv.cbSize      = sizeof(TRACKMOUSEEVENT);
	stTrackMsEv.dwFlags     = TME_HOVER | TME_LEAVE;
	stTrackMsEv.hwndTrack   = hTgtWnd;
	stTrackMsEv.dwHoverTime = HOVER_DEFAULT;	//	���ԁA���̂��������o����悤��
	TrackMouseEvent( &stTrackMsEv );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	WM_MOUSEHOVER���󂯎��
	@param[in]	hEvWnd		���������E�C���h�E�̃n���h��
	@param[in]	wParam		�ǉ����P
	@param[in]	lParam		�ǉ����Q
	@param[in]	pfInfoGet	�\�����e�����������R�[���o�b�N����
	@return	����������O
*/
LRESULT HoverTipOnMouseHover( HWND hEvWnd, WPARAM wParam, LPARAM lParam, HOVERTIPDISP pfInfoGet )
{
	INT		x, y;
	UINT	keyFlags;
	LPTSTR	ptText;

	HDC		hdc;
	HFONT	hOldFnt;

	RECT	deskRect;
	POINT	point;
	INT		xSub, ySub;

//	HOVERTIPINFO	stTipInfo;

	TRACE( TEXT("MOUSE HOVER RISING") );

	keyFlags = (UINT)wParam;
	//	Indicates whether various virtual keys are down.
	//	This parameter can be one or more of the following values.
	//	Value		Meaning
	//	MK_CONTROL	The CTRL key is depressed.
	//	MK_LBUTTON	The left mouse button is depressed.
	//	MK_MBUTTON	The middle mouse button is depressed.
	//	MK_RBUTTON	The right mouse button is depressed.
	//	MK_SHIFT	The SHIFT key is depressed.
	//	MK_XBUTTON1	The first X button is down.
	//	MK_XBUTTON2	The second X button is down.

	x = (INT)(SHORT)LOWORD(lParam);
	y = (INT)(SHORT)HIWORD(lParam);
	//	The low-order word specifies the x-coordinate of the cursor.
	//	The coordinate is relative to the upper-left corner of the client area.
	//	The high-order word specifies the y-coordinate of the cursor.
	//	The coordinate is relative to the upper-left corner of the client area.

	point.x = x;
	point.y = y;
	ClientToScreen( hEvWnd, &point );

//	ZeroMemory( &stTipInfo, sizeof(HOVERTIPINFO) );

	FREE( gptContent );

	ptText = pfInfoGet( NULL );
	if( !(ptText) )	//	�Ȃ񂩂�������
	{
		return 0;
	}

	gptContent = ptText;

	hdc = GetDC( ghTipWnd );

	SetRect( &gstContSize, 0, 0, 2222, 100 );

	hOldFnt = SelectFont( hdc, ghTipFont );

	DrawText( hdc, gptContent, -1, &gstContSize, DT_LEFT | DT_CALCRECT | DT_NOPREFIX );
	TRACE( TEXT("HOVER Size[ %d x %d, %d : %d"), gstContSize.left, gstContSize.top, gstContSize.right, gstContSize.bottom );

	SelectFont( hdc, hOldFnt );

	gstContSize.right  += 4;
	gstContSize.bottom += 4;

	//	�f�X�N�g�b�v�T�C�Y�m��
	GetWindowRect( GetDesktopWindow(), &deskRect );
	//	��ʂ��f�J���Ȃ�J�b�g
	if( gstContSize.right  >  deskRect.right  ){	gstContSize.right  = deskRect.right;	}
	if( gstContSize.bottom >  deskRect.bottom ){	gstContSize.bottom = deskRect.bottom;	}

	xSub = (point.x + gstContSize.right) - deskRect.right;
	if( 0 <  xSub ){	point.x -= xSub;	}
	ySub = (point.y + gstContSize.bottom) - deskRect.bottom;
	if( 0 <  ySub ){	point.y -= ySub;	}


	SetWindowPos( ghTipWnd, HWND_TOP, point.x, point.y, gstContSize.right, gstContSize.bottom, SWP_SHOWWINDOW );

	ReleaseDC( ghTipWnd, hdc );

	//	�h���t�g�{�[�h�Ɠ����悤�ɁA�t���[�e�B���O�E�C���h�E����āAWM_PAINT �ŕ`�悷��΂���


	return 0;	//	If an application processes this message, it should return zero.
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
LRESULT CALLBACK HoverTipProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	//	HANDLE_MSG( hWnd, WM_SIZE,        Htp_OnSize );	
	//	HANDLE_MSG( hWnd, WM_COMMAND,     Htp_OnCommand );	
	//	HANDLE_MSG( hWnd, WM_NOTIFY,      Htp_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_LBUTTONUP,   Htp_OnLButtonUp );
		HANDLE_MSG( hWnd, WM_MBUTTONUP,   Htp_OnMButtonUp );
		HANDLE_MSG( hWnd, WM_RBUTTONUP,   Htp_OnRButtonUp );
		HANDLE_MSG( hWnd, WM_MOUSEMOVE,   Htp_OnMouseMove );
		HANDLE_MSG( hWnd, WM_PAINT,       Htp_OnPaint );
	//	HANDLE_MSG( hWnd, WM_CONTEXTMENU, Htp_OnContextMenu );	//	�E�N�����j���[
	//	HANDLE_MSG( hWnd, WM_DESTROY,     Htp_OnDestroy );
		HANDLE_MSG( hWnd, WM_KILLFOCUS,   Htp_OnKillFocus );	//	�t�H�[�J�X��������
	//	HANDLE_MSG( hWnd, WM_VSCROLL,     Htp_OnVScroll );		//	�c�X�N���[���֘A
	//	HANDLE_MSG( hWnd, WM_MOUSEWHEEL,  Htp_OnMouseWheel );	//	�}�E�X�z�E�B�[��
		HANDLE_MSG( hWnd, WM_TIMER,       htp_OnTimer );


		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Htp_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;
	HFONT		hOldFnt;
	RECT	rect;

	hdc = BeginPaint( hWnd, &ps );

	hOldFnt = SelectFont( hdc, ghTipFont );
	SetTextColor( hdc, GetSysColor( COLOR_INFOTEXT ) );
	SetBkMode( hdc, TRANSPARENT );

	rect = gstContSize;
	rect.left += 2;
	rect.top  += 2;

	DrawText( hdc, gptContent, -1, &rect, DT_LEFT | DT_NOPREFIX );

	SelectFont( hdc, hOldFnt );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�C�}�C�x���g����
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	id		�^�C�}�h�c
*/
VOID htp_OnTimer( HWND hWnd, UINT id )
{



	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�H�[�J�X���������ꍇ
	@param[in]	hWnd			�E�C���h�E�n���h��
	@param[in]	hwndNewFocus	�t�H�[�J�X�𓾂��E�C���h�E�̃n���h��
*/
VOID Htp_OnKillFocus( HWND hWnd, HWND hwndNewFocus )
{
	ShowWindow( hWnd, SW_HIDE );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̍��{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Htp_OnLButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	TRACE( TEXT("HTP LUP %d x %d"), x , y );	//	�N���C�����g���W

	ShowWindow( hWnd, SW_HIDE );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̒��{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Htp_OnMButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	TRACE( TEXT("HTP MUP %d x %d"), x , y );	//	�N���C�����g���W

	ShowWindow( hWnd, SW_HIDE );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�̉E�{�^�������������ꂽ�Ƃ�
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			���������w���W�l
	@param[in]	y			���������x���W�l
	@param[in]	keyFlags	���ɉ�����Ă�L�[�ɂ���
*/
VOID Htp_OnRButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	TRACE( TEXT("HTP RUP %d x %d"), x , y );	//	�N���C�����g���W

	ShowWindow( hWnd, SW_HIDE );

	return;
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
VOID Htp_OnMouseMove( HWND hWnd, INT x, INT y, UINT keyFlags )
{


	return;
}
//-------------------------------------------------------------------------------------------------

#endif
