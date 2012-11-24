/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグイン - 編集操作管理クラス
*
*---------------------------------------------------------------------------------*/
#include "PluginEditControl.h"
#include <string.h>


/*-- ハンドル取得用マクロ --*/
#define GET_HANDLE( type, elem ) case OE_##type: ret = m_HostWindowHandle.elem ? *m_HostWindowHandle.elem : NULL; break


namespace oe { namespace plugin {


/**** 編集操作管理クラス ****/
/**
* コンストラクタ
*/
OePluginEditControl::OePluginEditControl( void )
{
	_initializeHostHandle();
}

/**
* ウィンドウハンドルを取得する
*	@param[in]	type					取得するウィンドウの種別
*	@return		HWND					ウィンドウハンドル、失敗時はNULL
*/
HWND OePluginEditControl::GetWindowHandle( WindowType type )
{
	HWND ret = NULL;

	// ウィンドウハンドル取得
	switch( type )
	{
	GET_HANDLE( WT_MAINWINDOW, hMainWnd );
	}

	// ウィンドウハンドルチェック
	if( ret && !::IsWindow( ret ) ) ret = NULL;

	return ret;
}

/**
* メニューハンドルを取得する
*	@param[in]	type					取得するメニューの種別
*	@return		HMENU					メニューハンドル、失敗時はNULL
*/
HMENU OePluginEditControl::GetMenuHandle( MenuType type )
{
	HMENU ret = NULL;

	// ウィンドウハンドル取得
	switch( type )
	{
	GET_HANDLE( MT_MAINMENU, hMainMenu );
	}

	return ret;
}

/**
* 各種パスを取得する
*	@param[in]	type					取得するパスの種別
*	@param[out]	path					パス格納先
*	@param[in]	length					pathに格納可能な文字数
*	@return		int						格納した文字数、存在しない場合は-1
*/
int OePluginEditControl::GetPath( PathType type, wchar_t* path, unsigned long length )
{
	int ret = -1;
	const wchar_t* srcpath = _getPath( type );

	if( srcpath )
	{
		if( path == NULL )
		{
			ret = (int)wcslen( srcpath );
		}
		else
		{
			wcsncpy_s( path, length, srcpath, length );
			ret = (int)wcslen( path );
		}
	}

	return ret;
}

/**
* オープン済みのファイル数を取得する
*	@return		unsigned long			オープン済みのファイル数
*/
unsigned long OePluginEditControl::GetFileCount( void )
{
	return _getFileCount();
}

/**
* 指定された番号のファイルパスを取得する
*	@param[in]	index					ファイルインデックス番号
*	@param[out]	path					ファイルパス格納先
*	@param[in]	length					pathに格納可能な文字数
*	@return		int						格納した文字数、存在しない場合は-1
*/
int OePluginEditControl::GetFileName( unsigned long index, wchar_t* path, unsigned long length )
{
	int ret = -1;

	const wchar_t* name = _getFileName( index );
	if( name )
	{
		if( path == NULL )
		{
			ret = (int)wcslen( name );
		}
		else
		{
			wcsncpy_s( path, length, name, length );
			ret = (int)wcslen( path );
		}
	}

	return ret;
}

/**
* ファイルを追加で読み込む
*	@param[in]	path					追加するファイルパス
*	@return		bool					成功したら真、それ以外は偽
*/
bool OePluginEditControl::InsertFile( LPCWSTR path )
{
	(void)path;
	return false;
}

/**
* 指定のファイルを削除する
*	@param[in]	index					削除するファイルインデックス
*	@return		bool					削除に成功したら真、それ以外は偽
*/
bool OePluginEditControl::EraseFile( unsigned long index )
{
	(void)index;
	return false;
}


} }
