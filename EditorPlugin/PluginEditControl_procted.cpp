/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグイン - 編集操作管理クラス
*								protected 関数群
*
*---------------------------------------------------------------------------------*/
#include "..\\stdafx.h"
#include "..\\OrinrinEditor.h"
#include "PluginEditControl.h"
#include <string.h>
#include "PluginExternVariables.h"


namespace oe { namespace plugin {


/**** 編集操作管理クラス - protected 関数群 ****/
/**
* HostHandleを初期化
*/
void OePluginEditControl::_initializeHostHandle( void )
{
#define SET_HOST( var, elem ) m_HostWindowHandle.elem = &var

	//-- ウィンドウハンドル
	SET_HOST( ghMainWnd, hMainWnd );

	//-- メニューハンドル
	SET_HOST( ghMenu, hMainMenu );
}

/**
* 指定された種類のファイルパスを取得
*	@param[in]	type					パスの種類
*	@return		const wchar_t*			ファイルパス
*/
const wchar_t* OePluginEditControl::_getPath( PathType type )
{
	const wchar_t* ret = NULL;

	switch( type )
	{
	case OE_PT_ORINRINEDITOR:
		ret = gatExePath;
		break;
	case OE_PT_INIFILE:
		ret = gatIniPath;
		break;
	}

	return ret;
}

/**
* ファイル数取得
*	@return		unsigned long			ファイル数
*/
unsigned long OePluginEditControl::_getFileCount( void )
{
	return (unsigned long)gltMultiFiles.size();
}

/**
* 指定されたファイルパスを取得
*	@param[in]	index					ファイルインデックス番号
*	@return		const wchar_t*			ファイルパス
*/
const wchar_t* OePluginEditControl::_getFileName( unsigned long index )
{
	const wchar_t* ret = NULL;

	if( index < _getFileCount() )
	{
		FILES_ITR it = gltMultiFiles.begin();
		std::advance( it, index );
		ret = it->atFileName;
	}

	return ret;
}

} }
