/*! @file
	@brief �X�v���b�g�o�[�̏������܂�
	���̃t�@�C���� SplitBar.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/04
*/
//-------------------------------------------------------------------------------------------------

//	�����g���Ƃ��E�o�[�E�C���h�E��TAG���Ă���

#include "stdafx.h"
#include "SplitBar.h"
//-------------------------------------------------------------------------------------------------

LRESULT	CALLBACK SplitProc( HWND, UINT, WPARAM, LPARAM );

VOID	Spt_OnPaint( HWND );
VOID	Spt_OnLButtonDown( HWND, BOOL, INT, INT, UINT );
VOID	Spt_OnMouseMove( HWND, INT, INT, UINT );
VOID	Spt_OnLButtonUp( HWND, INT, INT, UINT );
//-------------------------------------------------------------------------------------------------


/*!
	�X�v���b�g�o�[�̃N���X���쐬
	@param[in]	hInst	���̃��W���[���̃C���X�^���X�n���h��
	@return		�o�^�����N���X�A�g��
*/
ATOM SplitBarClass( HINSTANCE hInst )
{
	WNDCLASSEX	wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= SplitProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor( NULL, IDC_SIZEWE );
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= SPLITBAR_CLASS;
	wcex.hIconSm		= NULL;

	return RegisterClassEx( &wcex );
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�v���b�g�o�[�����
	@param[in]	hInst	���̃��W���[���̃C���X�^���X�n���h��
	@param[in]	hPrtWnd	�e�E�C���h�E�n���h��
	@param[in]	x		�N���C�����g�I�ȍ����W
	@param[in]	y		�N���C�����g�I�ȏ���W
	@param[in]	dHeight	����
	@return		�X�v���b�g�o�[�̃n���h��
*/
HWND SplitBarCreate( HINSTANCE hInst, HWND hPrWnd, INT x, INT y, INT dHeight )
{
	HWND	hWorkWnd;

	hWorkWnd = CreateWindowEx( WS_EX_WINDOWEDGE, SPLITBAR_CLASS, TEXT("SplitBar"),
		WS_CHILD | WS_VISIBLE, x, y, SPLITBAR_WIDTH, dHeight,
		hPrWnd, NULL, hInst, NULL );
	SetWindowLongPtr( hWorkWnd, GWLP_USERDATA, 0 );

	return hWorkWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�v���b�g�o�[�̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK SplitProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_PAINT,       Spt_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_LBUTTONDOWN, Spt_OnLButtonDown );	
		HANDLE_MSG( hWnd, WM_MOUSEMOVE,   Spt_OnMouseMove );	
		HANDLE_MSG( hWnd, WM_LBUTTONUP,   Spt_OnLButtonUp );	

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
VOID Spt_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	EndPaint( hWnd, &ps );

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
*/
VOID Spt_OnLButtonDown( HWND hWnd, BOOL fDoubleClick, INT x, INT y, UINT keyFlags )
{
	if( fDoubleClick )	return;	//	�_�u���N���b�N�͉������Ȃ�

	//	�X�v���b�g�o�[���N���b�N���ꂽ��T�C�Y�ύX���[�h�ɂȂ�
	SetWindowLongPtr( hWnd, GWLP_USERDATA, 1 );

	//	�}�E�X�̓������Ď�����
	SetCapture( hWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X���������Ƃ��̏���
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			�X�v���b�g�o�[������̑��΍��W�w
	@param[in]	y			�X�v���b�g�o�[�ォ��̑��΍��W�x
	@param[in]	keyFlags	������Ă鑼�̃{�^��
*/
VOID Spt_OnMouseMove( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	HWND	hPrWnd;
	RECT	stRect;
//	POINT	stPoint;
	LONG	wWidth = 0;
	LONG	wLeft = 0;
	LONG	bSpliting;

	bSpliting = GetWindowLongPtr( hWnd, GWLP_USERDATA );

	//	�T�C�Y�ύX���[�h�Ȃ�A���肸�蓮����
	if( bSpliting )
	{
		hPrWnd = GetParent( hWnd );
		GetClientRect( hPrWnd, &stRect );	//	�e�E�C���h�E
		wWidth = stRect.right;	//	���m��

		SplitBarPosGet( hWnd, &stRect );	//	�X�v���b�g�o�[�̍���ƕ�����
		wLeft = stRect.left + x;

		if( wLeft < SPLITBAR_LEFTLIMIT){	wLeft =  SPLITBAR_LEFTLIMIT;	}
		else if( wLeft >  wWidth - SPLITBAR_LEFTLIMIT ){	wLeft = wWidth - SPLITBAR_LEFTLIMIT;	}

		SetWindowPos( hWnd, HWND_TOP, wLeft, stRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�}�E�X�{�^���������ꂽ
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	x			�N���C�A���g���W�w
	@param[in]	y			�N���C�A���g���W�x
	@param[in]	keyFlags	������Ă鑼�̃{�^��
*/
VOID Spt_OnLButtonUp( HWND hWnd, INT x, INT y, UINT keyFlags )
{
	HWND	hPrWnd;
	RECT	stRect;
	//LONG	wWidth = 0;
	//LONG	wHeight = 0;
	LONG	bSpliting;

	bSpliting = GetWindowLongPtr( hWnd, GWLP_USERDATA );

	//	�T�C�Y�ύX���[�h�I��
	if( bSpliting )
	{
		hPrWnd = GetParent( hWnd );

		ReleaseCapture(   );	//	�}�E�X�L���v�`���I��
		SetWindowPos( hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		GetClientRect( hPrWnd, &stRect );
		//wHeight = stRect.bottom - stRect.top;
		//wWidth  = stRect.right  - stRect.left;

		//	�T�C�Y�ύX������e�E�C���h�E�ɑ��M
		FORWARD_WM_SIZE( hPrWnd, keyFlags, x,	y, PostMessage );
	}

	SetWindowLongPtr( hWnd, GWLP_USERDATA, 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�v���b�g�o�[�̃N���C�A���g��̍�����W�ƁA���ƍ������m�ۂ���
	@param[in]	hSplitWnd	�Ώۂ̃X�v���b�g�o�[�n���h��
	@param[out]	ptRect		�X�v���b�g�o�[�̃T�C�Y������\���̂ւ̃|�C���^�[
*/
VOID SplitBarPosGet( HWND hSplitWnd, LPRECT ptRect )
{
	HWND	hPrWnd = GetParent( hSplitWnd );
	RECT	clRect;
	POINT	point;
	LONG	wHeight;

	GetWindowRect( hSplitWnd, &clRect );
	wHeight = clRect.bottom - clRect.top;
	point.x = clRect.left;
	point.y = clRect.top;

	ScreenToClient( hPrWnd, &point );

	SetRect( ptRect, point.x, point.y, SPLITBAR_WIDTH, wHeight );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	��ʃT�C�Y���ς�����̂ŃT�C�Y�ύX
	@param[in]	hSplitWnd	�Ώۂ̃X�v���b�g�o�[�n���h��
	@param[in]	ptRect		�N���C�A���g�̈�
*/
VOID SplitBarResize( HWND hSplitWnd, LPRECT ptRect )
{
	RECT	rect;

	SplitBarPosGet( hSplitWnd, &rect );

	SetWindowPos( hSplitWnd, HWND_TOP, rect.left, ptRect->top, SPLITBAR_WIDTH, ptRect->bottom, 0 );

	InvalidateRect( hSplitWnd, NULL, TRUE );
	UpdateWindow( hSplitWnd );

	return;
}
//-------------------------------------------------------------------------------------------------
