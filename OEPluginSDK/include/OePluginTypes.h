/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグインSDK 型定義
*
*---------------------------------------------------------------------------------*/
#pragma once

#include <Windows.h>


namespace oe { namespace plugin {


/*---------------------------------------------------
* 定数定義
*/
const int MAX_PLUGIN_NAME = 64;				//!< プラグイン名最大文字数
const int MAX_PLUGIN_COUNT = 32;			//!< プラグインセットに含まれるプラグイン最大数


/*---------------------------------------------------
* 列挙体定義
*/
/**
* OEプラグイン 種別
*/
enum PluginType
{
	OE_PT_NONE				= 0x00000000,			//!< 初期化、廃棄以外に呼び出されません
	OE_PT_MENUITEM			= 0x00000001,			//!< メニューに追加し、項目の選択で呼び出されるようにします

	OE_PLUGINTYPE_FULLBIT	= 0xffffffff			//!< すべてのフラグを立てる
};

/**
* OEウィンドウ取得用種別
*/
enum WindowType
{
	OE_WT_MAINWINDOW = 0x00,						//!< メインウィンドウ

	OE_WINDOWTYPE_MAX,								//!< 最大値
	OE_WINDOWTYPE_UNKNOWN = 0xffffffff				//!< 不明なウィンドウ
};

/**
* OEメニュー取得用種別
*/
enum MenuType
{
	OE_MT_MAINMENU = 0x00,							//!< メインメニュー

	OE_MENUTYPE_MAX,								//!< 最大値
	OE_MENUTYPE_UNKNOWN = 0xffffffff				//!< 不明なウィンドウ
};

/**
* OEパス取得用種別
*/
enum PathType
{
	OE_PT_ORINRINEDITOR = 0x00,						//!< OrinrinEditor.exeへのパス
	OE_PT_INIFILE,									//!< .iniファイルへのパス

	OE_PATHTYPE_MAX,								//!< 最大値
	OE_PATHTYPE_UNKNOWN = 0xffffffff				//!< 不明なウィンドウ
};


/*---------------------------------------------------
* インターフェイス定義
*/
/**
* プラグインへ渡されるエディタ操作インターフェイス
*/
typedef class IEditControl
{
public:
	/**
	* ウィンドウハンドルを取得する
	*	@param[in]	type					取得するウィンドウの種別
	*	@return		HWND					ウィンドウハンドル、失敗時はNULL
	*/
	virtual HWND GetWindowHandle( WindowType type ) = 0;

	/**
	* メニューハンドルを取得する
	*	@param[in]	type					取得するメニューの種別
	*	@return		HMENU					メニューハンドル、失敗時はNULL
	*/
	virtual HMENU GetMenuHandle( MenuType type ) = 0;

	/**
	* 各種パスを取得する
	*	@param[in]	type					取得するパスの種別
	*	@param[out]	path					パス格納先
	*	@param[in]	length					pathに格納可能な文字数
	*	@return		int						格納した文字数、存在しない場合は-1
	*/
	virtual int GetPath( PathType type, wchar_t* path, unsigned long length ) = 0;

	/**
	* オープン済みのファイル数を取得する
	*	@return		unsigned long			オープン済みのファイル数
	*/
	virtual unsigned long GetFileCount( void ) = 0;

	/**
	* 指定された番号のファイルパスを取得する
	*	@param[in]	index					ファイルインデックス番号
	*	@param[out]	path					ファイルパス格納先
	*	@param[in]	length					pathに格納可能な文字数
	*	@return		int						格納した文字数、存在しない場合は-1
	*/
	virtual int GetFileName( unsigned long index, wchar_t* path, unsigned long length ) = 0;

	/**
	* ファイルを追加で読み込む
	*	@param[in]	path					追加するファイルパス
	*	@return		bool					成功したら真、それ以外は偽
	*/
	virtual bool InsertFile( LPCWSTR path ) = 0;

	/**
	* 指定のファイルを削除する
	*	@param[in]	index					削除するファイルインデックス
	*	@return		bool					削除に成功したら真、それ以外は偽
	*/
	virtual bool EraseFile( unsigned long index ) = 0;
} EDITCONTROL, *PEDITCONTROL, *LPEDITCONTROL;

/**
* プラグイン
*	このクラスを継承したインスタンスを
*	ホストへ返すようにしてください。
*/
typedef class IPlugin
{
public:
	/**
	* プラグインの読み込み時にコールされます
	*	@param[in]	pEditCtrl				編集操作インターフェイス
	*	@return		bool					初期化に成功した場合は真、それ以外は偽
	*/
	virtual bool Initialize( LPEDITCONTROL pEditCtrl ) = 0;

	/**
	* プラグインの廃棄時にコールされます
	*	@param[in]	pEditCtrl				編集操作インターフェイス
	*/
	virtual void Finalize( LPEDITCONTROL pEditCtrl ) = 0;

	/**
	* メニューで選択された際にコールされます
	*	@param[in]	pEditCtrl				編集操作インターフェイス
	*/
	virtual void Run( LPEDITCONTROL pEditCtrl ) = 0;
} PLUGIN, *PPLUGIN, *LPPLUGIN;


/*---------------------------------------------------
* 構造体定義
*/
/**
* プラグイン情報構造体
*/
typedef struct PluginInfo
{
	wchar_t				pluginName[ MAX_PLUGIN_NAME ];		//!< プラグイン名
	unsigned long		type;								//!< プラグイン種別
	plugin::LPPLUGIN	plugin;								//!< プラグインポインタ
} PLUGININFO, *PPLUGININFO, *LPPLUGININFO;

/**
* プラグインセット情報構造体
*/
typedef struct PluginSetInfo
{
	wchar_t				pluginsetName[ MAX_PLUGIN_NAME ];	//!< プラグインセット名
	unsigned long		count;								//!< プラグイン数
	PluginInfo			pluginInfo[ MAX_PLUGIN_COUNT ];		//!< 個々のプラグイン情報
} PLUGINSETINFO, *PPLUGINSETINFO, *LPPLUGINSETINFO;


} }
