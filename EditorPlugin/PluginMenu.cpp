/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C�����j���[
*
*---------------------------------------------------------------------------------*/
#include "../stdafx.h"
#include "../OrinrinEditor.h"
#include "Plugin.h"
#include "PluginExternVariables.h"


namespace oe { namespace plugin {


/**** �ÓI�����o ****/
HMENU		OePlugin::m_hPluginMenu = NULL;					//!< �v���O�C�����j���[


/**** �v���O�C�����j���[�֌W ****/
/**
* �v���O�C�����j���[�𐶐�����
*/
void OePlugin::createPluginMenu( void )
{
	if( m_PluginSetList.size() > 0 )
	{
		/**** �v���O�C�����[�g���j���[�𐶐� ****/
		m_hPluginMenu = ::CreatePopupMenu();

		/**** �v���O�C���Z�b�g���Ƃɓo�^ ****/
		UINT cnt = 0;
		PluginSetList::iterator it;
		for( it = m_PluginSetList.begin(); it != m_PluginSetList.end(); it++ )
		{
			if( it->pPluginsetInfo->count > 0 )
			{
				// �o�^���ׂ����̂����邩
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

					// �v���O�C�����Ƃɓo�^
					for( ULONG i = 0; i < it->pPluginsetInfo->count; i++ )
					{
						it->nMenuID[ i ] = 0;
						if( it->pPluginsetInfo->pluginInfo[ i ].type & OE_PT_MENUITEM )
						{// ���j���[�ɒǉ�����Ȃ�o�^
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
* �z�X�g�̃��j���[�Ƀv���O�C�����j���[��ڑ�����
*/
void OePlugin::appendPluginMenu( void )
{
	if( m_hPluginMenu )
	{
		// ���C�����j���[�֒ǉ�
		::AppendMenu( ghMenu, MF_ENABLED | MF_STRING | MF_POPUP, (UINT)m_hPluginMenu, _T("�v���O�C��(&P)") );
		DrawMenuBar( ghMainWnd );
	}
}

/**
* �v���O�C�����j���[�t�b�N�p�v���V�[�W��
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
				/**** �����^�C��Plugin�����s���� ****/
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
