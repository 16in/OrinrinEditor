/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C�� - �ҏW����Ǘ��N���X
*								protected �֐��Q
*
*---------------------------------------------------------------------------------*/
#include "..\\stdafx.h"
#include "..\\OrinrinEditor.h"
#include "PluginEditControl.h"
#include <string.h>
#include "PluginExternVariables.h"


namespace oe { namespace plugin {


/**** �ҏW����Ǘ��N���X - protected �֐��Q ****/
/**
* HostHandle��������
*/
void OePluginEditControl::_initializeHostHandle( void )
{
#define SET_HOST( var, elem ) m_HostWindowHandle.elem = &var

	//-- �E�B���h�E�n���h��
	SET_HOST( ghMainWnd, hMainWnd );

	//-- ���j���[�n���h��
	SET_HOST( ghMenu, hMainMenu );
}

/**
* �w�肳�ꂽ��ނ̃t�@�C���p�X���擾
*	@param[in]	type					�p�X�̎��
*	@return		const wchar_t*			�t�@�C���p�X
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
* �t�@�C�����擾
*	@return		unsigned long			�t�@�C����
*/
unsigned long OePluginEditControl::_getFileCount( void )
{
	return (unsigned long)gltMultiFiles.size();
}

/**
* �w�肳�ꂽ�t�@�C���p�X���擾
*	@param[in]	index					�t�@�C���C���f�b�N�X�ԍ�
*	@return		const wchar_t*			�t�@�C���p�X
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
