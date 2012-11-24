/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグインSDK 共通ヘッダ
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
* プラグインセットの情報を得る
*	@return		LPPLUGINSETINFO			プラグインセットへのポインタ
*/
DLLEXPORT LPPLUGINSETINFO __stdcall OePlugin_GetPluginSetInfo( void );


} }

}
