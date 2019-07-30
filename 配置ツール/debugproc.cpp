//**************************************************************************
//
//				�f�o�b�N�\������[debugproc.cpp]
//				Auther : Shun Yokomichi
//
//**************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include <stdarg.h >
#include <string.h>

//*********************************************************************
//	�ÓI�����o�ϐ��錾
//*********************************************************************
LPD3DXFONT		CDebugProc::m_pFont = NULL;
char			CDebugProc::m_aStr[MAX_WORD] = {};
bool CDebugProc::m_bUse = false;

//*********************************************************************
//	�R���X�g���N�^
//*********************************************************************
CDebugProc::CDebugProc()
{
	// �f�t�H���g�l�ɂ���
//	m_aStr[MAX_WORD] = {};
}

//*********************************************************************
//	�f�X�g���N�^
//*********************************************************************
CDebugProc::~CDebugProc() {}

//*********************************************************************
//	����������
//*********************************************************************
HRESULT CDebugProc::Init()
{
	//m_aStr[MAX_WORD] = {};

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "���C���I", &m_pFont);
	return S_OK;
}

//*********************************************************************
//	�I������
//*********************************************************************
void CDebugProc::Uninit()
{
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//*********************************************************************
//	�`�揈��
//*********************************************************************
void CDebugProc::Draw()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_aStr[MAX_WORD];

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
	{
		m_bUse = m_bUse ? false : true;		// �g�p�؂�ւ�
	}

	// �e�L�X�g�`��
	if (m_bUse == false)
	{
		m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	m_aStr[0] = '\0';		// �N���A
}

//*********************************************************************
//	�f�o�b�N�\��
//*********************************************************************
void CDebugProc::Print(char *fmt, ...)
{
	va_list v1;
	char cString[MAX_WORD];	// �ۊǗp��
	cString[0] = '\0';		// �����̃N���A
	va_start(v1, fmt);

	vsprintf(cString, fmt, v1);

	strcat(m_aStr, cString);	// �ۊǂ��Ă����������������Ƃ�

	va_end(v1);		// ���X�g�̏I��
}