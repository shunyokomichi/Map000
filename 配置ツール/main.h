//=======================================================//
//
// ���C������ [main.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _MAIN_H_
#define _MAIN_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>					// �E�B���h�E�Y�t�@�C��
#include "d3dx9.h"						// �`�揈���ɕK�v
#define	 DIRECTINPUT_VERSION (0x0800)	// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						// ���͏����ɕK�v
#include "xaudio2.h"					// �T�E���h�����ŕK�v
#include "xinput.h"						// Xinput�R���g���[���[�ŕK�v
#include "dshow.h"						// ���[�r�[�����ŕK�v
#include "Vmr9.h"

#include <crtdbg.h>

#include <stdio.h>						// io
#include <string.h>						// string
#include <stdlib.h>						// lib
#include <time.h>						// time
#include <stdarg.h>						// arg


//*******************************************************//
// ���C�u�����t�@�C���̃����N
//*******************************************************//
#pragma comment(lib, "d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		// DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		// �V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")		// XINPUT�ɕK�v
#pragma comment(lib, "Strmiids.lib")	// ����ɕK�v

//*******************************************************//
// �}�N����`
//*******************************************************//
#define SCREEN_WIDTH	(1280)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)			// �E�C���h�E�̍���

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@����� / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)			

//*******************************************************//
// �\���̒�`
//*******************************************************//
// �Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;					// ���_���W
	float rhw;							// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;						// ���_�J���[
	D3DXVECTOR2 tex;					// �e�N�X�`�����W
} VERTEX_2D;


// �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;					// ���_���W
	D3DXVECTOR3 nor;					// �@���x�N�g��
	D3DCOLOR col;						// ���_�J���[
	D3DXVECTOR2 tex;					// �e�N�X�`�����W
} VERTEX_3D;

//*******************************************************//
// �v���g�^�C�v�錾
//*******************************************************//

#endif