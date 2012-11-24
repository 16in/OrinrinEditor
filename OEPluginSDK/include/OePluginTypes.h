/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C��SDK �^��`
*
*---------------------------------------------------------------------------------*/
#pragma once

#include <Windows.h>


namespace oe { namespace plugin {


/*---------------------------------------------------
* �萔��`
*/
const int MAX_PLUGIN_NAME = 64;				//!< �v���O�C�����ő啶����
const int MAX_PLUGIN_COUNT = 32;			//!< �v���O�C���Z�b�g�Ɋ܂܂��v���O�C���ő吔


/*---------------------------------------------------
* �񋓑̒�`
*/
/**
* OE�v���O�C�� ���
*/
enum PluginType
{
	OE_PT_NONE				= 0x00000000,			//!< �������A�p���ȊO�ɌĂяo����܂���
	OE_PT_MENUITEM			= 0x00000001,			//!< ���j���[�ɒǉ����A���ڂ̑I���ŌĂяo�����悤�ɂ��܂�

	OE_PLUGINTYPE_FULLBIT	= 0xffffffff			//!< ���ׂẴt���O�𗧂Ă�
};

/**
* OE�E�B���h�E�擾�p���
*/
enum WindowType
{
	OE_WT_MAINWINDOW = 0x00,						//!< ���C���E�B���h�E

	OE_WINDOWTYPE_MAX,								//!< �ő�l
	OE_WINDOWTYPE_UNKNOWN = 0xffffffff				//!< �s���ȃE�B���h�E
};

/**
* OE���j���[�擾�p���
*/
enum MenuType
{
	OE_MT_MAINMENU = 0x00,							//!< ���C�����j���[

	OE_MENUTYPE_MAX,								//!< �ő�l
	OE_MENUTYPE_UNKNOWN = 0xffffffff				//!< �s���ȃE�B���h�E
};

/**
* OE�p�X�擾�p���
*/
enum PathType
{
	OE_PT_ORINRINEDITOR = 0x00,						//!< OrinrinEditor.exe�ւ̃p�X
	OE_PT_INIFILE,									//!< .ini�t�@�C���ւ̃p�X

	OE_PATHTYPE_MAX,								//!< �ő�l
	OE_PATHTYPE_UNKNOWN = 0xffffffff				//!< �s���ȃE�B���h�E
};


/*---------------------------------------------------
* �C���^�[�t�F�C�X��`
*/
/**
* �v���O�C���֓n�����G�f�B�^����C���^�[�t�F�C�X
*/
typedef class IEditControl
{
public:
	/**
	* �E�B���h�E�n���h�����擾����
	*	@param[in]	type					�擾����E�B���h�E�̎��
	*	@return		HWND					�E�B���h�E�n���h���A���s����NULL
	*/
	virtual HWND GetWindowHandle( WindowType type ) = 0;

	/**
	* ���j���[�n���h�����擾����
	*	@param[in]	type					�擾���郁�j���[�̎��
	*	@return		HMENU					���j���[�n���h���A���s����NULL
	*/
	virtual HMENU GetMenuHandle( MenuType type ) = 0;

	/**
	* �e��p�X���擾����
	*	@param[in]	type					�擾����p�X�̎��
	*	@param[out]	path					�p�X�i�[��
	*	@param[in]	length					path�Ɋi�[�\�ȕ�����
	*	@return		int						�i�[�����������A���݂��Ȃ��ꍇ��-1
	*/
	virtual int GetPath( PathType type, wchar_t* path, unsigned long length ) = 0;

	/**
	* �I�[�v���ς݂̃t�@�C�������擾����
	*	@return		unsigned long			�I�[�v���ς݂̃t�@�C����
	*/
	virtual unsigned long GetFileCount( void ) = 0;

	/**
	* �w�肳�ꂽ�ԍ��̃t�@�C���p�X���擾����
	*	@param[in]	index					�t�@�C���C���f�b�N�X�ԍ�
	*	@param[out]	path					�t�@�C���p�X�i�[��
	*	@param[in]	length					path�Ɋi�[�\�ȕ�����
	*	@return		int						�i�[�����������A���݂��Ȃ��ꍇ��-1
	*/
	virtual int GetFileName( unsigned long index, wchar_t* path, unsigned long length ) = 0;

	/**
	* �t�@�C����ǉ��œǂݍ���
	*	@param[in]	path					�ǉ�����t�@�C���p�X
	*	@return		bool					����������^�A����ȊO�͋U
	*/
	virtual bool InsertFile( LPCWSTR path ) = 0;

	/**
	* �w��̃t�@�C�����폜����
	*	@param[in]	index					�폜����t�@�C���C���f�b�N�X
	*	@return		bool					�폜�ɐ���������^�A����ȊO�͋U
	*/
	virtual bool EraseFile( unsigned long index ) = 0;
} EDITCONTROL, *PEDITCONTROL, *LPEDITCONTROL;

/**
* �v���O�C��
*	���̃N���X���p�������C���X�^���X��
*	�z�X�g�֕Ԃ��悤�ɂ��Ă��������B
*/
typedef class IPlugin
{
public:
	/**
	* �v���O�C���̓ǂݍ��ݎ��ɃR�[������܂�
	*	@param[in]	pEditCtrl				�ҏW����C���^�[�t�F�C�X
	*	@return		bool					�������ɐ��������ꍇ�͐^�A����ȊO�͋U
	*/
	virtual bool Initialize( LPEDITCONTROL pEditCtrl ) = 0;

	/**
	* �v���O�C���̔p�����ɃR�[������܂�
	*	@param[in]	pEditCtrl				�ҏW����C���^�[�t�F�C�X
	*/
	virtual void Finalize( LPEDITCONTROL pEditCtrl ) = 0;

	/**
	* ���j���[�őI�����ꂽ�ۂɃR�[������܂�
	*	@param[in]	pEditCtrl				�ҏW����C���^�[�t�F�C�X
	*/
	virtual void Run( LPEDITCONTROL pEditCtrl ) = 0;
} PLUGIN, *PPLUGIN, *LPPLUGIN;


/*---------------------------------------------------
* �\���̒�`
*/
/**
* �v���O�C�����\����
*/
typedef struct PluginInfo
{
	wchar_t				pluginName[ MAX_PLUGIN_NAME ];		//!< �v���O�C����
	unsigned long		type;								//!< �v���O�C�����
	plugin::LPPLUGIN	plugin;								//!< �v���O�C���|�C���^
} PLUGININFO, *PPLUGININFO, *LPPLUGININFO;

/**
* �v���O�C���Z�b�g���\����
*/
typedef struct PluginSetInfo
{
	wchar_t				pluginsetName[ MAX_PLUGIN_NAME ];	//!< �v���O�C���Z�b�g��
	unsigned long		count;								//!< �v���O�C����
	PluginInfo			pluginInfo[ MAX_PLUGIN_COUNT ];		//!< �X�̃v���O�C�����
} PLUGINSETINFO, *PPLUGINSETINFO, *LPPLUGINSETINFO;


} }
