/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグインメニュー
*
*---------------------------------------------------------------------------------*/
#include "../stdafx.h"
#include "../OrinrinEditor.h"
#include "Plugin.h"
#include "PluginExternVariables.h"


namespace oe { namespace plugin {


/**** 静的メンバ ****/
HMENU		OePlugin::m_hPluginMenu = NULL;					//!< プラグインメニュー


/**** プラグインメニュー関係 ****/
/**
* プラグインメニューを生成する
*/
void OePlugin::createPluginMenu( void )
{
	if( m_PluginSetList.size() > 0 )
	{
		/**** プラグインルートメニューを生成 ****/
		m_hPluginMenu = ::CreatePopupMenu();

		/**** プラグインセットごとに登録 ****/
		UINT cnt = 0;
		PluginSetList::iterator it;
		for( it = m_PluginSetList.begin(); it != m_PluginSetList.end(); it++ )
		{
			if( it->pPluginsetInfo->count > 0 )
			{
				// 登録すべきものがあるか
				bool reg = false;
				for( ULONG i = 0; i < it->pPluginsetInfo->count; i++ )
				{
					it->nMenuID[ i ] = 0;
					if( it->pPluginsetInfo->pluginInfo[ i ].type & OE_PT_MENUITEM )
					{
						reg = true;
						break;
					}
				}

				if( reg )
				{
					HMENU hplset = ::CreatePopupMenu();
					::AppendMenuW( m_hPluginMenu, MF_ENABLED | MF_STRING | MF_POPUP, (UINT)hplset, it->pPluginsetInfo->pluginsetName );

					// プラグインごとに登録
					for( ULONG i = 0; i < it->pPluginsetInfo->count; i++ )
					{
						it->nMenuID[ i ] = 0;
						if( it->pPluginsetInfo->pluginInfo[ i ].type & OE_PT_MENUITEM )
						{// メニューに追加するなら登録
							it->nMenuID[ i ] = IDM_PLUGIN_ITEM_BASE + cnt++;
							::AppendMenuW( hplset, MF_ENABLED | MF_STRING, (UINT)(it->nMenuID[ i ]), it->pPluginsetInfo->pluginInfo[ i ].pluginName );
						}
					}
				}
			}
		}
	}
}

/**
* ホストのメニューにプラグインメニューを接続する
*/
void OePlugin::appendPluginMenu( void )
{
	if( m_hPluginMenu )
	{
		// メインメニューへ追加
		::AppendMenu( ghMenu, MF_ENABLED | MF_STRING | MF_POPUP, (UINT)m_hPluginMenu, _T("プラグイン(&P)") );
		DrawMenuBar( ghMainWnd );
	}
}

/**
* プラグインメニューフック用プロシージャ
*/
LRESULT CALLBACK OePlugin::mainPluginMenuProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_COMMAND:
		{
			int idm = LOWORD( wParam );
			if( idm >= IDM_PLUGIN_ITEM_BASE )
			{
				/**** ワンタイムPluginを実行する ****/
				runOnetimePlugin( idm );
				break;
			}
		}

	default:
		return ::CallWindowProc( m_OldMainProc, hWnd, msg, wParam, lParam );
	}

	return 0;
}



} }
