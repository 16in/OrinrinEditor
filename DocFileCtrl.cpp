/*! @file
	@brief �t�@�C���ɕۑ��E�J���̖ʓ|���܂�
	���̃t�@�C���� DocFileCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/27
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

//	TODO:	�ۑ�����Ƃ��A�������O�̃t�@�C��������΁A���������Ď����Ńo�b�N�A�b�v�����

//	TODO:	�ۑ�����Ƃ��A���j�R�[�h��SJIS���I���o����悤�ɁE�f�t�H��SJIS�ł�����

#ifdef MULTI_FILE
extern list<ONEFILE>	gltMultiFiles;	//!<	�����t�@�C���ێ�

extern FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
#define gstFile	(*gitFileIt)	//!<	�C�e���[�^���\���̂ƌ��Ȃ�
#else
extern ONEFILE	gstFile;			//!<	�t�@�C���P�ʁE�����t�@�C���ɂ͂ǂ�����đΉ����H
#endif
extern INT		gixFocusPage;		//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  UINT	gbAutoBUmsg;	//	�����o�b�N�A�b�v���b�Z�[�W�o�����H

static TCHAR	gatBackUpDirty[MAX_PATH];

//-------------------------------------------------------------------------------------------------

#ifdef MULTI_FILE
INT	DocAstSeparatorGetAlloc( INT, UINT, LPVOID *, FILES_ITR );
#else
INT	DocAstSeparatorGetAlloc( INT, UINT, LPVOID * );
#endif
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C������ǂݍ���
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileOpen( HWND hWnd )
{
	OPENFILENAME	stOpenFile;
	BOOLEAN	bOpened;
	LPARAM	dNumber;
	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];


	ZeroMemory( &stOpenFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );

	stOpenFile.lStructSize     = sizeof(OPENFILENAME);
	stOpenFile.hwndOwner       = hWnd;
	stOpenFile.lpstrFilter     = TEXT("�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0�S�Ă̌`��(*.*)\0*.*\0\0");
	stOpenFile.nFilterIndex    = 1;
	stOpenFile.lpstrFile       = atFilePath;
	stOpenFile.nMaxFile        = MAX_PATH;
	stOpenFile.lpstrFileTitle  = atFileName;
	stOpenFile.nMaxFileTitle   = MAX_STRING;
//	stOpenFile.lpstrInitialDir = 
	stOpenFile.lpstrTitle      = TEXT("�J���t�@�C�����w�肷�邩����[");
	stOpenFile.Flags           = OFN_EXPLORER | OFN_HIDEREADONLY;
	stOpenFile.lpstrDefExt     = TEXT("mlt");

	//������ FileOpenDialogue ���o��
	bOpened = GetOpenFileName( &stOpenFile );

	ViewFocusSet(  );

	if( !(bOpened) ){	return  E_ABORT;	}	//	�L�����Z�����Ă��牽�����Ȃ�

	dNumber = DocFileInflate( atFilePath );	//	�J���Ē��g�W�J
	if( !(dNumber) )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ�����������[�I�H"), NULL, MB_OK | MB_ICONERROR );
	}
#ifdef MULTI_FILE
	else{	MultiFileTabAppend( dNumber, atFilePath );	}
#endif


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�b�N�A�b�v�f�B���N�g���[���m��
	@param[in]	ptCurrent	��f�B���N�g��
*/
VOID DocBackupDirectoryInit( LPTSTR ptCurrent )
{
	StringCchCopy( gatBackUpDirty, MAX_PATH, ptCurrent );
	PathAppend( gatBackUpDirty, BACKUP_DIR );
	CreateDirectory( gatBackUpDirty, NULL );
	//	�߂Ƀf�B���N�g�����������生�������s���邾���Ȃ̂Ŗ�薳��

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���^�[�o���Ŏ����ۑ�
	@param[in]	hWnd	�E�C���h�E�n���h���E�ǂ��̃E�C���h�E�n���h�����͔�Œ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileBackup( HWND hWnd )
{
	CONST  TCHAR	aatExte[3][5] = { {TEXT(".ast")}, {TEXT(".mlt")}, {TEXT(".txt")} };

	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atBuffer[MAX_PATH];

	HANDLE	hFile;
	DWORD	wrote;

	LPTSTR	ptExten;	//	�t�@�C�����̊g���q
	TCHAR	atExBuf[10];

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize, iNullTmt, iCrLf;

	LPVOID	pbSplit;
	UINT	cbSplSz;

	INT		isAST, isMLT, ddExten;

	UINT_PTR	iPages, i;	//	�Ő�

#ifdef MULTI_FILE
	FILES_ITR	itFile;
#endif

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );
	ZeroMemory( atBuffer,  sizeof(atBuffer) );

//�����t�@�C���A�e�t�@�C�����Z�[�u����ɂ́H
#ifdef MULTI_FILE
	for( itFile = gltMultiFiles.begin(); itFile != gltMultiFiles.end(); itFile++ )
	{
		iPages = itFile->vcCont.size( );	//	���Ő�
#else
		iPages = gstFile.vcCont.size( );	//	���Ő�
#endif
		if( 1 >= iPages )	isMLT = FALSE;
		else				isMLT = TRUE;

#ifdef MULTI_FILE
		isAST = PageListIsNamed( itFile );	//	�łɖ��O���t���Ă�H
#else
		isAST = PageListIsNamed(  );	//	�łɖ��O���t���Ă�H
#endif
		if( isAST ){		ddExten = 0;	}	//	AST
		else if( isMLT ){	ddExten = 1;	}	//	MLT
		else{				ddExten = 2;	}	//	TXT

#ifdef MULTI_FILE
		StringCchCopy( atBuffer, MAX_PATH, itFile->atFileName );
#else
		StringCchCopy( atBuffer, MAX_PATH, gstFile.atFileName );
#endif
		if( atBuffer[0] == NULL )	//	���̖��ݒ���
		{
#ifdef MULTI_FILE
			StringCchCopy( atFileName, MAX_STRING, itFile->atDummyName );
#else
			StringCchCopy( atFileName, MAX_STRING, NAMELESS_DUMMY );
#endif
		}
		else
		{
			PathStripPath( atBuffer );
			StringCchCopy( atFileName, MAX_STRING, atBuffer );
		}

		//	�g���q���m�F�E�h�b�g���݂���`��
		ptExten = PathFindExtension( atFileName );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
		if( 0 == *ptExten )
		{
			//	�g���q�w�肪�Ȃ��Ȃ炻�̂܂ܑΉ��̂���������
			StringCchCopy( ptExten, 5, aatExte[ddExten] );
		}
		else	//	�����̊g���q����������
		{
			StringCchCopy( atExBuf, 10, ptExten );
			CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

			if( isAST )	//	AST�͗D��I�ɓK�p
			{
				if( StrCmp( atExBuf, aatExte[0] ) )	//	����AST����Ȃ�������ύX
				{
					StringCchCopy( ptExten, 5, aatExte[0] );
				}
			}
			else if( isMLT )	//	���O�������Ǖ����łȂ�MLT����Ȃ��ƃ_��
			{
				if( StrCmp( atExBuf, aatExte[1] ) )	//	����MLT����Ȃ�������ύX
				{
					StringCchCopy( ptExten, 5, aatExte[1] );
				}
			}
			//	�ꖇ�Ȃ�ATXT�ł�MLT�ł��C�ɂ��Ȃ��Ă悩�΂�
		}

		StringCchCopy( atFilePath, MAX_STRING, gatBackUpDirty );
		PathAppend( atFilePath, atFileName );	//	Backup�t�@�C����


		hFile = CreateFile( atFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		if( INVALID_HANDLE_VALUE == hFile )
		{
			NotifyBalloonExist( NULL, TEXT("���������A�o�b�N�A�b�v�o���Ȃ������̂ł��B��������"), TEXT("��������"), NIIF_ERROR );
			//	gbAutoBUmsg	�o�b�N�A�b�v�o���Ȃ��������b�Z�[�W�͏�ɕ\����������
			return E_HANDLE;
		}

		iNullTmt = 1;
		iCrLf = CH_CRLF_CCH;
		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

		if( isAST )
		{
			pbSplit = NULL;
		}
		else
		{
			pbSplit = malloc( 30 );
			ZeroMemory( pbSplit, 30 );

			cbSplSz = MLT_SPRT_CCH + CH_CRLF_CCH;
			StringCchPrintfA( (LPSTR)pbSplit, 30, ("%s%s"), MLT_SEPARATERA, CH_CRLFA );
		}

		for( i = 0; iPages > i; i++ )	//	�S�ŕۑ�
		{
			if( isAST )
			{
#ifdef MULTI_FILE
				cbSplSz = DocAstSeparatorGetAlloc( i, D_SJIS, &pbSplit, itFile );
#else
				cbSplSz = DocAstSeparatorGetAlloc( i, D_SJIS, &pbSplit );
#endif
				WriteFile( hFile , pbSplit, (cbSplSz- iNullTmt), &wrote, NULL );
				FREE(pbSplit);
			}
			else
			{
				if( 1 <= i ){	WriteFile( hFile , pbSplit, cbSplSz, &wrote, NULL );	}
			}

#ifdef MULTI_FILE
			iByteSize = DocAllTextGetAlloc( i, D_SJIS, &pBuffer, itFile );
#else
			iByteSize = DocAllTextGetAlloc( i, D_SJIS, &pBuffer );
#endif
			if( (i+1) == iPages ){	iByteSize -=  iCrLf;	}
			//	�ŏI�ł̖��[�̉��s�͕s�v�̂͂�
			WriteFile( hFile, pBuffer, iByteSize - iNullTmt, &wrote, NULL );

			FREE( pBuffer );
		}

		SetEndOfFile( hFile );
		CloseHandle( hFile );

		FREE( pbSplit );
#ifdef MULTI_FILE
	}
#endif
	if( gbAutoBUmsg ){	NotifyBalloonExist( NULL, TEXT("��ƒ��̃t�@�C�����o�b�N�A�b�v�ۑ������̂ł��B��������"), TEXT("��������"), NIIF_INFO );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���ɕۑ�����
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	bStyle	SJIS�����j�R�[�h���A�㏑�������l�[����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileSave( HWND hWnd, UINT bStyle )
{
	//	���j�R�[�h���[�h�ɂ��āE�`���w��̎��̑I�����ɂ��ꂿ�Ⴄ��

	CONST  TCHAR	aatExte[3][5] = { {TEXT(".ast")}, {TEXT(".mlt")}, {TEXT(".txt")} };
	CONST  WCHAR	rtHead = 0xFEFF;	//	���j�R�[�h�e�L�X�g�w�b�_

	SYSTEMTIME		stSysTile;
	OPENFILENAME	stSaveFile;

	BOOLEAN	bOpened;

	TCHAR	atFilePath[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atBuffer[MAX_STRING];

	HANDLE	hFile;
	DWORD	wrote;

	LPTSTR	ptExten;	//	�t�@�C�����̊g���q
	TCHAR	atExBuf[10];

	LPVOID	pBuffer;	//	������o�b�t�@�p�|�C���^�[
	INT		iByteSize, iNullTmt, iCrLf;

	LPVOID	pbSplit;
	UINT	cbSplSz;

	INT		isAST, isMLT, ddExten;
	BOOLEAN	bExtChg =FALSE, bLastChg = FALSE;

	UINT_PTR	iPages, i;	//	�Ő�

	ZeroMemory( &stSaveFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );
	ZeroMemory( atBuffer,  sizeof(atBuffer) );

	//	�ۑ����͏�ɑI�����Ă���t�@�C����ۑ�

	iPages = gstFile.vcCont.size( );	//	���Ő�
	if( 1 >= iPages )	isMLT = FALSE;
	else				isMLT = TRUE;

//�����̊g���q��AST�Ȃ�A�����D�悷��

#ifdef MULTI_FILE
	isAST = PageListIsNamed( gitFileIt );	//	�łɖ��O���t���Ă�H
#else
	isAST = PageListIsNamed(  );	//	�łɖ��O���t���Ă�H
#endif
	if( isAST ){		ddExten = 0;	}	//	AST
	else if( isMLT ){	ddExten = 1;	}	//	MLT
	else{				ddExten = 2;	}	//	TXT

	GetLocalTime( &stSysTile );

	StringCchCopy( atFilePath, MAX_PATH, gstFile.atFileName );

	//	���l�[�����A�t�@�C����������������ۑ��_�C�����O�J��
	if( (bStyle & D_RENAME) || NULL == gstFile.atFileName[0] )
	{

		//������ FileSaveDialogue ���o��
		stSaveFile.lStructSize     = sizeof(OPENFILENAME);
		stSaveFile.hwndOwner       = hWnd;
		stSaveFile.lpstrFilter     = TEXT("�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0���O�t�A�X�L�[�A�[�g�t�@�C�� ( ast )\0*.ast\0�S�Ă̌`��(*.*)\0*.*\0\0");
		stSaveFile.nFilterIndex    = 1;	//	�f�t�H�̃t�B���^�I����
		stSaveFile.lpstrFile       = atFilePath;
		stSaveFile.nMaxFile        = MAX_PATH;
		stSaveFile.lpstrFileTitle  = atFileName;
		stSaveFile.nMaxFileTitle   = MAX_STRING;
//		stSaveFile.lpstrInitialDir = 
		stSaveFile.lpstrTitle      = TEXT("�ۑ�����t�@�C�������w�肷��ł���");
		stSaveFile.Flags           = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
		//if( 1 >= iPages )	stSaveFile.lpstrDefExt = TEXT("txt");
		//else				stSaveFile.lpstrDefExt = TEXT("mlt");

		bOpened = GetSaveFileName( &stSaveFile );

		ViewFocusSet(  );

		if( !(bOpened) ){	return  E_ABORT;	}
		//	�L�����Z�����Ă��牽�����Ȃ�

		//�I�������t�B���^�ԍ����P�C���f�b�N�X�œ���
//		if( 1 == stSaveFile.nFilterIndex ){}	//	MLT��TXT�̏ꍇ

		bLastChg = TRUE;	//	20110713	�V�K�ۑ��E���l�[���ۑ�
	}

	//	�g���q���m�F�E�h�b�g���݂���`��
	ptExten = PathFindExtension( atFilePath );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	if( 0 == *ptExten )
	{
		//	�g���q�w�肪�Ȃ��Ȃ炻�̂܂ܑΉ��̂���������
		StringCchCopy( ptExten, 5, aatExte[ddExten] );
		bExtChg = TRUE;
	}
	else	//	�����̊g���q����������
	{
		StringCchCopy( atExBuf, 10, ptExten );
		CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

		//	�����̊g���q���AAST�Ȃ炻���D�悷��
		if( !( StrCmp( atExBuf, aatExte[0] ) ) )	//	AST�ł���Ȃ�
		{
			//	AST�`�����ێ�����
			isAST = TRUE;	isMLT = FALSE;	ddExten = 0;
		}

		if( isAST )	//	AST�͗D��I�ɓK�p
		{
			if( StrCmp( atExBuf , aatExte[0] ) )	//	����AST����Ȃ�������ύX
			{
				StringCchCopy( ptExten, 5, aatExte[0] );
				bExtChg = TRUE;
			}
		}
		else if( isMLT )	//	���O�������Ǖ����łȂ�MLT����Ȃ��ƃ_��
		{
			if( StrCmp( atExBuf , aatExte[1] ) )	//	����MLT����Ȃ�������ύX
			{
				StringCchCopy( ptExten, 5, aatExte[1] );
				bExtChg = TRUE;
			}
		}
		//	�ꖇ�Ȃ�ATXT�ł�MLT�ł��C�ɂ��Ȃ��Ă悩�΂�
	}


	//	�㏑���Ȃ璼�O�̏�Ԃ̃o�b�N�A�b�v�Ƃ����ׂ�
	//	�������O�̃t�@�C��������΁A���Ă��Ƃ�

	StringCchCopy( gstFile.atFileName, MAX_PATH, atFilePath );

	hFile = CreateFile( atFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ������ł���"), NULL, MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}

	iNullTmt = 1;
	iCrLf = CH_CRLF_CCH;
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	//	���j�R�[�h���[�h�Ȃ�ABOM������
	if( bStyle & D_UNI )
	{
		WriteFile( hFile, &rtHead, 2, &wrote, NULL );
		iNullTmt = 2;
		iCrLf *= 2;
	}


	if( isAST )
	{
		pbSplit = NULL;
	}
	else
	{
		pbSplit = malloc( 30 );
		ZeroMemory( pbSplit, 30 );

		if( bStyle & D_UNI )
		{
			cbSplSz = (MLT_SPRT_CCH + CH_CRLF_CCH) * 2;
			StringCchPrintfW( (LPTSTR)pbSplit, 15, TEXT("%s%s"), MLT_SEPARATERW, CH_CRLFW );
		}
		else
		{
			cbSplSz = MLT_SPRT_CCH + CH_CRLF_CCH;
			StringCchPrintfA( (LPSTR)pbSplit, 30, ("%s%s"), MLT_SEPARATERA, CH_CRLFA );
		}
	}

	for( i = 0; iPages > i; i++ )	//	�S�ŕۑ�
	{
		if( isAST )
		{
#ifdef MULTI_FILE
			cbSplSz = DocAstSeparatorGetAlloc( i, bStyle, &pbSplit, gitFileIt );
#else
			cbSplSz = DocAstSeparatorGetAlloc( i, bStyle, &pbSplit );
#endif
			WriteFile( hFile , pbSplit, (cbSplSz- iNullTmt), &wrote, NULL );
			FREE(pbSplit);
		}
		else
		{
			if( 1 <= i ){	WriteFile( hFile , pbSplit, cbSplSz, &wrote, NULL );	}
		}

#ifdef MULTI_FILE
		iByteSize = DocAllTextGetAlloc( i, bStyle, &pBuffer, gitFileIt );
#else
		iByteSize = DocAllTextGetAlloc( i, bStyle, &pBuffer );
#endif
		if( (i+1) == iPages ){	iByteSize -=  iCrLf;	}
		//	�ŏI�ł̖��[�̉��s�͕s�v�̂͂�
		WriteFile( hFile, pBuffer, iByteSize - iNullTmt, &wrote, NULL );

		FREE( pBuffer );
	}

	SetEndOfFile( hFile );
	CloseHandle( hFile );

	FREE( pbSplit );

	DocModifyContent( FALSE  );


	//	�Ȃ񂩃��b�Z�[�W
	if( bExtChg )
	{
		InitLastOpen( INIT_SAVE, atFilePath );	//	���X�g�I�[�|��������
#ifdef MULTI_FILE
		MultiFileTabRename( gstFile.dUnique, atFilePath );	//	�^�u���̕ύX
#endif
		AppTitleChange( atFilePath );
		StringCchPrintf( atBuffer, MAX_STRING, TEXT("�g���q�� %s �ɂ��ĕۑ������̂ł��B��������"), aatExte[ddExten] );
		NotifyBalloonExist( NULL, atBuffer, TEXT("�g���q��ύX�����̂ł�"), NIIF_INFO );
	}
	else
	{
		//	20110713	�V�K�����l�[�����Ă��烉�X�g�I�[�|��������
		if( bLastChg )
		{
			InitLastOpen( INIT_SAVE, atFilePath );
#ifdef MULTI_FILE
			MultiFileTabRename( gstFile.dUnique, atFilePath );	//	�^�u���̕ύX
#endif
			AppTitleChange( atFilePath );
		}

		NotifyBalloonExist( NULL, TEXT("�ۑ������̂ł��B��������"), TEXT("�t�@�C���o�͂����̂ł�"), NIIF_INFO );
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W���O��AST��؂�t���Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	dPage	�m�ۂ���Ŕԍ�
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS�ŁA��`���ǂ���
	@param[out]	pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
#ifdef MULTI_FILE
INT DocAstSeparatorGetAlloc( INT dPage, UINT bStyle, LPVOID *pText, FILES_ITR itFile )
#else
INT DocAstSeparatorGetAlloc( INT dPage, UINT bStyle, LPVOID *pText )
#endif
{
	UINT	cchSize, cbSize;
	TCHAR	atBuffer[MAX_PATH];

#ifdef MULTI_FILE
	StringCchPrintf( atBuffer, MAX_PATH, TEXT("[AA][%s]\r\n"), itFile->vcCont.at( dPage ).atPageName );
#else
	StringCchPrintf( atBuffer, MAX_PATH, TEXT("[AA][%s]\r\n"), gstFile.vcCont.at( dPage ).atPageName );
#endif
	StringCchLength( atBuffer, MAX_PATH, &cchSize );

	if( bStyle & D_UNI )
	{
		cbSize = (cchSize + 1) * 2;	//	NULL�^�[�~�l�[�^

		*pText = (LPTSTR)malloc( cbSize );
		ZeroMemory( *pText, cbSize );
		StringCchCopy( (LPTSTR)(*pText), cbSize, atBuffer );
	}
	else
	{
		cbSize = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atBuffer, cchSize, NULL, 0, NULL, NULL );
		cbSize++;	//	NULL�^�[�~�l�[�^
		*pText = (LPSTR)malloc( cbSize );
		ZeroMemory( *pText, cbSize );
		WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atBuffer, cchSize, (LPSTR)(*pText), cbSize, NULL, NULL );
	}

	return cbSize;
}
//-------------------------------------------------------------------------------------------------Yippee-ki-yay!

/*!
	�y�[�W�S�̂𕶎���Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	dPage	�m�ۂ���Ŕԍ�
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS�ŁA��`���ǂ���
	@param[out]	pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
#ifdef MULTI_FILE
INT DocAllTextGetAlloc( INT dPage, UINT bStyle, LPVOID *pText, FILES_ITR itFile )
#else
INT DocAllTextGetAlloc( INT dPage, UINT bStyle, LPVOID *pText )
#endif
{
	UINT_PTR	iLines, i, iLetters, j, iSize;

	string	srString;
	wstring	wsString;

	srString.clear( );
	wsString.clear( );

#ifdef MULTI_FILE
	iLines = itFile->vcCont.at( dPage ).vcPage.size( );
#else
	iLines = gstFile.vcCont.at( dPage ).vcPage.size( );
#endif
	//	�S�����𒸂�
	for( i = 0; iLines > i; i++ )
	{
#ifdef MULTI_FILE
		iLetters = itFile->vcCont.at( dPage ).vcPage.at( i ).vcLine.size( );
#else
		iLetters = gstFile.vcCont.at( dPage ).vcPage.at( i ).vcLine.size( );
#endif
		for( j = 0; iLetters > j; j++ )
		{
#ifdef MULTI_FILE
			srString +=  string( itFile->vcCont.at( dPage ).vcPage.at( i ).vcLine.at( j ).acSjis );
			wsString += itFile->vcCont.at( dPage ).vcPage.at( i ).vcLine.at( j ).cchMozi;
#else
			srString +=  string( gstFile.vcCont.at( dPage ).vcPage.at( i ).vcLine.at( j ).acSjis );
			wsString += gstFile.vcCont.at( dPage ).vcPage.at( i ).vcLine.at( j ).cchMozi;
#endif
		}
		
		if( !(1 == iLines && 0 == iLetters) )	//	��s���땶���͋�ł���
		{
			//	�\����A��ɉ��s�͕K�v
			srString +=  string( CH_CRLFA );
			wsString += wstring( CH_CRLFW );
		}
	}

	if( bStyle & D_UNI )
	{
		iSize = wsString.size( ) + 1;	//	NULL�^�[�~�l�[�^
		iSize *= 2;	//	���j�R�[�h�Ȃ̂Ńo�C�g���͂Q�{�ł���

		if( pText )
		{
			*pText = (LPTSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopy( (LPTSTR)(*pText), iSize, wsString.c_str( ) );
		}
	}
	else
	{
		iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^

		if( pText )
		{
			*pText = (LPSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopyA( (LPSTR)(*pText), iSize, srString.c_str( ) );
		}
	}

	return (INT)iSize;
}
//-------------------------------------------------------------------------------------------------

/*!
	�摜�ŕł�ۑ��EBMP��PNG�AJPEG�͌����ĂȂ�
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	bStyle	�Ȃ񂩃t���O�E�Ƃ�₦�����g�p
	@param[in]	hFont	�`��Ɏg���t�H���g�󂯎��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocImageSave( HWND hWnd, UINT bStyle, HFONT hFont )
{

	LPVOID	pBuffer;
	UINT	dLines;
	INT		iDotX, iDotY, iByteSize, bType;
	UINT_PTR	cchSize;
	RECT	rect;

	BOOL	bOpened;
	OPENFILENAME	stSaveFile;

	TCHAR	atOutName[MAX_PATH], atFileName[MAX_STRING];
	TCHAR	atPart[MIN_STRING];

	HDC		hdc, hMemDC;
	HBITMAP	hBitmap,hOldBmp;
	HFONT	hOldFont;



	//	�Ƃ肠���Ã_�~�[���O�Ńt�@�C��
	StringCchCopy( atOutName, MAX_PATH, gitFileIt->atFileName );
	//	�g���q���I����D�悷��悤�ɂ����Ⴄ
	PathRemoveExtension( atOutName );	//	�g���q���ځ`��

	StringCchPrintf( atPart, MIN_STRING, TEXT("_Page%d"), gixFocusPage );
	StringCchCat( atOutName, MAX_PATH, atPart );

	ZeroMemory( &stSaveFile, sizeof(OPENFILENAME) );
	stSaveFile.lStructSize     = sizeof(OPENFILENAME);
	stSaveFile.hwndOwner       = hWnd;
	stSaveFile.lpstrFilter     = TEXT("BMP �t�@�C�� ( *.bmp )\0*.bmp\0PNG �t�@�C�� ( *.png )\0*.png\0\0");
	stSaveFile.nFilterIndex    = 1;	//	�f�t�H�̃t�B���^�I����
	stSaveFile.lpstrFile       = atOutName;
	stSaveFile.nMaxFile        = MAX_PATH;
	stSaveFile.lpstrFileTitle  = atFileName;
	stSaveFile.nMaxFileTitle   = MAX_STRING;
//		stSaveFile.lpstrInitialDir = 
	stSaveFile.lpstrTitle      = TEXT("�ۑ�����t�@�C�����ƌ`�����w�肷��ł���");
	stSaveFile.Flags           = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	bOpened = GetSaveFileName( &stSaveFile );

	ViewFocusSet(  );

	if( !(bOpened) ){	return  E_ABORT;	}

	//�I�������t�B���^�ԍ����P�C���f�b�N�X�œ���
	switch( stSaveFile.nFilterIndex )
	{
		default:	bType = ISAVE_BMP;	break;
		case  2:	bType = ISAVE_PNG;	break;
	}

	//	�ŏ��m��
	dLines = DocPageParamGet( NULL, NULL );
	iDotX  = DocPageMaxDotGet( -1, -1 );
	iDotY  = dLines * LINE_HEIGHT;
	//	������Ɨ]�T����Ƃ�
	iDotX += 8;
	iDotY += 8;

	SetRect( &rect, 4, 4, iDotX - 4, iDotY- 4 );

	TRACE( TEXT("�T�C�Y %d x %d"), iDotX, iDotY );

	iByteSize = DocAllTextGetAlloc( gixFocusPage, D_UNI, &pBuffer, gitFileIt );
	StringCchLength( (LPTSTR)pBuffer, STRSAFE_MAX_CCH, &cchSize );

	//	�`��p�r�b�g�}�b�v�쐬
	hdc = GetDC( hWnd );

	hBitmap = CreateCompatibleBitmap( hdc, iDotX, iDotY );
	hMemDC  = CreateCompatibleDC( hdc );

	hOldBmp  = SelectBitmap( hMemDC, hBitmap );
	hOldFont = SelectFont( hMemDC, hFont );

	PatBlt( hMemDC, 0, 0, iDotX, iDotY, WHITENESS );

	ReleaseDC( hWnd, hdc );

	DrawText( hMemDC, (LPTSTR)pBuffer, cchSize, &rect, DT_LEFT | DT_NOPREFIX | DT_NOCLIP | DT_WORDBREAK );

	FREE(pBuffer);

	if( SUCCEEDED( ImageFileSaveDC( hMemDC, atOutName, bType ) ) )
	{
		//	��������
		TRACE( TEXT("�ۑ� %s"), atOutName );
	}
	else
	{
		//	�����ς�
		TRACE( TEXT("���s %s"), atOutName );
	}

	SelectBitmap( hMemDC, hOldBmp );
	DeleteBitmap( hBitmap );

	SelectFont( hMemDC, hOldFont );

	DeleteDC( hMemDC );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

