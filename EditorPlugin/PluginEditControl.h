/*---------------------------------------------------------------------------------
*
* OrinrinEditor プラグイン - 編集操作管理クラス
*
*---------------------------------------------------------------------------------*/
#pragma once

#include <tchar.h>
#include <Windows.h>
#include <OePlugin.h>


namespace oe { namespace plugin {


/**
* 編集操作管理クラス
*/
class OePluginEditControl : public IEditControl
{
public:
	//! ウィンドウハンドル格納用クラス
	struct HostHandleSet
	{
		//-- ウィンドウハンドル
		HWND*			hMainWnd;				//!< メインウィンドウ

		//-- メニューハンドル
		HMENU*			hMainMenu;				//!< メインメニュー
	};


protected:
	HostHandleSet		m_HostWindowHandle;		//!< ホストプログラムのウィンドウハンドル


public:
	/**
	* コンストラクタ
	*/
	OePluginEditControl( void );

	/**
	* ウィンドウハンドルを取得する
	*	@param[in]	type					取得するウィンドウの種別
	*	@return		HWND					ウィンドウハンドル、失敗時はNULL
	*/
	virtual HWND GetWindowHandle( WindowType type );

	/**
	* メニューハンドルを取得する
	*	@param[in]	type					取得するメニューの種別
	*	@return		HMENU					メニューハンドル、失敗時はNULL
	*/
	virtual HMENU GetMenuHandle( MenuType type );

	/**
	* 各種パスを取得する
	*	@param[in]	type					取得するパスの種別
	*	@param[out]	path					パス格納先
	*	@param[in]	length					pathに格納可能な文字数
	*	@return		int						格納した文字数、存在しない場合は-1
	*/
	virtual int GetPath( PathType type, wchar_t* path, unsigned long length );

	/**
	* オープン済みのファイル数を取得する
	*	@return		unsigned long			オープン済みのファイル数
	*/
	virtual unsigned long GetFileCount( void );

	/**
	* 指定された番号のファイルパスを取得する
	*	@param[in]	index					ファイルインデックス番号
	*	@param[out]	path					ファイルパス格納先
	*	@param[in]	length					pathに格納可能な文字数
	*	@return		int						格納した文字数、存在しない場合は-1
	*/
	virtual int GetFileName( unsigned long index, wchar_t* path, unsigned long length );

	/**
	* ファイルを追加で読み込む
	*	@param[in]	path					追加するファイルパス
	*	@return		bool					成功したら真、それ以外は偽
	*/
	virtual bool InsertFile( LPCWSTR path );

	/**
	* 指定のファイルを削除する
	*	@param[in]	index					削除するファイルインデックス
	*	@return		bool					削除に成功したら真、それ以外は偽
	*/
	virtual bool EraseFile( unsigned long index );


protected:
	//----------------------------------
	// 内部のみで呼び出される関数群
	//
	/**
	* HostHandleを初期化
	*/
	virtual void _initializeHostHandle( void );

	/**
	* 指定された種類のファイルパスを取得
	*	@param[in]	type					パスの種類
	*	@return		const wchar_t*			ファイルパス
	*/
	virtual const wchar_t* _getPath( PathType type );

	/**
	* ファイル数取得
	*	@return		unsigned long			ファイル数
	*/
	virtual unsigned long _getFileCount( void );

	/**
	* 指定されたファイルパスを取得
	*	@param[in]	index					ファイルインデックス番号
	*	@return		const wchar_t*			ファイルパス
	*/
	virtual const wchar_t* _getFileName( unsigned long index );
};


} }
