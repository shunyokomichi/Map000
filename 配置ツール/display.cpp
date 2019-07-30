//=============================================================================
//
// �I�u�W�F�N�g�\������ [said.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "display.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODE_MOVE			(10.0f)			// �ړ���
#define MODE_DECELERATION	(0.2f)			// ���x����
#define Y_MOVE				(1.0f)			// Y���̏グ�����̕�
#define ROT_VECTER_NUM		(4)
#define PRESS_COUNT			(10)			// �����Ă��鎞�Ԃ̏���
#define SQUARE_MOVE			(5.0f)			// ���ڈړ���
#define ADD_MOVE			(5.0f)			// �ʏ�ړ���
#define SLOWING_DOWN		(0.2f)			// ������
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
D3DXVECTOR3				CDisplay::m_pos = {};						// �ʒu
D3DXVECTOR3				CDisplay::m_rot = {};						// ����
bool					CDisplay::m_bUse = false;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CDisplay::CDisplay()
{
	m_move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �������(����)
	m_rotDest	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �������(�ړI)
	m_nNumRot	= 0;
	m_nCntPress = 0;
	m_fMove		= 0.0f;
	m_State		= STATE_NONE;
	m_bPress	= false;
	//m_SizeType	= TYPE_SIZE_BIG;
}
//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CDisplay::~CDisplay()
{
}
//*****************************************************************************
// ��{����
//*****************************************************************************
void CDisplay::BasicOperation(void)
{
	float fRot = 0.0f;		// ��]�ʂ̕ۊ�

	CCamera *pCamera = CManager::GetCamera();	// �J�����̎擾
	D3DXVECTOR3 CameraRot = pCamera->GetRot();		// �J�����̌����擾

	if (CGame::GetTabCount() == 1)
	{
		MoveChange(CameraRot);		// �ړ����@�̕ω�
	}

	if (m_State == STATE_TRIGGER) { m_bPress = true; }		// �����TRIGGER��
	else { m_bPress = false; }

	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" ��SHIFT ������						: �}�X�ڈړ�\n");								// �}�X�ڈړ�
	CDebugProc::Print(" W �O�ړ�							: ( Z     / %0.1f )\n", CDisplay::m_pos.z);		// �O�ړ�
	CDebugProc::Print(" S ��ړ�							: ( Z     / %0.1f )\n", CDisplay::m_pos.z);		// ��ړ�
	CDebugProc::Print(" A ���ړ�							: ( X     / %0.1f )\n", CDisplay::m_pos.x);		// ���ړ�
	CDebugProc::Print(" D �E�ړ�							: ( X     / %0.1f )\n", CDisplay::m_pos.x);		// �E�ړ�
	CDebugProc::Print(" R �㏸								: ( Y     / %0.1f )\n", CDisplay::m_pos.y);		// �㏸
	CDebugProc::Print(" F ���~								: ( Y     / %0.1f )\n", CDisplay::m_pos.y);		// ���~
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" ��]�ݒ�\n");																		// ��]�ݒ�
	CDebugProc::Print(" Q �E��]							: ( rot.Y / %0.1f )\n", CDisplay::m_rot.y);		// �E��]
	CDebugProc::Print(" E ����]							: ( rot.Y / %0.1f )\n", CDisplay::m_rot.y);		// ����]
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" ���Z�b�g���@\n");																	// ���Z�b�g���@
	CDebugProc::Print(" Z �ʒu�ƌ��������Z�b�g\n");															// �ʒu�ƌ��������Z�b�g
	CDebugProc::Print(" X �ʒu�����Z�b�g\n");																// �ʒu�����Z�b�g
	CDebugProc::Print(" C ���������Z�b�g\n");																// ���������Z�b�g
}

//*****************************************************************************
// ��{����(�����ݒ�)
//*****************************************************************************
void CDisplay::BasicSetRot(void)
{
	if (CGame::GetTabCount() == 1)
	{
		float fRotY = 0.0f;
		if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT))
		{
			fRotY = 0.5f;
			// ��]����
			if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_Q))
			{	// ����]
				m_nNumRot = (m_nNumRot + 1) % ROT_VECTER_NUM;		// �I�����f����ς���
			}
			else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_E))
			{	// �E��]
				m_nNumRot = (m_nNumRot + (ROT_VECTER_NUM - 1)) % ROT_VECTER_NUM;		// �I�����f����ς���
			}

			if (m_nNumRot >= ROT_VECTER_NUM - 1)
			{
				fRotY = fRotY - (0.5f * (ROT_VECTER_NUM - m_nNumRot));
			}
			m_rot.y = D3DX_PI * (0.5f * m_nNumRot);
		}
		else
		{
			if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_Q))
			{	// ����]
				m_rot.y += 0.05f;		// �I�����f����ς���
			}
			else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_E))
			{	// �E��]
				m_rot.y -= 0.05f;		// �I�����f����ς���
			}
		}

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }
	}
}

//*****************************************************************************
// ��{����(�ړ����@�I��)
//*****************************************************************************
void CDisplay::MoveChange(D3DXVECTOR3 fCameraRot)
{
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT)) { m_bFlag = true; }
	else { m_bFlag = false; }

	if (m_bFlag == true) { SquareMove(fCameraRot); }			// �}�X�ڈړ�
	else if (m_bFlag == false) { NomalMove(fCameraRot); }		// �ʏ�ړ�
}

//*****************************************************************************
// �ړ����@(���ڈړ�)
//*****************************************************************************
void CDisplay::SquareMove(D3DXVECTOR3 fCameraRot)
{
	// �ړ�
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_L))
	{	// L�L�[�����̏ꍇ
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_BIG;
			m_pos.x -= sinf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_pos.z -= cosf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_J))
	{	// J�L�[�����̏ꍇ
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_MIN;
			m_pos.x -= sinf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_pos.z -= cosf((D3DX_PI * 0.5f) + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_K))
	{	// K�L�[����
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_MIN;
			m_pos.x += sinf(D3DX_PI + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_pos.z += cosf(D3DX_PI + fCameraRot.y) * (m_sizeMin.x * m_ChangeNum.y);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_I))
	{	// I�L�[����
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_BIG;
			m_pos.x -= sinf(0 + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_pos.z -= cosf(0 + fCameraRot.y) * (m_sizeMax.x * m_ChangeNum.x);
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_UP))
	{
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_MIN;
			m_pos.y += m_sizeMin.y;
			m_State = STATE_TRIGGER;
		}
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN))
	{
		if (m_State == STATE_NONE)
		{
			m_SizeType = TYPE_SIZE_BIG;
			m_pos.y += m_sizeMax.y;
			m_State = STATE_TRIGGER;
		}
	}

	StateCounter(m_bPress);		// ���ڈړ��p�̃t���O
}

//*****************************************************************************
// �ړ����@(�ʏ�ړ�)
//*****************************************************************************
void CDisplay::NomalMove(D3DXVECTOR3 fCameraRot)
{
	// �ړ�
	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_D))
	{	// L�L�[�����̏ꍇ
		m_pos.x += sinf((D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf((D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_A))
	{	// J�L�[�����̏ꍇ
		m_pos.x += sinf((-D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf((-D3DX_PI * 0.5f) + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_S))
	{	// K�L�[����
		m_pos.x += sinf(D3DX_PI + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf(D3DX_PI + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_W))
	{	// I�L�[����
		m_pos.x += sinf(0 + fCameraRot.y) * ADD_MOVE;
		m_pos.z += cosf(0 + fCameraRot.y) * ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_R))
	{
		m_pos.y += ADD_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_F))
	{
		m_pos.y -= ADD_MOVE;
	}
}

//*****************************************************************************
// ��{����(�����I��)
//*****************************************************************************
int CDisplay::SelectNum(int nSelectNum, int nMaxOperation, int nButtan000, int nButtan001)
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(nButtan000) == true)
	{	// Z�L�[�����̏ꍇ
		nSelectNum = (nSelectNum + (nMaxOperation - 1)) % nMaxOperation;		// �I��ԍ���ς���
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(nButtan001) == true)
	{	// Z�L�[�����̏ꍇ
		nSelectNum = (nSelectNum + 1) % nMaxOperation;		// �I��ԍ���ς���
	}

	return nSelectNum;
}

//*****************************************************************************
// ���Z�b�g�{�^��
//*****************************************************************************
void CDisplay::ResetButton(void)
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_Z))
	{	// R�L�[�����������ꍇ
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu��񃊃Z�b�g
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ������񃊃Z�b�g
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_X))
	{	// B�L�[�����������ꍇ
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̃��Z�b�g
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_C))
	{	// F�L�[�����������ꍇ
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�̃��Z�b�g
	}
}

//*****************************************************************************
// ��Ԃ̊Ǘ�
//*****************************************************************************
void CDisplay::StateCounter(bool bPress)
{
	if (m_bPress == true)
	{	// ���肪true�Ȃ�
		m_nCntPress++;	//���Z
		if ((m_nCntPress % PRESS_COUNT) == 0)
		{
			m_State = STATE_NONE;
			m_nCntPress = 0;
		}
	}
}
