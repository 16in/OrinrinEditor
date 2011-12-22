/*! @file
	@brief �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
	���̃t�@�C���� OrinrinEditor.cpp �ł��B
	@author	SikigamiHNQ
	@date	2011/05/10
*/

/*
Orinrin Editor : AsciiArt Story Editor for Japanese Only
Copyright (C) 2011 Orinrin/SikigamiHNQ

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see <http://www.gnu.org/licenses/>.
*/
//-------------------------------------------------------------------------------------------------

//	����{�鍑���p��́�������ꂽ��



//	TODO:	�ő剻���ďI��遄�N�����āA�ő剻��������ƁA���C���X�v���b�g�o�[�������Ȃ��Ȃ�
//�E�ɃC�b������Ă�܂�

//	TODO:	�f�J���t�@�C���J�����玞�Ԃ�����i1000�R�}�Ƃ��j
//�����ǂݍ��݂ɂ���Ƃ��A��ǂ݂�����vector�m�ۂ��Ă��炢���Ƃ�
//vector����Ȃ�list�ɂ���Ƃ��H
//��s���̃T�C�Y�m�F���āA�m�ۂ��Ă���u�`���ނƂ�

//	TODO:	�����t�@�C���^�u�A�}�E�X�I�[�o�[�Ńt���p�X�\��
//�E�N�����j���[�Ńt�@�C�����o��

//	TODO:	�����s�e���v�����̎�^�u�Ńz�C�[���X�N���[���������Ȃ�
//���C�����̕ҏW�X�y�[�X�APage List�A�����s�e���v������AA�ꗗ�c���[�ȂǁA���̏ꏊ�ł͖��Ȃ�

//	TODO:	ASD�Ή����Ǎ��͍�������ǂ��Ŏg���H


//	TODO:	�f�J���t�@�C���J�����Ƃ����Ƃ��̓��b�Z�[�W�o���Ƃ�

//	TODO:	MAA�̃t�@�C���������A�t�H���_�������o����悤�ɁH�J���Ƃ��́H

//	TODO:	SQL��INSERT�Aprepare�͏d���B�N�G�������āAreset���Ȃ���񂷂̂������񂶂�

//	TODO:	�u��`�I���v�Ɓu�������o�v�̗����Ƀ`�F�b�N����ꂽ��ԂŃh���b�O����Ɨ�����

//	TODO:	�ŕ���������e�ʕ\������������
//	TODO:	�y�[�W���������A���s�̐��Ƃ����������H

//	TODO:	�o�b�N�A�b�v�@�\�̋����E�㏑���ۑ�������o�b�N�A�b�v�Ƃ��E�ŐV�K�쐬������ۑ�

//	TODO:	�ۑ�����Ƃ��A�����t�@�C������������A�����������ăo�b�N�A�b�v�Ƃ�

//	TODO:	�䎌�pEnter�@�\�BShift+Enter�Ƃ��ŁA���s�}���ł͂Ȃ��A���̍s�̖��[����̒��O��
//�󔒂̈ʒu�����͂̊J�n�n�_�܂ŁA���̍s�ɋ󔒂����āA�����ɃJ�[�\���ړ��B�䎌�}���Ɏg����B
//���s���Ȃ���΁A���s���ċ󔒁B�����̕�����Ɉ���������Ȃ�A�J�[�\���ړ��̂�

//	TODO:	MLT�̃u�b�N�}�[�N�@�\�E�^�u���₷���A�c���[�ɑ��₷���E���^�u���Ⴂ���Ȃ��H


//	TODO:	MAA�����g�p����I�v�V���b

//	TODO:	�g�@�\�ŁA�����s�p�[�c���g������

//	TODO:	�Ń��X�g�A���O�ɑI�����Ă����ł��D�F�ɂ��Ă����@�\

//	TODO:	���C���X�v���b�g�o�[�̈ʒu��񃊃Z�b�g�����邩��

//	TODO:	�t�@�C�����e���v�@�\���~�����B�S�o�C�g���A�Ő��Ƃ�

//	TODO:	Ctrl+K�̓��v�@�\�EPageList�̍��ڑ��₵�đΏ��ECtrl+K�Ń��t���b�V���Ƃ�
//	�A�����p�Ƃ��A�s���󔒂Ƃ��E�����ȏ��������E�������^�C�����Əd���̂ő���Ń��t���b�V��

//	TODO:	�g�}��������A�E�����ɋ󂭁B���j�R�g���Ƃ��́A�����J���͕s�v��

//	TODO:	�g�ҏW�̃T���v�������������Ƃ�������E�`�惋�[�`��������

//	TODO:	�g���X�@�\�A�_�ŃR���g���X�g�H�ƋP�x�����̕��������̂����傢�Ƌꂵ���炵��
//	TODO:	�g���X���[�h���ɁA�O���b�hON/OFF��t����
//	TODO:	�g���X���[�h�A�e���͒l�͒����͂ł���悤��
//	TODO:	�g���X���[�h�{�^���́ATOGGLE�ł���悤�ɂ���
//	TODO:	�g���X�̂P��ʁA�Q��ʐ؂�ւ����[�h
//	TODO:	�g���X�̂Q��ʂ��ƁA�w�i�G�͂ǂ����̃y�C���ł��\���o����悤�ɂ�����
//	TODO:	�g���X�ŁA�摜���܂�Œ��ڈړ��ł���悤��

//	TODO:	�ʒu���̃��Z�b�g�@�\�E�V�X�e�����j���[�ɓ��ڂ���

//	TODO:	�g�p�̃A���A��MLT�ɓ���AA���L������n�b�V�����������Ă��ƂŃ������Ă܂��B�ǂ����悤

//	TODO:	�c�����@�\�E�s�Ԓ����@�\�Ƃ�

//	TODO:	�c�[���`�b�v�t������

//	TODO:	�����̌����E�s���@�\
//���j�R�[�h�󔒈ꊇ�����Ƃ�

//	TODO:	�E����������


//	TODO:	�؂蔲�����t�@�����XAA���g���āA�����̂Ƃ������؂蔲���A�Ȃ��Ƃ��낾���؂蔲���A�Ƃ��ł���悤�ɂ�����

//	TODO:	�o�b�N�A�b�v���b�Z�[�W�A�X�e�[�^�X�o�[�ɏo���Ȃ���
//	TODO:	�S���ۑ����
//	TODO:	ALT+N���A�Ŗ��̈ꊇ�ҏW�ɂ���H

//	TODO:	�_�u���N���b�N��Space�G���A�Ƃ��̕����I��

//	TODO:	�t�@�C�����J�������@�\

//	TODO:	DocBadSpaceCheck���̍X�V�w�߂̍œK�����K�v

//	TODO:	�e���v���́u��ɍőO�ʂɕ\���vOrinrinediter��\�����Ă��鎞�����őO�ʂɗ���悤��
//�������́A���C�����ɓ����Ƃ�

//	TODO:	�g�p�������t�@�C���o�͂ł���悤��


//	TODO:	���o�[�́A��ԃ��Z�b�g���K�v

//	TODO:	�悭�g�����j�R�[�h��Z�߂Ă����Ȃ����A���[�U��`�ł����񂶂��

//	TODO:	�e���v���̃R���{�b�N�X���A���X�g�{�b�N�X�^�Ɛ؂�ւ�����Ƃ�

//	TODO:	�łɁA�w�b�_�t�b�^�}���@�\�B�w�b�_�ɂ͕Ŕԍ��A�t�b�^�ɂ͗e�ʁA�Ƃ�

//	TODO:	�t�@�C���^�u��[�ύX]���d���H�ύX������A���ڈȍ~�͏����Ȃ��悤��
//�݂Ă�t�@�C���ς����Ƃ��̃X�e�[�^�X�o�[�̏��������ɒ���

//	TODO:	�N�����ɁA���C���e���v���[�g�̃��X�g���J�����ēǂݍ��݂ł���悤�ɂ��ė~����
//	Brush���������B�E�N�����j���[��







//�O���X�N���v�g�͂ǂ̂悤�Ɏ������邩�BRuby�̑g���Ƃ������Ȃ��H
//(�t)Edit��FreePascal�H



//	���j���[�̃R���O����
//        MENUITEM SEPARATOR
//        MENUITEM "�h��ׂ��i�u���V�j���[�h(&M)\tCtrl + T",  IDM_BRUSH_STYLE
//    "T",            IDM_BRUSH_STYLE,        VIRTKEY, CONTROL, NOINVERT



//	�X�N���[�������������ꍇ�A��ʑS�̏����K�v��


//	TODO:	MLT/AST�́A�T���l�C���\���E�^�C�����ۂ������ŁB��������I���ł���
//�g�\�������āA�t�H�[�J�X�����������
//50x50�̃^�C�����A5x3�ɂȂ�ׂ�B�t�H���g���ł�낵�E�t�H�[�J�X�Ń|�b�v�A�b�v�Ƃ�
//�ł���AA�͒����t�H���g�ł������邩�B�����̂���͋C�ɂ��Ȃ��E�T���l����
//����̃L�[�^�b�v�ŗǂ����낤�E�c�X�N���[���o�[���t����B���̂Ƃ��A�N�e�B�u��
//�Ȃ��Ă���^�u�̓��e�Ƃ��A�c���[�ŃJ�[�\���̉��ɂ����Ƃ�

//	�c�[���`�b�v�̃|�b�v�f�B���C�́ATTM_SETDELAYTIME �Œ����o����
//	wParam	TTDT_INITIAL	�\���܂ł̎���
//	lParam	The LOWORD specifies the delay time, in milliseconds. The HIWORD must be zero.
//�ς���Ă�悤�Ɏv���Ȃ��B�Ȃ񂩊ԈႦ�Ă邩


//	�t�@�C����ǂݍ��ނƃ����^�C���G���[�ł邱�Ƃ�����E�Ȃ񂩕ύX���Ă���H


//OK?
//	TODO:	�ő剻�ŏI�������Ƃ��́A�ő剻��Ԃ��o���Ă�����������
//	TODO:	���^�u��I��������ԂŌ����ŐV�����t�@�C�����J���ƁA��̃^�u���X�g�͕��^�u���I�����ꂽ�܂�
//�u�S�āv�ɖ߂������e�؂���
//	TODO:	MAA�v���t�@�C���A�\�z�_�C�����O�ŁA���X�g�A�b�v������A�o�^�ς݂̂�Ƀ`�F�L���Ă���
//	TODO:	Safari�̃��[�f�B���O���X�g�݂����ȋ@�\�B
//	TODO:	MAA���e���T���l�\�����邱�Ƃ́H
//	TODO:	�T���l�̉E���j���[�ɂ����̂�����B�J���Ƃ��ɒ�����
//	TODO:	�T���l�\�����璼�ڃh���t�g�{�[�h�ɒǉ��ł���悤��
//	TODO:	�F��GUI�ŕύX�ł���悤��
//	TODO:	���C�Ƀ��X�g�A�O���[�v���ꊇ�폜����@�\
//	TODO:	�h���t�g��T���l�AAA�T�C�Y�\���o����悤�ɂ�����
//	TODO:	�t�@�C���؂�ւ��Ă��A�J�[�\���ʒu�o���Ă���
//	TODO:	�N�����ɁA�����Ƃ��̃t�@�C���𕜌����邩�ǂ����I���ł���悤��


//	TODO:	�u���V���s�e���v���A�}�E�X�I�[�o�[�c�[���`�b�v�ŁA�����h�b�g���\����������

//�R�s�[���[�h����ւ��A�N�����̏����ƁACtrl+C�̕\�L
//���C���{�b�N�X�ŁASJIS�R�s�[�����璆�g�����̂��C��

//	���o�[�́A�u�ҏW�v�Ƃ��̐��������N���b�N����ƃX���C�h����@�\
//	�_�u���N���b�N�ňړ�����悤�ɂ����B�ő剻�ƍŏ����炵��

/*

ASD�t�@�C���@�@��s����R���e���c

���e�̉��s�@�@�@�@�@0x01,0x01
���e�Ɛ����̋�؂�@0x02,0x02

*/

/*
2011/06/30	0.10	�������[�X�A�v���A���t�@��
2011/07/04	0.11	�I�𒆂ɉE�N������Ɖ������ꂿ�Ⴄ�̂��C��
					�S�I�����ăR�s�[�����Ƃ��̖��[���s���B���C��
					���C���{�b�N�X�ŁA�ҏW�r���[�̃X�N���[�����l������ĂȂ������̂��C��
					MAA�ŕ����t�@�C�����^�u�ŊJ������悤�ɂ���
2011/07/19	0.12	�g�̕ێ����P�O�ɂ���
					�I�����̕��^�u�𕜋A�o����悤�ɂ���
					�V�K�ۑ��⃊�l�[���ۑ����ƁA���X�g�I�[�|�����L�^����Ȃ��̂��C��
					PageList�ŁA�폜�E�ړ��ł���悤�ɂ���
2011/07/20	0.13	MLT�c���[�ŁAAST���ǂ߂�悤�ɂ���
					�u���V�A��s�e���v���̃R���{�b�N�X�̓}�E�X�z�C�[�����Ȃ��悤�ɂ���
					�I����ԉ�������Ă邯�ǃt���O����������ĂȂ��̂��C����������
					��`�I��͈͂��A�J�[�\���ʒu�ɏ�����悤�ɂȂ����H
					�؂���̃L�[�{�[�h�A�N�Z�����[�^�����������E�P�Ȃ����Y��
					�O�����̍s��Alt�{�������Ƃ��ځ[�񂵂Ă��̂��C��
					MAA�e���v���ŁA�c�[���`�b�v��ON/OFF�o����悤�ɂ���
					�g�ɖ��O�t������悤�ɂ���
					�g���X�@�\�����E�������P���K�v
					�I��͈͂𔽓]�\�������Ⴄ
					�e���v���t�@�C�����ATemplates�f�B���N�g���Ɉړ�
2011/08/01	0.14	�R���e�L�X�g���j���[�ɂ��A�N�Z�����[�^�L�[��t����
					�g���X�F�\����\���� Alt+I �ŏo����悤�ɂ�����
					�g���X�F��\�����͕����F�߂��悤�ɂ���
					�g���X�F����̑��삵����t�H�[�J�X���`��r���[�ɖ߂�悤�ɂ���
					�u���V�ƈ�s�e���v���̕\�����i�q��ɂ����E�ł��I���̐F�ς����w��
					MAA�̃c�[���`�b�v�����T�C�Y���X�ƂP�Q�őI���o����悤�ɂ���
					���C���{�b�N�X�A�\��t�����炷�����邩�ǂ����̑I���`�F�b�N����
					���j�R�[�h�p�f�B���O���g����悤�ɂ���
2011/08/09	0.15	�g���X���̓^�C�g���o�[��[�g���X���[�h]����
					�g���X�̕\����\����Alt+H�ɕύX
					�g���X�ŁA�G���J������Y�[�����P�O�O���ɖ߂��悤�ɂ���
					�g���X�̉摜�J�����ɃT���l�C���t����
					�ňꗗ�Ƀc�[���o�[�t����
					�ł̕����@�\������
					Alt���E�̏ꍇ�ɂ����j�R�[�h�����ăp�f�B���O����悤�ɂ���
					Ctrl+Home,End�������悤�ɂ���
					MAA���Ƀt�H�[�J�X������Ƃ��ATAB,Shift+TAB�Ń^�u��؂�ւ��悤�ɂ���
					MAA�̕��^�u���폜�E�ǉ�������I�������������Ȃ��Ă�̂��C��
					���j�R�[�h���l�Q�ƁA�P�U�i���^�ɑΉ�
					���C���{�b�N�X�A�u�b���ޓ��e�ɍ��킹�āA���̑傫�����g�傷��悤�ɂ���
					�����o�b�N�A�b�v�@�\��t����
2011/08/18	0.16	�L���v�V�����o�[�̊J���Ă�t�@�C�������ύX����Ȃ���������̂��C��
					���̎Q�Ƃ̓Ǎ�������悤�ɂ����B�����������Ń��j�R�[�h�ɂ��Ă�
					�t�@�C���J�����Ƃ��ɁA�y�[�W���X�g�Ƀo�C�g���Ƃ������f����Ȃ��̂��C��
					���C�������A����邱�Ƃ��������̂��C��
					���C���{�b�N�X�̓��ߍ������o����悤�ɂ���
					�N���b�v�{�[�h���e�L�X�g�̃��j�R�[�h���ʏo����悤�ɂ���
					�c�[���`�b�v���AWin7���ƃ`�����̂����Ȃ��悤�ɂł����H
					�y�[�W���X�g�ɁA�e�ł̃T���l�C�����ۂ��̂��|�b�p�b�v����悤�ɂ���
					MAA�E�C���h�E��AA�\�����A���j�R�[�h�ɕϊ����ĕ\������悤�ɂ����E���Ȃ��Ǝv������
					���C���{�b�N�X�̃T�C�Y�ύX�����������̂𒼂���
					AAViewer�����𕪗����ĒP��Project��������悤�ɂ���
2011/08/24	0.17	�o�b�N�A�b�v�f�B���N�g������ɍ쐬����悤�ɂ���
					�o�b�N�A�b�v�������b�Z�[�W��ON/OFF�o����悤�ɂ����E�~�X�͏�ɕ\��
					�ł�V�K�쐬�����Ƃ����o�b�N�A�b�v����悤�ɂ���
					�o�C�g���v�Z�̍�蒼���E�������m�ɏo����悤�ɂȂ���
					ALT�㉺�ŕŐ؊��ł���
					�h���b�O�I�������₷������
					�ł̓����ƕ������o����悤�ɂ���
					���^�u�́A�Ō�Ɍ��Ă����ʒu���o���Ă�����悤�ɂ����E�I�������������
2011/08/31	0.18	�����t�@�C���I�[�|���o����悤�ɂȂ����͂��E���v������
					Ctrl+F4 �Ń^�u���ł���
					ALT�{�����ŕňړ�������APageList���X�N���[������悤�ɂ���
					�c�[���o�[�Ɂu��蒼���v��ǉ�
					�󔒂̕\���E��\����؂�ւ�����悤�ɂ���
					�I��͈͂��A�󔒂��u���V�œh��Ԃ���悤�ɂ���
					�e�Ɨ������A�őO�ʕ\���o����悤�ɂ���
					�������o�ł���悤�ɂȂ�������
					�J�����g�f�B���N�g�����AEXE�t�@�C���̂���ꏊ�Ŏ��悤�ɂ���
					�o�O�C�����낢��
2011/09/06	0.19	�R�}���h���C������̃t�@�C���I�[�|���ɑΉ������H�ŏ��̈����
					�󔒌x������������A�s�ԍ����g���Ȃ�悤�ɂ���
					�ŏI�ō폜�����琁����Ԃ̂��C��
					�O���b�h����\���o����悤�ɂ���
					�E�[���[����\���ł���悤�ɂ���
					�w�i�F���ύX�ł���悤�ɂ����B�ł�GUI���Ȃ�
					���̕ł̑O�ɑ}���A���A��ɑ}���ɂ����E�����쐬�̃L�[�o�C���h�ύX
					�E�N�����j���[�F�X�ǉ�
					�}�E�X�z�C�[���̃X�N���[���ʂ��n�r�W�����g���悤�ɂ����E����������
					PageUp,PageDn�łP�O�s���X�N���[������悤�ɂ���
					�s��������Əd���Ȃ�̂��Ȃ��悤�ɂ����E�����C�P�Ă�
					�s���������U���܂őΉ�
					�����t�@�C���Ԃ̈ړ����ACtrl+PageUp,Down�ňړ��ł���悤�ɂ���
					IE�R���|�[�l���g���g�����ȈՃv���r���[����
					Ctrl+Home,End�ŁA�X�N���[������肭�����Ȃ��̂��C��
					�ŏ��Ԉړ��̏㉺���t�������̂��C��
					�Ń��X�g�A�w�b�_�̕��ύX���Ă����X�g�̂ق����ς��Ȃ��̂��C���E���v���ȁH
					�o�O�C�����낢��
2011/09/16	0.20	���C���{�b�N�X�̓��e���N���b�v�{�[�h�ł���悤�ɂ���
					�����s�e���v������ҏW�r���[�ɊJ������悤�ɂ���
					�v���r���[�@�\���A<DD>�Ƃ��g���Ď��ۂ̉�ʂ��ۂ��Ȃ�悤�ɂ���
					��ʂ�ALL�v���r���[���Ă����Ȃ��Ȃ���������
					�����x���@�\�Ƃ��āA�ő���SJIS�R�s�[�@�\��ǉ��B�z�b�g�L�[[Ctrl+Shift+D]���g�p�B
					���s�}�[�N[CrLfMark]�A��SJIS����[CantSjis]�̐F�ύX�o����悤�ɂ���
					�c�[���o�[�����o�[�ɂ����B�ł��ύX�̕������ł��Ă˂�
					�E�N�����j���[�̘g�̖��̂��\�������悤�ɂ���
					���o����Alt+Space����������A���o���C���{�b�N�X�J���ł�����
					����ɔ����A���o���ă��C���{�b�N�X�֋@�\�͎����
					���o����UNICODE�R�s�[���o����悤�ɂȂ�������
					��`�E�h�ׁE���o�E�󔒂̏�Ԃ��X�e�[�^�X�o�[�ɕ\��
					���[�󔒍폜���Ă��A�s���\�����Ԃ��܂܂������̂��C��
					�����Ă�ł��ABMP��PNG�ŕۑ��o����悤�ɂ���
2011/09/30	0.21	�X�y�[�X���\���ɂ��Ă��A�x���͕\������悤�ɂ���
					�g���X���[�h�̊G�̏�ɁA�O���b�h���o���悤�ɂ���
					�c�[���o�[�A���j���[�́A�g���X���[�h�E���j�R�[�h�\��TOGGLE����ɂ���
					���j�R�[�h�\�Ń_�u���N���b�N������}�������悤�ɂ���
					�ŕ����E�����E�폜�̊m�F���b�Z�[�W�ɁA������\�����Ȃ��I�v�V����
					�J�[�\���ʒu���o����悤�ɂ����E���܂����Ȃ���
					�S�̂̃h�b�g�V�t�g�@�\�E�I��͈͂̂ݏ������ł���
					�����s�e���v���̋�؂���A���ƐF�Ⴂ��؂�ւ�����悤�ɂ���
					�����s�e���v���ŁA�v���t�@�C���@�\�Ő؂�ւ�����悤�ɂ���
					�����s�e���v���v���t�@�C���ɁA�c���[���e���L���b�V������
					�����s�e���v���̎g�p���X�g�̕��ނ��ڂ�������
					�����s�e���v���ŁA�g�p���X�g���^�u�ŊJ����悤�ɂ���
					Viewer���o�[�W�����A�b�v
2011/10/20	0.22	�����̊g���q���AAST�Ȃ炻����ێ�����悤�ɂ���
					�g���X���[�h�A�X���C�_���X�N���[���o�[�ɂ��Ă݂�
					�ҏW�G���A�̃X�N���[���ɍ��킹�ĊG�������悤�ɂ���
					�������������x��������ꂽ������
					�u���V�E��s�e���v���ŁA�e�A�C�e���̉������c�[���`�b�v�ɕ\���E�ł��Ȃ񂩃w��
					�I�����ē��́��폜�A��`�I�����h��ׂ��A����悤�ɂł���������
					�g�}�������쐬
					���C���{�b�N�X�A�g�}�������A�J�[�\���L�[�ňٓ��o����悤�ɂ���
					���e�ύX������A�t�@�C���^�u��[�ύX]
					�őS�̂��󔒂Ŗ��߂�@�\
					�Ŕԍ��}���@�\
					��s�e���v���A�u���V�e���v���A�Ń��X�g�����C�����ɂ�������@�\
					�����s�e���v���̃^�u���A�����i�\���o����悤�ɂ���
					�o�O�C��������
2011/11/07	0.23	�I��͈͂̃o�C�g���\������悤�ɂ���
					�g�}�����ɁA�}�����������X�C�b�`�𓋍�
					�����s�e���v������̊����}�����A�����Ƌ�`�}���Ȃ�悤�ɂ���
					���C���{�b�N�X�J���Ƃ��A�J�[�\���ʒu�ɊJ���悤�ɂ�����
					�����s�e���v���Ɍ��o���@�\��t�����BAST�ƍs���́y�z�ɔ���
					�����s�e���v���̃c���[�̃t�@�C���ւ́A���e�ǉ��ł���悤�ɂ�����
					�u���V����Ƃ��ɁA�h�b�g���͂ݏo���Ă��C�ɂ��Ȃ��悤�ɂ���
					�R�s�[���[�h���AUnicode��ShiftJIS�����ւ�����悤�ɂ���
					�����`�`�ϊ��@�\�o��������
					�o�O�C��������
2011/11/11	0.24	�R���e�L�X�g���j���[�ҏW�@�\�𓋍ڂł������H
					Ctrl�{�����ŁA�e���v���O���[�v�̕ő���
					�����s�e���v���̌��o���@�\�������B�}�V�ɂȂ������H
					�o�O�C�����낢��
2011/11/14	0.24.1	�����`�`�ϊ������A��ʊO�ɍs���Ȃ��悤�ɂ���
					���d�N���h�~���ꂽ
					�v���r���[�E�C���h�E�̈ʒu�Ƒ傫�����o���Ă����悤�ɂ���
					�g�}�������Ƃ��ɉ��̕����X�V����Ȃ��̂��C��
					�o�O�C�����낢��
2011/11/28	0.25	MAA�̃t�@�C���������@�\
					MAA�̃c���[�W�J�������Ȃ����C������iViewer���݁j
					�v���t�@�C���\�z�ŁA�����̃v���t�ƈ�v����Ȃ�`�F�L����悤�ɂ����iViewer���݁j
					�v���t�@�C�����Ƃ��̎��Ԃ��Z�k�ł����C������iViewer���݁j
					���C�����̃e���v���G���A�̃T�C�Y�ςɂȂ���
					�ő剻��Ԃ��o���Ă����悤�ɂ���
					4096�o�C�g��������A�Ń��X�g�̃o�C�g���̂Ƃ���Ԃ�����悤�ɂ���
2011/12/22	0.26	�h���t�g�{�[�h�@�\�iViewer���݁j
					MAA�T���l�C���@�\�iViewer���݁j
					MAA�̎g�p�̃O���[�v�ꊇ�폜�iViewer���݁j
					�z�F�ύX���f�t�h�ŏo����悤�ɂ���
					�I�[�|���L�^���Ȃ���΁A����AST��\������悤�ɂ���
					�t�@�C������Caret�ʒu��ۑ�����悤�ɂ���
					�N�������܂܁A��s�e���v���A�u���V�e���v���̃����[�h�ł���悤�񂵂�
					�N������Ƃ��A�Ō�ɊJ���Ă��t�@�C�����J���Ƃ��I���ł���悤�ɂ���

�X�V��������

�y�[�W���X�g�́A�N���b�N���Ă��t�H�[�J�X�ڂ�Ȃ��悤�ɂ���


	���o�[�̕ύX�̕���

*/

/*
IME���̃Q�b�g

	HKL	hKl;
	TCHAR	atImeName[MAX_STRING];
	hKl = GetKeyboardLayout( 0 );
	ImmGetDescription( hKl, atImeName, MAX_STRING );
*/

//-------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "OrinrinEditor.h"
//-------------------------------------------------------------------------------------------------

static CONST TCHAR	gcatLicense[] = {
TEXT("���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F�A���c�ɂ���Ĕ��s���ꂽGNU��ʌ��O���p������(�o�[�W����3���A����ȍ~�̃o�[�W�����̂����ǂꂩ)����߂�����̉��ōĔЕz�܂��͉��ς��邱�Ƃ��ł��܂��B\r\n\r\n")
TEXT("���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�*�ł��B���Ɖ\���̕ۏ؂����ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂��܂߁A�S�����݂��܂���B\r\n\r\n")
TEXT("�ڂ�����GNU��ʌ��O���p���������������������B\r\n\r\n")
TEXT("���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU��ʌ��O���p�������̃R�s�[���ꕔ�󂯎���Ă���͂��ł��B\r\n\r\n")
TEXT("�����󂯎���Ă��Ȃ���΁A<http://www.gnu.org/licenses/> ���������������B\r\n\r\n")
};

// �O���[�o���ϐ�:
static  HINSTANCE	ghInst;						//!<	���݂̃C���^�[�t�F�C�X
static  TCHAR		gszTitle[MAX_STRING];		//!<	�^�C�g���o�[�̃e�L�X�g
static  TCHAR		gszWindowClass[MAX_STRING];	//!<	���C���E�B���h�E�N���X��

static  HMENU	ghMenu;			//!<	���j���[

static   HWND	ghFileTabWnd;	//!<	�����t�@�C���^�u

static   HWND	ghMainWnd;		
static   HWND	ghStsBarWnd;	//!<	�X�e�[�^�X�o�[

static HANDLE	ghMutex;		//!<	���d�N���h�~�pMutex

EXTERNED HFONT	ghNameFont;		//!<	�t�@�C���^�u�p�t�H���g

EXTERNED HWND	ghMaaWnd;		//!<	�����s�`�`�e���v��
EXTERNED HWND	ghPgVwWnd;		//!<	�y�[�W���X�g
EXTERNED HWND	ghLnTmplWnd;	//!<	��s�e���v��
EXTERNED HWND	ghBrTmplWnd;	//!<	�u���V�e���v��

EXTERNED HWND	ghMainSplitWnd;	//!<	���C���̃X�v���b�g�o�[�n���h��
EXTERNED LONG	grdSplitPos;	//!<	�X�v���b�g�o�[�́A�����́A��ʉE����̃I�t�Z�b�g

EXTERNED UINT	gbUniPad;		//!<	�p�f�B���O�Ƀ��j�R�[�h�������āA�h�b�g�������Ȃ��悤�ɂ���
EXTERNED UINT	gbUniRadixHex;	//!<	���j�R�[�h���l�Q�Ƃ��P�U�i���ł��邩

static   UINT	gdBUInterval;	//!<	�o�b�N�A�b�v���o�E�f�t�H�R�����炢�H
EXTERNED UINT	gbAutoBUmsg;	//!<	�����o�b�N�A�b�v���b�Z�[�W�o�����H
EXTERNED UINT	gbCrLfCode;		//!<	���s�R�[�h�F�O������΁E��O�x�x 

//@@�R�s�[����
EXTERNED UINT	gbCpModSwap;	//!<	SJIS�ƃ��j�R�[�h�R�s�[�����ւ���

static TCHAR	gatExePath[MAX_PATH];	//!<	���s�t�@�C���̈ʒu
static TCHAR	gatIniPath[MAX_PATH];	//!<	�h�m�h�t�@�C���̈ʒu

EXTERNED INT	gbTmpltDock;	//!<	�e���v���̃h�b�L���O

#ifdef OPEN_HISTORY
static vector<OPENHIST>	gvcOpenHist;	//!<	�t�@�C���J��������
#endif

#ifdef FIND_STRINGS
extern  HWND	ghFindDlg;		//	�����_�C�����O�̃n���h��
#endif
extern  HWND	ghMoziWnd;		//	�����`�`�ϊ��_�C�����O�̃n���h��

extern  HWND	ghMaaFindDlg;	//	MAA�����_�C�����O�n���h��

#ifdef DRAUGHT_STYLE
extern  UINT	gdClickMode;	//
#endif

extern  HWND	ghViewWnd;		//	�r���[

extern  UINT	gdGridXpos;		//	�O���b�h���̂w�Ԋu
extern  UINT	gdGridYpos;		//	�O���b�h���̂x�Ԋu
extern  UINT	gdRightRuler;	//	�E���̈ʒu
//-------------------------------------------------------------------------------------------------

//	�X�e�[�^�X�o�[�̋�؂�
#define SB_ITEMS	8
CONST INT	gadStsBarSize[] = { 50, 200, 350, 500, 650, 750, 850, -1 };
//-------------------------------------------------------------------------------------------------

VOID	Cls_OnActivate( HWND, UINT, HWND, BOOL );	//!<	
BOOLEAN	Cls_OnCreate( HWND, LPCREATESTRUCT );		//!<	�{�̂� WM_CREATE �̏����E�Œ�Edit�Ƃ�����
VOID	Cls_OnCommand( HWND , INT, HWND, UINT );	//!<	�{�̂� WM_COMMAND �̏���
VOID	Cls_OnPaint( HWND );						//!<	�{�̂� WM_PAINT �̏����E�g���`��Ƃ�
VOID	Cls_OnSize( HWND , UINT, INT, INT );		//!<	
VOID	Cls_OnMove( HWND, INT, INT );				//!<	
VOID	Cls_OnDestroy( HWND );						//!<	�{�̂� WM_DESTROY �̏����EBRUSH�Ƃ��̃I�u�W�F�N�g�̔j���Y��Ȃ��悤��
LRESULT	Cls_OnNotify( HWND , INT, LPNMHDR );		//!<	
VOID	Cls_OnTimer( HWND, UINT );					//!<	
VOID	Cls_OnDropFiles( HWND , HDROP );			//!<	
VOID	Cls_OnContextMenu(HWND,HWND,UINT,UINT );	//!<	
VOID	Cls_OnHotKey(HWND, INT, UINT, UINT );		//!<	

INT_PTR	CALLBACK OptionDlgProc( HWND, UINT, WPARAM, LPARAM );	//!<	
//-------------------------------------------------------------------------------------------------


/*!
	�A�v���P�[�V�����̃G���g���|�C���g
	@param[in]	hInstance		���̃��W���[���̃C���X�^���X�n���h��
	@param[in]	hPrevInstance	�O�̃C���X�^���X�B���͖��g�p
	@param[in]	lpCmdLine		�R�}���h���C���B�g�[�N�������͂���ĂȂ��A�����̕�����
	@param[in]	nCmdShow		�N�����̕\����Ԃ������Ă�B�\���Ƃ�����������
	@retval FALSE	�r���I��
*/
INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HACCEL	hAccelTable;
	INT		msRslt;
	MSG		msg;

	INT		iArgc;
	TCHAR	atArgv[MAX_PATH];
	LPTSTR	*pptArgs;

	INT		iCode;

#ifdef _DEBUG
	//_CRTDBG_ALLOC_MEM_DF;		//	�w�肪�K�v�ȃt���O
	//_CRTDBG_CHECK_ALWAYS_DF;	//	���������`�F�b�N		_CRTDBG_CHECK_EVERY_128_DF
	//_CRTDBG_LEAK_CHECK_DF;		//	�I�����Ƀ��������[�N���`�F�b�N
	//_CRTDBG_DELAY_FREE_MEM_DF;	//	
	//	�����Ŏg�p����t���O���w��
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc( 1979 );
	//	���l�̓����������ԍ��B���[�N���Ă���A���̕\���̂Ƃ����
	//	�����Ă��邩��A���̊֐��Ő��l��錾���Ă��������s����ƁA
	//	���[�N����m�ۂ����Ă�Ƃ���Ŏ~�܂�
	//	���\���̕������璍��
#endif

	//	���d�N���h�~
	ghMutex = CreateMutex( NULL , TRUE, TEXT("OrinrinEditor") );	//	���łɋN�����Ă��邩����
	if( GetLastError() == ERROR_ALREADY_EXISTS )	//	���łɋN�����Ă���
	{
		MessageBox( NULL, TEXT("���łɋN�����Ă���̂ł��B��������"), TEXT("���d�N���͏o���Ȃ��̂ł�"), MB_OK|MB_ICONINFORMATION );
		ReleaseMutex( ghMutex );
		CloseHandle( ghMutex );
		return 0;
	}

	//	�R�}���h���C���������m�F�E�O�͎��s�t�@�C�����A�P�ȍ~�Ɉ��������Ă�
	ZeroMemory( atArgv, sizeof(atArgv) );
	pptArgs = CommandLineToArgvW( GetCommandLine( ), &iArgc );
	//	�ŏ��̈�ڂ����L��
	if( 2 <= iArgc ){	StringCchCopy( atArgv, MAX_PATH, pptArgs[1] );	}
	LocalFree( pptArgs );



	INITCOMMONCONTROLSEX	iccex;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC  = ICC_WIN95_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES;
	InitCommonControlsEx( &iccex );

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, gszTitle, MAX_STRING);
	LoadString(hInstance, IDC_ORINRINEDITOR, gszWindowClass, MAX_STRING);
	InitWndwClass( hInstance );

	//	�ݒ�t�@�C���ʒu�m�F
	GetModuleFileName( hInstance, gatExePath, MAX_PATH );
	PathRemoveFileSpec( gatExePath );

	StringCchCopy( gatIniPath, MAX_PATH, gatExePath );
	PathAppend( gatIniPath, INI_FILE );

	DocBackupDirectoryInit( gatExePath );
	FrameInitialise( gatExePath, hInstance );
	MoziInitialise( gatExePath, hInstance );

	gbUniPad = 0;

	iCode = InitParamValue( INIT_LOAD, VL_CLASHCOVER, 0 );
	if( iCode )
	{
		iCode = MessageBox( NULL, TEXT("�G�f�B�^���������I���o���Ȃ������C�z�������B\r\n�o�b�N�A�b�v���c���Ă��邩������Ȃ�����A��Ɋm�F���Ă݂āI\r\n���̂܂܋N�����Ă����������H�@�u�������v��I�ԂƁA�����ŏI�������B"), TEXT("���߂��"), MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2 );
		if( IDNO == iCode ){	return 0;	}
	}

	InitParamValue( INIT_SAVE, VL_CLASHCOVER, 1 );

	// �A�v���P�[�V�����̏����������s���܂�:
	if( !InitInstance( hInstance, nCmdShow , atArgv ) ){	return FALSE;	}

	CntxEditInitialise( gatExePath, hInstance );

	RegisterHotKey( ghMainWnd, IDHK_THREAD_DROP, MOD_CONTROL | MOD_SHIFT, VK_D );

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ORINRINEDITOR));

	//	���C�����b�Z�[�W���[�v
	for(;;)
	{
		msRslt = GetMessage( &msg, NULL, 0, 0 );
		if( 1 != msRslt )	break;

#ifdef FIND_STRINGS	//	�����񌟍��_�C�����O
		if( ghFindDlg )
		{	//�g�b�v�ɗ��Ă邩�ǂ������f����
			if( ghFindDlg == GetForegroundWindow(  ) )
			{
				if( TranslateAccelerator( ghFindDlg, hAccelTable, &msg ) )	continue;
				if( IsDialogMessage( ghFindDlg, &msg ) )	continue;
			}
		}
#endif
		//	MAA�����_�C�����O
		if( ghMaaFindDlg )
		{	//�g�b�v�ɗ��Ă邩�ǂ������f����
			if( ghMaaFindDlg == GetForegroundWindow(  ) )
			{
				if( TranslateAccelerator( ghMaaFindDlg, hAccelTable, &msg ) )	continue;
				if( IsDialogMessage( ghMaaFindDlg, &msg ) )	continue;
			}
		}

		if( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	InitParamValue( INIT_SAVE, VL_CLASHCOVER, 0 );

	UnregisterHotKey( ghMainWnd, IDHK_THREAD_DROP );

	ReleaseMutex( ghMutex );
	CloseHandle( ghMutex );

	return (int)msg.wParam;
}
//-------------------------------------------------------------------------------------------------

LPTSTR ExePathGet( VOID )
{
	return gatExePath;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�N���X����
	���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
	Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
	�A�v���P�[�V�������A�֘A�t����ꂽ�������`���̏������A�C�R����
	�擾�ł���悤�ɂ���ɂ́A���̊֐����Ăяo���Ă��������B
	@param[in]	hInstance	���̃��W���[���̃C���X�^���X�n���h��
	@return �o�^�����N���X�A�g��
*/
ATOM InitWndwClass( HINSTANCE hInstance )
{
	WNDCLASSEX	wcex;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORINRINEDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ORINRINEDITOR);
	wcex.lpszClassName	= gszWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//-------------------------------------------------------------------------------------------------

/*!
	�C���X�^���X�n���h����ۑ����āA���C���E�B���h�E���쐬���܂��B
	���̔����ŁA�O���[�o���ϐ��ŃC���X�^���X�n���h����ۑ����A
	���C���v���O�����E�B���h�E���쐬����ѕ\�����܂��B
	@param[in]	hInstance	�C���X�^���X�n���h��
	@param[in]	nCmdShow	�N�����̕\�����
	@param[in]	ptArgv		�R�}���h���C���œn���ꂽ�t�@�C�����E������΂O�N������ԁENULL�ł͂Ȃ�
*/
BOOL InitInstance( HINSTANCE hInstance, INT nCmdShow, LPTSTR ptArgv )
{
	HWND	hWnd;
	RECT	rect, wnRect;
	INT		isMaxim = 0, sptBuf = PLIST_DOCK;
//	DWORD	dwStyle;

#ifdef MAA_PROFILE
	HMENU	hSubMenu;
#endif

	ghInst = hInstance;	//	�O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

	SplitBarClass( hInstance );	//	�X�v���b�g�o�[�̏���

	isMaxim = InitParamValue( INIT_LOAD, VL_MAXIMISED, 0 );

	InitWindowPos( INIT_LOAD, WDP_MVIEW, &rect );
	if( 0 == rect.right || 0 == rect.bottom )	//	���������O�̓f�[�^����
	{
		hWnd = GetDesktopWindow( );
		GetWindowRect( hWnd, &rect );
		rect.left   = ( rect.right  - W_WIDTH ) / 2;
		rect.top    = ( rect.bottom - W_HEIGHT ) / 2;
		rect.right  = W_WIDTH;
		rect.bottom = W_HEIGHT;
		InitWindowPos( INIT_SAVE, WDP_MVIEW, &rect );
	}

	hWnd = CreateWindowEx( 0, gszWindowClass, gszTitle, WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right, rect.bottom, NULL, NULL, hInstance, NULL);

	if( !hWnd ){	return FALSE;	}

	gbUniPad      = InitParamValue( INIT_LOAD, VL_USE_UNICODE,  1 );	//	���j�R�󔒁@�P�g���@�O�g��Ȃ�

	gbUniRadixHex = InitParamValue( INIT_LOAD, VL_UNIRADIX_HEX, 1 );

	gdBUInterval  = InitParamValue( INIT_LOAD, VL_BACKUP_INTVL, 3 );
	gbAutoBUmsg   = InitParamValue( INIT_LOAD, VL_BACKUP_MSGON, 1 );	//	
	gbCrLfCode    = InitParamValue( INIT_LOAD, VL_CRLF_CODE, 0 );		//	�O������΁@�PYY

	gbTmpltDock   = InitParamValue( INIT_LOAD, VL_PLS_LN_DOCK,  1 );	//	�O�Ɨ��@�P��������

	gbCpModSwap   = InitParamValue( INIT_LOAD, VL_SWAP_COPY, 0 );		//	�O���j�R�[�h�@�PSJIS


	ghMainWnd = hWnd;

	//	�@�\�`�F�b�N
	//hSubMenu = GetSystemMenu( hWnd, FALSE );
	//DeleteMenu( hSubMenu, 1, MF_BYPOSITION );
	//InsertMenu( hSubMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, 0 );

	ghMenu = GetMenu( hWnd );
#ifdef MAA_PROFILE
	//	���j���[�Ƀc���[�\�z�͂���Ȃ��̂ŃJ�b�g
	hSubMenu = GetSubMenu( ghMenu, 0 );
	DeleteMenu( hSubMenu, IDM_TREE_RECONSTRUCT, MF_BYCOMMAND );
#endif

	hSubMenu = GetSubMenu( ghMenu, 1 );	//	�ҏW
	if( gbCpModSwap )
	{
		ModifyMenu( hSubMenu, IDM_COPY,     MF_BYCOMMAND | MFT_STRING, IDM_COPY,     TEXT("SJIS�R�s�|(&C)\tCtrl + C") );
		ModifyMenu( hSubMenu, IDM_SJISCOPY, MF_BYCOMMAND | MFT_STRING, IDM_SJISCOPY, TEXT("Unicode�R�s�|(&J)") );
	}
#ifndef FIND_STRINGS
	DeleteMenu( hSubMenu, IDM_FIND_DLG_OPEN, MF_BYCOMMAND );
	DeleteMenu( hSubMenu, IDM_FIND_HIGHLIGHT_OFF, MF_BYCOMMAND );
	DeleteMenu( hSubMenu, 19, MF_BYPOSITION );	//	�폜���Ԓ���
#endif
#ifndef DRAUGHT_STYLE
	DeleteMenu( hSubMenu, IDM_COPY_TO_DRAUGHT, MF_BYCOMMAND );
#endif

	hSubMenu = GetSubMenu( ghMenu, 4 );	//	�\��
	if( gbTmpltDock )
	{
		DeleteMenu( hSubMenu, IDM_PAGELIST_VIEW, MF_BYCOMMAND );
		DeleteMenu( hSubMenu, IDM_LINE_TEMPLATE, MF_BYCOMMAND );
		DeleteMenu( hSubMenu, IDM_BRUSH_PALETTE, MF_BYCOMMAND );
	}
#ifndef DRAUGHT_STYLE
	DeleteMenu( hSubMenu, IDM_DRAUGHT_ADDING, MF_BYCOMMAND );
#endif



	AaFontCreate( 1 );


	AppClientAreaCalc( &rect  );	//	

	SqnSetting(  );

	if( gbTmpltDock )
	{
		//	�ݒ肩��X�v���b�g�o�[�̈ʒu����������
		grdSplitPos = InitParamValue( INIT_LOAD, VL_MAIN_SPLIT, PLIST_DOCK );
		if( grdSplitPos < SPLITBAR_WIDTH || rect.right <= grdSplitPos ){	grdSplitPos = PLIST_DOCK;	}
		sptBuf = grdSplitPos;

		ghMainSplitWnd = SplitBarCreate( hInstance, hWnd, rect.right - grdSplitPos, rect.top, rect.bottom );
	}
	else{	ghMainSplitWnd = NULL;	grdSplitPos = 0;	}


	ghPgVwWnd = PageListInitialise( hInstance, hWnd, &rect );

	ghLnTmplWnd = LineTmpleInitialise( hInstance, hWnd, &rect );

	ViewInitialise( hInstance, hWnd, &rect, ptArgv );
	ViewMaaItemsModeSet( InitParamValue( INIT_LOAD, VL_SETMETHOD, MAA_INSERT ) );

	LayerBoxInitialise( hInstance, &rect );
	LayerBoxAlphaSet( InitParamValue( INIT_LOAD, VL_LAYER_TRANS, 192 ) );

	UserDefInitialise( hWnd, TRUE );

	PreviewInitialise( hInstance, hWnd );

	FrameNameModifyMenu( hWnd );

	TraceInitialise( hWnd, TRUE );

	SetFocus( ghViewWnd );

	if( 1 <= gdBUInterval )
	{
		SetTimer( hWnd, IDT_BACKUP_TIMER, (gdBUInterval * 60000), NULL );
	}

	ghBrTmplWnd = BrushTmpleInitialise( hInstance, hWnd, &rect, ghMaaWnd );	//	�u���V��


	ghMaaWnd = MaaTmpltInitialise( hInstance, hWnd, &wnRect );

#ifdef DRAUGHT_STYLE
	DraughtInitialise( hInstance, hWnd );
#endif

	if( isMaxim )
	{
		ShowWindow( hWnd, SW_MAXIMIZE );
		AppClientAreaCalc( &rect  );	//	
		grdSplitPos = sptBuf;
		SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );
		Cls_OnSize( hWnd, SIZE_MINIMIZED, rect.right, rect.top );
	}
	else{	ShowWindow( hWnd, nCmdShow );	}

	UpdateWindow( hWnd );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�C�g���o�[��ύX����
	@param[in]	ptText	�^�C�g���ɂ������镶����ENULL�ŏI����ĂȂ��ƍ����ڂɂ�����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AppTitleChange( LPTSTR ptText )
{
	LPTSTR	ptName;
	TCHAR	atBuff[MAX_PATH];

	if( ptText )
	{
		if( NULL != ptText[0] ){	ptName = PathFindFileName( ptText );	}
		else{						ptName = NAMELESS_DUMMY;	}
		StringCchPrintf( atBuff, MAX_PATH, TEXT("%s - %s"), gszTitle, ptName );
	}
	else
	{
		StringCchCopy( atBuff, MAX_PATH, gszTitle );
	}

	SetWindowText( ghMainWnd, atBuff );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�C�g���o�[�Ƀg���X���[�h�\�L��Ђ������������
	@param[in]	bMode	��O�g���X���[�h�J�n�@�O�I���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT AppTitleTrace( UINT bMode )
{
	static	TCHAR	atOrig[MAX_PATH];
	TCHAR	atBuff[MAX_PATH];

	if( bMode )
	{
		GetWindowText( ghMainWnd, atOrig, MAX_PATH );
		StringCchPrintf( atBuff, MAX_PATH, TEXT("%s [�g���X���[�h]"), atOrig );
		SetWindowText( ghMainWnd, atBuff );
	}
	else
	{
		if( 0 != atOrig[0] ){	SetWindowText( ghMainWnd , atOrig );	}
		else{	SetWindowText( ghMainWnd, gszTitle );	}
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�o�[�W�������{�b�N�X�̃��b�Z�[�W�n���h���ł��B
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_INITDIALOG:
			SetDlgItemText( hDlg, IDE_ABOUT_DISP, gcatLicense );
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���̃E�C���h�E�v���V�[�W��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�����ς�
	@retval no0	�����ł͏����������ɉ�
*/
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HMENU	hSubMenu;
#ifdef NDEBUG
	INT_PTR	iRslt;
#endif

	switch( message )
	{
		HANDLE_MSG( hWnd, WM_CREATE,		Cls_OnCreate );		//	��ʂ̍\���p�[�c�����B�{�^���Ƃ�
		HANDLE_MSG( hWnd, WM_PAINT,			Cls_OnPaint );		//	��ʂ̍X�V�Ƃ�
		HANDLE_MSG( hWnd, WM_COMMAND,		Cls_OnCommand );	//	�{�^�������ꂽ�Ƃ��̃R�}���h����
		HANDLE_MSG( hWnd, WM_DESTROY,		Cls_OnDestroy );	//	�\�t�g�I�����̏���
		HANDLE_MSG( hWnd, WM_SIZE,			Cls_OnSize );		//	
		HANDLE_MSG( hWnd, WM_MOVE,			Cls_OnMove );		//	
		HANDLE_MSG( hWnd, WM_DROPFILES,		Cls_OnDropFiles );	//	D&D
		HANDLE_MSG( hWnd, WM_ACTIVATE,		Cls_OnActivate );	//	
		HANDLE_MSG( hWnd, WM_NOTIFY,		Cls_OnNotify  );	//	�R�����R���g���[���̌ʃC�x���g
		HANDLE_MSG( hWnd, WM_TIMER,			Cls_OnTimer );		//	
		HANDLE_MSG( hWnd, WM_CONTEXTMENU,	Cls_OnContextMenu );	//	
		HANDLE_MSG( hWnd, WM_HOTKEY,		Cls_OnHotKey  );	//	

		HANDLE_MSG( hWnd, WM_KEYDOWN,		Evw_OnKey );			//	
		HANDLE_MSG( hWnd, WM_KEYUP,			Evw_OnKey );			//	
		HANDLE_MSG( hWnd, WM_CHAR,			Evw_OnChar );			//	
		HANDLE_MSG( hWnd, WM_MOUSEWHEEL,	Evw_OnMouseWheel );	//	

#ifdef NDEBUG
		case WM_CLOSE:
			iRslt = DocFileCloseCheck( hWnd, TRUE );
			if( !(iRslt)  ){	return FALSE;	}
			break;
#endif
		//	�^�X�N�g���C�̃A�C�R���ŃG�x���c����
		case WMP_TRAYNOTIFYICON:
	//		if( WM_LBUTTONUP == lParam ||  WM_RBUTTONUP == lParam ){	SetActiveWindow( hWnd );	}
			return 0;
			
		case WMP_BRUSH_TOGGLE:
			hSubMenu = GetSubMenu( ghMenu, 1 );
			CheckMenuItem( hSubMenu, IDM_BRUSH_STYLE, wParam ? MF_CHECKED : MF_UNCHECKED );
			return 0;

		case WMP_PREVIEW_CLOSE:
			DestroyCaret(  );
			ViewFocusSet(  );
			ViewShowCaret(  );
			return 0;

		default:	break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
//-------------------------------------------------------------------------------------------------

/*!
	�������ԂȂ�����Ă΂��
	@param[in]	hWnd			�E�C���h�E�n���h��
	@param[in]	state			���
	@param[in]	hWndActDeact	�������Ԗ��͔񂠂����ԂɂȂ��ENULL�ł���
	@param[in]	fMinimized		�ŏ����Ȃ�NULL
*/
VOID Cls_OnActivate( HWND hWnd, UINT state, HWND hWndActDeact, BOOL fMinimized )
{
	LONG_PTR	rdExStyle;
	HWND		hWorkWnd;

/*
WA_INACTIVE		0	�E�B���h�E����A�N�e�B�u�ɂȂ�܂��B
WA_ACTIVE		1	�}�E�X���N���b�N����ȊO�̕��@ (�E�B���h�E�̑I���ɃL�[�{�[�h �C���^�[�t�F�C�X���g�p������@�Ȃ�) �ŁA�E�B���h�E���A�N�e�B�u�ɂȂ�܂��B
WA_CLICKACTIVE	2	�}�E�X�N���b�N�ɂ���āA�E�B���h�E���A�N�e�B�u�ɂȂ�܂��B
*/
//	TRACE( TEXT("MAIN_WM_ACTIVATE STATE[%u] HWND[%X] MIN[%u]"), state, hWndActDeact, fMinimized );

	if( WA_INACTIVE == state )
	{
	}
	else
	{
		rdExStyle = GetWindowLongPtr( ghPgVwWnd, GWL_EXSTYLE );
		if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
		else							hWorkWnd = hWnd;
		SetWindowPos( ghPgVwWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );

		rdExStyle = GetWindowLongPtr( ghMaaWnd, GWL_EXSTYLE );
		if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
		else							hWorkWnd = hWnd;
		SetWindowPos( ghMaaWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );

		if( IsWindowVisible(ghLnTmplWnd ) )
		{
			rdExStyle = GetWindowLongPtr( ghLnTmplWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
			else							hWorkWnd = hWnd;
			SetWindowPos( ghLnTmplWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );
		}

		if( IsWindowVisible(ghBrTmplWnd ) )
		{
			rdExStyle = GetWindowLongPtr( ghBrTmplWnd, GWL_EXSTYLE );
			if( WS_EX_TOPMOST & rdExStyle )	hWorkWnd = HWND_TOPMOST;
			else							hWorkWnd = hWnd;
			SetWindowPos( ghBrTmplWnd, hWorkWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE );
		}

		TRACE( TEXT("WINDOW ACTVATE") );
		ViewFocusSet(  );
	}

	return;
}
//-------------------------------------------------------------------------------------------------


/*!
	���C���̃N���G�C�g�B
	@param[in]	hWnd			�e�E�C���h�E�̃n���h��
	@param[in]	lpCreateStruct	�A�v���P�[�V�����̏��������e
	@return	TRUE	���ɂȂ�
*/
BOOLEAN Cls_OnCreate( HWND hWnd, LPCREATESTRUCT lpCreateStruct )
{
	RECT	clRect, tbRect;

	RECT	tiRect;
	TCITEM	stTcItem;


	HINSTANCE lcInst = lpCreateStruct->hInstance;	//	�󂯎������������񂩂�A�C���X�^���X�n���h�����Ђ��ς�

	DragAcceptFiles( hWnd, TRUE );

	GetClientRect( hWnd, &clRect );

#ifdef USE_NOTIFYICON
	NOTIFYICONDATA	nid;
	//	�^�X�N�g���C�A�C�R���ƃo���[���w���v
	ZeroMemory( &nid, sizeof(NOTIFYICONDATA) );
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd   = hWnd;
	nid.uID    = 0;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WMP_TRAYNOTIFYICON;
	nid.hIcon  = LoadIcon( lcInst, MAKEINTRESOURCE(IDI_ORINRINEDITOR) );
	StringCchCopy( nid.szTip, 128, gszTitle );
	Shell_NotifyIcon( NIM_ADD, &nid );
#endif

//�c�[���o�[�쐬
	ToolBarCreate( hWnd, lcInst );

	ghNameFont = CreateFont( 12, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("MS UI Gothic") );

//�}���`�t�@�C���I�[�|��
	ghFileTabWnd = CreateWindowEx( 0, WC_TABCONTROL, TEXT("filetab"),
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | TCS_SINGLELINE,
		0, 0, clRect.right, 0, hWnd, (HMENU)IDTB_MULTIFILE, lcInst, NULL );

	SetWindowFont( ghFileTabWnd, ghNameFont, FALSE );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask    = TCIF_TEXT;
	stTcItem.pszText = NAMELESS_DUMMY;
	TabCtrl_InsertItem( ghFileTabWnd, 0, &stTcItem );

	ToolBarSizeGet( &tbRect );
	TabCtrl_GetItemRect( ghFileTabWnd, 1, &tiRect );
	tiRect.bottom  += tiRect.top;
	MoveWindow( ghFileTabWnd, 0, tbRect.bottom, clRect.right, tiRect.bottom, TRUE );


	ghStsBarWnd = CreateStatusWindow( WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, TEXT(""), hWnd, IDSB_VIEW_STATUS_BAR );
	SendMessage( ghStsBarWnd, SB_SIMPLE, FALSE, 0L );
	SendMessage( ghStsBarWnd, SB_SETPARTS, (WPARAM)SB_ITEMS, (LPARAM)(LPINT)gadStsBarSize );

	StatusBar_SetText( ghStsBarWnd, 1 , TEXT("") );

	return TRUE;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C����COMMAND���b�Z�[�W�̎󂯎��B�{�^�������ꂽ�Ƃ��Ŕ���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	id			���b�Z�[�W�𔭐��������q�E�C���h�E�̎��ʎq	LOWORD(wParam)
	@param[in]	hWndCtl		���b�Z�[�W�𔭐��������q�E�C���h�E�̃n���h��	lParam
	@param[in]	codeNotify	�ʒm���b�Z�[�W	HIWORD(wParam)
	@return		�Ȃ�
*/
VOID Cls_OnCommand( HWND hWnd, INT id, HWND hWndCtl, UINT codeNotify )
{
	INT		iRslt;

	//	�L�[�{�[�h�V���[�g�J�b�g�́A���̂Ƃ�ACTIVE�ȃE�C���h�E�ɍs��

	switch( id )
	{
		case  IDM_ABOUT:	DialogBox( ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About );	return;

		case  IDM_EXIT:	//	WM_CLOSE�Ƃ̐������ɒ��ӃZ��
			iRslt = DocFileCloseCheck( hWnd, TRUE );
			if( iRslt ){	DestroyWindow( hWnd );	}
			return;
	}

	OperationOnCommand( hWnd, id, hWndCtl, codeNotify );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	PAINT�B�����̈悪�o�����Ƃ��ɔ����B�w�i�̈����ɒ��ӁB�w�i��h��Ԃ��Ă���A�I�u�W�F�N�g��`��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Cls_OnPaint( HWND hWnd )
{
	PAINTSTRUCT	ps;
	HDC			hdc;

	hdc = BeginPaint( hWnd, &ps );

	EndPaint( hWnd, &ps );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�^�C�}�̃A��
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	id		�^�C�}�h�c
	@return		����
*/
VOID Cls_OnTimer( HWND hWnd, UINT id )
{
	if( IDT_BACKUP_TIMER != id )	return;
	//	�^�C�}�������璍��

	DocFileBackup( hWnd );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E���T�C�Y�ύX���ꂽ�Ƃ�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	state	�ύX�̏�ԁESIZE_MINIMIZED �Ƃ�
	@param[in]	cx		�N���C�����g�w�T�C�Y
	@param[in]	cy		�N���C�����g�x�T�C�Y
	@return		����
*/
VOID Cls_OnSize( HWND hWnd, UINT state, INT cx, INT cy )
{
	static  UINT	ccState;

	RECT	rect;

	if( SIZE_MINIMIZED == state )	//	�ŏ�����
	{
		ShowWindow( ghMaaWnd, SW_HIDE );
		if( !(gbTmpltDock) )
		{
			ShowWindow( ghPgVwWnd, SW_HIDE );
			ShowWindow( ghLnTmplWnd, SW_HIDE );
			ShowWindow( ghBrTmplWnd, SW_HIDE );
		}
		ccState = SIZE_MINIMIZED;

		return;
	}

	if( SIZE_MINIMIZED == ccState &&  ccState != state )	//	�ŏ������畜�A
	{
		ShowWindow( ghMaaWnd, SW_SHOW );
		if( !(gbTmpltDock) )
		{
			ShowWindow( ghPgVwWnd,   SW_SHOW );
			ShowWindow( ghLnTmplWnd, SW_SHOW );
			ShowWindow( ghBrTmplWnd, SW_SHOW );
		}
		ccState = SIZE_RESTORED;
	}

	if( SIZE_MAXIMIZED == state )	//	�ő剻��
	{
		AppClientAreaCalc( &rect  );	//	�E�ɕ����Ĉړ�
		SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );

		ccState = SIZE_MAXIMIZED;
	}

	if( SIZE_RESTORED == state &&  SIZE_MAXIMIZED == ccState )	//	�ő剻���畜�A��
	{
		if( !(IsZoomed( hWnd ) ) )	//	�܂��ő剻���Ȃ�A�X�v���b�g�o�[�����̂͂�
		{
			AppClientAreaCalc( &rect  );	//	�E�ɕ����Ĉړ�
			SetWindowPos( ghMainSplitWnd, HWND_TOP, rect.right - grdSplitPos, rect.top, 0, 0, SWP_NOSIZE );

			ccState = SIZE_RESTORED;
		}
	}

	MoveWindow( ghStsBarWnd, 0, 0, 0, 0, TRUE );	//	�X�e�[�^�X�o�[�͏���Ɉʒu�Ƃ����������̂ł���ł���

	ToolBarOnSize( hWnd, state, cx, cy );

	AppClientAreaCalc( &rect  );	//	

	ViewSizeMove( hWnd, &rect );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E���ړ������Ƃ�
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	x		
	@param[in]	y		
	@return		����
*/
VOID Cls_OnMove( HWND hWnd, INT x, INT y )
{
	DWORD	dwStyle;

	dwStyle = GetWindowStyle( hWnd );
	if( dwStyle & WS_MINIMIZE )
	{
		LayerMoveFromView( hWnd, SIZE_MINIMIZED );
		FrameMoveFromView( hWnd, SIZE_MINIMIZED );
		MoziMoveFromView( hWnd, SIZE_MINIMIZED );
	}
	else
	{
		LayerMoveFromView( hWnd, SIZE_RESTORED );
		FrameMoveFromView( hWnd, SIZE_RESTORED );
		MoziMoveFromView( hWnd, SIZE_RESTORED );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�����Ƃ��ɔ����B�f�o�C�X�R���e�L�X�g�Ƃ��S���I���B
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@return		����
*/
VOID Cls_OnDestroy( HWND hWnd )
{
	RECT	rect;
	DWORD	dwStyle;

#ifdef USE_NOTIFYICON
	NOTIFYICONDATA	nid;

	ZeroMemory( &nid, sizeof(NOTIFYICONDATA) );
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd   = hWnd;
	Shell_NotifyIcon( NIM_DELETE, &nid );
#endif

	PreviewInitialise( NULL, NULL );

	TraceInitialise( hWnd, FALSE );

	BrushTmpleInitialise( NULL, NULL, NULL, NULL );

	LayerBoxInitialise( NULL, NULL );

	FrameInitialise( NULL, NULL );

	MoziInitialise( NULL, NULL );

#ifdef DRAUGHT_STYLE
	DraughtInitialise( NULL, NULL );
#endif

	CntxEditInitialise( NULL, NULL );

	//	�E�C���h�E�̏�Ԃ��m�F���āA�ŏ�����ԂȂ�L�^���Ȃ�
	dwStyle = GetWindowStyle( hWnd );
	if( !(dwStyle & WS_MINIMIZE) )
	{
		if( dwStyle & WS_MAXIMIZE ){	InitParamValue( INIT_SAVE, VL_MAXIMISED, 1 );	}
		else
		{
			//	�ő剻���Ă���L�^���Ȃ�
			GetWindowRect( ghMainWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_MVIEW, &rect );//�I�����ۑ�

			InitParamValue( INIT_SAVE, VL_MAXIMISED, 0 );
		}

		if( !(gbTmpltDock) )
		{
			GetWindowRect( ghPgVwWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_PLIST, &rect );//�I�����ۑ�

			GetWindowRect( ghLnTmplWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_LNTMPL, &rect );//�I�����ۑ�

			GetWindowRect( ghBrTmplWnd, &rect );
			rect.right  = rect.right  - rect.left;
			rect.bottom = rect.bottom - rect.top;
			InitWindowPos( INIT_SAVE, WDP_BRTMPL, &rect );//�I�����ۑ�
		}
	}

	//	MAA�̂͂��������ł���Ă�
	DestroyWindow( ghMaaWnd );

	DestroyWindow( ghPgVwWnd );


	InitMultiFileTabOpen( INIT_SAVE, 0, NULL );

	SetWindowFont( ghFileTabWnd, GetStockFont(DEFAULT_GUI_FONT), FALSE );

	DeleteFont( ghNameFont );

	ToolBarDestroy(  );

	DestroyWindow( ghViewWnd );
	AaFontCreate( 0 );

	PostQuitMessage( 0 );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�R���e�L�X�g���j���[�Ăт����A�N�V����(�v�͉E�N���b�N�j
	@param[in]	hWnd		�E�C���h�E�n���h��
	@param[in]	hWndContext	�R���e�L�X�g�����������E�C���h�E�̃n���h��
	@param[in]	xPos		�X�N���[���w���W
	@param[in]	yPos		�X�N���[���x����
	@return		����
*/
VOID Cls_OnContextMenu( HWND hWnd, HWND hWndContext, UINT xPos, UINT yPos )
{
	HMENU	hMenu, hSubMenu;
	UINT	dRslt;
	INT		curSel;
	INT		iCount;
	POINT	stPost;
	TCHITTESTINFO	stTcHitInfo;

	MENUITEMINFO	stMenuItemInfo;

	TCITEM	stTcItem;
	TCHAR	atText[MAX_PATH];
	UINT	cchSize;

	stPost.x = (SHORT)xPos;	//	��ʍ��W�̓}�C�i�X�����肤��
	stPost.y = (SHORT)yPos;

	TRACE( TEXT("MAIN CONTEXT") );
	//���o�[�́A�o�[���̂ƍ��̎����̂Ƃ���͔�������E

	ToolBarOnContextMenu( hWnd, hWndContext, stPost.x, stPost.y );

	//	�����^�u�ȊO�ɂ͂Ȃ�
	if( ghFileTabWnd != hWndContext )	return;

	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	hMenu = LoadMenu( GetModuleHandle(NULL), MAKEINTRESOURCE(IDM_MULTIFILE_POPUP) );
	hSubMenu = GetSubMenu( hMenu, 0 );

	stTcHitInfo.pt = stPost;
	ScreenToClient( ghFileTabWnd, &(stTcHitInfo.pt) );
	curSel = TabCtrl_HitTest( ghFileTabWnd, &stTcHitInfo );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask       = TCIF_TEXT | TCIF_PARAM;
	stTcItem.pszText    = atText;
	stTcItem.cchTextMax = MAX_PATH;
	TabCtrl_GetItem( ghFileTabWnd, curSel, &stTcItem );

	//	�E�N�������z��I������
	TabCtrl_SetCurSel( ghFileTabWnd, curSel );
	DocMultiFileSelect( stTcItem.lParam );

	StringCchCat( atText, MAX_PATH, TEXT(" �����(&Q)") );
	StringCchLength( atText, MAX_PATH, &cchSize );

	ZeroMemory( &stMenuItemInfo, sizeof(MENUITEMINFO) );
	stMenuItemInfo.cbSize     = sizeof(MENUITEMINFO);
	stMenuItemInfo.fMask      = MIIM_TYPE;
	stMenuItemInfo.fType      = MFT_STRING;
	stMenuItemInfo.cch        = cchSize;
	stMenuItemInfo.dwTypeData = atText;

	if( 1 >= iCount )	//	�c��t�@�C�����������̎��͕���Ȃ�
	{
		stMenuItemInfo.fMask |= MIIM_STATE;
		stMenuItemInfo.fState = MFS_GRAYED;
//		EnableMenuItem( hSubMenu, IDM_FILE_CLOSE, MF_BYCOMMAND | MF_GRAYED );
	}
	
	SetMenuItemInfo( hSubMenu, IDM_FILE_CLOSE, FALSE, &stMenuItemInfo );

	dRslt = TrackPopupMenu( hSubMenu, 0, stPost.x, stPost.y, 0, hWnd, NULL );

	DestroyMenu( hMenu );

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���E�C���h�E�̃m�[�e�B�t�@�C���b�Z�[�W�̏���
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idFrom		NOTIFY�𔭐��������R���g���[���̂h�c
	@param[in]	pstNmhdr	NOTIFY�̏ڍ�
	@return		�����������e�Ƃ�
*/
LRESULT Cls_OnNotify( HWND hWnd, INT idFrom, LPNMHDR pstNmhdr )
{
	INT	curSel;
	TCITEM	stTcItem;
	TCHAR	atText[MAX_PATH];


	ToolBarOnNotify( hWnd, idFrom, pstNmhdr );

	if( IDTB_DOCK_TAB == idFrom )
	{
		if( TCN_SELCHANGE == pstNmhdr->code )
		{
			curSel = TabCtrl_GetCurSel( pstNmhdr->hwndFrom );

			TRACE( TEXT("TMPL TAB sel [%d]"), curSel );

			//	�t�H�[�J�X����ւ�
			switch( curSel )
			{
				case  0:	ShowWindow( ghLnTmplWnd , SW_SHOW );	ShowWindow( ghBrTmplWnd , SW_HIDE );	break;
				case  1:	ShowWindow( ghLnTmplWnd , SW_HIDE );	ShowWindow( ghBrTmplWnd , SW_SHOW );	break;
				default:	break;
			}
		}
	}

	if( IDTB_MULTIFILE == idFrom )
	{
		if( TCN_SELCHANGE == pstNmhdr->code )
		{
			curSel = TabCtrl_GetCurSel( ghFileTabWnd );

			TRACE( TEXT("FILE TAB sel [%d]"), curSel );

			ZeroMemory( &stTcItem, sizeof(TCITEM) );
			stTcItem.mask       = TCIF_TEXT | TCIF_PARAM;
			stTcItem.pszText    = atText;
			stTcItem.cchTextMax = MAX_PATH;
			TabCtrl_GetItem( ghFileTabWnd, curSel, &stTcItem );

			TRACE( TEXT("FILE [%s] param[%d]"), atText, stTcItem.lParam );

			DocMultiFileSelect( stTcItem.lParam );
		}
	}

	return 0;	//	�����Ȃ��Ȃ�O��߂�
}
//-------------------------------------------------------------------------------------------------

/*!
	�h���b�O���h���b�v�̎󂯓���
	@param[in]	hWnd	�e�E�C���h�E�̃n���h��
	@param[in]	hDrop	�h���b�s���I�u�W�F�N�g�n���h�D
*/
VOID Cls_OnDropFiles( HWND hWnd, HDROP hDrop )
{
	TCHAR	atFileName[MAX_PATH];
	LPARAM	dNumber;

	ZeroMemory( atFileName, sizeof(atFileName) );

	DragQueryFile( hDrop, 0, atFileName, MAX_PATH );
	DragFinish( hDrop );

	TRACE( TEXT("DROP[%s]"), atFileName );

	dNumber = DocFileInflate( atFileName );	//	�J���Ē��g�W�J
	if( !(dNumber) )
	{
		MessageBox( hWnd, TEXT("�t�@�C�����J���Ȃ�����������[�I�H"), NULL, MB_OK | MB_ICONERROR );
	}
	else{	MultiFileTabAppend( dNumber, atFileName );	}


	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�z�b�g�L�[�����ꂽ��Ă΂��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	idHotKey	�o�^���Ă���h�c
	@param[in]	fuModifiers	CTRL�ASHIFT�AALT �̑g�ݍ��킹���
	@param[in]	vk			������Ă����ʃL�[
	@return		����
*/
VOID Cls_OnHotKey(HWND hWnd, INT idHotKey, UINT fuModifiers, UINT vk )
{
	if( VK_D == vk )
	{
		TRACE( TEXT("Hotkey Incoming!!") );
		DocThreadDropCopy(  );
	}

	return;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�e�[�^�X�o�[�ɕ�����ǉ�
	@param[in]	room	�����g�O�C���f�b�N�X
	@param[in]	ptText	����镶����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT StatusBarSetText( INT room, LPCTSTR ptText )
{
	StatusBar_SetText( ghStsBarWnd, room , ptText );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�X�e�[�^�X�o�[�ɐ��������
	@param[in]	room	�����g�O�C���f�b�N�X
	@param[in]	value	����鐔�l
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT StatusBarSetTextInt( INT room, INT value )
{
	TCHAR	atString[SUB_STRING];

	ZeroMemory( atString, sizeof(atString) );
	StringCchPrintf( atString, SUB_STRING, TEXT("%d"), value );

	StatusBar_SetText( ghStsBarWnd, room , atString );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------




/*!
	���j���[���ڂ��`�F�b�N������O������
	@param[in]	itemID	���삷�郁�j���[�̂h�c
	@param[in]	bCheck	��O�`�F�b�N�E�O�͂���
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MenuItemCheckOnOff( UINT itemID, UINT bCheck )
{
	CheckMenuItem( ghMenu, itemID, bCheck ? MF_CHECKED : MF_UNCHECKED );

	ToolBarCheckOnOff( itemID, bCheck );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�O���Ńu���V�@�\ON/OFF�����ꍇ
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT BrushModeToggle( VOID )
{
	LRESULT	rlst;
	HMENU	hSubMenu;

	rlst = SendMessage( ghBrTmplWnd, WMP_BRUSH_TOGGLE, 0, 0 );
	hSubMenu = GetSubMenu( ghMenu, 1 );
	CheckMenuItem( hSubMenu, IDM_BRUSH_STYLE, rlst ? MF_CHECKED : MF_UNCHECKED );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���C���E�C���h�E�̃N���C�����g�̈�����߂�
	@param[out]	pstRect	�̈悢���\���̃|�C���^�[
*/
UINT AppClientAreaCalc( LPRECT pstRect )
{
	RECT	rect, sbRect, tbRect;
	RECT	ftRect;


	if( !(pstRect) )	return 0;

	if( !(ghMainWnd)  ){	SetRect( pstRect , 0, 0, 0, 0 );	return 0;	}

	GetClientRect( ghMainWnd, &rect );	//	

	ToolBarSizeGet( &tbRect );	//	�c�[���o�[�̃T�C�Y�Ƃ���
	rect.top      += tbRect.bottom;
	rect.bottom   -= tbRect.bottom;

	GetClientRect( ghStsBarWnd, &sbRect );	//	�X�e�[�^�X�o�[�̃T�C�Y�m�F
	rect.bottom   -= sbRect.bottom;

	GetWindowRect( ghFileTabWnd, &ftRect );	//	�^�u�o�[�̃T�C�Y�m�F
	ftRect.bottom -= ftRect.top;
	SetWindowPos( ghFileTabWnd, HWND_TOP, 0, tbRect.bottom, rect.right, ftRect.bottom, SWP_NOZORDER );
	rect.top      += ftRect.bottom;
	rect.bottom   -= ftRect.bottom;	//	�^�u�o�[�̕��k�߂�

	SetRect( pstRect, rect.left, rect.top, rect.right, rect.bottom );

	return 1;
}
//-------------------------------------------------------------------------------------------------

#ifndef MAA_PROFILE

/*!
	MAA�����t�@�C���̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dCount	�f�[�^�̌�[NULL==ptFile]�^�ʂ��ԍ�
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@param[out]	ptBase	��_�f�B���N�g���[���EMAX_PATH�ł��邱��
	@return		INT	���[�h�F�f�[�^�̌��@�Z�[�u�F�O
*/
INT InitMultipleFile( UINT dMode, UINT dCount, LPTSTR ptFile, LPTSTR ptBase )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	INT	dBuff = 0;

	if( ptFile )
	{
		if( dMode  )	//	���[�h
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%d"), dCount );
			GetPrivateProfileString( TEXT("MaaSubOpen"), atKeyName, TEXT(""), ptFile, MAX_PATH, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Base%d"), dCount );
			GetPrivateProfileString( TEXT("MaaSubOpen"), atKeyName, TEXT(""), ptBase, MAX_PATH, gatIniPath );
		}
		else	//	�Z�[�u
		{
			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Item%d"), dCount );
			WritePrivateProfileString( TEXT("MaaSubOpen"), atKeyName, ptFile, gatIniPath );

			StringCchPrintf( atKeyName, MIN_STRING, TEXT("Base%d"), dCount );
			WritePrivateProfileString( TEXT("MaaSubOpen"), atKeyName, ptBase, gatIniPath );
		}
	}
	else	//	��
	{
		if( dMode  )	//	���[�h
		{
			GetPrivateProfileString( TEXT("MaaSubOpen"), TEXT("Count"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
			dBuff = StrToInt( atBuff );
		}
		else	//	�Z�[�u
		{
			//	��U�Z�N�V��������ɂ���
			ZeroMemory( atBuff, sizeof(atBuff) );
			WritePrivateProfileSection( TEXT("MaaSubOpen"), atBuff, gatIniPath );

			StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), dCount );
			WritePrivateProfileString( TEXT("MaaSubOpen"), TEXT("Count"), atBuff, gatIniPath );
		}

		return dBuff;
	}


	return 0;
}
//-------------------------------------------------------------------------------------------------
#endif

/*!
	�g���X�̊e���[�h�̐��l
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	pstInfo	���[�h�Z�[�u�̓��e�ۑ�
	@return		��O���[�h�n�j�@�O�Ȃ�����
*/
INT InitTraceValue( UINT dMode, LPTRACEPARAM pstInfo )
{
	TCHAR	atBuff[MIN_STRING];
	INT	dBuff = 0;

	if( dMode )	//	���[�h
	{
		dBuff = GetPrivateProfileInt( TEXT("Trace"), TEXT("Turning"), -1, gatIniPath );
		if( 0 > dBuff )	return 0;

		pstInfo->dTurning     = dBuff;
		pstInfo->dZooming     = GetPrivateProfileInt( TEXT("Trace"), TEXT("Zooming"), 0, gatIniPath );
		pstInfo->dGrayMoph    = GetPrivateProfileInt( TEXT("Trace"), TEXT("GrayMoph"), 0, gatIniPath );
		pstInfo->dGamma       = GetPrivateProfileInt( TEXT("Trace"), TEXT("Gamma"), 0, gatIniPath );
		pstInfo->dContrast    = GetPrivateProfileInt( TEXT("Trace"), TEXT("Contrast"), 0, gatIniPath );
		pstInfo->stOffsetPt.y = GetPrivateProfileInt( TEXT("Trace"), TEXT("OffsetY"), 0, gatIniPath );
		pstInfo->stOffsetPt.x = GetPrivateProfileInt( TEXT("Trace"), TEXT("OffsetX"), 0, gatIniPath );
		pstInfo->bUpset       = GetPrivateProfileInt( TEXT("Trace"), TEXT("Upset"), 0, gatIniPath );
		pstInfo->bMirror      = GetPrivateProfileInt( TEXT("Trace"), TEXT("Mirror"), 0, gatIniPath );
		pstInfo->dMoziColour  = GetPrivateProfileInt( TEXT("Trace"), TEXT("MoziColour"), 0, gatIniPath );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->stOffsetPt.x );
		WritePrivateProfileString( TEXT("Trace"), TEXT("OffsetX"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->stOffsetPt.y );
		WritePrivateProfileString( TEXT("Trace"), TEXT("OffsetY"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dContrast );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Contrast"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dGamma );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Gamma"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dGrayMoph );
		WritePrivateProfileString( TEXT("Trace"), TEXT("GrayMoph"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dZooming );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Zooming"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstInfo->dTurning );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Turning"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo->bUpset );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Upset"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo->bMirror );
		WritePrivateProfileString( TEXT("Trace"), TEXT("Mirror"), atBuff, gatIniPath );

		StringCchPrintf( atBuff, MIN_STRING, TEXT("%u"), pstInfo->dMoziColour );
		WritePrivateProfileString( TEXT("Trace"), TEXT("MoziColour"), atBuff, gatIniPath );
	}


	return 1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�F�ݒ�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�p�����[�^�̎��
	@param[in]	nColour	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return	COLORREF	���[�h�F�l�@�Z�[�u�F�O
*/
COLORREF InitColourValue( UINT dMode, UINT dStyle, COLORREF nColour )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	LPTSTR	ptEnd;

	switch( dStyle )
	{
		case CLRV_BASICPEN:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BasicPen") );	break;	//	�����F
		case CLRV_BASICBK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BasicBack") );	break;	//	�w�i�F
		case CLRV_GRIDLINE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GridLine") );	break;	//	�O���b�h
		case CLRV_CRLFMARK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CrLfMark") );	break;	//	���s�}�[�N
		case CLRV_CANTSJIS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("CantSjis") );	break;	//	Unicode����
		default:	return nColour;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%08X"), nColour );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( TEXT("Colour"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		nColour = (COLORREF)_tcstoul( atBuff, &ptEnd, 16 );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( TEXT("Colour"), atKeyName, atBuff, gatIniPath );
	}

	return nColour;
}
//-------------------------------------------------------------------------------------------------

/*!
	�p�����[�^�l�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�p�����[�^�̎��
	@param[in]	nValue	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return		INT	���[�h�F�l�@�Z�[�u�F�O
*/
INT InitParamValue( UINT dMode, UINT dStyle, INT nValue )
{
	TCHAR	atKeyName[MIN_STRING], atBuff[MIN_STRING];
	INT	dBuff = 0;

	switch( dStyle )
	{
		case  VL_CLASHCOVER:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ClashCover") );		break;
		case  VL_GROUP_UNDO:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GroupUndo") );		break;
		case  VL_USE_UNICODE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UseUnicode")  );	break;
		case  VL_LAYER_TRANS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("LayerTrans")  );	break;
		case  VL_RIGHT_SLIDE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RightSlide")  );	break;
		case  VL_MAA_SPLIT:		StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSplit") );		break;
		case  VL_SETMETHOD:		StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaMethod") );		break;
		case  VL_UNILISTLAST:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UniListLast") );	break;
		case  VL_MAATIP_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTip")  );	break;
		case  VL_MAATIP_SIZE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaToolTipSize") );	break;
		case  VL_LINETMP_CLM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("LineTmplClm") );	break;
		case  VL_BRUSHTMP_CLM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BrushTmplClm") );	break;
		case  VL_UNIRADIX_HEX:	StringCchCopy( atKeyName, SUB_STRING, TEXT("UniRadixHex") );	break;
		case  VL_BACKUP_INTVL:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BackUpIntvl") );	break;
		case  VL_BACKUP_MSGON:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BackUpMsgOn") );	break;
		case  VL_CRLF_CODE:		StringCchCopy( atKeyName, SUB_STRING, TEXT("CrLfCode") );		break;
	//	case  VL_SPACE_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("SpaceView") );		break;
		case  VL_GRID_X_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GridXpos") );		break;
		case  VL_GRID_Y_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("GridYpos") );		break;
		case  VL_GRID_VIEW:		StringCchCopy( atKeyName, SUB_STRING, TEXT("GridView") );		break;
		case  VL_R_RULER_POS:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RightRuler")  );	break;
		case  VL_R_RULER_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("RitRulerView") );	break;
		case  VL_PAGETIP_VIEW:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageToolTip") );	break;
		case  VL_PCOMBINE_NM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageCombineNM") );	break;
		case  VL_PDIVIDE_NM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageDivideNM") );	break;
		case  VL_PDELETE_NM:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageDeleteNM") );	break;
		case  VL_MAASEP_STYLE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MaaSepLine")  );	break;
		case  VL_PLS_LN_DOCK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PLstLineDock") );	break;
	//	case  VL_BRUSH_DOCK:	StringCchCopy( atKeyName, SUB_STRING, TEXT("BrushDock") );		break;
		case  VL_SWAP_COPY:		StringCchCopy( atKeyName, SUB_STRING, TEXT("CopyModeSwap") );	break;
		case  VL_MAIN_SPLIT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("MainSplit") );		break;
		case  VL_MAXIMISED:		StringCchCopy( atKeyName, SUB_STRING, TEXT("Maximised") );		break;
		case  VL_DRAUGHT_MODE:	StringCchCopy( atKeyName, SUB_STRING, TEXT("DraughtDefault") );	break;
		case  VL_FIRST_READED:	StringCchCopy( atKeyName, SUB_STRING, TEXT("FirstStep") );		break;
		case  VL_LAST_OPEN:		StringCchCopy( atKeyName, SUB_STRING, TEXT("LastOpenStyle") );	break;
		default:	return nValue;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( TEXT("General"), atKeyName, atBuff, atBuff, MIN_STRING, gatIniPath );
		dBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( TEXT("General"), atKeyName, atBuff, gatIniPath );
	}

	return dBuff;
}
//-------------------------------------------------------------------------------------------------

/*!
	������̐ݒ���e���Z�[�u���[�h
	@param[in]		dMode	��O���[�h�@�O�Z�[�u
	@param[in]		dStyle	�p�����[�^�̎��
	@param[in,out]	ptFile	MAX_PATH�ł��邱��
	@return			HRESULT	�I����ԃR�[�h
*/
HRESULT InitParamString( UINT dMode, UINT dStyle, LPTSTR ptFile )
{
	TCHAR	atKeyName[MIN_STRING], atDefault[MAX_PATH];

	if(  !(ptFile) )	return E_INVALIDARG;

	switch( dStyle )
	{
		case VS_PROFILE_NAME:	StringCchCopy( atKeyName, SUB_STRING, TEXT("ProfilePath") );	break;
		case VS_PAGE_FORMAT:	StringCchCopy( atKeyName, SUB_STRING, TEXT("PageFormat")  );	break;
		default:	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		StringCchCopy( atDefault, MAX_PATH, ptFile );
		GetPrivateProfileString( TEXT("General"), atKeyName, atDefault, ptFile, MAX_PATH, gatIniPath );
	}
	else
	{
		WritePrivateProfileString( TEXT("General"), atKeyName, ptFile, gatIniPath );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------


#ifndef MAA_PROFILE
/*!
	MLT�f�B���N�g���̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitMaaFldrOpen( UINT dMode, LPTSTR ptFile )
{

	if(  !(ptFile) )	return 0;

	if( dMode ){	GetPrivateProfileString( TEXT("General"), TEXT("MultiLinePath"), TEXT(""), ptFile, MAX_PATH, gatIniPath );	}
	else{			WritePrivateProfileString( TEXT("General"), TEXT("MultiLinePath"), ptFile, gatIniPath );	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif


/*!
	�őO�ʏ�Ԃ̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�Q�ňꗗ�@�R��s�@�R�u���V�@�S�����s
	@param[in]	nValue	���[�h�F�f�t�H�l�@�Z�[�u�F�l
	@return		INT	���[�h�F�l�@�Z�[�u�F�O
*/
INT InitWindowTopMost( UINT dMode, UINT dStyle, INT nValue )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];
	INT	dBuff = 0;

	switch( dStyle )
	{
		case  WDP_PLIST:	StringCchCopy( atAppName, SUB_STRING, TEXT("PageList") );	break;
		case  WDP_LNTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("LineTmple") );	break;
		case  WDP_BRTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("BrushTmple") );	break;
		case  WDP_MAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MaaTmple") );	break;
		default:	return 0;
	}

	StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), nValue );

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("TopMost"), atBuff, atBuff, MIN_STRING, gatIniPath );
		dBuff = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		WritePrivateProfileString( atAppName, TEXT("TopMost"), atBuff, gatIniPath );
	}

	return dBuff;
}
//-------------------------------------------------------------------------------------------------

/*!
	�E�C���h�E�ʒu�̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	dStyle	�P�r���[�@�Q�ňꗗ�@�R��s�@�R�u���V�@�S�����s
	@param[in]	pstRect	���[�h���ʂ����邩�A�Z�[�u���e������
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitWindowPos( UINT dMode, UINT dStyle, LPRECT pstRect )
{
	TCHAR	atAppName[MIN_STRING], atBuff[MIN_STRING];

	if( !pstRect )	return E_INVALIDARG;

	switch( dStyle )
	{
		case  WDP_MVIEW:	StringCchCopy( atAppName, SUB_STRING, TEXT("MainView") );	break;
		case  WDP_PLIST:	StringCchCopy( atAppName, SUB_STRING, TEXT("PageList") );	break;
		case  WDP_LNTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("LineTmple") );	break;
		case  WDP_BRTMPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("BrushTmple") );	break;
		case  WDP_MAATPL:	StringCchCopy( atAppName, SUB_STRING, TEXT("MaaTmple") );	break;
		case  WDP_PREVIEW:	StringCchCopy( atAppName, SUB_STRING, TEXT("Preview") );	break;
		default:	return E_INVALIDARG;
	}

	if( dMode )	//	���[�h
	{
		GetPrivateProfileString( atAppName, TEXT("LEFT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->left   = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("TOP"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->top    = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("RIGHT"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->right  = StrToInt( atBuff );
		GetPrivateProfileString( atAppName, TEXT("BOTTOM"), TEXT("0"), atBuff, MIN_STRING, gatIniPath );
		pstRect->bottom = StrToInt( atBuff );
	}
	else	//	�Z�[�u
	{
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->left );
		WritePrivateProfileString( atAppName, TEXT("LEFT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->top );
		WritePrivateProfileString( atAppName, TEXT("TOP"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->right );
		WritePrivateProfileString( atAppName, TEXT("RIGHT"), atBuff, gatIniPath );
		StringCchPrintf( atBuff, MIN_STRING, TEXT("%d"), pstRect->bottom );
		WritePrivateProfileString( atAppName, TEXT("BOTTOM"), atBuff, gatIniPath );
	}
	
	return S_OK;
}
//-------------------------------------------------------------------------------------------------




/*!
	�ŏI�g�p�t�@�C���̃Z�[�u���[�h
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT InitLastOpen( UINT dMode, LPTSTR ptFile )
{
#ifdef OPEN_HISTORY
	TCHAR	atKeyName[SUB_STRING];

	if(  !(ptFile) )	return E_INVALIDARG;

	if( dMode )	//	���[�h�̂Ƃ��́A�ԍ��͂P�����Ďg��
	{

	}
	else
	{

	}

#else
	if(  !(ptFile) )	return E_INVALIDARG;
	if( dMode ){	GetPrivateProfileString( TEXT("History"), TEXT("LastOpen"), TEXT(""), ptFile, MAX_PATH, gatIniPath );	}
	else{			WritePrivateProfileString( TEXT("History") , TEXT("LastOpen"), ptFile, gatIniPath );	}
#endif

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�g���̃Z�[�u���[�h
	@param[in]	nowWnd	���̑��ԍ�
	@param[in]	iDir	�W�����v�����E�{�P���|�P�ŁE�O�͕s��
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT WindowFocusChange( INT nowWnd, INT iDir )
{
	INT	nextWnd;

	if( gbTmpltDock )	//	��������������
	{
		if( WND_MAIN == nowWnd )	nextWnd = WND_MAAT;
		else						nextWnd = WND_MAIN;
	}
	else
	{
		nextWnd = nowWnd + iDir;
		if( 0 >= nextWnd )			nextWnd = WND_BRUSH;	//	WND_BRUSH
		if( WND_BRUSH < nextWnd )	nextWnd = WND_MAIN;
		//	���[�ɒ��ӁE�������������Ȃ��Ǝv��
	}

	switch( nextWnd )
	{
		default:
		case WND_MAIN:	SetForegroundWindow( ghMainWnd );	break;
		case WND_MAAT:	SetForegroundWindow( ghMaaWnd );	break;
		case WND_PAGE:	SetForegroundWindow( ghPgVwWnd );	break;
		case WND_LINE:	SetForegroundWindow( ghLnTmplWnd );	break;
		case WND_BRUSH:	SetForegroundWindow( ghBrTmplWnd );	break;
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���Ղ����_�C�����O�J��
*/
HRESULT OptionDialogueOpen( VOID )
{
	HMENU	hSubMenu;
	UINT	bURHbuff, bABUIbuff, bCpNodBuf;

	bURHbuff  = gbUniRadixHex;
	bABUIbuff = gdBUInterval;
	bCpNodBuf = gbCpModSwap;

	//	���s�R�[�h�ύX�Ȃ�o�C�g���Čv�Z���K�v��
	DialogBoxParam( ghInst, MAKEINTRESOURCE(IDD_GENERAL_OPTION_DLG), ghMainWnd, OptionDlgProc, NULL );

	//	�ݒ�ύX����Ă�����e����
	if( bURHbuff != gbUniRadixHex ){	UnicodeRadixExchange( NULL );	};
	if( bABUIbuff != gdBUInterval )
	{
		if( 1 <= bABUIbuff )	KillTimer( ghMainWnd, IDT_BACKUP_TIMER );
		if( 1 <= gdBUInterval )	SetTimer(  ghMainWnd, IDT_BACKUP_TIMER, (gdBUInterval * 60000), NULL );
	}

	SqnSetting(   );	//	�O���[�v�A���h�D�̃A��

//@@�R�s�[����
	if( bCpNodBuf != gbCpModSwap )	//	�R�s�[���[�h����
	{
		hSubMenu = GetSubMenu( ghMenu, 1 );
		if( gbCpModSwap )
		{
			ModifyMenu( hSubMenu, IDM_COPY,     MF_BYCOMMAND | MFT_STRING, IDM_COPY,     TEXT("SJIS�R�s�|(&C)\tCtrl + C") );
			ModifyMenu( hSubMenu, IDM_SJISCOPY, MF_BYCOMMAND | MFT_STRING, IDM_SJISCOPY, TEXT("Unicode�R�s�|(&J)") );
		}
		else	//	�ʏ�
		{
			ModifyMenu( hSubMenu, IDM_COPY,     MF_BYCOMMAND | MFT_STRING, IDM_COPY,     TEXT("Unicode�R�s�[(&C)\tCtrl + C") );
			ModifyMenu( hSubMenu, IDM_SJISCOPY, MF_BYCOMMAND | MFT_STRING, IDM_SJISCOPY, TEXT("SJIS�R�s�|(&J)") );
		}

		CntxMenuCopySwap(  );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	���Ղ����_�C�����O�̃v���V�[�W��
	@param[in]	hDlg		�_�C�����O�n���h��
	@param[in]	message		�E�C���h�E���b�Z�[�W�̎��ʔԍ�
	@param[in]	wParam		�ǉ��̏��P
	@param[in]	lParam		�ǉ��̏��Q
	@retval 0	���b�Z�[�W�͏������Ă��Ȃ�
	@retval no0	�Ȃ񂩏������ꂽ
*/
INT_PTR CALLBACK OptionDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	static  UINT	cdGrXp, cdGrYp, cdRtRr;

	UINT	id;
	INT		dValue, dBuff;
	TCHAR	atBuff[SUB_STRING];
//	TCHAR	atPath[MAX_PATH];

	switch( message )
	{
		case WM_INITDIALOG:
			//	�X���C�_�����l�E�������قǔ����Ȃ�
			SendDlgItemMessage( hDlg, IDSL_LAYERBOX_TRANCED, TBM_SETRANGE, TRUE, MAKELPARAM( 0, 0xE0 ) );	//	0xE0

#ifdef MAA_PROFILE
			Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), TEXT("�`�`�f�B���N�g���̓v���t�@�C������ݒ肵�Ă�") );
			EnableWindow( GetDlgItem(hDlg,IDE_AA_DIRECTORY), FALSE );
			ShowWindow( GetDlgItem(hDlg,IDB_AADIR_SEARCH), SW_HIDE );
#else
			//	MAA�ꗗ
			InitMaaFldrOpen( INIT_LOAD, atPath );
			Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), atPath );
#endif
			//	MAA�|�b�v�A�b�v�ɂ���
			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_SIZE, 16 );	//	�T�C�Y�m�F
			if( FONTSZ_REDUCE == dValue )	CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_REDUCE );
			else							CheckRadioButton( hDlg, IDRB_POPUP_NOMAL, IDRB_POPUP_REDUCE, IDRB_POPUP_NOMAL );
			dValue = InitParamValue( INIT_LOAD, VL_MAATIP_VIEW, 1 );	//	�|�b�p�b�v���邩
			CheckDlgButton( hDlg, IDCB_POPUP_VISIBLE, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�E�񂹃h�b�g�ʒu
			dValue = InitParamValue( INIT_LOAD, VL_RIGHT_SLIDE, 790 );
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), dValue );
			Edit_SetText( GetDlgItem(hDlg,IDE_RIGHTSLIDE_DOT), atBuff );

			//	�O���b�h�ʒu
			cdGrXp = gdGridXpos;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdGridXpos );
			Edit_SetText( GetDlgItem(hDlg,IDE_GRID_X_POS), atBuff );

			cdGrYp = gdGridYpos;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdGridYpos );
			Edit_SetText( GetDlgItem(hDlg,IDE_GRID_Y_POS), atBuff );

			//	�E���[���ʒu
			cdRtRr = gdRightRuler;
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), gdRightRuler );
			Edit_SetText( GetDlgItem(hDlg,IDE_RIGHT_RULER_POS), atBuff );

			//	�����ۑ��Ԋu
			dValue = InitParamValue( INIT_LOAD, VL_BACKUP_INTVL, 3 );
			StringCchPrintf( atBuff, SUB_STRING, TEXT("%d"), dValue );
			Edit_SetText( GetDlgItem(hDlg,IDE_AUTO_BU_INTVL), atBuff );
			//	�����ۑ����b�Z�[�W
			//dValue = InitParamValue( INIT_LOAD, VL_BACKUP_MSGON, 1 );
			CheckDlgButton( hDlg, IDCB_AUTOBU_MSG_ON, gbAutoBUmsg ? BST_CHECKED : BST_UNCHECKED );

			//	���s�R�[�h�I��
			//dValue = InitParamValue( INIT_LOAD, VL_CRLF_CODE, 0 );
			CheckRadioButton( hDlg, IDRB_CRLF_STRB, IDRB_CRLF_2CH_YY, gbCrLfCode ? IDRB_CRLF_2CH_YY : IDRB_CRLF_STRB );

			//	�󔒃��j�R�[�h�p�f�B���O
			//dValue = InitParamValue( INIT_LOAD, VL_USE_UNICODE, 1 );
			CheckDlgButton( hDlg, IDCB_USE_UNISPACE_SET, gbUniPad ? BST_CHECKED : BST_UNCHECKED );

			//	���j�R�[�h�P�U�i��
			//dValue = InitParamValue( INIT_LOAD, VL_UNIRADIX_HEX, 1 );
			CheckDlgButton( hDlg, IDCB_UNIRADIX_HEX, gbUniRadixHex ? BST_CHECKED : BST_UNCHECKED );

			//	�O���[�v�A���h�D
			dValue = InitParamValue( INIT_LOAD, VL_GROUP_UNDO, 1 );
			CheckDlgButton( hDlg, IDCB_GROUPUNDO_SET, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�R�s�[�W���X�^�C��
			//dValue = InitParamValue( INIT_LOAD, VL_SWAP_COPY, 0 );	//	�O���j�R�[�h�@�PSJIS
			CheckDlgButton( hDlg, IDCB_COPY_STYLE_SWAP, gbCpModSwap ? BST_CHECKED : BST_UNCHECKED );

			//	�h�b�L���O�X�^�C��
			dValue = InitParamValue( INIT_LOAD, VL_PLS_LN_DOCK, 1 );
			CheckDlgButton( hDlg, IDCB_DOCKING_STYLE, dValue ? BST_CHECKED : BST_UNCHECKED );

			//	�N�����I�[�|��
			dValue = InitParamValue( INIT_LOAD, VL_LAST_OPEN, LASTOPEN_DO );
			CheckRadioButton( hDlg, IDRB_LASTOPEN_DO, IDRB_LASTOPEN_ASK, (IDRB_LASTOPEN_DO + dValue) );

			//	���C���{�b�N�X�̓����x
			dValue = InitParamValue( INIT_LOAD, VL_LAYER_TRANS, 192 );
			SendDlgItemMessage( hDlg, IDSL_LAYERBOX_TRANCED, TBM_SETPOS, TRUE, (dValue - 0x1F) );

			//	�����s�e���v�����N���b�N�����Ƃ��̓���
			dValue = InitParamValue( INIT_LOAD, VL_SETMETHOD, MAA_INSERT );
			switch( dValue )
			{
				case MAA_INSERT:	id =  IDRB_SEL_INS_EDIT;	break;
				case MAA_INTERRUPT:	id =  IDRB_SEL_INTRPT_EDIT;	break;
				case MAA_LAYERED:	id =  IDRB_SEL_SET_LAYER;	break;
				case MAA_UNICLIP:	id =  IDRB_SEL_CLIP_UNI;	break;
				case MAA_SJISCLIP:	id =  IDRB_SEL_CLIP_SJIS;	break;
				case MAA_DRAUGHT:	id =  IDRB_SEL_DRAUGHT;		break;
			}
			CheckRadioButton( hDlg, IDRB_SEL_INS_EDIT, IDRB_SEL_DRAUGHT, id );

#ifdef DRAUGHT_STYLE
			//	�h���t�g�{�[�h�ŃN���b�N�����Ƃ��̓���
			switch( gdClickMode )
			{
				case MAA_INSERT:	id =  IDRB_DRT_INS_EDIT;	break;
				case MAA_INTERRUPT:	id =  IDRB_DRT_INTRPT_EDIT;	break;
				case MAA_LAYERED:	id =  IDRB_DRT_SET_LAYER;	break;
				case MAA_UNICLIP:	id =  IDRB_DRT_CLIP_UNI;	break;
				case MAA_SJISCLIP:	id =  IDRB_DRT_CLIP_SJIS;	break;
			}
			CheckRadioButton( hDlg, IDRB_DRT_INS_EDIT, IDRB_DRT_CLIP_SJIS, id );
#endif
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
	#ifndef MAA_PROFILE
				case IDB_AADIR_SEARCH:	//	MAA�f�B���N�g���T��
					if( SelectDirectoryDlg( hDlg, atPath, MAX_PATH ) )
					{
						Edit_SetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), atPath );
					}
					return (INT_PTR)TRUE;
	#endif
				case IDB_APPLY://�K�p
				case IDOK:
	#ifndef MAA_PROFILE
					//	MAA�̃f�B���N�g���[
					Edit_GetText( GetDlgItem(hDlg,IDE_AA_DIRECTORY), atPath, MAX_PATH );
					InitMaaFldrOpen( INIT_SAVE, atPath );
	#endif
					//	MAA�|�b�v�A�b�v�ɂ���
					dValue = FONTSZ_NORMAL;
					if( IsDlgButtonChecked( hDlg, IDRB_POPUP_REDUCE ) ){	dValue =  FONTSZ_REDUCE;	}
					InitParamValue( INIT_SAVE, VL_MAATIP_SIZE, dValue );
					dBuff = IsDlgButtonChecked( hDlg, IDCB_POPUP_VISIBLE );
					AaItemsTipSizeChange( dValue, dBuff );
					InitParamValue( INIT_SAVE, VL_MAATIP_VIEW, dBuff );

					//	�E�񂹊�h�b�g
					Edit_GetText( GetDlgItem(hDlg,IDE_RIGHTSLIDE_DOT), atBuff, SUB_STRING );
					dValue = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_RIGHT_SLIDE, dValue );

					//	�O���b�h�ʒu
					Edit_GetText( GetDlgItem(hDlg,IDE_GRID_X_POS), atBuff, SUB_STRING );
					gdGridXpos = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_GRID_X_POS, gdGridXpos );
					if( cdGrXp != gdGridXpos )	ViewRedrawSetLine( -1 );

					Edit_GetText( GetDlgItem(hDlg,IDE_GRID_Y_POS), atBuff, SUB_STRING );
					gdGridYpos = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_GRID_Y_POS, gdGridYpos );
					if( cdGrYp != gdGridYpos )	ViewRedrawSetLine( -1 );

					//	�E���[���ʒu
					Edit_GetText( GetDlgItem(hDlg,IDE_RIGHT_RULER_POS), atBuff, SUB_STRING );
					gdRightRuler = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_R_RULER_POS, gdRightRuler );
					if( cdRtRr != gdRightRuler )	ViewRedrawSetLine( -1 );

					//	�����ۑ��Ԋu
					Edit_GetText( GetDlgItem(hDlg,IDE_AUTO_BU_INTVL), atBuff, SUB_STRING );
					gdBUInterval = StrToInt( atBuff );
					InitParamValue( INIT_SAVE, VL_BACKUP_INTVL, gdBUInterval );
					//	�����ۑ����b�Z�[�W
					dValue = IsDlgButtonChecked( hDlg, IDCB_AUTOBU_MSG_ON );
					gbAutoBUmsg = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_BACKUP_MSGON, gbAutoBUmsg );

					//	���s�R�[�h�I��
					gbCrLfCode = 0;
					if( IsDlgButtonChecked( hDlg , IDRB_CRLF_2CH_YY ) ){	gbCrLfCode =  1;	}
					InitParamValue( INIT_SAVE, VL_CRLF_CODE, gbCrLfCode );

					//	���j�R�[�h�X�y�[�X
					dValue = IsDlgButtonChecked( hDlg, IDCB_USE_UNISPACE_SET );
					gbUniPad = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_USE_UNICODE, gbUniPad );

					//	���j�R�[�h�P�U�i��
					dValue = IsDlgButtonChecked( hDlg, IDCB_UNIRADIX_HEX );
					gbUniRadixHex = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_UNIRADIX_HEX, gbUniRadixHex );

					//	�O���[�v�A���h�D
					dValue = IsDlgButtonChecked( hDlg, IDCB_GROUPUNDO_SET );
					InitParamValue( INIT_SAVE, VL_GROUP_UNDO, dValue ? 1 : 0 );

					//	�R�s�[�W���X�^�C��
					dValue = IsDlgButtonChecked( hDlg, IDCB_COPY_STYLE_SWAP );
					gbCpModSwap = dValue ? 1 : 0;
					InitParamValue( INIT_SAVE, VL_SWAP_COPY, gbCpModSwap );

					//	�h�b�L���O�X�^�C���E�ύX���Ă��A�ċN������܂ł͖���
					dValue = IsDlgButtonChecked( hDlg, IDCB_DOCKING_STYLE );
					InitParamValue( INIT_SAVE, VL_PLS_LN_DOCK, dValue ? 1 : 0 );

					//	�N�����I�[�|��
					if( IsDlgButtonChecked( hDlg, IDRB_LASTOPEN_NON ) ){	dValue = LASTOPEN_NON;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_LASTOPEN_ASK ) ){	dValue = LASTOPEN_ASK;	}
					else{	dValue = LASTOPEN_DO;	}	//	IDRB_LASTOPEN_DO
					InitParamValue( INIT_SAVE, VL_LAST_OPEN, dValue );

					//	�����x
					dValue = SendDlgItemMessage( hDlg, IDSL_LAYERBOX_TRANCED, TBM_GETPOS, 0, 0 );
					dValue += 0x1F;
					InitParamValue( INIT_SAVE, VL_LAYER_TRANS, dValue );
					LayerBoxAlphaSet( dValue );

					//	MAA�̑���
					if( IsDlgButtonChecked( hDlg, IDRB_SEL_INTRPT_EDIT )  ){	dValue = MAA_INTERRUPT;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_SET_LAYER ) ){	dValue = MAA_LAYERED;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_CLIP_UNI ) ){	dValue = MAA_UNICLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_CLIP_SJIS ) ){	dValue = MAA_SJISCLIP;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_SEL_DRAUGHT ) ){	dValue = MAA_DRAUGHT;	}
					else{	dValue = MAA_INSERT;	}	//	IDRB_SEL_INS_EDIT
					InitParamValue( INIT_SAVE, VL_SETMETHOD, dValue );
					ViewMaaItemsModeSet( dValue );	//	MAA�ɂ��ݒ肨����

#ifdef DRAUGHT_STYLE
					//	�h���t�g�{�[�h�̑���
					if( IsDlgButtonChecked( hDlg, IDRB_DRT_INTRPT_EDIT )  ){	gdClickMode = MAA_INTERRUPT;	}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRT_SET_LAYER ) ){	gdClickMode = MAA_LAYERED;		}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRT_CLIP_UNI ) ){	gdClickMode = MAA_UNICLIP;		}
					else if( IsDlgButtonChecked( hDlg, IDRB_DRT_CLIP_SJIS ) ){	gdClickMode = MAA_SJISCLIP;		}
					else{	gdClickMode = MAA_INSERT;	}	//	IDRB_DRT_INS_EDIT
					InitParamValue( INIT_SAVE, VL_DRAUGHT_MODE, gdClickMode );
#endif
					//	�n�j�Ȃ�����Ⴄ
					if( IDOK == id ){	EndDialog( hDlg, IDOK );	}
					return (INT_PTR)TRUE;

				case IDCANCEL:
					EndDialog( hDlg, IDCANCEL );
					return (INT_PTR)TRUE;

				default:	break;
			}
			break;

		default:	break;
	}

	return (INT_PTR)FALSE;
}
//-------------------------------------------------------------------------------------------------

/*!
	�f�B���N�g���I���_�C�A���O�̕\��
	@param[in]	hWnd		�e�E�C���h�E�̃n���h��
	@param[in]	ptSelDir	�f�B���N�g����������o�b�t�@�ւ̃|�C���^�[
	@param[in]	cchLen		�o�b�t�@�̕������B�o�C�g������Ȃ���
	@return		��O�F�f�B���N�g���Ƃ����@�O�F�L�����Z������
*/
BOOLEAN SelectDirectoryDlg( HWND hWnd, LPTSTR ptSelDir, UINT_PTR cchLen )
{
	BROWSEINFO		stBrowseInfo;
	LPITEMIDLIST	pstItemIDList;
	TCHAR	atDisplayName[MAX_PATH];

	if( !(ptSelDir) )	return FALSE;
	ZeroMemory( ptSelDir, sizeof(TCHAR) * cchLen );

	//	BROWSEINFO�\���̂ɒl��ݒ�
	stBrowseInfo.hwndOwner		 = hWnd;	//	�_�C�A���O�̐e�E�C���h�E�̃n���h��
	stBrowseInfo.pidlRoot		 = NULL;	//	���[�g�f�B���N�g��������ITEMIDLIST�̃|�C���^�ENULL�̏ꍇ�f�X�N�g�b�v
	stBrowseInfo.pszDisplayName	 = atDisplayName;	//	�I�����ꂽ�f�B���N�g�������󂯎��o�b�t�@�̃|�C���^
	stBrowseInfo.lpszTitle		 = TEXT("�`�`�̓����Ă�f�B���N�g����I������́[�I");	//	�c���[�r���[�̏㕔�ɕ\������镶����
	stBrowseInfo.ulFlags		 = BIF_RETURNONLYFSDIRS;	//	�\�������f�B���N�g���̎�ނ������t���O
	stBrowseInfo.lpfn			 = NULL;		//	BrowseCallbackProc�֐��̃|�C���^
	stBrowseInfo.lParam			 = (LPARAM)0;	//	�R�[���o�b�N�֐��ɓn���l

	//	�f�B���N�g���I���_�C�A���O��\��
	pstItemIDList = SHBrowseForFolder( &stBrowseInfo );
	if( !(pstItemIDList) )
	{
		//	�߂�l��NULL�̏ꍇ�A�f�B���N�g�����I�����ꂸ�Ƀ_�C�A���O������ꂽ�Ƃ�������
		return FALSE;
	}
	else
	{
		//	ItemIDList���p�X���ɕϊ�
		if( !SHGetPathFromIDList( pstItemIDList, atDisplayName ) )
		{
			//	�G���[����
			return FALSE;
		}
		//	atDisplayName�ɑI�����ꂽ�f�B���N�g���̃p�X�������Ă�
		StringCchCopy( ptSelDir, cchLen, atDisplayName );

		//	pstItemIDList���J�������߂�
		CoTaskMemFree( pstItemIDList );
	}

	return TRUE;
}
//-------------------------------------------------------------------------------------------------


//	�N�����̍ŏ��̈�̃t�@�C���^�u��ǉ�
HRESULT MultiFileTabFirst( LPTSTR ptName )
{
	TCHAR	atName[MAX_PATH];
	TCITEM		stTcItem;

	StringCchCopy( atName, MAX_PATH, ptName );
	PathStripPath( atName );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_TEXT | TCIF_PARAM;
	stTcItem.pszText = atName;
	stTcItem.lParam  = 1;	//	���j�[�N�ԍ��E���INCREMENT
	TabCtrl_InsertItem( ghFileTabWnd, 1, &stTcItem );

	TabCtrl_DeleteItem( ghFileTabWnd, 0 );

	TabCtrl_SetCurSel( ghFileTabWnd, 0 );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	��ڈȍ~�̃t�@�C����ǉ�
	@param[in]	dNumber	�ʂ��ԍ�
	@param[in]	ptName	�t�@�C����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabAppend( LPARAM dNumber, LPTSTR ptName )
{
	INT		iCount;
	TCHAR	atName[MAX_PATH];
	TCITEM		stTcItem;

	StringCchCopy( atName, MAX_PATH, ptName );
	PathStripPath( atName );

	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_TEXT | TCIF_PARAM;
	stTcItem.pszText = atName;
	stTcItem.lParam  = dNumber;	//	���j�[�N�ԍ�
	TabCtrl_InsertItem( ghFileTabWnd, iCount, &stTcItem );

	TabCtrl_SetCurSel( ghFileTabWnd, iCount );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̒ʂ��ԍ����������^�u���������ă^�u�C���f�b�N�X��Ԃ�
	@param[in]	dNumber	�ʂ��ԍ�
	@return		INT		�^�u�C���f�b�N�X�E�Y���Ȃ�������|�P
*/
INT MultiFileTabSearch( LPARAM dNumber )
{
	INT		iCount, i;
	TCITEM	stTcItem;

	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_PARAM;

	for( i = 0; iCount > i; i++ )
	{
		TabCtrl_GetItem( ghFileTabWnd, i, &stTcItem );

		if( dNumber == stTcItem.lParam )	return i;
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̒ʂ��ԍ����������^�u��I����Ԃɂ���
	@param[in]	dNumber	�ʂ��ԍ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabSelect( LPARAM dNumber )
{
	INT	iRslt;

	iRslt = MultiFileTabSearch( dNumber );

	if( 0 <= iRslt )
	{
		TabCtrl_SetCurSel( ghFileTabWnd, iRslt );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�L�[�{�[�h����̃^�u�ړ�����E�ׂ̃^�u�Ɉړ�
	@param[in]	xDir	�����F�E�ց@�����F���ց@�O�Ȃɂ����Ȃ�
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabSlide( INT xDir )
{
	INT		curSel, iCount, iTarget;
	LPARAM	dSele;
	TCITEM	stTcItem;

	if( 0 == xDir ){	return  S_FALSE;	}

	//	�^�u�̑���
	iCount = TabCtrl_GetItemCount( ghFileTabWnd );

	//	�I�����Ă�z��I������
	curSel = TabCtrl_GetCurSel( ghFileTabWnd );

	//	�ړ���^�u������
	if( 0 < xDir )
	{
		iTarget = curSel + 1;
		if( iCount <= iTarget ){	iTarget = 0;	}
	}
	else
	{
		iTarget = curSel - 1;
		if( 0 > iTarget ){	iTarget = iCount - 1;	}
	}

	//	���̃^�u��I����Ԃɂ���
	TabCtrl_SetCurSel( ghFileTabWnd, iTarget );

	//	���̃^�u�̃t�@�C���ԍ����m�F����
	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_PARAM;
	TabCtrl_GetItem( ghFileTabWnd, iTarget, &stTcItem );
	dSele = stTcItem.lParam;

	//	�Y���̃t�@�C�����t�H�[�J�X����
	DocMultiFileSelect( dSele );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�w��̒ʂ��ԍ����������^�u�̖��̂�ύX
	@param[in]	dNumber	�ʂ��ԍ�
	@param[in]	ptName	�t�@�C����
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabRename( LPARAM dNumber, LPTSTR ptName )
{
	INT	iRslt;
	TCHAR	atName[MAX_PATH];
	TCITEM		stTcItem;

	iRslt = MultiFileTabSearch( dNumber );
	if( 0 > iRslt ){	return E_OUTOFMEMORY;	}

	StringCchCopy( atName, MAX_PATH, ptName );
	PathStripPath( atName );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_TEXT;
	stTcItem.pszText = atName;
	TabCtrl_SetItem( ghFileTabWnd, iRslt, &stTcItem );

	return S_OK;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u��ۑ��������т�������
	@param[in]	dMode	��O���[�h�@�O�Z�[�u
	@param[in]	iTgt	�ǂݏo���ԍ��E�O�C���f�b�N�X�E�����Ȃ�t�@�C�����̂݊m��
	@param[out]	ptFile	�t���p�X�EMAX_PATH�ł��邱��
	@return		INT	�Z�[�u�F�O�@���[�h�F�L�^��
*/
INT InitMultiFileTabOpen( UINT dMode, INT iTgt, LPTSTR ptFile )
{
	if( dMode )
	{
		return DocMultiFileFetch( iTgt, ptFile, gatIniPath );
	}
	else
	{
		DocMultiFileStore( gatIniPath );
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------

/*!
	�J���Ă�^�u�����Commando
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT MultiFileTabClose( VOID )
{
	INT	curSel;
	LPARAM	dSele;
	TCITEM	stTcItem;

	//	�I�����Ă�z��I������
	curSel = TabCtrl_GetCurSel( ghFileTabWnd );

	ZeroMemory( &stTcItem, sizeof(TCITEM) );
	stTcItem.mask = TCIF_PARAM;
	TabCtrl_GetItem( ghFileTabWnd, curSel, &stTcItem );

	dSele = DocMultiFileDelete( ghMainWnd, stTcItem.lParam );
	if( dSele )
	{
		TabCtrl_DeleteItem( ghFileTabWnd, curSel );
		MultiFileTabSelect( dSele );
	}

	return S_OK;
}
//-------------------------------------------------------------------------------------------------





#ifdef USE_NOTIFYICON
/*!
	�^�X�N�g���C�A�C�R���ɂ΂��`��߂��[�����ڂ���
	@param[in]	ptInfo	�o���[���̖{��
	@param[in]	ptTitle	�o���[���̃^�C�g��
	@param[in]	dIconTy	�������A�C�R���A�P���A�Q�x���A�R�G���[
	@return		HRESULT	�I����ԃR�[�h
*/
HRESULT NotifyBalloonExist( LPTSTR ptInfo, LPTSTR ptTitle, DWORD dIconTy )
{
	NOTIFYICONDATA	nid;

	ZeroMemory( &nid, sizeof(NOTIFYICONDATA) );
	nid.cbSize      = sizeof(NOTIFYICONDATA);
	nid.uFlags      = NIF_INFO;
	nid.hWnd        = ghMainWnd;
	StringCchCopy( nid.szInfoTitle, 64, ptTitle );
	StringCchCopy( nid.szInfo, 256, ptInfo );
	nid.uTimeout    = 15000;	//	�����łP�T�b�\�������Ă݂�
	nid.dwInfoFlags = dIconTy;	//	�p����A�C�R���^�C�v
	Shell_NotifyIcon( NIM_MODIFY, &nid );

//	NIIF_INFO       0x00000001
//	NIIF_WARNING    0x00000002
//	NIIF_ERROR      0x00000003

	return S_OK;
}
//-------------------------------------------------------------------------------------------------
#endif




#ifdef _DEBUG
VOID OutputDebugStringPlus( DWORD rixError, LPSTR pcFile, INT rdLine, LPSTR pcFunc, LPTSTR ptFormat, ... )
{
	va_list	argp;
	TCHAR	atBuf[MAX_PATH], atOut[MAX_PATH], atFiFu[MAX_PATH], atErrMsg[MAX_PATH];
	CHAR	acFile[MAX_PATH], acFiFu[MAX_PATH];
	UINT	length;

	StringCchCopyA( acFile, MAX_PATH, pcFile );
	PathStripPathA( acFile );

	StringCchPrintfA( acFiFu, MAX_PATH, ("%s %d %s"), acFile, rdLine, pcFunc );
	length = (UINT)strlen( acFiFu );

	ZeroMemory( atFiFu, sizeof(atFiFu) );
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, acFiFu, length, atFiFu, MAX_PATH );
	//	�R�[�h�y�[�W,�����̎�ނ��w�肷��t���O,�}�b�v��������̃A�h���X,�}�b�v��������̃o�C�g��,
	//	�}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X,�o�b�t�@�̃T�C�Y

	va_start(argp, ptFormat);
	StringCchVPrintf( atBuf, MAX_PATH, ptFormat, argp );
	va_end( argp );

	StringCchPrintf( atOut, MAX_PATH, TEXT("%s @ %s\r\n"), atBuf, atFiFu );//

	OutputDebugString( atOut );

	if( rixError )
	{
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, rixError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), atErrMsg, MAX_PATH, NULL );
		//	���b�Z�[�W�ɂ͉��s���܂܂�Ă���悤��
		StringCchPrintf( atBuf, MAX_PATH, TEXT("[%d]%s"), rixError, atErrMsg );//

		OutputDebugString( atBuf );
		SetLastError( 0 );
	}
}
//-------------------------------------------------------------------------------------------------
#endif

//	Dirty Deeds Done Dirt Cheap �����̃X�^���h�Ɂu���Ƃ��e�Ղ��s���邦���Ȃ��s�ׁv�Ȃ�Ė��O��t����̂͂ǂ����Ǝv���B
