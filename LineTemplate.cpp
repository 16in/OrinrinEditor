/*! @file
	@brief ��s�e���v���[�g�̖ʓ|���܂�
	���̃t�@�C���� LineTemplate.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/17
*/

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

#define LINETEMPLATE_CLASS	TEXT("LINE_TEMPLATE")
#define LT_WIDTH	240
#define LT_HEIGHT	240
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

extern HFONT	ghAaFont;		//	AA�p�t�H���g
extern HFONT	ghNameFont;		//	�t�@�C���^�u�p�t�H���g

extern INT		gbTmpltDock;	//	�ŁE��s�e���v���̃h�b�L���O

#ifdef MAIN_SPLITBAR
extern  HWND	ghMainSplitWnd;	//	���C���̃X�v���b�g�o�[�n���h��
extern  LONG	grdSplitPos;	//	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g
#endif

static  ATOM	gTmpleAtom;		//!<	
static  HWND	ghTmpleWnd;		//!<	���̃E�C���h�E�n���h��

static  HWND	ghCtgryBxWnd;	//!<	
static  HWND	ghLvItemWnd;	//!<	
static  HWND	ghLnLvTipWnd;	//!<	��s���X�g�c�[���`�b�v

static  HWND	ghDockTabWnd;	//!<	�h�b�L���O�����Ƃ��̑I�����p

static  UINT	gNowGroup;		//!<	

static WNDPROC	gpfOrigLineCtgryProc;	//!<	
static WNDPROC	gpfOrigLineItemProc;	//!<	

static  UINT	gLnClmCnt;	//!<	�\���J������

static vector<AATEMPLATE>	gvcTmples;	//!<	�e���v���̕ێ�
//-------------------------------------------------------------------------------------------------

LRESULT	CALLBACK LineTmpleProc( HWND, UINT, WPARAM, LPARAM );	//!<	
VOID	Ltp_OnCommand( HWND , INT, HWND, UINT );	//!<	
VOID	Ltp_OnSize( HWND , UINT, INT, INT );	//!<	
LRESULT	Ltp_OnNotify( HWND , INT, LPNMHDR );	//!<	
VOID	Ltp_OnContextMenu( HWND, HWND, UINT, UINT );	

UINT	CALLBACK LineTmpleItemData( LPTSTR, LPTSTR, INT );	//!<	

HRESULT	LineTmpleItemListOn( UINT );	//!<	

HRESULT	TemplateItemSplit( LPTSTR, UINT, PAGELOAD );	//!<	
HRESULT	TemplateItemScatter( LPTSTR, INT, PAGELOAD );	//!<	

LRESULT	CALLBACK gpfLineCtgryProc( HWND, UINT, WPARAM, LPARAM );	//!<	
LRESULT	CALLBACK gpfLineItemProc(  HWND, UINT, WPARAM, LPARAM );	//!<	
LRESULT	Ltl_OnNotify( HWND, INT, LPNMHDR );

HWND	DockingTabCreate( HINSTANCE, HWND, LPRECT );
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------

/*!
	��s�e���v���E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�e�E�C���h�E�̃n���h��
	@param[in]	pstFrame	���C���N���C�����g�̈�
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND LineTmpleInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame )
{

	WNDCLASSEX	wcex;
	RECT		wdRect, clRect, rect, cbxRect;
	UINT_PTR	dItems, i;
	DWORD		dwExStyle, dwStyle;
	HWND		hPrWnd;
#ifdef MAIN_SPLITBAR
	INT			spPos;
#endif

	TTTOOLINFO	stToolInfo;
	LVCOLUMN	stLvColm;


	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= LineTmpleProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= LINETEMPLATE_CLASS;
	wcex.hIconSm		= NULL;

	gTmpleAtom = RegisterClassEx( &wcex );

//�e���v���f�[�^�ǂݏo��
	TemplateItemLoad( AA_LIST_FILE, LineTmpleItemData );


	InitWindowPos( INIT_LOAD, WDP_LNTMPL, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		GetWindowRect( hParentWnd, &wdRect );
		rect.left   = wdRect.right;
		rect.top    = wdRect.top;
		rect.right  = LT_WIDTH;
		rect.bottom = LT_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_LNTMPL, &rect );//�N�����ۑ�
	}

	//	�J�������m�F
	gLnClmCnt = InitParamValue( INIT_LOAD, VL_LINETMP_CLM, 4 );

	if( gbTmpltDock )
	{
#ifdef MAIN_SPLITBAR
		spPos = grdSplitPos - SPLITBAR_WIDTH;	//	�E����̃I�t�Z�b�g
#endif
		hPrWnd    = hParentWnd;
		dwExStyle = 0;
		dwStyle   = WS_CHILD | WS_VISIBLE;

		rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
		rect.left  = rect.right - spPos;
		rect.right = PLIST_DOCK;
		rect.bottom >>= 1;
		rect.top    += rect.bottom;

		//	�u���V�Ɛ؊��^�u���쐬
		ghDockTabWnd = DockingTabCreate( hInstance, hPrWnd, &rect );
	}
	else
	{
		//	��ɍőS�ʂɕ\�����H
		dwExStyle = WS_EX_TOOLWINDOW;
		if( InitWindowTopMost( INIT_LOAD, WDP_LNTMPL, 0 ) ){	dwExStyle |=  WS_EX_TOPMOST;	}
		dwStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE | WS_SYSMENU;
		hPrWnd = NULL;
	}

	//	�E�C���h�E�쐬
	ghTmpleWnd = CreateWindowEx( dwExStyle, LINETEMPLATE_CLASS, TEXT("Line Template"),
		dwStyle, rect.left, rect.top, rect.right, rect.bottom, hPrWnd, NULL, hInstance, NULL);

	GetClientRect( ghTmpleWnd, &clRect );

	//	�J�e�S���R���{�b�N�X
	ghCtgryBxWnd = CreateWindowEx( 0, WC_COMBOBOX, TEXT("category"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
		0, 0, clRect.right, 127, ghTmpleWnd,
		(HMENU)IDCB_LT_CATEGORY, hInstance, NULL );

	gpfOrigLineCtgryProc = SubclassWindow( ghCtgryBxWnd, gpfLineCtgryProc );

	dItems = gvcTmples.size( );
	for( i = 0; dItems > i; i++ )
	{
		ComboBox_AddString( ghCtgryBxWnd, gvcTmples.at( i ).atCtgryName );
	}
	ComboBox_SetCurSel( ghCtgryBxWnd, 0 );
	gNowGroup = 0;

	GetClientRect( ghCtgryBxWnd, &cbxRect );

	//	�A�C�e���ꗗ���X�g�r���[
	ghLvItemWnd = CreateWindowEx( 0, WC_LISTVIEW, TEXT("lineitem"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LVS_REPORT | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER,
		0, cbxRect.bottom, clRect.right, clRect.bottom - cbxRect.bottom,
		ghTmpleWnd, (HMENU)IDLV_LT_ITEMVIEW, hInstance, NULL );
	ListView_SetExtendedListViewStyle( ghLvItemWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP | LVS_EX_ONECLICKACTIVATE );

	SetWindowFont( ghLvItemWnd, ghAaFont, TRUE );

	gpfOrigLineItemProc = SubclassWindow( ghLvItemWnd, gpfLineItemProc );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt      = LVCFMT_LEFT;
	stLvColm.pszText  = TEXT("Item");
	stLvColm.cx       = 10;	//	��ō��킹��̂œK���ŗǂ�
	for( i = 0; gLnClmCnt > i; i++ )
	{
		stLvColm.iSubItem = i;
		ListView_InsertColumn( ghLvItemWnd, i, &stLvColm );
	}

	LineTmpleItemListOn( 0 );	//	���g�ǉ�

	//	���X�g�r���[�c�[���`�b�v
	ghLnLvTipWnd = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, ghTmpleWnd, NULL, hInstance, NULL );
	SetWindowFont( ghLnLvTipWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );

	//	�c�[���`�b�v���R�[���o�b�N�Ŋ���t��
	ZeroMemory( &stToolInfo, sizeof(TTTOOLINFO) );
	stToolInfo.cbSize   = sizeof(TTTOOLINFO);
	stToolInfo.uFlags   = TTF_SUBCLASS;
	stToolInfo.hinst    = NULL;	//	
	stToolInfo.hwnd     = ghLvItemWnd;
	stToolInfo.uId      = IDLV_LT_ITEMVIEW;
	GetClientRect( ghLvItemWnd, &stToolInfo.rect );
	stToolInfo.lpszText = LPSTR_TEXTCALLBACK;	//	�R�����w�肷��ƃR�[���o�b�N�ɂȂ�
	SendMessage( ghLnLvTipWnd, TTM_ADDTOOL, 0, (LPARAM)&stToolInfo );
	SendMessage( ghLnLvTipWnd, TTM_SETMAXTIPWIDTH, 0, 0 );	//	�`�b�v�̕��B�O�ݒ�ł����B���ꂵ�Ƃ��Ȃ��Ɖ��s����Ȃ�


	ShowWindow( ghTmpleWnd, SW_SHOW );
	UpdateWindow( ghTmpleWnd );


	return ghTmpleWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�L���O��ԂŔ����E�h�b�L���O���Ă���e�؊��^�u
	@param[in]	hPrWnd	�e�E�C���h�E�n���h��
	@param[in]	pstRect	�g����T�C�Y�E����̏㕔�ɂ�������
*/
HWND DockingTabCreate( HINSTANCE hInst, HWND hPrWnd, LPRECT pstRect )
{
	HWND	hWorkWnd;
	RECT	itRect;
	TCITEM	stTcItem;

	hWorkWnd = CreateWindowEx( 0, WC_TABCONTROL, TEXT("dockseltab"),
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | TCS_TABS | TCS_SINGLELINE,
		pstRect->left, pstRect->top, pstRect->right, 10, hPrWnd, (HMENU)IDTB_DOCK_TAB, hInst, NULL );	//	TCS_SINGLELINE
	SetWindowFont( hWorkWnd, ghNameFont, FALSE );

	ZeroMemory( &stTcItem, sizeof(stTcItem) );
	stTcItem.mask = TCIF_TEXT;
	stTcItem.pszText = TEXT("��s");	TabCtrl_InsertItem( hWorkWnd, 0, &stTcItem );
	stTcItem.pszText = TEXT("�u���V");	TabCtrl_InsertItem( hWorkWnd, 1, &stTcItem );

	//	�I�΂ꂵ�t�@�C�����^�u�I�ɒǉ��H�@�^�u���̓E�C���h�E��

	TabCtrl_GetItemRect( hWorkWnd, 1, &itRect );
	itRect.bottom  += itRect.top;
	MoveWindow( hWorkWnd, pstRect->left, pstRect->top, pstRect->right, itRect.bottom, TRUE );

	pstRect->top    += itRect.bottom;
	pstRect->bottom -= itRect.bottom;

	return hWorkWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	�h�b�L���O��ԂŔ����E�������Ă�E�C���h�E�����T�C�Y���ꂽ��
	@param[in]	hPrntWnd	�������Ă�E�C���h�E�n���h��
	@param[in]	pstFrame	�g����T�C�Y
*/
VOID LineTmpleResize( HWND hPrntWnd, LPRECT pstFrame )
{
	RECT	rect, tbRect;

	rect = *pstFrame;	//	�N���C�����g�Ɏg����̈�
	rect.left    = rect.right - (grdSplitPos - SPLITBAR_WIDTH);
	rect.right   = (grdSplitPos - SPLITBAR_WIDTH);
	rect.bottom >>= 1;
	rect.top    += rect.bottom;

	GetWindowRect( ghDockTabWnd, &tbRect );
	tbRect.left    = rect.left;
	tbRect.right   = (grdSplitPos - SPLITBAR_WIDTH);
	tbRect.bottom -= tbRect.top;
	tbRect.top     = rect.top;
	MoveWindow( ghDockTabWnd, tbRect.left, tbRect.top, tbRect.right, tbRect.bottom, TRUE );

	rect.top    += tbRect.bottom;
	rect.bottom -= tbRect.bottom;

	SetWindowPos( ghTmpleWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER );

	return;
}
//-------------------------------------------------------------------------------------------------

VOID DockingTabSizeGet( LPRECT pstRect )
{
	ZeroMemory( pstRect, sizeof(RECT) );

	if( ghDockTabWnd )
	{
		GetWindowRect( ghDockTabWnd, pstRect );
		pstRect->right -= pstRect->left;
		pstRect->bottom -= pstRect->top;
	}

	return;
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
LRESULT CALLBACK LineTmpleProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		HANDLE_MSG( hWnd, WM_SIZE,        Ltp_OnSize );	
		HANDLE_MSG( hWnd, WM_COMMAND,     Ltp_OnCommand );	
		HANDLE_MSG( hWnd, WM_NOTIFY,      Ltp_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_CONTEXTMENU, Ltp_OnContextMenu );

		case WM_MOUSEWHEEL:	SendMessage( ghLvItemWnd, WM_MOUSEWHEEL, wParam, lParam );	return 0;

		case WM_CLOSE:	ShowWindow( ghTmpleWnd, SW_HIDE );	return 0;

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
VOID Ltp_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	INT		rslt;
	UINT	dClm;
	LONG_PTR	rdExStyle;

	switch( id )
	{
		case IDCB_LT_CATEGORY:
			if( CBN_SELCHANGE == codeNotify )
			{
				rslt = ComboBox_GetCurSel( ghCtgryBxWnd );
				gNowGroup = rslt;

				LineTmpleItemListOn( rslt );
			}
			break;

		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_LINE,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_LINE, -1 );	break;

		case IDM_TMPL_GRID_INCREASE:
		case IDM_TMPL_GRID_DECREASE:
			dClm = TemplateGridFluctuate( ghLvItemWnd, ((IDM_TMPL_GRID_INCREASE == id) ? 1 : -1) );
			if( dClm )
			{
				gLnClmCnt = dClm;
				LineTmpleItemListOn( gNowGroup );
				InitParamValue( INIT_SAVE, VL_LINETMP_CLM, gLnClmCnt );
			}
			break;

		case IDM_TMPLT_GROUP_PREV:
			if( 0 < gNowGroup )
			{
				gNowGroup--;
				ComboBox_SetCurSel( ghCtgryBxWnd, gNowGroup );
				LineTmpleItemListOn( gNowGroup );
			}
			break;

		case IDM_TMPLT_GROUP_NEXT:
			if( (gNowGroup + 1) < gvcTmples.size() )
			{
				gNowGroup++;
				ComboBox_SetCurSel( ghCtgryBxWnd, gNowGroup );
				LineTmpleItemListOn( gNowGroup );
			}
			break;

		case IDM_TOPMOST_TOGGLE:	//	�펞�őS�ʂƒʏ�E�C���h�E�̃g�O��
			rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )
			{
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_LNTMPL, 0 );
			}
			else
			{
				SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
				InitWindowTopMost( INIT_SAVE, WDP_LNTMPL, 1 );
			}
			break;

		case IDM_TMPLGROUPSTYLE_TGL:
			break;


		default:	break;
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�T�C�Y�ύX���ꂽ
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	state	�Ȃɂ��̏��
	@param[in]	cx		�ύX���ꂽ�N���C�����g��
	@param[in]	cy		�ύX���ꂽ�N���C�����g����
	@return		�Ȃ�
*/
VOID Ltp_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	LONG	width;
	UINT	i;
	RECT	cbxRect, rect;

	MoveWindow( ghCtgryBxWnd, 0, 0, cx, 127, TRUE );
	GetClientRect( ghCtgryBxWnd, &cbxRect );

	MoveWindow( ghLvItemWnd, 0, cbxRect.bottom, cx, cy - cbxRect.bottom, TRUE );

	GetClientRect( ghLvItemWnd, &rect );
	width = rect.right / gLnClmCnt;

	for( i = 0; gLnClmCnt > i; i++ )
	{
		ListView_SetColumnWidth( ghLvItemWnd, i, width );
	}

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
LRESULT Ltp_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	HWND	hLvWnd;
	INT		iPos, iItem, nmCode, iSubItem;
	INT_PTR	items;
	TCHAR	atItem[SUB_STRING];
	LPNMLISTVIEW	pstLv;
	LVHITTESTINFO	stHitTestInfo;

	if( IDLV_LT_ITEMVIEW == idFrom )
	{
		pstLv = (LPNMLISTVIEW)pstNmhdr;

		hLvWnd = pstLv->hdr.hwndFrom;
		nmCode = pstLv->hdr.code;

		//	���ʂ̃N���b�N�ɂ���
		if( NM_CLICK == nmCode )
		{
			stHitTestInfo.pt = pstLv->ptAction;
			ListView_SubItemHitTest( hLvWnd, &stHitTestInfo );

			iItem = stHitTestInfo.iItem;
			iSubItem = stHitTestInfo.iSubItem;
			iPos = iItem * gLnClmCnt + iSubItem;

			if( 0 < gvcTmples.size() )
			{
				items = gvcTmples.at( gNowGroup ).vcItems.size( );

				TRACE( TEXT("LINE TMPL[%d x %d]"), iItem, iSubItem );

				if( 0 <= iPos && iPos <  items )	//	�Ȃ񂩑I������
				{
					StringCchCopy( atItem, SUB_STRING, gvcTmples.at( gNowGroup ).vcItems.at( iPos ).c_str( ) );
					ViewInsertTmpleString( atItem );	//	�}������
					ViewFocusSet(  );	//	20110720	�t�H�[�J�X��`��ɖ߂�
				}
			}
			else
			{
				ViewFocusSet(  );	//	20110720	�t�H�[�J�X��`��ɖ߂�
			}
		}
	}
	

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
VOID Ltp_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	LONG_PTR	rdExStyle;

	POINT	stPoint;

	//	��̉��Ȃ���ɏo�����m�͂Ȃ�
	if( gbTmpltDock )	return;

	stPoint.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPoint.y = (SHORT)yPos;

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_TEMPLATE_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );
	AppendMenu( hSubMenu, MF_STRING, IDM_TMPLGROUPSTYLE_TGL, TEXT("�J�e�S���\���ؑ�") );

	rdExStyle = GetWindowLongPtr( hWnd, GWL_EXSTYLE );
	if( WS_EX_TOPMOST & rdExStyle ){	CheckMenuItem( hSubMenu , IDM_TOPMOST_TOGGLE, MF_BYCOMMAND | MF_CHECKED );	}

	dRslt = TrackPopupMenu( hSubMenu, 0, stPoint.x, stPoint.y, 0, hWnd, NULL );
	//	�I�������łO���|�P�H�A�I�������炻�̃��j���[�̂h�c��WM_COMMAND�����s
	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Z�b�g���̂ƁA���e���i�ǂ������̂ݗL���j����s�Â󂯎��
	@param[in]	ptName	�Z�b�g���́E�����Ȃ�NULL�E�������̑��ݗD��
	@param[in]	ptLine	���ڂ̓��e�E�����Ȃ�NULL�E����NULL�Ŗ��[����
	@param[in]	cchSize	�ǂ������̓��e�̕�����
	@return	UINT	���ɈӖ��͂Ȃ�
*/
UINT CALLBACK LineTmpleItemData( LPTSTR ptName, LPTSTR ptLine, INT cchSize )
{
//	����NULL��������A�{�̂ɒǉ�����������
	static AATEMPLATE	cstItem;


	if( ptName )	//	�Z�b�g���́E�J�n�ł�����
	{
		StringCchCopy( cstItem.atCtgryName, SUB_STRING, ptName );
		cstItem.vcItems.clear(  );
	}
	else if( ptLine )	//	�{�̃f�[�^
	{
		cstItem.vcItems.push_back( wstring( ptLine ) );
	}
	else	//	�ǂ�����NULL�A�Z�b�g�I��
	{
		gvcTmples.push_back( cstItem );
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����R���{�b�N�X�ƃ��X�g�ɓW�J
	@param[in]	listNum	�W�J����Z�b�g�ԍ��E�O�C���f�b�N�X
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT LineTmpleItemListOn( UINT listNum )
{
	INT			width;
	UINT_PTR	i, items;
	TCHAR		atItem[SUB_STRING];
	LVITEM		stLvi;
	RECT		rect;

	ZeroMemory( atItem, sizeof(atItem) );

	if( 0 >= gvcTmples.size() ){	return E_OUTOFMEMORY;	}

	items = gvcTmples.at( listNum ).vcItems.size( );

	TRACE( TEXT("LINE open NUM[%u] ITEM[%u] GRID[%d]"), listNum, items, gLnClmCnt );

	ListView_DeleteAllItems( ghLvItemWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask    = LVIF_TEXT;
	stLvi.pszText = atItem;

	for( i = 0; items > i; i++ )
	{
		StringCchCopy( atItem, SUB_STRING, gvcTmples.at( listNum ).vcItems.at( i ).c_str( ) );

		stLvi.iItem    = i / gLnClmCnt;
		stLvi.iSubItem = i % gLnClmCnt;
		if( 0 == stLvi.iSubItem )	ListView_InsertItem( ghLvItemWnd, &stLvi );
		else						ListView_SetItem( ghLvItemWnd, &stLvi );
	}

	//	�u�`���񂾂畝����
	GetClientRect( ghLvItemWnd, &rect );
	width = rect.right / gLnClmCnt;
	for( i = 0; gLnClmCnt > i; i++ ){	ListView_SetColumnWidth( ghLvItemWnd, i, width );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------



/*!
	�t�@�C�����J���ăf�[�^�m�ۊJ�n
	@param[in]	ptFileName	�J���t�@�C���̖��O
	@param[in]	pfCalling	�󂯎�����f�[�^���������锟���ւ̃|�C���^�[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TemplateItemLoad( LPTSTR ptFileName, PAGELOAD pfCalling )
{
	CONST WCHAR rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_
	WCHAR	rtUniBuf;

	HANDLE	hFile;
	DWORD	readed;

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize;

	LPTSTR	ptString;
	LPSTR	pcText;
	UINT	cchSize;

	TCHAR	atFileName[MAX_PATH];

	StringCchCopy( atFileName, MAX_PATH, ExePathGet() );
	PathAppend( atFileName, TEMPLATE_DIR );
	PathAppend( atFileName, ptFileName );

	hFile = CreateFile( atFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return E_HANDLE;	}

	iByteSize = GetFileSize( hFile, NULL );
	pBuffer = malloc( iByteSize + 2 );
	ZeroMemory( pBuffer, iByteSize + 2 );

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pBuffer, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	//	���j�R�[�h�`�F�b�N
	CopyMemory( &rtUniBuf, pBuffer, 2 );
	if( rtHead == rtUniBuf )	//	���j�R�[�h�w�b�_�������
	{	//	���ʂ̓��j�R�[�h����Ȃ�
		ptString = (LPTSTR)pBuffer;
		ptString++;	//	���j�R�[�h�w�b�_���i�߂Ă���
	}
	else	//	����SJIS�ł���Ȃ�
	{
		pcText = (LPSTR)pBuffer;
		ptString = SjisDecodeAlloc( pcText );	//	SJIS�̓��e�����j�R�[�h�ɂ���

		FREE( pBuffer );	//	�������ŊJ��
		pBuffer = ptString;	//	�|�C���g����Ƃ����ύX
	}

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	TemplateItemSplit( ptString, cchSize, pfCalling );

	FREE( pBuffer );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	ListName�ȃ��j�R�[�h��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	ptStr		����Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize		���̕�����̕�����
	@param[in]	pfCalling	�󂯎�����f�[�^���������锟���ւ̃|�C���^�[
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT TemplateItemSplit( LPTSTR ptStr, UINT cchSize, PAGELOAD pfCalling )
{
	LPTSTR	ptCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPTSTR	ptStart;	//	�Z�p���[�^�̒��O
	LPTSTR	ptEnd;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
	UINT	cchItem;
//	INT		dmyX = 0, dmyY = 0;
	BOOLEAN	bLast;
	TCHAR	atName[MAX_PATH];

	ptCaret = ptStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	bLast = FALSE;

	//	�ŏ���[ListName=�܂ňړ�
	ptEnd = StrStr( ptCaret, TMPLE_BEGINW );
	if( !ptEnd )	return E_INVALIDARG;	//	�t�@�C���̒��g���Ⴄ

	ptCaret = ptEnd;

	do	//	
	{
		ptStart = NextLine( ptCaret );	//	���̍s���炪�{��
		if( !ptStart )	return  S_FALSE;	//	������Ȃ�������

		ptEnd = StrStr( ptCaret, TEXT("=") );	//	=
		ptCaret = ptEnd + 1;	//	���O�̏��܂ł�����
		cchItem = ptStart - ptCaret;	//	���O�����̕�����
		cchItem -= 3;	//	]rn

		ZeroMemory( atName, sizeof(atName) );	//	���O�m��
		if( 0 < cchItem ){	StringCchCopyN( atName, MAX_PATH, ptCaret, cchItem );	}
		else{	StringCchPrintf( atName, MAX_PATH, TEXT("Nameless%d"), iNumber );	}

		StringCchLength( atName, MAX_PATH, &cchItem );
		pfCalling( atName, NULL, cchItem );

		ptCaret = ptStart;	//	�{�̕���

		ptEnd = StrStr( ptCaret, TMPLE_ENDW );	//	�I�[�Z�p���[�^��T��
		//	���̎��_��ptEnd��[end]�������Ă�ENULL�͂��肦�Ȃ�
		if( !ptEnd ){	return  S_FALSE;	}	//	������Ȃ�������

		cchItem = ptEnd - ptCaret;	//	WCHAR�P�ʂȂ̂Ōv�Z���ʂ͕������̂悤��

		TemplateItemScatter( ptCaret, cchItem, pfCalling );

		pfCalling( NULL, NULL, 0 );

		iNumber++;

		ptCaret = NextLine( ptEnd );	//	���̍s�����̊J�n�n�_

	}while( *ptCaret );	//	�f�[�^�L����胋�[�v�ŒT��



	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e�����o�����ĂԂ�����
	@param[in]	ptCont		���ڂ̓��e
	@param[in]	cchSize		���e�̕�����
	@param[in]	pfCalling	�󂯎�����f�[�^���������锟���ւ̃|�C���^�[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT TemplateItemScatter( LPTSTR ptCont, INT cchSize, PAGELOAD pfCalling )
{
	//	���s�ŋ�؂�ꂽ��s�P�ʂ̃A�C�e���ł���
	INT	nowCaret, nYct, nXct, rtcnt;

	TCHAR	hdBuf[MAX_STRING];	//	�f�[�^���m��

	ZeroMemory( hdBuf, sizeof(hdBuf) );	//	�N�����i�b�v

	nowCaret = 0;
	rtcnt = 0;	//	���s�J�E���g�E����˂��H
	//�X�^�C���b�V���ɖ{�̓Ǎ�
	for( nYct = 0, nXct = 0; nowCaret <= cchSize; nowCaret++, nXct++ )
	{
		if( nXct >= MAX_STRING )	nXct = MAX_STRING - 1;

		hdBuf[nXct] = ptCont[nowCaret];

		if( ( TEXT('\r') == ptCont[nowCaret] && TEXT('\n') == ptCont[nowCaret + 1]) || 0x0000 == ptCont[nowCaret] )
		{
			if( 0 == nXct )	continue;	//	���X�g���s����Ȃ炱���ɓ���E�����񖳂��Ȃ牽�����Ȃ�
			hdBuf[nXct] = 0x0000;	//	������̏I�_�̓k��
			//	�����Ńf�[�^����
			pfCalling( NULL, hdBuf, nXct );

			nXct = -1;	//	���[�v���Ł{�{����邽�߁A�O�ɂ����悤�ɂ��Ă���
			nYct = 0;	//	���̃G�������g�ɃX�^�C���b�V���Ɉڂ�
			nowCaret++;	//	���s�R�[�h�����G���K���g�ɐi�߂�

			rtcnt++;
			ZeroMemory( hdBuf, sizeof(hdBuf) );	//	�N�����i�b�v
		}
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�i�q�̐��𑝌�������
	@param[in]	hLvWnd	�Ώۂ̃��X�g�r���[�̃n���h��
	@param[in]	dFluct	���F���₷�@���F���炷
	@return	�O�F���얳���@�P�`�F���̐��ɂȂ���
*/
UINT TemplateGridFluctuate( HWND hLvWnd, INT dFluct )
{
	INT	clmCount, clmNew, i;
	LVCOLUMN	stLvColm;

	if( 0 == dFluct )	return 0;

	//	�J�����̐��́A���X�g�r���[�̃w�b�_�[����Q�b�g���Ȃ�����
	clmCount = Header_GetItemCount( ListView_GetHeader(hLvWnd) );

	TRACE( TEXT("�J��������[%u][%d]"), clmCount, dFluct );

	if( 0 > dFluct && 1 >= clmCount )	return 0;	//	�P��茸�点�Ȃ�

	clmNew =  clmCount + dFluct;	//	�J��������������

	if( 0 > dFluct )
	{
		for( i = clmCount; clmNew < i; i-- )
		{
			ListView_DeleteColumn( hLvWnd, (i-1) );
		}
	}

	if( 0 < dFluct )
	{
		ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
		stLvColm.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		stLvColm.fmt      = LVCFMT_LEFT;
		stLvColm.pszText  = TEXT("Item");
		stLvColm.cx       = 10;	//	���ƂŒ�������̂œK����
		for( i = clmCount; clmNew > i; i++ )
		{
			stLvColm.iSubItem = i;
			ListView_InsertColumn( hLvWnd, i, &stLvColm );
		}
	}

	return clmNew;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J�e�S���R���{�b�N�X�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�Ȃ񂩏�����������
*/
LRESULT CALLBACK gpfLineCtgryProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT	id;

	switch( msg )
	{
		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDM_WINDOW_CHANGE:
				case IDM_WINDOW_CHG_RVRS:
				case IDM_TMPL_GRID_INCREASE:
				case IDM_TMPL_GRID_DECREASE:
				case IDM_TMPLT_GROUP_PREV:
				case IDM_TMPLT_GROUP_NEXT:
					SendMessage( ghTmpleWnd, WM_COMMAND, wParam, lParam );
					return 0;
			}
			break;

		case WM_MOUSEWHEEL:
			SendMessage( ghLvItemWnd, WM_MOUSEWHEEL, wParam, lParam );
			return 0;

	}

	return CallWindowProc( gpfOrigLineCtgryProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����X�g�r���[�T�u�N���X
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	msg		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@return	�Ȃ񂩏�����������
*/
LRESULT CALLBACK gpfLineItemProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	INT	id;

	switch( msg )
	{
		HANDLE_MSG( hWnd, WM_NOTIFY, Ltl_OnNotify );	//	�R�����R���g���[���̌ʃC�x���g

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
				case IDM_WINDOW_CHANGE:
				case IDM_WINDOW_CHG_RVRS:
				case IDM_TMPL_GRID_INCREASE:
				case IDM_TMPL_GRID_DECREASE:
				case IDM_TMPLT_GROUP_PREV:
				case IDM_TMPLT_GROUP_NEXT:
					SendMessage( ghTmpleWnd, WM_COMMAND, wParam, lParam );
					return 0;
			}
			break;
	}

	return CallWindowProc( gpfOrigLineItemProc, hWnd, msg, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------


/*!
	�m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Ltl_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	HWND	hLvWnd;
	INT		iPos, iItem, nmCode, iSubItem, iDot;
	INT_PTR	items;
	TCHAR	atItem[SUB_STRING];
	LVHITTESTINFO	stHitTestInfo;
	LPNMLISTVIEW	pstLv;
	LPNMTTDISPINFO	pstDispInfo;

	pstLv = (LPNMLISTVIEW)pstNmhdr;

	//	���X�g�r���[���̂̃v���V�[�W���Ȃ̂�
	hLvWnd = hWnd;//pstLv->hdr.hwndFrom;
	nmCode = pstLv->hdr.code;

	if( TTN_GETDISPINFO == nmCode )
	{
		ZeroMemory( &stHitTestInfo, sizeof(LVHITTESTINFO) );
		GetCursorPos( &(stHitTestInfo.pt) );
		ScreenToClient( hLvWnd, &(stHitTestInfo.pt) );
		ListView_SubItemHitTest( hLvWnd, &stHitTestInfo );

		iItem = stHitTestInfo.iItem;
		iSubItem = stHitTestInfo.iSubItem;
		iPos = iItem * gLnClmCnt + iSubItem;

		TRACE( TEXT("LLvTT[%d]"), iPos );

		pstDispInfo = (LPNMTTDISPINFO)pstNmhdr;

		ZeroMemory( &(pstDispInfo->szText), sizeof(pstDispInfo->szText) );
		pstDispInfo->lpszText = pstDispInfo->szText;

		if( 0 < gvcTmples.size( ) )
		{
			items = gvcTmples.at( gNowGroup ).vcItems.size( );

			if( 0 <= iPos && iPos <  items )	//	�Ȃ񂩑I������
			{
				StringCchCopy( atItem, SUB_STRING, gvcTmples.at( gNowGroup ).vcItems.at( iPos ).c_str( ) );
				iDot = ViewStringWidthGet( atItem );

				StringCchPrintf( pstDispInfo->szText, 80, TEXT("%s [%d Dot]"), atItem, iDot );
			}
		}

		return 0;
	}

	return CallWindowProc( gpfOrigLineItemProc, hWnd, WM_NOTIFY, (WPARAM)idFrom, (LPARAM)pstNmhdr );
}
//-------------------------------------------------------------------------------------------------

