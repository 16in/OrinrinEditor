//	SplitBar.h

#pragma once

//-------------------------------------------------------------------------------------------------

#define SPLITBAR_CLASS		TEXT("CSplitBar")	//	�X�v���b�g�o�[�N���X��
#define SPLITBAR_WIDTH		4	//	�X�v���b�g�o�[�̕�
#define SPLITBAR_LEFTLIMIT	120	//	�X�v���b�g�o�[�̍��ʒu���~�b�g
//-------------------------------------------------------------------------------------------------

ATOM	SplitBarClass( HINSTANCE );
HWND	SplitBarCreate( HINSTANCE, HWND, INT, INT, INT );
VOID	SplitBarPosGet( HWND, LPRECT );
VOID	SplitBarResize( HWND, LPRECT );
