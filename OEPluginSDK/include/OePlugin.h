/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C��SDK ���ʃw�b�_
*
*---------------------------------------------------------------------------------*/
#pragma once

#include "OePluginTypes.h"

#ifndef OE_PLUGIN_IMPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif


extern "C" {

namespace oe { namespace plugin {


/**
* �v���O�C���Z�b�g�̏��𓾂�
*	@return		LPPLUGINSETINFO			�v���O�C���Z�b�g�ւ̃|�C���^
*/
DLLEXPORT LPPLUGINSETINFO __stdcall OePlugin_GetPluginSetInfo( void );


} }

}
