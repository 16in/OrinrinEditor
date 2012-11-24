/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグインで共通で使用する外部定義変数宣言
*
*---------------------------------------------------------------------------------*/
#pragma once

#include "..\\OrinrinEditor.h"


/**** 外部定義変数 ****/
//-- ウィンドウハンドル
extern  HWND				ghMainWnd;		// メインウインドウハンドル

//-- メニューハンドル
extern  HMENU				ghMenu;			// メニュー

//-- 操作対象オブジェクト類
extern std::list<ONEFILE>	gltMultiFiles;	// 複数ファイル保持


//-- パス関係
extern TCHAR				gatExePath[MAX_PATH];	// 実行ファイルの位置
extern TCHAR				gatIniPath[MAX_PATH];	// ＩＮＩファイルの位置
