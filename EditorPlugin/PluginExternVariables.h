/*---------------------------------------------------------------------------------
*
* OrinrinEditor �v���O�C���ŋ��ʂŎg�p����O����`�ϐ��錾
*
*---------------------------------------------------------------------------------*/
#pragma once

#include "..\\OrinrinEditor.h"


/**** �O����`�ϐ� ****/
//-- �E�B���h�E�n���h��
extern  HWND				ghMainWnd;		// ���C���E�C���h�E�n���h��

//-- ���j���[�n���h��
extern  HMENU				ghMenu;			// ���j���[

//-- ����ΏۃI�u�W�F�N�g��
extern std::list<ONEFILE>	gltMultiFiles;	// �����t�@�C���ێ�


//-- �p�X�֌W
extern TCHAR				gatExePath[MAX_PATH];	// ���s�t�@�C���̈ʒu
extern TCHAR				gatIniPath[MAX_PATH];	// �h�m�h�t�@�C���̈ʒu
