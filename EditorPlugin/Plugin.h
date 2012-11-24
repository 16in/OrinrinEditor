/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C���Ǘ��N���X
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
* �v���O�C���Ǘ��N���X
*/
class OePlugin
{
public:
	//! �v���O�C�����擾�֐��^
	typedef LPPLUGINSETINFO (__stdcall *GetPluginsetInfo)( void );

	//! �v���O�C���Z�b�g���z��
	struct PluginLibInfo
	{
		TCHAR					path[ MAX_PATH ];				//!< �v���O�C���ւ̃t���p�X
		HMODULE 				hDll;							//!< DLL�n���h��
		GetPluginsetInfo		getInfoFunc;					//!< �v���O�C�����擾�֐�
		UINT					nMenuID[ MAX_PLUGIN_COUNT ];	//!< �v���O�C���Ɋ֘A�Â��Ă��郁�j���[ID
		LPPLUGINSETINFO			pPluginsetInfo;					//!< �v���O�C���Z�b�g�ւ̃|�C���^
	};
	typedef std::vector<PluginLibInfo>	PluginSetList;

protected:
	//-- �v���O�C���Ǘ����
	static bool					m_Initialized;					//!< �������ς݂ł��邩
	static PluginSetList		m_PluginSetList;				//!< �v���O�C���Z�b�g���X�g
	static OePluginEditControl	m_PluginEditControl;			//!< �v���O�C���ɓn���ҏW�⏕�N���X

	//-- �v���O�C�����j���[
	static HMENU				m_hPluginMenu;					//!< �v���O�C�����j���[

	//-- �T�u�N���X��
	static WNDPROC				m_OldMainProc;					//!< �T�u�N���X����Ă΂�郁�C���E�B���h�E�v���V�[�W��


public:
	/**
	* �v���O�C���Ǘ��̏�����
	*	@param[in]	dir						�v���O�C���f�B���N�g���p�X
	*	@return		bool					�������ɐ���������^�A����ȊO�͋U
	*/
	static bool Initialize( const LPCTSTR dir = _T("Plugin") );

	/**
	* �v���O�C���Ǘ�������������Ă��邩
	*	@return		bool					����������Ă���Ȃ�ΐ^�A����ȊO�͋U
	*/
	static bool IsInitialized( void );

	/**
	* �v���O�C���Ǘ��̔p��
	*/
	static void Finalize( void );

	/**
	* �v���O�C���Ǘ������X�V����
	*/
	static void Update( void );

	/**
	* �S�v���O�C��������������
	*/
	static void AllPluginInitialize( void );

	/**
	* �v���O�C�����j���[��o�^����
	*/
	static void AppendPluginMenu( void );

	/**
	* �v���O�C���ꗗ���𓾂�
	*	@return		PluginSetList&			�v���O�C���Z�b�g���X�g�ւ̎Q��
	*/
	static PluginSetList& GetPluginSetList( void );


protected:
	//-- �v���O�C������
	/**
	* OneTime�v���O�C���𓮍삳����
	*	@param[in]	nMenuID					����Ώۂ̃��j���[ID
	*	@return		bool					�v���O�C������������^�A�\���ȊO�͋U
	*/
	static bool runOnetimePlugin( UINT nMenuID );

	//-- ���j���[�֌W
	/**
	* �v���O�C�����j���[�𐶐�����
	*/
	static void createPluginMenu( void );

	/**
	* �z�X�g�̃��j���[�Ƀv���O�C�����j���[��ڑ�����
	*/
	static void appendPluginMenu( void );

	/**
	* �v���O�C�����j���[�t�b�N�p�v���V�[�W��
	*/
	static LRESULT CALLBACK mainPluginMenuProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};


} }
