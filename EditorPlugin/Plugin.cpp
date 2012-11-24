/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグイン管理クラス
*
*---------------------------------------------------------------------------------*/
#include "../stdafx.h"
#include "Plugin.h"
#include "PluginExternVariables.h"


namespace oe { namespace plugin {


/**** 静的メンバ定義 ****/
bool						OePlugin::m_Initialized = false;		//!< 初期化済みであるか
OePlugin::PluginSetList		OePlugin::m_PluginSetList;				//!< プラグインセットリスト
OePluginEditControl			OePlugin::m_PluginEditControl;			//!< プラグインに渡す編集補助クラス

//-- サブクラス化用
WNDPROC						OePlugin::m_OldMainProc = NULL;			//!< サブクラスから呼ばれるメインウィンドウプロシージャ


/**** プラグイン管理クラス ****/
/**
* プラグイン管理の初期化
*	@param[in]	dir						プラグインディレクトリパス
*	@return		bool					初期化に成功したら真、それ以外は偽
*/
bool OePlugin::Initialize( const LPCTSTR dir )
{
	if( !m_Initialized )
	{
		m_OldMainProc = NULL;


		/**** プラグインセットを初期化 ****/
		m_PluginSetList.clear();


		/**** 検索パスを生成 ****/
		LPCTSTR plginExp[] =
		{
			_T("opi"), _T("dll"), NULL
		};
		for( int i = 0; plginExp[ i ]; i++ )
		{
			TCHAR findPath[ MAX_PATH ];
			_stprintf_s( findPath, _T("%s\\*.%s"), dir, plginExp[ i ] );


			/**** 指定されたプラグインディレクトリを走査 ****/
			WIN32_FIND_DATA findData;
			HANDLE hFind = FindFirstFile( findPath, &findData );
			if( hFind != INVALID_HANDLE_VALUE )
			{
				do
				{
					// プラグインパスを設定
					PluginLibInfo set = { _T(""), NULL, NULL };
					_stprintf_s( set.path, _T("%s\\%s"), dir, findData.cFileName );

					// ライブラリを読み込む
					set.hDll = LoadLibrary( set.path );
					if( set.hDll )
					{
						set.getInfoFunc = (GetPluginsetInfo)GetProcAddress( set.hDll, "_OePlugin_GetPluginSetInfo@0" );
						if( set.getInfoFunc && (set.pPluginsetInfo = set.getInfoFunc()) )
						{
							m_PluginSetList.push_back( set );
							TRACE( _T("plugin[%d]:%s"), m_PluginSetList.size(), set.pPluginsetInfo->pluginsetName );
						}
						else
						{
							FreeLibrary( set.hDll );
							//TRACE( _T("Can't loaded plugin:%s"), m_PluginSetList.size(), set.pPluginsetInfo->pluginsetName );
						}
					}
				} while( FindNextFile( hFind, &findData ) );
				FindClose( hFind );
			}
		}


		/**** 初期化フラグを立てる ****/
		m_Initialized = true;
	}


	return m_Initialized;
}

/**
* プラグイン管理が初期化されているか
*	@return		bool					初期化されているならば真、それ以外は偽
*/
bool OePlugin::IsInitialized( void )
{
	return m_Initialized;
}

/**
* プラグイン管理の廃棄
*/
void OePlugin::Finalize( void )
{
	if( m_Initialized )
	{
		/**** 読み込み済みのプラグインを開放 ****/
		PluginSetList::iterator it;
		for( it = m_PluginSetList.begin(); it != m_PluginSetList.end(); it++ )
		{
			if( it->hDll )
			{
				for( unsigned long i = 0; i < it->pPluginsetInfo->count; i++ )
				{
					it->pPluginsetInfo->pluginInfo[ i ].plugin->Finalize( &m_PluginEditControl );
				}
				FreeLibrary( it->hDll );
			}
		}
		m_PluginSetList.clear();


		/**** 初期化フラグを降ろす ****/
		m_Initialized = false;
	}
}

/**
* プラグイン管理情報を更新する
*/
void OePlugin::Update( void )
{
}

/**
* 全プラグインを初期化する
*/
void OePlugin::AllPluginInitialize( void )
{
	/**** プラグインの初期化ルーチンをコール ****/
	PluginSetList::iterator it;
	for( it = m_PluginSetList.begin(); it != m_PluginSetList.end(); it++ )
	{
		for( unsigned long i = 0; i < it->pPluginsetInfo->count; i++ )
		{
			it->pPluginsetInfo->pluginInfo[ i ].plugin->Initialize( &m_PluginEditControl );
		}
	}
}

/**
* プラグインメニューを登録する
*/
void OePlugin::AppendPluginMenu( void )
{
	/**** メニューに追加する ****/
	createPluginMenu();
	appendPluginMenu();

	/**** プロシージャをサブクラス化 ****/
	if( (WNDPROC)GetWindowLong( ghMainWnd, GWL_WNDPROC ) != (WNDPROC)mainPluginMenuProc )
	{
		m_OldMainProc = (WNDPROC)GetWindowLong( ghMainWnd, GWL_WNDPROC );
		SetWindowLong( ghMainWnd, GWL_WNDPROC, (LONG)mainPluginMenuProc );
	}
}

/**
* プラグイン一覧情報を得る
*	@return		PluginSetList&			プラグインセットリストへの参照
*/
OePlugin::PluginSetList& OePlugin::GetPluginSetList( void )
{
	return m_PluginSetList;
}


//-- プラグイン動作
/**
* OneTimeプラグインを動作させる
*	@param[in]	nMenuID					動作対象のメニューID
*	@return		bool					プラグインがあったら真、ソレ以外は偽
*/
bool OePlugin::runOnetimePlugin( UINT nMenuID )
{
	bool ret = false;

	PluginSetList::iterator it;
	for( it = m_PluginSetList.begin(); it != m_PluginSetList.end(); it++ )
	{
		if( it->pPluginsetInfo->count > 0 )
		{// プラグイン検索
			for( ULONG i = 0; i < it->pPluginsetInfo->count; i++ )
			{
				if( it->nMenuID[ i ] == nMenuID )
				{
					it->pPluginsetInfo->pluginInfo[ i ].plugin->Run( &m_PluginEditControl );
					ret = true;
					break;
				}
			}
		}
	}

	return ret;
}


} }
