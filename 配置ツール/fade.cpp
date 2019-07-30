//=======================================================//
//
// �t�F�[�h���� [Fade.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "fade.h"		// �t�F�[�h
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����

//*******************************************************//
// �}�N����`
//*******************************************************//
#define TEXTURE_NAME	"data\\TEXTURE\\explosion000.png"	// �e�N�X�`��
#define ADDCOLOR_A		(1.0f / 60.0f)						// �t�F�[�h���̃J���[�ݒ�

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
LPDIRECT3DTEXTURE9	CFade::m_pTexture	= NULL;				// �e�N�X�`�����ւ̃|�C���^
CFade::FADE			CFade::m_fade		= CFade::FADE_NONE;	// �t�F�[�h���

//=======================================================//
// �R���X�g���N�^(CFade)
//=======================================================//
CFade::CFade() : CScene2D(CScene::PRIORITY_FADE, CScene::OBJTYPE_FADE)
{
	// �����l�ݒ�
	m_nCntTimer = 0;	// �^�C�}�[
}

//=======================================================//
// �ǂݍ��ݏ���(CFade)
//=======================================================//
HRESULT CFade::Load(void)
{
	if (m_pTexture == NULL)
	{	// NULL�̏ꍇ

		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// �e�N�X�`������
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=======================================================//
// �J������(CFade)
//=======================================================//
void CFade::Unload(void)
{
	if (m_pTexture != NULL)
	{	// �e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=======================================================//
// ��������(CFade)
//=======================================================//
CFade *CFade::Create(CManager::MODE mode, FADE fade)
{
	// �|�C���^����
	CFade *pFade = NULL;

	// �t�F�[�h���Ă��Ȃ��ꍇ
	if (m_fade == FADE_NONE)
	{
		// NULL�̏ꍇ�A�t�F�[�h����
		if (pFade == NULL) { pFade = new CFade; }

		if (pFade != NULL)
		{
			pFade->Init(mode, fade);	// ����������
			pFade->BindTexture(NULL);	// �e�N�X�`�����蓖��
		}

		return pFade;	// �l��Ԃ�
	}
	return NULL;		// �l��Ԃ�
}

//=======================================================//
// ����������(CFade)
//=======================================================//
HRESULT CFade::Init(CManager::MODE mode, FADE fade)
{
	if (fade == FADE_OUT)
	{	// �t�F�[�h�A�E�g���A�����l�ݒ�
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else
	{	// ����ȊO�̏ꍇ�̏����l�ݒ�
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	}

	CScene2D::Init();	// ����������

	m_fade = FADE_OUT;	// �t�F�[�h�A�E�g
	m_modeNext = mode;	// �����[�h�ڍs
	m_nCntTimer = 0;	// �J�E���^

	return S_OK;
}

//=======================================================//
// �X�V����(CFade)
//=======================================================//
void CFade::Update(void)
{
	// �J���[�擾
	D3DXCOLOR col = CScene2D::Getcol();
	bool bDelete = false;	// �j���t���O

	if (m_fade != FADE_NONE)
	{	// �t�F�[�h�g�p��
		if (m_fade == FADE_IN)
		{	// �t�F�[�h�C��
			col.a -= ADDCOLOR_A;	// �����x�㏸

			if (col.a <= 0.0f)
			{	// ����
				col.a		= 0.0f;			// �����x����
				m_fade		= FADE_NONE;	// ���g�p���
				m_nCntTimer = 0;			// �J�E���g������
				bDelete		= true;			// �t�F�[�h�j��
			}
		}
		else if (m_fade == FADE_OUT)
		{	// �t�F�[�h�A�E�g
			col.a += ADDCOLOR_A;	// �����x�ቺ

			if (col.a >= 1.0f)
			{	// �s����
				col.a = 1.0f;	// �����x����
				m_nCntTimer++;	// �J�E���^���Z

				if (m_nCntTimer > 30)
				{	// ���t���[�����B��
					m_fade = FADE_IN;	// �t�F�[�h�C��
				}
				else if (m_nCntTimer == 15)
				{	// �t���[�����Ԏ�
					CManager::SetMode(m_modeNext);	// ���[�h�ڍs
					CScene::SetStartUp(0);			// �X�V�͈͏�����
				}
			}
		}
	}

	CScene2D::Setcol(col);	// �J���[�ݒ�
	CScene2D::Update();		// �X�V����

	// �j���t���O
	if (bDelete == true) { Uninit(); }	// �I������
}