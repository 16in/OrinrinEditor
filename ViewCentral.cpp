/*! @file
	@brief �ҏW�r���[�̊Ǘ��Ƃ��`�悵�܂�
	���̃t�@�C���� ViewCentral.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/15
*/

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

//	
//	�ҏW�E�C���h�E�̐��䂷��E�^�u�Ƃ����H

//-------------------------------------------------------------------------------------------------

#define EDIT_VIEW_CLASS	TEXT("EDIT_VIEW")
//-------------------------------------------------------------------------------------------------

/*

�`��p������̎󂯎��
�������̂ƁA�`��w��R�[�h�ɂ��\���̔z��ł��Ƃ肷��Ƃ�

�y���Ɣw�i�͓s�x�ύX�E�O��Ɠ����Ȃ����ւ��s�v��

��ʃT�C�Y����Ɉӎ�����

�`�攟���̓J�v�Z�������Ă����B�`�攟���́A������̃f�[�^���󂯎���āA
�����F�Ƃ��w�i�̂�Ńe�L�X�g�����āA�F�ƕ�����œn���΂���
���[���[�ƍs�ԍ��̕`��g�̕������炷�͕̂`��J�v�Z�������̒��ł��΂���
�J�[�\���ʒu�Ƃ��������E�`��̈挴�_�ňӎ�����΂���

�㕔���[���[�ɔ��s���A�s�ԍ��\���ɂR�����ƁA�󔒁A�P�h�b�g���g��

�e�`�攟���ƁA�c�����n�������󂯎��w�x�l�́A���e�����S�ɕ\�������Ƃ��́A
�h�L�������g���̂̐�Έʒu�E���[���[�Ƃ��A�X�N���[���͍l������Ă��Ȃ�

�e�`�攟�����ŁA�`��ʒu�V�t�g����������ŁA�`��ʒu���g�����X�t�H�[������

*/

static HINSTANCE	ghInst;		//!<	���݂̃C���^�[�t�F�C�X

EXTERNED HWND	ghPrntWnd;		//!<	���C���E�C���h�E�n���h��
EXTERNED HWND	ghViewWnd;		//!<	���̃E�C���h�E�̃n���h��

extern  HWND	ghPgVwWnd;		//		�y�[�W���X�g
extern  HWND	ghMaaWnd;		//!<	�����s�`�`�e���v��
extern  HWND	ghLnTmplWnd;	//!<	��s�e���v��
extern  HWND	ghBrTmplWnd;	//!<	�u���V�e���v��

EXTERNED INT	gdXmemory;		//!<	���O�̂w�ʒu���o���Ă���
EXTERNED INT	gdDocXdot;		//!<	�L�����b�g�̂w�h�b�g�E�h�L�������g�ʒu
EXTERNED INT	gdDocLine;		//!<	�L�����b�g�̂x�s���E�h�L�������g�ʒu
EXTERNED INT	gdDocMozi;		//!<	�L�����b�g�̍����̕�����

//	��ʃT�C�Y���m�F���āA�ړ��ɂ��X�N���[���̖ʓ|�݂�
EXTERNED INT	gdHideXdot;		//!<	���̉B�ꕔ��
EXTERNED INT	gdViewTopLine;	//!<	�\�����̍ŏ㕔�s�ԍ�
EXTERNED SIZE	gstViewArea;	//!<	�\���̈�̃h�b�g�T�C�Y�E���[���[���̗̈�͖���
EXTERNED INT	gdDispingLine;	//!<	�����Ă�s���E���r���[�Ɍ����Ă閖�[�͊܂܂Ȃ�



EXTERNED BOOLEAN	gbExtract;	//!<	���o���[�h��

//	�t�H���g�͕`�斈�Ƀf�o�C�X�R���e�L�X�g�Ɋ���t����K�v������
EXTERNED HFONT	ghAaFont;		//!<	AA�p�t�H���g
static HFONT	ghRulerFont;	//!<	���[���[�p�t�H���g
static HFONT	ghNumFont4L;	//!<	�s�ԍ��p�t�H���g�S���p
static HFONT	ghNumFont5L;	//!<	�s�ԍ��p�t�H���g�T���p
static HFONT	ghNumFont6L;	//!<	�s�ԍ��p�t�H���g�U���p


static INT		gdAutoDiffBase;	//!<	���������̃x�[�X

static  UINT	gdUseMode;		//!<	�}�����C���N���b�v�w��

static  UINT	gdSpaceView;	//!<	�󔒂�\������

EXTERNED UINT	gdGridXpos;		//!<	�O���b�h���̂w�Ԋu
EXTERNED UINT	gdGridYpos;		//!<	�O���b�h���̂x�Ԋu
EXTERNED UINT	gdRightRuler;	//!<	�E���̈ʒu

static BOOLEAN	gbGridView;		//!<	�O���b�h�\�����邩
static BOOLEAN	gbRitRlrView;	//!<	�E���\�����邩

static  UINT	gdWheelLine;	//!<	�}�E�X�z�E�B�[���̍s�ړ��ʂ�OS�W��

extern  UINT	gbSqSelect;		//		��`�I�𒆂ł���
extern  UINT	gbBrushMode;	//		�u���V���ł���

extern INT		gixFocusPage;	//	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern INT		gbTmpltDock;	//	�y�[�W���̃h�b�L���O

//@@�R�s�[����
extern  UINT	gbCpModSwap;	//	SJIS�ƃ��j�R�[�h�R�s�[�����ւ���
//-------------------------------------------------------------------------------------------------

//	�g�p����F

/*
�F���m�ۂ��锟��
ViewMoziColourGet
ViewBackColourGet
�p�ӂ��Ă����E�������H
*/

//	�F
static COLORREF	gaColourTable[] = { 
	0x000000,
	0xFFFFFF, 0xABABAB, 0x0000FF, 0xAAAAAA, 0x000000,
	0xFFFFFF, 0x8080FF, 0xC0C000, 0xC0C000, 0x101010,
	0xEEEEEE, 0xFFCCCC, 0xFF0000, 0xE0E0E0, 0x00FFFF
};

#define CLRT_BASICPEN	0	//	��{�����F
#define CLRT_BASICBK	1	//	��{�w�i�F
#define CLRT_SELECTBK	2	//	�I����Ԃ̔w�i�F
#define CLRT_SPACEWARN	3	//	�A���󔒌x���F
#define CLRT_SPACELINE	4	//	�X�y�[�X�̐F
#define CLRT_CARETFD	5	//	�L�����b�g�F
#define CLRT_CARETBK	6	//	�L�����b�g�w�i
#define CLRT_LASTSPWARN	7	//	�s�[�󔒂̌x���F
#define CLRT_CRLF_MARK	8	//	���s�̐F
#define CLRT_EOF_MARK	9	//	�d�n�e�̐F
#define CLRT_RULER		10	//	���[���̐F
#define CLRT_RULERBK	11	//	���[���̔w�i�F
#define CLRT_CANTSJIS	12	//	��SJIS�����̔w�i�F
#define CLRT_CARET_POS	13	//	���[���[��CARET�\���F
#define CLRT_GRID_LINE	14	//	�O���b�h���̐F
#define CLRT_FINDBACK	15	//	�����q�b�g������̔w�i�F

//	�y��
#define PENS_MAX	6
static  HPEN	gahPen[PENS_MAX];
#define PENT_CRLF_MARK	0
#define PENT_RULER		1
#define PENT_SPACEWARN	2
#define PENT_SPACELINE	3
#define PENT_CARET_POS	4
#define PENT_GRID_LINE	5

//	�u���V
#define BRUSHS_MAX	6
static  HBRUSH	gahBrush[BRUSHS_MAX];
#define BRHT_BASICBK	0
#define BRHT_RULERBK	1
#define BRHT_SELECTBK	2
#define BRHT_LASTSPWARN	3
#define BRHT_CANTSJISBK	4
#define BRHT_FINDBACK	5

//-------------------------------------------------------------------------------------------------

static LOGFONT	gstBaseFont = {
	FONTSZ_NORMAL,			//	�t�H���g�̍���
	0,						//	���ϕ�
	0,						//	��������̕�����X���Ƃ̊p�x
	0,						//	�x�[�X���C����X���Ƃ̊p�x
	FW_NORMAL,				//	�����̑���(0~1000�܂ŁE400=nomal)
	FALSE,					//	�C�^���b�N��
	FALSE,					//	�A���_�[���C��
	FALSE,					//	�ł�������
	DEFAULT_CHARSET,		//	�����Z�b�g
	OUT_OUTLINE_PRECIS,		//	�o�͐��x
	CLIP_DEFAULT_PRECIS,	//	�N���b�s���O���x
	PROOF_QUALITY,			//	�o�͕i��
	VARIABLE_PITCH,			//	�Œ蕝���ϕ�
	TEXT("�l�r �o�S�V�b�N")	//	�t�H���g��
};


//-------------------------------------------------------------------------------------------------

LRESULT	CALLBACK ViewWndProc( HWND, UINT, WPARAM, LPARAM );
BOOLEAN	Evw_OnCreate( HWND, LPCREATESTRUCT );		//!<	WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Evw_OnCommand( HWND , INT, HWND, UINT );	//!<	WM_COMMAND �̏���
VOID	Evw_OnPaint( HWND );						//!<	WM_PAINT �̏����E�g���`��Ƃ�
VOID	Evw_OnDestroy( HWND );						//!<	WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
VOID	Evw_OnVScroll( HWND, HWND, UINT, INT );
VOID	Evw_OnHScroll( HWND, HWND, UINT, INT );
VOID	Evw_OnContextMenu( HWND, HWND, UINT, UINT );


//	�e�E�C���h�E����񂷕K�v���L��
//VOID	Evw_OnKey( HWND, UINT, BOOL, INT, UINT );	//!<	
//VOID	Evw_OnChar( HWND, TCHAR, INT );				//!<	

HRESULT	ViewScrollBarAdjust( LPVOID );

HRESULT	ViewRedrawDo( HWND, HDC );
HRESULT	ViewDrawMetricLine( HDC,UINT );
BOOLEAN	ViewDrawTextOut( HDC, INT, UINT, LPLETTER, UINT_PTR );
BOOLEAN	ViewDrawSpace( HDC, INT, UINT, LPTSTR, UINT_PTR, UINT );
HRESULT	ViewDrawReturnMark( HDC, INT, INT, UINT );
INT		ViewDrawEOFMark( HDC, INT, INT, UINT );

HRESULT	ViewDrawRuler( HDC );
HRESULT	ViewDrawLineNumber( HDC );

VOID	OperationUndoRedo( INT, PINT, PINT );
//-------------------------------------------------------------------------------------------------

VOID AaFontCreate( UINT bMode )
{
	if( bMode )	ghAaFont = CreateFontIndirect( &gstBaseFont );
	else		DeleteFont( ghAaFont );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[�E�C���h�E�̍쐬
	@param[in]	hInstance	�A�v���̃C���X�^���X
	@param[in]	hParentWnd	�e�E�C���h�E�̃n���h��
	@param[in]	pstFrame	�e�E�C���h�E�̃N���C�����g�T�C�Y
	@param[in]	ptArgv		�R�}���h���C���œn���ꂽ�t�@�C�����E������΂O�N������ԁENULL�ł͂Ȃ�
	@return		������r���[�̃E�C���h�E�n���h��
*/
HWND ViewInitialise( HINSTANCE hInstance, HWND hParentWnd, LPRECT pstFrame, LPTSTR ptArgv )
{
	TCHAR	atFile[MAX_PATH];
	WNDCLASSEX	wcex;
	RECT	vwRect, rect;

	LOGFONT	stFont;

	INT		iNewPage;
#ifdef MULTI_FILE
	INT		iFiles, i;
	LPARAM	dNumber;
	BOOLEAN	bOpen = FALSE;
#endif

	ghInst = hInstance;

	//	�}�E�X�z�E�B�[���̍s�ړ��ʂ̂n�r�ݒ���Q�b�c�I
	gdWheelLine = 0;
	SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &gdWheelLine, 0 );
	if( 0 == gdWheelLine )	gdWheelLine = 3;	//	�f�t�H�͂R

	//	�f�t�H�F�L��Ȃ炻����w��
	gaColourTable[CLRT_SELECTBK] = GetSysColor( COLOR_HIGHLIGHT );

	//	������Ԃ͂����ŏ㏑������΂���
	gaColourTable[CLRT_BASICPEN]  = InitColourValue( INIT_LOAD, CLRV_BASICPEN, gaColourTable[CLRT_BASICPEN] );
	gaColourTable[CLRT_BASICBK]   = InitColourValue( INIT_LOAD, CLRV_BASICBK,  gaColourTable[CLRT_BASICBK] );
	gaColourTable[CLRT_GRID_LINE] = InitColourValue( INIT_LOAD, CLRV_GRIDLINE, gaColourTable[CLRT_GRID_LINE] );
	gaColourTable[CLRT_CRLF_MARK] = InitColourValue( INIT_LOAD, CLRV_CRLFMARK, gaColourTable[CLRT_CRLF_MARK] );
	gaColourTable[CLRT_CANTSJIS]  = InitColourValue( INIT_LOAD, CLRV_CANTSJIS, gaColourTable[CLRT_CANTSJIS] );

	//	�w�i�F�쐬
	gahBrush[BRHT_BASICBK]    = CreateSolidBrush( gaColourTable[CLRT_BASICBK] );
	gahBrush[BRHT_RULERBK]    = CreateSolidBrush( gaColourTable[CLRT_RULERBK] );
	gahBrush[BRHT_SELECTBK]   = CreateSolidBrush( gaColourTable[CLRT_SELECTBK] );
	gahBrush[BRHT_LASTSPWARN] = CreateSolidBrush( gaColourTable[CLRT_SPACEWARN] );
	gahBrush[BRHT_CANTSJISBK] = CreateSolidBrush( gaColourTable[CLRT_CANTSJIS] );
	gahBrush[BRHT_FINDBACK]   = CreateSolidBrush( gaColourTable[CLRT_FINDBACK] );

	//	�y�����쐬
	gahPen[PENT_CRLF_MARK] = CreatePen( PS_SOLID, 1, gaColourTable[CLRT_CRLF_MARK] );
	gahPen[PENT_RULER]     = CreatePen( PS_SOLID, 1, gaColourTable[CLRT_RULER] );
	gahPen[PENT_SPACEWARN] = CreatePen( PS_SOLID, 1, gaColourTable[CLRT_SPACEWARN] );
	gahPen[PENT_SPACELINE] = CreatePen( PS_SOLID, 1, gaColourTable[CLRT_SPACELINE] );
	gahPen[PENT_CARET_POS] = CreatePen( PS_SOLID, 1, gaColourTable[CLRT_CARET_POS] );
	gahPen[PENT_GRID_LINE] = CreatePen( PS_SOLID, 1, gaColourTable[CLRT_GRID_LINE] );

	//	��p�̃E�C���h�E�N���X�쐬
	ZeroMemory( &wcex, sizeof(WNDCLASSEX) );
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= ViewWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_IBEAM);
	wcex.hbrBackground	= gahBrush[BRHT_BASICBK];
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= EDIT_VIEW_CLASS;
	wcex.hIconSm		= NULL;

	RegisterClassEx( &wcex );

	gdHideXdot = 0;
	gdViewTopLine  = 0;

	gdAutoDiffBase = 0;

	//	�O���b�h���\������
	gdGridXpos   = InitParamValue( INIT_LOAD, VL_GRID_X_POS, 54 );
	gdGridYpos   = InitParamValue( INIT_LOAD, VL_GRID_Y_POS, 54 );
	gbGridView   = InitParamValue( INIT_LOAD, VL_GRID_VIEW, 0 );
	MenuItemCheckOnOff( IDM_GRID_VIEW_TOGGLE, gbGridView );

	//	�E���[���\������
	gdRightRuler = InitParamValue( INIT_LOAD, VL_R_RULER_POS, 800 );
	gbRitRlrView = InitParamValue( INIT_LOAD, VL_R_RULER_VIEW, 1 );
	MenuItemCheckOnOff( IDM_RIGHT_RULER_TOGGLE, gbRitRlrView );

	//	�󔒕\������
	gdSpaceView = TRUE;//InitParamValue( INIT_LOAD, VL_SPACE_VIEW, 1 );
	MenuItemCheckOnOff( IDM_SPACE_VIEW_TOGGLE, gdSpaceView );
	OperationOnStatusBar(  );

	ghPrntWnd =  hParentWnd;	//	�e�E�C���h�E�n���h���L�^

	rect = *pstFrame;
	if( gbTmpltDock ){	rect.right -= PLIST_DOCK;	};

	ghViewWnd = CreateWindowEx( 0, EDIT_VIEW_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
		rect.left, rect.top, rect.right, rect.bottom, hParentWnd, NULL, hInstance, NULL);

	if( !ghViewWnd ){	return NULL;	}


	stFont = gstBaseFont;
	stFont.lfPitchAndFamily = FIXED_PITCH;
	StringCchCopy( stFont.lfFaceName, LF_FACESIZE, TEXT("�l�r �S�V�b�N") );
	ghNumFont4L = CreateFontIndirect( &stFont );

	stFont.lfHeight = 13;
	ghNumFont5L = CreateFontIndirect( &stFont );

	stFont.lfHeight = 11;
	ghNumFont6L = CreateFontIndirect( &stFont );

	stFont.lfHeight = FONTSZ_REDUCE;
	stFont.lfPitchAndFamily = VARIABLE_PITCH;
	StringCchCopy( stFont.lfFaceName, LF_FACESIZE, TEXT("MS UI Gothic") );
	ghRulerFont = CreateFontIndirect( &stFont );
//	ghRulerFont = CreateFont( FONTSZ_REDUCE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("MS UI Gothic") );

	GetClientRect( ghViewWnd, &vwRect );	//	�X�N���[���o�[�͊܂�łȂ�
	gstViewArea.cx = vwRect.right - LINENUM_WID;
	gstViewArea.cy = vwRect.bottom - RULER_AREA;

	gdDispingLine = gstViewArea.cy / LINE_HEIGHT;

	DocInitialise( NULL );

	ZeroMemory( atFile, sizeof(atFile) );

#ifdef MULTI_FILE
	bOpen = FALSE;
	//	INI�̃��X�g�I�[�|���L�^���m�F
	iFiles = InitMultiFileTabOpen( INIT_LOAD, -1, NULL );
	for( i = 0; iFiles >= i; i++ )	//	�R�}���h���C���I�[�|�����Ď�
	{
		//	�Ō�ɃR�}���h���C���I�[�|�����m���߂�
		if( iFiles == i ){	StringCchCopy( atFile, MAX_PATH, ptArgv );	}
		else{				InitMultiFileTabOpen( INIT_LOAD, i, atFile );	}

		dNumber = DocFileInflate( atFile  );
		if( 0 < dNumber )	//	�L���Ȃ�
		{
			if( !(bOpen) )	//	�ŏ��̂�����
			{
				MultiFileTabFirst( atFile );
				bOpen = TRUE;
			}
			else
			{
				MultiFileTabAppend( dNumber, atFile );
			}
			AppTitleChange( atFile );
		}
	}

	if( !(bOpen) )	//	���S�ɊJ���Ȃ�������
	{
		DocMultiFileCreate( atFile );	//	�V�����t�@�C���u����̏����E�����ŕԂ茌�͗v��Ȃ�
		iNewPage = DocPageCreate( -1 );	//	�y�[�W����Ă���
		PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�
		DocPageChange( 0 );
		MultiFileTabFirst( atFile );
		AppTitleChange( atFile );
	}
#else
	//	INI�Ƀ��X�g�I�[�|���L�^���m�F
	InitLastOpen( INIT_LOAD, atFile );
	if( 0 == DocFileInflate( atFile )  )	//	�ŏ��̂�����
	{
		//	�J���Ȃ������ꍇ
		StringCchCopy( atFile, MAX_PATH, NAMELESS_DUMMY );
		iNewPage = DocPageCreate( -1 );	//	�y�[�W����Ă���
		PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�
		DocPageChange( 0 );
	}
	AppTitleChange( atFile );
#endif

	ViewScrollBarAdjust( NULL );


	ShowWindow( ghViewWnd, SW_SHOW );
	UpdateWindow( ghViewWnd );

	//	�L�����b�g�������Ⴄ����		
	ViewCaretCreate( ghViewWnd, gaColourTable[CLRT_CARETFD], gaColourTable[CLRT_CARETBK] );

	//	���[���[�Ƃ��ɒ���
	gdDocXdot = 0;
	gdDocMozi = 0;
	gdDocLine = 0;
	ViewDrawCaret( gdDocXdot, gdDocLine, 1 );	//	�ʒu�����߂�

	gdXmemory = 0;

	ViewNowPosStatus(  );	//	�����l���o

	return ghViewWnd;
}
//-------------------------------------------------------------------------------------------------

/*!
	��{�����F�ƃL�����b�g�F���m�ۂ���
	@return	COLORREF	�F
*/
COLORREF ViewMoziColourGet( LPCOLORREF pCrtColour )
{
	if( pCrtColour )	*pCrtColour = gaColourTable[CLRT_CARETFD];
	//	�C���f�b�N�X�w��ŃQ�b�g��������̂ق�����������
	return gaColourTable[CLRT_BASICPEN];
}
//-------------------------------------------------------------------------------------------------

//	�w�i�F�Q�b�g
COLORREF ViewBackColourGet( LPVOID pVoid )
{
	return gaColourTable[CLRT_BASICBK];
}
//-------------------------------------------------------------------------------------------------


/*!
	�e�E�C���h�E���ړ�������傫���ς������
	@param[in]	hPrntWnd	�e�E�C���h�E�n���h��
	@param[in]	pstFrame	�N���C�A���g�T�C�Y
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT ViewSizeMove( HWND hPrntWnd, LPRECT pstFrame )
{
	RECT	rect;

	//	����̈ʒu�𒲐�
	rect = *pstFrame;

	if( gbTmpltDock )
	{
		PageListResize( hPrntWnd, pstFrame );
		LineTmpleResize( hPrntWnd, pstFrame );
		BrushTmpleResize( hPrntWnd, pstFrame );

		rect.right -= PLIST_DOCK;
	};

	SetWindowPos( ghViewWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );

	GetClientRect( ghViewWnd, &rect );
	gstViewArea.cx = rect.right - LINENUM_WID;
	gstViewArea.cy = rect.bottom - RULER_AREA;

	gdDispingLine = gstViewArea.cy / LINE_HEIGHT;

	//	��ʃT�C�Y�ύX�����Ƃ��̃X�N���[���o�[�ƕ\���ʒu�̒Ǐ]
	//	�s�����܂�Ȃ��Ȃ炻�̂܂܁A���܂����Ⴄ�悤�Ȃ�A�\���ʒu���ŏ�ʂɂ���
	ViewScrollBarAdjust( NULL );

	ViewDrawCaret( gdDocXdot, gdDocLine, 1 );	//	�J�[�\���ʒu�ĕ`��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�N���[���o�[�̒���������
	@param[in]	pVoid	���ɂȂ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewScrollBarAdjust( LPVOID pVoid )
{
	INT	dMargin, dRange, dDot, dPos, dLines;

//�w�o�[�@��ʉE�ɂ͗]�T�K�v���H
	dMargin = gstViewArea.cx / 2;	//	���ʂ̔����H

	dDot = DocPageMaxDotGet( -1, -1 );
	dRange = dMargin + dDot;
	dRange -= gstViewArea.cx;

	if( 0 >= dRange )
	{
		EnableScrollBar( ghViewWnd, SB_HORZ, ESB_DISABLE_BOTH );
		if( 0 != gdHideXdot )	//	�t���Ɏ��܂邯�ǁA�ʒu���Y���Ă����ꍇ
		{
			gdHideXdot = 0;
		}
	}
	else
	{
		EnableScrollBar( ghViewWnd, SB_HORZ, ESB_ENABLE_BOTH );
		SetScrollRange( ghViewWnd, SB_HORZ, 0, dRange, TRUE );
		dPos = gdHideXdot;
		if( 0 > dPos )	dPos = 0;
		SetScrollPos( ghViewWnd, SB_HORZ, dPos, TRUE );
	}

//�x�o�[�@���ݍs�����X�N���[���Ɏg��
	dLines = DocPageParamGet( NULL, NULL );
	dRange = dLines - gdDispingLine;	//	�S�s���[�\�������K�vSCROLL�i�K

//	TRACE( TEXT("SCL Y Line[%d] Range[%d] Top[%d]"), dLines, dRange, gdViewTopLine );

	if( 0 >= dRange )
	{
		EnableScrollBar( ghViewWnd, SB_VERT, ESB_DISABLE_BOTH );
		//	�t���Ɏ��܂邯�ǁA�ʒu���Y���Ă����ꍇ
		if( 0 != gdViewTopLine )
		{
			gdViewTopLine = 0;
			ViewRedrawSetLine( -1 );	//	�����ő傫���X�N���[������A����
		}
	}
	else
	{
		EnableScrollBar( ghViewWnd, SB_VERT, ESB_ENABLE_BOTH );
		SetScrollRange( ghViewWnd, SB_VERT, 0, dRange, TRUE );
		dPos = gdViewTopLine;
		if( 0 > dPos )	dPos = 0;
		SetScrollPos( ghViewWnd, SB_VERT, dPos, TRUE );
	}




	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�̕`��ʒu�h�b�g���󂯎���āA���[����X�N���[�����l�������\���ʒu�ɕϊ�����
	@param[in]	pDotX	�`��ʒu�̉��h�b�g���ENULL�s��
	@param[in]	pDotY	�`��ʒu�̏c�h�b�g���ENULL�s��
	@param[in]	bTrans	��O�����ʒu���`��ʒu�@�O�`��ʒu�������ʒu
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewPositionTransform( PINT pDotX, PINT pDotY, BOOLEAN bTrans )
{
	assert( pDotX );
	assert( pDotY );

	if( bTrans )	//	�l������
	{
		*pDotX = *pDotX + LINENUM_WID;	//	�s�ԍ��\���̈敪�V�t�g
		*pDotX = *pDotX - gdHideXdot;	//	�B��̈敪����

		*pDotY = *pDotY + RULER_AREA;	//	���[���[�\���̈敪�V�t�g
		*pDotY = *pDotY - (gdViewTopLine*LINE_HEIGHT);
	}
	else	//	�O��
	{
		*pDotX = *pDotX + gdHideXdot;	//	�B��̈敪�E��
		*pDotX = *pDotX - LINENUM_WID;	//	�s�ԍ��\���̈敪�V�t�g

		*pDotY = *pDotY - RULER_AREA;	//	���[���[�\���̈敪�V�t�g
		*pDotY = *pDotY + (gdViewTopLine*LINE_HEIGHT);
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w�肳�ꂽ�`��|�C���g���A�\���g���ł��邩
	@param[in]	xx	�`��ʒu�̂w�h�b�g�ʒu
	@param[in]	yy	�`��ʒu�̂x�h�b�g�ʒu
	@return		��O�g���ł���@�O�͂ݏo���Ă�
*/
BOOLEAN ViewIsPosOnFrame( INT xx, INT yy )
{
	POINT	stPoint;
	RECT	stRect;

	SetRect( &stRect, 0, 0, gstViewArea.cx, gstViewArea.cy );

	//	���[���ƍs�ԍ��̕��A���_�ʒu������ɃV�t�g
	stPoint.x = xx - LINENUM_WID;
	stPoint.y = yy - RULER_AREA;

	return PtInRect( &stRect, stPoint );
}
//-------------------------------------------------------------------------------------------------

/*!
	�`��̈�̃h�b�g���ƍs�����Q�b�g
	@param[in]	pdXdot	�h�b�g��������o�b�t�@�ւ̃|�C���^
	@return		�s��
*/
INT ViewAreaSizeGet( PINT pdXdot )
{
	if( pdXdot )	*pdXdot = gstViewArea.cx;

	return gdDispingLine;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�����b�g�ʒu�ƃX�N���[�������Z�b�g
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewEditReset( VOID )
{
	gdDocXdot = 0;
	gdDocLine = 0;
	gdDocMozi = 0;

	//gdViewXdot = 0;
	//gdViewLine = 0;

	gdHideXdot = 0;
	gdViewTopLine = 0;

	ViewDrawCaret( 0, 0, TRUE );

	ViewScrollBarAdjust( NULL );

	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK ViewWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HIMC	hImc;

	switch( message )
	{
		HANDLE_MSG(hWnd, WM_CREATE,         Evw_OnCreate );			//	��ʂ̍\���p�[�c�����B�{�^���Ƃ�
		HANDLE_MSG(hWnd, WM_PAINT,          Evw_OnPaint );			//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG(hWnd, WM_COMMAND,        Evw_OnCommand );		//	�{�^�������ꂽ�Ƃ��̃R�}���h����
		HANDLE_MSG(hWnd, WM_DESTROY,        Evw_OnDestroy );		//	�\�t�g�I�����̏���
		HANDLE_MSG(hWnd, WM_VSCROLL,        Evw_OnVScroll );		//	
		HANDLE_MSG(hWnd, WM_HSCROLL,        Evw_OnHScroll );		//	
		HANDLE_MSG(hWnd, WM_KEYDOWN,        Evw_OnKey );			//	
		HANDLE_MSG(hWnd, WM_KEYUP,          Evw_OnKey );			//	
		HANDLE_MSG(hWnd, WM_CHAR,           Evw_OnChar );			//	
		HANDLE_MSG(hWnd, WM_MOUSEMOVE,      Evw_OnMouseMove );		//	
		HANDLE_MSG(hWnd, WM_MOUSEWHEEL,     Evw_OnMouseWheel );		//	
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN,    Evw_OnLButtonDown );	//	
		HANDLE_MSG(hWnd, WM_LBUTTONDBLCLK,  Evw_OnLButtonDown );	//	
		HANDLE_MSG(hWnd, WM_LBUTTONUP,      Evw_OnLButtonUp );		//	
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN,    Evw_OnRButtonDown );	//	
		HANDLE_MSG(hWnd, WM_CONTEXTMENU,    Evw_OnContextMenu );	//	

/* void Cls_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) */
		//case WM_SYSKEYDOWN:
		//	TRACE( TEXT("VIEW_WM_SYSKEYDOWN[0x%X][0x%X]"), wParam, lParam );
		//	if( VK_LEFT == wParam || VK_RIGHT == wParam )
		//	{
		//		Evw_OnKey( hWnd, wParam, TRUE, (INT)(SHORT)LOWORD(lParam), (UINT)HIWORD(lParam) );
		//		return 0;
		//	}
		//	break;

/* void Cls_OnSetFocus(HWND hwnd, HWND hwndOldFocus) */
		case WM_SETFOCUS:
			TRACE( TEXT("VIEW_WM_SETFOCUS[0x%X][0x%X]"), wParam, lParam );
			ViewShowCaret(  );
			break;

/* void Cls_OnKillFocus(HWND hwnd, HWND hwndNewFocus) */
		case WM_KILLFOCUS:
			TRACE( TEXT("VIEW_WM_KILLFOCUS[0x%X][0x%X]"), wParam, lParam );
			break;

/* void Cls_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized) */
		case WM_ACTIVATE:
			TRACE( TEXT("VIEW_WM_ACTIVATE[0x%X][0x%X]"), wParam, lParam );
			break;

		case WM_IME_NOTIFY:
			TRACE( TEXT("WM_IME_NOTIFY[0x%X][0x%X]"), wParam, lParam );
			break;

		case WM_IME_REQUEST:
			TRACE( TEXT("WM_IME_REQUEST[0x%X][0x%X]"), wParam, lParam );
			break;

		case WM_IME_STARTCOMPOSITION:	//	�ϊ��ŕ������͂��J�n�����甭������
			TRACE( TEXT("WM_IME_STARTCOMPOSITION[0x%X][0x%X]"), wParam, lParam );
				hImc = ImmGetContext( ghViewWnd );	//	IME�n���h���m��
				if( hImc )	//	�m�ۏo������
				{
					ImmSetCompositionFont( hImc, &gstBaseFont );
					ImmReleaseContext( ghViewWnd , hImc );
				}
			break;

		case WM_IME_ENDCOMPOSITION:
			TRACE( TEXT("WM_IME_ENDCOMPOSITION[0x%X][0x%X]"), wParam, lParam );
			break;

		case WM_IME_COMPOSITION:
			Evw_OnImeComposition( hWnd, wParam, lParam );
			break;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�̃N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	�N���G�C�g�ł�����TRUE
*/
BOOLEAN Evw_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	HINSTANCE lcInst = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�
	UNREFERENCED_PARAMETER(lcInst);



	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[��COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Evw_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	OperationOnCommand( ghPrntWnd, id, hWndCtl, codeNotify );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[��PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Evw_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	ViewRedrawDo( hWnd, hdc );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�̃E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��m�ۂ�����ʍ\���̃������Ƃ����I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Evw_OnDestroy( HWND hWnd )
{
	UINT	i;

	SetWindowFont( hWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );
	DeleteFont( ghRulerFont );
	DeleteFont( ghNumFont4L );
	DeleteFont( ghNumFont5L );
	DeleteFont( ghNumFont6L );

	ViewCaretDelete(  );

	for( i = 0; PENS_MAX > i; i++ )
	{
		DeletePen( gahPen[i] );
	}

	for( i = 0; BRUSHS_MAX > i; i++ )
	{
		DeleteBrush( gahBrush[i] );
	}

#ifdef MULTI_FILE
	DocMultiFileDeleteAll(  );
#else
	DocContentsObliterate(  );
#endif
	PostQuitMessage( 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�̉��X�N���[���o�[�����삳�ꂽ
	@param[in]	hWnd	�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	hWndCtl	�X�N���[���o�[�̃n���h���E�R���g���[���ɂ������Ă���̂łO�ł���
	@param[in]	code	�X�N���[���R�[�h
	@param[in]	pos		�X�N���[���{�b�N�X�i�܂݁j�̈ʒu
	@return		����
*/
VOID Evw_OnHScroll( HWND hWnd, HWND hWndCtl, UINT code, INT pos )
{
	SCROLLINFO	stScrollInfo;
	INT	dDot = gdHideXdot;

	//	��Ԃ����₵��
	ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
	stScrollInfo.cbSize = sizeof(SCROLLINFO);
	stScrollInfo.fMask = SIF_ALL;
	GetScrollInfo( hWnd, SB_HORZ, &stScrollInfo );

	switch( code )	//	�X�N���[�������ɍ��킹�ē��e�����炷
	{
		case SB_LINEUP:	//	��������
			dDot--;
			break;

		case SB_PAGEUP: //	�o�[������
			dDot -= gstViewArea.cx / 5;
			break;

		case SB_THUMBTRACK:	//	�c�}�~�ňړ�
			dDot = stScrollInfo.nTrackPos;
			break;

		case SB_PAGEDOWN:
			dDot += gstViewArea.cx / 5;
			break;

		case SB_LINEDOWN:
			dDot++;
			break;

		default:	return;
	}

	if( 0 > dDot )	dDot = 0;
	if( stScrollInfo.nMax < dDot )	dDot = stScrollInfo.nMax;

	gdHideXdot = dDot;

	stScrollInfo.fMask = SIF_POS;
	stScrollInfo.nPos  = dDot;
	SetScrollInfo( ghViewWnd, SB_HORZ, &stScrollInfo, TRUE );

	ViewRedrawSetLine( -1 );

#if 0
	//	�L�����b�g��Ǐ]�E���Ȃ��ق��������H
	if( gdHideXdot >  gdDocXdot )	gdDocXdot = gdHideXdot + 5;	//	�K���Ȓl
	if( (gdHideXdot+gstViewArea.cx-EOF_WIDTH) <= gdDocXdot )	gdDocXdot = (gstViewArea.cx - EOF_WIDTH);
	DocLetterPosGetAdjust( &gdDocXdot, gdDocLine, 0, 0 );
#endif

	ViewDrawCaret( gdDocXdot, gdDocLine, 0 );	//	�ʒu�����߂�

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�̏c�X�N���[���o�[�����삳�ꂽ
	@param[in]	hWnd	�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	hWndCtl	�X�N���[���o�[�̃n���h���E�R���g���[���ɂ������Ă���̂łO�ł���
	@param[in]	code	�X�N���[���R�[�h
	@param[in]	pos		�X�N���[���{�b�N�X�i�܂݁j�̈ʒu
	@return		����
*/
VOID Evw_OnVScroll( HWND hWnd, HWND hWndCtl, UINT code, INT pos )
{
	SCROLLINFO	stScrollInfo;
	INT	dPos = gdViewTopLine, iLines, dPrev;

	//	pos���A�z�C�[���t���O�ɂ���

	//	���s�����A�\���̈�̂ق����傫�������珈�����Ȃ�
	iLines = DocPageParamGet( NULL, NULL );
	if( gdDispingLine >= iLines )	return;

	//	��Ԃ����₵��
	ZeroMemory( &stScrollInfo, sizeof(SCROLLINFO) );
	stScrollInfo.cbSize = sizeof(SCROLLINFO);
	stScrollInfo.fMask = SIF_ALL;
	GetScrollInfo( hWnd, SB_VERT, &stScrollInfo );

	TRACE( TEXT("code[%d] pos[%d] dPos[%d] InfoMax[%d]"), code, pos, dPos, stScrollInfo.nMax );

	dPrev = dPos;

	switch( code )	//	�X�N���[�������ɍ��킹�ē��e�����炷
	{
		case SB_LINEUP:	//	��������
			if( pos ){	dPos = dPos - gdWheelLine;	}
			else{	 dPos--;	}
			break;

		case SB_PAGEUP: //	�o�[������
			dPos -= gdDispingLine / 2;
			break;

		case SB_THUMBTRACK:	//	�c�}�~�ňړ�
			dPos = stScrollInfo.nTrackPos;
			break;

		case SB_PAGEDOWN:
			dPos += gdDispingLine / 2;
			break;

		case SB_LINEDOWN:
			if( pos ){	dPos = dPos + gdWheelLine;	}
			else{	 dPos++;	}
			break;

		default:	return;
	}

	if( 0 > dPos )	dPos = 0;
	if( stScrollInfo.nMax < dPos )	dPos = stScrollInfo.nMax;

	gdViewTopLine = dPos;

	stScrollInfo.fMask = SIF_POS;
	stScrollInfo.nPos  = dPos;
	SetScrollInfo( ghViewWnd, SB_VERT, &stScrollInfo, TRUE );

	//	�X�N���[�����ĂȂ�������X�V���Ȃ�
	if( dPrev != dPos ){	ViewRedrawSetLine( -1 );	}


#if 0
	//	�L�����b�g��Ǐ]�E���Ȃ��ق��������H
	if( gdViewTopLine >  gdDocLine )	gdViewTopLine = ++gdDocLine;
	if( (gdViewTopLine+gdDispingLine) <= gdDocLine )	gdDocLine--;
	DocLetterPosGetAdjust( &gdDocXdot, gdDocLine, 0, 0 );
#endif

	ViewDrawCaret( gdDocXdot, gdDocLine, 0 );	//	�ʒu�����߂�

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h���E�r���[�̂Ƃ͌���Ȃ��̂Œ��ӃZ��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		����
*/
VOID Evw_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	INT	posX, posY;
#ifndef CONTEXT_EDIT
	HMENU	hMenu;
#endif
	HMENU	hSubMenu;
	UINT	dRslt;

	posX = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	posY = (SHORT)yPos;

	TRACE( TEXT("VIEW_WM_CONTEXTMENU %d x %d"), posX, posY );

#ifdef CONTEXT_EDIT
	hSubMenu = CntxtMenuGet(  );
#else
	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_VIEWPOPUPMENU) );
	hSubMenu = GetSubMenu( hMenu, 0 );
#endif

	CheckMenuItem( hSubMenu , IDM_SQSELECT,           gbSqSelect   ? MF_CHECKED : MF_UNCHECKED );
	CheckMenuItem( hSubMenu , IDM_SPACE_VIEW_TOGGLE,  gdSpaceView  ? MF_CHECKED : MF_UNCHECKED );
	CheckMenuItem( hSubMenu , IDM_GRID_VIEW_TOGGLE,   gbGridView   ? MF_CHECKED : MF_UNCHECKED );
	CheckMenuItem( hSubMenu , IDM_RIGHT_RULER_TOGGLE, gbRitRlrView ? MF_CHECKED : MF_UNCHECKED );

#ifndef CONTEXT_EDIT
//@@�R�s�[����
	if( gbCpModSwap )	//	�R�s�[���[�h����
	{
		ModifyMenu( hSubMenu, IDM_COPY,     MF_BYCOMMAND | MFT_STRING, IDM_COPY, TEXT("SJIS�R�s�|(&C)") );
		ModifyMenu( hSubMenu, IDM_SJISCOPY, MF_BYCOMMAND | MFT_STRING, IDM_SJISCOPY, TEXT("Unicode�R�s�|(&J)") );
	}
#endif

	FrameNameModifyPopUp( hSubMenu, 1 );	//	�g�̖��O��}��

	dRslt = TrackPopupMenu( hSubMenu, TPM_RETURNCMD, posX, posY, 0, hWnd, NULL );	//	TPM_CENTERALIGN | TPM_VCENTERALIGN | 
#ifndef CONTEXT_EDIT
	DestroyMenu( hMenu );
#endif
	//	�I�������łO���|�P�H�A�I�������炻�̃��j���[�̂h�c���߂�悤�ɃZ�b�g

	//	���ꂼ��̏����ɔ�΂�
	FORWARD_WM_COMMAND( ghViewWnd, dRslt, hWndContext, 0, PostMessage );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�[�{�[�h�t�H�[�J�X��ҏW�r���[��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewFocusSet( VOID )
{
//	ViewShowCaret(  );

	SetFocus( ghViewWnd );

//	SetForegroundWindow( ghPrntWnd );
	SetWindowPos( ghPrntWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	TRACE( TEXT("�L�[�{�[�h�t�H�[�J�X�Z�b�g") );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���̃h�b�g�l�ƍs�ԍ����X�e�[�^�X�o�[�ɑ��M����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewNowPosStatus( VOID )
{
	static INT	cdPreDot;	//	���O�̃h�b�g�ʒu����
	TCHAR	atString[SUB_STRING];

	StringCchPrintf( atString, SUB_STRING, TEXT("%d[dot] %d[char] %d[line]"), gdDocXdot, gdDocMozi, gdDocLine + 1 );

	StatusBarSetText( SB_CURSOR, atString );

	//	���[���́A���O�̃h�b�g�ʒu�ƍ��̃h�b�g�ʒu�̂�����ŁA�ĕ`�攭��������

	cdPreDot = gdDocXdot;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w�蕶���̕�����s��
	@param[in]	ch	�����v�肽������
	@return		���h�b�g��
*/
INT ViewLetterWidthGet( TCHAR ch )
{
	SIZE	stSize;
	HDC		hdc= GetDC( ghViewWnd );
	HFONT	hFtOld;

	hFtOld = SelectFont( hdc, ghAaFont );

	GetTextExtentPoint32( hdc, &ch, 1, &stSize );

	SelectFont( hdc, hFtOld );

	ReleaseDC( ghViewWnd, hdc );

	return stSize.cx;
}
//-------------------------------------------------------------------------------------------------

/*!
	������̃h�b�g���𐔂���
	@param[in]	ptStr	��������������
	@return		���h�b�g���E�O�Ȃ�G���[
*/
INT ViewStringWidthGet( LPTSTR ptStr )
{
	SIZE	stSize;
	UINT	cchSize;
	HDC		hdc= GetDC( ghViewWnd );
	HFONT	hFtOld;

	StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );

	if( 0 >= cchSize )	return 0;	//	�ُ펖��

	hFtOld = SelectFont( hdc, ghAaFont );

	GetTextExtentPoint32( hdc, ptStr, cchSize, &stSize );

	SelectFont( hdc, hFtOld );

	ReleaseDC( ghViewWnd, hdc );

	return stSize.cx;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[�̓���̗̈���ĕ`��Ώۗ̈�ɂ���
	@param[in]	pstRect	�Ώۂ̕����ʒu�̋�`����ꂽ�\���̂��̃s���^�[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewRedrawSetRect( LPRECT pstRect )
{
	RECT	rect;

	if( !(pstRect) )	return E_INVALIDARG;

	rect = *pstRect;
	rect.right++;
	rect.bottom++;	//	�L���Ă���

	ViewPositionTransform( (PINT)&(rect.left),  (PINT)&(rect.top),    1 );
	ViewPositionTransform( (PINT)&(rect.right), (PINT)&(rect.bottom), 1 );

	InvalidateRect( ghViewWnd, &rect, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̍s�ԍ��ĕ`��
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X�E�}�C�i�X�Ȃ��ʑS�̍ĕ`��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewRedrawSetVartRuler( INT rdLine )
{
	RECT	rect;
	INT	dDummy = 0;

	//	�\���͈͊O�Ȃ�i�j������K�v�͖���
	if( gdViewTopLine > rdLine )	return S_FALSE;
	if( (gdViewTopLine + gdDispingLine + 1) < rdLine )	return S_FALSE;

	rect.top    = rdLine * LINE_HEIGHT;
	ViewPositionTransform( &dDummy, (PINT)&(rect.top), 1 );

	rect.bottom = rect.top + LINE_HEIGHT;
	rect.left   = 0;
	rect.right  = LINENUM_WID + 2;	//	������Ɨ]�T

	InvalidateRect( ghViewWnd, &rect, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[�̍ĕ`��̈��ݒ肵�ĕ`��w��
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X�E�}�C�i�X�Ȃ��ʑS�̍ĕ`��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewRedrawSetLine( INT rdLine )
{
	RECT	rect, clRect;
	INT	dDummy;

//InvalidateRect�́A�Ώۗ̈�ɑ΂���WM_PAINT�𔭍s����B�ʏ킾��
//�E�C���h�E�v���V�[�W���ɏ���������āAWM_PAINT����������邪�A
//UpdateWindow�́A���̏�ő��`�揈�����͂���B

	ViewScrollBarAdjust( NULL );

	if( 0 > rdLine )
	{
		InvalidateRect( ghViewWnd, NULL, TRUE );
		return S_OK;
	}

	//	�\���͈͊O�Ȃ�i�j������K�v�͖���
	if( gdViewTopLine > rdLine )	return S_FALSE;
	if( (gdViewTopLine + gdDispingLine + 1) < rdLine )	return S_FALSE;

	GetClientRect( ghViewWnd, &clRect );

	SetRect( &rect, 0, rdLine * LINE_HEIGHT, clRect.right, (rdLine+1) * LINE_HEIGHT );

	dDummy = 0;
	ViewPositionTransform( &dDummy, (PINT)&(rect.top), 1 );
	ViewPositionTransform( &dDummy, (PINT)&(rect.bottom), 1 );

	InvalidateRect( ghViewWnd, &rect, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�̕\����`��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	hdc		�`���f�o�C�X�R���e�L�X�g
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewRedrawDo( HWND hWnd, HDC hdc )
{
	LPLETTER	pstTexts = NULL;
	INT		cchLen = 0, dot, iLines, i, vwLines;
	UINT	dFlag = 0;
	HFONT	hFtOld;

	UINT	bTrace = FALSE;	//	�g���X���ł��邩

	//	��ʂɓ����ĂȂ��ꍇ�́H

	hFtOld = SelectFont( hdc, ghAaFont );	//	�t�H���g����������

	iLines = DocPageParamGet( NULL, NULL );	//	���݂̃y�[�W�̑��s���Q�b�g

	//	�K�v�Ȃ��Ƃ���̏����܂ł͂��Ȃ��Ă���
	vwLines = gdDispingLine + 2 + gdViewTopLine;	//	�]�T��������

	//	�g���X�C���[�W
	bTrace = TraceImageAppear( hdc, gdHideXdot, gdViewTopLine * LINE_HEIGHT );	//	����ʒu���l���Z��
	if( bTrace )	SetBkMode( hdc, TRANSPARENT );

	//	�g���X�摜����ɗ���悤��
	ViewDrawMetricLine( hdc, 0 );	//	���C���n

	for( i = 0; iLines > i; i++ )	//	������`��
	{
		//	�K�v�ȏ����珈������
		if( gdViewTopLine > i ){	continue;	}
		if( vwLines <= i )	break;
		//	���S�ɉ�ʊO�ɂȂ�����I����Ă���

		dot = DocLineDataGetAlloc( i, 0, &(pstTexts), &cchLen, &dFlag );
		if( 0 < cchLen )	//	cchLen�ɂ̓k���^�[�~�l�[�^�������ĂȂ�
		{
			//	���̒��ňʒu��F�𒲐����Ă��カ�����
			ViewDrawTextOut( hdc, 0, i, pstTexts, cchLen );
		}
		FREE( pstTexts );

		if( dFlag & CT_RETURN )	//	���s�`��
		{
			ViewDrawReturnMark( hdc, dot, i, dFlag );
		}

		if( dFlag & CT_EOF )	//	EOF�`��
		{
			ViewDrawEOFMark( hdc, dot, i, dFlag );
		}
	}

	SelectFont( hdc, hFtOld );	//	�t�H���g���O��

	ViewDrawRuler( hdc );	//	�ニ�[���[
	ViewDrawLineNumber( hdc );	//	���̍s�ԍ�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��������A�X�^�C���w��ɏ]���ĕ`�悵�Ă���
	@param[in]	hdc			�f�o�C�X�R���e�L�X�g
	@param[in]	dDot		�`��J�n����h�b�g�l
	@param[in]	rdLine		�`�悷��s
	@param[in]	pstTexts	�����ƃX�^�C�����
	@param[in]	cchLen		������
	@return		BOOLEAN		�`��n�j���ǂ���
*/
BOOLEAN ViewDrawTextOut( HDC hdc, INT dDot, UINT rdLine, LPLETTER pstTexts, UINT_PTR cchLen )
{
	UINT_PTR	mz, cchMr;
	COLORREF	clrTextOld, clrBackOld, clrTrcMozi, clrMozi, clrRvsMozi;
	INT		dX, dY;	//	�`��ʒu����
	INT		width, rdStart;
	LPTSTR	ptText;
	UINT	bStyle, cbSize;
	BOOLEAN	bRslt, doDraw;
	RECT	rect;

	dX = dDot;
	dY = rdLine * LINE_HEIGHT;


	cbSize = (cchLen + 1) * sizeof(TCHAR);
	ptText = (LPTSTR)malloc( cbSize );
	if( !(ptText) ){	TRACE( TEXT("malloc error") );	return FALSE;	}
	ZeroMemory( ptText, cbSize );

	//	�ŏ��Ɋ�{���[�h�Z�b�g���āA�W���ݒ���m�ۂ��Ă���
	if( TraceMoziColourGet( &clrTrcMozi ) ){	clrMozi = clrTrcMozi;	}
	else{					clrMozi =  gaColourTable[CLRT_BASICPEN];	}
	clrTextOld = SetTextColor( hdc, clrMozi );

	clrRvsMozi = ~clrMozi;	//	�I����ԗp�ɐF�𔽓]
	clrRvsMozi &= 0x00FFFFFF;

	clrBackOld = SetBkColor(   hdc, gaColourTable[CLRT_BASICBK] );

	bStyle  = pstTexts[0].mzStyle;
	cchMr   = 0;
	width   = 0;
	rdStart = 0;
	doDraw  = FALSE;

	ViewPositionTransform( &rdStart, &dY, 1 );

	SetBkMode( hdc, TRANSPARENT );

	for( mz = 0; cchLen >= mz; mz++ )
	{
		//	�����X�^�C���������Ȃ�
		if( bStyle == pstTexts[mz].mzStyle )
		{
			ptText[cchMr++] = pstTexts[mz].cchMozi;	//	��q����̕�����Ƃ��Ċm��
			width += pstTexts[mz].rdWidth;
		}
		else{	doDraw = TRUE;	}

		//	���[�܂ł����������
		if( cchLen ==  mz ){	doDraw = TRUE;	}

		if( doDraw )	//	�`��^�C�~���O�ł���Ȃ�
		{
			//	�X�y�[�X�Ȃ牺����`�悷��E�����ŕ�����
			if( bStyle & CT_SPACE )
			{
				ViewDrawSpace( hdc, rdStart, dY, ptText, cchMr, bStyle );
				//���̒��ɂ��w�i�F�̓h��Ƃ�����B�������ɒ��ӃZ��
			}
			else
			{
				if( bStyle & CT_SELECT )	//	�I���̏ꍇ�w�i�F�Ƙg�h��ׂ�
				{
					SetTextColor( hdc, clrRvsMozi );
					SetBkColor(   hdc, gaColourTable[CLRT_SELECTBK] );

					SetRect( &rect, rdStart, dY, rdStart + width, dY + LINE_HEIGHT );
					FillRect( hdc, &rect, gahBrush[BRHT_SELECTBK] );
				}
				else if( bStyle & CT_FINDED )	//	�����q�b�g������̏ꍇ
				{
					SetTextColor( hdc, clrMozi );
					SetBkColor(   hdc, gaColourTable[CLRT_FINDBACK] );

					SetRect( &rect, rdStart, dY, rdStart + width, dY + LINE_HEIGHT );
					FillRect( hdc, &rect, gahBrush[BRHT_FINDBACK] );
				}
				else if( bStyle & CT_CANTSJIS )	//	SJIS�s�i���j�R�[�h�����j�̏ꍇ
				{
					SetTextColor( hdc, clrMozi );
					SetBkColor(   hdc, gaColourTable[CLRT_CANTSJIS] );

					SetRect( &rect, rdStart, dY, rdStart + width, dY + LINE_HEIGHT );
					FillRect( hdc, &rect, gahBrush[BRHT_CANTSJISBK] );
				}
				else
				{
					SetTextColor( hdc, clrMozi );
					SetBkColor(   hdc, gaColourTable[CLRT_BASICBK] );
				}

				bRslt = ExtTextOut( hdc, rdStart, dY, 0, NULL, ptText, cchMr, NULL );
				if( !(bRslt)  ){	TRACE( TEXT("ExtTextOut error") );	return FALSE;	}
			}

			rdStart += width;
			//	�`�悵����A���̕�����V�����X�^�C���Ƃ��ēo�^���ă��[�v�ĊJ
			bStyle = pstTexts[mz].mzStyle;
			ZeroMemory( ptText, cbSize );
			ptText[0] = pstTexts[mz].cchMozi;
			width  = pstTexts[mz].rdWidth;
			cchMr  = 1;

			doDraw = FALSE;
		}
	}

	FREE( ptText );	//	�m�ۂ����̈�͊J�����Ȃ��Ǝ���

	//	���ɖ߂��Ă����Ɨǂ����Ƃ�����
	SetTextColor( hdc, clrTextOld );
	SetBkColor(   hdc, clrBackOld );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�y�[�X���A�D�F�����ŕ`�悷��
	@param[in]	hdc		�f�o�C�X�R���e�L�X�g
	@param[in]	dX		�`��J�n���鉡�h�b�g�ʒu
	@param[in]	dY		�`��J�n����c�h�b�g�ʒu
	@param[in]	ptText	�X�y�[�X�̗���A���p�S�p
	@param[in]	cchLen	������
	@param[in]	bStyle	�x���t���E�I��
	@return		����
*/
BOOLEAN ViewDrawSpace( HDC hdc, INT dX, UINT dY, LPTSTR ptText, UINT_PTR cchLen, UINT bStyle )
{
	HPEN	hPenOld;	//	�`��p�Ƀy����p��
	INT		width, xx, yy;
	UINT	cchPos;
	SIZE	stSize;
	RECT	stRect;

//	SetBkColor(   hdc, gaColourTable[CLRT_SPACELINE] );	//	�w�i�F�͕s�v
	//	�`��ʒu�͂��낢�뒲���ς�

	xx = dX;
	yy = dY;

	dY += (LINE_HEIGHT - 2);	//	�����Ȃ̂Ł��̂ق�

	if( bStyle & CT_WARNING )	//	�y����������E�x���ƒʏ�
	{
		hPenOld = SelectPen( hdc , gahPen[PENT_SPACEWARN] );
	}
	else
	{
		hPenOld = SelectPen( hdc , gahPen[PENT_SPACELINE] );
	}

	GetTextExtentPoint32( hdc, ptText, cchLen, &stSize );
	if( bStyle & CT_SELECT )	//	�I����ԂȂ�
	{
		SetRect( &stRect, xx, yy, xx + stSize.cx, yy + stSize.cy );
		FillRect( hdc, &stRect, gahBrush[BRHT_SELECTBK] );
	}
	else if( bStyle & CT_FINDED )	//	�����q�b�g������̏ꍇ
	{
		SetRect( &stRect, xx, yy, xx + stSize.cx, yy + stSize.cy );
		FillRect( hdc, &stRect, gahBrush[BRHT_FINDBACK] );
	}
	else if( bStyle & CT_CANTSJIS )	//	SJIS�s�i���j�R�[�h�����j�̏ꍇ
	{
		SetRect( &stRect, xx, yy, xx + stSize.cx, yy + stSize.cy );
		if( gdSpaceView )	FillRect( hdc, &stRect, gahBrush[BRHT_CANTSJISBK] );
		//	�L���Ȃ�h��
	}

	if( gdSpaceView || (bStyle & CT_WARNING) )	//	�L���Ȃ�`��
	{
		for( cchPos = 0; cchLen > cchPos; cchPos++ )
		{
			if( TEXT(' ') == ptText[cchPos] )	//	���p
			{
				width = SPACE_HAN;
			}
			else if( TEXT('�@') == ptText[cchPos] )	//	�S�p
			{
				width = SPACE_ZEN;
			}
			else	//	���j�R�[�h�󔒂̏ꍇ
			{
				width = ViewLetterWidthGet( ptText[cchPos] );
			}

			MoveToEx( hdc, dX, dY, NULL );	//	�J�n�n�_
			LineTo(   hdc, (dX + width - 1), dY  );	//	�`�敝�P�h�b�g�]�T��������

			dX += width;
		}
	}

	SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	800�h�b�g�̐���O���b�hLine������
	@param[in]	hdc		�f�o�C�X�R���e�L�X�g
	@param[in]	bUpper	�O�����̕`��O�@��O�����̕`���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewDrawMetricLine( HDC hdc, UINT bUpper )
{
	HPEN	hPenOld;	//	�`��p�Ƀy����p��
	INT		dX, dY;	//	�`��g����
	INT		aX, aY;	//	�y���̈ʒu
	LONG	width, height;


	width = gstViewArea.cx + LINENUM_WID;	//	LineTo�͍��W�Ȃ̂ŁA�Ō�܂Ŏw�肷��
	height = gstViewArea.cy + RULER_AREA;	//	LineTo�͍��W�Ȃ̂ŁA�Ō�܂Ŏw�肷��

	//	�������A�㏑���AON/OFF���݂ɂȂ�悤��
	if( gbGridView )
	{

		//	�O���b�h���C���E�ʒu�͐ݒ肩���������悤��
		hPenOld = SelectPen( hdc , gahPen[PENT_GRID_LINE] );	//	���炩���ߊm�ۂ��Ƃ�

		aX = gdGridXpos;
		aY = gdGridYpos;
		ViewPositionTransform( &aX, &aY, 1 );

		while( height > aY )	//	����
		{
			MoveToEx( hdc , LINENUM_WID, aY, NULL );	//	�J�n�n�_
			LineTo(   hdc , width, aY );	//	���E���т�[
			aY += gdGridYpos;
		}

		while( width  > aX )	//	�c��
		{
			MoveToEx( hdc, aX, RULER_AREA-1, NULL );	//	�J�n�n�_
			LineTo(   hdc, aX, height );	//	���E���т�[
			aX += gdGridXpos;
		}

		SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���
	}

	if( gbRitRlrView )
	{
		//	�W�O�O�̐��E�O���b�h����O�ɏ����悤�ɂ���
		hPenOld = SelectPen( hdc , gahPen[PENT_SPACEWARN] );	//	���炩���ߊm�ۂ��Ƃ�

		dX = gdRightRuler;	//	�ݒ肩���������
		dY = 0;

		ViewPositionTransform( &dX, &dY, 1 );

		MoveToEx( hdc, dX, RULER_AREA-1, NULL  );	//	�J�n�n�_
		LineTo(   hdc, dX, height  );	//	���E���т�[

		SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���s�}�[�N��`���E���������ł�������
	@param[in]	hdc		�f�o�C�X�R���e�L�X�g
	@param[in]	dDot	�`��J�n����h�b�g�l
	@param[in]	rdLine	�`�悷��s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewDrawReturnMark( HDC hdc, INT dDot, INT rdLine, UINT dFlag )
{
	HPEN	hPenOld;	//	�`��p�Ƀy����p��
	INT		dX, dY;	//	�`��g����
	INT		aX, aY;	//	�y���̈ʒu
	COLORREF	clrBackOld = 0;
	RECT	rect;

	dX = dDot;
	dY = rdLine * LINE_HEIGHT;

	ViewPositionTransform( &dX, &dY, 1 );

	//	��ʂɓ����ĂȂ��ꍇ���l��

	SetRect( &rect, dX, dY, dX + SPACE_ZEN, dY + LINE_HEIGHT );

	//	�w�i�̐F
	if( dFlag & CT_SELRTN )
	{
		clrBackOld = SetBkColor( hdc , gaColourTable[CLRT_SELECTBK] );
		FillRect( hdc, &rect, gahBrush[BRHT_SELECTBK] );
	}
	else if( dFlag & CT_FINDRTN )
	{
		clrBackOld = SetBkColor( hdc , gaColourTable[CLRT_FINDBACK] );
		FillRect( hdc, &rect, gahBrush[BRHT_FINDBACK] );
	}
	else if( dFlag & CT_LASTSP )
	{
		clrBackOld = SetBkColor( hdc , gaColourTable[CLRT_LASTSPWARN] );
		FillRect( hdc, &rect, gahBrush[BRHT_LASTSPWARN] );
	}
	else
	{
		clrBackOld = SetBkColor( hdc , gaColourTable[CLRT_BASICBK] );
		FillRect( hdc, &rect, gahBrush[BRHT_BASICBK] );
	}

	ExtTextOut( hdc , dX, dY, 0, NULL, TEXT("�@"), 1, NULL );	//	�ꏊ�����

	SetBkColor( hdc, clrBackOld );

	hPenOld = SelectPen( hdc , gahPen[PENT_CRLF_MARK] );	//	�y����������

	aX = dX + 3;
	aY = dY + 3;	//	��}�[�W��
	MoveToEx( hdc, aX, aY, NULL );	//	�J�n�n�_
	LineTo(   hdc, aX, aY + 12  );	//	�ォ�牺��
	LineTo(   hdc, dX, aY + 9  );	//	�������獶���
	MoveToEx( hdc, aX, aY + 12, NULL );	//	���̐���ۂ�
	LineTo(   hdc, aX + 3, aY + 9 );	//	�����ĉE���
	//	����������ƃX�}�[�g�ɂł��Ȃ�������

	SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	EOF�L����`��
	@param[in]	hdc		�f�o�C�X�R���e�L�X�g
	@param[in]	dDot	�`��J�n����h�b�g�l
	@param[in]	rdLine	�`�悷��s
	@param[in]	dFlag	������I�[�̏��
	@return		�g�����h�b�g��
*/
INT ViewDrawEOFMark( HDC hdc, INT dDot, INT rdLine, UINT dFlag )
{

	INT			dX, dY;	//	�`��g����
	COLORREF	clrTextOld, clrBackOld = 0;
	RECT		stClip;
	SIZE		stSize;

	dX = dDot;
	dY = rdLine * LINE_HEIGHT;

	ViewPositionTransform( &dX, &dY, 1 );

	clrTextOld = SetTextColor( hdc , gaColourTable[CLRT_EOF_MARK] );	//	EOF�̐F
	if( dFlag & CT_LASTSP )	clrBackOld = SetBkColor(   hdc , gaColourTable[CLRT_LASTSPWARN] );	//	�w�i�̐F

	GetTextExtentPoint32( hdc, gatEOF, EOF_SIZE, &stSize );

	//	��ʂ̍��[�ɂ߂荞��ł�ꍇ���l��

	//	�\���ꏊ�m�F
	stClip.left   = dX + 1;
	stClip.right  = dX + 1 + stSize.cx;
	stClip.top    = dY + 1;
	stClip.bottom = dY + LINE_HEIGHT;

	ExtTextOut( hdc, stClip.left, stClip.top, 0, &stClip, gatEOF, EOF_SIZE, NULL );

	SetTextColor( hdc, clrTextOld );
	if( dFlag & CT_LASTSP )	SetBkColor( hdc, clrBackOld );

	return stSize.cx;
}
//-------------------------------------------------------------------------------------------------

/*!
	�r���[�̈�̏����𔭐�������
	@param[in]	iBgn	�X�V�͈͂w�J�n
	@param[in]	iEnd	�X�V�͈͂w�I��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewRulerRedraw( INT iBgn, INT iEnd )
{
	RECT	rect;

	GetClientRect( ghViewWnd, &rect );
	rect.bottom = RULER_AREA;

	if( 0 <= iBgn ){	rect.left  = iBgn;	}
	if( 0 <= iEnd ){	rect.right = iEnd;	}

	InvalidateRect( ghViewWnd, &rect, TRUE );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g�㕔��Ruler��`��
	@param[in]	hdc		�f�o�C�X�R���e�L�X�g
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewDrawRuler( HDC hdc )
{
	HPEN	hPenOld;	//	�`��p�Ƀy����p��
	HFONT	hFtOld;		//	���[���[�p
	LONG	width, pos, ln, start, dif, sbn, hei;
	TCHAR	atStr[10];
	UINT_PTR	count;
	RECT		rect;

	hPenOld = SelectPen( hdc, gahPen[PENT_RULER] );	//	���炩���ߊm�ۂ��Ƃ�

	width = gstViewArea.cx + LINENUM_WID;	//	LineTo�͍��W�Ȃ̂ŁA�Ō�܂Ŏw�肷��

	SetBkMode( hdc, TRANSPARENT );

	SetRect( &rect, 0, 0, width, RULER_AREA );
	FillRect( hdc, &rect, gahBrush[BRHT_RULERBK] );

	MoveToEx( hdc, LINENUM_WID, RULER_AREA-1, NULL );	//	�J�n�n�_
	LineTo(   hdc, width, RULER_AREA-1 );	//	���E���т�[

	start = gdHideXdot;	//	�����ɃX�N���[���ʂ��l������΂���

	dif = start % 10;
	sbn = start / 10;
	if( dif ){	sbn++;	dif =  10 - dif;	}	//	����ʂ̒l�v�Z����
	//	�c��
	for( pos = 0, ln = sbn; width > pos; pos+=10, ln++ )
	{
		hei = 6;
		if( !( ln % 5 ) )	hei = 3;
		if( !( ln % 10 ) )	hei = 0;
		MoveToEx( hdc, LINENUM_WID+pos+dif, hei, NULL );	//	�J�n�n�_
		LineTo(   hdc, LINENUM_WID+pos+dif, RULER_AREA-1 );	//	���E���т�[
	}

	SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���

	hFtOld = SelectFont( hdc, ghRulerFont );	//	�t�H���g����������

	//	���l
	dif = start % 100;	if( dif )	dif = 100 - dif;
	sbn = start / 100;	if( dif )	sbn++;
	sbn *= 100;
	for( pos = 0, ln = sbn; width > pos; pos+=100, ln+=100 )
	{
		StringCchPrintf( atStr, 10, TEXT("%d"), ln );
		StringCchLength( atStr, 10, &count );
		ExtTextOut( hdc, LINENUM_WID+pos+2+dif, 0, 0, NULL, atStr, count, NULL );
	}

	SelectFont( hdc, hFtOld );


	//	�L�����b�g�ʒu�E�ĕ`����H
	hPenOld = SelectPen( hdc, gahPen[PENT_CARET_POS] );	//	�F

	MoveToEx( hdc, LINENUM_WID + gdDocXdot, 1, NULL );	//	�J�n�n�_
	LineTo(   hdc, LINENUM_WID + gdDocXdot, RULER_AREA-1 );	//	�ǂ҂���ƈ���

	SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���


	if( 1 <= gdAutoDiffBase )	//	���������̊
	{
		//	�F�A�Ƃ肠�����󔒌x����
		hPenOld = SelectPen( hdc, gahPen[PENT_SPACEWARN] );

		MoveToEx( hdc, LINENUM_WID + gdAutoDiffBase, 1, NULL );	//	�J�n�n�_
		LineTo(   hdc, LINENUM_WID + gdAutoDiffBase, RULER_AREA-1 );	//	�ǂ҂���ƈ���

		SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s�ԍ���`��
	@param[in]	hdc		�f�o�C�X�R���e�L�X�g
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewDrawLineNumber( HDC hdc )
{
	HPEN		hPenOld;	//	�`��p�Ƀy����p��
	HFONT		hFtOld;		//	���[���[�p
	LONG		height, num, hei;
	TCHAR		atStr[10];
	UINT_PTR	count;
	UINT		figure = 4;
	RECT		rect;

	hPenOld = SelectPen( hdc , gahPen[PENT_RULER] );	//	���炩���ߊm�ۂ��Ƃ�

	height = gstViewArea.cy + RULER_AREA;	//	LineTo�͍��W�Ȃ̂ŁA�Ō�܂Ŏw�肷��

	SetBkMode( hdc, TRANSPARENT );

	SetRect( &rect, 0, 0, LINENUM_WID-1, height );
	FillRect( hdc, &rect, gahBrush[BRHT_RULERBK] );

	//	���ƕ`��Area�̊ԂɂP�h�b�g�]�T������̂Ł|�Q
	MoveToEx( hdc, LINENUM_WID-2, RULER_AREA-1, NULL  );	//	�J�n�n�_
	LineTo(   hdc, LINENUM_WID-2, height  );	//	���E���т�[

	SelectPen( hdc, hPenOld );	//	���ɖ߂��Ă���

	num = gdViewTopLine;	//	�J�n���l
	if( 9999 > num )	//	num�F�O�C���f�b�N�X
	{
		figure =  1;
		hFtOld = SelectFont( hdc, ghNumFont4L );
	}
	else if( 9999 <= num && num < 99999 )
	{
		figure =  3;
		hFtOld = SelectFont( hdc, ghNumFont5L );
	}
	else
	{
		figure =  5;
		hFtOld = SelectFont( hdc, ghNumFont6L );
	}
	//	�t�H���g����������

	for( hei = 0; height > hei; hei+=LINE_HEIGHT, num++ )
	{
		if( 1 == figure && 9999 <= num )	//	�S���T�̐؂�ւ��
		{	figure =  3;	SelectFont( hdc , ghNumFont5L );	}

		if( 3 == figure && 99999 <= num )	//	�T���U�̐؂�ւ��
		{	figure =  5;	SelectFont( hdc , ghNumFont6L );	}

		if( DocBadSpaceIsExist( num )  )	//	�󔒌x������Ȃ�Ԃ�����Ƃ�
		{
			SetRect( &rect, 0, hei+RULER_AREA, LINENUM_WID-2, hei+RULER_AREA+LINE_HEIGHT );
			FillRect( hdc, &rect, gahBrush[BRHT_LASTSPWARN] );
		}

		switch( figure )
		{
			default:
			case  1:	StringCchPrintf( atStr, 10 , TEXT("%4d"), num + 1 );	break;
			case  3:	StringCchPrintf( atStr, 10 , TEXT("%5d"), num + 1 );	break;
			case  5:	StringCchPrintf( atStr, 10 , TEXT("%6d"), num + 1 );	break;
		}
		StringCchLength( atStr, 10, &count );
		ExtTextOut( hdc, 0, hei + RULER_AREA + figure, 0, NULL, atStr, count, NULL );
		//	�S�܂łP�@�T�łR�@�U�łT
	}

	SelectFont( hdc, hFtOld );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g��ǉ����鏈���̓���
	@param[in]	dMode	�g�ԍ��O�C���f�b�N�X
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewFrameInsert( INT dMode )
{
	return DocFrameInsert( dMode , gbSqSelect );
}
//-------------------------------------------------------------------------------------------------

/*!
	�ꗗ����̎g�p���[�h���Z�b�g
	@param[in]	dMode	�O�ʏ�}���@�P�����}���@�R���C���@�S���j�R�s�[�@�TSJIS�R�s�[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT ViewMaaItemsModeSet( UINT dMode )
{
	gdUseMode = dMode;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	MAA����SJIS���󂯎���ď�������
	@param[in]	pcCont	AA�̕�����
	@param[in]	cbSize	�o�C�g���E���[NULL�͊܂܂Ȃ�
	@param[in]	dMode	�g�p���[�h�E�f�t�H�������͌ʎw��
	@return		��O�f�t�H���삵���@�O�w�胂�[�h������
*/
UINT ViewMaaMaterialise( LPSTR pcCont, UINT cbSize, UINT dMode )
{
	LPTSTR		ptString;
	UINT_PTR	cchSize;
	UINT		uRslt = FALSE;	//	�f�t�H����ł���Ȃ�TRUE
	INT			xDot;

	//	�f�t�H����ł��邩�ǂ���
	if( dMode == gdUseMode ){		uRslt = TRUE;	}
	if( MAA_DEFAULT ==  dMode ){	dMode = gdUseMode;	uRslt = TRUE;	}

	//	���SJIS�Œ�̃C�x���g����ς܂���
	if( MAA_SJISCLIP == dMode )
	{
		DocClipboardDataSet( pcCont, (cbSize + 1), D_SJIS );
		return uRslt;
	}

	xDot = 0;

	//	�����̓��j�R�[�h�����Ȃ̂ŁE���e��@�Ϗ����{�ɕϊ�����
	ptString = SjisDecodeAlloc( pcCont );
	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

	switch( dMode )
	{
		case MAA_UNICLIP:	//	���j�R�[�h�I�ɃN���b�y
			DocClipboardDataSet( ptString, (cchSize + 1) * sizeof(TCHAR), D_UNI );
			break;

		case MAA_LAYERED:	//	���C������
			LayerBoxVisibalise( ghInst, ptString, 0x00 );
			break;

		case MAA_INTERRUPT:	//	�����}��
			DocInsertString( &gdDocXdot, &gdDocLine, NULL, ptString, D_SQUARE, TRUE );
			DocPageInfoRenew( -1, 1 );
			ViewPosResetCaret( 0, gdDocLine );
			//	�Y���Ȃ��}���������Ȃ�A���C���g���΂���
			break;

		case MAA_INSERT:	//	�ʏ�}��
			DocInsertString( &xDot, &gdDocLine, NULL, ptString, 0, TRUE );
			DocPageInfoRenew( -1, 1 );
			ViewPosResetCaret( xDot, gdDocLine );
			break;

		default:	break;
	}

	FREE(ptString);

	return uRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	���炩�̑��샂�[�h�ɂ��āA�X�e�[�^�X�o�[�̕\����ύX����
	���ꂼ��̃��[�h������ւ������Ă�
*/
HRESULT OperationOnStatusBar( VOID )
{
	CONST  TCHAR	*catTexts[] = { { TEXT("[��`]") }, { TEXT("[�h��]") },
		{ TEXT("[���o]") }, { TEXT("[��]") } };

	TCHAR	atString[SUB_STRING];

	ZeroMemory( atString, sizeof(atString) );

	if( gbSqSelect ){	StringCchCat( atString, SUB_STRING, catTexts[0] );	}
	if( gbBrushMode ){	StringCchCat( atString, SUB_STRING, catTexts[1] );	}
	if( gbExtract ){	StringCchCat( atString, SUB_STRING, catTexts[2] );	}
	if( gdSpaceView ){	StringCchCat( atString, SUB_STRING, catTexts[3] );	}

	StatusBarSetText( SB_OP_STYLE, atString );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A���h�D�ƃ��h�D�̖ʓ|����
	@param[in]	id		�����̎��ʎq
	@param[in]	pxDot	�J�[�\���h�b�g�ʒu�E����������߂�
	@param[in]	pyLine	�J�[�\���s�E����������߂�
	@return		�Ȃ�
*/
VOID OperationUndoRedo( INT id, PINT pxDot, PINT pyLine )
{
	INT		dCrLf;
	
	if( IDM_UNDO == id ){		dCrLf = DocUndoExecute( pxDot, pyLine );	}
	else if( IDM_REDO == id ){	dCrLf = DocRedoExecute( pxDot, pyLine );	}
	else{	 return;	}	//	���֌W�Ȃ�i�j�����Ȃ�

	if( dCrLf ){	ViewRedrawSetLine( -1 );	}
	else{		ViewRedrawSetLine( *pyLine );	}

	ViewDrawCaret( *pxDot, *pyLine, TRUE );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�r���[�ƃ��C���r���[��COMMAND���b�Z�[�W�̎󂯎��
	@param[in]	hWnd		���C���E�C���h�E�n���h���ł��邱��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID OperationOnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	UINT	bMode;

	//	���[�U��`���j���[
	if( IDM_USERINS_ITEM_FIRST <= id && id <= IDM_USERINS_ITEM_LAST )
	{
		UserDefItemInsert( hWnd, (id - IDM_USERINS_ITEM_FIRST) );
		return;
	}

	switch( id )
	{
		default:					TRACE( TEXT("������") );	break;

		//	�t�@�C������
		case IDM_FILE_CLOSE:		MultiFileTabClose(  );	break;

		//	���j�R�[�h�p���b�g�I�[�|��
		case  IDM_UNI_PALETTE:		UniDialogueEntry( ghInst, hWnd );	break;

		//	�v���r���[�I�[�|��
		case  IDM_ON_PREVIEW:		PreviewVisibalise( gixFocusPage );	break;

		//	�����s�e���v����O�ʂ�
		case  IDM_MAATMPLE_VIEW:	SetForegroundWindow( ghMaaWnd );	break;

		//	�ňꗗ��O�ʂ�
		case  IDM_PAGELIST_VIEW:	SetForegroundWindow( ghPgVwWnd );	break;

		//	��s�e���v����O�ʂ�
		case  IDM_LINE_TEMPLATE:	ShowWindow( ghLnTmplWnd , SW_SHOW );	SetForegroundWindow( ghLnTmplWnd  );	break;

		//	�u���V�e���v����O�ʂ�
		case  IDM_BRUSH_PALETTE:	ShowWindow( ghBrTmplWnd , SW_SHOW );	SetForegroundWindow( ghBrTmplWnd  );	break;

		//	�g�ݒ�_�C�����O
		case  IDM_INSFRAME_EDIT:	FrameEditDialogue( ghInst, hWnd, 0 );	break;

		//	��ʐݒ�_�C�����O
		case  IDM_GENERAL_OPTION:	OptionDialogueOpen(   );	break;

		//	�g���X�@�\���J��
		case  IDM_TRACE_MODE_ON:	TraceDialogueOpen( ghInst, hWnd );	break;

#ifdef MOZI_SCRIPT
		//	�����X�N���v�g���J��
		case  IDM_MOZI_SCR_OPEN:	MoziScripterCreate( ghInst , hWnd );	break;
#endif

#ifndef MAA_PROFILE
		//Profile���[�h�Ȃ�O���悤�ɂ���
		case IDM_TREE_RECONSTRUCT:	FORWARD_WM_COMMAND( ghMaaWnd, IDM_TREE_RECONSTRUCT, NULL, 0 , PostMessage );	break;
#endif
		//	�c�[���o�[�̃h���b�v�_�E�����j���[�̌ďo
		case IDM_IN_UNI_SPACE:
		case IDM_INSTAG_COLOUR:
		case IDM_USERINS_NA:		ToolBarPseudoDropDown( hWnd , id );	break;

		//	�ő}�����I�[�|��
		case IDM_PAGENUM_DLG_OPEN:	DocPageNumInsert( ghInst, hWnd );	break;

		//	�u���V�@�\��ON/OFF����
		case IDM_BRUSH_STYLE:		BrushModeToggle(  );	break;

		//	�E�C���h�E�̃t�H�[�J�X��ύX����
		case  IDM_WINDOW_CHANGE:	WindowFocusChange( WND_MAIN,  1 );	break;
		case  IDM_WINDOW_CHG_RVRS:	WindowFocusChange( WND_MAIN, -1 );	break;

		//	�g���X���ɁA�摜�̕\���E��\������
		case IDM_TRC_VIEWTOGGLE:	TraceImgViewTglExt(   );	break;

		//	�t�@�C���V�K�쐬
		case IDM_NEWFILE:			DocOpenFromNull( hWnd );	break;

		//	�t�@�C���J��
		case IDM_OPEN:				DocFileOpen( hWnd );	break;

		//	�㏑���ۑ�
		case IDM_OVERWRITESAVE:		DocFileSave( hWnd, D_SJIS );	break;

		//	���O��t���ĕۑ�
		case IDM_RENAMESAVE:		DocFileSave( hWnd, (D_SJIS|D_RENAME) );	break;

		//	�摜�Ƃ��ĕۑ�
		case IDM_IMAGE_SAVE:		DocImageSave( hWnd, 0, ghAaFont );	break;

		//	�g�}������������肵��
		case IDM_INSFRAME_ALPHA:	ViewFrameInsert( 0 );	break;
		case IDM_INSFRAME_BRAVO:	ViewFrameInsert( 1 );	break;
		case IDM_INSFRAME_CHARLIE:	ViewFrameInsert( 2 );	break;
		case IDM_INSFRAME_DELTA:	ViewFrameInsert( 3 );	break;
		case IDM_INSFRAME_ECHO:		ViewFrameInsert( 4 );	break;
		case IDM_INSFRAME_FOXTROT:	ViewFrameInsert( 5 );	break;
		case IDM_INSFRAME_GOLF:		ViewFrameInsert( 6 );	break;
		case IDM_INSFRAME_HOTEL:	ViewFrameInsert( 7 );	break;
		case IDM_INSFRAME_INDIA:	ViewFrameInsert( 8 );	break;
		case IDM_INSFRAME_JULIETTE:	ViewFrameInsert( 9 );	break;

		//	�g�}�����I�[�|��
		case IDM_FRMINSBOX_OPEN:	FrameInsBoxCreate( ghInst, hWnd );	break;

		//	�R���e�L�X�g���j���[�ҏW
		case IDM_MENUEDIT_DLG_OPEN:	CntxEditDlgOpen( hWnd );	break;

#ifdef FIND_STRINGS
		//	�����񌟍�
		case  IDM_FIND_DLG_OPEN:		FindDialogueOpen( ghInst, hWnd );	break;
		case IDM_FIND_HIGHLIGHT_OFF:	FindHighlightOff(  );	break;

#endif

		//	�A���h�D����
		case IDM_UNDO:	OperationUndoRedo( IDM_UNDO, &gdDocXdot, &gdDocLine );
			//iRslt = DocUndoExecute( &gdDocXdot, &gdDocLine );
			//if( iRslt ){	ViewRedrawSetLine( -1 );	}
			//else{		ViewRedrawSetLine( gdDocLine );	}
			//ViewDrawCaret( gdDocXdot, gdDocLine, TRUE );
			break;

		//	���h�D����
		case IDM_REDO:	OperationUndoRedo( IDM_REDO, &gdDocXdot, &gdDocLine );
			//iRslt = DocRedoExecute( &gdDocXdot, &gdDocLine );
			//if( iRslt ){	ViewRedrawSetLine( -1 );	}
			//else{		ViewRedrawSetLine( gdDocLine );	}
			//ViewDrawCaret( gdDocXdot, gdDocLine, TRUE );
			break;

		//	�؂���
		case IDM_CUT:
//@@�R�s�[����
			bMode = gbCpModSwap ? D_SJIS : D_UNI;
			DocExClipSelect( bMode | gbSqSelect );	//	�R�s�[���č폜����΂���
			if( IsSelecting( NULL ) ){	Evw_OnKey( hWnd, VK_DELETE, TRUE, 0, 0 );	}
			break;

		//	�R�s�[
		case IDM_COPY:
			if( gbExtract )
			{
				DocExtractExecute( NULL );
				gbExtract = FALSE;		//	���o�����烂�[�h�I��
				ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�
				ViewRedrawSetLine( -1 );	//	��ʕ\���X�V
				MenuItemCheckOnOff( IDM_EXTRACTION_MODE, 0 );
				OperationOnStatusBar(  );
			}
			else
			{
//@@�R�s�[����
				bMode = gbCpModSwap ? D_SJIS : D_UNI;
				DocExClipSelect( bMode | gbSqSelect );
			}
			break;

		//	SJIS�R�s�[
//@@�R�s�[����
		case IDM_SJISCOPY:
			bMode = gbCpModSwap ? D_UNI : D_SJIS;	//	�����͋t�ɂ���K�v���L��
			DocExClipSelect( bMode | gbSqSelect  );
			break;

		//	�SSJIS�R�s�[
		case IDM_SJISCOPY_ALL:	DocPageAllCopy( D_SJIS );	break;

		//	�J�[�\���ʒu�̑�����K�v�E�|�C���^�n�����Ē��ŘM��
		case IDM_PASTE:			DocInputFromClipboard( &gdDocXdot, &gdDocLine, &gdDocMozi );	break;

		//	�폜
		case IDM_DELETE:		Evw_OnKey( hWnd, VK_DELETE, TRUE, 0, 0 );	break;

		//	�S�I��
		case IDM_ALLSEL:		ViewSelPageAll( 1 );	break;

		//	��`�I���g�O��
		case IDM_SQSELECT:		ViewSqSelModeToggle( NULL );	break;

		//	�I��͈͂��󔒂ɂ���
		case IDM_FILL_SPACE:
			DocSelectedBrushFilling( NULL, &gdDocXdot , &gdDocLine );
			ViewDrawCaret( gdDocXdot, gdDocLine, 1 );	//	�L�����b�g�ʒu�����߂�
			DocPageInfoRenew( -1, 1 );
			break;

		//	��ʂ̍ĕ`��
		case IDM_NOW_PAGE_REFRESH:	ViewRedrawSetLine( -1 );	break;

		//	800D��40L���炢�܂ł�S�p�X�y�[�X�Ŗ��߂��Ⴄ
		case IDM_FILL_ZENSP:	DocScreenFill( TEXT("�@") );	break;
	
		//	���o���[�hTOGGLE
		case IDM_EXTRACTION_MODE:
			if( gbExtract )
			{
				gbExtract = FALSE;
				ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�
				ViewRedrawSetLine( -1 );	//	��ʕ\���X�V
			}
			else{	gbExtract = TRUE;	}
			MenuItemCheckOnOff( IDM_EXTRACTION_MODE, gbExtract );
			OperationOnStatusBar(  );
			break;

		//	���C���{�b�N�X�N��
		case IDM_LAYERBOX:
			if( gbExtract )
			{
				DocExtractExecute( ghInst );
				gbExtract = FALSE;		//	���o�����烂�[�h�I��
				ViewSelPageAll( -1 );	//	�I��͈͖����Ȃ�
				ViewRedrawSetLine( -1 );	//	��ʕ\���X�V
				MenuItemCheckOnOff( IDM_EXTRACTION_MODE, 0 );
				OperationOnStatusBar(  );
			}
			else
			{
				LayerBoxVisibalise( ghInst, NULL, 0x00 );
			}
			break;

		//	���j�R�[�h�󔒑}��
		case IDM_IN_01SPACE:
		case IDM_IN_02SPACE:
		case IDM_IN_03SPACE:
		case IDM_IN_04SPACE:
		case IDM_IN_05SPACE:
		case IDM_IN_08SPACE:
		case IDM_IN_10SPACE:
		case IDM_IN_16SPACE:		ViewInsertUniSpace( id );	break;

		//	�F�^�O�}��
		case IDM_INSTAG_WHITE:
		case IDM_INSTAG_BLUE:
		case IDM_INSTAG_BLACK:
		case IDM_INSTAG_RED:
		case IDM_INSTAG_GREEN:		ViewInsertColourTag( id );	break;

		//	�E������
		case IDM_RIGHT_GUIDE_SET:	DocLeftGuideline( NULL );	break;

		//	�E�Ɋ񂹂�
		case IDM_RIGHT_SLIDE:		DocRightSlide( &gdDocXdot , gdDocLine );	break;

		//	�s���󔒍폜
		case  IDM_DEL_LASTSPACE:	DocLastSpaceErase( &gdDocXdot , gdDocLine );	break;

		//	�s���ɑS�p�󔒒ǉ�
		case IDM_INS_TOPSPACE:		DocTopLetterInsert( TEXT('�@'), &gdDocXdot, gdDocLine );	break;

		//	�s���󔒍폜
		case IDM_DEL_TOPSPACE:		DocTopSpaceErase( &gdDocXdot, gdDocLine );	break;

		//	�s�������폜
		case IDM_DEL_LASTLETTER:	DocLastLetterErase( &gdDocXdot, gdDocLine );	break;

//		case IDM_DEL_HANSPACE:	//	�s���y�јA�����p�󔒍폜
//			MENUITEM "�s���y�јA�����p�󔒍폜(&U)\tCtrl + L",	IDM_DEL_HANSPACE, GRAYED
//			break;	//	�@�\�ǉ��ǂ����ׁE�g��������H

		//	�P�h�b�g���₷
		case  IDM_INCREMENT_DOT:	DocSpaceShiftProc( VK_RIGHT, &gdDocXdot, gdDocLine );	break;

		//	�P�h�b�g���炷
		case  IDM_DECREMENT_DOT:	DocSpaceShiftProc( VK_LEFT,  &gdDocXdot, gdDocLine );	break;

		//	�S�̂P�h�b�g�E��
		case IDM_INCR_DOT_LINES:	DocPositionShift( VK_RIGHT, &gdDocXdot, gdDocLine );	break;

		//	�S�̂P�h�b�g����
		case IDM_DECR_DOT_LINES:	DocPositionShift( VK_LEFT,  &gdDocXdot, gdDocLine );	break;

		//	���������ʒu����
		case IDM_DOTDIFF_LOCK:
			gdAutoDiffBase = DocDiffAdjBaseSet( gdDocLine );
			ViewRulerRedraw( -1, -1 );
			break;

		//	������������
		case IDM_DOTDIFF_ADJT:	DocDiffAdjExec( &gdDocXdot, gdDocLine );	break;

		//	�󔒂̕\����\���؊�
		case IDM_SPACE_VIEW_TOGGLE:
			gdSpaceView = !(gdSpaceView);
			MenuItemCheckOnOff( IDM_SPACE_VIEW_TOGGLE, gdSpaceView );
			OperationOnStatusBar(  );
			ViewRedrawSetLine( -1 );	//	��ʕ\���X�V
			break;

		//	�O���b�h��TOGGLE
		case IDM_GRID_VIEW_TOGGLE:
			gbGridView = !(gbGridView);
			InitParamValue( INIT_SAVE, VL_GRID_VIEW, gbGridView );
			MenuItemCheckOnOff( IDM_GRID_VIEW_TOGGLE, gbGridView );
			ViewRedrawSetLine( -1 );	//	��ʕ\���X�V
			break;

		//	�E�[�K�C�hTOGGLE
		case IDM_RIGHT_RULER_TOGGLE:
			gbRitRlrView = !(gbRitRlrView);
			InitParamValue( INIT_SAVE, VL_R_RULER_VIEW, gbRitRlrView );
			MenuItemCheckOnOff( IDM_RIGHT_RULER_TOGGLE, gbRitRlrView );
			ViewRedrawSetLine( -1 );	//	��ʕ\���X�V
			break;

		//	�ŕ���
		case IDM_PAGEL_DIVIDE:	DocPageDivide( hWnd, ghInst, gdDocLine );	break;

		//	PageCtrl�֔�΂�
		case IDM_PAGEL_ADD:
		case IDM_PAGEL_INSERT:
		case IDM_PAGEL_DUPLICATE:
		case IDM_PAGEL_COMBINE:
		case IDM_PAGEL_UPFLOW:
		case IDM_PAGEL_DOWNSINK:
		case IDM_PAGE_PREV:
		case IDM_PAGE_NEXT:
		case IDM_PAGEL_RENAME:
			FORWARD_WM_COMMAND( ghPgVwWnd, id, hWndCtl, codeNotify, SendMessage );
			break;

		case IDM_TMPLT_GROUP_PREV:
		case IDM_TMPLT_GROUP_NEXT:
		case IDM_TMPL_GRID_INCREASE:
		case IDM_TMPL_GRID_DECREASE:
			if( gbTmpltDock )
			{
				if( IsWindowVisible( ghLnTmplWnd ) ){		FORWARD_WM_COMMAND( ghLnTmplWnd, id, hWndCtl, codeNotify, SendMessage );	}
				else if( IsWindowVisible( ghBrTmplWnd ) ){	FORWARD_WM_COMMAND( ghBrTmplWnd, id, hWndCtl, codeNotify, SendMessage );	}
			}
			break;

		//	Ctrl+PageUpDown�Ńt�@�C���^�u���ړ�
		case  IDM_FILE_PREV:	MultiFileTabSlide( -1 );	break;
		case  IDM_FILE_NEXT:	MultiFileTabSlide(  1 );	break;

		case IDM_TESTCODE:
			TRACE( TEXT("�@�\�e�X�g") );
			break;
	}


	return;
}
//-------------------------------------------------------------------------------------------------