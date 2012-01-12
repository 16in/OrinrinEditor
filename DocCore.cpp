/*! @file
	@brief �h�L�������g�̓��e�̊Ǘ������܂�
	���̃t�@�C���� DocCore.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/04/30
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
/*
	�t�@�C���̊Ǘ�����\���̂�p�ӁE��������X�g�łȂ��ł����Ε����t�@�C���ɂȂ�
	���g���A��s���Ƀ��X�g�ŊǗ��A�e�s�̓��e������܂��Ǘ�����
	���e�̓��j�R�[�h�ŊǗ����邩�E�����ڂ͕ς��Ȃ��͂�

	�������́A�L�����b�g�̍����̕������Ő�����B�L�����b�g�����[�Ȃ�O������

*/

//	TODO:	�����t�@�C���𓯎��ɕێ��ł���悤�ɂ���

//-------------------------------------------------------------------------------------------------

//	�\���̐錾�̓R�����ֈړ�

//-------------------------------------------------------------------------------------------------

/*
(*pstTexts).acSjis	�\���̃|�C���^�͂���ł�����

�|�C���^�m�ۂ͂������
vector<int>::iterator it = vec.begin();   // vec �� vector<int>�^�̕ϐ�
it++;
int* p = &*it;  // �����v�f�̃A�h���X���擾���悤�Ƃ��Ă���BOK

gstFile���A�}�N���ŃC�e���[�^�|�C���^�Ɏd���ďグ��
�t�H�[�J�X���Ă�t�@�C���͏�Ɉ�Ȃ̂ŁA�|�C���^�̒��g��ς��邩�A
�������̓}�N���ŃC�e���[�^���̂ɂ��Ă��܂��΂�����

(*ltrItr).cchMozi;	����ł������ۂ�
*/


EXTERNED list<ONEFILE>	gltMultiFiles;	//!<	�����t�@�C���ێ�
//�C�e���[�^��typedef�̓w�b�_��

static LPARAM	gdNextNumber;	//!<	�J�����t�@�C���̒ʂ��ԍ��E��ɃC���N��

EXTERNED FILES_ITR	gitFileIt;	//!<	�����Ă�t�@�C���̖{��
#define gstFile	(*gitFileIt)	//!<	�C�e���[�^���\���̂ƌ��Ȃ�

EXTERNED INT		gixFocusPage;	//!<	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

EXTERNED INT		gixDropPage;	//!<	�����z�b�g�ԍ�

extern  UINT		gbUniRadixHex;	//	���j�R�[�h���l�Q�Ƃ��P�U�i���ł��邩
extern  UINT		gbCrLfCode;		//	���s�R�[�h�F�O������΁E��O�x�x 
//-------------------------------------------------------------------------------------------------

UINT	CALLBACK DocPageLoad( LPTSTR, LPTSTR, INT );
//-------------------------------------------------------------------------------------------------

/*!
	�Ȃ񂩏�����
*/
HRESULT DocInitialise( LPVOID pVoid )
{
	gdNextNumber = 1;

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�������݂͈̔͊O�G���[���������Ă��Ȃ���
	@param[in]	iPage	�`�F�b�N�������Ŕԍ�
	@param[in]	iLine	�`�F�b�N�������s�ԍ�
	@return	BOOLEAN		��O�͈͊O�G���[�@�O��薳��
*/
BOOLEAN DocRangeIsError( INT iPage, INT iLine )
{
	INT_PTR	iSize;

	if( 0 > iPage || 0 > iLine )	return TRUE;

	iSize = gstFile.vcCont.size( );
	if( 0 >= iSize || iPage >= iSize )	return TRUE;

	iSize = gstFile.vcCont.at( iPage ).vcPage.size( );
	if( 0 >= iSize || iLine >= iSize )	return TRUE;

	return FALSE;
}
//-------------------------------------------------------------------------------------------------


/*!
	�ύX������
	@param[in]	dMode	��O�ύX�����@�O�ۑ���������ύX�͂Ȃ��������Ƃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocModifyContent( UINT dMode )
{
	if( dMode )
	{
		if( gstFile.dModify )	return S_FALSE;
		//	�ύX�̂Ƃ��A�߂ɕύX�̏������Ă��牽�����Ȃ��ėǂ�

		StatusBarSetText( SB_MODIFY, MODIFY_MSG );
	}
	else
	{
		StatusBarSetText( SB_MODIFY, TEXT("") );
	}

	DocMultiFileModify( dMode );

	gstFile.dModify =  dMode;	//	�����ŋL�^���Ă���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�V�����t�@�C���u���������ăt�H�[�J�X����
	@param[in]	ptDmyName	�_�~�[����Ԃ��BNULL�BMAX_PATH�ł��邱��
	@return		LPARAM	�Ή����郆�j�[�N�ԍ�
*/
LPARAM DocMultiFileCreate( LPTSTR ptDmyName )
{
	ONEFILE	stFile;
	FILES_ITR	itNew;

	ZeroMemory( stFile.atFileName, sizeof(stFile.atFileName) );
	stFile.dModify   = FALSE;
	stFile.dNowPage  = 0;
	stFile.dUnique   = gdNextNumber++;
	stFile.stCaret.x = 0;
	stFile.stCaret.y = 0;

	ZeroMemory( stFile.atDummyName, sizeof(stFile.atDummyName) );
	StringCchPrintf( stFile.atDummyName, MAX_PATH, TEXT("%s%d.%s"), NAME_DUMMY_NAME, stFile.dUnique, NAME_DUMMY_EXT );

	if( ptDmyName ){	StringCchCopy( ptDmyName, MAX_PATH, stFile.atDummyName );	}

	stFile.vcCont.clear(  );

	gltMultiFiles.push_back( stFile );

	//	�V�K�쐬�̏���
	gixFocusPage = -1;

	PageListClear(  );	//	�y�[�W���X�g�r���[���j��


	itNew = gltMultiFiles.end( );
	itNew--;	//	���[�ɒǉ��������炱��ł���

	gitFileIt = itNew;	//	�t�@�C���Ȃ�

	return stFile.dUnique;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e��ύX������^�u�̃t�@�C������[�ύX]����
	@param[in]	dMode	��O�ύX�����@�O�ύX�͂Ȃ��������Ƃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileModify( UINT dMode )
{
	TCHAR	atFile[MAX_PATH];	//!<	�t�@�C����

	StringCchCopy( atFile, MAX_PATH, gstFile.atFileName );
	if( 0 == atFile[0] ){	StringCchCopy( atFile, MAX_PATH , gstFile.atDummyName );	}

	PathStripPath( atFile );

	if( dMode ){	StringCchCat( atFile, MAX_PATH, MODIFY_MSG );	}

	MultiFileTabRename( gstFile.dUnique, atFile );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���^�u��I������
	@param[in]	uqNumber	�I�����ꂽ�^�u�̒ʂ��ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileSelect( LPARAM uqNumber )
{
	FILES_ITR	itNow;
	POINT	stCaret;

	for( itNow = gltMultiFiles.begin(); itNow != gltMultiFiles.end(); itNow++ )
	{
		if( uqNumber == itNow->dUnique )	break;
	}
	if( itNow == gltMultiFiles.end() )	return E_OUTOFMEMORY;

	ViewSelPageAll( -1 );	//	���J���Ă�ł͈̔͑I����j��

	PageListClear(  );	//	�y�[�W���X�g�r���[���j��

	gitFileIt = itNow;	//	�t�@�C���Ȃ�

//	TODO:	����ǂݍ��ݎ��̃o�C�g���v�Z�ԈႦ�Ă�悤���E�Ȃ������H

	PageListBuild( NULL );	//	�y�[�W���X�g��蒼��

	AppTitleChange( itNow->atFileName );

	gixFocusPage = itNow->dNowPage;

	DocModifyContent( itNow->dModify );	//	�ύX�������ǂ���

	DocCaretPosMemory( INIT_LOAD, &stCaret );	//	��ɓǂݏo���Ȃ��Ǝ��ŃN�����[�����

	PageListViewChange( gixFocusPage );	//	�S���ǂݍ��񂾂̂Ń��X�g�y�[�W��\������

	ViewPosResetCaret( stCaret.x, stCaret.y );	//	Caret�ʒu�Đݒ�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S���e��j��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileDeleteAll( VOID )
{
	UINT_PTR	i, j, iPage, iLine;
	FILES_ITR	itNow;

	for( itNow = gltMultiFiles.begin( ); itNow != gltMultiFiles.end(); itNow++ )
	{
		iPage = itNow->vcCont.size( );
		for( i = 0; iPage > i; i++ )
		{
			iLine = itNow->vcCont.at( i ).vcPage.size( );
			for( j = 0; iLine > j; j++ )
			{
				itNow->vcCont.at( i ).vcPage.at( j ).vcLine.clear(   );	//	�e�s�̒��g�S����
			}
			itNow->vcCont.at( i ).vcPage.clear(  );	//	�s��S����
			SqnFreeAll( &(itNow->vcCont.at( i ).stUndoLog) );
		}
		itNow->vcCont.clear(  );	//	�y�[�W��S����
	}

	gltMultiFiles.clear(  );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C���^�u�����Ƃ��E�Ō�̈�͕���Ȃ��悤�ɂ��邩
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	uqNumber	�������^�u�̒ʂ��ԍ�
	@return		LPARAM		�J���������^�u�̒ʂ��ԍ��E���s������O
*/
LPARAM DocMultiFileDelete( HWND hWnd, LPARAM uqNumber )
{
	INT			iRslt;
	UINT_PTR	i, j, iPage, iLine;
	UINT_PTR	iCount;
	LPARAM	dNowNum, dPrevi;
	FILES_ITR	itNow;

	//	������J���ĂȂ��Ȃ���Ȃ�
	iCount = gltMultiFiles.size();
	if( 1 >= iCount )	return 0;

	//	�����ύX���c���Ă�Ȃ璍�ӂ𑣂�
	if( gitFileIt->dModify )
	{
		iRslt = MessageBox( hWnd, TEXT("���������I�H\r\n�ύX�����܂܂Ȃ̂ł���I�@�ۑ����ĕ���̂ł����H"), TEXT("�^(^o^)�_"), MB_YESNOCANCEL | MB_ICONQUESTION );
		if( IDCANCEL == iRslt ){	return 0;	}

		if( IDYES == iRslt ){	DocFileSave( hWnd, D_SJIS );	}
	}

	dNowNum = gitFileIt->dUnique;	//	���J���Ă郄�c�̔ԍ�

	itNow = gltMultiFiles.begin( );
	itNow++;	//	���̂�̒ʂ��ԍ����m�ۂ��Ă����B
	dPrevi = itNow->dUnique;

	for( itNow = gltMultiFiles.begin( ); itNow != gltMultiFiles.end(); itNow++ )
	{
		if( uqNumber == itNow->dUnique )	break;
		dPrevi = itNow->dUnique;
	}
	if( itNow == gltMultiFiles.end() )	return 0;
	//	�����폜�Ώۂ��擪�Ȃ�AdPrevi�͎��̂�̂܂܁A���ȍ~�Ȃ�A���O�̂������Ă�͂�

	//	DocContentsObliterate���̂��

	iPage = itNow->vcCont.size( );
	for( i = 0; iPage > i; i++ )
	{
		iLine = itNow->vcCont.at( i ).vcPage.size( );
		for( j = 0; iLine > j; j++ )
		{
			itNow->vcCont.at( i ).vcPage.at( j ).vcLine.clear(   );	//	�e�s�̒��g�S����
		}
		itNow->vcCont.at( i ).vcPage.clear(  );	//	�s��S����
		SqnFreeAll( &(itNow->vcCont.at( i ).stUndoLog) );
	}
	itNow->vcCont.clear(  );	//	�y�[�W��S����

	gltMultiFiles.erase( itNow );	//	�{�̂�����

	//	��ɑI�����L���ɂȂ�̂ŁA�펞�X�̏������͂���͂�
	if( dNowNum == uqNumber )	//	�J���Ă�̂������
	{
		gixFocusPage = -1;
		DocMultiFileSelect( dPrevi );	//	�L���Ȃ̂��J������
	}

	return dPrevi;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u�������Ă���
	@param[in]	iTgt		�ǂݍ��݂����ԍ��E�����Ȃ�t�@�C�����̂݊m��
	@param[in]	ptFile		�t�@�C���p�X�����EMAX_PATH�ł��邱��
	@param[in]	ptIniPath	INI�t�@�C���̃p�X
	@return		HRESULT	�I����ԃR�[�h
*/
INT DocMultiFileFetch( INT iTgt, LPTSTR ptFile, LPTSTR ptIniPath )
{
	TCHAR	atKeyName[MIN_STRING];
	INT		iCount;

	assert( ptIniPath );

	iCount = GetPrivateProfileInt( TEXT("MultiOpen"), TEXT("Count"), 0, ptIniPath );
	if( 0 > iTgt )	return iCount;

	assert( ptFile );

	if( iCount <= iTgt ){	ptFile[0] = NULL;	return iCount;	}
	//	�I�[�o�[���Ă��疳���ɂ��ďI��

	StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%u"), iTgt );

	GetPrivateProfileString( TEXT("MultiOpen"), atKeyName, TEXT(""), ptFile, MAX_PATH, ptIniPath );

	return iCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u��ۑ�����
	@param[in]	ptIniPath	INI�t�@�C���̃p�X
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocMultiFileStore( LPTSTR ptIniPath )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	UINT	i;
	FILES_ITR	itNow;

	assert( ptIniPath );

	//	��U�Z�N�V��������ɂ���
	ZeroMemory( atBuff, sizeof(atBuff) );
	WritePrivateProfileSection( TEXT("MultiOpen"), atBuff, ptIniPath );

	//	�t�@�C���������L�^
	i = 0;
	for( itNow = gltMultiFiles.begin( ); itNow != gltMultiFiles.end(); itNow++ )
	{
		if( NULL != itNow->atFileName[0] )
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%u"), i );
			WritePrivateProfileString( TEXT("MultiOpen"), atKeyName, itNow->atFileName, ptIniPath );
			i++;
		}
	}

	//	�����L�^
	StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), i );
	WritePrivateProfileString( TEXT("MultiOpen"), TEXT("Count"), atBuff, ptIniPath );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Ώۃt�@�C���̖��O���Q�b�c�I����
	@param[in]	tabNum	���O��m�肽�����c�̃^�u�ԍ�
	@return		LPTSTR	���O�o�b�t�@�̃|�C���^�[�E�����Ȃ�NULL��Ԃ�
*/
LPTSTR DocMultiFileNameGet( INT tabNum )
{
	INT	i;
	FILES_ITR	itNow;

	//	�q�b�g����܂ŃT�[�`
	for( i = 0, itNow = gltMultiFiles.begin(); itNow != gltMultiFiles.end(); i++, itNow++ )
	{
		if( tabNum == i )	break;
	}
	if( itNow == gltMultiFiles.end() )	return NULL;	//	�q�b�g�����E�A���G�i�[�C

	//	�������Ȃ�_�~�[��
	if( NULL == itNow->atFileName[ 0] ){	return itNow->atDummyName;	}

	return itNow->atFileName;	//	�t�@�C�����߂�
}
//-------------------------------------------------------------------------------------------------



/*!
	Caret�ʒu���펞�L�^�E�t�@�C���؂�ւ����Ƃ��ɈӖ�������
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in,out]	pstPos	Caret�ʒu�A�h�b�g�A�s��
*/
VOID DocCaretPosMemory( UINT dMode, LPPOINT pstPos )
{
	if( dMode )	//	���[�h
	{
		pstPos->x = gitFileIt->stCaret.x;
		pstPos->y = gitFileIt->stCaret.y;
	}
	else	//	�Z�[�u
	{
		gitFileIt->stCaret.x = pstPos->x;
		gitFileIt->stCaret.y = pstPos->y;
	}

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�ҏW���̂�j�����ĐV�����̍��E�V�����t�@�C�����J��
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocOpenFromNull( HWND hWnd )
{
	LPARAM	dNumber;

	TCHAR	atDummyName[MAX_PATH];
	//	�����t�@�C�������Ȃ�A�j���͕s�v�A�V�����t�@�C���C���X�^���X����đΉ�

	//	�V�����t�@�C���u����̏���
	dNumber = DocMultiFileCreate( atDummyName );

	MultiFileTabAppend( dNumber, gstFile.atDummyName );

	AppTitleChange( atDummyName );

	gixFocusPage = DocPageCreate( -1 );
	PageListInsert( gixFocusPage  );	//	�y�[�W���X�g�r���[�ɒǉ�
	DocPageChange( 0 );

	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C������O�ɕύX���m�F
	@param[in]	hWnd	�e�ɂ���E�C���h�E�n���h��
	@param[in]	dMode	��O���郁�b�Z�[�W����@�O�ύX����������f�ʂ�
	@return	�P���Ă����@�O�_��
*/
INT DocFileCloseCheck( HWND hWnd, UINT dMode )
{
	INT		rslt, ret;

	BOOLEAN	bMod = FALSE;
	FILES_ITR	itFiles;

	//	���ۑ��̃t�@�C�����`�F�L
	for( itFiles = gltMultiFiles.begin(); itFiles != gltMultiFiles.end(); itFiles++ )
	{
		if( itFiles->dModify )
		{
			bMod = TRUE;
			break;
		}
	}

	if( bMod )	//	�X�V���̂����Ă遁�ۑ�����ĂȂ��Ȃ�
	{
		rslt = MessageBox( hWnd, TEXT("���������I�H\r\n�ۑ����ĂȂ��t�@�C��������̂ł���I\r\n�I���������Ă����̂ł����H"), TEXT("�^(^o^)�_"), MB_YESNO | MB_ICONQUESTION );
		if( IDYES == rslt ){	ret = 1;	}
		else{	ret = 0;	}
	}
	else
	{
		if( dMode )
		{
			rslt = MessageBox( hWnd, TEXT("�����H\r\n�I���̂ł����H"), TEXT("�_(^o^)�^"), MB_YESNO | MB_ICONQUESTION );
			if( IDYES == rslt ){	ret = 1;	}
			else{					ret = 0;	}
		}
		else
		{
			ret = 1;
		}
	}

	return ret;
}
//-------------------------------------------------------------------------------------------------

/*!
	�t�@�C�����m��
	@param[in]	ptFileName	�w�肳�ꂽ�t�@�C�����ŊJ��
	@return		LPARAM	�O���s�@�P�`����
*/
LPARAM DocFileInflate( LPTSTR ptFileName )
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

	LPARAM	dNumber;

	//TCHAR	atBuff[10];
	//ZeroMemory( atBuff, sizeof(atBuff) );

	assert( ptFileName );	//	�t�@�C���J���Ȃ��̂̓o�O

	//	�t�@�C����������ۂ������玩���I�ɃA�E�c�I
	if( NULL == ptFileName[0] ){	return 0;	}

	//	���b�c�I�[�|��
	hFile = CreateFile( ptFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return 0;	}

	InitLastOpen( INIT_SAVE, ptFileName );	//	�����t�@�C���ł͈Ӗ�������

	//	�V�����t�@�C���u����̏���
	dNumber = DocMultiFileCreate( NULL );

	iByteSize = GetFileSize( hFile, NULL );
	pBuffer = malloc( iByteSize + 2 );
	ZeroMemory( pBuffer, iByteSize + 2 );

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pBuffer, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	StringCchCopy( gstFile.atFileName, MAX_PATH, ptFileName );

	//	���j�R�[�h�`�F�b�N
	CopyMemory( &rtUniBuf, pBuffer, 2 );
	if( rtHead == rtUniBuf )	//	���j�R�[�h�w�b�_�������
	{
		ptString = (LPTSTR)pBuffer;
		ptString++;	//	���j�R�[�h�w�b�_���i�߂Ă���
	}
	else
	{
		pcText = (LPSTR)pBuffer;
		//	�V�t�gJIS���J���ꍇ�A&#0000;�̕������ǂ��ɂ�����Ƃ�����
		ptString = SjisDecodeAlloc( pcText );	//	SJIS�̓��e�����j�R�[�h�ɂ���

		FREE( pBuffer );	//	�������ŊJ��
		pBuffer = ptString;	//	�|�C���g����Ƃ����ύX

	}

	StringCchLength( ptString, STRSAFE_MAX_CCH, &cchSize );

/*
	//	�g���q���m�F�E�h�b�g���݂���`��
	ptExten = PathFindExtension( ptFileName );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	if( 0 == *ptExten )
	{
		//	�g���q�w�肪�Ȃ��Ȃ�
	}
	else	//	�����̊g���q����������
	{
		StringCchCopy( atExBuf, 10, ptExten );
		CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

		//	�����̊g���q���AAST�Ȃ炻���D�悷��
		if( !( StrCmp( atExBuf, aatExte[0] ) ) )	//	AST�ł���Ȃ�
*/

	//	����AST�Ȃ�A�擪��[AA]�ɂȂ��Ă�͂��E�����͒��ł��
	if( StrCmpN( AST_SEPARATERW, ptString, 4 ) )
	{
		DocStringSplitMLT( ptString , cchSize, DocPageLoad );
	}
	else
	{
		DocStringSplitAST( ptString , cchSize, DocPageLoad );
	}

	//	�t�@�C���J������L�����b�g�Ƃ��X�N���[�������Z�b�g����
	ViewEditReset(  );

	FREE( pBuffer );	//	��ptString

	DocPageChange( 0  );	//	�S���ǂݍ��񂾂̂ōŏ��̃y�[�W��\������

	AppTitleChange( ptFileName );
	

	return dNumber;
}
//-------------------------------------------------------------------------------------------------



/*!
	�ł�����ē��e���Ԃ�����
	@param[in]	ptName	���ڂ̖��O�E��������NULL
	@param[in]	ptCont	���ڂ̓��e
	@param[in]	cchSize	���e�̕�����
	@return		UINT	���ɈӖ��Ȃ�
*/
UINT CALLBACK DocPageLoad( LPTSTR ptName, LPTSTR ptCont, INT cchSize )
{
	INT	dmyX = 0, dmyY = 0;

	gixFocusPage = DocPageCreate(  -1 );	//	�ł��쐬
	PageListInsert( gixFocusPage  );	//	�y�[�W���X�g�r���[�ɒǉ�

	//	�V����������y�[�W�ɂ���

	if( ptName ){	DocPageNameSet( ptName );	}	//	���O���Z�b�g���Ă���

	if( 0 < cchSize )	//	��s�łȂ��̂Ȃ�
	{
		DocStringAdd( &dmyX, &dmyY, ptCont, cchSize );	//	���̒��ŉ��s�Ƃ��ʓ|����
	}

	//	�Čv�Z�����Ⴄ
	DocPageParamGet( NULL, NULL );	//	DocPageInfoRenew( -1, 0 );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�l�k�s�������͂s�w�s�ȃ��j�R�[�h��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	ptStr		����Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize		���̕�����̕�����
	@param[in]	pfPageLoad	���e������R�[���o�b�N�����̃A��
	@return		UINT		�쐬�����Ő�
*/
UINT DocStringSplitMLT( LPTSTR ptStr, INT cchSize, PAGELOAD pfPageLoad )
{
	LPTSTR	ptCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPTSTR	ptEnd;		//	�y�[�W�̖��[�ʒu�E�Z�p���[�^�̒��O
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
	UINT	cchItem;
//	INT		dmyX = 0, dmyY = 0;
	BOOLEAN	bLast;

	ptCaret = ptStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X
	//	�n�_�ɂ̓Z�p���[�^�������̂Ƃ݂Ȃ��B�A������Z�p���[�^�́A�󔒓��e�Ƃ��Ĉ���

	bLast = FALSE;

	do
	{
		ptEnd = StrStr( ptCaret, MLT_SEPARATERW );	//	�Z�p���[�^��T��
		if( !ptEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			ptEnd = ptStr + cchSize;	//	WCHAR�T�C�Y�Ōv�Z�����H
			bLast = TRUE;
		}
		cchItem = ptEnd - ptCaret;	//	WCHAR�P�ʂȂ̂Ōv�Z���ʂ͕������̂悤��

		if( !(bLast) && 0 < cchItem )	//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		{
			cchItem -= CH_CRLF_CCH;
		}

		pfPageLoad( NULL, ptCaret, cchItem );

		iNumber++;

		ptCaret = NextLineW( ptEnd );

	}while( *ptCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�r�s�ȃ��j�R�[�h��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	ptStr		����Ώە�����ւ̃|�C���^�[
	@param[in]	cchSize		���̕�����̕�����
	@param[in]	pfPageLoad	���e������R�[���o�b�N�����̃A��
	@return		UINT		�쐬�����Ő�
*/
UINT DocStringSplitAST( LPTSTR ptStr, INT cchSize, PAGELOAD pfPageLoad )
{
	LPTSTR	ptCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPTSTR	ptStart;	//	�Z�p���[�^�̒��O
	LPTSTR	ptEnd;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
	UINT	cchItem;
	BOOLEAN	bLast;
	TCHAR	atName[MAX_PATH];

	ptCaret = ptStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	bLast = FALSE;



	do	//	�Ƃ肠������ԍŏ���ptCaret��[AA]�ɂȂ��Ă�
	{
		ptStart = NextLineW( ptCaret );	//	���̍s���炪�{��

		ptCaret += 5;	//	[AA][
		cchItem = ptStart - ptCaret;	//	���O�����̕�����
		cchItem -= 3;	//	]rn

		ZeroMemory( atName, sizeof(atName) );	//	���O�m��
		if( 0 < cchItem )	StringCchCopyN( atName, MAX_PATH, ptCaret, cchItem );

		ptCaret = ptStart;	//	�{�̕���

		ptEnd = StrStr( ptCaret, AST_SEPARATERW );	//	�Z�p���[�^��T��
		//	���̎��_��ptEnd�͎���[AA]�������Ă�E��������NULL(�Ō�̃R�})
		if( !ptEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			ptEnd = ptStr + cchSize;	//	WCHAR�T�C�Y�Ōv�Z�����H
			bLast = TRUE;
		}
		cchItem = ptEnd - ptCaret;	//	WCHAR�P�ʂȂ̂Ōv�Z���ʂ͕������̂悤��

		if( !(bLast) && 0 < cchItem )	//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		{
			cchItem -= CH_CRLF_CCH;
		}

		pfPageLoad( atName, ptCaret, cchItem );

		iNumber++;

		ptCaret = ptEnd;

	}while( *ptCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�r�c�Ȃr�i�h�r��������󂯎���ĕ������y�[�W�ɓ����
	@param[in]	pcStr		����Ώ�SJIS������ւ̃|�C���^�[
	@param[in]	cchSize		���̕�����̕�����
	@param[in]	pfPageLoad	���e������R�[���o�b�N�����̃A��
	@return		UINT		�쐬�����Ő�
*/
UINT DocImportSplitASD( LPSTR pcStr, INT cbSize, PAGELOAD pfPageLoad )
{
//ASD�Ȃ�ASJIS�̂܂܂�0x01,0x01�A0x02,0x02��Ή�����K�v������
//0x01,0x01�����s�A0x02,0x02�������̋�؂�A�A�C�e����؂肪���s

	LPSTR	pcCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPSTR	pcEnd, pcDesc;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g
	UINT	cbItem, d;
	BOOLEAN	bLast;

	LPTSTR		ptName, ptCont;
	UINT_PTR	cchItem;


	pcCaret = pcStr;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	bLast = FALSE;


	do	//	�Ƃ�₦�����s
	{
		pcEnd = NextLineA( pcCaret );	//	���̍s�܂łłP�A�C�e��
		//if( !(*pcEnd) )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		//{
		//	pcEnd = pcStr + cbSize;	//	CHAR�T�C�Y�Ōv�Z�����H
		//	bLast = TRUE;
		//}
		//	���g��NULL�Ȃ����ŁA�|�C���^�͗L���Ȃ̂œ��Ɉʒu�v�Z�͕s�v��
		cbItem  = pcEnd - pcCaret;	//	��s�̕�����

		pcDesc = NULL;
		ptName = NULL;
		ptCont = NULL;

		for( d = 0; cbItem > d; d++ )
		{
			if( (0x0D == pcCaret[d]) && (0x0A == pcCaret[d+1]) )	//	���[�ł��邩
			{
				pcCaret[d]   = 0x00;	//	���[�Ȃ̂�NULL�ɂ���
				pcCaret[d+1] = 0x00;

				if( pcDesc ){	ptName =  SjisDecodeAlloc( pcDesc );	}

				break;
			}

			//	�������Ԓ���
			if( (0x01 == pcCaret[d]) && (0x01 == pcCaret[d+1]) )	//	���s�ł��邩
			{
				pcCaret[d]   = 0x0D;	//	����
				pcCaret[d+1] = 0x0A;	//	����
				d++;	//	�ϊ������̂Ŏ��ɐi�߂�̂��悢
			}

			if( (0x02 == pcCaret[d]) && (0x02 == pcCaret[d+1]) )	//	�A�C�e���Ɛ����̋�؂�
			{
				pcDesc = &(pcCaret[d+2]);	//	�����J�n�ʒu

				pcCaret[d]   = 0x00;	//	�d�؂�Ȃ̂�NULL�ɂ���
				pcCaret[d+1] = 0x00;
				d++;	//	�ϊ������̂Ŏ��ɐi�߂�̂��悢
			}
		}

		ptCont = SjisDecodeAlloc( pcCaret );	//	����Ă���
		StringCchLength( ptCont, STRSAFE_MAX_CCH, &cchItem );

		pfPageLoad( ptName, ptCont, cchItem );

		iNumber++;

		FREE( ptCont );
		FREE( ptName );

		pcCaret = pcEnd;

	}while( *pcCaret  );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------


/*!
	�Ŗ����Z�b�g����
	@param[in]	ptName	�Z�b�g����Ŗ��̂ւ̃|�C���^�[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageNameSet( LPTSTR ptName )
{
	StringCchCopy( gstFile.vcCont.at( gixFocusPage ).atPageName, SUB_STRING, ptName );

	PageListNameSet( gixFocusPage, ptName );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�y�[�W�ǉ�����
	@param[in]	iAdding	���̎w��y�[�W�̎��ɒǉ��E-1�Ŗ��[�ɒǉ�
	@return		UINT	�V�K�쐬�����y�[�W�ԍ�
*/
UINT DocPageCreate( INT iAdding )
{
	INT_PTR		iTotal, iNext;
	UINT_PTR	iAddPage = 0;
	INT		i;

	ONELINE	stLine;
	ONEPAGE	stPage;

	PAGE_ITR	itPage;


	ZeroONELINE( &stLine  );	//	�V�K�쐬������A��s�ڂ��O�����Șg�����

	//	��������ZeroONEPAGE�Ƃ��ɂ܂Ƃ߂邩
	ZeroMemory( stPage.atPageName, sizeof(stPage.atPageName) );
//	stPage.dDotCnt = 0;
	stPage.dByteSz = 0;
	stPage.vcPage.clear(  );
	stPage.vcPage.push_back( stLine );	//	�P�ł̘g�������
	stPage.dSelLineTop    =  -1;		//	�����́|�P�𒍈�
	stPage.dSelLineBottom =  -1;		//	

	SqnInitialise( &(stPage.stUndoLog) );

	//	���̕ł̎��ɍ쐬
	iTotal = gstFile.vcCont.size(  );

	if( 0 <= iAdding )
	{
		iNext = iAdding + 1;	//	���̕�
		if( iTotal <= iNext ){	iNext =  -1;	}	//	�S�ł�葽���Ȃ疖�[�w��
	}
	else
	{
		iNext = -1;
	}

	if( 0 >  iNext )	//	�����ɒǉ�
	{
		gstFile.vcCont.push_back( stPage  );	//	�t�@�C���\���̂ɒǉ�

		iAddPage = gstFile.vcCont.size( );
		iAddPage--;	//	���[�ɒǉ������񂾂���A�������ā|�P������O�C���f�b�N�X�ԍ�
	}
	else
	{
		itPage = gstFile.vcCont.begin(  );
		for( i = 0; iNext >  i; i++ ){	itPage++;	}
		gstFile.vcCont.insert( itPage, stPage );

		iAddPage = iNext;
	}

	return iAddPage;	//	�ǉ������y�[�W�ԍ�
}
//-------------------------------------------------------------------------------------------------

/*!
	�ł��폜
	@param[in]	iPage	�폜����ł̔ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageDelete( INT iPage )
{
	INT	i, iNew;
	PAGE_ITR	itPage;

	if( 1 >= gstFile.vcCont.size( ) )	return E_ACCESSDENIED;

	//	�����Ńo�b�N�A�b�v���H

	//	�X���ʒu�܂ŃC�e���[�^�������Ă���
	itPage = gstFile.vcCont.begin(  );
	for( i = 0; iPage > i; i++ ){	itPage++;	}

	SqnFreeAll( &(itPage->stUndoLog) );	//	�A���h�D���O�폜
	gstFile.vcCont.erase( itPage  );	//	��������폜
	gixFocusPage = -1;	//	�őI�𖳌��ɂ���

	PageListDelete( iPage );

	iNew = iPage - 1;	//	�폜�������O�̕ł�
	if( 0 > iNew )	iNew = 0;

	DocPageChange( iNew );	//	�폜������ňړ�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
/*!
	�y�[�W��ύX
	@param[in]	dPageNum	�ύX�������Ŕԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageChange( INT dPageNum )
{
	//	���̕\�����e�j���Ƃ����낢�날��

	ViewSelPageAll( -1 );	//	�͈͑I����j��

	gixFocusPage = dPageNum;	//	��ɕύX����

	gstFile.dNowPage = dPageNum;	//	�L�^

	PageListViewChange( dPageNum );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	ONLINE�\���̂��N�����[����
	@param[in]	pstLine	�N�����[��������̃|�C���^�[
	@return	����
*/
VOID ZeroONELINE( LPONELINE pstLine )
{
//	pstLine->dCaret     = 0;
//	pstLine->dNumber    = 0;
	pstLine->iDotCnt    = 0;
	pstLine->iByteSz    = 0;
	pstLine->dStyle     = 0;
	pstLine->bBadSpace  = FALSE;
	pstLine->vcLine.clear(  );
	pstLine->dFrtSpDot  = 0;
	pstLine->dFrtSpMozi = 0;
//	pstLine->dOffset    = 0;

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	�󔒌x�������
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@return	BOOLEAN		��O�x������@�O����
*/
BOOLEAN DocBadSpaceIsExist( INT rdLine )
{
	//	��Ԋm�F
	if( DocRangeIsError( gixFocusPage, rdLine ) ){	return 0;	}
	//	�����͈̔͊O�����͕K�R�Ȃ̂œ��Ɍx���͕s�v

	return gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).bBadSpace;
}
//-------------------------------------------------------------------------------------------------

/*!
	�A�����锼�p�X�y�[�X�A�擪���p�󔒂��L�邩�ǂ����`�F�L
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@return	UINT		��O�x������@�O����
*/
UINT DocBadSpaceCheck( INT rdLine )
{
	UINT_PTR	iCount, iRslt;
	BOOLEAN	bWarn;
	TCHAR	ch, chn;
	LTR_ITR	ltrItr, ltrEnd, ltrNext;

//���[�󔒂�DocLineDataGetAlloc�ł����Ă�

	//	��Ԋm�F
	if( DocRangeIsError( gixFocusPage, rdLine ) )
	{
		TRACE( TEXT("�͈͊O�G���[���� PAGE[%d], LINE[%d]"), gixFocusPage, rdLine );
		return 0;
	}

	//	�������m�F
	iCount = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.size( );
	if( 0 == iCount )	//	�O�Ȃ牽���m�F���邱�Ƃ��Ȃ��̂ŁA�G���[�����ďI���
	{
		//	�G���[�c���Ă���
		if( gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).bBadSpace )
		{
			gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).bBadSpace = 0;
			ViewRedrawSetLine( rdLine );	//	���[�����ƍs�ĕ`��
		}
		return 0;
	}

	iRslt = 0;

	bWarn = FALSE;
	ltrEnd = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.end( );

	for( ltrItr = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.begin(); ltrEnd != ltrItr; ltrItr++ )
	{
		ch = ltrItr->cchMozi;
		//	�ʓ|�Ȃ̂Ōx���͈�U�����Ă���
		ltrItr->mzStyle &= ~CT_WARNING;

		if( 0xFF < ch ){	bWarn = FALSE;	continue;	}
		//	���p�ȊO�Ȃ�֌W�Ȃ��̂Ōx�������ďI���
		if( isspace( ch ) )
		{
			if( !(bWarn) )	//	�܂����x����������
			{
				//	���̕������m�F����
				ltrNext = ltrItr + 1;
				if( ltrNext !=  ltrEnd )	//	�������݂��Ă���
				{
					chn = ltrNext->cchMozi;
					if( 0xFF >= chn )	//	�������p��
					{
						if( isspace( chn ) )	//	�܂��󔒂Ȃ�
						{
							ltrItr->mzStyle |= CT_WARNING;
							bWarn = TRUE;	//	�����󔒂�������A�x���ƃt���O�n�m
							iRslt = 1;
						}
					}
				}
			}
			else	//	�߂Ɍx���o�Ă��炻�̂܂܌x�����ꂿ�Ⴄ
			{
				ltrItr->mzStyle |= CT_WARNING;
			}
		}
		else{	bWarn = FALSE;	}	//	���p�󔒈ȊO�Ȃ�x�����[�h�����ďI���
	}

	//	���[�ɋ󔒂������ǂ����m�F
	ltrEnd--;
	if( iswspace( ltrEnd->cchMozi ) ){	iRslt = 1;	}

	//	�s���ɔ��p�󔒂Ȃ����m�F
	ch = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( 0 ).cchMozi;
	if( 0xFF >= ch )	//	���p��������
	{
		if( isspace( ch ) )
		{
			gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( 0 ).mzStyle |= CT_WARNING;
			iRslt = 1;
		}
	}

#pragma message ("DocBadSpaceCheck���̍X�V�w�߂̍œK�����K�v")

	//	�O��܂łƏ󋵂�����Ă�����
	if( iRslt != gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).bBadSpace )
	{
//		ViewRedrawSetVartRuler( rdLine );	//	���[���ĕ`��
		ViewRedrawSetLine( rdLine );	//	���[�����ƍs�ĕ`��
	}
	gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).bBadSpace = iRslt;

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃t�@�C���̕Ő���Ԃ�
	@return	INT_PTR	�Ő�
*/
INT_PTR DocPageCount( VOID )
{
	return gstFile.vcCont.size( );
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃y�[�W�̑��s���ƕ������ƃo�C�g����Ԃ��E���łɃo�C�g���Ƃ��X�V
	@param[in]	pdMozi	�����������o�b�t�@�ւ̃|�C���^�ENULL�ł���
	@param[in]	pdByte	�o�C�g�������o�b�t�@�ւ̃|�C���^�ENULL�ł���
	@return	UINT	�s��
*/
UINT DocPageParamGet( PINT pdMozi, PINT pdByte )
{
	INT_PTR	iLines, i, dMozis = 0;
	INT		dBytes = 0;

	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );

	for( i = 0; iLines > i; i++ )
	{
		//	���s�̃o�C�g���E2ch�AYY��6byte�E������΁�4byte
		if( 1 <= i )	//	��s�ڂ�����s���ǉ�
		{
			if( gbCrLfCode )	dBytes += YY2_CRLF;
			else				dBytes += STRB_CRLF;
		}

		dMozis += gstFile.vcCont.at( gixFocusPage ).vcPage.at( i ).vcLine.size( );
		dBytes += gstFile.vcCont.at( gixFocusPage ).vcPage.at( i ).iByteSz;
	}

	if( pdMozi )	*pdMozi = dMozis;
	if( pdByte )	*pdByte = dBytes;

	gstFile.vcCont.at( gixFocusPage ).dByteSz = dBytes;

	DocPageInfoRenew( -1, 1 );

	return iLines;
}
//-------------------------------------------------------------------------------------------------

/*!
	�Y���y�[�W�̃o�C�g���ƕ��������J�E���g������
	@param[in]	dPage	�Ŏw��E�����Ȃ猻�݂̕�
	@param[out]	pMozi	������������ENULL��
*/
INT DocPageByteCount( INT dPage, PINT pMozi )
{
	INT		iBytes, iMozis;
	LINE_ITR	itLine, endLine;
	LTR_ITR		itMozi, endMozi;

	if( 0 > dPage ){	dPage = gixFocusPage;	}

//#error �s���Ƃ��̏����܂Ƃ߂ă��j���[�����ق�������

	iBytes = 0;
	iMozis = 0;

	itLine  = gstFile.vcCont.at( dPage ).vcPage.begin();
	endLine = gstFile.vcCont.at( dPage ).vcPage.end();

	for( ; itLine != endLine; itLine++ )
	{
		itMozi  = itLine->vcLine.begin();
		endMozi = itLine->vcLine.end();

		for( ; itMozi != endMozi; itMozi++ )
		{
			iBytes += itMozi->mzByte;
			iMozis++;
		}
	}

	gstFile.vcCont.at( dPage ).dByteSz = iBytes;

	if( pMozi ){	*pMozi = iMozis;	}

	return iBytes;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ňꗗ�ɕ\��������e�𑗂�B
	@param[in]	dPage	�Ŏw��E�����Ȃ猻�݂̕�
	@param[in]	bMode	��O�X�e�[�^�X�o�[�\���E�O�X�e�[�^�X�o�[����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageInfoRenew( INT dPage, UINT bMode )
{
	UINT_PTR	dLines;
	UINT		dBytes;
	TCHAR		atBuff[SUB_STRING];

	if( 0 > dPage ){	dPage = gixFocusPage;	}


	dBytes = gstFile.vcCont.at( dPage ).dByteSz;
	
	if( bMode )
	{
		//	�o�C�g��������
		StringCchPrintf( atBuff, SUB_STRING, TEXT("%d Bytes"), dBytes );
		StatusBarSetText( SB_BYTECNT, atBuff );
	}

	dLines = gstFile.vcCont.at( dPage ).vcPage.size( );

	PageListInfoSet( dPage, dBytes, dLines );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��͈͂̍ł������w�h�b�g����Ԃ�
	@param[in]	dTop	�J�n�s�E�܂ށE�|�P�ōŏ�����
	@param[in]	dBottom	�I���s�E�܂ށE�|�P�ōŌ�܂�
	@return	INT		�h�b�g��
*/
INT DocPageMaxDotGet( INT dTop, INT dBottom )
{
	INT		maxDot = 0, thisDot, i;
	UINT_PTR	iLines;

	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );
	if( 0 > dTop )		dTop = 0;
	if( 0 > dBottom )	dBottom = iLines - 1;

	for( i = dTop; dBottom >= i; i++ )
	{
		thisDot = gstFile.vcCont.at( gixFocusPage ).vcPage.at( i ).iDotCnt;
	//	TRACE( TEXT("MAX:%d THIS:%d"), maxDot, thisDot );
		if( maxDot < thisDot )	maxDot = thisDot;
	}

	return maxDot;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̎g�p�h�b�g���ƕ������ƃo�C�g����Ԃ�
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@param[in]	pdMozi	�������ENULL�ł�OK
	@param[in]	pdByte	�o�C�g���ENULL�ł�OK
	@return	INT		�h�b�g��
*/
INT DocLineParamGet( INT rdLine, PINT pdMozi, PINT pdByte )
{
	INT_PTR	iCount, i, iLines;
	INT		dDotCnt, dByteCnt;

	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );
	if( iLines <= rdLine )	return -1;


	iCount = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.size( );

	//	�������K�v�Ȃ�
	if( pdMozi )	*pdMozi = iCount;

	dDotCnt = 0;
	dByteCnt = 0;
	for( i = 0; iCount > i; i++ )
	{
		dDotCnt += gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).rdWidth;

		//	���o�C�g��
		dByteCnt += gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).mzByte;
		
	}

	if( pdByte )	*pdByte = dByteCnt;

	//	�����̃T�C�Y����Ƃ�
	gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).iDotCnt = dDotCnt;
	gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).iByteSz = dByteCnt;

	return dDotCnt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�s���ƃh�b�g�l���󂯎���āA���̏ꏊ�̕�������Ԃ��E�L�����b�g�ʒu����Ă��璼��
	@param[in]	pNowDot	���̃L�����b�g�E������؂�ɂȂ�悤�Ɉʒu������
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@param[in]	round	�O�F�l�̌ܓ��I�ȁ@�����F���̕����Œ�@�����F�O�̕����Œ�
	@return		������
*/
INT DocLetterPosGetAdjust( PINT pNowDot, INT rdLine, INT round )
{
	INT		i, iCount, iLines;	//	INT_PTR
	INT		iLetter;	//	�L�����b�g�̍����̕�����
	INT		dDotCnt = 0, dPrvCnt = 0, rdWidth = 0;

	assert( pNowDot );

	//	�s�̂͂ݏo���ƕ������m�F
	iLines = DocLineParamGet( rdLine, &iCount, NULL );
	if( 0 > iLines )	return 0;

	for( i = 0, iLetter = 0; iCount > i; i++, iLetter++ )
	{
		if( dDotCnt >= *pNowDot ){	break;	}

		dPrvCnt = dDotCnt;
		rdWidth = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).rdWidth;
		dDotCnt += rdWidth;
	}	//	�U��؂�悤�Ȃ疖�[

	if( dDotCnt != *pNowDot )	//	�����L�����b�g�ʒu���������E����Ȃ�������
	{
		if( 0 <  round )	//	��Ύ��̕���
		{
			*pNowDot = dDotCnt;
		}
		else if( 0 > round )	//	��ΑO�̕���
		{
			*pNowDot = dPrvCnt;
			iLetter--;	//	��O�Ȃ̂�
		}
		else	//	�߂�����
		{
			//	���E�Ƃ̋������m�F���āA�߂����ɍ��킹��
			if( (*pNowDot - dPrvCnt) < (dDotCnt - *pNowDot ) )
			{
				*pNowDot = dPrvCnt;
				iLetter--;	//	��O�Ȃ̂�
			}
			else	//	���̕����̖@���߂�
			{
				*pNowDot = dDotCnt;
			}
		}
	}

	return iLetter;
}
//-------------------------------------------------------------------------------------------------

/*!
	���݈ʒu����P�����O�サ���Ƃ��̈ʒu�𒲂ׂ�
	@param[in]	nowDot		���̃L�����b�g�̃h�b�g�ʒu
	@param[in]	rdLine		�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@param[in]	bDirect		�ړ������@(-)�擪�ց@(+)������
	@param[out]	pdAbsDot	�ړ���̐�΃h�b�g��������ENULL�ł���
	@param[out]	pbJump		���s�𒴂�����TRUE�A�����Ȃ�������FALSE�ENULL�ł���
	@return		���ꂽ�h�b�g��
*/
INT DocLetterShiftPos( INT nowDot, INT rdLine, INT bDirect, PINT pdAbsDot, PBOOLEAN pbJump )
{
	INT_PTR	iCount, iLetter, iLines;	//	�L�����b�g�̍����̕�����
	INT		dLtrDot = 0;//dDotCnt = 0;

	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );
	if( iLines <=  rdLine )	return -1;



	if( 0 == bDirect )
	{
		if( pdAbsDot ){	*pdAbsDot = nowDot;	}
		if( pbJump ){	*pbJump = FALSE;	}
		return nowDot;	//	�ړ����������ł͈Ӗ�������
	}

	iCount = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.size( );	//	���̍s�̕������m�F����

	//	�������J�E���g
	iLetter = DocLetterPosGetAdjust( &nowDot, rdLine, 0 );

	//	���[�ō��ֈړ��A�������́A�E�[�ŉE�ֈړ��Ƃ������Ƃׂ͗̍s�ւ̈ړ��ɂȂ�
	if( ((0 == iLetter) && (0 > bDirect)) || ((iCount <= iLetter) && (0 < bDirect)) )
	{
		if((0 > bDirect) && (0 == rdLine))	return 0;
		//	�i���[���j�擪�s�ō��ֈړ�����̂Ȃ�A���s�͔������Ȃ�

		if( pbJump ){	*pbJump =  TRUE;	}

		return 0;	//	�͈͊O�Ȃ̂łO�ɂ��Ă���
	}

	if( 0 > bDirect )	//	��(�擪)�ֈړ�
	{
		dLtrDot = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( iLetter-1 ).rdWidth;
		//	���O�̕����̕����m�F
		nowDot -= dLtrDot;
	}//	dDotCnt

	if( 0 < bDirect )	//	�E(����)�ֈړ�
	{
		dLtrDot = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( iLetter ).rdWidth;
		//	����̕����̕����m�F
		nowDot += dLtrDot;
	}//	dDotCnt

	if( pdAbsDot )	*pdAbsDot = nowDot;//dDotCnt

	return dLtrDot;	//	�h�b�g���߂��Ăn�j
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̃e�L�X�g���w�肵������������Q�b�g����
	�|�C���^�����n���Ă������Ń������m�ۂ���B�J���͌Ă񂾑���
	@param[in]	rdLine		�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@param[in]	iStart		�J�n�����ʒu�O�C���f�b�N�X�E���̕�������J�n�E�펞�O�ł悢�H
	@param[out]	*pstTexts	�����ƃX�^�C�����i�[����o�b�t�@����邽�߂̃|�C���^�[�ւ̃|�C���^�[�ENULL�Ȃ�K�v�����������Ԃ�
	@param[out]	pchLen		�m�ۂ����������ENULL�^�[�~�l�[�^�̓m�[�J���E�o�C�g����Ȃ���
	@param[out]	pdFlag		������ɂ��āE���ʂ̂Ƃ��A���󔒂Ƃ��ENULL�s��
	@return					������̎g�p�h�b�g��
*/
INT DocLineDataGetAlloc( INT rdLine, INT iStart, LPLETTER *pstTexts, PINT pchLen, PUINT pdFlag )
{
	INT		iSize, i = 0, j, dotCnt;
	INT_PTR	iCount, iLines;

	//	�n�_�ƏI�_���g����悤�ɂ���	//	�|�P�Ȃ疖�[

	assert( pdFlag );
	assert( pchLen );

	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );
	if( iLines <=  rdLine )	return -1;

	iCount = (gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.size( ) );

	*pdFlag = 0;

	if( 0 == iCount )	//	������̒��g���Ȃ�
	{
		*pchLen = 0;
		dotCnt  = 0;
	}
	else
	{
		if( iStart >= iCount )	return 0;	//	�ʂ�߂���

		iSize = iCount - iStart;	//	������������
		//	�F�����̕K�v������Ƃ���܂łƂ��A��򂸂Ŗʓ|����悤��
		*pchLen = iSize;
		iSize++;	//	NULL�^�[�~�l�[�^�ׂ̈ɑ��₷

		if( !pstTexts )	return 0;	//	�����Ƃ���Ȃ��Ȃ炱���ŏI���

		*pstTexts = (LPLETTER)malloc( iSize * sizeof(LETTER) );
		if( !( *pstTexts ) ){	TRACE( TEXT("malloc error") );	return 0;	}

		ZeroMemory( *pstTexts, iSize * sizeof(LETTER) );

		dotCnt = 0;
		for( i = iStart, j = 0; iCount > i; i++, j++ )
		{
			(*pstTexts)[j].cchMozi = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).cchMozi;
			(*pstTexts)[j].rdWidth = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).rdWidth;
			(*pstTexts)[j].mzStyle = gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).mzStyle;

			dotCnt += gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( i ).rdWidth;

		}

		//	���[��space���ǂ����m�F
		if( iswspace( gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).vcLine.at( iCount-1 ).cchMozi ) )
		{	*pdFlag |= CT_LASTSP;	}
	}

	if( iLines - 1 >  rdLine )	*pdFlag |= CT_RETURN;	//	���̍s������Ȃ���s
	else						*pdFlag |= CT_EOF;		//	�Ȃ��Ȃ炱�̍s���[��EOF

	//	���s�̏�Ԃ��m��
	*pdFlag |= gstFile.vcCont.at( gixFocusPage ).vcPage.at( rdLine ).dStyle;

	return dotCnt;
}
//-------------------------------------------------------------------------------------------------

/*!
	������̍s���𐔂���E���j�R�[�h�p
	@param[in]	ptStr	��������������
	@param[in]	cchSize	������
	@return		�s��
*/
INT DocLineCount( LPTSTR ptStr, UINT cchSize )
{
	INT	lineCnt = 0;
	LPTSTR	ptEnd, ptCaret;

	ptCaret = ptStr;

	do
	{
		ptEnd = StrStr( ptCaret, CH_CRLFW );	//	���s��T��
		if( !ptEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			lineCnt++;
			break;
		}

		ptCaret = NextLineW( ptEnd );

		lineCnt++;

	}while( *ptCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	TRACE( TEXT("LINE COUNT [%d]"), lineCnt );

	return lineCnt;
}
//-------------------------------------------------------------------------------------------------


/*!
	�y�[�W�S�̂��m�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	bStyle	�P���j�R�[�h���V�t�gJIS
	@param[out]	*pText	�m�ۂ����̈��Ԃ��E���C�h�������}���`�����ɂȂ�ENULL���ƕK�v�o�C�g����Ԃ��̂�
	@return				�m�ۂ����o�C�g���ENULL�^�[�~�l�[�^���܂�
*/
INT DocPageTextAllGetAlloc( UINT bStyle, LPVOID *pText )
{
	//	SJIS�̏ꍇ�́A���j�R�[�h������&#dddd;�Ŋm�ۂ����

	UINT_PTR	iLines, i, iLetters, j;
	INT_PTR		iSize;

	string	srString;	//	���j�R�[�h�E�V�t�gJIS�Ŋm��
	wstring	wsString;

	srString.clear( );
	wsString.clear( );

	//	�y�[�W�S�̂̍s��
	iLines = gstFile.vcCont.at( gixFocusPage ).vcPage.size( );

	for( i = 0; iLines > i; i++ )
	{
		//	�e�s�̕�����
		iLetters = gstFile.vcCont.at( gixFocusPage ).vcPage.at( i ).vcLine.size( );

		if( bStyle & D_UNI )
		{
			for( j = 0; iLetters > j; j++ )
			{
				wsString += gstFile.vcCont.at( gixFocusPage ).vcPage.at( i ).vcLine.at( j ).cchMozi;
			}

			if( iLines > (i+1) )	wsString += wstring( CH_CRLFW );
		}
		else
		{
			for( j = 0; iLetters > j; j++ )
			{
				srString +=  string( gstFile.vcCont.at( gixFocusPage ).vcPage.at( i ).vcLine.at( j ).acSjis );
			}

			if( iLines > (i+1) )	srString +=  string( CH_CRLFA );
		}
	}

	if( bStyle & D_UNI )	//	���j�R�[�h�ł���
	{
		iSize = wsString.size( ) + 1;	//	NULL�^�[�~�l�[�^������
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
		iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^������

		if( pText )
		{
			*pText = (LPSTR)malloc( iSize );
			ZeroMemory( *pText, iSize );
			StringCchCopyA( (LPSTR)(*pText), iSize, srString.c_str( ) );
		}
	}

	return iSize;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w�肳�ꂽ�y�[�W�S�̂��v���r���[�p��SJIS�Ŋm�ۂ���Efree�͌Ă񂾕��ł��
	@param[in]	iPage	�^�[�Q�b�g�Ŕԍ�
	@param[out]	pdBytes	�m�ۂ����o�C�g���Ԃ��ENULL�^�[�~�l�[�^���܂�
	@return				�m�ۂ����̈�E�}���`�����ɂȂ�
*/
LPSTR DocPageTextPreviewAlloc( INT iPage, PINT pdBytes )
{
	//	SJIS�̏ꍇ�́A���j�R�[�h������&#dddd;�Ŋm�ۂ����

	UINT_PTR	iLines, i, iLetters;
	INT_PTR		iSize;
	LPSTR	pcText = NULL;
	CHAR	acEntity[10];

	string	srString;	//	�V�t�gJIS�Ŋm��
	LINE_ITR	itLine, itLineEnd;
	LTR_ITR		itLtr;

	srString.clear( );

	if( pdBytes )	*pdBytes = 0;

	if( DocRangeIsError( iPage, 0 ) )	return NULL;

	//	�y�[�W�S�̂̍s��
	iLines = gstFile.vcCont.at( iPage ).vcPage.size( );

	itLine    = gstFile.vcCont.at( iPage ).vcPage.begin( );
	itLineEnd = gstFile.vcCont.at( iPage ).vcPage.end( );

	for( i = 0; itLine != itLineEnd; i++, itLine++ )
	{
		//	�e�s�̕�����
		iLetters = itLine->vcLine.size( );

		for( itLtr = itLine->vcLine.begin(); itLtr != itLine->vcLine.end(); itLtr++ )
		{
			//	HTML�I�Ƀ��o�C�������G���e�B�e�B����
			if( HtmlEntityCheck( itLtr->cchMozi, acEntity, 10 ) )
			{
				srString +=  string( acEntity );
			}
			else
			{
				srString +=  string( itLtr->acSjis );
			}
		}

//�S�s�ɉ��s�����Ă��܂�Ȃ��H
		srString +=  string( "<br>" );
	}

	iSize = srString.size( ) + 1;	//	NULL�^�[�~�l�[�^������

	pcText = (LPSTR)malloc( iSize );
	ZeroMemory( pcText, iSize );
	StringCchCopyA( pcText, iSize, srString.c_str( ) );

	if( pdBytes )	*pdBytes = iSize;

	return pcText;
}
//-------------------------------------------------------------------------------------------------


/*!
	���݂̃��j�R�[�h���l�Q�Ƃ̋�ɍ��킹�ĕ�������`�F�C���W
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UnicodeRadixExchange( LPVOID pVoid )
{
	INT_PTR	iPage, iLine, iMozi, dP, dL, dM;
	TCHAR	cchMozi;
	CHAR	acSjis[10];

	iPage = gstFile.vcCont.size(  );

	for( dP = 0; iPage >  dP; dP++ )	//	�S��
	{
		iLine = gstFile.vcCont.at( dP ).vcPage.size(  );

		for( dL = 0; iLine >  dL; dL++ )	//	�S�s
		{
			iMozi = gstFile.vcCont.at( dP ).vcPage.at( dL ).vcLine.size(  );

			for( dM = 0; iMozi >  dM; dM++ )	//	�S��
			{
				if( gstFile.vcCont.at( dP ).vcPage.at( dL ).vcLine.at( dM ).mzStyle & CT_CANTSJIS )
				{
					cchMozi = gstFile.vcCont.at( dP ).vcPage.at( dL ).vcLine.at( dM ).cchMozi;
					if( gbUniRadixHex ){	StringCchPrintfA( acSjis, 10, ("&#x%X;"), cchMozi );	}
					else{					StringCchPrintfA( acSjis, 10, ("&#%d;"),  cchMozi );	}

					StringCchCopyA( gstFile.vcCont.at( dP ).vcPage.at( dL ).vcLine.at( dM ).acSjis, 10, acSjis );
//	TODO:	�o�C�g���Čv�Z���K�v
				}
			}
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�y�[�W���������E�J�[�\���ʒu�̎��̍s����I���܂ł����̕ł�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	iNow	���̍s
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocPageDivide( HWND hWnd, HINSTANCE hInst, INT iNow )
{
	INT	iDivLine = iNow + 1;
	INT	iLines, mRslt, iNewPage;
//	INT_PTR	iTotal, iNext;
	ONELINE	stLine;
	LINE_ITR	itLine, itEnd;

	ZeroONELINE( &stLine );

	//mRslt = MessageBox( hWnd, TEXT("��������������畜�A�ł��Ȃ��̂ł���E�E�E\r\n�{���Ƀo�����Ă����̂ł����H"), TEXT("�m�F�Ȃ̂ł��B��������"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
	mRslt = MessageBoxCheckBox( hWnd, hInst, 1 );
	if( IDNO == mRslt ){	return  E_ABORT;	}

//�����́A�A���h�D�����Z�b�g���ׂ�
//���̕ł̊Y���������폜�����Ⴄ

	iLines = DocPageParamGet( NULL, NULL );
	if( iLines <= iDivLine )	return E_OUTOFMEMORY;

	//	���̕ł̎��ɍ쐬
	//iTotal = gstFile.vcCont.size(  );
	//iNext = gixFocusPage + 1;	//	���̕�
	//if( iTotal <= iNext ){	iNext =  -1;	}	//	�S�ł�葽���Ȃ疖�[�w��

	iNewPage = DocPageCreate( gixFocusPage );	//	�V��
	PageListInsert( iNewPage  );	//	�y�[�W���X�g�r���[�ɒǉ�

	//	��̈�s������Ă�̂ŁA�폜���Ă���
	gstFile.vcCont.at( iNewPage ).vcPage.clear(  );

	itLine = gstFile.vcCont.at( gixFocusPage ).vcPage.begin( );
	itLine += iDivLine;

	itEnd  = gstFile.vcCont.at( gixFocusPage ).vcPage.end( );

	std::copy(	itLine, itEnd, back_inserter( gstFile.vcCont.at( iNewPage ).vcPage ) );

	gstFile.vcCont.at( gixFocusPage ).vcPage.erase( itLine, itEnd );

	SqnFreeAll( &(gstFile.vcCont.at( gixFocusPage ).stUndoLog) );	//	�A���h�D���O�폜

	//	�o�C�g���Ƃ��̎�蒼��
	DocPageByteCount( gixFocusPage, NULL );
	DocPageInfoRenew( gixFocusPage, TRUE );

	DocPageByteCount( iNewPage, NULL );
	DocPageInfoRenew( iNewPage, FALSE );

	ViewRedrawSetLine( -1 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�z�b�g�L�[�ɂ�铊���̒���
*/
HRESULT DocThreadDropCopy( VOID )
{
	CHAR	acBuf[260];
	TCHAR	atTitle[64], atInfo[256];
	INT	cbSize, dFocusBuf, maxPage;
	LPVOID	pcString = NULL;

	dFocusBuf = gixFocusPage;	//	���ݕł���U�Ҕ�������

	gixFocusPage = gixDropPage;	//	�����p�łɂ���

	cbSize = DocPageTextAllGetAlloc( D_SJIS, &pcString );

	gixFocusPage = dFocusBuf;	//	�I�������߂�

	TRACE( TEXT("%d �ł��R�s�["), gixDropPage );

	DocClipboardDataSet( pcString, cbSize, D_SJIS );

	ZeroMemory( acBuf, sizeof(acBuf) );
	StringCchCopyNA( acBuf, 260, (LPCSTR)pcString, 250 );
	ZeroMemory( atInfo, sizeof(atInfo) );
	MultiByteToWideChar( CP_ACP, 0, acBuf, (INT)strlen(acBuf), atInfo, 256 );

	StringCchPrintf( atTitle, 64, TEXT("%d �ł��R�s�[�����̂ł��B����"), gixDropPage + 1 );

	NotifyBalloonExist( atInfo, atTitle, NIIF_INFO );

	FREE( pcString );

	gixDropPage++;	//	���̕ł�

	maxPage = DocPageCount(  );
	if( maxPage <= gixDropPage )	gixDropPage = 0;
	//	�ŏI�ł܂ŃC�b����擪�ɖ߂�


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

