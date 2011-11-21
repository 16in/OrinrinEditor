/*! @file
	@brief MLT�c���[�Ƃ��̕���
	���̃t�@�C���� MaaTemplate.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/21
*/

//	TODO:	MLT�Ƃ��̈�s���o���������Ȃ����EAST�ł������Ȃ��H


/*

HukuTabs �̒��g
2ch\���v�h��\���v\���v11�i�~�j�T�C�Y�j.mlt
2ch\���v�h��\���Ȃ��v\���Ȃ��v01�i��{�j.mlt

PathStripPath	���[���t�@�C�����f�B���N�g���֌W�Ȃ��ɐ؂�o��

*/


#include "stdafx.h"
#include "OrinrinEditor.h"
#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------

#ifdef _ORRVW
#define  MAATMPLT_CLASS_NAME	TEXT("ORINRINVIEWER")
#else
#define  MAATMPLT_CLASS_NAME	TEXT("MULTILINEAA_CLASS")
#endif
#define MA_WIDTH	320
#define MA_HEIGHT	320
//-------------------------------------------------------------------------------------------------

static HINSTANCE	ghInst;		//!<	�A�v���̎���

static  HWND	ghMainWnd;		//!<	�{�̃E�C���h�E

static  HWND	ghMaaWnd;		//!<	���̃E�C���h�E
static  HWND	ghStsBarWnd;	//!<	�X�e�[�^�X�o�[�n���h��

EXTERNED HWND	ghSplitaWnd;	//!<	�X�v���b�g�o�[�n���h��

EXTERNED UINT	gbAAtipView;	//!<	��O�ŁA�`�`�c�[���`�b�v�\��

#ifdef MAA_PROFILE
static TCHAR	gatProfilePath[MAX_PATH];	//!<	�v���t�@�C���f�B���N�g��
#endif
static TCHAR	gatTemplatePath[MAX_PATH];	//!<	MLT���[�g�f�B���N�g��

static CONST INT	giStbRoom[] = { 250 , 350 , -1 };
//-------------------------------------------------------------------------------------------------

LRESULT	CALLBACK MaaTmpltWndProc( HWND, UINT, WPARAM, LPARAM );
BOOLEAN	Maa_OnCreate( HWND, LPCREATESTRUCT );		//!<	WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Maa_OnCommand( HWND , INT, HWND, UINT );	//!<	WM_COMMAND �̏���
VOID	Maa_OnPaint( HWND );						//!<	WM_PAINT �̏����E�g���`��Ƃ�
VOID	Maa_OnDestroy( HWND );						//!<	WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
LRESULT	Maa_OnNotify( HWND , INT, LPNMHDR );			//!<	
VOID	Maa_OnDrawItem( HWND, CONST DRAWITEMSTRUCT * );	//!<	
VOID	Maa_OnMeasureItem( HWND, MEASUREITEMSTRUCT * );	//!<	

#ifdef MAA_PROFILE
INT_PTR	CALLBACK TreeProfileDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	
HRESULT	TreeProfListUp( HWND, LPTSTR, HTREEITEM, UINT, INT );	//!<	

UINT	TreeLoadNodeProc( HWND, HTREEITEM, UINT );	//!<	

VOID	TreeProfCheckState( HWND, HTREEITEM, UINT );	//!<	

#endif
//-------------------------------------------------------------------------------------------------

/*!
	�����s�`�`�e���v���E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�ҏW�r���[�̂������Ă�E�C���h�E�̃n���h���E���ꂪ�{��
	@param[in]	pstFrame	
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND MaaTmpltInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame )
{
	WNDCLASSEX	wcex;
	RECT	wdRect, rect;
#ifndef _ORRVW
	RECT	sbRect;
#endif
	INT		bMode = 0;

#ifdef _ORRVW
	INT	bTopMost;
#endif

#ifdef MAA_PROFILE
	WIN32_FIND_DATA	stFindData;
	HANDLE	hFind;
#endif

	ghMainWnd  = hParentWnd;
	ghInst = hInstance;

	//	��p�̃E�C���h�E�N���X�쐬
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MaaTmpltWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= MAATMPLT_CLASS_NAME;
#ifdef _ORRVW
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORINRINEDITOR));
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ORINRINVIEWER);
#else
	wcex.hIcon			= NULL;
	wcex.hIconSm		= NULL;
	wcex.lpszMenuName	= NULL;
#endif

	RegisterClassEx( &wcex );


#ifdef _ORRVW
	SplitBarClass( hInstance );	//	�X�v���b�g�o�[�̏���
#endif

	InitWindowPos( INIT_LOAD, WDP_MAATPL, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
#ifdef _ORRVW
		rect.left = ( wdRect.right  - MA_WIDTH ) / 2;
		rect.top  = ( wdRect.bottom - MA_HEIGHT ) / 2;
#else
		rect.left   = wdRect.right + 64;
		rect.top    = wdRect.top + 64;
#endif
		rect.right  = MA_WIDTH;
		rect.bottom = MA_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_MAATPL, &rect );//�N�����ۑ�
	}

	ghMaaWnd = CreateWindowEx(
#ifdef _ORRVW
		0, MAATMPLT_CLASS_NAME, TEXT("Orinrin Viewer"), WS_OVERLAPPEDWINDOW,
#else
		WS_EX_TOOLWINDOW,
		MAATMPLT_CLASS_NAME, TEXT("Multi Line AA Template"),
		WS_POPUP | WS_THICKFRAME | WS_BORDER | WS_CAPTION | WS_VISIBLE,
#endif
		rect.left, rect.top, rect.right, rect.bottom,
		NULL, NULL, hInstance, NULL);

	if( !(ghMaaWnd) )	return NULL;

	//	��ɍőS�ʂɕ\�����H
#ifdef _ORRVW
	bTopMost = InitParamValue( INIT_LOAD, VL_MAA_TOPMOST, 1 );
	if( bTopMost )
	{
		SetWindowPos( ghMaaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		CheckMenuItem( GetMenu(ghMaaWnd), IDM_TOPMOST_TOGGLE, MF_CHECKED );
	}
#else
	if( InitWindowTopMost( INIT_LOAD, WDP_MAATPL , 0 ) )
	{	SetWindowPos( ghMaaWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );	}
#endif

	ShowWindow( ghMaaWnd, SW_SHOW );
	UpdateWindow( ghMaaWnd );

	ZeroMemory( gatTemplatePath, sizeof(gatTemplatePath) );

#ifdef MAA_PROFILE
	//	�v���t�@�C�����[�h
	ZeroMemory( gatProfilePath, sizeof(gatProfilePath) );
	InitParamString( INIT_LOAD, VS_PROFILE_NAME, gatProfilePath );

	//	���̃t�@�C���͑��݂��邩�H
	hFind = FindFirstFile( gatProfilePath, &stFindData );	//	TEXT("*")
	if( INVALID_HANDLE_VALUE != hFind ){	FindClose( hFind  );	}
	else{		ZeroMemory( gatProfilePath, sizeof(gatProfilePath) );	};

	if( NULL != gatProfilePath[0] )	//	�N����������Ή������Ȃ�
	{
		bMode = TreeProfileMake( ghMaaWnd, gatProfilePath );
		if( 0 > bMode ){	bMode = 0;	}
	}
#else
	//	MLT�f�B���N�g���̈ʒu
	InitMaaFldrOpen( INIT_LOAD, gatTemplatePath );
	bMode = 0;
#endif
	TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );

	gbAAtipView = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );

#ifndef MAA_PROFILE
	SqlDatabaseOpenClose( M_CREATE, MAA_FAVDB_FILE );
	SqlFavTableCreate( NULL );
#endif

#ifndef _ORRVW
	//	���������l�������ċA��
	MaaTabBarSizeGet( pstFrame );
	pstFrame->top = pstFrame->bottom;
	GetClientRect( ghStsBarWnd, &sbRect );
	pstFrame->bottom = sbRect.bottom;	//	�X�e�[�^�X�o�[�̕��̖ʓ|����
#endif

	return ghMaaWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK MaaTmpltWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	UINT	uRslt;

	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,      Maa_OnCreate );		//	��ʂ̍\���p�[�c�����B�{�^���Ƃ�
		HANDLE_MSG( hWnd, WM_PAINT,       Maa_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_COMMAND,     Maa_OnCommand );		//	�{�^�������ꂽ�Ƃ��̃R�}���h����
		HANDLE_MSG( hWnd, WM_DESTROY,     Maa_OnDestroy );		//	�\�t�g�I�����̏���
		HANDLE_MSG( hWnd, WM_NOTIFY,      Maa_OnNotify );		//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_SIZE,        Maa_OnSize  );		//	
		HANDLE_MSG( hWnd, WM_CHAR,        Maa_OnChar  );		//	
		HANDLE_MSG( hWnd, WM_DRAWITEM,    Maa_OnDrawItem  );	//	
		HANDLE_MSG( hWnd, WM_MEASUREITEM, Maa_OnMeasureItem );	//	
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Maa_OnContextMenu );	//	
		
		HANDLE_MSG( hWnd, WM_VSCROLL,     Aai_OnVScroll );	

		case WM_MOUSEWHEEL:	//	�Ԃ�l���K�v�ȏꍇ���l��
			uRslt = Maa_OnMouseWheel( hWnd, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam), (INT)(SHORT)HIWORD(wParam), (UINT)(SHORT)LOWORD(wParam) );
			break;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	���ɂȂ�
*/
BOOLEAN Maa_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	HINSTANCE lcInst = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�
	RECT	rect, sbRect, tbRect;

	INT	spPos;


	GetClientRect( hWnd, &rect );

	//	�X�e�[�^�X�o�[
	ghStsBarWnd = CreateStatusWindow( WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, TEXT(""), hWnd, IDSB_STATUSBAR );
	GetClientRect( ghStsBarWnd, &sbRect );
	rect.bottom -= sbRect.bottom;

	SendMessage( ghStsBarWnd, SB_SETPARTS, 3, (LPARAM)giStbRoom );

	TreeInitialise( hWnd, lcInst, &rect );	//	�c���[�r���[�Ƃ��C�Ƀr���[���

	MaaTabBarSizeGet( &tbRect );

	//	�ݒ肩��X�v���b�g�o�[�̈ʒu����������
	spPos = InitParamValue( INIT_LOAD, VL_MAA_SPLIT, 150 );
	ghSplitaWnd = SplitBarCreate( lcInst, hWnd, spPos, tbRect.bottom, rect.bottom - tbRect.bottom );

	AaItemsInitialise( hWnd, lcInst, &rect );	//	MLT�̒��g�\���r���[����

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hwndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Maa_OnCommand( HWND hWnd, INT id, HWND hwndCtl, UINT codeNotify )
{
	LONG_PTR	rdExStyle;

	switch( id )
	{
		//	���X�g�X�^�e�B�b�N�ł̃N���b�N�͂����ɂ���
		case IDSO_AAITEMS:	TRACE( TEXT("static") );	break;

		case IDLB_FAVLIST:	FavListSelected( hWnd, codeNotify );	break;

		case IDM_TOPMOST_TOGGLE:	//	�펞�őS�ʂƒʏ�E�C���h�E�̃g�O��
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#ifdef _ORRVW
				CheckMenuItem( GetMenu(hWnd), IDM_TOPMOST_TOGGLE, MF_UNCHECKED );
				InitParamValue( INIT_SAVE, VL_MAA_TOPMOST, 0 );
#else
				InitWindowTopMost( INIT_SAVE, WDP_MAATPL, 0 );
#endif
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
#ifdef _ORRVW
				CheckMenuItem( GetMenu(hWnd), IDM_TOPMOST_TOGGLE, MF_CHECKED );
				InitParamValue( INIT_SAVE, VL_MAA_TOPMOST, 1 );
#else
				InitWindowTopMost( INIT_SAVE, WDP_MAATPL, 1 );
#endif
			}
#ifdef _ORRVW
			DrawMenuBar( hWnd );
#endif
			break;

		case IDCB_AAITEMTITLE:	AaTitleSelect( hWnd, codeNotify );	break;

		//	�����ɗ���̂́A���C�����j���[����̑I���̂�
#ifndef MAA_PROFILE
		case IDM_TREE_RECONSTRUCT:	//	�v���t���[�h�ł́A�f�B���N�g���I���_�C�����O�o��
			InitMaaFldrOpen( INIT_LOAD, gatTemplatePath );
			TreeConstruct( hWnd , gatTemplatePath, FALSE );
			break;
#endif

#ifdef _ORRVW
		case IDM_ABOUT:	DialogBox( ghInst , MAKEINTRESOURCE(IDD_ORRVWR_ABOUTBOX), hWnd, About );	break;
		case IDM_EXIT:	DestroyWindow( hWnd );	break;

		case IDM_GENERAL_OPTION:	//	�ݒ�
			DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_ORRVWR_OPTION_DLG), hWnd, OptionDlgProc, NULL );
			break;

	#ifdef MAA_PROFILE
		case IDM_MAA_PROFILE_MAKE:	TreeProfileOpen( hWnd );	break;
		case IDM_TREE_RECONSTRUCT:	TreeProfileRebuild( hWnd  );	break;
	#endif
#else
		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_MAAT,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_MAAT, -1 );	break;
#endif

		default:	break;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Maa_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��m�ۂ�����ʍ\���̃������Ƃ����I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Maa_OnDestroy( HWND hWnd )
{
	RECT	rect;
	DWORD	dwStyle;

	dwStyle = GetWindowStyle( hWnd );
	if( !(dwStyle & WS_MINIMIZE) )
	{
		//	�ő剻���Ă���L�^���Ȃ��H

		//	�X�v���b�g�o�[�̈ʒu���L�^
		SplitBarPosGet( ghSplitaWnd, &rect );
		InitParamValue( INIT_SAVE, VL_MAA_SPLIT, rect.left );

		//	�E�C���h�E�ʒu���L�^
		GetWindowRect( hWnd, &rect );
		rect.right  -= rect.left;
		rect.bottom -= rect.top;
		InitWindowPos( INIT_SAVE, WDP_MAATPL, &rect );//�I�����ۑ�
	}

	AaItemsInitialise( NULL, NULL, NULL );
	AacMatrixClear(   );	//	�����̓��e�S�j��

	TreeInitialise( NULL, NULL, NULL );

	SqlDatabaseOpenClose( M_DESTROY, NULL );

#ifdef _ORRVW
	PostQuitMessage( 0 );
#endif

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�̃T�C�Y�ύX
	@param[in]	hWnd	�E�C���h�E�n���h��
*/
VOID Maa_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	RECT	rect, sbRect, tbRect;

	GetClientRect( hWnd, &rect );

	MoveWindow( ghStsBarWnd, 0, 0, 0, 0, TRUE );	//	�X�e�[�^�X�o�[
	GetClientRect( ghStsBarWnd, &sbRect );
	rect.bottom -= sbRect.bottom;	//	�X�e�[�^�X�o�[�̕��̖ʓ|����

	TabBarResize( hWnd, &rect );
	MaaTabBarSizeGet( &tbRect );
	rect.top = tbRect.bottom;
	rect.bottom -= tbRect.bottom;	//	�^�u�o�[�̕��̖ʓ|������

	SplitBarResize( ghSplitaWnd, &rect );	//	�X�v���b�g�o�[

	TreeResize( hWnd, &rect );		//	�c���[�r���[
	AaItemsResize( hWnd, &rect );	//	�ꗗ�r���[

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
LRESULT Maa_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	//	�c���[�r���[�̂��
	if( IDTV_ITEMTREE == idFrom ){	TreeNotify( hWnd , (LPNMTREEVIEW)pstNmhdr );	}
	if( IDTB_TREESEL == idFrom ){	TabBarNotify( hWnd , pstNmhdr );	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�[�i�[�h���[�̔�����������
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	*pstDrawItem	�h���[�ׂ̈̏��
*/
VOID Maa_OnDrawItem( HWND hWnd, CONST DRAWITEMSTRUCT *pstDrawItem )
{
	if( IDSO_AAITEMS == pstDrawItem->CtlID )
	{
		AaItemsDrawItem( hWnd, pstDrawItem );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�[�i�[�h���[�̕`��G���A�̏���
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	*pstMeasureItem	�`��ׂ̈̏����o��������ꂽ��
*/
VOID Maa_OnMeasureItem( HWND hWnd, MEASUREITEMSTRUCT *pstMeasureItem )
{
#if 0
	if( IDSO_AAITEMS == pstMeasureItem->CtlID )
	{
		AaItemsMeasureItem( hWnd, pstMeasureItem );
	}
#endif
	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�܂����ق��`��������
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	xPos	�����������W�w
	@param[in]	yPos	�����������W�x
	@param[in]	zDelta	��]�ʁEWHEEL_DELTA�̔{���E���̒l�͑O(��)�A���̒l�͌��(��O)�։񂳂ꂽ
	@param[in]	fwKeys	������Ă�L�[
	@return		��O�t�H�[�J�X�Ɛ^�����Ⴄ�@�O����
*/
UINT Maa_OnMouseWheel( HWND hWnd, INT xPos, INT yPos, INT zDelta, UINT fwKeys )
{
	HWND	hChdWnd;
	POINT	stPoint;

	stPoint.x = xPos;
	stPoint.y = yPos;
	ScreenToClient( ghMaaWnd, &stPoint );
	hChdWnd = ChildWindowFromPointEx( ghMaaWnd, stPoint, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED | CWP_SKIPTRANSPARENT );

	if( hChdWnd == hWnd )	return 0;

	if( AaItemsIsUnderCursor( ghMaaWnd, hChdWnd , zDelta ) )	return 1;

	TreeFavIsUnderCursor( ghMaaWnd, hChdWnd, xPos, yPos, zDelta, fwKeys );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�e�[�^�X�o�[�ɂȂ񂩕\��
	@param[in]	room	�`�悷��g�O�C���f�b�N�X
	@param[in]	ptMsg	�`�悷�镶����
*/
VOID StatusBarMsgSet( UINT room, LPTSTR ptMsg )
{

	SendMessage( ghStsBarWnd, SB_SETTEXT, room, (LPARAM)ptMsg );

	UpdateWindow( ghStsBarWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

#ifdef MAA_PROFILE

/*!
	�v���b�t�@�C���������MLT�f�B���N�g�����w�肵���蒼���J������
	@param[in]	hWnd	�E�C���h�E�n���h���E�ǂ��̂��낤�H
	@param[in]	ptProf	�J���v���b�t�@�C����
	@return		UINT	���F�r�p�k����\�z�@�O�t�@�C������\�z�Z���@���F�G���[
*/
INT TreeProfileMake( HWND hWnd, LPTSTR ptProf )
{
	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;
	UINT	iCount;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING], atBuffer[MAX_PATH];
	TCHAR	atFolder[MAX_PATH];
	INT_PTR	iRslt;



	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	if( ptProf )
	{
		StringCchCopy( atFilePath, MAX_PATH, ptProf );
	}
	else	//	�J���w�肪�Ȃ�������A����������w�肷��
	{
		ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );
		stOpenFile.lStructSize     = sizeof(OPENFILENAME);
		stOpenFile.hwndOwner       = ghMaaWnd;
		stOpenFile.lpstrFilter     = TEXT("�v���t�@�C��(*.qor)\0*.qor\0�S�Ẵt�@�C��(*.*)\0*.*\0\0");
//		stOpenFile.nFilterIndex    = 1;
		stOpenFile.lpstrFile       = atFilePath;
		stOpenFile.nMaxFile        = MAX_PATH;
		stOpenFile.lpstrFileTitle  = atFileName;
		stOpenFile.nMaxFileTitle   = MAX_STRING;
	//	stOpenFile.lpstrInitialDir = 
		stOpenFile.lpstrTitle      = TEXT("�t�@�C�������w�肷�邩�A�쐬���������O����͂��邩����[");
		stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
		stOpenFile.lpstrDefExt     = TEXT("qor");

		//������ FileSaveDialogue ���o��
		bOpened = GetOpenFileName( &stOpenFile );

		if( !(bOpened) ){	return -1;	}	//	�L�����Z�����Ă��牽�����Ȃ�

		StringCchCopy( gatProfilePath, MAX_PATH, atFilePath );
		InitParamString( INIT_SAVE, VS_PROFILE_NAME, atFilePath );
	}


	TabMultipleStore( hWnd );	//	�����̃f�[�^����O�ɊJ���Ă镛�^�u��ۑ�
	TabMultipleDeleteAll( hWnd );	//	���^�u�S������

	SqlDatabaseOpenClose( M_DESTROY, NULL );	//	�����̃f�[�^�͕���

	//	�v���b�t�@�C�������t���p�X�œn��

	SqlDatabaseOpenClose( M_CREATE, atFilePath );//MAA_TREE_CACHE	TEXT(":memory:")
	//	���ŊJ��������ɍ����

	//	�v���t�@�C�������X�e�[�^�X�o�[�ɕ\�����Ă���
	StringCchCopy( atBuffer, MAX_PATH, atFilePath );
	PathStripPath( atBuffer );
	StatusBarMsgSet( 0, atBuffer );

	ZeroMemory( atFolder,  sizeof(atFolder) );

	//	MLT�f�B���N�g�����Z�b�g
	ZeroMemory( gatTemplatePath,  sizeof(gatTemplatePath) );
	SqlTreeProfSelect( NULL, 0, gatTemplatePath, MAX_PATH );
	//	�f�B���N�g�����Ȃ�������t�H���_�I�[�|���_�C�����O�Ŏw�肵�čč\�z
	//	�܂ރf�B���N�g���Ɗ܂܂Ȃ��f�B���N�g�����w��o����悤�ɂ���
	//	�f�B���N�g�����L������ACache�����čč\�z����΂���
	if( NULL == gatTemplatePath[0] )
	{
		//�_�C�����O�Ăяo��
		//�_�C�����O�ł́A�f�B���N�g�������̃f�B���N�g�������X�g�A�b�v
		//ON/OFF��Check���āA�č\�z�t���c���[�č\�z
		//OFF�͍̂č\�z���Ȃ�
		//	�c���[�č\�z�Ȃ�A���̃_�C�����O���Ăяo��
		iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PROFILE_TREESEL_DLG), hWnd, TreeProfileDlgProc, (LPARAM)atFolder );
		if( IDCANCEL == iRslt ){	return -1;	}

		StringCchCopy( gatTemplatePath, MAX_PATH, atFolder );
	}

	//	�f�[�^���邩�H
	iCount = SqlTreeCount( 1, NULL );
	if( 0 >= iCount )	return 0;

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	��Profile�J������������
*/
INT TreeProfileOpen( HWND hWnd )
{
	INT	iRslt;

	iRslt = TreeProfileMake( hWnd, NULL );
	if( 0 > iRslt ){	return 0;	}	//	���e�ύX�Ȃ̂ŁA���Ȃ�i�j�����Ȃ��B
	//	�����ŁA�\�z������Ăׂ΂�낵��
	TreeConstruct( ghMaaWnd, gatTemplatePath, TRUE );
	//�����̊J���Ȃ�P�A�V�K�쐬�Ȃ�O���߂�͂�
	//������ɂ��Ă��A���^�u�č\�z���K�v

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃v���t�@�C���̓��e��ҏW����
	@param[in]	hWnd	�n���h��
*/
INT TreeProfileRebuild( HWND hWnd )
{
	TCHAR	atFolder[MAX_PATH];
	INT_PTR	iRslt;

	//	�v���t�@�C�����J���ĂȂ��ꍇ�́A�x�����b�Z�[�W�����ĂȂɂ����Ȃ�

	//	���[�g�f�B���N�g�����A�_�C�����O�ɓn���ăI�[�|��
	StringCchCopy( atFolder, MAX_PATH, gatTemplatePath );	//	���[�g�f�B���N�g�����R�s�[

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_PROFILE_TREESEL_DLG), hWnd, TreeProfileDlgProc, (LPARAM)atFolder );
	if( IDCANCEL == iRslt ){	return (-1);	}	//	���e�ύX�Ȃ̂ŁA�L�����Z���Ȃ�i�j�����Ȃ��B
	//	���[�g�f�B���N�g���ύX���Ă���č\�z�J�n
	if( NULL != atFolder[0] )	//	���̎��_�ŋ�͂Ȃ����H
	{
		StringCchCopy( gatTemplatePath, MAX_PATH, atFolder );
		//	����������J�����ꍇ�́A���^�u�č\�z�s�v
		TreeConstruct( ghMaaWnd, gatTemplatePath, FALSE );
	}

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[�Ɏ�荞�ރf�B���N�g���[��ݒ肵�č\�z����
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK TreeProfileDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static BOOLEAN	cbAct;
	static LPTSTR	ptFolder;
	static  HWND	chTvWnd;

	static HTREEITEM	chTreeRoot;
	static TVINSERTSTRUCT	cstRootIns;


	TCHAR	atTgtDir[MAX_PATH];
	INT		id;

	DWORD	mPos;
	UINT	bCheck, count;
	INT		idFrom;
	LPNMHDR	pstNmhdr;

	LPNMTREEVIEW	pstNmTrVw;
	TVHITTESTINFO	stTreeHit;
	HTREEITEM		hItem;

	SHFILEINFO		stShFileInfo;

//���̂܂܃��X�g�A�b�v����ƁA���e�ҏW�̂Ƃ��ɒ��g���𒃋�ׂɂȂ�

	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			ptFolder = (LPTSTR)lParam;

			cbAct = FALSE;

			Edit_SetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), ptFolder );

			chTvWnd = GetDlgItem( hDlg, IDTV_PRTREE_DIR_TVIEW );

			//	�c���[�r���[�\�z
			TreeView_DeleteAllItems( chTvWnd );
			//	���[�g�A�C�e�����
			ZeroMemory( &cstRootIns, sizeof(TVINSERTSTRUCT) );
			cstRootIns.hParent      = TVI_ROOT;
			cstRootIns.hInsertAfter = TVI_SORT;
			cstRootIns.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			cstRootIns.item.pszText = TEXT("ROOT");
			cstRootIns.item.lParam  = 0;

			SHGetFileInfo( TEXT(""), 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON) );
			cstRootIns.item.iImage = stShFileInfo.iIcon;
			SHGetFileInfo( TEXT(""), 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON) );
			cstRootIns.item.iSelectedImage = stShFileInfo.iIcon;

			chTreeRoot = TreeView_InsertItem( chTvWnd, &cstRootIns );

			SqlTreeCacheOpenClose( M_CREATE );

			//	�J�����Ƃ��A���[�g�t�H���_�w�肪�L���Ȃ�A�������X�g�A�b�v���āA�f�B���N�g�����m�F���ă`�F�b�N�t����
			if( NULL != ptFolder[0] )
			{
				TreeProfListUp( chTvWnd, ptFolder, chTreeRoot, 0, 1 );
				TreeView_Expand( chTvWnd, chTreeRoot, TVE_EXPAND );
			}
			return (INT_PTR)TRUE;


		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case  IDB_PRTREE_DIR_REF:	//	�f�B���N�g���Q��
					if( SelectDirectoryDlg( hDlg, atTgtDir, MAX_PATH ) )
					{
						Edit_SetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir );
					}
					return (INT_PTR)TRUE;

				case  IDB_PRTREE_LISTUP:	//	���X�g�A�b�|�J�n
					Edit_GetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir, MAX_PATH );
					if( NULL != atTgtDir[0] )
					{
						//	��U�S�j�󂵂ă��[�g��蒼��
						TreeView_DeleteAllItems( chTvWnd  );
						chTreeRoot = TreeView_InsertItem( chTvWnd, &cstRootIns );
						TreeView_SetCheckState( chTvWnd , chTreeRoot, TRUE );	//	�`�F�L�}�[�N�H

						StringCchCopy( ptFolder, MAX_PATH, atTgtDir );	//	�������L����������
						TreeProfListUp( chTvWnd, atTgtDir, chTreeRoot, 0, 1 );
						TreeView_Expand( chTvWnd, chTreeRoot, TVE_EXPAND );
					}
					return (INT_PTR)TRUE;

				case IDOK:
					count = TreeView_GetCount( chTvWnd );
					if( 1 >= count )
					{
						MessageBox( hDlg, TEXT("���X�g�A�b�v�o���Ă��Ȃ��悤�Ȃ̂ł��B\r\n���̂܂܂ł̓c���[�����Ȃ��̂ł��B"), TEXT("��������"), MB_OK | MB_ICONERROR );
						return (INT_PTR)TRUE;
					}

					//	�f�B���N�g���I���ɍ��킹��SQL���Œ�
					if( SUCCEEDED( TreeLoadDirCheck( hDlg, chTvWnd ) ) )
					{
						SqlTreeCacheOpenClose( M_DESTROY );
						EndDialog(hDlg, IDOK );
					}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					SqlTreeCacheOpenClose( M_DESTROY );
					EndDialog(hDlg, IDCANCEL );
					return (INT_PTR)TRUE;
			}
			break;


		case WM_NOTIFY:
			idFrom   = (INT)wParam;
			pstNmhdr = (LPNMHDR)lParam;
			if( IDTV_PRTREE_DIR_TVIEW == idFrom )
			{
				pstNmTrVw = (LPNMTREEVIEW)pstNmhdr;
				if( NM_CLICK == pstNmhdr->code )
				{
					ZeroMemory( &stTreeHit, sizeof(TVHITTESTINFO) );
					mPos = GetMessagePos( );
					stTreeHit.pt.x = GET_X_LPARAM( mPos );
					stTreeHit.pt.y = GET_Y_LPARAM( mPos );
					MapWindowPoints( HWND_DESKTOP, chTvWnd, &stTreeHit.pt, 1 );
					TreeView_HitTest( chTvWnd, &stTreeHit );

					//	�`�F�b�N������O�ɗ���
					if ( TVHT_ONITEMSTATEICON & stTreeHit.flags )
					{
						//	�`�F�b�N�����鎞���`�F�b�N�������Ă��Ȃ����
						bCheck = TreeView_GetCheckState( chTvWnd, stTreeHit.hItem );

						//	��ɉ��������X�L���@�`�����Ȃ��Ƃ��������Ȃ�悤��
						hItem = TreeView_GetChild( chTvWnd, stTreeHit.hItem );
						TreeProfCheckState( chTvWnd, hItem, bCheck );

						//	������ɂ́A�`�F�b�N��������A���n�m�ɂ���
						if( !(bCheck)  )	//	�`�F�L����Ƃ�
						{
							hItem = TreeView_GetParent( chTvWnd, stTreeHit.hItem );
							while( hItem  )
							{
								TreeView_SetCheckState( chTvWnd, hItem, TRUE );
								hItem = TreeView_GetParent( chTvWnd, hItem );
							}
						}
					}

					SetWindowLong( hDlg, DWL_MSGRESULT, 0 );
					return (INT_PTR)TRUE;
				}
			}
			break;


		//case WM_ACTIVATE:
		//	TRACE( TEXT("WM_ACTIVATE") );
		//	if( !(cbAct) )	PostMessage( hDlg, WMP_PROF_ACT, 0, 0 );
		//	cbAct = TRUE;
		//	break;
		//	�������˂��A�Ȃ�ł�˂�
		//case WMP_PROF_ACT:
		//	TreeProfCheckState( chTvWnd, chTreeRoot, FALSE );
		//	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

//	�ċA�ŁA�ȉ��̃c���[�̃`���b�N��ON/OFF����
VOID TreeProfCheckState( HWND hTvWnd, HTREEITEM hNode, UINT bCheck )
{
	HTREEITEM	hItem;

	if( !(hNode) )	return;

	do
	{
		TreeView_SetCheckState( hTvWnd, hNode, bCheck ? FALSE : TRUE );

		hItem = TreeView_GetChild( hTvWnd , hNode );	//	�q�m�[�h���m�F

		//	���݂��Ă��牺�̊K�w���`�F�b�N
		if( hItem ){	TreeProfCheckState( hTvWnd, hItem, bCheck );	}

		//	�I������玟�ɂ���
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;
	}
	while( hNode );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���X�g�A�b�v����
	@param[in]	hTvWnd	�c���[�r���[�n���h��
	@param[in]	ptRoot	MLT���[�g�f�B���N�g��
	@param[in]	hTreePr	�Ώۃf�B���N�g���̃c���[�A�C�e���E�����ɂԂ牺���Ă���
	@param[in]	dPrntID	SQL��ID�E�f�B���N�g���ԍ�
	@param[in]	fCheck	�P�S�`���b�N����@�O�r�p�k�Ɋ����Ȃ�`�F�L�@�|�P�`���b�N���Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TreeProfListUp( HWND hTvWnd, LPTSTR ptRoot, HTREEITEM hTreePr, UINT dPrntID, INT fCheck )
{
	HANDLE	hFind;
	TCHAR	atPath[MAX_PATH], atNewTop[MAX_PATH], atTarget[MAX_PATH];
	BOOL	bRslt;
	UINT	dPnID = 0;

	WIN32_FIND_DATA	stFindData;

	HTREEITEM	hNewParent, hLastDir = TVI_FIRST;
	TVINSERTSTRUCT	stTreeIns;
	SHFILEINFO	stShFileInfo;

	ZeroMemory( &stTreeIns, sizeof(TVINSERTSTRUCT) );
	stTreeIns.hParent      = hTreePr;
	stTreeIns.hInsertAfter = TVI_LAST;
	stTreeIns.item.mask    = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	ZeroMemory( atTarget, sizeof(atTarget) );
	StringCchCopy( atTarget, MAX_PATH, ptRoot );
	PathAppend( atTarget, TEXT("*") );

	hFind = FindFirstFile( atTarget, &stFindData );	//	TEXT("*")
	do{
		if( lstrcmp( stFindData.cFileName, TEXT("..") ) && lstrcmp( stFindData.cFileName, TEXT(".") ) )
		{
			StringCchCopy( atPath, MAX_PATH, ptRoot );
			PathAppend( atPath, stFindData.cFileName );

			SHGetFileInfo( atPath, 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON) );
			stTreeIns.item.iImage = stShFileInfo.iIcon;
			SHGetFileInfo( atPath, 0, &stShFileInfo, sizeof(SHFILEINFO), (SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON) );
			stTreeIns.item.iSelectedImage = stShFileInfo.iIcon;

			stTreeIns.item.pszText = stFindData.cFileName;

			if( stFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{	//	�f�B���N�g���̏ꍇ
				dPnID = SqlTreeCacheInsert( FILE_ATTRIBUTE_DIRECTORY, dPrntID, stFindData.cFileName );

				stTreeIns.item.lParam  = dPnID;	//	�ʂ��ԍ��ł��邱�Ƃɒ���
				stTreeIns.hInsertAfter = hLastDir;
				hNewParent = TreeView_InsertItem( hTvWnd, &stTreeIns );
				hLastDir = hNewParent;

				TreeView_SetCheckState( hTvWnd , hNewParent, TRUE );	//	�`�F�L�}�[�N�H

				StringCchCopy( atNewTop, MAX_PATH, ptRoot );
				PathAppend( atNewTop, stFindData.cFileName );

				TreeProfListUp( hTvWnd, atNewTop, hNewParent, dPnID, fCheck );	//	�Y���f�B���N�g�������ċA����

			}
			else
			{	//	�t�@�C���̏ꍇ
				bRslt  = PathMatchSpec( stFindData.cFileName, TEXT("*.mlt") );	//	�q�b�g������TRUE
				bRslt |= PathMatchSpec( stFindData.cFileName, TEXT("*.ast") );	//	�q�b�g������TRUE
				if( bRslt )	//	20110720	AST���C�P��悤�ɂ���
				{
					dPnID = SqlTreeCacheInsert( FILE_ATTRIBUTE_NORMAL, dPrntID, stFindData.cFileName );

					stTreeIns.item.lParam  = dPnID;	//	�ʂ��ԍ��ł��邱�Ƃɒ���
					stTreeIns.hInsertAfter = TVI_LAST;
					hNewParent = TreeView_InsertItem( hTvWnd, &stTreeIns );

					TreeView_SetCheckState( hTvWnd , hNewParent, TRUE );	//	�`�F�L�}�[�N�H
				}
			}
		}

	}while( FindNextFile( hFind, &stFindData ) );

	FindClose( hFind );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�c���[�v���t�@�C���쐬�ŁA�`�F�b�N����Ă�ł�ꂭ�Ƃ�Ɵ����̃t�@�C�����L�^
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	hTvWnd	�c���[�r���[�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TreeLoadDirCheck( HWND hDlg, HWND hTvWnd )
{
	TCHAR	atTgtDir[MAX_PATH];
	HTREEITEM	hTreeRoot, hItem;
	UINT	dCacheMax, dCacheCnt, m, count;
	UINT	dType, dPrnt, index, logoa;
	TCHAR	atName[MAX_PATH];

	hTreeRoot = TreeView_GetRoot( hTvWnd  );	//	�Ƃ�₦�����[�g�m��
	//	���[�g�͊֌W�Ȃ��̂ŁA�����𒲂ׂ�
	hItem = TreeView_GetChild( hTvWnd, hTreeRoot );	//	�q�m�[�h���m�F

	//	�`�F�b�N�󋵂��m�F�E�t�@�C���̃`�F�b�N�������Ȃ烄�o�C
	count = TreeLoadNodeProc( hTvWnd, hItem, 0 );
	TRACE( TEXT("%u"), count );
	if( 0 == count )
	{
		MessageBox( hDlg, TEXT("�t�@�C��������I������Ă��Ȃ��̂ł��B\r\n���̂܂܂��Ǝg���Ȃ��̂ł��B"), TEXT("��������"), MB_OK | MB_ICONERROR );
		return E_ABORT;
	}

	TreeLoadNodeProc( hTvWnd, hItem, 1 );

	Edit_GetText( GetDlgItem(hDlg,IDE_PRTREE_DIR), atTgtDir, MAX_PATH );

	//	�c���[�f�[�^�̓���ւ�
	SqlTreeProfUpdate( NULL, atTgtDir );	//	���[�g�p�X��ύX
	SqlTreeNodeDelete(  1 );	//	�t�@�C������\�z����ꍇ�ASQL�̒��g����ɂ��Ă���

	dCacheCnt = SqlTreeCount( 3, &dCacheMax );
	index = 0;
	for( m = 0; dCacheMax > m; m++ )
	{
		ZeroMemory( atName, sizeof(atName) );
		index = SqlTreeNodePickUpID( index, &dType, &dPrnt, atName, 0x00 );
		TRACE( TEXT("[%4u]%4u\t%4u\t%4u\t%s"), m, index, dType, dPrnt, atName );
		if( 0 >= index )	break;	//	�f�[�^�����Ȃ�����I���

		logoa = SqlTreeNodeInsert( index, dType, dPrnt, atName );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ċA�����ŁA�c���[�̃`�F�b�N�󋵂𒲂ׂăL���b�V���𑀍�
	@param[in]	hTvWnd	�c���[�n���h��
	@param[in]	hNode	�`�F�L���_�m�[�h
	@param[in]	bFixe	��O���ۂɑ���@�OCheck�󋵂̊m�F
*/
UINT TreeLoadNodeProc( HWND hTvWnd, HTREEITEM hNode, UINT bFixe )
{
	TCHAR		atName[MAX_PATH];
	INT			param;
	UINT		dRslt, dType, dPrID, count = 0;
	HTREEITEM	hItem;
	TVITEM		stItem;

	do
	{
		//	�Y������A�C�e���h�c������������
		ZeroMemory( &stItem, sizeof(TVITEM) );
		stItem.mask  = TVIF_HANDLE | TVIF_PARAM | TVIF_TEXT;
		stItem.hItem      = hNode;
		stItem.pszText    = atName;
		stItem.cchTextMax = MAX_PATH;
		TreeView_GetItem( hTvWnd, &stItem );
		param = stItem.lParam;

		//	�`�F�b�N�󋵂��m�F
		dRslt = TreeView_GetCheckState( hTvWnd, hNode );
		TRACE( TEXT("Node:[%d][%d]%s"), dRslt, param, atName );

		if( bFixe )
		{
			//	�`�F�b�N������������
			if( !(dRslt)  ){	SqlTreeCacheDelID( param  );	}
		}
		else
		{	//	���͒l�𒴂����l�Ȃ̂ŁA�ڕW���Q�b�g����ɂ́|�P����
			SqlTreeNodePickUpID( param-1, &dType, &dPrID, atName, 0x00 );
			//	�`�F�b�N������A�t�@�C���ł���ꍇ
			if( dRslt && (FILE_ATTRIBUTE_NORMAL==dType) ){	count++;	}
		}

		hItem = TreeView_GetChild( hTvWnd , hNode );	//	�q�m�[�h���m�F

		//	���݂��Ă��牺�̊K�w���`�F�b�N
		if( hItem ){	count += TreeLoadNodeProc( hTvWnd, hItem, bFixe );	}

		//	�I������玟�ɂ���
		hItem = TreeView_GetNextSibling( hTvWnd, hNode );
		hNode = hItem;
	}
	while( hNode );

	return count;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ώۂ̃f�B���N�g����Check�ɂ��邩
	@param[in]	ptDir	�m�F�������f�B���N�g�����̂�
	@return		UINT	��O�A���@�O����
*/
UINT TreeIsExistLoad( LPTSTR ptDir )
{


	return 0;
}
//-------------------------------------------------------------------------------------------------


/*!
	�p�X���󂯎���āA�擪�̂ł�ꂭ�Ƃ���R�s�[����
	@param[in]	ptSource	���p�X�EMAX_PATH�ł��邱��
	@param[in]	ptSplits	�擪�p�X���R�s�[����o�b�t�@�EMAX_PATH�ł��邱��
	@return		���p�X�́A���̃f�B���N�g���ʒu
*/
LPTSTR PathSplitFirstPath( LPTSTR ptSource, LPTSTR ptSplit )
{
	UINT	d;

	if( !(ptSource) )	return NULL;
	if( NULL ==  ptSource[0] )	return NULL;

	for( d = 0; MAX_PATH > d; d++ )
	{
		if( TEXT('\\') == ptSource[d] ){	ptSplit[d] = NULL;	d++;	break;	}
		else if( NULL ==  ptSource[d] ){	ptSplit[d] = NULL;	break;	}
		else{	ptSplit[d] = ptSource[d];	}
	}

	return &(ptSource[d]);
}
//-------------------------------------------------------------------------------------------------

/*!
	������̎��̉��s�܂ł����
	@param[in]	ptSource	��������
	@param[in]	*ptNextLn	���̍s�̐擪
	@return		�m�ۂ���������Efree�Z��
*/
LPTSTR StringLineGet( LPCTSTR ptSource, LPCTSTR *ptNextLn )
{
	UINT		t = 0;
	UINT_PTR	len;
	wstring		wStr;
	LPTSTR		ptDest;

	while( ptSource[t] )
	{
		if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	break;

		wStr += ptSource[t];
		t++;
	}

	if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	t++;
	if( TEXT('\r') == ptSource[t] || TEXT('\n') == ptSource[t] )	t++;
	*ptNextLn = &(ptSource[t]);

	len = wStr.size() + 1;
	ptDest = (LPTSTR)malloc( len * 2 );
	ZeroMemory( ptDest, len * 2 );
	StringCchCopy( ptDest, len, wStr.c_str() );

	return ptDest;
}
//-------------------------------------------------------------------------------------------------






/*
�c���[������T�[�`
�����e�ԍ������ғ��ŃT�[�`�E�����f�B���N�g�����ɂ́A�P�ꖼ��������Ȃ�����A
���ꂪ�������m���Ɣ��f�ł���͂�

*/

#endif	//	MAA_PROFILE

