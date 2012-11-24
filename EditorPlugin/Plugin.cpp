/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C���Ǘ��N���X
*
*---------------------------------------------------------------------------------*/
#include "../stdafx.h"
#include "Plugin.h"
#include "PluginExternVariables.h"


namespace oe { namespace plugin {


/**** �ÓI�����o��` ****/
bool						OePlugin::m_Initialized = false;		//!< �������ς݂ł��邩
OePlugin::PluginSetList		OePlugin::m_PluginSetList;				//!< �v���O�C���Z�b�g���X�g
OePluginEditControl			OePlugin::m_PluginEditControl;			//!< �v���O�C���ɓn���ҏW�⏕�N���X

//-- �T�u�N���X���p
WNDPROC						OePlugin::m_OldMainProc = NULL;			//!< �T�u�N���X����Ă΂�郁�C���E�B���h�E�v���V�[�W��


/**** �v���O�C���Ǘ��N���X ****/
/**
* �v���O�C���Ǘ��̏�����
*	@param[in]	dir						�v���O�C���f�B���N�g���p�X
*	@return		bool					�������ɐ���������^�A����ȊO�͋U
*/
bool OePlugin::Initialize( const LPCTSTR dir )
{
	if( !m_Initialized )
	{
		m_OldMainProc = NULL;


		/**** �v���O�C���Z�b�g�������� ****/
		m_PluginSetList.clear();


		/**** �����p�X�𐶐� ****/
		LPCTSTR plginExp[] =
		{
			_T("opi"), _T("dll"), NULL
		};
		for( int i = 0; plginExp[ i ]; i++ )
		{
			TCHAR findPath[ MAX_PATH ];
			_stprintf_s( findPath, _T("%s\\*.%s"), dir, plginExp[ i ] );


			/**** �w�肳�ꂽ�v���O�C���f�B���N�g���𑖍� ****/
			WIN32_FIND_DATA findData;
			HANDLE hFind = FindFirstFile( findPath, &findData );
			if( hFind != INVALID_HANDLE_VALUE )
			{
				do
				{
					// �v���O�C���p�X��ݒ�
					PluginLibInfo set = { _T(""), NULL, NULL };
					_stprintf_s( set.path, _T("%s\\%s"), dir, findData.cFileName );

					// ���C�u������ǂݍ���
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


		/**** �������t���O�𗧂Ă� ****/
		m_Initialized = true;
	}


	return m_Initialized;
}

/**
* �v���O�C���Ǘ�������������Ă��邩
*	@return		bool					����������Ă���Ȃ�ΐ^�A����ȊO�͋U
*/
bool OePlugin::IsInitialized( void )
{
	return m_Initialized;
}

/**
* �v���O�C���Ǘ��̔p��
*/
void OePlugin::Finalize( void )
{
	if( m_Initialized )
	{
		/**** �ǂݍ��ݍς݂̃v���O�C�����J�� ****/
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


		/**** �������t���O���~�낷 ****/
		m_Initialized = false;
	}
}

/**
* �v���O�C���Ǘ������X�V����
*/
void OePlugin::Update( void )
{
}

/**
* �S�v���O�C��������������
*/
void OePlugin::AllPluginInitialize( void )
{
	/**** �v���O�C���̏��������[�`�����R�[�� ****/
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
* �v���O�C�����j���[��o�^����
*/
void OePlugin::AppendPluginMenu( void )
{
	/**** ���j���[�ɒǉ����� ****/
	createPluginMenu();
	appendPluginMenu();

	/**** �v���V�[�W�����T�u�N���X�� ****/
	if( (WNDPROC)GetWindowLong( ghMainWnd, GWL_WNDPROC ) != (WNDPROC)mainPluginMenuProc )
	{
		m_OldMainProc = (WNDPROC)GetWindowLong( ghMainWnd, GWL_WNDPROC );
		SetWindowLong( ghMainWnd, GWL_WNDPROC, (LONG)mainPluginMenuProc );
	}
}

/**
* �v���O�C���ꗗ���𓾂�
*	@return		PluginSetList&			�v���O�C���Z�b�g���X�g�ւ̎Q��
*/
OePlugin::PluginSetList& OePlugin::GetPluginSetList( void )
{
	return m_PluginSetList;
}


//-- �v���O�C������
/**
* OneTime�v���O�C���𓮍삳����
*	@param[in]	nMenuID					����Ώۂ̃��j���[ID
*	@return		bool					�v���O�C������������^�A�\���ȊO�͋U
*/
bool OePlugin::runOnetimePlugin( UINT nMenuID )
{
	bool ret = false;

	PluginSetList::iterator it;
	for( it = m_PluginSetList.begin(); it != m_PluginSetList.end(); it++ )
	{
		if( it->pPluginsetInfo->count > 0 )
		{// �v���O�C������
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
