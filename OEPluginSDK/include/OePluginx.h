/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C��SDK �e������N���X��`
*
*---------------------------------------------------------------------------------*/
#pragma once

#include "OePlugin.h"


namespace oe { namespace plugin {


/*---------------------------------------------------
* �N���X��`
*/
/**
* ���j���[�A�C�e���v���O�C�����N���X
*/
class MenuItemPlugin : public IPlugin
{
public:
	// �p���̕K�v���K�������Ȃ�����
	virtual bool Initialize( LPEDITCONTROL pEditCtrl ){ (void)pEditCtrl; return true; }
	virtual void Finalize( LPEDITCONTROL pEditCtrl ){ (void)pEditCtrl; }
};


} }
