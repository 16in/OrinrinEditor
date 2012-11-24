/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C�� - �ҏW����Ǘ��N���X
*
*---------------------------------------------------------------------------------*/
#pragma once

#include <tchar.h>
#include <Windows.h>
#include <OePlugin.h>


namespace oe { namespace plugin {


/**
* �ҏW����Ǘ��N���X
*/
class OePluginEditControl : public IEditControl
{
public:
	//! �E�B���h�E�n���h���i�[�p�N���X
	struct HostHandleSet
	{
		//-- �E�B���h�E�n���h��
		HWND*			hMainWnd;				//!< ���C���E�B���h�E

		//-- ���j���[�n���h��
		HMENU*			hMainMenu;				//!< ���C�����j���[
	};


protected:
	HostHandleSet		m_HostWindowHandle;		//!< �z�X�g�v���O�����̃E�B���h�E�n���h��


public:
	/**
	* �R���X�g���N�^
	*/
	OePluginEditControl( void );

	/**
	* �E�B���h�E�n���h�����擾����
	*	@param[in]	type					�擾����E�B���h�E�̎��
	*	@return		HWND					�E�B���h�E�n���h���A���s����NULL
	*/
	virtual HWND GetWindowHandle( WindowType type );

	/**
	* ���j���[�n���h�����擾����
	*	@param[in]	type					�擾���郁�j���[�̎��
	*	@return		HMENU					���j���[�n���h���A���s����NULL
	*/
	virtual HMENU GetMenuHandle( MenuType type );

	/**
	* �e��p�X���擾����
	*	@param[in]	type					�擾����p�X�̎��
	*	@param[out]	path					�p�X�i�[��
	*	@param[in]	length					path�Ɋi�[�\�ȕ�����
	*	@return		int						�i�[�����������A���݂��Ȃ��ꍇ��-1
	*/
	virtual int GetPath( PathType type, wchar_t* path, unsigned long length );

	/**
	* �I�[�v���ς݂̃t�@�C�������擾����
	*	@return		unsigned long			�I�[�v���ς݂̃t�@�C����
	*/
	virtual unsigned long GetFileCount( void );

	/**
	* �w�肳�ꂽ�ԍ��̃t�@�C���p�X���擾����
	*	@param[in]	index					�t�@�C���C���f�b�N�X�ԍ�
	*	@param[out]	path					�t�@�C���p�X�i�[��
	*	@param[in]	length					path�Ɋi�[�\�ȕ�����
	*	@return		int						�i�[�����������A���݂��Ȃ��ꍇ��-1
	*/
	virtual int GetFileName( unsigned long index, wchar_t* path, unsigned long length );

	/**
	* �t�@�C����ǉ��œǂݍ���
	*	@param[in]	path					�ǉ�����t�@�C���p�X
	*	@return		bool					����������^�A����ȊO�͋U
	*/
	virtual bool InsertFile( LPCWSTR path );

	/**
	* �w��̃t�@�C�����폜����
	*	@param[in]	index					�폜����t�@�C���C���f�b�N�X
	*	@return		bool					�폜�ɐ���������^�A����ȊO�͋U
	*/
	virtual bool EraseFile( unsigned long index );


protected:
	//----------------------------------
	// �����݂̂ŌĂяo�����֐��Q
	//
	/**
	* HostHandle��������
	*/
	virtual void _initializeHostHandle( void );

	/**
	* �w�肳�ꂽ��ނ̃t�@�C���p�X���擾
	*	@param[in]	type					�p�X�̎��
	*	@return		const wchar_t*			�t�@�C���p�X
	*/
	virtual const wchar_t* _getPath( PathType type );

	/**
	* �t�@�C�����擾
	*	@return		unsigned long			�t�@�C����
	*/
	virtual unsigned long _getFileCount( void );

	/**
	* �w�肳�ꂽ�t�@�C���p�X���擾
	*	@param[in]	index					�t�@�C���C���f�b�N�X�ԍ�
	*	@return		const wchar_t*			�t�@�C���p�X
	*/
	virtual const wchar_t* _getFileName( unsigned long index );
};


} }
