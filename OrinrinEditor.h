/*! @file
	@brief �A�v���S�̂Ŏg���萔�┟���ł�
	���̃t�@�C���� OrinrinEditor.h �ł��B
	@author	SikigamiHNQ
	@date	2011/00/00
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

#pragma once

#define STRICT

#include "resource.h"
//-------------------------------------------------------------------------------------------------

#include "SplitBar.h"
//-------------------------------------------------------------------------------------------------

//	MLT�̋�؂蕶����
#define MLT_SEPARATERW	TEXT("[SPLIT]")
#define MLT_SEPARATERA	("[SPLIT]")
#define MLT_SPRT_CCH	7

//	AST�̋�؂蕶����
#define AST_SEPARATERW	TEXT("[AA]")
#define AST_SEPARATERA	("[AA]")
#define AST_SPRT_CCH	4

//	�e���v���̃A��
#define TMPLE_BEGINW	TEXT("[ListName=")
#define TMPLE_ENDW		TEXT("[end]")

//	���s
#define CH_CRLFW	TEXT("\r\n")
#define CH_CRLFA	("\r\n")
#define CH_CRLF_CCH	2

//	EOF�}�[�N
#define EOF_SIZE	5
CONST  TCHAR	gatEOF[] = TEXT("[EOF]");
#define EOF_WIDTH	39

//	���s�o�C�g��
#define YY2_CRLF	6
#define STRB_CRLF	4

#define PAGE_BYTE_MAX	4096

#define MODIFY_MSG	TEXT("[�ύX]")
//-------------------------------------------------------------------------------------------------

//	������Ηp�F�w��^�O
#define COLOUR_TAG_WHITE	TEXT("<jbbs fontcolor=\"#ffffff\">")
#define COLOUR_TAG_BLUE		TEXT("<jbbs fontcolor=\"#0000ff\">")
#define COLOUR_TAG_BLACK	TEXT("<jbbs fontcolor=\"#000000\">")
#define COLOUR_TAG_RED		TEXT("<jbbs fontcolor=\"#ff0000\">")
#define COLOUR_TAG_GREEN	TEXT("<jbbs fontcolor=\"#00ff00\">")

//-------------------------------------------------------------------------------------------------



#define CC_TAB	0x09
#define CC_CR	0x0D
#define CC_LF	0x0A

//-----------------------------------------------------------------------------------------------------------------------------------------

//�A���h�D�pCOMMANDO
#define DO_INSERT	1	//	�������́E�y�[�X�g�Ƃ�
#define DO_DELETE	2	//	�����폜�E�؂���Ƃ�

//-----------------------------------------------------------------------------------------------------------------------------------------


#define LINE_HEIGHT	18

#define RULER_AREA	13

#define LINENUM_WID	37
#define LINENUM_COLOUR		0xFF8000

#define RUL_LNNUM_COLOURBK	0xC0C0C0
//-------------------------------------------------------------------------------------------------

//	�X�y�[�X�̕�
#define SPACE_HAN	5
#define SPACE_ZEN	11
//-------------------------------------------------------------------------------------------------

#define CLR_BLACK	0x000000
#define CLR_MAROON	0x000080
#define CLR_GREEN	0x008000
#define CLR_OLIVE	0x008080
#define CLR_NAVY	0x800000
#define CLR_PURPLE	0x800080
#define CLR_TEAL	0x808000
#define CLR_GRAY	0x808080
#define CLR_SILVER	0xC0C0C0
#define CLR_RED		0x0000FF
#define CLR_LIME	0x00FF00
#define CLR_YELLOW	0x00FFFF
#define CLR_BLUE	0xFF0000
#define CLR_FUCHSIA	0xFF00FF
#define CLR_AQUA	0xFFFF00
#define CLR_WHITE	0xFFFFFF

//-------------------------------------------------------------------------------------------------

//	�`�敶���̃A��
#define CT_NORMAL	0x0000	//	���ʂ̕�����
#define CT_WARNING	0x0001	//	�A�����p�󔒂̂悤�Ȍx��
#define CT_SPACE	0x0002	//	��
#define CT_SELECT	0x0004	//	�I����Ԃł���
#define CT_CANTSJIS	0x0008	//	�V�t�gJIS�ɕϊ��ł��Ȃ�����
#define CT_LYR_TRNC	0x0010	//	���C���{�b�N�X�œ��ߔ͈�
#define CT_FINDED	0x0020	//	�����q�b�g������

#define CT_SELRTN	0x0100	//	�s�����s���I�����
#define CT_LASTSP	0x0200	//	�s�����󔒂ł���
#define CT_RETURN	0x0400	//	���s���K�v
#define CT_EOF		0x0800	//	���[�ł���
#define CT_FINDRTN	0x1000	//	�s�����s�������q�b�g
//-------------------------------------------------------------------------------------------------

#ifndef _ORRVW

#ifdef OPEN_HISTORY
//�J��������p
#define OPENHIST_MAX	10
typedef struct tagOPENHISTORY
{
	TCHAR	atFile[MAX_PATH];

} OPENHIST, *LPOPENHIST;
typedef vector<OPENHIST>::iterator	OPHIS_ITR;
#endif
//----------------

//	�g�p�[�c�f�[�^
#define PARTS_CCH	10
typedef struct tagFRAMEITEM
{
	TCHAR	atParts[PARTS_CCH];	//!<	�p�[�c������E�X���܂�
	INT		dDot;	//!<	

} FRAMEITEM, *LPFRAMEITEM;
//----------------
//	�g�����p
typedef struct tagFRAMEINFO
{
	TCHAR	atFrameName[MAX_STRING];	//!<	���O�E���g�p

	FRAMEITEM	stDaybreak;	//!<	��		��
	FRAMEITEM	stMorning;	//!<	����	��
	FRAMEITEM	stNoon;		//!<	��		��
	FRAMEITEM	stAfternoon;//!<	�E��	��
	FRAMEITEM	stNightfall;//!<	�E		��
	FRAMEITEM	stTwilight;	//!<	�E��	��
	FRAMEITEM	stMidnight;	//!<	��		��
	FRAMEITEM	stDawn;		//!<	����	��

	INT		dLeftOffset;	//!<	���ǂ̔z�u�I�t�Z�b�g�E�O�͍���̎n�_
	INT		dRightOffset;	//!<	�E�ǂ̔z�u�I�t�Z�b�g�E�O�͉E��̎n�_

} FRAMEINFO, *LPFRAMEINFO;
//-----------------------------

//	�g���X���[�h�pParameter�ێ�
typedef struct tagTRACEPARAM
{
	POINT	stOffsetPt;	//!<	�ʒu���킹
	INT		dContrast;	//!<	�R���g���X�g
	INT		dGamma;		//!<	�K���}
	INT		dGrayMoph;	//!<	�W�F
	INT		dZooming;	//!<	�g��k��
	INT		dTurning;	//!<	��]

	UINT	bUpset;		//!<	
	UINT	bMirror;	//!<	

	COLORREF	dMoziColour;	//!<	�����F�ɂ���

} TRACEPARAM, *LPTRACEPARAM;
//----------------


//!	���샍�O�{��
typedef struct tagOPERATELOG
{
	UINT	dCommando;	//!<	����^�C�v
	UINT	ixSequence;	//!<	����ԍ��E�ӂ肫������ǂ�����
	UINT	ixGroup;	//!<	����O���[�v�E�P��̏����E�P�C���f�b�N�X

	INT		rdXdot;		//!<	����̂������h�b�g�E�����ʒu�Ƃǂ������ǂ��H
//	INT		rdXmozi;	//!<	����̂������h�L�������g�����ʒu
	INT		rdYline;	//!<	����̂������h�L�������g�s

	LPTSTR	ptText;		//!<	���삳�ꂽ������
	UINT	cchSize;	//!<	������̕�����

} OPERATELOG, *LPOPERATELOG;
typedef vector<OPERATELOG>::iterator	OPSQ_ITR;

typedef struct tagUNDOBUFF
{
	UINT_PTR	dNowSqn;	//!<	�Q�͒��̑���ʒu�H
	UINT		dTopSqn;	//!<	�ŐV�̑���ԍ��P�C���f�b�N�X�E�ǉ��̓C���N�����Ă���
	UINT		dGrpSqn;	//!<	����O���[�v�E�P�C���f�b�N�X

	vector<OPERATELOG>	vcOpeSqn;	//!<	���샍�O�{��

} UNDOBUFF, *LPUNDOBUFF;


//	�땶���̏��E�󂯓n���ɂ��g��
typedef struct tagLETTER
{
	TCHAR	cchMozi;	//!<	����
	INT		rdWidth;	//!<	���̕����̕�
	UINT	mzStyle;	//!<	�����̃^�C�v�E�󔒂Ƃ�����������
	CHAR	acSjis[10];	//!<	�V�t�gJIS�R�[�h�A�������́u&#dddd;�v�`���������
	INT_PTR	mzByte;		//!<	SJIS�o�C�g�T�C�Y

} LETTER, *LPLETTER;
typedef vector<LETTER>::iterator	LTR_ITR;

//	��s�̊Ǘ�
typedef struct tagONELINE
{
	INT		iDotCnt;	//!<	�h�b�g��
	INT		iByteSz;	//!<	�o�C�g��
//	INT		dCaret;		//!<	�L�����b�g�̌��݈ʒu�O�C���f�b�N�X
	UINT	dStyle;		//!<	���̍s�̓�����
	BOOLEAN	bBadSpace;	//!<	�x���t���󔒂����邩

	vector<LETTER>	vcLine;	//!<	���̍s�̓��e�E���s�͊܂܂Ȃ�

	//	���C���{�b�N�X�p
	INT		dFrtSpDot;	//!<	�O�̋󔒃h�b�g
	INT		dFrtSpMozi;	//!<	�O�̋󔒕�����
//	INT		dOffset;	//!<	��`�I�������Ƃ��̃Y��

} ONELINE, *LPONELINE;
typedef vector<ONELINE>::iterator	LINE_ITR;

//	SPLIT�y�[�W�땪
typedef struct tagONEPAGE
{
	TCHAR	atPageName[SUB_STRING];	//!<	

	INT		dByteSz;		//!<	�o�C�g��

	//	�I����Ԃɂ���
	INT		dSelLineTop;	//!<	��ԏ�̑I��������s
	INT		dSelLineBottom;	//!<	��ԉ��̑I��������s

	vector<ONELINE>	vcPage;	//!<	�s�S��

	UNDOBUFF	stUndoLog;	//!<	���엚���E�A���h�D�Ɏg��

} ONEPAGE, *LPONEPAGE;
typedef vector<ONEPAGE>::iterator	PAGE_ITR;

//	��̃t�@�C���ێ�
typedef struct tagONEFILE
{
	TCHAR	atFileName[MAX_PATH];	//!<	�t�@�C����
	//	�Ő��̓��F�N�^�����J�E���c����΂n�j�H
	UINT	dModify;	//!<	�ύX�������ǂ���

#ifdef MULTI_FILE
	LPARAM	dUnique;	//!<	�ʂ��ԍ��E�P�C���f�b�N�X
	TCHAR	atDummyName[MAX_PATH];	//!<	�t�@�C�����Ȃ��Ƃ��̉�����
#endif

	INT		dNowPage;	//!<	���Ă��

	vector<ONEPAGE>	vcCont;	//!<	�y�[�W��ێ�����

} ONEFILE, *LPONEFILE;
//-----------------------------

#ifdef MULTI_FILE
typedef list<ONEFILE>::iterator	FILES_ITR;
#endif

//	�����t�@�C�������Ȃ�A����ɃR�����܂���΂����H

//	��s�E�u���V�e���v���p
typedef struct tagAATEMPLATE
{
	TCHAR	atCtgryName[SUB_STRING];	//!<	�Z�b�g�̖��O

	vector<wstring>	vcItems;	//!<	�e���v��������{��

} AATEMPLATE, *LPAATEMPLATE;

//-------------------------------------------------------------------------------------------------

typedef UINT (CALLBACK* PAGELOAD)(LPTSTR, LPTSTR, INT);

#endif	//	NOT _ORRVW

//-------------------------------------------------------------------------------------------------


//	���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾
INT_PTR		CALLBACK About( HWND, UINT, WPARAM, LPARAM );

INT_PTR		MessageBoxCheckBox( HWND, HINSTANCE, UINT );

VOID		WndTagSet( HWND, LONG_PTR );
LONG_PTR	WndTagGet( HWND );

HRESULT		InitWindowPos( UINT, UINT, LPRECT );
INT			InitParamValue( UINT, UINT, INT );
HRESULT		InitParamString( UINT, UINT, LPTSTR );

BOOLEAN		SelectDirectoryDlg( HWND, LPTSTR, UINT_PTR );

UINT		ViewMaaMaterialise( LPSTR, UINT, UINT );

LPTSTR		SjisDecodeAlloc( LPSTR );
LPSTR		SjisEntityExchange( LPCSTR );
BOOLEAN		HtmlEntityCheck( TCHAR, LPSTR, UINT_PTR );

BOOLEAN		FileExtensionCheck( LPTSTR, LPTSTR );

HWND		MaaTmpltInitialise( HINSTANCE, HWND, LPRECT );
VOID		MaaTabBarSizeGet( LPRECT  );	//!<	

HRESULT		AaItemsTipSizeChange( INT, UINT );


#ifndef _ORRVW

LPSTR		SjisEncodeAlloc( LPTSTR );

ATOM		InitWndwClass( HINSTANCE );
BOOL		InitInstance( HINSTANCE, INT, LPTSTR );
LRESULT		CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

HRESULT		StatusBarSetText( INT, LPCTSTR );
HRESULT		StatusBarSetTextInt( INT, INT );

HRESULT		MenuItemCheckOnOff( UINT, UINT );
HRESULT		NotifyBalloonExist( HWND, LPTSTR, LPTSTR, DWORD );

HRESULT		BrushModeToggle( VOID );

HRESULT		WindowFocusChange( INT, INT );

HRESULT		OptionDialogueOpen( VOID );

COLORREF	InitColourValue( UINT, UINT, COLORREF );
INT			InitTraceValue( UINT, LPTRACEPARAM );
HRESULT		InitLastOpen( UINT, LPTSTR );
INT			InitWindowTopMost( UINT, UINT, INT );
#ifndef MAA_PROFILE
HRESULT		InitMaaFldrOpen( UINT, LPTSTR );
INT			InitMultipleFile( UINT, UINT, LPTSTR, LPTSTR );
#endif


VOID		ToolBarCreate( HWND, HINSTANCE );
VOID		ToolBarDestroy( VOID );
HRESULT		ToolBarSizeGet( LPRECT );
HRESULT		ToolBarCheckOnOff( UINT, UINT );
HRESULT		ToolBarOnSize( HWND, UINT, INT, INT );
LRESULT		ToolBarOnNotify( HWND, INT, LPNMHDR );
LRESULT		ToolBarOnContextMenu( HWND, HWND, LONG, LONG );
VOID		ToolBarPseudoDropDown( HWND, INT );

UINT		AppClientAreaCalc( LPRECT );

HRESULT		AppTitleChange( LPTSTR );
HRESULT		AppTitleTrace( UINT );

LPTSTR		ExePathGet( VOID );

HRESULT		UniDialogueEntry( HINSTANCE, HWND );

HRESULT		FrameInitialise( LPTSTR, HINSTANCE );
HRESULT		FrameNameModifyPopUp( HMENU, UINT );
INT_PTR		FrameEditDialogue( HINSTANCE, HWND, UINT );

HWND		FrameInsBoxCreate( HINSTANCE, HWND );
HRESULT		FrameMoveFromView( HWND, UINT );


#ifdef CONTEXT_EDIT

HRESULT		CntxEditInitialise( LPTSTR, HINSTANCE );
HRESULT		CntxEditDlgOpen( HWND );
HMENU		CntxMenuGet( VOID );
HRESULT		CntxMenuCopySwap( VOID );


#endif

#ifdef MULTI_FILE
HRESULT		MultiFileTabFirst( LPTSTR );
HRESULT		MultiFileTabAppend( LPARAM, LPTSTR );
HRESULT		MultiFileTabSelect( LPARAM );
HRESULT		MultiFileTabSlide( INT );
HRESULT		MultiFileTabRename( LPARAM, LPTSTR );
HRESULT		MultiFileTabClose( VOID );
INT			InitMultiFileTabOpen( UINT, INT, LPTSTR );
#endif

VOID		OperationOnCommand( HWND, INT, HWND, UINT );

VOID		AaFontCreate( UINT );

HWND		ViewInitialise( HINSTANCE, HWND, LPRECT, LPTSTR );
HRESULT		ViewSizeMove( HWND, LPRECT );
HRESULT		ViewFocusSet( VOID );

BOOL		ViewShowCaret( VOID );			//!<	
VOID		ViewHideCaret( VOID );

HRESULT		ViewFrameInsert( INT );
HRESULT		ViewMaaItemsModeSet( UINT );

INT			ViewLetterWidthGet( TCHAR );
INT			ViewStringWidthGet( LPTSTR );
HRESULT		ViewNowPosStatus( VOID );

HRESULT		ViewRedrawSetLine( INT );
HRESULT		ViewRedrawSetRect( LPRECT );
HRESULT		ViewRedrawSetVartRuler( INT );
HRESULT		ViewRulerRedraw( INT, INT );
HRESULT		ViewEditReset( VOID );

COLORREF	ViewMoziColourGet( LPCOLORREF );
COLORREF	ViewBackColourGet( LPVOID );

HRESULT		ViewCaretCreate( HWND, COLORREF, COLORREF );	//!<	
HRESULT		ViewCaretDelete( VOID );		//!<	
BOOLEAN		ViewDrawCaret( INT, INT , BOOLEAN );	//!<	�{���̓h���[����Ȃ��ă|�W�V�����`�F���W����
BOOLEAN		ViewPosResetCaret( INT, INT );	//!<	
HRESULT		ViewCaretReColour( COLORREF );	//!<	

HRESULT		ViewPositionTransform( PINT, PINT, BOOLEAN );
BOOLEAN		ViewIsPosOnFrame( INT, INT );	//!<	
INT			ViewAreaSizeGet( PINT );

HRESULT		ViewSelPositionSet( LPVOID );	//!<	
HRESULT		ViewSelMoveCheck( UINT );		//!<	
HRESULT		ViewSelRangeCheck( UINT );		//!<	
INT			ViewSelPageAll( INT );			//!<	
UINT		ViewSqSelModeToggle( LPVOID );	//!<	

INT			ViewInsertUniSpace( UINT );
INT			ViewInsertColourTag( UINT );
INT			ViewInsertTmpleString( LPTSTR );

HRESULT		ViewBrushStyleSetting( UINT, LPTSTR );

VOID		Evw_OnMouseMove( HWND, INT, INT, UINT );
VOID		Evw_OnLButtonDown( HWND, BOOL, INT, INT, UINT );
VOID		Evw_OnLButtonUp( HWND, INT, INT, UINT );
VOID		Evw_OnRButtonDown( HWND, BOOL, INT, INT, UINT );

VOID		Evw_OnKey( HWND, UINT, BOOL, INT, UINT );	//!<	
VOID		Evw_OnChar( HWND, TCHAR, INT );				//!<	
VOID		Evw_OnMouseWheel( HWND, INT, INT, INT, UINT );

VOID		Evw_OnImeComposition( HWND, WPARAM, LPARAM );

BOOLEAN		IsSelecting( PUINT );

HRESULT		OperationOnStatusBar( VOID );

HWND		PageListInitialise( HINSTANCE, HWND, LPRECT );
VOID		PageListResize( HWND, LPRECT );
HRESULT		PageListClear( VOID );
HRESULT		PageListInsert( INT );
HRESULT		PageListDelete( INT );
HRESULT		PageListViewChange( INT );
HRESULT		PageListInfoSet( INT, INT, INT );
HRESULT		PageListNameSet( INT, LPTSTR );
#ifdef MULTI_FILE
INT			PageListIsNamed( FILES_ITR );
#else
INT			PageListIsNamed( VOID );
#endif

HRESULT		PageListBuild( LPVOID );


HRESULT		TemplateItemLoad( LPTSTR, PAGELOAD );
UINT		TemplateGridFluctuate( HWND, INT );

HWND		LineTmpleInitialise( HINSTANCE, HWND, LPRECT );
VOID		LineTmpleResize( HWND, LPRECT );

VOID		DockingTabSizeGet( LPRECT );

HWND		BrushTmpleInitialise( HINSTANCE, HWND, LPRECT, HWND );
LPTSTR		BrushStringMake( INT, LPTSTR );
VOID		BrushTmpleResize( HWND, LPRECT );

INT			UserDefInitialise( HWND, UINT );
HRESULT		UserDefItemInsert( HWND, UINT );
HRESULT		UserDefMenuWrite( HMENU );
HRESULT		UserDefSetString( vector<ONELINE> *, LPTSTR, UINT );	//!<	

HRESULT		FrameNameModifyMenu( HWND );

VOID		PreviewInitialise( HINSTANCE, HWND );
HRESULT		PreviewVisibalise( INT );

INT			TraceInitialise( HWND, UINT );
HRESULT		TraceDialogueOpen( HINSTANCE, HWND );
HRESULT		TraceImgViewTglExt( VOID );
UINT		TraceImageAppear( HDC, INT, INT );
UINT		TraceMoziColourGet( LPCOLORREF );

HRESULT		ImageFileSaveDC( HDC, LPTSTR, INT );


VOID		LayerBoxInitialise( HINSTANCE, LPRECT );
HRESULT		LayerBoxAlphaSet( UINT );
HRESULT		LayerMoveFromView( HWND, UINT );
HWND		LayerBoxVisibalise( HINSTANCE, LPTSTR, UINT );
INT			LayerHeadSpaceCheck( vector<LETTER> *, PINT );	//!<	
HRESULT		LayerTransparentToggle( HWND, UINT );			//!<	
HRESULT		LayerContentsImportable( HWND, UINT, LPINT, LPINT, UINT );	//!<	
HRESULT		LayerBoxPositionChange( HWND, LONG, LONG );
HRESULT		LayerStringReplace( HWND, LPTSTR );



HRESULT		DocInitialise( LPVOID );

BOOLEAN		DocRangeIsError( INT, INT );

INT_PTR		DocPageCount( VOID );

HRESULT		DocOpenFromNull( HWND );
UINT		DocPageParamGet( PINT, PINT );
INT			DocPageMaxDotGet( INT, INT );
INT			DocPageByteCount( INT, PINT );
HRESULT		DocPageInfoRenew( INT, UINT );

HRESULT		DocPageNameSet( LPTSTR );

UINT		DocPageCreate( INT );
HRESULT		DocPageDelete( INT );
HRESULT		DocPageChange( INT );

HRESULT		DocModifyContent( UINT );

#ifdef MULTI_FILE
LPARAM		DocMultiFileCreate( LPTSTR );
#else
HRESULT		DocContentsObliterate( VOID );
#endif
INT			DocLineParamGet( INT, PINT, PINT );

UINT		DocBadSpaceCheck( INT );
BOOLEAN		DocBadSpaceIsExist( INT );

HRESULT		DocPageDivide( HWND, HINSTANCE, INT );

BOOLEAN		DocIsSjisTrance( TCHAR, LPSTR );
INT_PTR		DocLetterByteCheck( LPLETTER );

INT			DocInputLetter( INT, INT, TCHAR );
INT			DocInputBkSpace( PINT, PINT );
INT			DocInputDelete( INT, INT );
INT			DocInputFromClipboard( PINT, PINT, PINT );

INT			DocAdditionalLine( INT, BOOLEAN );

INT			DocStringAdd( PINT, PINT, LPTSTR, INT );
HRESULT		DocCrLfAdd( INT, INT, BOOLEAN );
INT			DocSquareAdd( PINT, PINT, LPTSTR, INT, LPPOINT * );
INT			DocStringErase( INT, INT, LPTSTR, INT );

INT			DocInsertLetter( PINT, INT, TCHAR );
INT			DocInsertString( PINT, PINT, PINT, LPTSTR, UINT, BOOLEAN );

INT			DocIterateDelete( LTR_ITR, INT );
HRESULT		DocLineCombine( INT );

BOOLEAN		DocLineErase( INT, BOOLEAN );


HRESULT		DocFrameInsert( INT, INT );
HRESULT		DocScreenFill( LPTSTR );

HRESULT		DocPageNumInsert( HINSTANCE, HWND );

INT			DocExClipSelect( UINT );
HRESULT		DocPageAllCopy( UINT );

INT			DocLetterShiftPos( INT, INT, INT, PINT, PBOOLEAN );
INT			DocLetterPosGetAdjust( PINT, INT, INT );

HRESULT		DocReturnSelStateToggle( INT, INT );
INT			DocRangeSelStateToggle( INT, INT, INT, INT );
INT			DocPageSelStateToggle( INT );
HRESULT		DocSelRangeSet( INT, INT );
HRESULT		DocSelRangeGet( PINT, PINT );
//BOOLEAN		DocIsSelecting( VOID );

LPTSTR		DocClipboardDataGet( PUINT );
HRESULT		DocClipboardDataSet( LPVOID, INT, UINT );

INT			DocLineDataGetAlloc( INT, INT, LPLETTER *, PINT, PUINT );
INT			DocPageTextAllGetAlloc( UINT, LPVOID * );
LPSTR		DocPageTextPreviewAlloc( INT, PINT );

HRESULT		DocThreadDropCopy( VOID );

INT			DocSelectedDelete( PINT, PINT, UINT );
INT			DocSelectedBrushFilling( LPTSTR, PINT, PINT );
INT			DocSelectTextGetAlloc( UINT, LPVOID *, LPPOINT * );

HRESULT		DocExtractExecute( HINSTANCE );

HRESULT		DocFileSave( HWND, UINT );
HRESULT		DocFileOpen( HWND );
#ifdef MULTI_FILE
INT			DocAllTextGetAlloc( INT, UINT, LPVOID *, FILES_ITR );
#else
INT			DocAllTextGetAlloc( INT, UINT, LPVOID * );
#endif
HRESULT		DocImageSave( HWND, UINT, HFONT );


UINT		DocStringSplitAST( LPTSTR, INT, PAGELOAD );

INT			DocLineStateCheckWithDot( INT, INT, PINT, PINT, PINT, PINT, PBOOLEAN );
HRESULT		DocLeftGuideline( LPVOID );
INT			DocSpaceShiftProc( UINT, PINT, INT );
LPTSTR		DocPaddingSpaceMake( INT );
LPTSTR		DocPaddingSpaceUni( INT, PINT, PINT, PINT );
LPTSTR		DocPaddingSpaceWithGap( INT, PINT, PINT );
LPTSTR		DocPaddingSpaceWithPeriod( INT, PINT, PINT, PINT, BOOLEAN );
HRESULT		DocLastSpaceErase( PINT, INT );
HRESULT		DocTopLetterInsert( TCHAR, PINT, INT );
HRESULT		DocLastLetterErase( PINT, INT );
HRESULT		DocTopSpaceErase( PINT, INT );
HRESULT		DocRightSlide( PINT, INT );

HRESULT		DocPositionShift( UINT, PINT, INT );

LPTSTR		DocLastSpDel( vector<LETTER> * );

INT			DocDiffAdjBaseSet( INT );
INT			DocDiffAdjExec( PINT, INT );

VOID		ZeroONELINE( LPONELINE );
LPTSTR		NextLine( LPTSTR );
INT			DocLineCount( LPTSTR, UINT );

UINT		DocRangeDeleteByMozi( INT, INT, INT, INT, PBOOLEAN );

INT			DocUndoExecute( PINT, PINT );
INT			DocRedoExecute( PINT, PINT );

LPARAM		DocFileInflate( LPTSTR );
INT			DocFileCloseCheck( HWND, UINT );
HRESULT		DocClipLetter( TCHAR );
VOID		DocBackupDirectoryInit( LPTSTR );
HRESULT		DocFileBackup( HWND );
#ifdef MULTI_FILE
HRESULT		DocMultiFileDeleteAll( VOID );
LPARAM		DocMultiFileDelete( HWND, LPARAM );
HRESULT		DocMultiFileSelect( LPARAM );
HRESULT		DocMultiFileModify( UINT );
HRESULT		DocMultiFileStore( LPTSTR );
INT			DocMultiFileFetch( INT, LPTSTR, LPTSTR );
#endif

HRESULT		SqnInitialise( LPUNDOBUFF );
HRESULT		SqnFreeAll( LPUNDOBUFF );
HRESULT		SqnSetting( VOID );
UINT		SqnAppendLetter( LPUNDOBUFF, UINT, TCHAR, INT, INT, UINT );
UINT		SqnAppendString( LPUNDOBUFF, UINT, LPTSTR, INT, INT, UINT );
UINT		SqnAppendSquare( LPUNDOBUFF, UINT, LPTSTR, LPPOINT, INT, UINT );

HRESULT		UnicodeRadixExchange( LPVOID );

INT			MoziInitialise( LPTSTR, HINSTANCE );
HWND		MoziScripterCreate( HINSTANCE, HWND );
HRESULT		MoziMoveFromView( HWND, UINT );

#ifdef FIND_STRINGS
HRESULT		FindDialogueOpen( HINSTANCE, HWND );
HRESULT		FindHighlightOff( VOID );
#endif

#endif	//	NOT _ORRVW

