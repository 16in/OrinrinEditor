/*! @file
	@brief �t�@�C�������╶���񌟍��̏��������܂�
	���̃t�@�C���� DocSearchCtrl.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/11/15
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

/*
����������A�t�@�C�����E�Ŕԍ��E�ʒu��S���o���Ă����K�v������
Ctrl+F�Ō������I�[�|��
Ctrl+F3�ŁA�I��͈͂������͈͂ɁE�͈͂Ȃ������疳��
�e�R�ŃW�����v�AShift+F3�ŋt����

������ԂȂ�A������ɕύX���������猟���������B���̍s�����ł������H
F3�p�W�����v�e�[�u�����C���E�ҏW�͂�������A�Y���s�̃f�[�^�S�������āA���̍s�����Č���

�C�e���[�^�Aerase�̕Ԃ�l�́A�폜������̎��̈ʒu


F3�W�����v�̋L�^�A�ňړ�������A���̕łɍ��킹��
�V�K���������炻��ɍ��킹��

�����ʒu�ɕ�����ǉ��폜�����Ƃ��̏����́H
*/

#ifdef FIND_STRINGS


//	�����񌟍��͊Ȉ�Boyer-Moore�@
typedef struct tagFINDPATTERN
{
	TCHAR	cchMozi;
	INT		iDistance;

} FINDPATTERN, *LPFINDPATTERN;
//--------------------------------

//	�q�b�g�ʒu���L�^
typedef struct tagFINDPOSITION
{
	LPARAM	dUnique;	//!<	�t�@�C���ʂ��ԍ��E�P�C���f�b�N�X
	INT		iPage;		//!<	�����Ă��
	INT		iLine;		//!<	�Y���s
	INT		iCaret;		//!<	�s���ł̕����ʒu

} FINDPOSITION, *LPFINDPOSITION;

//-------------------------------------------------------------------------------------------------

extern list<ONEFILE>	gltMultiFiles;	//	�����t�@�C���ێ�
//�C�e���[�^��typedef�̓w�b�_��

extern FILES_ITR	gitFileIt;			//		�����Ă�t�@�C���̖{��

extern INT		gixFocusPage;			//		���ڒ��̃y�[�W�E�Ƃ肠�����O�E�O�C���f�b�N�X

EXTERNED HWND	ghFindDlg;				//!<	�����_�C�����O�̃n���h��

static TCHAR	gatNowPtn[MAX_PATH];	//!<	�ŐV�̌���������
static INT		gixFindMode;			//!<	�������[�h�@�O�ł̂݁@�P�P�t�@�C���@�Q�S�t�@�C��

static FINDPOSITION	gstFindPos;			//!<	�����W�����v�ʒu

//static list<FINDPOSITION>	gltFindPosition;	//!<	�������ʕێ�
//-------------------------------------------------------------------------------------------------


INT_PTR	CALLBACK FindStrDlgProc( HWND, UINT, WPARAM, LPARAM );
HRESULT	FindExecute( HWND );
HRESULT	FindPageSearch( LPTSTR, INT, FILES_ITR );
INT		FindPageHighlightOff( INT, FILES_ITR );
HRESULT	FindPageHitHighlight( INT, INT, INT, FILES_ITR );
HRESULT FindLineHighlightOff( UINT, LINE_ITR );
//-------------------------------------------------------------------------------------------------


/*!
	BM�����e�[�u������
	@param[in]	ptPattern	�����p�^�[��
	@return	LPFINDPATTERN	�쐬���������e�[�u��
*/
LPFINDPATTERN FindTableMake( LPTSTR ptPattern )
{
	UINT		i;
	UINT_PTR	dLength;
	LPFINDPATTERN	pstPtrn;

	//	�e�[�u�����p�^�[���̒����ŏ���������
	StringCchLength( ptPattern, STRSAFE_MAX_CCH, &dLength );
	pstPtrn = (LPFINDPATTERN)malloc( (dLength+1) * sizeof(FINDPATTERN) );
	ZeroMemory( pstPtrn, (dLength+1) * sizeof(FINDPATTERN) );

	for( i = 0; dLength >= i; i++ ){	pstPtrn[i].iDistance =  dLength;	}

	//	�p�^�[���̐擪����A�����ɑΉ�����ʒu�ɖ�������̒�����o�^����
	while( dLength > 0 )
	{
		i = 0;
		while( pstPtrn[i].cchMozi )
		{
			if( pstPtrn[i].cchMozi ==  *ptPattern ){	break;	}
			i++;
		}
		pstPtrn[i].cchMozi   = *ptPattern;
		pstPtrn[i].iDistance = --dLength;

		ptPattern++;
	}

	return pstPtrn;
}
//-------------------------------------------------------------------------------------------------

/*!
	BM��������
	@param[in]	ptText		�����Ώە�����
	@param[in]	ptPattern	�����p�^�[��
	@return		LPTSTR		�q�b�g���������̊J�n�E�Ȃ�������NULL
*/
LPTSTR FindStringProc( LPTSTR ptText, LPTSTR ptPattern, LPINT pdCch )
{
	UINT_PTR	dPtrnLen, dLength;
	LPTSTR	ptTextEnd;
	INT		i, j, k, jump, cch;

	LPFINDPATTERN	pstPattern;

	StringCchLength( ptText, STRSAFE_MAX_CCH, &dLength );

	StringCchLength( ptPattern, STRSAFE_MAX_CCH, &dPtrnLen );
	dPtrnLen--;

	ptTextEnd = ptText + dLength - dPtrnLen;

	pstPattern = FindTableMake( ptPattern );

	cch = 0;
	while( ptText < ptTextEnd )
	{
		for( i = dPtrnLen ; i >= 0 ; i-- )
		{
			if( ptText[i] != ptPattern[i] ){	break;	}
		}

		//	�S�Ĉ�v����
		if( i < 0 ){	FREE( pstPattern  );	*pdCch = cch;	return( ptText );	}

		//	�e�[�u������ړ��ʂ����߂�(�����Ȃ�ړ��ʂ͂Q)
		k = 0;
		while( pstPattern[k].cchMozi )
		{
			if( pstPattern[k].cchMozi == ptText[i] ){	break;	}
			k++;
		}
		j = pstPattern[k].iDistance - ( dPtrnLen - i );
		jump = ( 0 < j ) ? j : 2;
		ptText += jump;
		cch += jump;
	}

	FREE( pstPattern );

	*pdCch = 0;

	return( NULL );
}
//-------------------------------------------------------------------------------------------------

/*!
	�����_�C�����O���J���E���[�_���X��
	@param[in]	hInst	�A�|���P�[�V�����̎���
	@param[in]	hWnd	�E�C���h�E�n���h��
	@retval HRESULT	�I����ԃR�[�h
*/
HRESULT FindDialogueOpen( HINSTANCE hInst, HWND hWnd )
{
	if( ghFindDlg )
	{
		SetForegroundWindow( ghFindDlg );
		return S_OK;
	}

	ghFindDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_FIND_STRING_DLG), hWnd, FindStrDlgProc, 0 );

	ShowWindow( ghFindDlg, SW_SHOW );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK FindStrDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	HWND	hWorkWnd;
	UINT	id;
//	HWND	hWndChild;


	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			//	�R���{�{�b�N�X�ɍ��ړ����
			hWorkWnd = GetDlgItem( hDlg, IDCB_FIND_TARGET );
			ComboBox_InsertString( hWorkWnd, 0, TEXT("���݂̕�") );
			ComboBox_InsertString( hWorkWnd, 1, TEXT("���݂̃t�@�C��") );
	//		ComboBox_InsertString( hWorkWnd, 2, TEXT("�J���Ă���S�Ẵt�@�C��") );�����ł�������
			ComboBox_SetCurSel( hWorkWnd, 0 );
			ComboBox_Enable( hWorkWnd , FALSE );	//	�@�\�o����܂œ���
			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			Edit_SetText( hWorkWnd, gatNowPtn );
			SetFocus( hWorkWnd );
			return (INT_PTR)FALSE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			switch( id )
			{
				case IDCANCEL:	DestroyWindow( hDlg );	return (INT_PTR)TRUE;
				case IDOK:		FindExecute( hDlg );	return (INT_PTR)TRUE;	//	��������

				case IDM_PASTE:	SendMessage( hWorkWnd, WM_PASTE, 0, 0 );	return (INT_PTR)TRUE;
				case IDM_COPY:	SendMessage( hWorkWnd, WM_COPY,  0, 0 );	return (INT_PTR)TRUE;
				case IDM_CUT:	SendMessage( hWorkWnd, WM_CUT,   0, 0 );	return (INT_PTR)TRUE;
				case IDM_UNDO:	SendMessage( hWorkWnd, WM_UNDO,  0, 0 );	return (INT_PTR)TRUE;

				case IDB_FIND_CLEAR:	FindHighlightOff(  );	return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		case WM_CLOSE:
			DestroyWindow( hDlg );
			ghFindDlg = NULL;
			return (INT_PTR)TRUE;

		case WM_DESTROY:
			ViewFocusSet(  );	//	�t�H�[�J�X��`��ɖ߂�
			return (INT_PTR)TRUE;

	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�������s
	@param[in]	hDlg	�_�C�����O�n���h��
*/
HRESULT FindExecute( HWND hDlg )
{
//	HWND	hWorkWnd;
	INT		dRange;	//	�����͈́@�O�Ł@�P�t�@�C���@�Q�S�J���t�@�C��
	BOOLEAN	bModCrlf;
	UINT	d, h;

	TCHAR	atPattern[MAX_PATH], atBuf[MAX_PATH];

	FindHighlightOff(  );	//	��̃p���[���j��
	ZeroMemory( &gstFindPos, sizeof(FINDPOSITION) );

	//	���������s�A���������ɂ��邩
	bModCrlf = IsDlgButtonChecked( hDlg, IDCB_MOD_CRLF_YEN );

	//	�����͈�
	dRange = ComboBox_GetCurSel( GetDlgItem(hDlg,IDCB_FIND_TARGET) );
	//	�O�Ł@�P�t�@�C���@�Q�S�I�[�|���t�@�C��
	gixFindMode = dRange;

	//�����p���[��
	Edit_GetText( GetDlgItem(hDlg,IDE_FIND_TEXT), atBuf, MAX_PATH );
	ZeroMemory( atPattern, sizeof(atPattern) );
	if( bModCrlf )	//	�G�X�P�[�v�V�[�P���X��W�J
	{
		for( d = 0, h = 0; MAX_PATH > d; d++, h++ )
		{
			atPattern[h] = atBuf[d];	
			if( 0x005C == atBuf[d] )	//	0x005C�́�
			{
				d++;
				if( TEXT('n') ==  atBuf[d] )	//	���s�w���ł���ꍇ
				{
					atPattern[h] = TEXT('\r');	h++;
					atPattern[h] = TEXT('\n');
				}
			}
		}
	}
	else
	{
		StringCchCopy( atPattern, MAX_PATH, atBuf );
	}

	//	�Ƃ肠������Search
	FindPageSearch( atPattern, gixFocusPage, gitFileIt );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�w��p���[�����A�w��t�@�C���́A�w��łŌ���
	@param[in]	ptPattern	�����p���[��
	@param[in]	iTgtPage	�Ώە�
	@param[in]	itFile		�t�@�C���C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FindPageSearch( LPTSTR ptPattern, INT iTgtPage, FILES_ITR itFile )
{

	INT			dCch, dLeng;
	INT_PTR		dBytes;
	UINT_PTR	cchSize, cchSzPtn;
	LPTSTR		ptPage, ptCaret, ptFind = NULL;

	TCHAR	ttBuf;

	//	�f�B���C���Ă���΃`�F�b�N���Ȃ��Ă���
	if( PageIsDelayed( itFile, iTgtPage ) ){	return  E_ABORT;	}

	//	���X�g�������[
	StringCchCopy( gatNowPtn, MAX_PATH, ptPattern );

	StringCchLength( ptPattern, MAX_PATH, &cchSzPtn );

	//	�őS�̊m��
	dBytes = DocPageTextGetAlloc( itFile, iTgtPage, D_UNI, (LPVOID *)(&ptPage), FALSE );
	StringCchLength( ptPage, STRSAFE_MAX_CCH, &cchSize );

	ptCaret = ptPage;

	//	��������
	dLeng = 0;
	do
	{
		ptFind = FindStringProc( ptCaret, ptPattern, &dCch );	//	�q�b�g������擪����������
		if( !(ptFind) ){	break;	}

		dLeng += dCch;	//	�����ʒu�E�O�C���f�b�N�X
		ttBuf = ptPage[dLeng];

		//	�q�b�g���������ɐF��t����
		FindPageHitHighlight( dLeng, cchSzPtn, iTgtPage, itFile );

		ptCaret = ptFind;
		ptCaret++;

		dLeng++;

	}while( *ptCaret );

	FREE(ptPage);

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��t�@�C���̎w��ł̎w�蕶���ʒu����w�蕶�������n�C���C�g�w��ɂ���B���s�R�[�h�܂ށB
	@param[in]	iOffset	�Ő擪����̕������E���s�R�[�h�܂ށB
	@param[in]	iRange	�n�C���C�g������
	@param[in]	iPage	�Ώە�
	@param[in]	itFile	�t�@�C���C�e���[�^
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FindPageHitHighlight( INT iOffset, INT iRange, INT iPage, FILES_ITR itFile )
{
	UINT_PTR	ln, iLetters;//, iLines;
	INT_PTR		dMozis;
	INT			iTotal, iDot, iLnTop, iSlide, mz, iNext, iWid = 0;
	RECT		inRect;

	LINE_ITR	itLine, itLnEnd;

	itLine  = itFile->vcCont.at( iPage ).ltPage.begin();
	itLnEnd = itFile->vcCont.at( iPage ).ltPage.end();

	iTotal = 0;
	iLnTop = 0;
	for( ln = 0; itLnEnd != itLine; itLine++, ln++ )
	{
		inRect.top    = ln * LINE_HEIGHT;
		inRect.bottom = inRect.top + LINE_HEIGHT;
		inRect.left   = 0;
		inRect.right  = 0;

		dMozis = itLine->vcLine.size( );	//	���̍s�̕������m�F����
		iLetters = dMozis + 2;	//	���s�R�[�h

		iTotal += iLetters;

		if( iOffset < iTotal )	//	�s���[�܂ł̕��������I�t�Z�b�g��������������A���̍s�Ɋ܂܂��
		{
			iSlide = iOffset - iLnTop;	//	���̍s�擪����̕�����
			//	�������s���猟����������AiSlide = dMozis �ɂȂ�
			iNext = 0;	//	���s���L��ꍇ�̎c�蕶����

			//	�����ŉ��s�̊������ݏ󋵂��m�F���āA���̍s���Ԃ�Ƃ��`�F�b�N�H
			if( dMozis < (iSlide + iRange) )	//	�����Ȃ�A���s���͂ݏo���Ă�
			{
				iNext  = iRange;
				iRange = dMozis - iSlide;	//	�c�蕶����
				iNext -= iRange;	//	���s���܂߂��c�蕶����
			}

			iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
			for( mz = 0; iSlide > mz; mz++ )	//	�Y�������܂Ői�߂ăh�b�g�����߂Ƃ�
			{
				//	�������s���猟���Ȃ炱�ꂪ����
				if( dMozis <=  mz ){	iDot += iWid;	break;	}

				iDot += itLine->vcLine.at( mz ).rdWidth;

				iWid  = itLine->vcLine.at( mz ).rdWidth;	//	���̕����̕�
			}

			for(  ; (iSlide+iRange) > mz; mz++ )
			{
				//	���s�܂ł߂荞�ނȂ炱��������
				if( dMozis <=  mz ){	break;	}

				iWid = itLine->vcLine.at( mz ).rdWidth;

				inRect.left  = iDot;
				inRect.right = iDot + iWid;

				itLine->vcLine.at( mz ).mzStyle |= CT_FINDED;
				ViewRedrawSetRect( &inRect );

				iDot += iWid;
			}

			if( 0 <	 iNext )	//	���s������Ȃ琔�l������
			{
				inRect.right += 20;

				itLine->dStyle |= CT_FINDRTN;
				ViewRedrawSetRect( &inRect );

				iNext -=  2;	//	���s�������ւ炵��

			}

			//	�K�v�ɉ����āA�����ŏI������莟�̍s�̏����ɂ�������
			if( 0 >= iNext ){	break;	}
			else
			{
				iOffset += (2 + iRange);	//	���s�������{���̍s�Ŏg�����������i�߂�
				iRange  = iNext;	//	�c�蕶��������
			}
		}

		iLnTop += iLetters;
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------


/*!
	�����ʒu�փW�����v
	@param[in]	dMode	�O���ց@�P�O��
	@param[in]	pxDot	�J�[�\���h�b�g�ʒu�E����������߂�
	@param[in]	pyLine	�J�[�\���s�E����������߂�
	@param[in]	pMozi	�L�����b�g�̍����̕������E����������߂�
	@return		INT		���F�G���[�@�O�F�q�b�g�����@�P�`�F�q�b�g����
*/
INT FindStringJump( UINT dMode, PINT pXdot, PINT pYline, PINT pMozi )
{
	INT			iXdot, iYline, iMozi;
	BOOLEAN		bStart, bBegin;
	INT_PTR		dTotalPage, dTotalLine;
	PAGE_ITR	itPage;	//	�ł����Ɍ��Ă���
	LINE_ITR	itLine;	//	�s�����Ɍ��Ă���
	LETR_ITR	itMozi;	//	���������Ɍ��Ă���

	if( !(pXdot) || !(pYline) || !(pMozi) ){	return -1;	}

	//	�ł͂ݏo���`�F�L
	dTotalPage = gitFileIt->vcCont.size();
	if( !(0 <= gixFocusPage && gixFocusPage < dTotalPage) ){	return -1;	}
	//	���̕łɒ���
	itPage = gitFileIt->vcCont.begin();
	std::advance( itPage, gixFocusPage );

	iXdot  = *pXdot;
	iYline = *pYline;
	iMozi  = *pMozi;


	//	�s�͂ݏo���`�F�L
	dTotalLine = itPage->ltPage.size();
	if( !(0 <= iYline && iYline < dTotalLine) ){	return -1;	}
	//	���̍s�ɒ���
	itLine = itPage->ltPage.begin();
	std::advance( itLine, iYline );

	itMozi = itLine->vcLine.begin();
	std::advance( itMozi, iMozi );

	bStart = TRUE;	bBegin = TRUE;
	for( ; itPage->ltPage.end() != itLine; itLine++ )
	{
		//	�ŏ��͂��̂܂܁A���񂩂�͂��̍s�̐擪�������猩�Ă���
		if( bStart ){	bStart =  FALSE;	}
		else{	itMozi = itLine->vcLine.begin();	}

		for( ;itLine->vcLine.end() != itMozi; itMozi++ )
		{
			if( itMozi->mzStyle & CT_FINDED )	//	�q�b�g
			{
				if( !(bBegin)  )	//	�����J�n�ʒu�͖̂�������
				{
					*pXdot  = iXdot;
					*pYline = iYline;
					*pMozi  = iMozi;

					ViewDrawCaret( iXdot, iYline, TRUE );	//	�L�����b�g���ړ�

					return 1;
					//	�ł܂����͂ǂ������
				}
			}
			else{	bBegin =  FALSE;	}
			//	

			iXdot += itMozi->rdWidth;
			iMozi++;
		}

		iYline++;	//	�s�ԍ��͑��₷
		iXdot = 0;
		iMozi = 0;
	}

	//	�ł܂����͂ǂ������

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�S���n�C���C�g�n�e�e
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT FindHighlightOff( VOID )
{
	UINT_PTR	dPage, d;

	dPage = gitFileIt->vcCont.size( );

	for( d = 0; dPage > d; d++ )
	{
		FindPageHighlightOff( d, gitFileIt );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��t�@�C���̎w��ł̌����n�C���C�g��OFF����
	@param[in]	iPage	�Ώە�
	@param[in]	itFile	�t�@�C���C�e���[�^
	@return		���ɂȂ�
*/
INT FindPageHighlightOff( INT iPage, FILES_ITR itFile )
{
	UINT_PTR	ln;//, iLines, iLetters, mz;
//	UINT		dStyle;
//	INT			iDot, iWid;
//	RECT		inRect;

	LINE_ITR	itLine, itLnEnd;


	if( 0 >  iPage )	return 0;	//	����ȏ󋵉��ł͏������Ȃ�

	ZeroMemory( gatNowPtn, sizeof(gatNowPtn) );

	itLine  = itFile->vcCont.at( iPage ).ltPage.begin();
	itLnEnd = itFile->vcCont.at( iPage ).ltPage.end();

	for( ln = 0; itLnEnd != itLine; itLine++, ln++ )
	{
		FindLineHighlightOff( ln, itLine );	//	����REDRAW�������Ă�
	}

//	ViewRedrawSetLine( -1 );	//	��ʕ\���X�V

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��s�̌����n�C���C�g��OFF����
	@param[in]	iLine	�s���E�O�C���f�b�N�X
	@param[in]	itFile	�s�C�e���[�^
	@return		���ɂȂ�
*/
HRESULT FindLineHighlightOff( UINT iLine, LINE_ITR itLine )
{
	UINT_PTR	iLetters, mz;//, iLines, ln;
	UINT		dStyle;
	INT			iDot, iWid;
	RECT		inRect;


	iDot = 0;	//	�����܂ł̃h�b�g�������ߍ���
	inRect.top    = iLine * LINE_HEIGHT;
	inRect.bottom = inRect.top + LINE_HEIGHT;

	iLetters = itLine->vcLine.size();
	//	�땶�����A�S�����`�F�L���Ă���
	for( mz = 0; iLetters > mz; mz++ )
	{
		//	���O�̏��
		dStyle = itLine->vcLine.at( mz ).mzStyle;
		iWid   = itLine->vcLine.at( mz ).rdWidth;

		inRect.left  = iDot;
		inRect.right = iDot + iWid;

		itLine->vcLine.at( mz ).mzStyle &= ~CT_FINDED;
		if( dStyle & CT_FINDED )	ViewRedrawSetRect( &inRect );

		iDot += iWid;
	}

	//	��s�I������疖���󋵊m�F�B���s�E�{�����[�ɉ��s�͂Ȃ��E�I���̂Ƃ��̂�
	dStyle = itLine->dStyle;
	inRect.left  = iDot;
	inRect.right = iDot + 20;	//	���s�`��G���A�E��̂��ꂭ�炢

	itLine->dStyle &=  ~CT_FINDRTN;
	if( dStyle & CT_FINDRTN )	ViewRedrawSetRect( &inRect );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���ł��Č���
*/
HRESULT FindNowPageReSearch( VOID )
{
	FindHighlightOff(  );

	FindPageSearch( gatNowPtn, gixFocusPage, gitFileIt );	//	�d��

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#endif
