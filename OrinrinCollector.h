#pragma once

#include "resource.h"
//-------------------------------------------------------------------------------------------------

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾
ATOM	InitWndwClass( HINSTANCE );
BOOL	InitInstance( HINSTANCE, INT );
LRESULT	CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

BOOLEAN	Cls_OnCreate( HWND, LPCREATESTRUCT );		//!<	�{�̂� WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Cls_OnCommand( HWND , INT, HWND, UINT );	//!<	�{�̂� WM_COMMAND �̏���
VOID	Cls_OnPaint( HWND );						//!<	�{�̂� WM_PAINT �̏����E�g���`��Ƃ�
VOID	Cls_OnDestroy( HWND );						//!<	�{�̂� WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
HBRUSH	Cls_OnCtlColor( HWND , HDC, HWND, INT );	//!<	
VOID	Cls_OnHotKey( HWND, INT, UINT, UINT );		//!<	
VOID	Cls_OnDrawClipboard( HWND );				//!<	
VOID	Cls_OnChangeCBChain( HWND, HWND, HWND );	//!<	

VOID		WndTagSet( HWND, LONG_PTR );			//!<	
LONG_PTR	WndTagGet( HWND );						//!<	

BOOLEAN	SelectFileDlg( HWND, LPTSTR, UINT_PTR );	//!<	

HRESULT	ToolTipSetting( HWND, UINT, LPTSTR );		//!<	

HRESULT	TasktrayIconAdd( HWND );					//!<	
VOID	TaskTrayIconEvent( HWND, UINT, UINT );		//!<	
HRESULT	TaskTrayIconCaptionChange( HWND );			//!<	
HRESULT	TaskTrayIconBalloon( HWND, LPTSTR, LPTSTR, DWORD );	//!<	

INT		InitParamValue( UINT, UINT, INT );			//!<	
HRESULT	InitClipStealOpen( UINT, UINT, LPTSTR );	//!<	
HRESULT	InitSettingSave( HWND );					//!<	

HRESULT	FileListViewInit( HWND );				//!<	
INT		FileListViewAdd( HWND, LPTSTR );		//!<	
HRESULT	FileListViewGet( HWND, INT, LPTSTR );	//!<	
INT		FileListViewDelete( HWND  );			//!<	

BOOLEAN	FileTypeCheck( LPTSTR );			//!<	

HMENU	CreateFileSelMenu( HWND, UINT );	//!<	

HRESULT	ClipStealDoing( HWND  );		//!<	
LPTSTR	ClipboardDataGet( LPVOID  );	//!<	

LPTSTR	SjisDecodeAlloc( LPSTR );		//!<	
LPSTR	SjisEncodeAlloc( LPTSTR );		//!<	
LPSTR	SjisEntityExchange( LPCSTR );	//!<	
TCHAR	UniRefCheck( LPSTR );			//!<	
