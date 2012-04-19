/*! @file
	@brief ���̑��F�X�ȏ���������
	���̃t�@�C���� DocMiscellaneous.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/17
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

extern list<ONEFILE>	gltMultiFiles;	//	�����t�@�C���ێ�

extern FILES_ITR	gitFileIt;		//	�����Ă�t�@�C���̖{��

extern INT			gixFocusPage;	//	���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

extern  UINT		gbCrLfCode;		//	���s�R�[�h�F�O������΁E��O�x�x 
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

/*!
	���v���
	@param[in]	iPage	�`�F�b�N�������Ŕԍ�
	@param[in]	itFile	�t�@�C���C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT DocStatisticsPage( INT iPage, FILES_ITR itFile )
{

	//	�g�p�����ꗗ�Ƃ��ESQL�ŕ����f�[�^�x�[�X���΂���



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

	iSize = (*gitFileIt).vcCont.size( );
	if( 0 >= iSize || iPage >= iSize )	return TRUE;

	iSize = (*gitFileIt).vcCont.at( iPage ).ltPage.size( );
	if( 0 >= iSize || iLine >= iSize )	return TRUE;

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�󔒌x�������
	@param[in]	rdLine	�Ώۂ̍s�ԍ��E��΂O�C���f�b�N�X��
	@return	BOOLEAN		��O�x������@�O����
*/
BOOLEAN DocBadSpaceIsExist( INT rdLine )
{
	LINE_ITR	itLine;

	//	��Ԋm�F
	if( DocRangeIsError( gixFocusPage, rdLine ) ){	return 0;	}
	//	�����͈̔͊O�����͕K�R�Ȃ̂œ��Ɍx���͕s�v

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	return itLine->bBadSpace;
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
	BOOLEAN		bWarn;
	TCHAR		ch, chn;
	LETR_ITR	ltrItr, ltrEnd, ltrNext;
	LINE_ITR	itLine;


//���[�󔒂�DocLineDataGetAlloc�ł����Ă�

	//	��Ԋm�F
	if( DocRangeIsError( gixFocusPage, rdLine ) )
	{
		TRACE( TEXT("�͈͊O�G���[���� PAGE[%d], LINE[%d]"), gixFocusPage, rdLine );
		return 0;
	}

	//	�������m�F
	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	iCount = itLine->vcLine.size( );
	if( 0 == iCount )	//	�O�Ȃ牽���m�F���邱�Ƃ��Ȃ��̂ŁA�G���[�����ďI���
	{
		//	�G���[�c���Ă���
		if( itLine->bBadSpace )
		{
			itLine->bBadSpace = 0;
			ViewRedrawSetLine( rdLine );	//	���[�����ƍs�ĕ`��
		}
		return 0;
	}

	iRslt = 0;

	bWarn = FALSE;
	ltrEnd = itLine->vcLine.end( );

	for( ltrItr = itLine->vcLine.begin(); ltrEnd != ltrItr; ltrItr++ )
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
	ch = itLine->vcLine.at( 0 ).cchMozi;

	if( 0xFF >= ch )	//	���p��������
	{
		if( isspace( ch ) )
		{
			itLine->vcLine.at( 0 ).mzStyle |= CT_WARNING;
			iRslt = 1;
		}
	}

#pragma message ("DocBadSpaceCheck���̍X�V�w�߂̍œK�����K�v")

	//	�O��܂łƏ󋵂�����Ă�����
	if( iRslt != itLine->bBadSpace )
	{
		ViewRedrawSetLine( rdLine );	//	���[�����ƍs�ĕ`��
	}

	itLine->bBadSpace = iRslt;

	return iRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	���݂̃t�@�C���̕Ő���Ԃ�
	@return	INT_PTR	�Ő�
*/
INT_PTR DocPageCount( VOID )
{
	return (*gitFileIt).vcCont.size( );
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

	LINE_ITR	itLine;

	iLines = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.size( );

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	for( i = 0; iLines > i; i++, itLine++ )
	{
		//	���s�̃o�C�g���E2ch�AYY��6byte�E������΁�4byte
		if( 1 <= i )	//	��s�ڂ�����s���ǉ�
		{
			if( gbCrLfCode )	dBytes += YY2_CRLF;
			else				dBytes += STRB_CRLF;
		}

		dMozis += itLine->vcLine.size( );
		dBytes += itLine->iByteSz;
	}

	if( pdMozi )	*pdMozi = dMozis;
	if( pdByte )	*pdByte = dBytes;

	(*gitFileIt).vcCont.at( gixFocusPage ).dByteSz = dBytes;

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
	LETR_ITR	itMozi, endMozi;

	if( 0 > dPage ){	dPage = gixFocusPage;	}

//#error �s���Ƃ��̏����܂Ƃ߂ă��j���[�����ق�������

	iBytes = 0;
	iMozis = 0;

	itLine  = (*gitFileIt).vcCont.at( dPage ).ltPage.begin();
	endLine = (*gitFileIt).vcCont.at( dPage ).ltPage.end();

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

	(*gitFileIt).vcCont.at( dPage ).dByteSz = iBytes;

	if( pMozi ){	*pMozi = iMozis;	}

	return iBytes;
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

	LINE_ITR	itLine;

	iLines = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.size( );

	if( 0 > dTop )		dTop = 0;
	if( 0 > dBottom )	dBottom = iLines - 1;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, dTop );

	for( i = dTop; dBottom >= i; i++, itLine++ )
	{
		thisDot = itLine->iDotCnt;
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
	@return	INT		�h�b�g���E���F�ُ픭��
*/
INT DocLineParamGet( INT rdLine, PINT pdMozi, PINT pdByte )
{
	INT_PTR	iCount, i, iLines;
	INT		dDotCnt, dByteCnt;

#ifdef DO_TRY_CATCH
	try{
#endif

	LINE_ITR	itLine;

	iLines = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.size( );
	if( iLines <= rdLine )	return -1;

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	iCount = itLine->vcLine.size( );

	//	�������K�v�Ȃ�
	if( pdMozi )	*pdMozi = iCount;

	dDotCnt = 0;
	dByteCnt = 0;
	for( i = 0; iCount > i; i++ )
	{
		dDotCnt  += itLine->vcLine.at( i ).rdWidth;
		dByteCnt += itLine->vcLine.at( i ).mzByte;	//	���o�C�g��
	}

	if( pdByte )	*pdByte = dByteCnt;

	//	�����̃T�C�Y����Ƃ�
	itLine->iDotCnt = dDotCnt;
	itLine->iByteSz = dByteCnt;

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), -1 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), -1 );	}
#endif

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
	INT	i, iCount, iLines;	//	INT_PTR
	INT	iLetter;	//	�L�����b�g�̍����̕�����
	INT	iMaxLine;
	INT	dDotCnt = 0, dPrvCnt = 0, rdWidth = 0;

	LINE_ITR	itLine;

#ifdef DO_TRY_CATCH
	try{
#endif

	//	�����͈͊O�Ȃ�A�͈͓��ɂ���Ă���
	iMaxLine = DocPageParamGet( NULL, NULL );
	if( iMaxLine <= rdLine )	rdLine = iMaxLine - 1;



	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	assert( pNowDot );

	//	�s�̂͂ݏo���ƕ������m�F
	iLines = DocLineParamGet( rdLine, &iCount, NULL );
	if( 0 > iLines )	return 0;

	for( i = 0, iLetter = 0; iCount > i; i++, iLetter++ )
	{
		if( dDotCnt >= *pNowDot ){	break;	}

		dPrvCnt = dDotCnt;
		rdWidth = itLine->vcLine.at( i ).rdWidth;

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

#ifdef DO_TRY_CATCH
	}
	catch( exception &err ){	return ETC_MSG( err.what(), 0 );	}
	catch( ... ){	return  ETC_MSG( ("etc error"), 0 );	}
#endif

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

	LINE_ITR	itLine;

	iLines = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.size( );
	if( iLines <=  rdLine ){	return -1;	}

	if( 0 == bDirect )
	{
		if( pdAbsDot ){	*pdAbsDot = nowDot;	}
		if( pbJump ){	*pbJump = FALSE;	}
		return nowDot;	//	�ړ����������ł͈Ӗ�������
	}

	itLine = (*gitFileIt).vcCont.at( gixFocusPage ).ltPage.begin();
	std::advance( itLine, rdLine );

	iCount = itLine->vcLine.size( );	//	���̍s�̕������m�F����

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
		dLtrDot = itLine->vcLine.at( iLetter-1 ).rdWidth;
		//	���O�̕����̕����m�F
		nowDot -= dLtrDot;
	}//	dDotCnt

	if( 0 < bDirect )	//	�E(����)�ֈړ�
	{
		dLtrDot = itLine->vcLine.at( iLetter ).rdWidth;
		//	����̕����̕����m�F
		nowDot += dLtrDot;
	}//	dDotCnt

	if( pdAbsDot )	*pdAbsDot = nowDot;//dDotCnt

	return dLtrDot;	//	�h�b�g���߂��Ăn�j
}
//-------------------------------------------------------------------------------------------------

/*!
	������̍s���ƁA��Ԓ����h�b�g�ƕ������𐔂���E���j�R�[�h�p
	@param[in]	ptStr	��������������
	@param[in]	cchSize	�������E�O�Ȃ炱�����Œ��ׂ�
	@param[out]	pMaxDot	�ő�h�b�g��
	@param[out]	pMaxLtr	�ő啶����
	@return		�s��
*/
INT DocStringInfoCount( LPCTSTR ptStr, UINT_PTR cchSize, PINT pMaxDot, PINT pMaxLtr )
{
	INT	iDot, iLine, iMax, iMozi, e;
	UINT_PTR	d;
	wstring	wsBuffer;

	//	�������w�肪�Ȃ��Ȃ炱���Œ��ׂĂ���
	if(  0 == cchSize ){	StringCchLength( ptStr, STRSAFE_MAX_CCH, &cchSize );	}

	wsBuffer.clear();
	iDot = 0;	iLine = 0;	iMax = 0;	iMozi = 0;

	for( d = 0, e = 0; cchSize >= d; d++ )	//	�I�[�������K�v
	{
		if( cchSize <= d || 0x000D == ptStr[d] )
		{
			iLine++;	//	��s
			iDot = ViewStringWidthGet( wsBuffer.c_str() );
			if( iMax  < iDot ){	iMax  = iDot;	}	//	�h�b�g��
			if( iMozi < e ){	iMozi = e;		}	//	������

			if( cchSize <= d )	break;	//	�I���Ȃ炱���܂�

			d++;	//	���̍s�ׂ̈�

			e = 0;
			wsBuffer.clear();
		}
		else	//	���������s���m�F���Ă���
		{
			wsBuffer.push_back( ptStr[d] );
		}
	}

	if( pMaxDot ){	*pMaxDot = iMax;	}
	if( pMaxLtr ){	*pMaxLtr = iMozi;	}

	return iLine;
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

