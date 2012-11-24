/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C�� - �ҏW����Ǘ��N���X
*
*---------------------------------------------------------------------------------*/
#include "PluginEditControl.h"
#include <string.h>


/*-- �n���h���擾�p�}�N�� --*/
#define GET_HANDLE( type, elem ) case OE_##type: ret = m_HostWindowHandle.elem ? *m_HostWindowHandle.elem : NULL; break


namespace oe { namespace plugin {


/**** �ҏW����Ǘ��N���X ****/
/**
* �R���X�g���N�^
*/
OePluginEditControl::OePluginEditControl( void )
{
	_initializeHostHandle();
}

/**
* �E�B���h�E�n���h�����擾����
*	@param[in]	type					�擾����E�B���h�E�̎��
*	@return		HWND					�E�B���h�E�n���h���A���s����NULL
*/
HWND OePluginEditControl::GetWindowHandle( WindowType type )
{
	HWND ret = NULL;

	// �E�B���h�E�n���h���擾
	switch( type )
	{
	GET_HANDLE( WT_MAINWINDOW, hMainWnd );
	}

	// �E�B���h�E�n���h���`�F�b�N
	if( ret && !::IsWindow( ret ) ) ret = NULL;

	return ret;
}

/**
* ���j���[�n���h�����擾����
*	@param[in]	type					�擾���郁�j���[�̎��
*	@return		HMENU					���j���[�n���h���A���s����NULL
*/
HMENU OePluginEditControl::GetMenuHandle( MenuType type )
{
	HMENU ret = NULL;

	// �E�B���h�E�n���h���擾
	switch( type )
	{
	GET_HANDLE( MT_MAINMENU, hMainMenu );
	}

	return ret;
}

/**
* �e��p�X���擾����
*	@param[in]	type					�擾����p�X�̎��
*	@param[out]	path					�p�X�i�[��
*	@param[in]	length					path�Ɋi�[�\�ȕ�����
*	@return		int						�i�[�����������A���݂��Ȃ��ꍇ��-1
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
* �I�[�v���ς݂̃t�@�C�������擾����
*	@return		unsigned long			�I�[�v���ς݂̃t�@�C����
*/
unsigned long OePluginEditControl::GetFileCount( void )
{
	return _getFileCount();
}

/**
* �w�肳�ꂽ�ԍ��̃t�@�C���p�X���擾����
*	@param[in]	index					�t�@�C���C���f�b�N�X�ԍ�
*	@param[out]	path					�t�@�C���p�X�i�[��
*	@param[in]	length					path�Ɋi�[�\�ȕ�����
*	@return		int						�i�[�����������A���݂��Ȃ��ꍇ��-1
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
* �t�@�C����ǉ��œǂݍ���
*	@param[in]	path					�ǉ�����t�@�C���p�X
*	@return		bool					����������^�A����ȊO�͋U
*/
bool OePluginEditControl::InsertFile( LPCWSTR path )
{
	(void)path;
	return false;
}

/**
* �w��̃t�@�C�����폜����
*	@param[in]	index					�폜����t�@�C���C���f�b�N�X
*	@return		bool					�폜�ɐ���������^�A����ȊO�͋U
*/
bool OePluginEditControl::EraseFile( unsigned long index )
{
	(void)index;
	return false;
}


} }
