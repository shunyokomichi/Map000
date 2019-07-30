//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Yokomichi Shun
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"	// �����_���[
#include "debugproc.h"
#include "game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_FIRST (2.5f)		// �J�����̐����

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CCamera::CAMTYPE	CCamera::m_Type		= CAMTYPE_NONE;		// �}�E�X�ł̃J�����ړ��̃^�C�v

//*********************************************************************
//	�R���X�g���N�^
//*********************************************************************
CCamera::CCamera()
{
	// �f�t�H���g�l�ɂ���

}

//*********************************************************************
//	�f�X�g���N�^
//*********************************************************************
CCamera::~CCamera() {}

//*********************************************************************
// ����������
//*********************************************************************
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 300.0f, -400.0f);	// ���_�̏�����
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �����_�̏�����
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����(����)�̏�����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����(�ړI)�̏�����
	m_DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAngle = 0.0f;
	m_Length = m_posV - m_posR;				// �����̏�����
	m_nCounterAngle = 0;							// �ؑւ̃J�E���^�[�̏�����
	m_bSwitching = false;

	m_MousePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MousePosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_MousePosAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMouseAngle = 0.0f;
	m_fMousLength = 0.0f;

	return S_OK;
}

//*********************************************************************
// �I������
//*********************************************************************
void CCamera::Uninit(void)
{
}

//*********************************************************************
// �X�V����
//*********************************************************************
void CCamera::Update(void)
{
	if (CGame::GetTabCount() == 0)
	{
		CInputMouse *pMouse = CManager::GetInputMouse();	// ���͏��̎擾(�}�E�X)
		m_MousePos = *pMouse->GetMousePos();				// �}�E�X�̈ʒu�̎擾

		if (pMouse->GetMousePress(1) || pMouse->GetMousePress(2))
		{// �E�N���b�N���z�C�[���������Ă��鎞�Ƀ|�C���^�[�����[�v������
			pMouse->MouseLoop();

			if (pMouse->GetMousePress(1))
			{// �����_�̑���
				Move_R();
			}
			else if (pMouse->GetMousePress(2))
			{// ���_�ƒ����_�̑���(X,Y)
				Move_VR();
			}
		}
		m_MousePosOld = m_MousePos;		// �ߋ��ʒu�̍X�V

		BasicMove();					// ��{�I�Ȉړ�

		// �J�����̊g��k��
		if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_Q) == true)
		{// Q�Ŋg��
			m_Length.z += 4.0f;
		}
		if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_E) == true)
		{// E�ŏk��
			m_Length.z += -4.0f;
		}

		SetUpCamera();	// �J�����̍ŏI�X�V

		CDebugProc::Print("***********************************************************\n");
		CDebugProc::Print(" �J��������\n", m_fAngle);
		CDebugProc::Print(" Q : �J���������� (���� / %0.1f )\n", m_fAngle);
		CDebugProc::Print(" E : �J�����E���� (���� / %0.1f )\n", m_fAngle);
		CDebugProc::Print(" W : �O�ړ�       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" S : ��ړ�       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" A : ���ړ�       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" D : �E�ړ�       (X / %.1f , Z / %.1f)\n", m_posR.x, m_posR.z);
		CDebugProc::Print(" H : �`������\n");
		CDebugProc::Print(" Y : ���Ք��]\n");
		CDebugProc::Print("**********************************************************\n");
	}

}

//*********************************************************************
// �ݒ菈��
//*********************************************************************
void CCamera::SetCamera(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH /
		(float)SCREEN_HEIGHT,
		10.0f,
		20000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
//*********************************************************************
// ��{�I�Ȉړ�
//*********************************************************************
void CCamera::BasicMove(void)
{
	// ���ꂼ��̃L�[�ňړ�
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
	{	// D�ŉE�ړ�(X��)
		m_posR.x += sinf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
		m_posR.z += cosf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
	}
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
	{	// A�ō��ړ�(X��)
		m_posR.x -= sinf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
		m_posR.z -= cosf((D3DX_PI * 0.5f) + m_rot.y) * 20.0f;
	}
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_W) == true)
	{	// W�ŏ�ړ�(Z��)
		m_posR.x -= sinf((D3DX_PI)+m_rot.y) * 20.0f;
		m_posR.z -= cosf((D3DX_PI)+m_rot.y) * 20.0f;
	}
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_S) == true)
	{	// S�ŉ��ړ�(Z��)
		m_posR.x += sinf((D3DX_PI)+m_rot.y) * 20.0f;
		m_posR.z += cosf((D3DX_PI)+m_rot.y) * 20.0f;
	}
}
//*********************************************************************
// �����_�̈ړ�
//*********************************************************************
void CCamera::Move_R(void)
{
	CInputMouse	*pMouse = CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)

	if (pMouse != NULL)
	{
		//=============================
		//   �}�E�X�ŃJ��������
		//=============================
		// �����_�ʒu�ύX
		if (m_MousePosOld != m_MousePos)
		{
			m_MousePosAngle = m_MousePos - m_MousePosOld;		// �����̌v�Z

			if (m_MousePosAngle.y < 0)
			{// Y���W(��ړ�)
				if (m_rot.x > -1.5f)
				{
					if (m_MousePosAngle.y <= -15.0f)
					{
						m_MousePosAngle.y = -15.0f;
					}
					m_rot.x += (m_MousePosAngle.y * 0.005f);
				}
			}
			if (m_MousePosAngle.y > 0)
			{// Y���W(���ړ�)
				if (m_rot.x < 1.5f)
				{
					if (m_MousePosAngle.y >= 15.0f)
					{
						m_MousePosAngle.y = 15.0f;
					}
					m_rot.x += (m_MousePosAngle.y * 0.005f);
				}
			}
			if (m_MousePosAngle.x < 0)
			{// X���W(���ړ�)
				m_rot.y += (m_MousePosAngle.x * 0.005f);
			}
			if (m_MousePosAngle.x > 0)
			{// X���W(�E�ړ�)
				m_rot.y += (m_MousePosAngle.x * 0.005f);
			}
		}
	}
	CDebugProc::Print(" ���� : ( %f ,%f , %f )\n", m_MousePosAngle.x, m_MousePosAngle.y, m_MousePosAngle.z);
}
//*********************************************************************
// ���_�������_�̈ړ�
//*********************************************************************
void CCamera::Move_VR(void)
{
	CInputMouse	*pMouse = CManager::GetInputMouse();		// ���͏��̎擾(�}�E�X)

	if (pMouse != NULL)
	{
		// �����_�ʒu�ύX
		if (m_MousePosOld != m_MousePos)
		{
			m_MousePosAngle = m_MousePos - m_MousePosOld;		// �����̌v�Z

			if (m_MousePosAngle.y < 0)
			{// Y���W(���ړ�)
				if (m_MousePosAngle.y <= -15.0f)
				{
					m_MousePosAngle.y = -15.0f;
				}
				m_posR.y += m_MousePosAngle.y;
			}
			if (m_MousePosAngle.y > 0)
			{// Y���W(��ړ�)
				if (m_MousePosAngle.y >= 15.0f)
				{
					m_MousePosAngle.y = 15.0f;
				}
				m_posR.y += m_MousePosAngle.y;
			}
			if (m_MousePosAngle.x > 0)
			{// X���W(���ړ�)
				if (m_MousePosAngle.x >= 15.0f)
				{
					m_MousePosAngle.x = 15.0f;
				}
				m_posR.x += sinf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
				m_posR.z += cosf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
			}
			if (m_MousePosAngle.x < 0)
			{// X���W(�E�ړ�)
				if (m_MousePosAngle.x <= -15.0f)
				{
					m_MousePosAngle.x = -15.0f;
				}
				m_posR.x += sinf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
				m_posR.z += cosf((D3DX_PI * 0.5f) + m_rot.y)* (m_MousePosAngle.x * 2.0f);
			}
		}
	}
	CDebugProc::Print(" ���� : ( %f ,%f , %f )\n", m_MousePosAngle.x, m_MousePosAngle.y, m_MousePosAngle.z);
}
//*********************************************************************
// �J�����̍ŏI�X�V
//*********************************************************************
void CCamera::SetUpCamera(void)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;	// �v�Z�p�}�g���b�N�X

	// �J�����̒ǔ�
	D3DXMatrixIdentity(&mtxWorld);		// ���[���h�}�g���b�N�X�̏�����
	// ��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, (m_rot.x * -1.0f), m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	// �ʒu
	D3DXMatrixTranslation(&mtxTrans, m_posR.x, m_posR.y, m_posR.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	// �ʒu
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, m_Length.z);
	D3DXMatrixMultiply(&mtxTrans, &mtxTrans, &mtxWorld);		// mtxWorld�̊p�x�𗘗p���Đ���

	m_posV.x = mtxTrans._41;
	m_posV.y = mtxTrans._42;
	m_posV.z = mtxTrans._43;
}