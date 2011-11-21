/*! @file
	@brief MLT�t�@�C����ǂ�ŁA���g���������ɓW�J���܂�
	���̃t�@�C���� MaaCatalogue.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/06/22
*/
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#include "MaaTemplate.h"
//-------------------------------------------------------------------------------------------------

/*

AA��1�s��������2�s�ŁA�����e���ȉ��̕�����ȊO����n�܂�ꍇ�A
����AA�������o���Ƃ��ĔF�����Amenu���ڂɒǉ����܂��B

Last Mod
�ŏI�X
�@�@�i�S�p��2�j
�@ �i�S�p�󔒁{���p�󔒁j
:
_
�i
(
.
/

�������A�u��؂�.mlt�v�E�u�����o���E�͂ݗp.mlt�v�ɂ��ẮA�ȉ��̕����񂩂�n�܂�ꍇ�̂݁A
����AA�������o���Ƃ��ĔF���Amenu���ڂɒǉ����܂��B

�y


�ʓ|������̂ł��ƂŃV���N

�y�@�����Q�O�����ȏ�͔F�����Ȃ��Ƃ��̃��W�b�N�K�v��
*/


#define MLT_SEPARATER	("[SPLIT]")
#define AST_SEPARATER	("[AA]")



//!	MLT�̕ێ�
typedef struct tagAAMATRIX
{
	UINT	ixNum;	//!<	�ʂ��ԍ��O�C���f�b�N�X
	DWORD	cbItem;	//!<	AA�̃o�C�g��
	LPSTR	pcItem;	//!<	�ǂݍ���AA��ێ����Ă����|�C���^�ESJIS�`���̂܂܂ł������H

	CHAR	acAstName[MAX_STRING];	//!<	AST�t�@�C���̖��̂������Ă���

} AAMATRIX, *LPAAMATRIX;
//-------------------------------------------------------------------------------------------------

static  vector<AAMATRIX>	gvcArts;	//!<	�J����AA�̕ێ�
//-------------------------------------------------------------------------------------------------


DWORD	AacInflateMlt( LPSTR, DWORD );
DWORD	AacInflateAst( LPSTR, DWORD );

UINT	AacTitleCheck( LPAAMATRIX );

LPSTR	NextLine( LPSTR );

LRESULT	CALLBACK AacFavInflate( UINT, UINT, UINT, LPCVOID );

//-------------------------------------------------------------------------------------------------

/*!
	MLT�t�@�C�������󂯎���āA�W�J������i�߂�
	@param[in]	hWnd		�e�E�C���h�E�n���h��
	@param[in]	ptFileName	�t�@�C����
	@return		DWORD		�m�ۂ�����
*/
DWORD AacAssembleFile( HWND hWnd, LPTSTR ptFileName )
{
	HANDLE	hFile;
	DWORD	rdFileSize, readed, rdCount;
	BOOL	isAST;
	LPSTR	pcFullBuff;

	hFile = CreateFile( ptFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( INVALID_HANDLE_VALUE == hFile ){	return 0;	}

	SetFilePointer( hFile, 0, NULL, FILE_BEGIN );

	rdFileSize = GetFileSize( hFile, NULL );	//	�S�f�����ɑΉ����H

	//	AST�ł��邩�H
	isAST = PathMatchSpec( ptFileName , TEXT("*.ast") );

	//	�S�̃o�b�t�@�����
	pcFullBuff = (LPSTR)malloc( rdFileSize + 1 );
	if( !(pcFullBuff) ){	return 0;	}

	ZeroMemory( pcFullBuff, rdFileSize + 1 );
	//	���g�Ǎ�
	ReadFile( hFile, pcFullBuff, rdFileSize, &readed, NULL );

	CloseHandle( hFile );	//	���e�m�ۂ���������Ă���

	AacMatrixClear(   );	//	�����̓��e�S�j��

//	AaTitleClear(  );
//	AST�͂����œW�J����΂���
	//	�W�J��������
	if( isAST ){	rdCount = AacInflateAst( pcFullBuff, readed );	}
	else{			rdCount = AacInflateMlt( pcFullBuff, readed );	}

	free( pcFullBuff );	//	�J���Y��Ȃ��悤�ɒ��ӃZ��

	return rdCount;
}
//-------------------------------------------------------------------------------------------------

/*!
	AST�̑S�̃o�b�t�@���󂯎���āA�������ĕێ�
	@param[in]	pcTotal	�S�̃o�b�t�@
	@param[in]	cbTotal	�o�C�g��
	@return		DWORD	�m�ۂ�����
*/
DWORD AacInflateAst( LPSTR pcTotal, DWORD cbTotal )
{
	LPSTR	pcCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPSTR	pcStart;	//	�Z�p���[�^�̒��O
	LPSTR	pcEnd;
	UINT	iNumber;	//	�ʂ��ԍ��J�E���g

	UINT	cbItem;
	BOOLEAN	bLast;
//	CHAR	acName[MAX_STRING];

	AAMATRIX	stAAbuf;//	���AA�̕ێ��E�x�N�^�[�ɓ����


	bLast = FALSE;
	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X

	pcCaret = StrStrA( pcTotal, AST_SEPARATER );	//	�ŏ��̃Z�p���[�^�܂Ői�߂�
	if( !pcCaret )	return 0;	//	���̎��_�Ŗ��������炨������

	do	//	ptCaret��[AA]�ɂȂ��Ă�͂�
	{
		ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );

		stAAbuf.ixNum = iNumber;

		pcStart = NextLine( pcCaret );	//	���̍s���炪�{��

		pcCaret += 5;	//	[AA][
		cbItem  = pcStart - pcCaret;	//	���O�����̕�����
		cbItem -= 3;	//	]rn

		//	���O�m��
		if( 0 < cbItem )
		{
			StringCchCopyNA( stAAbuf.acAstName, MAX_STRING, pcCaret, cbItem );
			AaTitleAddString( iNumber, stAAbuf.acAstName );
		}

		pcCaret = pcStart;	//	�{�̕���

		pcEnd = StrStrA( pcCaret, AST_SEPARATER );	//	�Z�p���[�^��T��
		//	���̎��_��pcEnd�͎���[AA]�������Ă�E��������NULL(�Ō�̃R�})
		if( !pcEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			pcEnd = pcTotal + cbTotal;	//	CHAR�T�C�Y�Ōv�Z�����H
			bLast = TRUE;
		}
		stAAbuf.cbItem = pcEnd - pcCaret;	//	CHAR�P�ʂł��邩

		//	�ŏI�łłȂ��ꍇ�͖��[�̉��s������
		//if( !(bLast) && 0 < cbItem ){	cbItem -= CH_CRLF_CCH;	}

		stAAbuf.pcItem = (LPSTR)malloc( stAAbuf.cbItem + 2 );
		ZeroMemory( stAAbuf.pcItem, stAAbuf.cbItem + 2 );

		if( 0 >= stAAbuf.cbItem )
		{
			stAAbuf.cbItem = 2;
			StringCchCopyA( stAAbuf.pcItem, stAAbuf.cbItem, (" ") );
		}
		else
		{
			CopyMemory( stAAbuf.pcItem, pcCaret, stAAbuf.cbItem );
		}

		gvcArts.push_back( stAAbuf );	//	�x�N�^�[�ɒǉ�

		iNumber++;

		pcCaret = pcEnd;

	}while( *pcCaret );	//	�f�[�^�L����胋�[�v�ŒT��

	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	MLT�̑S�̃o�b�t�@���󂯎���āA�������ĕێ�
	@param[in]	pcTotal	�S�̃o�b�t�@
	@param[in]	cbTotal	�o�C�g��
	@return		DWORD	�m�ۂ�����
*/
DWORD AacInflateMlt( LPSTR pcTotal, DWORD cbTotal )
{
	LPSTR	pcCaret;	//	�Ǎ��J�n�E���݈ʒu
	LPSTR	pcEnd;		//	���AA�̖��[�ʒu�E�Z�p���[�^�̒��O
	DWORD	iNumber;	//	�ʂ��ԍ��J�E���g
	AAMATRIX	stAAbuf;//	���AA�̕ێ��E�x�N�^�[�ɓ����

	pcCaret = pcTotal;	//	�܂��͍ŏ�����

	iNumber = 0;	//	�ʂ��ԍ��O�C���f�b�N�X
	//	�n�_�ɂ̓Z�p���[�^�������̂Ƃ݂Ȃ��B�A������Z�p���[�^�́A���s�݂̂Ƃ��Ĉ���

	do
	{
		ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );

		stAAbuf.ixNum = iNumber;

		pcEnd = StrStrA( pcCaret, MLT_SEPARATER );	//	�Z�p���[�^��T��
		if( !pcEnd )	//	������Ȃ������灁���ꂪ�Ō�Ȃ灁NULL
		{
			pcEnd = pcTotal + cbTotal;
		}
		stAAbuf.cbItem = pcEnd - pcCaret;	//	�o�C�g���Ȃ̂ł���ł����͂�
		stAAbuf.pcItem = (LPSTR)malloc( stAAbuf.cbItem + 2 );
		ZeroMemory( stAAbuf.pcItem, stAAbuf.cbItem + 2 );

		if( 0 >= stAAbuf.cbItem )
		{
			stAAbuf.cbItem = 2;
			StringCchCopyA( stAAbuf.pcItem, stAAbuf.cbItem, (" ") );
		}
		else
		{
			CopyMemory( stAAbuf.pcItem, pcCaret, stAAbuf.cbItem );
		}

		//	���e���y�����z�ɂȂ��Ă������o���ƌ��Ȃ��H
		AacTitleCheck( &stAAbuf );

		gvcArts.push_back( stAAbuf );	//	�x�N�^�[�ɒǉ�

		iNumber++;

		pcCaret = NextLine( pcEnd );

	}while( *pcCaret );	//	�f�[�^�L����胋�[�v�ŒT��


	return iNumber;
}
//-------------------------------------------------------------------------------------------------

/*!
	���o���ɂȂ蓾��Tango��T��
	@param[in]	pstItem	�`�F�b�N����^�[�Q�b�g���e
	@return		UINT	��O���o���������@�O�Ȃ�����
*/
UINT AacTitleCheck( LPAAMATRIX pstItem )
{
	LPSTR	pcCaret, pcEnd, pcOpen;
	LPSTR	pcLine;
	UINT	cbSize, d;

	if( 4 > pstItem->cbItem )	return 0;	//	�����������牽������

	//	�Ƃ肠�����擪��s�ڂɂ���
	pcCaret = pstItem->pcItem;

	//	�擪�ɂ���
	if( !( strncmp( pcCaret, "�y", 2 ) ) )	//	�u�����P�b�g�ł��邩
	{
		pcOpen = pcCaret + 2;	//	���e�J�n�ʒu

		cbSize  = pstItem->cbItem;
		cbSize -= 2;	//	��s�̏ꍇ�����z���Čv�Z

		pcEnd = NextLine( pcCaret );
		//	���g��NULL�Ȃ�A���s������s�ł���
		if( *pcEnd )
		{
			pcCaret = pcOpen;
			cbSize = pcEnd - pcCaret;
		}
		//	�����ŁAcbSize�͈�s�̒����ɂȂ�
		pcLine = (LPSTR)malloc( cbSize+1 );
		ZeroMemory( pcLine, cbSize+1 );
		CopyMemory( pcLine, pcOpen, cbSize );
		pcCaret = pcLine;
		for( d = 0; cbSize > d; d++ )
		{
			if( !(strncmp( pcCaret, "�z", 2 ) ) )
			{
				*pcCaret = NULL;
				break;
			}
			pcCaret++;
		}
		//�����܂łŁApcLine�Ɂy�z�̕����񂪎c���Ă���͂�
		AaTitleAddString( pstItem->ixNum, pcLine );

		FREE(pcLine);
	}
	else
	{
		//	�擪������̕����ł���Ȃ猩�o���ł͂Ȃ�
		if( ':' ==  pcCaret[0] )	return 0;
		if( '_' ==  pcCaret[0] )	return 0;
		if( '(' ==  pcCaret[0] )	return 0;
		if( '.' ==  pcCaret[0] )	return 0;
		if( '/' ==  pcCaret[0] )	return 0;
		//	�擪������������̕�����ł���Ȃ猩�o���ł͂Ȃ�
		if( !( strncmp( pcCaret, "�i", 2 ) ) )	return 0;
		if( !( strncmp( pcCaret, "�@ ", 3 ) ) )	return 0;	//	�S�p�󔒁{���p��
		if( !( strncmp( pcCaret, "�@�@", 4 ) ) )	return 0;	//	�S�p��2��
		if( !( strncmp( pcCaret, "�ŏI�X", 4 ) ) )	return 0;
		if( !( strncmp( pcCaret, "Last Mod", 8 ) ) )	return 0;

		//	���o���ƌ��Ȃ��ē��e���Q�b�g����B
		cbSize = pstItem->cbItem;	//	��s�̏ꍇ

		pcEnd = NextLine( pcCaret );
		//NULL�Ȃ�A���s������s�ł���
		if( *pcEnd )
		{
			cbSize = pcEnd - pcCaret;	//	���s������

			pcOpen = pcEnd;
			pcEnd  = NextLine( pcOpen );	//	���s�m�F
			//	�����ŁA�R�s�ڈȍ~������Ό��o���Ƃ͌��Ȃ��Ȃ�
			if( *pcEnd )	return 0;
		}

		//	�����ŁAcbSize�͈�s�̒����ɂȂ�

		//	�����A�s�����ɒ���������A���o���Ƃ͌��Ȃ��Ȃ�
		if( 42 <= cbSize )	return 0;

		pcLine = (LPSTR)malloc( cbSize );
		ZeroMemory( pcLine, cbSize );
		CopyMemory( pcLine, pcCaret, cbSize-2 );	//	���s���J�b�g

		AaTitleAddString( pstItem->ixNum, pcLine );

		FREE(pcLine);
	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�ʂ��ԍ����󂯂āA���e��Ԃ�
	@param[in]	iNumber	�ʂ��ԍ��O�C���f�b�N�X
	@return	LPSTR	AA�̓��e���A�������m�ۂ��ēn���B�󂯎�������ŊJ������
*/
LPSTR AacAsciiArtGet( DWORD iNumber )
{
	size_t	items;
	LPSTR	pcBuff;

	items = gvcArts.size( );
	if( items <= iNumber )	return NULL;

	pcBuff = (LPSTR)malloc( gvcArts.at( iNumber ).cbItem + 1 );
	ZeroMemory( pcBuff, gvcArts.at( iNumber ).cbItem + 1 );
	CopyMemory( pcBuff, gvcArts.at( iNumber ).pcItem, gvcArts.at( iNumber ).cbItem );

	return pcBuff;
}
//-------------------------------------------------------------------------------------------------

/*!
	MLT�̕ێ����e��S�j��
	@return	HRESULT	�I����ԃR�[�h
*/
HRESULT AacMatrixClear( VOID )
{
	size_t	szItem, i;

	szItem = gvcArts.size();	//	���m�F����

	//	��ɗ̈���J��
	for( i = 0; szItem > i; i++ )
	{
		free( gvcArts.at( i ).pcItem );
	}

	gvcArts.clear();	//	�����đS�j��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���ݍs����A���̍s�̐擪�ֈړ�
	@param[in]	pt	���s�������J�n����Ƃ���
	@return		���s�̎��̈ʒu
*/
LPSTR NextLine( LPSTR pt )
{
	while( *pt && *pt != 0x0D ){	pt++;	}

	if( 0x0D == *pt )
	{
		pt++;
		if( 0x0A == *pt ){	pt++;	}
	}

	return pt;
}
//-------------------------------------------------------------------------------------------------

/*!
	�敪�f�B���N�g�������󂯎���āA�r�p�k���璆�g���m�ۂ���
	@param[in]	hWnd		�e�E�C���h�E�n���h���ENULL�Ȃ�j��
	@param[in]	ptBlockName	�敪�f�B���N�g����
	@return		DWORD		�m�ۂ�����
*/
DWORD AacAssembleSql( HWND hWnd, LPCTSTR ptBlockName )
{
	DWORD	ixItems;

	ixItems = SqlFavCount( ptBlockName, NULL );

	AacMatrixClear(   );	//	�����̓��e�S�j��

	if( 0 == ixItems )	return 0;

	//	�R�[���o�b�N���Ē��g���x�N�^�[�Ɋm��
	SqlFavArtEnum( ptBlockName, AacFavInflate );

	return ixItems;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R�[���o�b�N�ł��C�ɓ��胊�X�g�̒��g���Q�b�c
	@param[in]	dLength	�o�C�g��
	@param[in]	dummy	���g�p
	@param[in]	fake	���g�p
	@param[in]	pcConts	AA�{��
	@return		UINT	�m�ۂ�����
*/
LRESULT CALLBACK AacFavInflate( UINT dLength, UINT dummy, UINT fake, LPCVOID pcConts )
{
	UINT_PTR	iNumber;	//	�ʂ��ԍ�
	AAMATRIX	stAAbuf;	//	���AA�̕ێ��E�x�N�^�[�ɓ����

	iNumber = gvcArts.size( );	//	�����o�Ă���̂ŁA�O�C���f�b�N�X�ɂȂ�

	TRACE( TEXT("NUM[%d] byte[%d]"), iNumber, dLength );

	ZeroMemory( &stAAbuf, sizeof(AAMATRIX) );

	stAAbuf.ixNum = iNumber;

	stAAbuf.cbItem = dLength;	//	�o�C�g���Ȃ̂ł���ł����͂�
	stAAbuf.pcItem = (LPSTR)malloc( stAAbuf.cbItem + 2 );
	ZeroMemory( stAAbuf.pcItem, stAAbuf.cbItem + 2 );

	CopyMemory( stAAbuf.pcItem, pcConts, stAAbuf.cbItem );

	gvcArts.push_back( stAAbuf );	//	�x�N�^�[�ɒǉ�

	return 0;
}
//-------------------------------------------------------------------------------------------------



	
		
