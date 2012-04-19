/*! @file
	@brief �t�@�C���ɕۑ��E�J���̖ʓ|���܂�
	���̃t�@�C���� DocFileCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/27
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

//	TODO:	�ۑ�����Ƃ��A�������O�̃t�@�C��������΁A���������Ď����Ńo�b�N�A�b�v�����

//	TODO:	�ۑ�����Ƃ��A���j�R�[�h��SJIS���I���o����悤�ɁE�f�t�H��SJIS�ł�����


extern list<ONEFILE>	gltMultiFiles;	//!<	�����t�@�C���ێ�

extern FILES_ITR	gitFileIt;		//!<	�����Ă�t�@�C���̖{��
//#define gstFile	(*gitFileIt)	//!<	�C�e���[�^���\���̂ƌ��Ȃ�

extern INT		gixFocusPage;		//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  UINT	gbAutoBUmsg;		//	�����o�b�N�A�b�v���b�Z�[�W�o�����H

static TCHAR	gatBackUpDirty[MAX_PATH];

//-------------------------------------------------------------------------------------------------

INT	DocAstSeparatorGetAlloc( INT, UINT, LPVOID *, FILES_ITR );

INT	DocUnicode2UTF8( LPVOID * );
//-------------------------------------------------------------------------------------------------


/*!
	�Y������t�@�C���͊J���ς�
	@param[in]	ptFile	�m�F�������t�@�C����
	@return	UINT	���F�����@�P�ȏ�F�q�b�g�������UNIQUE�ԍ�
*/
LPARAM DocOpendFileCheck( LPTSTR ptFile )
{
	FILES_ITR	itFile;

	for( itFile = gltMultiFiles.begin(); gltMultiFiles.end() != itFile; itFile++ )
	{
		//	�q�b�g�����炻��ł���
		if( !( StrCmp( itFile->atFileName, ptFile ) ) ){	return  itFile->dUnique;	}
	}

	return -1;
}
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
//	LPARAM	dNumber;
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

	DocDoOpenFile( hWnd, atFilePath );	//	�J���Ē��g�W�J

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C�������󂯂āA�I�[�|����������
	@param[in]	ptFile	�J���t�@�C���t���p�X
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT DocDoOpenFile( HWND hWnd, LPTSTR ptFile )
{
	LPARAM	dNumber;

	//	�����̃t�@�C�����J�����Ƃ����炻�������t�H�[�J�X���邾���ɂ���̂��ǂ��͂�
	dNumber = DocOpendFileCheck( ptFile );
	if( 1 <= dNumber )	//	�����̃t�@�C���q�b�g�E�������Ɉړ�����
	{
		if( SUCCEEDED( MultiFileTabSelect( dNumber ) ) )	//	�Y���̃^�u�Ƀt�H�[�J�X�ڂ���
		{
			DocMultiFileSelect( dNumber );	//	���̃^�u�̃t�@�C����\��
			return S_OK;
		}
	}

	dNumber = DocFileInflate( ptFile );	//	�J���Ē��g�W�J
	if( !(dNumber) )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ�����������[�I�H"), NULL, MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}
	else
	{
		MultiFileTabAppend( dNumber, ptFile );	//	�_�C�����O����t�@�C���I�[�|��
		OpenHistoryLogging( hWnd , ptFile );	//	�t�@�C���I�[�|���L�^��ǉ�
	}

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

	FILES_ITR	itFile;

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );
	ZeroMemory( atBuffer,  sizeof(atBuffer) );

//�����t�@�C���A�e�t�@�C�����Z�[�u����ɂ́H
	for( itFile = gltMultiFiles.begin(); itFile != gltMultiFiles.end(); itFile++ )
	{
		iPages = itFile->vcCont.size( );	//	���Ő�

		if( 1 >= iPages )	isMLT = FALSE;
		else				isMLT = TRUE;

		isAST = PageListIsNamed( itFile );	//	�łɖ��O���t���Ă�H

		if( isAST ){		ddExten = 0;	}	//	AST
		else if( isMLT ){	ddExten = 1;	}	//	MLT
		else{				ddExten = 2;	}	//	TXT

		StringCchCopy( atBuffer, MAX_PATH, itFile->atFileName );

		if( atBuffer[0] == NULL )	//	���̖��ݒ���
		{
			StringCchCopy( atFileName, MAX_STRING, itFile->atDummyName );
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
			NotifyBalloonExist( TEXT("���������A�o�b�N�A�b�v�o���Ȃ������̂ł��B��������"), TEXT("��������"), NIIF_ERROR );
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
				cbSplSz = DocAstSeparatorGetAlloc( i, D_SJIS, &pbSplit, itFile );

				WriteFile( hFile , pbSplit, (cbSplSz- iNullTmt), &wrote, NULL );
				FREE(pbSplit);
			}
			else
			{
				if( 1 <= i ){	WriteFile( hFile , pbSplit, cbSplSz, &wrote, NULL );	}
			}

			iByteSize = DocPageTextGetAlloc( itFile, i, D_SJIS, &pBuffer, TRUE );

			if( (i+1) == iPages ){	iByteSize -=  iCrLf;	}
			//	�ŏI�ł̖��[�̉��s�͕s�v�̂͂�
			WriteFile( hFile, pBuffer, iByteSize - iNullTmt, &wrote, NULL );

			FREE( pBuffer );
		}

		SetEndOfFile( hFile );
		CloseHandle( hFile );

		FREE( pbSplit );
	}

	if( gbAutoBUmsg ){	NotifyBalloonExist( TEXT("��ƒ��̃t�@�C�����o�b�N�A�b�v�ۑ������̂ł��B��������"), TEXT("��������"), NIIF_INFO );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���ɕۑ�����
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	bStyle	�㏑�������l�[�����E�t�H�[�}�b�g�I���̓_�C�����O�ł��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocFileSave( HWND hWnd, UINT bStyle )
{
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

	BOOLEAN	bNoName = FALSE;

	BOOLEAN	bUtf8 = FALSE;	//	�t�s�e�W�ŕۑ��Z��
	BOOLEAN	bUnic = FALSE;	//	���j�R�[�h�ŕۑ��Z��

	UINT_PTR	iPages, i;	//	�Ő�

	ZeroMemory( &stSaveFile, sizeof(OPENFILENAME) );

	ZeroMemory( atFilePath,  sizeof(atFilePath) );
	ZeroMemory( atFileName,  sizeof(atFileName) );
	ZeroMemory( atBuffer,  sizeof(atBuffer) );

	//	�ۑ����͏�ɑI�����Ă���t�@�C����ۑ�

	iPages = (*gitFileIt).vcCont.size( );	//	���Ő�
	if( 1 >= iPages )	isMLT = FALSE;
	else				isMLT = TRUE;

//�����̊g���q��AST�Ȃ�A�����D�悷��

	isAST = PageListIsNamed( gitFileIt );	//	�łɖ��O���t���Ă�H

	if( isAST ){		ddExten = 0;	}	//	AST
	else if( isMLT ){	ddExten = 1;	}	//	MLT
	else{				ddExten = 2;	}	//	TXT

	GetLocalTime( &stSysTile );

	StringCchCopy( atFilePath, MAX_PATH, (*gitFileIt).atFileName );

	if( NULL == (*gitFileIt).atFileName[0] )	bNoName = TRUE;

	//	���l�[�����A�t�@�C����������������ۑ��_�C�����O�J��
	if( (bStyle & D_RENAME) || bNoName )
	{

		//������ FileSaveDialogue ���o��
		stSaveFile.lStructSize     = sizeof(OPENFILENAME);
		stSaveFile.hwndOwner       = hWnd;
		stSaveFile.lpstrFilter     = TEXT("[ShiftJIS]�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0[UTF8]�A�X�L�[�A�[�g�t�@�C�� ( mlt, ast, txt )\0*.mlt;*.ast;*.txt\0\0");
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
		if(  2 == stSaveFile.nFilterIndex ){	bUtf8 = TRUE;	}
		//	�t�s�e�W�ŕۑ��E���j�R�[�h����ϊ�����΂�낵

		if( bUnic || bUtf8 )
		{	//	�������̂܂܃G�N�X�|�[�g���悤�Ƃ��Ă��疳��
			if( NULL == (*gitFileIt).atFileName[0] )
			{
				MessageBox( hWnd, TEXT("��ɒʏ�̕ۑ������Ă����ė~�����̂ł��B"), NULL, MB_OK | MB_ICONINFORMATION );
				return E_FAIL;
			}
		}
		else{	bLastChg = TRUE;	}	//	�V�K�ۑ��E���l�[���ۑ�
		//	����t�H�[�}�b�g�̏ꍇ�̓G�N�X�|�[�g�Ƃ��A������Ԃɂ͉e�����Ȃ��悤�ɂ���
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

	//	�I���W�i���t�@�C�����ɒ���
	if( !(bUnic) &&  !(bUtf8) ){	StringCchCopy( (*gitFileIt).atFileName, MAX_PATH, atFilePath );	}

	hFile = CreateFile( atFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ������ł���"), NULL, MB_OK | MB_ICONERROR );
		return E_HANDLE;
	}

	iNullTmt = 1;
	iCrLf = CH_CRLF_CCH;
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	if( bUnic )	//	���j�R�[�h���[�h�Ȃ�ABOM������
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

		if( bUnic )
		{
			cbSplSz = (MLT_SPRT_CCH + CH_CRLF_CCH) * sizeof(TCHAR);
			StringCchPrintfW( (LPTSTR)pbSplit, 15, TEXT("%s%s"), MLT_SEPARATERW, CH_CRLFW );
		}
		else	//	UTF8�̏ꍇ�́AASCII�����͂��̂܂܂ł���
		{
			cbSplSz = MLT_SPRT_CCH + CH_CRLF_CCH;
			StringCchPrintfA( (LPSTR)pbSplit, 30, ("%s%s"), MLT_SEPARATERA, CH_CRLFA );
		}
	}

	//	�{���̎捞�̓��j�R�[�h�ł��K�v������
	if( bUnic || bUtf8 ){	bStyle |= D_UNI;	}

	for( i = 0; iPages > i; i++ )	//	�S�ŕۑ�
	{
		if( isAST )	//	�`�r�s�̏ꍇ�́A�Ő擪�Ƀ^�C�g���������Ă�
		{
			//	�Ԃ�l�̊m�ۃo�C�g���ɂ͂m�t�k�k�^�[�~�l�[�^�܂�ł�̂Œ���
			cbSplSz = DocAstSeparatorGetAlloc( i, bStyle, &pbSplit, gitFileIt );

			if( bUtf8 ){	cbSplSz = DocUnicode2UTF8( &pbSplit );	}
			//	pbSplit�̒��g��t���ւ���

			WriteFile( hFile , pbSplit, (cbSplSz- iNullTmt), &wrote, NULL );
			FREE(pbSplit);
		}
		else	//	MLT�̏ꍇ�́A��ڈȍ~�ŋ�؂肪�K�v
		{
			if( 1 <= i ){	WriteFile( hFile , pbSplit, cbSplSz, &wrote, NULL );	}
		}

		iByteSize = DocPageTextGetAlloc( gitFileIt, i, bStyle, &pBuffer, TRUE );

		if( bUtf8 ){	iByteSize = DocUnicode2UTF8( &pBuffer );	}
		//	pBuffer�̒��g��t���ւ���

		if( (i+1) == iPages ){	iByteSize -=  iCrLf;	}	//	�ŏI�ł̖��[�̉��s�͕s�v�̂͂�
		WriteFile( hFile, pBuffer, iByteSize - iNullTmt, &wrote, NULL );

		FREE( pBuffer );
	}

	SetEndOfFile( hFile );
	CloseHandle( hFile );

	FREE( pbSplit );

	//	�G�N�X�|�[�g�Ȃ̂ŕۑ����ĂȂ����Ƃ�
	if( !(bUnic) &&  !(bUtf8) ){	DocModifyContent( FALSE );	}

	//	�Ȃ񂩃��b�Z�[�W
	if( bExtChg )
	{
		//InitLastOpen( INIT_SAVE, atFilePath );	//	���X�g�I�[�|��������
		MultiFileTabRename( (*gitFileIt).dUnique, atFilePath );	//	�^�u���̕ύX
		AppTitleChange( atFilePath );
		StringCchPrintf( atBuffer, MAX_STRING, TEXT("�g���q�� %s �ɂ��ĕۑ������̂ł��B��������"), aatExte[ddExten] );
		NotifyBalloonExist( atBuffer, TEXT("�g���q��ύX�����̂ł�"), NIIF_INFO );

		OpenHistoryLogging( hWnd , atFilePath );	//	�t�@�C�����ύX�����̂ŋL�^��蒼��
	}
	else
	{
		//	20110713	�V�K�����l�[�����Ă��烉�X�g�I�[�|��������
		if( bLastChg )
		{
			//InitLastOpen( INIT_SAVE, atFilePath );
			MultiFileTabRename( (*gitFileIt).dUnique, atFilePath );	//	�^�u���̕ύX
			AppTitleChange( atFilePath );

			OpenHistoryLogging( hWnd , atFilePath );	//	�t�@�C�����ύX�����̂ŋL�^��蒼��
		}

		if( bUnic || bUtf8 )
		{
			NotifyBalloonExist( TEXT("�ۑ������̂ł��B����"), TEXT("�G�N�X�|�[�g�����̂ł�"), NIIF_INFO );
		}
		else
		{
			NotifyBalloonExist( TEXT("�ۑ������̂ł��B��������"), TEXT("�t�@�C���o�͂����̂ł�"), NIIF_INFO );
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	���j�R�[�h��������󂯎���āA�t�s�e�W�̃A�h���X���m�ۂ��Ă��ǂ��B
	@param[in,out]	pText	���I���j�R�[�h��������E���I�t�s�e�W����������B�������̈�������
	@return	INT	�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
INT DocUnicode2UTF8( LPVOID *pText )
{
	UINT_PTR	cchSz;	//	���j�R�[�h�p
	INT	cbSize, rslt;	//	UTF8�p
	LPVOID		pUtf8;	//	�m��

	StringCchLength( (LPTSTR)(*pText), STRSAFE_MAX_CCH, &cchSz );

	//	�K�v�o�C�g���m�F
	cbSize = WideCharToMultiByte( CP_UTF8, 0, (LPTSTR)(*pText), -1, NULL, 0, NULL, NULL );
	TRACE( TEXT("cbSize[%d]"), cbSize );
	pUtf8 = (LPSTR)malloc( cbSize );
	ZeroMemory( pUtf8, cbSize );
	rslt = WideCharToMultiByte( CP_UTF8, 0, (LPTSTR)(*pText), -1, (LPSTR)(pUtf8), cbSize, NULL, NULL );
	TRACE( TEXT("rslt[%d]"), rslt );

	FREE( *pText );	//	���j�R�[�h������̂ق��͔j�󂷂�

	*pText = pUtf8;	//	�t�s�e�W�̂ق��ɕt���ւ���

	return cbSize;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W���O��AST��؂�t���Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	dPage	�m�ۂ���Ŕԍ�
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS�ŁA��`���ǂ���
	@param[out]	pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^�܂�
*/
INT DocAstSeparatorGetAlloc( INT dPage, UINT bStyle, LPVOID *pText, FILES_ITR itFile )
{
	UINT	cchSize, cbSize;
	TCHAR	atBuffer[MAX_PATH];

	StringCchPrintf( atBuffer, MAX_PATH, TEXT("[AA][%s]\r\n"), itFile->vcCont.at( dPage ).atPageName );
	StringCchLength( atBuffer, MAX_PATH, &cchSize );

	if( bStyle & D_UNI )
	{
		cbSize = (cchSize + 1) * sizeof(TCHAR);	//	NULL�^�[�~�l�[�^

		*pText = (LPTSTR)malloc( cbSize );
		ZeroMemory( *pText, cbSize );
		StringCchCopy( (LPTSTR)(*pText), (cchSize + 1), atBuffer );
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

	iByteSize = DocPageTextGetAlloc( gitFileIt, gixFocusPage, D_UNI, &pBuffer, TRUE );
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

