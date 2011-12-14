/*! @file
	@brief ���C���̃R���e�L�X�g���j���[�̊Ǘ����܂�
	���̃t�@�C���� ContextCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/07
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

//	���[�U��`�Ƃ��g�Ƃ��́A�T�u���j���[�p�̃_�~�[�A�C�e����p�ӂ���

typedef struct tagCONTEXTITEM
{
	TCHAR	atString[MIN_STRING];		//!<	�\�����镶����
	TCHAR	atAccelerator[MIN_STRING];	//!<	�A�N�Z�����[�^������E�Ȃ�������󕶎���
	UINT	dCommandoID;				//!<	�R�}���h�h�c�E0�ŃZ�p���[�^

} CONTEXTITEM, *LPCONTEXTITEM;
//-------------------------------------------------------------------------------------------------




const static CONTEXTITEM	gstContextItem[] =
{
/* 0*/	{  TEXT("�V�K�쐬"),	/*�t�@�C��*/	TEXT("Ctrl + N"),		IDM_NEWFILE			},
/* 1*/	{  TEXT("�t�@�C���J��"),				TEXT("Ctrl + O"),		IDM_OPEN			},
		{  TEXT("�㏑���ۑ�"),					TEXT("Ctrl + S"),		IDM_OVERWRITESAVE	},
		{  TEXT("���O��t���ĕۑ�"),			TEXT("Ctrl + Alt + S"),	IDM_RENAMESAVE		},
		{  TEXT("���ݕł��摜�ɂ��ĕۑ�"),		TEXT(""),				IDM_IMAGE_SAVE		},
/* 5*/	{  TEXT("��ʐݒ�"),					TEXT(""),				IDM_GENERAL_OPTION	},
		{  TEXT("�i�Z�p���[�^�j"),				TEXT(""),				0					},
		{  TEXT("���ɖ߂�"),	/*�ҏW*/		TEXT("Ctrl + Z"),			IDM_UNDO			},
		{  TEXT("��蒼��"),					TEXT("Ctrl + Y"),			IDM_REDO			},
		{  TEXT("�؂���"),					TEXT("Ctrl + X"),			IDM_CUT				},
/*10*/	{  TEXT("Unicode�R�s�["),				TEXT("Ctrl + C"),			IDM_COPY			},
		{  TEXT("SJIS�R�s�["),					TEXT(""),					IDM_SJISCOPY		},
		{  TEXT("�S�̂�SJIS�R�s�["),			TEXT("Ctrl + Shift + C"),	IDM_SJISCOPY_ALL	},
		{  TEXT("�\�t"),						TEXT("Ctrl + V"),			IDM_PASTE			},
		{  TEXT("�폜"),						TEXT("Delete"),				IDM_DELETE			},
/*15*/	{  TEXT("�S�I��"),						TEXT("Ctrl + A"),			IDM_ALLSEL			},
		{  TEXT("��`�I��"),					TEXT("Ctrl + Alt + B"),		IDM_SQSELECT		},
		{  TEXT("���C���{�b�N�X"),				TEXT("Alt + Space"),		IDM_LAYERBOX		},
		{  TEXT("�������o���[�h"),				TEXT(""),					IDM_EXTRACTION_MODE	},
		{  TEXT("���̍s�ȍ~��V�łɕ���"),		TEXT(""),					IDM_PAGEL_DIVIDE	},
/*20*/	{  TEXT("�i�Z�p���[�^�j"),				TEXT(""),					0					},
		{  TEXT("���j�R�[�h��"),	/*�}��*/	TEXT(""),	IDM_MN_UNISPACE		},//�T�u���j���[�_�~�[
		{  TEXT(" 1 dot SPACE"),				TEXT(""),	IDM_IN_01SPACE		},
		{  TEXT(" 2 dot SPACE"),				TEXT(""),	IDM_IN_02SPACE		},
		{  TEXT(" 3 dot SPACE"),				TEXT(""),	IDM_IN_03SPACE		},
/*25*/	{  TEXT(" 4 dot SPACE"),				TEXT(""),	IDM_IN_04SPACE		},
		{  TEXT(" 5 dot SPACE"),				TEXT(""),	IDM_IN_05SPACE		},
		{  TEXT(" 8 dot SPACE"),				TEXT(""),	IDM_IN_08SPACE		},
		{  TEXT("10 dot SPACE"),				TEXT(""),	IDM_IN_10SPACE		},
		{  TEXT("16 dot SPACE"),				TEXT(""),	IDM_IN_16SPACE		},
/*30*/	{  TEXT("�F�w��"),						TEXT(""),	IDM_MN_COLOUR_SEL	},//�T�u���j���[�_�~�[
		{  TEXT("��"),							TEXT(""),	IDM_INSTAG_WHITE	},
		{  TEXT("��"),							TEXT(""),	IDM_INSTAG_BLUE		},
		{  TEXT("��"),							TEXT(""),	IDM_INSTAG_BLACK	},
		{  TEXT("��"),							TEXT(""),	IDM_INSTAG_RED		},
/*35*/	{  TEXT("��"),							TEXT(""),	IDM_INSTAG_GREEN	},
		{  TEXT("�g�I��"),						TEXT(""),			IDM_MN_INSFRAME_SEL		},//�T�u���j���[�_�~�[
		{  TEXT("�g�i�P�j"),					TEXT(""),			IDM_INSFRAME_ALPHA		},
		{  TEXT("�g�i�Q�j"),					TEXT(""),			IDM_INSFRAME_BRAVO		},
		{  TEXT("�g�i�R�j"),					TEXT(""),			IDM_INSFRAME_CHARLIE	},
/*40*/	{  TEXT("�g�i�S�j"),					TEXT(""),			IDM_INSFRAME_DELTA		},
		{  TEXT("�g�i�T�j"),					TEXT(""),			IDM_INSFRAME_ECHO		},
		{  TEXT("�g�i�U�j"),					TEXT(""),			IDM_INSFRAME_FOXTROT	},
		{  TEXT("�g�i�V�j"),					TEXT(""),			IDM_INSFRAME_GOLF		},
		{  TEXT("�g�i�W�j"),					TEXT(""),			IDM_INSFRAME_HOTEL		},
/*45*/	{  TEXT("�g�i�X�j"),					TEXT(""),			IDM_INSFRAME_INDIA		},
		{  TEXT("�g�i�P�O�j"),					TEXT(""),			IDM_INSFRAME_JULIETTE	},
		{  TEXT("�g�ҏW"),						TEXT(""),			IDM_INSFRAME_EDIT		},
		{  TEXT("�g�}���{�b�N�X"),				TEXT("Ctrl + W"),	IDM_FRMINSBOX_OPEN		},
		{  TEXT("�����`�`�ϊ��{�b�N�X"),		TEXT(""),			IDM_MOZI_SCR_OPEN		},
/*50*/	{  TEXT("���[�U��`"),					TEXT(""),			IDM_MN_USER_REFS		},//�T�u���j���[�_�~�[
		{  TEXT("�e�łɒʂ��ԍ�������"),		TEXT(""),			IDM_PAGENUM_DLG_OPEN	},
		{  TEXT("�i�Z�p���[�^�j"),				TEXT(""),			0						},
		{  TEXT("�E������"),	/*���`*/		TEXT("Ctrl + Alt + R"),		IDM_RIGHT_GUIDE_SET	},
		{  TEXT("�s���ɑS�p�󔒒ǉ�"),			TEXT("Ctrl + I"),			IDM_INS_TOPSPACE	},
/*55*/	{  TEXT("�s���󔒍폜"),				TEXT("Ctrl + U"),			IDM_DEL_TOPSPACE	},
		{  TEXT("�s���󔒍폜"),				TEXT("Ctrl + G"),			IDM_DEL_LASTSPACE	},
		{  TEXT("�s�������폜"),				TEXT("Ctrl + Shift + G"),	IDM_DEL_LASTLETTER	},
		{  TEXT("�I��͈͂��󔒂ɂ���"),		TEXT("Ctrl + Alt + G"),		IDM_FILL_SPACE		},
		{  TEXT("�őS�̂��󔒂Ŗ��߂�"),		TEXT(""),					IDM_FILL_ZENSP		},
/*60*/	{  TEXT("�E�Ɋ񂹂�"),					TEXT(""),					IDM_RIGHT_SLIDE		},
		{  TEXT("�P�h�b�g���₷"),				TEXT("Alt + ��"),			IDM_INCREMENT_DOT	},
		{  TEXT("�P�h�b�g���炷"),				TEXT("Alt + ��"),			IDM_DECREMENT_DOT	},
		{  TEXT("�S�̂��P�h�b�g�E��"),			TEXT("Alt + Shift + ��"),	IDM_INCR_DOT_LINES	},
		{  TEXT("�S�̂��P�h�b�g����"),			TEXT("Alt + Shift + ��"),	IDM_DECR_DOT_LINES	},
/*65*/	{  TEXT("��������b�N"),				TEXT("Ctrl + Alt + W"),		IDM_DOTDIFF_LOCK	},
		{  TEXT("�J�[�\���ʒu�Œ���"),			TEXT("Ctrl + Alt + E"),		IDM_DOTDIFF_ADJT	},
		{  TEXT("�i�Z�p���[�^�j"),				TEXT(""),					0					},
		{  TEXT("�󔒂�\��"),	/*�\��*/		TEXT(""),	IDM_SPACE_VIEW_TOGGLE	},
		{  TEXT("�O���b�h����\��"),			TEXT(""),	IDM_GRID_VIEW_TOGGLE	},
/*70*/	{  TEXT("�E�K�C�h����\��"),			TEXT(""),	IDM_RIGHT_RULER_TOGGLE	},
		{  TEXT("�����s�e���v���[�g"),			TEXT(""),	IDM_MAATMPLE_VIEW		},
		{  TEXT("�ňꗗ"),						TEXT(""),	IDM_PAGELIST_VIEW		},
		{  TEXT("��s�e���v���[�g"),			TEXT(""),	IDM_LINE_TEMPLATE		},
		{  TEXT("�h��ׂ��u���V"),				TEXT(""),	IDM_BRUSH_PALETTE		},
/*75*/	{  TEXT("���j�R�[�h�\"),				TEXT(""),	IDM_UNI_PALETTE			},
		{  TEXT("�g���X���[�h"),				TEXT(""),	IDM_TRACE_MODE_ON		},
		{  TEXT("�v���r���["),					TEXT(""),	IDM_ON_PREVIEW			},
		{  TEXT("�h���t�g�{�[�h���J��"),		TEXT("Ctrl + Space"),	IDM_DRAUGHT_OPEN	},
		{  TEXT("�T���l�C����\��"),			TEXT("Ctrl + T"),		IDM_MAA_THUMBNAIL_OPEN	},
/*80*/	{  TEXT("�i�Z�p���[�^�j"),				TEXT(""),	0						},
		{  TEXT("�I��͈͂��h���t�g�{�[�h��"),	TEXT(""),	IDM_COPY_TO_DRAUGHT		}
};
#define ALL_ITEMS	82



//�����ݒ�
#define DEF_ITEMS	32
const static UINT	gadDefItem[] =
{
	IDM_CUT,				//	�؂���
	IDM_COPY,				//	Unicode�R�s�[
	IDM_PASTE,				//	�\�t
	IDM_ALLSEL,				//	�S�I��
	0,
	IDM_COPY_TO_DRAUGHT,	//	�I��͈͂��h���t�g�{�[�h��
	0,
	IDM_SJISCOPY,			//	SJIS�R�s�[
	IDM_SJISCOPY_ALL,		//	�S�̂�SJIS�R�s�[
	0,
	IDM_SQSELECT,			//	��`�I��
	0,
	IDM_LAYERBOX,			//	���C���{�b�N�X
	IDM_FRMINSBOX_OPEN,		//	�g�}���{�b�N�X
	IDM_MOZI_SCR_OPEN,		//	�����`�`�ϊ��{�b�N�X
	0,
	IDM_RIGHT_GUIDE_SET,	//	�E������
	IDM_INS_TOPSPACE,		//	�s���ɑS�p�󔒒ǉ�
	IDM_DEL_TOPSPACE,		//	�s���󔒍폜
	IDM_DEL_LASTSPACE,		//	�s���󔒍폜
	IDM_DEL_LASTLETTER,		//	�s�������폜
	IDM_FILL_SPACE,			//	�I��͈͂��󔒂ɂ���
	IDM_RIGHT_SLIDE,		//	�E�Ɋ񂹂�
	0,
	IDM_INCR_DOT_LINES,		//	�S�̂��P�h�b�g�E��
	IDM_DECR_DOT_LINES,		//	�S�̂��P�h�b�g����
	0,
	IDM_SPACE_VIEW_TOGGLE,	//	�󔒂�\��	
	IDM_GRID_VIEW_TOGGLE,	//	�O���b�h����\��
	IDM_RIGHT_RULER_TOGGLE,	//	�E�K�C�h����\��
	0,
	IDM_PAGEL_DIVIDE		//	���̍s�ȍ~��V�łɕ���
};
//-------------------------------------------------------------------------------------------------








extern  UINT	gbCpModSwap;	//!<	SJIS�ƃ��j�R�[�h�R�s�[�����ւ���

static HINSTANCE	ghInst;
static TCHAR		gatCntxIni[MAX_PATH];	//!<	

static HMENU	ghPopupMenu;	
static HMENU	ghUniSpMenu;
static HMENU	ghColourMenu;
static HMENU	ghFrameMenu;
static HMENU	ghUsrDefMenu;

static vector<CONTEXTITEM>	gvcCntxItem;
typedef vector<CONTEXTITEM>::iterator	CTXI_VITR;

static list<CONTEXTITEM>	gltCntxEdit;
typedef list<CONTEXTITEM>::iterator	CTXI_LITR;
//-------------------------------------------------------------------------------------------------

VOID	CntxEditBuild( VOID );

INT_PTR	CALLBACK CntxEditDlgProc( HWND, UINT, WPARAM, LPARAM );

VOID	CntxDlgLvInit( HWND );
VOID	CntxDlgAllListUp( HWND );
VOID	CntxDlgBuildListUp( HWND );

VOID	CntxDlgItemAdd( HWND );
VOID	CntxDlgItemDel( HWND );
VOID	CntxDlgItemSpinUp( HWND );
VOID	CntxDlgItemSpinDown( HWND );
//-------------------------------------------------------------------------------------------------

/*!
	�N�����������EINI�f�B���N�g���i�[�Ƃ����������Ƃ�
	@param[in]	ptCurrent	��f�B���N�g��
	@param[in]	hInstance	���̃A�v���̎���
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT CntxEditInitialise( LPTSTR ptCurrent, HINSTANCE hInstance )
{
	UINT	dCount, cid;
	UINT	ams, ims;
	TCHAR	atKeyName[MIN_STRING];



	if( !(ptCurrent) || !(hInstance) )
	{
		if( ghPopupMenu ){	DestroyMenu( ghPopupMenu  );	}
		if( ghUniSpMenu ){	DestroyMenu( ghUniSpMenu  );	}
		if( ghColourMenu ){	DestroyMenu( ghColourMenu );	}
		if( ghFrameMenu ){	DestroyMenu( ghFrameMenu  );	}
		if( ghUsrDefMenu ){	DestroyMenu( ghUsrDefMenu );	}

		return S_OK;
	}



	ghInst = hInstance;

	StringCchCopy( gatCntxIni, MAX_PATH, ptCurrent );
	PathAppend( gatCntxIni, MZCX_INI_FILE );


	gvcCntxItem.clear();

	//	�������
	dCount = GetPrivateProfileInt( TEXT("Context"), TEXT("Count"), 0, gatCntxIni );
	if( 1 <= dCount )	//	�f�[�^�L��
	{
		for( ims = 0; dCount > ims; ims++ )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("CmdID%u"), ims );
			cid = GetPrivateProfileInt( TEXT("Context"), atKeyName, 0, gatCntxIni );

			for( ams = 0; ALL_ITEMS > ams; ams++ )
			{
				if( cid == gstContextItem[ams].dCommandoID )
				{
					gvcCntxItem.push_back( gstContextItem[ams] );
					break;
				}
			}
		}
	}
	else	//	�f�[�^�����Ȃ�f�t�H���j���[���\�z
	{
		for( ims = 0; DEF_ITEMS > ims; ims++ )
		{
			for( ams = 0; ALL_ITEMS > ams; ams++ )
			{
				if( gadDefItem[ims] == gstContextItem[ams].dCommandoID )
				{
					gvcCntxItem.push_back( gstContextItem[ams] );
					break;
				}
			}
		}
	}

	CntxEditBuild(  );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ł��������Ă�R���e�L�X�g���j���[���O���Ŏg��
*/
HMENU CntxMenuGet( VOID )
{
	return ghPopupMenu;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[��g�ݗ��Ă�B
*/
VOID CntxEditBuild( VOID )
{
	UINT	d, e;
	TCHAR	atItem[SUB_STRING], atKey[MIN_STRING];
	CTXI_VITR	itMnItm;

	if( ghPopupMenu ){	DestroyMenu( ghPopupMenu  );	}	ghPopupMenu  = NULL;
	if( ghUniSpMenu ){	DestroyMenu( ghUniSpMenu  );	}	ghUniSpMenu  = NULL;
	if( ghColourMenu ){	DestroyMenu( ghColourMenu );	}	ghColourMenu = NULL;
	if( ghFrameMenu ){	DestroyMenu( ghFrameMenu  );	}	ghFrameMenu  = NULL;
	if( ghUsrDefMenu ){	DestroyMenu( ghUsrDefMenu );	}	ghUsrDefMenu = NULL;

	ghPopupMenu = CreatePopupMenu(  );

	e = 0;
	for( itMnItm = gvcCntxItem.begin(); gvcCntxItem.end() != itMnItm; itMnItm++ )
	{
		if( 0 >= itMnItm->dCommandoID )
		{
			AppendMenu( ghPopupMenu, MF_SEPARATOR, 0, NULL );
		}
		else
		{
			StringCchCopy( atItem, SUB_STRING, itMnItm->atString );

			if( gbCpModSwap )	//	�R�s�[���[�h����
			{
				if( IDM_COPY == itMnItm->dCommandoID )		StringCchCopy( atItem, SUB_STRING, TEXT("SJIS�R�s�|") );
				if( IDM_SJISCOPY == itMnItm->dCommandoID )	StringCchCopy( atItem, SUB_STRING, TEXT("Unicode�R�s�|") );
			}

			if( 26 > e )
			{
				StringCchPrintf( atKey, MIN_STRING, TEXT("(&%c)"), 'A' + e );
				StringCchCat( atItem, SUB_STRING, atKey );
				e++;
			}

			switch( itMnItm->dCommandoID )
			{
				default:	AppendMenu( ghPopupMenu, MF_STRING, itMnItm->dCommandoID, atItem );	break;

#pragma message ("�T�u���j���[�A�C�e���̃T�[�`�A�L�������ł��Ȃ��悤�ɂ���")
				case IDM_MN_UNISPACE:
					ghUniSpMenu = CreatePopupMenu(  );
					for( d = 0; 8 > d; d++ )	AppendMenu( ghUniSpMenu, MF_STRING, gstContextItem[22+d].dCommandoID, gstContextItem[22+d].atString );
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghUniSpMenu, atItem );
					break;

				case IDM_MN_COLOUR_SEL:
					ghColourMenu = CreatePopupMenu(  );
					for( d = 0; 5 > d; d++ )	AppendMenu( ghColourMenu, MF_STRING, gstContextItem[31+d].dCommandoID, gstContextItem[31+d].atString );
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghColourMenu, atItem );
					break;

				case IDM_MN_INSFRAME_SEL:
					ghFrameMenu = CreatePopupMenu(  );
					for( d = 0; 10 > d; d++ )	AppendMenu( ghFrameMenu, MF_STRING, gstContextItem[37+d].dCommandoID, gstContextItem[37+d].atString );
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghFrameMenu, atItem );
					break;

				case IDM_MN_USER_REFS:
					ghUsrDefMenu = CreatePopupMenu(  );
					UserDefMenuWrite( ghUsrDefMenu );
					AppendMenu( ghPopupMenu, MF_POPUP, (UINT_PTR)ghUsrDefMenu, atItem );
					break;
			}
		}
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[��SJIS�ƃ��j�R�[�h�̃A�������ւ���
*/
HRESULT CntxMenuCopySwap( VOID )
{
	CntxEditBuild(  );	//	�Ă�����Build�������ł���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�_�C�����O�J��
	@param[in]	hWnd	�E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT CntxEditDlgOpen( HWND hWnd )
{
	INT_PTR	iRslt, i;
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	CTXI_LITR	itEdit;

	iRslt = DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_CONTEXT_ITEM_DLG), hWnd, CntxEditDlgProc, 0 );
	if( IDOK == iRslt )
	{
		//	��U�Z�N�V��������ɂ���
		ZeroMemory( atBuff, sizeof(atBuff) );
		WritePrivateProfileSection( TEXT("Context"), atBuff, gatCntxIni );

		gvcCntxItem.clear();	i = 0;
		for( itEdit = gltCntxEdit.begin(); gltCntxEdit.end() != itEdit; itEdit++ )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("CmdID%u"), i );
			StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), itEdit->dCommandoID );
			WritePrivateProfileString( TEXT("Context"), atKeyName, atBuff, gatCntxIni );

			gvcCntxItem.push_back( *itEdit );
			i++;
		}

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), i );
		WritePrivateProfileString( TEXT("Context"), TEXT("Count"), atBuff, gatCntxIni );

		CntxEditBuild(  );

		return S_OK;
	}

	return E_ABORT;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ҏW�_�C�����O�{�b�N�X�̃��Z�[�W�n���h�������Ă΂�
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK CntxEditDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	INT		id;
	HWND	hWndCtl;
	UINT	codeNotify;

	CTXI_VITR	itMnItm;


	switch( message )
	{
		case WM_INITDIALOG:
			gltCntxEdit.clear();
			for( itMnItm = gvcCntxItem.begin(); gvcCntxItem.end() != itMnItm; itMnItm++ ){	gltCntxEdit.push_back( *itMnItm );	}
			CntxDlgLvInit( hDlg );
			CntxDlgAllListUp( hDlg );
			CntxDlgBuildListUp( hDlg );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id         = LOWORD(wParam);	//	���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq
			hWndCtl    = (HWND)lParam;		//	���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��
			codeNotify = HIWORD(wParam);	//	�ʒm���b�Z�[�W
			switch( id )
			{
				case IDOK:		EndDialog( hDlg, IDOK );		return (INT_PTR)TRUE;
				case IDCANCEL:	EndDialog( hDlg, IDCANCEL );	return (INT_PTR)TRUE;

				case IDB_MENUITEM_ADD:		CntxDlgItemAdd( hDlg );	return (INT_PTR)TRUE;
				case IDB_MENUITEM_DEL:		CntxDlgItemDel( hDlg );	return (INT_PTR)TRUE;

				case IDB_MENUITEM_SPINUP:	CntxDlgItemSpinUp( hDlg );		return (INT_PTR)TRUE;
				case IDB_MENUITEM_SPINDOWN:	CntxDlgItemSpinDown( hDlg );	return (INT_PTR)TRUE;

				default:	break;
			}
			break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e���̃��X�g�r���[�쐬
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgLvInit( HWND hDlg )
{
	HWND	hLvWnd;
	LVCOLUMN	stLvColm;
	RECT	rect;


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );
	GetClientRect( hLvWnd, &rect );

	ListView_SetExtendedListViewStyle( hLvWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP );

	ZeroMemory( &stLvColm, sizeof(LVCOLUMN) );
	stLvColm.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	stLvColm.fmt      = LVCFMT_LEFT;
	stLvColm.iSubItem = 0;

	stLvColm.pszText  = TEXT("���j���[�A�C�e��");
	stLvColm.cx       = rect.right - 23;
	ListView_InsertColumn( hLvWnd, 0, &stLvColm );


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );
	GetClientRect( hLvWnd, &rect );

	ListView_SetExtendedListViewStyle( hLvWnd, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP );

	stLvColm.cx       = rect.right - 23;
	ListView_InsertColumn( hLvWnd, 0, &stLvColm );

}
//-------------------------------------------------------------------------------------------------

/*!
	�S�A�C�e�������X�g�r���[�Ƀu�b����
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgAllListUp( HWND hDlg )
{
	HWND	hLvWnd;
	UINT	d;
	LVITEM	stLvi;
	TCHAR	atItem[SUB_STRING];


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );

	ListView_DeleteAllItems( hLvWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask    = LVIF_TEXT;
	stLvi.pszText = atItem;

	for( d = 0; ALL_ITEMS > d; d++ )
	{
		StringCchCopy( atItem, SUB_STRING, gstContextItem[d].atString );

		if( IDM_MN_UNISPACE == gstContextItem[d].dCommandoID || 
		IDM_MN_COLOUR_SEL   == gstContextItem[d].dCommandoID || 
		IDM_MN_INSFRAME_SEL == gstContextItem[d].dCommandoID || 
		IDM_MN_USER_REFS    == gstContextItem[d].dCommandoID )
		{
			StringCchCat( atItem, SUB_STRING, TEXT("�i�T�u���j���[�W�J�j") );
		}

		if( gbCpModSwap )	//	�R�s�[���[�h����
		{
			if( IDM_COPY     ==  gstContextItem[d].dCommandoID )	StringCchCopy( atItem, SUB_STRING, TEXT("SJIS�R�s�|") );
			if( IDM_SJISCOPY ==  gstContextItem[d].dCommandoID )	StringCchCopy( atItem, SUB_STRING, TEXT("Unicode�R�s�|") );
		}

		stLvi.iItem = d;
		ListView_InsertItem( hLvWnd, &stLvi );
	}
}
//-------------------------------------------------------------------------------------------------

/*!
	���j���[�\�������X�g�r���[�ɕ\��
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgBuildListUp( HWND hDlg )
{
	HWND	hLvWnd;
	UINT	d;
	LVITEM	stLvi;
	TCHAR	atItem[SUB_STRING];

	CTXI_LITR	itMnItm;


	hLvWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	ListView_DeleteAllItems( hLvWnd );

	ZeroMemory( &stLvi, sizeof(stLvi) );
	stLvi.mask    = LVIF_TEXT;
	stLvi.pszText = atItem;

	for( itMnItm = gltCntxEdit.begin(), d = 0; gltCntxEdit.end() != itMnItm; itMnItm++, d++ )
	{
		stLvi.iItem = d;
		if( 0 >= itMnItm->dCommandoID )
		{
			StringCchCopy( atItem, SUB_STRING, TEXT("---------------") );
		}
		else
		{
			StringCchCopy( atItem, SUB_STRING, itMnItm->atString );

			if( IDM_MN_UNISPACE == itMnItm->dCommandoID || 
			IDM_MN_COLOUR_SEL   == itMnItm->dCommandoID || 
			IDM_MN_INSFRAME_SEL == itMnItm->dCommandoID || 
			IDM_MN_USER_REFS    == itMnItm->dCommandoID )
			{
				StringCchCat( atItem, SUB_STRING, TEXT("�@�@[��") );
			}

			if( gbCpModSwap )	//	�R�s�[���[�h����
			{
				if( IDM_COPY     == itMnItm->dCommandoID )	StringCchCopy( atItem, SUB_STRING, TEXT("SJIS�R�s�|") );
				if( IDM_SJISCOPY == itMnItm->dCommandoID )	StringCchCopy( atItem, SUB_STRING, TEXT("Unicode�R�s�|") );
			}
		}

		ListView_InsertItem( hLvWnd, &stLvi );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e����ǉ�
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemAdd( HWND hDlg )
{
	HWND	hListWnd, hBuildWnd;
	INT		iSel, iIns, iCount;
	CTXI_LITR	itMnItm;

	hListWnd  = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );
	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	//	���̂ق�
	iSel = ListView_GetNextItem( hListWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 > iSel )	 return;	//	�I�����ĂȂ�������I���

	iCount = ListView_GetItemCount( hBuildWnd );


	//	�I���������c�̎��ɂ����E���I���������Ȃ疖����
	iIns = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( (0 > iIns) || ((iIns+1) >= iCount) ){	gltCntxEdit.push_back( gstContextItem[iSel] );	}
	else
	{
		itMnItm = gltCntxEdit.begin();
		advance( itMnItm, iIns+1 );
		//	�C�e���[�^�̒��O�ɓ���
		gltCntxEdit.insert( itMnItm, gstContextItem[iSel] );
	}



	CntxDlgBuildListUp( hDlg );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e�����폜
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemDel( HWND hDlg )
{
	HWND	hListWnd, hBuildWnd;
	INT		iSel;
	CTXI_LITR	itMnItm;

	hListWnd  = GetDlgItem( hDlg, IDLV_MENU_ALLITEM );
	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	iSel = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 > iSel )	return;	//	�I�����ĂȂ�������I���

	itMnItm = gltCntxEdit.begin();
	advance( itMnItm, iSel );

	gltCntxEdit.erase( itMnItm );

	CntxDlgBuildListUp( hDlg );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e��������
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemSpinUp( HWND hDlg )
{
	HWND	hBuildWnd;
	INT		iSel;
	CTXI_LITR	itTgtItm, itSwpItm;
	CONTEXTITEM	stItem;

	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	iSel = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 >= iSel ){	 return;	}	//	�I�����ĂȂ�����ԏ�Ȃ�I���

	itTgtItm = gltCntxEdit.begin();
	advance( itTgtItm, iSel );
	itSwpItm = itTgtItm;
	itSwpItm--;	//	������͈�O

	StringCchCopy( stItem.atString, MIN_STRING, itTgtItm->atString );
	StringCchCopy( stItem.atAccelerator, MIN_STRING, itTgtItm->atAccelerator );
	stItem.dCommandoID = itTgtItm->dCommandoID;

	gltCntxEdit.erase( itTgtItm );
	gltCntxEdit.insert( itSwpItm, stItem );

	CntxDlgBuildListUp( hDlg );

	ListView_SetItemState( hBuildWnd, --iSel, LVIS_SELECTED, LVIS_SELECTED );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�C�e��������
	@param[in]	hDlg	�_�C�����O�n���h��
*/
VOID CntxDlgItemSpinDown( HWND hDlg )
{
	HWND	hBuildWnd;
	INT		iSel, iCount;
	CTXI_LITR	itTgtItm, itSwpItm;
	CONTEXTITEM	stItem;

	hBuildWnd = GetDlgItem( hDlg, IDLV_MENU_BUILDX );

	iSel = ListView_GetNextItem( hBuildWnd, -1, LVNI_ALL | LVNI_SELECTED );
	if( 0 > iSel )	return;	//	�I�����ĂȂ�������I���

	iCount = ListView_GetItemCount( hBuildWnd );
	if( iSel >= (iCount-1) )	 return;	//	���[�Ȃ�I���

	//	�����̈ʒu�֌W�ɒ���
	itSwpItm = gltCntxEdit.begin();
	advance( itSwpItm, iSel );
	itTgtItm = itSwpItm;
	itTgtItm++;	//	������͈��

	StringCchCopy( stItem.atString, MIN_STRING, itTgtItm->atString );
	StringCchCopy( stItem.atAccelerator, MIN_STRING, itTgtItm->atAccelerator );
	stItem.dCommandoID = itTgtItm->dCommandoID;

	gltCntxEdit.erase( itTgtItm );
	gltCntxEdit.insert( itSwpItm, stItem );

	CntxDlgBuildListUp( hDlg );

	ListView_SetItemState( hBuildWnd, ++iSel, LVIS_SELECTED, LVIS_SELECTED );

	return;
}
//-------------------------------------------------------------------------------------------------

