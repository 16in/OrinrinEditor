/*! @file
	@brief �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
	���̃t�@�C���� OrinrinViewer.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/08/18
*/
//-------------------------------------------------------------------------------------------------

/*
Orinrin Viewer : AsciiArt Viewer for Japanese Only
Copyright (C) 2011 SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.

����{�鍑���p��́�������ꂽ��
*/


#include "stdafx.h"
#include "OrinrinEditor.h"
#include "MaaTemplate.h"
//------------------------------------------------------------------------------------------------------------------------

static CONST TCHAR	gcatLicense[] = {
TEXT("���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F�A���c�ɂ���Ĕ��s���ꂽGNU��ʌ��O���p������(�o�[�W����3���A����ȍ~�̃o�[�W�����̂����ǂꂩ)����߂�����̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B\r\n\r\n")
TEXT("���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�*�ł��B���Ɖ\���̕ۏ؂����ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂��܂߁A�S�����݂��܂���B\r\n\r\n")
TEXT("�ڂ�����GNU��ʌ��O���p���������������������B\r\n\r\n")
TEXT("���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU��ʌ��O���p�������̃R�s�[���ꕔ�󂯎���Ă���͂��ł��B\r\n\r\n")
TEXT("�����󂯎���Ă��Ȃ���΁A<http://www.gnu.org/licenses/> ���������������B\r\n\r\n")
};

static  UINT	gdUseMode;		//!<	�}�����C���N���b�v�w���E�ݒ�ɒ���
static  HWND	ghMaaWnd;		//!<	���ꂽ�E�C���h�E�n���h��
static TCHAR	gatIniPath[MAX_PATH];	//!<	�h�m�h�t�@�C���̈ʒu
//------------------------------------------------------------------------------------------------------------------------

BOOLEAN	SelectFolderDlg( HWND, LPTSTR, UINT_PTR );
//------------------------------------------------------------------------------------------------------------------------


/*!
	�A�v���P�[�V�����̃G���g���|�C���g
	@param[in]	hInstance		���̃��W���[���̃C���X�^���X�n���h��
	@param[in]	hPrevInstance	�O�̃C���X�^���X�B���͖��g�p
	@param[in]	lpCmdLine		�R�}���h���C���B�g�[�N�������͂���ĂȂ��A�����̕�����
	@param[in]	nCmdShow		�N�����̕\����Ԃ������Ă�B�\���Ƃ�����������
	@retval FALSE	�r���I��
*/
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//	TODO: �����ɃR�[�h��}�����Ă��������B
	MSG		msg;
//	HACCEL	hAccelTable;
	INT		msRslt;

#ifdef _DEBUG
	//_CRTDBG_ALLOC_MEM_DF;		// �w�肪�K�v�ȃt���O
	//_CRTDBG_CHECK_ALWAYS_DF;	//	���������`�F�b�N		_CRTDBG_CHECK_EVERY_128_DF
	//_CRTDBG_LEAK_CHECK_DF;		//	�I�����Ƀ��������[�N���`�F�b�N
	//_CRTDBG_DELAY_FREE_MEM_DF;	//	
	//	�����Ŏg�p����t���O���w��
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	INITCOMMONCONTROLSEX	iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC  = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &iccex );

	//	�ݒ�t�@�C���ʒu�m�F
	GetCurrentDirectory( MAX_PATH, gatIniPath );
	PathAppend( gatIniPath, INI_FILE );

	SplitBarClass( hInstance );	//	�X�v���b�g�o�[�̏���

	gdUseMode = InitParamValue( INIT_LOAD, VL_SETMETHOD, MAA_SJISCLIP );

	// �A�v���P�[�V�����̏����������s���܂�:
	ghMaaWnd = MaaTmpltInitialise( hInstance, GetDesktopWindow(), NULL );
	if( !(ghMaaWnd) )	return (-1);

//	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ORINRINVIEWER));

	//	���C�����b�Z�[�W���[�v
	for(;;)
	{
		msRslt = GetMessage( &msg, NULL, 0, 0 );
		if( 1 != msRslt )	break;

//		if( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
//		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
//		}
	}

	return (int)msg.wParam;
}


//-------------------------------------------------------------------------------------------------

//	�o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch( message )
	{
		case WM_INITDIALOG:
			SetDlgItemText( hDlg, IDE_ABOUT_DISP, gcatLicense );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------


#pragma region ("�ݒ���e�Ǐ�")

#ifndef MAA_PROFILE

/*!
	MAA�����t�@�C���̃Z�[�u���[�h�EEditor���ɂ�����
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dCount	�f�[�^�̌�[NULL==ptFile]�^�ʂ��ԍ�
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@param[out]	ptBase	��_�f�B���N�g���[���EMAX_PATH�ł��邱��
	@return		INT	���[�h�F�f�[�^�̌��@�Z�[�u�F�O
*/
INT InitMultipleFile( UINT dMode, UINT dCount, LPTSTR ptFile, LPTSTR ptBase )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	INT	dBuff = 0;

	if( ptFile )
	{
		if( dMode  )	//	���[�h
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%d"), dCount );
			GetPrivateProfileString( TEXT("MaaSubOpen"), atKeyName, TEXT(""), ptFile, MAX_PATH, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Base%d"), dCount );
			GetPrivateProfileString( TEXT("MaaSubOpen"), atKeyName, TEXT(""), ptBase, MAX_PATH, gatIniPath );
		}
		else	//	�Z�[�u
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%d"), dCount );
			WritePrivateProfileString( TEXT("MaaSubOpen"), atKeyName, ptFile, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Base%d"), dCount );
			WritePrivateProfileString( TEXT("MaaSubOpen"), atKeyName, ptBase, gatIniPath );
		}
	}
	else	//	��
	{
		if( dMode  )	//	���[�h
		{
			GetPrivateProfileString( TEXT("MaaSubOpen"), TEXT("Count"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
			dBuff = StrToInt( atBuff );
		}
		else	//	�Z�[�u
		{
			//	��U�Z�N�V��������ɂ���
			ZeroMemory( atBuff, sizeof(atBuff) );
			WritePrivateProfileSection( TEXT("MaaSubOpen"), atBuff, gatIniPath );

			StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), dCount );
			WritePrivateProfileString( TEXT("MaaSubOpen"), TEXT("Count"), atBuff, gatIniPath );
		}

		return dBuff;
	}


	return 0;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�p�����[�^�l�̃Z�[�u���[�h�EEditor���ɂ�����
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�p�����[�^�̎��
	@param[in]	nValue	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return		INT	���[�h�F�l�@�Z�[�u�F�O
*/
INT InitParamValue( UINT dMode, UINT dStyle, INT nValue )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	INT	dBuff = 0;

	switch( dStyle )
	{
		case VL_MAA_SPLIT:		StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSplit") );		break;
		case VL_SETMETHOD:		StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaMethod") );		break;
		case VL_MAATIP_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTip")  );	break;
		case VL_MAATIP_SIZE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTipSize") );	break;
		case VL_MAA_TOPMOST:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaTopMost")  );	break;
		case VL_MAASEP_STYLE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSepLine")  );	break;
		default:	return 0;
	}

	if( dMode )	//	���[�h
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );
		GetPrivateProfileString( TEXT("General"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		dBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );
		WritePrivateProfileString( TEXT("General"), atKeyName, atBuff, gatIniPath );
	}

	return dBuff;
}
//-------------------------------------------------------------------------------------------------


/*!
	������̐ݒ���e���Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dStyle	�p�����[�^�̎��
	@param[in,out]	ptFile	MAX_PATH�ł��邱��
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitParamString( UINT dMode, UINT dStyle, LPTSTR ptFile )
{
	TCHAR	atKeyName[MIN_STRING], atDefault[MAX_PATH];

	if(  !(ptFile) )	return E_INVALIDARG;

	switch( dStyle )
	{
		case VS_PROFILE_NAME:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ProfilePath") );	break;
		case VS_PAGE_FORMAT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageFormat")  );	break;
		default:	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		StringCchCopy( atDefault, MAX_PATH, ptFile );
		GetPrivateProfileString( TEXT("General"), atKeyName, atDefault, ptFile, MAX_PATH, gatIniPath );
	}
	else
	{
		WritePrivateProfileString( TEXT("General"), atKeyName, ptFile, gatIniPath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�ʒu�̃Z�[�u���[�h�EEditor���ɂ�����
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�P�r���[�@�Q����
	@param[in]	pstRect	���[�h���ʂ����邩�A�Z�[�u���e������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitWindowPos( UINT dMode, UINT dStyle, LPRECT pstRect )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];

	if( !pstRect )	return E_INVALIDARG;

	switch( dStyle )
	{
		case  WDP_MAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MaaTmple") );	break;
		default:	SetRect( pstRect , 0, 0, 0, 0 );	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("LEFT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->left   = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("TOP"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->top    = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("RIGHT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->right  = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("BOTTOM"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->bottom = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->left );
		WritePrivateProfileString( atAppName, TEXT("LEFT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->top );
		WritePrivateProfileString( atAppName, TEXT("TOP"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->right );
		WritePrivateProfileString( atAppName, TEXT("RIGHT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->bottom );
		WritePrivateProfileString( atAppName, TEXT("BOTTOM"), atBuff, gatIniPath );
	}
	
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#ifndef MAA_PROFILE

/*!
	MLT�f�B���N�g���̃Z�[�u���[�h�EEditor���ɂ�����
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitMaaFldrOpen( UINT dMode, LPTSTR ptFile )
{

	if(  !(ptFile) )	return 0;

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( TEXT("General"), TEXT("MultiLinePath"), TEXT(""), ptFile, MAX_PATH, gatIniPath );
	}
	else
	{
		WritePrivateProfileString( TEXT("General"), TEXT("MultiLinePath"), ptFile, gatIniPath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

#pragma endregion



#pragma region ("�N���b�v���鏈��")
/*!
	�N���b�v�{�[�h�ɕ�����\��t���EEditor���ɂ�����
	@param[in]	pDatum	�\��t������������E���j��SJIS
	@param[in]	cbSize	������́ANULL���܂񂾃o�C�g��
	@param[in]	dStyle	��`���Ƃ����������w��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT DocClipboardDataSet( LPVOID pDatum, INT cbSize, UINT dStyle )
{
	HGLOBAL	hGlobal;
	HANDLE	hClip;
	LPVOID	pBuffer;
	HRESULT	hRslt;
	UINT	ixFormat, ixSqrFmt;

	//	�I���W�i���t�H�[�}�b�g�����`���Ă���
	ixFormat = RegisterClipboardFormat( CLIP_FORMAT );
	ixSqrFmt = RegisterClipboardFormat( CLIP_SQUARE );

	//	�N���b�v����f�[�^�͋��L�������ɓ����
	hGlobal = GlobalAlloc( GHND, cbSize );
	pBuffer = GlobalLock( hGlobal );
	CopyMemory( pBuffer, pDatum, cbSize );
	GlobalUnlock( hGlobal );

	//	�N���b�v�{�[�h�I�[�|��
	OpenClipboard( NULL );

	//	���g���������Ⴄ
	EmptyClipboard(  );

	//	���L�������Ƀu�b���񂾃f�[�^���N���b�y����
	if( dStyle & D_UNI )	hClip = SetClipboardData( CF_UNICODETEXT, hGlobal );
	else					hClip = SetClipboardData( CF_TEXT, hGlobal );

	if( hClip )
	{
		//	�N���b�|����肭��������A�I���W�i�����ł��L�^���Ă���
		SetClipboardData( ixFormat, hGlobal );
		hRslt = S_OK;
	}
	else
	{
		//	�o�^���s�̏ꍇ�́A�����ŋ��L��������j�󂹂Ȃ�����
		GlobalFree( hGlobal );
		hRslt = E_OUTOFMEMORY;
	}

	//	�N���b�v�{�[�h����
	CloseClipboard(  );

	TRACE( TEXT("COPY DONE") );

	return hRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	MAA����SJIS���󂯎���ď�������EEditor���ɂ�����
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

//	FLASHWINFO	stFshWInfo;

	//	�f�t�H����ł��邩�ǂ���
	if( dMode == gdUseMode ){		uRslt = TRUE;	}
	if( MAA_DEFAULT ==  dMode ){	dMode = gdUseMode;	uRslt = TRUE;	}

	if( MAA_UNICLIP ==  dMode )	//	���j�R�[�h
	{
		ptString = SjisDecodeAlloc( pcCont );	//	���j�R�[�h�ɂ��Ă���
		StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

		//	���j�R�[�h�I�ɃN���b�y
		DocClipboardDataSet( ptString, (cchSize + 1) * 2, D_UNI );

		FREE(ptString);
	}
	else	//	SJIS���قڕW���ł���
	{
		DocClipboardDataSet( pcCont, (cbSize + 1), D_SJIS );
	}


	//ZeroMemory( &stFshWInfo, sizeof(FLASHWINFO) );
	//stFshWInfo.cbSize    = sizeof(FLASHWINFO);	//	���̍\���̂̃T�C�Y
	//stFshWInfo.hwnd      = ghMaaWnd;	//	�����������������E�C���_�E�̃n���h��
	//stFshWInfo.dwFlags   = FLASHW_ALL;	//	�L���v�V�����ƃ^�X�N�o�[�̃{�^������
	//stFshWInfo.uCount    = 2;			//	���������������
	//stFshWInfo.dwTimeout = 0;			//	�Ԋu�B�O�Ńf�t�H���g�I�ȊԊu
	//FlashWindowEx( &stFshWInfo );		//	��������������

	return uRslt;
}
//-------------------------------------------------------------------------------------------------
#pragma endregion


#pragma region ("�ݒ�_�C�����O")

/*!
	���Ղ����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK OptionDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UINT	id;
	INT		dValue, dBuff;
#ifndef MAA_PROFILE
	TCHAR	atPath[MAX_PATH];//, atBuff[SUB_STRING];
#endif

	switch( message )
	{
		case WM_INITDIALOG:
			//	MAA�ꗗ
#ifdef MAA_PROFILE
			Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), TEXT("�`�`�f�B���N�g���̓v���t�@�C������ݒ肵�Ă�") );
			EnableWindow( GetDlgItem(hDlg,IDE_AA_DIRECTORY), FALSE );
			ShowWindow( GetDlgItem(hDlg,IDB_AADIR_SEARCH), SW_HIDE );
#else
			InitMaaFldrOpen( INIT_LOAD, atPath );
			Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), atPath );
#endif
			//	MAA�|�b�v�A�b�v�ɂ���
			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_SIZE, 16 );	//	�T�C�Y�m�F
			if( FONTSZ_REDUCE == dValue )	CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_REDUCE );
			else							CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_NOMAL );

			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );	//	�|�b�p�b�v���邩
			CheckDlgButton( hDlg, IDCB_POPUP_VISIBLE, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�����s�e���v�����N���b�N�����Ƃ��̓���
			dValue = InitParamValue( INIT_LOAD, VL_SETMETHOD, 4 );
			switch( dValue )
			{
				case MAA_UNICLIP:	id =  IDRB_SEL_CLIP_UNI;	break;

				default:
				case MAA_SJISCLIP:	id =  IDRB_SEL_CLIP_SJIS;	break;
			}
			CheckRadioButton( hDlg, IDRB_SEL_INS_EDIT, IDRB_SEL_CLIP_SJIS, id );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
	#ifndef MAA_PROFILE
				case IDB_AADIR_SEARCH:	//	MAA�f�B���N�g���T��
					if( SelectFolderDlg( hDlg, atPath, MAX_PATH ) )
					{
						Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), atPath );
					}
					return (INT_PTR)TRUE;
#endif

				case IDB_APPLY://�K�p
				case IDOK:
	#ifndef MAA_PROFILE
					//	MAA�̃f�B���N�g���[
					Edit_GetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), atPath, MAX_PATH );
					InitMaaFldrOpen( INIT_SAVE, atPath );
	#endif

					//	MAA�|�b�v�A�b�v�ɂ���
					dValue = FONTSZ_NORMAL;
					if( IsDlgButtonChecked( hDlg, IDRB_POPUP_REDUCE ) ){	dValue =  FONTSZ_REDUCE;	}
					InitParamValue( INIT_SAVE, VL_MAATIP_SIZE, dValue );
					dBuff = IsDlgButtonChecked( hDlg, IDCB_POPUP_VISIBLE );
					AaItemsTipSizeChange( dValue, dBuff );
					InitParamValue( INIT_SAVE, VL_MAATIP_VIEW, dBuff );

					//	MAA�̑���
					if( IsDlgButtonChecked( hDlg, IDRB_SEL_CLIP_UNI ) ){	dValue = MAA_UNICLIP;	}
					else{	dValue = MAA_SJISCLIP;	}
					InitParamValue( INIT_SAVE, VL_SETMETHOD, dValue );
					gdUseMode = dValue;

					//	�n�j�Ȃ�����Ⴄ
					if( IDOK == id ){	EndDialog( hDlg, IDOK );	}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					EndDialog( hDlg, IDCANCEL );
					return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		default:	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�B���N�g���I���_�C�A���O�̕\��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	ptSelFolder	�f�B���N�g����������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchLen		�o�b�t�@�̕������B�o�C�g������Ȃ���
	@return		��O�F�f�B���N�g���Ƃ����@�O�F�L�����Z������
*/
BOOLEAN SelectDirectoryDlg( HWND hWnd, LPTSTR ptSelFolder, UINT_PTR cchLen )
{
	BROWSEINFO		stBrowseInfo;
	LPITEMIDLIST	pstItemIDList;
	TCHAR	atDisplayName[MAX_PATH];

	if( !(ptSelFolder) )	return FALSE;
	ZeroMemory( ptSelFolder, sizeof(TCHAR) * cchLen );

	//	BROWSEINFO�\���̂ɒl��ݒ�
	stBrowseInfo.hwndOwner		 = hWnd;	//	�_�C�A���O�̐e�E�C���h�E�̃n���h��
	stBrowseInfo.pidlRoot		 = NULL;	//	���[�g�f�B���N�g��������ITEMIDLIST�̃|�C���^�ENULL�̏ꍇ�f�X�N�g�b�v
	stBrowseInfo.pszDisplayName	 = atDisplayName;	//	�I�����ꂽ�f�B���N�g�������󂯎��o�b�t�@�̃|�C���^
	stBrowseInfo.lpszTitle		 = TEXT("�`�`�̓����Ă�f�B���N�g����I������́[�I");	//	�c���[�r���[�̏㕔�ɕ\������镶����
	stBrowseInfo.ulFlags		 = BIF_RETURNONLYFSDIRS;	//	�\�������f�B���N�g���̎�ނ������t���O
	stBrowseInfo.lpfn			 = NULL;		//	BrowseCallbackProc�֐��̃|�C���^
	stBrowseInfo.lParam			 = (LPARAM)0;	//	�R�[���o�b�N�֐��ɓn���l

	//	�f�B���N�g���I���_�C�A���O��\��
	pstItemIDList = SHBrowseForFolder( &stBrowseInfo );
	if( !(pstItemIDList) )
	{
		//	�߂�l��NULL�̏ꍇ�A�f�B���N�g�����I�����ꂸ�Ƀ_�C�A���O������ꂽ�Ƃ�������
		return FALSE;
	}
	else
	{
		//	ItemIDList���p�X���ɕϊ�
		if( !SHGetPathFromIDList( pstItemIDList, atDisplayName ) )
		{
			//	�G���[����
			return FALSE;
		}
		//	atDisplayName�ɑI�����ꂽ�f�B���N�g���̃p�X�������Ă�
		StringCchCopy( ptSelFolder, cchLen, atDisplayName );

		//	pstItemIDList���J�������߂�
		CoTaskMemFree( pstItemIDList );
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------
#pragma endregion


#ifdef _DEBUG
VOID OutputDebugStringPlus( DWORD rixError, LPSTR pcFile, INT rdLine, LPSTR pcFunc, LPTSTR ptFormat, ... )
{
	va_list	argp;
	TCHAR	atBuf[MAX_PATH], atOut[MAX_PATH], atFiFu[MAX_PATH], atErrMsg[MAX_PATH];
	CHAR	acFile[MAX_PATH], acFiFu[MAX_PATH];
	UINT	length;

	StringCchCopyA( acFile, MAX_PATH, pcFile );
	PathStripPathA( acFile );

	StringCchPrintfA( acFiFu, MAX_PATH, ("%s %d %s"), acFile, rdLine, pcFunc );
	length = (UINT)strlen( acFiFu );

	ZeroMemory( atFiFu, sizeof(atFiFu) );
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, acFiFu, length, atFiFu, MAX_PATH );
	//	�R�[�h�y�[�W,�����̎�ނ��w�肷��t���O,�}�b�v��������̃A�h���X,�}�b�v��������̃o�C�g��,
	//	�}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X,�o�b�t�@�̃T�C�Y

	va_start(argp, ptFormat);
	StringCchVPrintf( atBuf, MAX_PATH, ptFormat, argp );
	va_end( argp );

	StringCchPrintf( atOut, MAX_PATH, TEXT("%s @ %s\r\n"), atBuf, atFiFu );//

	OutputDebugString( atOut );

	if( rixError )
	{
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, rixError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), atErrMsg, MAX_PATH, NULL );
		//	���b�Z�[�W�ɂ͉��s���܂܂�Ă���悤��
		StringCchPrintf( atBuf, MAX_PATH, TEXT("[%d]%s"), rixError, atErrMsg );//

		OutputDebugString( atBuf );
		SetLastError( 0 );
	}
}
//-------------------------------------------------------------------------------------------------
#endif

