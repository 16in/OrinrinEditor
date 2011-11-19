// stdafx.cpp : �W���C���N���[�h OrinrinEditor.pch �݂̂�
// �܂ރ\�[�X �t�@�C���́A�v���R���p�C���ς݃w�b�_�[�ɂȂ�܂��B
// stdafx.obj �ɂ̓v���R���p�C���ς݌^��񂪊܂܂�܂��B

#include "stdafx.h"
#include "OrinrinEditor.h"

#include "Entity.h"
//------------------------------------------------------------------------------------------------------------------------

#ifndef _ORRVW
extern  UINT	gbUniRadixHex;	//	���j�R�[�h���l�Q�Ƃ��P�U�i���ł��邩
#endif
//------------------------------------------------------------------------------------------------------------------------

//	�m�F�`�F�b�N���b�Z�[�W�_�C�����O�p
typedef struct tagMSGBOXMSG
{
	TCHAR	atMsg1[MAX_PATH];	//!<	������P
	TCHAR	atMsg2[MAX_PATH];	//!<	������Q

	UINT	bChecked;	//!<	��O�`�F�L��ꂽ�@�O�m�[�`�F�b�N

} MSGBOXMSG, *LPMSGBOXMSG;

//------------------------------------------------------------------------------------------------------------------------


/*!
	�E�C���h�E�Ƀ��[�U�f�[�^����������
	@param[in]	hWnd	�������݂����E�C���h�E�̃n���h��
	@param[in]	tag		�������ސ��l�BLONG�l
	@return	����
*/
VOID WndTagSet( HWND hWnd, LONG_PTR tag )
{
	SetWindowLongPtr( hWnd, GWLP_USERDATA, tag );
}
//------------------------------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�̃��[�U�f�[�^��ǂݍ���
	@param[in]	hWnd	�ǂݍ��ރE�C���h�E�̃n���h��
	@return	���[�U�f�[�^�l�B�����l��0
*/
LONG_PTR WndTagGet( HWND hWnd )
{
	return GetWindowLongPtr( hWnd, GWLP_USERDATA );
}
//-------------------------------------------------------------------------------------------------

/*!
	@param[in]	adMozi	�`�F�L�肽�����j�R�[�h����
	@param[in]	adMozi	�`�F�L�肽�����j�R�[�h����
	@return		BOOLEAN	��O�Y���g���q�ł���@�O�Ⴄ���A�g���q������
*/
BOOLEAN FileExtensionCheck( LPTSTR ptFile, LPTSTR ptExte )
{
	TCHAR	atExBuf[10];
	LPTSTR	ptExten;

	//	�g���q���m�F�E�h�b�g���݂���`��
	ptExten = PathFindExtension( ptFile );	//	�g���q�������Ȃ�NULL�A�Ƃ��������[�ɂȂ�
	if( 0 == *ptExten ){	return 0;	}
	//	�g���q�w�肪�Ȃ��Ȃ�Ȃɂ��ł��Ȃ�

	StringCchCopy( atExBuf, 10, ptExten );
	CharLower( atExBuf );	//	��r�̂��߂ɏ������ɂ����Ⴄ

	//	���e�Ⴄ�Ȃ�^�l�ƂȂ�
	if( StrCmp( atExBuf, ptExte ) ){	return 0;	}

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�v���r���p�ɁAHTML�I�Ƀ��o�C�������G���e�B�e�B����
	@param[in]	adMozi	�`�F�L�肽�����j�R�[�h����
	@param[out]	pcStr	�G���e�B�e�B�����ꍇ�́A���̕����������
	@param[in]	cbSize	������o�b�t�@�̕������i�o�C�g���j
	@return		BOOLEAN	��O�G���e�B�e�B�����@�O��薳��
*/
BOOLEAN HtmlEntityCheck( TCHAR adMozi, LPSTR pcStr, UINT_PTR cbSize )
{
	INT	i;

	ZeroMemory( pcStr, cbSize );

	for( i = 0; 4 > i; i++ )
	{
		if( 0 == gstEttySP[i].dUniCode )	break;

		if( gstEttySP[i].dUniCode == adMozi )
		{
			StringCchCopyA( pcStr, cbSize, gstEttySP[i].acCodeA );
			return TRUE;
		}
	}

	return FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	SJIS����&#0000;���l�����ă��j�R�[�h�ɕϊ�
	&#����n�܂镔�������̎Q�Ƃ��ǂ����`�F�b�N�E�P�O�����݂�A�k���^�[�~�l�[�^�܂�
	@param[in]	pcStr	�m�F���镶����̐擪
	@return		TCHAR	���j�R�[�h�����i���l�̕������̂܂܁j
*/
TCHAR UniRefCheck( LPSTR pcStr )
{
	CHAR	acValue[10];
	PCHAR	pcEnd;
	UINT	i, code;
	INT		radix = 10;
	BOOLEAN	bXcode = FALSE;

	ZeroMemory( acValue, sizeof(acValue) );
	if( NULL == pcStr[2] )	return 0x0000;	//	���[�������ꍇ

	pcStr += 2;	//	&#������i�߂�

	//	&#xhhhh;�ȂP�U�i�����m�F
	if( 'x' == pcStr[0] || 'X' == pcStr[0] )
	{
		bXcode = TRUE;
		pcStr++;	//	�����炪�{��
		radix = 16;
	}

	for( i = 0; 10 > i; i++ )
	{
		if( NULL == pcStr[i] )	return 0x0000;	//	���[�������ꍇ

		if( ';' == pcStr[i] )	break;	//	���[�m�F

		if( bXcode )
		{
			if( isxdigit( pcStr[i] ) ){	acValue[i] = pcStr[i];	}
			else	return 0x0000;	//	�オ���l����Ȃ������ꍇ
		}
		else
		{
			if( isdigit( pcStr[i] ) ){	acValue[i] = pcStr[i];	}
			else	return 0x0000;	//	�オ���l����Ȃ������ꍇ
		}
	}
	if( 10 <= i ){	return 0x0000;	}	//	�Ȃ񂩐U��؂��Ă�

	code = strtoul( acValue, &pcEnd, radix );	//	�i���ɍ��킹�ĕϊ������Ⴄ
	
	if( 0xFFFF < code ){	code = 0x0000;	}	//	���炩�ɂ��������ꍇ

	return (TCHAR)code;
}
//-------------------------------------------------------------------------------------------------

/*!
	SJIS��������󂯎���āA���̎Q�Ƃ����j�R�[�h���l�Q�Ƃɕϊ�����
	@param[in]	pcMoto	���̕�����
	@return	LPSTR	�ϊ�����������E�󂯎�����ق���FREE����E��������������NULL
*/
LPSTR SjisEntityExchange( LPCSTR pcMoto )
{
	LPSTR		pcOutput, pcTemp;
	CHAR		acSrp[12], acChk[3], acUni[10];
	UINT		check, el;
	UINT_PTR	szMoto, szStr, sc, dt, se, rp, cbSz;
	BOOLEAN		bStr = FALSE, bHit = FALSE;

	CONST ENTITYLIST	*pstEty;

	szMoto = strlen( pcMoto );
	szStr = szMoto + 2;
	pcOutput = (LPSTR)malloc( szStr );	//	�k���^�[�~�l�[�^�p
	if( !(pcOutput) )	return NULL;
	ZeroMemory( pcOutput, szStr );	


	for( sc = 0, dt = 0; szMoto > sc; sc++ )
	{
		if( '&' == pcMoto[sc] )
		{
			se = sc;	//	�g�p����
			bStr = FALSE;

			for( rp = 0; 10 > rp; rp++ )
			{
				acSrp[rp] = pcMoto[se++];	acSrp[rp+1] = 0x00;
				if( ';'  == acSrp[rp] ){	bStr = TRUE;	break;	}

				if( 1 == rp )
				{
					if( !( isalpha( acSrp[1] ) ) )	break;
				}
			}
			//	���̎��_�ŁAbStr��FALSE�Ȃ�A&xxxx; �ȓ��e����Ȃ�����
			if( !(bStr) ){	pcOutput[dt++] = pcMoto[sc];	continue;	}

			//	���̎��̎����m�F���āA�����͈͂��i�荞��
			acChk[0] = acSrp[1];	acChk[1] = 0x00;
			CharLowerA( acChk );
			check = acChk[0] - 'a';	//	�ʒu���m�F

			pstEty = gpstEntitys[check];

			bHit = FALSE;
			//	�����肪���邩�ǂ������Ă���
			for( el = 0; 0 != pstEty[el].dUniCode; el++ )
			{
				if( 0 == StrCmpA( acSrp, pstEty[el].acCodeA ) )	//	�q�b�g
				{
					szStr += 4;	//	�͂ݏo�����獢��̂ŏ������₷
					pcTemp = (LPSTR)realloc( pcOutput, szStr );
					if( pcTemp )	pcOutput = pcTemp;

					bHit = TRUE;
					StringCchPrintfA( acUni, 10, ("&#%d;"), pstEty[el].dUniCode );
					StringCchLengthA( acUni, 10, &cbSz );
					for( rp = 0; cbSz > rp; rp++ )
					{
						pcOutput[dt++] = acUni[rp];
					}
					sc = se - 1;	//	�ʒu���������߂��E���[�v�Ł{�{����邩������Ă���
					break;
				}
			}
			//	���̎��_�ŁAbHit��FALSE�Ȃ�A���̂̂�����e����Ȃ�����
			if( !(bHit) ){	pcOutput[dt++] = pcMoto[sc];	}
		}
		else	//	�G���e�B�e�B����Ȃ��Ȃ炻�̂܂܈ڂ��΂���
		{
			pcOutput[dt++] = pcMoto[sc];
		}
	}

	return pcOutput;
}
//-------------------------------------------------------------------------------------------------

/*!
	�V�t�gJIS��������󂯎���āA���j�R�[�h������ɕϊ����ĕԂ�l�E�Ă񂾂ق���free����̖Y����
	@param[in]	pcBuff	�V�t�gJIS������o�b�t�@�̃|�C���^�[
	@return		�m�ۂ������j�R�[�h������o�b�t�@�̃|�C���^�[�Efree�Z��
*/
LPTSTR SjisDecodeAlloc( LPSTR pcBuff )
{
	DWORD	cbWrtSize;
	LPSTR	pcPos, pcChk, pcPosEx;

	DWORD	cchSize, cchWrtSize;
	LPTSTR	ptBuffer, ptWrtpo;
	TCHAR	chMozi;

	DWORD	dStart, dEnd;


	cchSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcBuff, -1, NULL, 0 );

	//	�o�͗p���j�R�[�h�o�b�t�@
	ptBuffer = (LPTSTR)malloc( (cchSize+1) * 2 );
	ZeroMemory( ptBuffer, (cchSize+1) * 2 );

	dStart = 0;
	dEnd   = 0;

//	pcPos = pcBuff;
	ptWrtpo = ptBuffer;

	//�܂�SJIS�̂܂܂ŁA���̎Q�Ƃ����j�R�[�h�Q�Ƃɕϊ�����
	pcPosEx = SjisEntityExchange( pcBuff );

	pcPos = pcPosEx;

	pcChk = StrStrA( pcPosEx, "&#" );	//	�L�[���[�h�Ƀq�b�g���邩�ǂ���
	while( pcChk )
	{
		//	���̉ӏ��̎�O�܂ŕϊ�
		cbWrtSize  = pcChk - pcPos;	//	�����܂ł̃T�C�Y���߂�
		cchWrtSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcPos, cbWrtSize, ptWrtpo, cchSize );

		ptWrtpo += cchWrtSize;	//	�����ꏊ�ړ�
		cchSize -= cchWrtSize;	//	�c�蕶����

		chMozi = UniRefCheck( pcChk );	//	�L���ȓ��e���ǂ����m�F
		if( 0 != chMozi )	//	�L���ȃi�j�J��������
		{
			*ptWrtpo = chMozi;	//	�������Ԃ������
			ptWrtpo++;			//	���ɐi�߂�
			cchSize--;			//	�c�蕶�����͌��炷
			pcChk = StrStrA( pcChk, ";" );	//	�I�[�܂Ői�߂Ă���
			pcChk++;	//	';'�܂Ŋ܂ނ̂ŁA����ɂ��̎����炪�{��
		}
		else	//	�֌W�Ȃ��Ȃ�A�q�b�g�����Q�������J�L�R���āA���ɐi��
		{
			cchWrtSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcChk, 2, ptWrtpo, cchSize );

			ptWrtpo += cchWrtSize;	//	�����ꏊ�ړ�
			cchSize -= cchWrtSize;	//	�c�蕶����
			pcChk += 2;	//	�g�������i�߂�
		}
		pcPos = pcChk;	//	���͂������璲�ׂĂ���

		pcChk = StrStrA( pcPos , "&#" );	//	�L�[���[�h�Ƀq�b�g���邩�ǂ���
	}
	//	�c��̕����A�������͍ŏ�����Ō�܂ł�ϊ�
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcPos, -1, ptWrtpo, cchSize );

	FREE( pcPosEx );

	return ptBuffer;
}
//-------------------------------------------------------------------------------------------------

#ifndef _ORRVW

/*!
	���j�R�[�h��������󂯎���āASJIS������ɕϊ��E�Ԃ�l�͌Ă񂾕���free���ɂ�
	@param[in]	ptTexts	���j�R�[�h������o�b�t�@�̃|�C���^�[
	@return		�m�ۂ���SJIS������o�b�t�@�̃|�C���^�[�Efree�Z��
*/
LPSTR SjisEncodeAlloc( LPTSTR ptTexts )
{
	TCHAR	atMozi[2];
	CHAR	acSjis[10];
	BOOL	bCant = FALSE;
	INT		iRslt;
	UINT_PTR	cchSize, d, cbSize;
	LPSTR	pcString;

	string	sString;

	//	�����m�F
	StringCchLength( ptTexts, STRSAFE_MAX_CCH, &cchSize );

	sString.clear( );

	atMozi[1] = 0;
	for( d = 0; cchSize > d; d++ )
	{
		atMozi[0] = ptTexts[d];
		ZeroMemory( acSjis, sizeof(acSjis) );

		iRslt = WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, atMozi, 1, acSjis, 10, "?", &bCant );
		//	�ϊ��o����΂���ł����A�_���Ȃ琔�l�Q�ƌ`���ɂ����Ⴄ
		if( bCant )
		{
			if( gbUniRadixHex ){	StringCchPrintfA( acSjis, 10 , ("&#x%X;"), ptTexts[d] );	}
			else{					StringCchPrintfA( acSjis, 10 , ("&#%d;"),  ptTexts[d] );	}
		}

		sString += string( acSjis );
	}

	cbSize = sString.size( ) + 2;
	pcString = (LPSTR)malloc( cbSize );
	ZeroMemory( pcString, cbSize );
	StringCchCopyA( pcString, cbSize, sString.c_str() );

	return pcString;
}
//-------------------------------------------------------------------------------------------------
#endif


/*!
	������\�����Ȃ��`�F�L�{�b�N�X�t���_�C�����O�{�b�N�X�v���V�[�W��
	@param[in]	hDlg	�_�C�����O�n���h��
	@param[in]	message	�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam	�ǉ��̏��P
	@param[in]	lParam	�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK MsgCheckBoxProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static LPMSGBOXMSG	pcstMsg = NULL;
	INT	id;

	switch( message )
	{
		default:	break;

		case WM_INITDIALOG:
			pcstMsg = (LPMSGBOXMSG)lParam;
			SetDlgItemText( hDlg, IDS_MC_MSG1, pcstMsg->atMsg1 );
			SetDlgItemText( hDlg, IDS_MC_MSG2, pcstMsg->atMsg2 );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			if( pcstMsg ){	pcstMsg->bChecked = IsDlgButtonChecked( hDlg, IDCB_MC_CHECKBOX );	}
			if( IDYES == id || IDOK == id ){	EndDialog( hDlg, IDYES );	}
			if( IDNO == id || IDCANCEL == id ){	EndDialog( hDlg, IDNO );	}
			return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�`�F�b�N�{�b�N�X�t�����b�Z�[�W�{�b�N�X���ǂ�
	@param[in]	hWnd	�E�C���h�E�n���h��
	@param[in]	hInst	�A�v���̎���
	@param[in]	dStyle	�\��Message�@�O�œ����@�P�ŕ����@�Q�ō폜
*/
INT_PTR MessageBoxCheckBox( HWND hWnd, HINSTANCE hInst, UINT dStyle )
{
	INT_PTR	iRslt;
	UINT	number;
	MSGBOXMSG	stMsg;


	switch( dStyle )
	{
		case  0:	//	�����m�F
			StringCchCopy( stMsg.atMsg1, MAX_PATH, TEXT("��������������畜�A�ł��Ȃ��̂ł���E�E�E") );
			StringCchCopy( stMsg.atMsg2, MAX_PATH, TEXT("�{���ɂ������Ă����̂ł����H") );
			number = VL_PCOMBINE_NM;
			break;

		case  1:	//	�����m�F
			StringCchCopy( stMsg.atMsg1, MAX_PATH, TEXT("��������������畜�A�ł��Ȃ��̂ł���E�E�E") );
			StringCchCopy( stMsg.atMsg2, MAX_PATH, TEXT("�{���Ƀo�����Ă����̂ł����H") );
			number = VL_PDIVIDE_NM;
			break;

		case  2:	//	�폜�m�F
			StringCchCopy( stMsg.atMsg1, MAX_PATH, TEXT("�폜����������畜�A�ł��Ȃ��̂ł���E�E�E") );
			StringCchCopy( stMsg.atMsg2, MAX_PATH, TEXT("�{���ɂ��ځ[�񂵂Ă����̂ł����H") );
			number = VL_PDELETE_NM;
			break;

		default:	return IDCANCEL;
	}

	stMsg.bChecked = InitParamValue( INIT_LOAD, number, 0 );
	if( 1 == stMsg.bChecked ){	iRslt = IDYES;	}
	else
	{
		iRslt = DialogBoxParam( hInst, MAKEINTRESOURCE(IDD_MSGCHECKBOX_DLG), hWnd, MsgCheckBoxProc, (LPARAM)(&stMsg) );
		InitParamValue( INIT_SAVE, number, (BST_CHECKED==stMsg.bChecked) ? 1 : 0 );
	}
	return iRslt;
}
//-------------------------------------------------------------------------------------------------


