//=============================================================================
//
// ���\������ [disfloor.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "disfloor.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "selecticon.h"
#include "debugproc.h"
#include "game.h"
#include "meshfield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_OPERATION (3)								// �����̍ő吔

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		CDisFloor::m_pTexture[MAX_MODEL] = {};		// �e�N�X�`�����ւ̃|�C���^
int						CDisFloor::m_nNum = 0;						// �����z�u���̏��

//*****************************************************************************
// ���[�h����
//*****************************************************************************
HRESULT CDisFloor::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// �e�L�X�g�ɏ����Ă��郂�f���̐�����
		char *cModelName = {};					// ���f��������p
		cModelName = CGame::GetName(nCntModel, 1);	// �e�L�X�g�ɏ����Ă��郂�f������������

		D3DXCreateTextureFromFile(pDevice, cModelName, &m_pTexture[nCntModel]);
	}
	return S_OK;
}

//*****************************************************************************
// �A�����[�h����
//*****************************************************************************
void CDisFloor::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MAX_MODEL; nCntParnet++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCntParnet]->Release();		// ���
			m_pTexture[nCntParnet] = NULL;			// NULL��
		}
	}
}

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CDisFloor::CDisFloor() : CSceneMesh(CScene::PRIORITY_3, CScene::OBJTYPE_SAID)
{
}
//*****************************************************************************
// �m�ۏ���
//*****************************************************************************
CDisFloor *CDisFloor::Create(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex)
{
	CDisFloor *PSaid = NULL;			// �|�C���^

	if (PSaid == NULL)
	{// NULL�̏ꍇ
		PSaid = new CDisFloor;			// �������m��

		if (PSaid != NULL)
		{// NULL�ȊO�̏ꍇ
			PSaid->Init(Num, pos, rot, size, col, TexUV, meshTex, nTex);		// ����������
		}
	}

	return PSaid;			// �l��Ԃ�
}

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT CDisFloor::Init(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex)
{
	// ���b�V���t�B�[���h�̏�����
	CSceneMesh::Init();
	CSceneMesh::SetInitAll(CMeshField::MESHTYPE_FIELD, Num, pos, rot, size,col, TexUV, CMeshField::MESHTEX_NUMSIZE);
	CSceneMesh::BindTexture(m_pTexture[nTex]);
	m_SelectNum = 0;
	CDisplay::m_size = size;
	m_MeshSize = Num;
	CDisplay::SetSize(size, size);
	CDisplay::SetChange(D3DXVECTOR2(-1.0f, 1.0f));

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CDisFloor::Uninit(void)
{
	CSceneMesh::Uninit();		// �I������
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CDisFloor::Update(void)
{
	if (CGame::GetTabCount() == 1)
	{
		// ��{����(�ړ�, ����, ���Z�b�g)
		BasicOperation();			// �ړ�����
		BasicSetRot();				// �����ݒ菈��
		ResetButton();				// �ʒu�A��]���Z�b�g
		SelectNum(m_SelectNum);		// �I��ԍ�����
		FloorCreate();				// �I�������I�u�W�F�N�g�𐶐�
	}

	// �ʒu�ݒ� & �����ݒ�
	CSceneMesh::Setpos(CDisplay::m_pos);
	CSceneMesh::Setrot(CDisplay::m_rot);
	CSceneMesh::SetSize(CDisplay::m_size, m_MeshSize);
	CDisplay::SetSize(CDisplay::m_size, CDisplay::m_size);

	if(CDisplay::m_bUse == true)
	{	// �\������̂��؂�ւ�鎞
		CDisplay::m_bUse = false;
		Uninit();
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CDisFloor::Draw(void)
{
	CSceneMesh::Draw();	// �`�揈��
}

//*****************************************************************************
// Texture�I��
//*****************************************************************************
void CDisFloor::FloorCreate()
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// ���͏��̎擾(�}�E�X)

	if (pMouse->GetMouseTrigger(1))
	{
		CSceneMesh *pMesh = NULL;		//	CObject��NULL��

		if (pMesh == NULL)
		{	// NULL�Ȃ�
			pMesh = CMeshField::Create(CMeshField::MESHTYPE_FIELD, m_MeshSize, CDisplay::m_pos, CDisplay::m_rot, CDisplay::m_size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHTEX_NUMSIZE, m_nNum);
			int nNum = CMeshField::GetNum();											// �ő�z�u�����擾
			if (nNum == 1)
			{	// �ő�z�u����1�̏ꍇ
				CSceneMesh *pSceneMesh = ((CSceneMesh*)pMesh);		// CSceneX�փL���X�g(�^�̕ύX)
				D3DXVECTOR3 size = pSceneMesh->Getsize();		// �T�C�Y�̎擾(vtxMax���擾)
				CSelectIcon::Setpos(D3DXVECTOR3(CDisplay::m_pos.x, size.y + CDisplay::m_pos.y + 10.0f, CDisplay::m_pos.z));		// X���f���̈ʒu�擾
			}
		}
	}
}

//*****************************************************************************
// Texture�I��
//*****************************************************************************
void CDisFloor::SelectTex(void)
{
	int m_MaxTex = CGame::GetNum(1);		// �ǂݍ���ł��郂�f���̍ő吔���擾

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_1) == true)
	{	// ���L�[�����̏ꍇ
		m_nNum = (m_nNum + (m_MaxTex - 1)) % m_MaxTex;		// �I�����f����ς���
		CSceneMesh::BindTexture(m_pTexture[m_nNum]);
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_2) == true)
	{	// ���L�[�����̏ꍇ
		m_nNum = (m_nNum + 1) % m_MaxTex;		// �I�����f����ς���
		CSceneMesh::BindTexture(m_pTexture[m_nNum]);
	}

	// �f�o�b�N
	CDebugProc::Print(" Texture�̕ύX\n");
	CDebugProc::Print(" �� or ���L�[����              : �ԍ��I��\n");
	CDebugProc::Print(" Texture�ԍ�                    : %d / %d �Ԗڂ�I��\n", m_nNum, m_MaxTex);

}

//*****************************************************************************
// �T�C�Y�ύX
//*****************************************************************************
void CDisFloor::SetSizeChange()
{
	CDebugProc::Print(" ***********************************************************\n");

	CDebugProc::Print(" �T�C�Y�ύX\n");

	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_UP) == true)
	{	// ���L�[�����̏ꍇ
		CDisplay::m_size.z += 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN) == true)
	{	// ���L�[�����̏ꍇ
		CDisplay::m_size.z -= 1.0f;
	}
	CDebugProc::Print(" �� or ���L�[����              : Z�T�C�Y�ύX\n");

	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_RIGHT) == true)
	{	// ���L�[�����̏ꍇ
		CDisplay::m_size.x += 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LEFT) == true)
	{	// ���L�[�����̏ꍇ
		CDisplay::m_size.x -= 1.0f;
	}
	CDebugProc::Print(" �� or ���L�[����              : X�T�C�Y�ύX\n");
	

	if (CDisplay::m_size.x <= 1.0f)
	{	// �T�C�Y(X)���O��菬�����ꍇ
		CDisplay::m_size.x = 1.0f;
	}
	if (CDisplay::m_size.z <= 1.0f)
	{	// �T�C�Y(Z)���O��菬�����ꍇ
		CDisplay::m_size.z = 1.0f;
	}

	CDebugProc::Print(" �T�C�Y                            : X (%.1f) / Z (%.1f)\n", CDisplay::m_size.x, CDisplay::m_size.z);

}

//*****************************************************************************
// �I��ԍ��̏���
//*****************************************************************************
void CDisFloor::SelectNum(int nNum)
{
	m_SelectNum = CDisplay::SelectNum(m_SelectNum, 2, DIK_3, DIK_3);
	// �f�o�b�N
	CDebugProc::Print(" ***********************************************************\n");
	CDebugProc::Print("���̐���\n");
	CDebugProc::Print("�R�����ŃT�C�Y���ʐ��̐ݒ��I�ׂ�\n");
	CDebugProc::Print(" ***********************************************************\n");

	SelectOperation();		// �ԍ��ɂ���ď�����ς���
}

//*****************************************************************************
// ���b�V���t�B�[���h�̖ʐ�����
//*****************************************************************************
void CDisFloor::MeshSize()
{
	CDebugProc::Print(" ***********************************************************\n");
	CDebugProc::Print(" �ʐ��̐ݒ�\n");

	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_UP) == true)
	{	// ���L�[�����̏ꍇ
		m_MeshSize.z++;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN) == true)
	{	// ���L�[�����̏ꍇ
		m_MeshSize.z--;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_RIGHT) == true)
	{	// ���L�[�����̏ꍇ
		m_MeshSize.x++;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LEFT) == true)
	{	// ���L�[�����̏ꍇ
		m_MeshSize.x--;
	}

	if (m_MeshSize.x <= 0)
	{	// �T�C�Y(X)���O��菬�����ꍇ
		m_MeshSize.x = 1;
	}
	if (m_MeshSize.z <= 0)
	{	// �T�C�Y(Z)���O��菬�����ꍇ
		m_MeshSize.z = 1;
	}

	// �f�o�b�N
	CDebugProc::Print(" �� or ���L�[����              : X�ʐ��̕ύX\n");
	CDebugProc::Print(" �� or ���L�[����              : Z�ʐ��̕ύX\n");
	CDebugProc::Print(" �ʐ�                          : X (%.1f) / Z (%.1f)\n", m_MeshSize.x, m_MeshSize.z);

}

//*****************************************************************************
// �I��ԍ��ɂ�菈�������߂�
//*****************************************************************************
void CDisFloor::SelectOperation()
{
	SelectTex();			// Texture�ԍ���ς���

	if (m_SelectNum == 0)
	{
		SetSizeChange();		// �T�C�Y�ύX�̏���
	}
	else if (m_SelectNum == 1)
	{
		MeshSize();
	}
}
