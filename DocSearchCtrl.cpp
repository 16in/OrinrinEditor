/*! @file
	@brief ファイル検索や文字列検索の処理をします
	このファイルは DocSearchCtrl.cpp です。
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
Ctrl+Fで検索窓オーポン

Ctrl＋Ｆ３で、選択範囲を検索範囲に・範囲なかったら無視
Ｆ３で次の検索文字列のある頁へジャンプ
Ｆ５でリフレッシュ＜簡易対応


検索したら、ファイル名・頁番号・位置を全部覚えておく必要がある＜不要？

イテレータ、eraseの返り値は、削除したやつの次の位置

F3ジャンプの記録、頁移動したら、その頁に合わせる
新規検索したらそれに合わせる

検索位置に文字を追加削除したときの処理は？
壱文字追加削除・その行だけもう一度サーチすればいい
改行まではいったら、再描画範囲注意

ペーストとか、選択削除みたいな大技はいったら
削除は,終わった後のカーソル位置でいけるか？ペーストは、始めた行と,ペースト範囲行だけで？

*/

#ifdef FIND_STRINGS



//	ヒット位置を記録
//typedef struct tagFINDPOSITION
//{
//	LPARAM	dUnique;	//!<	ファイル通し番号・１インデックス
//	INT		iPage;		//!<	属してる頁
//	INT		iLine;		//!<	該当行
//	INT		iCaret;		//!<	行内での文字位置
//
//} FINDPOSITION, *LPFINDPOSITION;

//-------------------------------------------------------------------------------------------------

extern list<ONEFILE>	gltMultiFiles;	//	複数ファイル保持
//イテレータのtypedefはヘッダへ

extern FILES_ITR	gitFileIt;			//		今見てるファイルの本体

extern INT		gixFocusPage;			//		注目中のページ・とりあえず０・０インデックス

EXTERNED HWND	ghFindDlg;				//!<	検索ダイヤログのハンドル


static TCHAR	gatNowPtn[MAX_PATH];	//!<	最新の検索文字列
static INT		gixFindMode;			//!<	最新の検索モード　０頁のみ　１単ファイル　キャンセル＞２全ファイル
static INT		giCrLfCnt;				//!<	検索文字列中に改行がいくつあるか


//static FINDPOSITION	gstFindPos;			//!<	検索ジャンプ位置

//static list<FINDPOSITION>	gltFindPosition;	//!<	検索結果保持
//-------------------------------------------------------------------------------------------------


INT_PTR	CALLBACK FindStrDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	
HRESULT	FindExecute( HWND );	//!<	
HRESULT	FindPageSearch( LPTSTR, INT, FILES_ITR );	//!<	
INT		FindPageHighlightOff( INT , FILES_ITR );	//!<	
HRESULT	FindPageHighlightSet( INT, INT, INT, FILES_ITR );	//!<	
HRESULT FindLineHighlightOff( UINT , LINE_ITR );	//!<	
//-------------------------------------------------------------------------------------------------


/*!
	検索ダイヤログを開く・モーダレスで
	@param[in]	hInst	アポリケーションの実存
	@param[in]	hWnd	ウインドウハンドル
	@retval HRESULT	終了状態コード
*/
HRESULT FindDialogueOpen( HINSTANCE hInst, HWND hWnd )
{

	if( !(hInst) || !(hWnd) )	//	変数初期化しておくだけ
	{
		ZeroMemory( gatNowPtn, sizeof(gatNowPtn) );
		gixFindMode = 0;
		giCrLfCnt = 0;
	}

	if( ghFindDlg )
	{
		SetForegroundWindow( ghFindDlg );
		return S_FALSE;
	}

	ghFindDlg = CreateDialogParam( hInst, MAKEINTRESOURCE(IDD_FIND_STRING_DLG), hWnd, FindStrDlgProc, 0 );

	ShowWindow( ghFindDlg, SW_SHOW );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	検索ダイヤログのプロシージャ
	@param[in]	hDlg	ダイヤログハンドル
	@param[in]	message	ウインドウメッセージの識別番号
	@param[in]	wParam	追加の情報１
	@param[in]	lParam	追加の情報２
	@retval 0	メッセージは処理していない
	@retval no0	なんか処理された
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
			//	コンボボックスに項目入れる
			hWorkWnd = GetDlgItem( hDlg, IDCB_FIND_TARGET );
			ComboBox_InsertString( hWorkWnd, 0, TEXT("現在の頁") );
			ComboBox_InsertString( hWorkWnd, 1, TEXT("現在のファイル") );
	//		ComboBox_InsertString( hWorkWnd, 2, TEXT("開いている全てのファイル") );無しで
			ComboBox_SetCurSel( hWorkWnd, gixFindMode );	//	今の検索モードを反映する
	//		ComboBox_Enable( hWorkWnd , FALSE );	//	機能出来るまで凍結

			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			Edit_SetText( hWorkWnd, gatNowPtn );	//	今の検索内容があれば転写する
			SetFocus( hWorkWnd );
			return (INT_PTR)FALSE;


		case WM_COMMAND:
			id = LOWORD(wParam);
			hWorkWnd = GetDlgItem( hDlg, IDE_FIND_TEXT );
			switch( id )
			{
				case IDCANCEL:	DestroyWindow( hDlg );	return (INT_PTR)TRUE;
				case IDOK:		FindExecute( hDlg );	return (INT_PTR)TRUE;	//	検索する

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
			ViewFocusSet(  );	//	フォーカスを描画に戻す
			return (INT_PTR)TRUE;

	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	検索実行
	@param[in]	hDlg	ダイヤログハンドル
*/
HRESULT FindExecute( HWND hDlg )
{
//	HWND	hWorkWnd;
	UINT_PTR	dPage, d, h;
	INT		dRange;	//	検索範囲　０頁　１ファイル 　キャンセル＞２全開きファイル
	BOOLEAN	bModCrlf;

	TCHAR	atPattern[MAX_PATH], atBuf[MAX_PATH];


	//	￥ｎを改行、￥￥を￥にするか
	bModCrlf = IsDlgButtonChecked( hDlg, IDCB_MOD_CRLF_YEN );

	//	検索範囲	０頁　１ファイル
	dRange = ComboBox_GetCurSel( GetDlgItem(hDlg,IDCB_FIND_TARGET) );
	//
	//検索パヤーン
	Edit_GetText( GetDlgItem(hDlg,IDE_FIND_TEXT), atBuf, MAX_PATH );

	if( atBuf[0] )	return  E_ABORT;	//	空文字列なら何もしない

	giCrLfCnt = 0;	//	改行カウント・通常0かな
	ZeroMemory( atPattern, sizeof(atPattern) );
	if( bModCrlf )	//	エスケープシーケンスを展開
	{
		for( d = 0, h = 0; MAX_PATH > d; d++, h++ )
		{
			atPattern[h] = atBuf[d];	
			if( 0x005C == atBuf[d] )	//	0x005Cは￥
			{
				d++;
				if( TEXT('n') ==  atBuf[d] )	//	改行指示である場合
				{
					atPattern[h] = TEXT('\r');	h++;
					atPattern[h] = TEXT('\n');
					giCrLfCnt++;	//	改行カウント増やす
				}
			}
		}
	}
	else
	{
		StringCchCopy( atPattern, MAX_PATH, atBuf );
	}

//検索条件が前と変わらず、全頁検索なら、次のヒット頁に移動する
//変わっているか、単頁検索なら、普通に検索
	if( !( StrCmp(atPattern,gatNowPtn) ) && gixFindMode == dRange )
	{
		//	次ヒット頁に移動
#error 作成中
	}
	else
	{
		//	条件通りに検索

		gixFindMode = dRange;

		FindHighlightOff(  );	//	先のパヤーン破棄

		//	とりあえず頁Search
		if( dRange )	//	全頁検索しちゃったりして
		{
			dPage = gitFileIt->vcCont.size( );
			for( d = 0; dPage > d; d++ )
			{
				FindPageSearch( atPattern, d, gitFileIt );
			}
		}
		else	//	なう頁のみ
		{
			FindPageSearch( atPattern, gixFocusPage, gitFileIt );
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	指定パヤーンを、指定ファイルの、指定頁で検索
	@param[in]	ptPattern	検索パヤーン・NULLなら最後の検索文字列を使用する
	@param[in]	iTgtPage	対象頁
	@param[in]	itFile		ファイルイテレータ
	@return		HRESULT	終了状態コード・ヒットしたらS_OK
*/
HRESULT FindPageSearch( LPTSTR ptPattern, INT iTgtPage, FILES_ITR itFile )
{
	HRESULT		hRslt;
	INT			dCch, dLeng;
	INT_PTR		dBytes;
	UINT_PTR	cchSize, cchSzPtn;
	LPTSTR		ptPage, ptCaret, ptFind = NULL;

	TCHAR	ttBuf;

	TRACE( TEXT("PAGE[%d] SEARCH"), iTgtPage );

	if( ptPattern ){	StringCchCopy( gatNowPtn, MAX_PATH, ptPattern );	}	//	ラストメモリー

	StringCchLength( gatNowPtn, MAX_PATH, &cchSzPtn );

	//	ディレイしていればチェックしなくていい
	if( PageIsDelayed( itFile, iTgtPage ) ){	return  E_ABORT;	}
	//	ヒットがあるかどうかだけ返す、でもいいか

	//	頁全体確保
	dBytes = DocPageTextGetAlloc( itFile, iTgtPage, D_UNI, (LPVOID *)(&ptPage), FALSE );
	StringCchLength( ptPage, STRSAFE_MAX_CCH, &cchSize );

	ptCaret = ptPage;

	//	検索する
	hRslt = E_NOTIMPL;
	dLeng = 0;
	do
	{
		ptFind = FindStringProc( ptCaret, gatNowPtn, &dCch );	//	ヒットしたら先頭部分かえる
		if( !(ptFind) ){	break;	}	//	ヒットしなかった

		dLeng += dCch;	//	文字位置・０インデックス
		ttBuf = ptPage[dLeng];

		//	ヒットした部分に色を付ける
		FindPageHighlightSet( dLeng, cchSzPtn, iTgtPage, itFile );	//	中で該当領域の再描画まで指定してる

		hRslt = S_OK;

		ptCaret = ptFind;
		ptCaret++;

		dLeng++;

	}while( *ptCaret );

	FREE(ptPage);

	return hRslt;
}
//-------------------------------------------------------------------------------------------------

/*!
	指定ファイルの指定頁の指定文字位置から指定文字数をハイライト指定にする。改行コード含む。
	@param[in]	iOffset	頁先頭からの文字数・改行コード含む。
	@param[in]	iRange	ハイライト文字数
	@param[in]	iPage	対象頁
	@param[in]	itFile	ファイルイテレータ
	@return		HRESULT	終了状態コード
*/
HRESULT FindPageHighlightSet( INT iOffset, INT iRange, INT iPage, FILES_ITR itFile )
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

		dMozis = itLine->vcLine.size( );	//	この行の文字数確認して
		iLetters = dMozis + 2;	//	改行コード

		iTotal += iLetters;

		if( iOffset < iTotal )	//	行末端までの文字数よりオフセットが小さかったら、その行に含まれる
		{
			iSlide = iOffset - iLnTop;	//	その行先頭からの文字数
			//	もし改行から検索だったら、iSlide = dMozis になる
			iNext = 0;	//	改行が有る場合の残り文字数

			//	ここで改行の巻き込み状況を確認して、次の行かぶりとかチェック？
			if( dMozis < (iSlide + iRange) )	//	成立なら、改行がはみ出してる
			{
				iNext  = iRange;
				iRange = dMozis - iSlide;	//	残り文字数
				iNext -= iRange;	//	改行を含めた残り文字数
			}

			iDot = 0;	//	そこまでのドット数をため込む
			for( mz = 0; iSlide > mz; mz++ )	//	該当文字まで進めてドット数ためとく
			{
				//	もし改行から検索ならこれが成立
				if( dMozis <=  mz ){	iDot += iWid;	break;	}

				iDot += itLine->vcLine.at( mz ).rdWidth;

				iWid  = itLine->vcLine.at( mz ).rdWidth;	//	この文字の幅
			}

			for(  ; (iSlide+iRange) > mz; mz++ )
			{
				//	改行までめり込むならここが成立
				if( dMozis <=  mz ){	break;	}

				iWid = itLine->vcLine.at( mz ).rdWidth;

				inRect.left  = iDot;
				inRect.right = iDot + iWid;

				itLine->vcLine.at( mz ).mzStyle |= CT_FINDED;
				ViewRedrawSetRect( &inRect );

				iDot += iWid;
			}

			if( 0 <	 iNext )	//	改行があるなら数値が存在
			{
				inRect.right += 20;

				itLine->dStyle |= CT_FINDRTN;
				ViewRedrawSetRect( &inRect );

				iNext -=  2;	//	改行文字分へらして

			}

			//	必要に応じて、ここで終わったり次の行の処理にうつったり
			if( 0 >= iNext ){	break;	}
			else
			{
				iOffset += (2 + iRange);	//	改行文字分＋この行で使った文字数進める
				iRange  = iNext;	//	残り文字数書換
			}
		}

		iLnTop += iLetters;
	}


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	全部ハイライトＯＦＦ
	@return	HRESULT	終了状態コード
*/
HRESULT FindHighlightOff( VOID )
{
	UINT_PTR	dPage, d;

	dPage = gitFileIt->vcCont.size( );

	for( d = 0; dPage > d; d++ )
	{
		FindPageHighlightOff( d, gitFileIt );
	}

	//	現在内容も初期化
	FindDialogueOpen( NULL, NULL );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	指定ファイルの指定頁の検索ハイライトをOFFする
	@param[in]	iPage	対象頁
	@param[in]	itFile	ファイルイテレータ
	@return		特にない
*/
INT FindPageHighlightOff( INT iPage, FILES_ITR itFile )
{
	UINT_PTR	ln;//, iLines, iLetters, mz;
//	UINT		dStyle;
//	INT			iDot, iWid;
//	RECT		inRect;

	LINE_ITR	itLine, itLnEnd;


	if( 0 >  iPage )	return 0;	//	特殊な状況下では処理しない

	ZeroMemory( gatNowPtn, sizeof(gatNowPtn) );

	itLine  = itFile->vcCont.at( iPage ).ltPage.begin();
	itLnEnd = itFile->vcCont.at( iPage ).ltPage.end();

	for( ln = 0; itLnEnd != itLine; itLine++, ln++ )
	{
		FindLineHighlightOff( ln, itLine );	//	中でREDRAW処理してる
	}

//	ViewRedrawSetLine( -1 );	//	画面表示更新

	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	指定行の検索ハイライトをOFFする
	@param[in]	iLine	行数・０インデックス
	@param[in]	itFile	行イテレータ
	@return	HRESULT	終了状態コード
*/
HRESULT FindLineHighlightOff( UINT iLine, LINE_ITR itLine )
{
	UINT_PTR	iLetters, mz;//, iLines, ln;
	UINT		dStyle;
	INT			iDot, iWid;
	RECT		inRect;


	iDot = 0;	//	そこまでのドット数をため込む
	inRect.top    = iLine * LINE_HEIGHT;
	inRect.bottom = inRect.top + LINE_HEIGHT;

	iLetters = itLine->vcLine.size();
	//	壱文字ずつ、全部をチェキっていく
	for( mz = 0; iLetters > mz; mz++ )
	{
		//	直前の状態
		dStyle = itLine->vcLine.at( mz ).mzStyle;
		iWid   = itLine->vcLine.at( mz ).rdWidth;

		inRect.left  = iDot;
		inRect.right = iDot + iWid;

		itLine->vcLine.at( mz ).mzStyle &= ~CT_FINDED;
		if( dStyle & CT_FINDED )	ViewRedrawSetRect( &inRect );

		iDot += iWid;
	}

	//	壱行終わったら末尾状況確認。改行・本文末端に改行はない・選択のときのみ
	dStyle = itLine->dStyle;
	inRect.left  = iDot;
	inRect.right = iDot + 20;	//	改行描画エリア・大体これくらい

	itLine->dStyle &=  ~CT_FINDRTN;
	if( dStyle & CT_FINDRTN )	ViewRedrawSetRect( &inRect );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#if 0
/*!
	検索位置へジャンプ
	@param[in]	dMode	０次へ　１前へ
	@param[in]	pxDot	カーソルドット位置・処理したら戻す
	@param[in]	pyLine	カーソル行・処理したら戻す
	@param[in]	pMozi	キャレットの左側の文字数・処理したら戻す
	@return		INT		負：エラー　０：ヒット無し　１〜：ヒットした
*/
INT FindStringJump( UINT dMode, PINT pXdot, PINT pYline, PINT pMozi )
{
	INT			iXdot, iYline, iMozi;
	BOOLEAN		bStart, bBegin;
	INT_PTR		dTotalPage, dTotalLine;
	PAGE_ITR	itPage;	//	頁を順に見ていく
	LINE_ITR	itLine;	//	行を順に見ていく
	LETR_ITR	itMozi;	//	文字を順に見ていく

	if( !(pXdot) || !(pYline) || !(pMozi) ){	return -1;	}

	//	頁はみ出しチェキ
	dTotalPage = gitFileIt->vcCont.size();
	if( !(0 <= gixFocusPage && gixFocusPage < dTotalPage) ){	return -1;	}
	//	今の頁に注目
	itPage = gitFileIt->vcCont.begin();
	std::advance( itPage, gixFocusPage );

	iXdot  = *pXdot;
	iYline = *pYline;
	iMozi  = *pMozi;


	//	行はみ出しチェキ
	dTotalLine = itPage->ltPage.size();
	if( !(0 <= iYline && iYline < dTotalLine) ){	return -1;	}
	//	今の行に注目
	itLine = itPage->ltPage.begin();
	std::advance( itLine, iYline );

	itMozi = itLine->vcLine.begin();
	std::advance( itMozi, iMozi );

	bStart = TRUE;	bBegin = TRUE;
	for( ; itPage->ltPage.end() != itLine; itLine++ )
	{
		//	最初はそのまま、次回からはその行の先頭文字から見ていく
		if( bStart ){	bStart =  FALSE;	}
		else{	itMozi = itLine->vcLine.begin();	}

		for( ;itLine->vcLine.end() != itMozi; itMozi++ )
		{
			if( itMozi->mzStyle & CT_FINDED )	//	ヒット
			{
				if( !(bBegin)  )	//	検索開始位置のは無視する
				{
					*pXdot  = iXdot;
					*pYline = iYline;
					*pMozi  = iMozi;

					ViewDrawCaret( iXdot, iYline, TRUE );	//	キャレットも移動

					return 1;
					//	頁またぎはどうすんべ
				}
			}
			else{	bBegin =  FALSE;	}
			//	

			iXdot += itMozi->rdWidth;
			iMozi++;
		}

		iYline++;	//	行番号は増やす
		iXdot = 0;
		iMozi = 0;
	}

	//	頁またぎはどうすんべ

	return 0;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	今頁を再検索・画面再描画とか
	@return	HRESULT	終了状態コード
*/
HRESULT FindNowPageReSearch( VOID )
{
	FindHighlightOff(  );

	FindPageSearch( gatNowPtn, gixFocusPage, gitFileIt );	//	重い

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	遅延読込頁を開くときに、検索ハイライツの面倒見る
	@param[in]	iTgtPage	対象頁番号
	@return	HRESULT	終了状態コード
*/
HRESULT FindDelayPageReSearch( INT iTgtPage )
{

	//	全体検索でないか、検索文字列が空なら無視してよろし
	if(  1 != gixFindMode || NULL == gatNowPtn[0] ){	return  E_ABORT;	}


	//	とりあえず頁Search
	FindPageSearch( NULL, iTgtPage, gitFileIt );


	return S_OK;
}
//-------------------------------------------------------------------------------------------------

#if 0
/*!
	文字が挿入削除されたときに、その部分のCheckをする
	@param[in]	iTgtLine	対象行番号
	@return	HRESULT	終了状態コード
*/
HRESULT FindTextModifyLine( INT iTgtLine )
{
//該当行と改行が及ぶ範囲の行のみ、いったんチェック外して、該当範囲の行だけチェキ
//コピペされた場合は？　挿入削除処理が行われてからチェックするなら、影響半亥注意

	//	giCrLfCnt

	//行数確認・はみ出さないように


	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif

#endif	//	FIND_STRINGS
