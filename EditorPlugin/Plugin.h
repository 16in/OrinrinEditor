/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグイン管理クラス
*
*---------------------------------------------------------------------------------*/
#pragma once

#include <tchar.h>
#include <Windows.h>
#include <OePlugin.h>
#include <vector>
#include "PluginEditControl.h"


namespace oe { namespace plugin {


/**
* プラグイン管理クラス
*/
class OePlugin
{
public:
	//! プラグイン情報取得関数型
	typedef LPPLUGINSETINFO (__stdcall *GetPluginsetInfo)( void );

	//! プラグインセット情報配列
	struct PluginLibInfo
	{
		TCHAR					path[ MAX_PATH ];				//!< プラグインへのフルパス
		HMODULE 				hDll;							//!< DLLハンドル
		GetPluginsetInfo		getInfoFunc;					//!< プラグイン情報取得関数
		UINT					nMenuID[ MAX_PLUGIN_COUNT ];	//!< プラグインに関連づいているメニューID
		LPPLUGINSETINFO			pPluginsetInfo;					//!< プラグインセットへのポインタ
	};
	typedef std::vector<PluginLibInfo>	PluginSetList;

protected:
	//-- プラグイン管理情報
	static bool					m_Initialized;					//!< 初期化済みであるか
	static PluginSetList		m_PluginSetList;				//!< プラグインセットリスト
	static OePluginEditControl	m_PluginEditControl;			//!< プラグインに渡す編集補助クラス

	//-- プラグインメニュー
	static HMENU				m_hPluginMenu;					//!< プラグインメニュー

	//-- サブクラス化
	static WNDPROC				m_OldMainProc;					//!< サブクラスから呼ばれるメインウィンドウプロシージャ


public:
	/**
	* プラグイン管理の初期化
	*	@param[in]	dir						プラグインディレクトリパス
	*	@return		bool					初期化に成功したら真、それ以外は偽
	*/
	static bool Initialize( const LPCTSTR dir = _T("Plugin") );

	/**
	* プラグイン管理が初期化されているか
	*	@return		bool					初期化されているならば真、それ以外は偽
	*/
	static bool IsInitialized( void );

	/**
	* プラグイン管理の廃棄
	*/
	static void Finalize( void );

	/**
	* プラグイン管理情報を更新する
	*/
	static void Update( void );

	/**
	* 全プラグインを初期化する
	*/
	static void AllPluginInitialize( void );

	/**
	* プラグインメニューを登録する
	*/
	static void AppendPluginMenu( void );

	/**
	* プラグイン一覧情報を得る
	*	@return		PluginSetList&			プラグインセットリストへの参照
	*/
	static PluginSetList& GetPluginSetList( void );


protected:
	//-- プラグイン動作
	/**
	* OneTimeプラグインを動作させる
	*	@param[in]	nMenuID					動作対象のメニューID
	*	@return		bool					プラグインがあったら真、ソレ以外は偽
	*/
	static bool runOnetimePlugin( UINT nMenuID );

	//-- メニュー関係
	/**
	* プラグインメニューを生成する
	*/
	static void createPluginMenu( void );

	/**
	* ホストのメニューにプラグインメニューを接続する
	*/
	static void appendPluginMenu( void );

	/**
	* プラグインメニューフック用プロシージャ
	*/
	static LRESULT CALLBACK mainPluginMenuProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};


} }
