/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグインSDK 各動作基底クラス定義
*
*---------------------------------------------------------------------------------*/
#pragma once

#include "OePlugin.h"


namespace oe { namespace plugin {


/*---------------------------------------------------
* クラス定義
*/
/**
* メニューアイテムプラグイン基底クラス
*/
class MenuItemPlugin : public IPlugin
{
public:
	// 継承の必要が必ずしもないもの
	virtual bool Initialize( LPEDITCONTROL pEditCtrl ){ (void)pEditCtrl; return true; }
	virtual void Finalize( LPEDITCONTROL pEditCtrl ){ (void)pEditCtrl; }
};


} }
