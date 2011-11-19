/*! @file
	@brief ���[�U��`�}���̖ʓ|���܂�
	���̃t�@�C���� UserDefinition.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/16
*/

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

//	���[�U��`�A���͗v��Ȃ��H�l�k�s�ꗗ�̂ق��Ŗ�薳���̂ł�

//	���[�U��`���
typedef struct tagUSERITEMS
{
	TCHAR	atItemName[MAX_STRING];

	vector<ONELINE>	vcUnits;	//!<	�S��

} USERITEMS, *LPUSERITEMS;
//-------------------------------------------------------------------------------------------------

extern INT		gdDocLine;		//!<	�L�����b�g�̂x�s���E�h�L�������g�ʒu


static TCHAR	gatUsDfPath[MAX_PATH];	//!<	

static  UINT	gdItemCnt;	//!<	

static USERITEMS	gstUserItem[USER_ITEM_MAX];	//	�Œ�ł�������
//-------------------------------------------------------------------------------------------------

UINT	CALLBACK UserDefItemLoad( LPTSTR, LPTSTR, INT );	//!<	

HRESULT	UserDefAppendMenu( HWND );	//!<	
//-------------------------------------------------------------------------------------------------


/*!
	���J���Ă�USER��`�̓��e��S�Ĕj�����ĐV�����̍����Ԃɂ���
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefObliterate( HWND hWnd )
{
	UINT_PTR	i, j, iLine;

	for( i = 0; USER_ITEM_MAX > i; i++ )
	{
		iLine = gstUserItem[i].vcUnits.size( );
		for( j = 0; iLine > j; j++ )
		{
			gstUserItem[i].vcUnits.at( j ).vcLine.clear(   );	//	�e�s�̒��g�S����
		}
		gstUserItem[i].vcUnits.clear(  );	//	�s��S����

		ZeroMemory( gstUserItem[i].atItemName, sizeof(gstUserItem[i].atItemName) );
	}

	gdItemCnt = 0;

	//	���j���[�̓��e���A�����ɂ��Ƃ���

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�U��`�̃A����ǂݍ���
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@param[in]	bFirst	���񂪍ŏ��ł��邩
	@return	��O���g��������������@�O�t�@�C���Ȃ�����
*/
INT UserDefInitialise( HWND hWnd, UINT bFirst )
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

	if( bFirst )	//	�ŏ��Ȃ�p�X����Ă���
	{
		ZeroMemory( gatUsDfPath, sizeof(gatUsDfPath) );
		StringCchCopy( gatUsDfPath, MAX_PATH, ExePathGet() );
		PathAppend( gatUsDfPath, TEMPLATE_DIR );
		PathAppend( gatUsDfPath, USER_ITEM_FILE );
	}

	hFile = CreateFile( gatUsDfPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return 0;	}

	//	���̓��e�j��
	UserDefObliterate( hWnd );

	iByteSize = GetFileSize( hFile, NULL );
	pBuffer = malloc( iByteSize + 2 );
	ZeroMemory( pBuffer, iByteSize + 2 );
	//	���g����������
	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
	ReadFile( hFile, pBuffer, iByteSize, &readed, NULL );
	CloseHandle( hFile );	//	���e�S����荞�񂾂���J��

	//	���j�R�[�h�`���b�N
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

	//	AST����Ȃ���_��
	if( !( StrCmpN( AST_SEPARATERW, ptString, 4 ) ) )
	{
		DocStringSplitAST( ptString , cchSize, UserDefItemLoad );
	}

	FREE( pBuffer );

	UserDefAppendMenu( hWnd );

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	���e���Ԃ�����
	@param[in]	ptName	���ڂ̖��O
	@param[in]	ptCont	���ڂ̓��e
	@param[in]	cchSize	���e�̕�����
	@return	��O����肵���@�O���Ȃ�����
*/
UINT CALLBACK UserDefItemLoad( LPTSTR ptName, LPTSTR ptCont, INT cchSize )
{
	//	�g���̂͂���
	//	���O���Z�b�g���Ă���

	//	�͂ݏo�����͉������Ȃ���
	if( USER_ITEM_MAX <= gdItemCnt )	return 0;

	if( ptName )
	{
		StringCchCopy( gstUserItem[gdItemCnt].atItemName, MAX_STRING, ptName );
	}
	else
	{
		StringCchPrintf( gstUserItem[gdItemCnt].atItemName, MAX_STRING, TEXT("(���[�U�A�C�e�� No.%d"), gdItemCnt + 1 );
	}

	if( 0 < cchSize )	//	��s�łȂ��̂Ȃ�
	{
		UserDefSetString( &(gstUserItem[gdItemCnt].vcUnits), ptCont, cchSize );	//	���̒��ŉ��s�Ƃ��ʓ|����
	}

	gdItemCnt++;

	return 1;
}
//-------------------------------------------------------------------------------------------------


/*!
	�ێ��\���̂ɕ�������L�^����
	@param[in]	*pvcUnits	�ێ��x�N�[�^�ւ̃|�C���^�[
	@param[in]	ptText		�L�^���镶����
	@param[in]	cchSize		������
	@return		HRESULT		�I����ԃR�[�h
*/
HRESULT UserDefSetString( vector<ONELINE> *pvcUnits, LPTSTR ptText, UINT cchSize )
{
	UINT	i;
	INT		yLine;
	ONELINE	stLine;
	LETTER	stLetter;

	ZeroONELINE( &stLine );

	pvcUnits->push_back( stLine );	//	�딭��

	yLine = 0;
	for( i = 0; cchSize > i; i++ )
	{
		if( CC_CR == ptText[i] && CC_LF == ptText[i+1] )	//	���s�ł�������
		{
			pvcUnits->push_back( stLine );

			i++;		//	0x0D,0x0A������A�땶����΂��̂��|�C���g
			yLine++;	//	���s��������Focus�͎��̍s��
		}
		else if( CC_TAB == ptText[i] )
		{
			//	�^�u�͑}�����Ȃ�
		}
		else
		{
			ZeroMemory( &stLetter, sizeof(LETTER) );
			stLetter.cchMozi = ptText[i];
			stLetter.rdWidth = ViewLetterWidthGet( ptText[i] );
			stLetter.mzStyle = CT_NORMAL;
			if( iswspace( ptText[i] ) ){	stLetter.mzStyle |= CT_SPACE;	}
			if( !( DocIsSjisTrance( ptText[i], stLetter.acSjis ) ) ){	stLetter.mzStyle |= CT_CANTSJIS;	}
			//	��V�t�gJIS�������m�F
			DocLetterByteCheck( &stLetter );	//	�o�C�g���m�F
			//stLetter.mzByte  = strlen( stLetter.acSjis );

			pvcUnits->at( yLine ).vcLine.push_back( stLetter );

			pvcUnits->at( yLine ).iDotCnt += stLetter.rdWidth;
			pvcUnits->at( yLine ).iByteSz += stLetter.mzByte;
		}
	}

	//	�����Ƃ��̋󔒂��܂߂ăe���v�����ȂƎv���Ă�

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�U��`�̓��e�����j���[�ɒǉ�
	@param[in]	hWnd	���C���E�C���h�E�n���h��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefAppendMenu( HWND hWnd )
{
	HMENU	hMenu, hSubMenu;

	//	���j���[�\���ς�����炱�����ύX�E�ǂ��ɂ��Ȃ��̂�
	hMenu = GetMenu( hWnd );
	hSubMenu = GetSubMenu( hMenu, 2 );
	hMenu = hSubMenu;
	hSubMenu = GetSubMenu( hMenu, 6 );

	UserDefMenuWrite( hSubMenu );

	DeleteMenu( hSubMenu, IDM_USERINS_NA, MF_BYCOMMAND );	//	Dummy�p

	DrawMenuBar( hWnd );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���[�U��`���j���[�̒��g����������
*/
HRESULT UserDefMenuWrite( HMENU hMenu )
{
	UINT	i;

	for( i = 0; gdItemCnt > i; i++ )
	{
		AppendMenu( hMenu, MF_STRING, (IDM_USERINS_ITEM_FIRST + i), gstUserItem[i].atItemName );
	}
	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ΏۃA�C�e���̑Ώۍs�̓��e���e�L�X�g�Ŋm�ہE���s�͊܂܂�
	@param[in]	idNum	�A�C�e���ԍ��O�`�P�T
	@param[in]	uLine	�s�ԍ�
	@return		LPTSTR	�m�ۂ������e�E�J���͌Ă񂾑����ӔC���Ă�
*/
LPTSTR UserDefTextLineAlloc( UINT idNum, INT uLine )
{
	INT_PTR	iLines, iLetters, i, cchSz;
	LPTSTR	ptText;

	if( gdItemCnt <= idNum )	return NULL;

	iLines = gstUserItem[idNum].vcUnits.size( );
	if( iLines <= uLine )	return NULL;

	iLetters = gstUserItem[idNum].vcUnits.at( uLine ).vcLine.size( );

	cchSz = iLetters + 1;

	ptText = (LPTSTR)malloc( cchSz * sizeof(TCHAR) );
	ZeroMemory( ptText, cchSz * sizeof(TCHAR) );

	for( i = 0; iLetters > i; i++ )
	{
		ptText[i] = gstUserItem[idNum].vcUnits.at( uLine ).vcLine.at( i ).cchMozi;
	}
	ptText[i] = NULL;

	return ptText;
}
//-------------------------------------------------------------------------------------------------

/*!
	�I�����ꂽ���[�U��`�A�C�e���̏���
	@param[in]	hWnd	�������C���E�C���h�E�n���h��
	@param[in]	idNum	�A�C�e���ԍ��O�`�P�T
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT UserDefItemInsert( HWND hWnd, UINT idNum )
{
	INT		iLines, yLine, iMinus, i, dmyDot;
	INT_PTR	dNeedLine;
	LPTSTR	ptText;
	BOOLEAN	bFirst = TRUE;


	if( gdItemCnt <= idNum )	return E_OUTOFMEMORY;

	//	���̃J�[�\���s����A�s�擪�ɁA�e�s�̓��e��}�����Ă���
	yLine = gdDocLine;

	dNeedLine = gstUserItem[idNum].vcUnits.size( );

	//	�܂��͕ōs�������ʂ�
	iLines = DocPageParamGet( NULL, NULL );
	//	�S�̍s�����A�ǉ��s��������������A���s���₷
	if( iLines < (dNeedLine + yLine) )
	{
		iMinus = (dNeedLine + yLine) - iLines;	//	�ǉ�����s��

		DocAdditionalLine( iMinus, TRUE );
		bFirst = FALSE;

		//	���̕ł̍s����蒼��
		iLines = DocPageParamGet( NULL, NULL );
	}

	for( i = 0; dNeedLine > i; i++, yLine++ )
	{
		ptText = UserDefTextLineAlloc( idNum, i );

		dmyDot = 0;
		DocInsertString( &dmyDot, &yLine, NULL, ptText, 0, bFirst );	bFirst = FALSE;

		FREE(ptText);
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
