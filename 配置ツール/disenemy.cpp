//=============================================================================
//
// �G�l�~�[�\������ [disenemy.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "disenemy.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "enemy.h"
#include "selecticon.h"
#include "debugproc.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CDsiEnemy::m_pTexture[MAX_MODEL] = {};		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				CDsiEnemy::m_pMesh[MAX_MODEL] = {};			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			CDsiEnemy::m_pBuffMat[MAX_MODEL] = {};		// �}�e���A�����ւ̃|�C���^
DWORD					CDsiEnemy::m_nNumMat[MAX_MODEL] = {};		// �}�e���A�����̐�
int						CDsiEnemy::m_nNum = 0;						// �����z�u���̏��

//*****************************************************************************
// ���[�h����
//*****************************************************************************
HRESULT CDsiEnemy::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// �e�L�X�g�ɏ����Ă��郂�f���̐�����
		char *cModelName = {};					// ���f��������p
		cModelName = CGame::GetName(nCntModel, 2);	// �e�L�X�g�ɏ����Ă��郂�f������������

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(cModelName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntModel], NULL, &m_nNumMat[nCntModel], &m_pMesh[nCntModel]);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
		m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{	// �g���Ă���Material�̐�������
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[nCntModel][tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// �e�N�X�`�����g�p���Ă���
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[nCntModel][tex])))
				{
					//MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}

		delete[] matDef;
		matDef = NULL;
	}
	return S_OK;
}

//*****************************************************************************
// �A�����[�h����
//*****************************************************************************
void CDsiEnemy::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MAX_MODEL; nCntParnet++)
	{
		// �e�N�X�`���̔j��
		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// �J�E���g
			if (m_pTexture[nCntParnet][tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pTexture[nCntParnet][tex]->Release();		// ���
				m_pTexture[nCntParnet][tex] = NULL;			// NULL��
			}
		}

		delete[] m_pTexture[nCntParnet];					// Texuter�̔j��
		m_pTexture[nCntParnet] = NULL;						// NULL��

		// �}�e���A���̊J��
		if (m_pBuffMat[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pBuffMat[nCntParnet]->Release();				// ���
			m_pBuffMat[nCntParnet] = NULL;					// NULL��
		}

		// ���b�V���̊J��
		if (m_pMesh[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pMesh[nCntParnet]->Release();					// ���
			m_pMesh[nCntParnet] = NULL;						// NULL��
		}

		m_nNumMat[nCntParnet] = 0;				// ������
	}
}

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CDsiEnemy::CDsiEnemy() : CSceneX(CScene::PRIORITY_3, CScene::OBJTYPE_SAID)
{

}

//*****************************************************************************
// �m�ۏ���
//*****************************************************************************
CDsiEnemy *CDsiEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	CDsiEnemy *PSaid = NULL;			// �|�C���^

	if (PSaid == NULL)
	{// NULL�̏ꍇ
		PSaid = new CDsiEnemy;			// �������m��

		if (PSaid != NULL)
		{// NULL�ȊO�̏ꍇ
			PSaid->Init(pos, rot, objType);		// ����������
			PSaid->SetType(XTYPE::TYPE_SAID);	// �\����Ԃ�
			PSaid->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));	// �\�����郂�f���̐F(������)
		}
	}

	return PSaid;			// �l��Ԃ�
}

//*****************************************************************************
// ����������
//*****************************************************************************
HRESULT CDsiEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	// ���f���̏�����
	CSceneX::SetInitAll(m_pTexture[objType], m_pMesh[objType], m_pBuffMat[objType], m_nNumMat[objType], pos, rot);
	CSceneX::Init();
	CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));

	// �ʒu�ƌ����̐ݒ�
	CSceneX::Setpos(pos);
	CSceneX::Setrot(rot);
	CDisplay::SetChange(D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//*****************************************************************************
// �I������
//*****************************************************************************
void CDsiEnemy::Uninit(void)
{
	CSceneX::Uninit();		// �I������
}

//*****************************************************************************
// �X�V����
//*****************************************************************************
void CDsiEnemy::Update(void)
{
	// ��{����(�ړ�, ����, ���Z�b�g)
	BasicOperation();			// �ړ�����
	BasicSetRot();				// �����ݒ菈��
	ResetButton();				// �ʒu�A��]���Z�b�g

	// ���f���I������
	SelectEnemy();

	// Enter�L�[�őI�������I�u�W�F�N�g�𐶐�
	EnemyCreate();

	// �f�o�b�N
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" �G�̔z�u\n");																	// ���f���z�u�ݒ�
	CDebugProc::Print(" ENTER ���f������             : %d �Ԗڂ𐶐�\n", m_nNum);						// �������郂�f���ԍ�
	CDebugProc::Print(" �� or �� ���f���I��            : %d / %d ��\n", m_nNum, CGame::GetNum(2));		// �I��ł郂�f���ԍ�

	// �ʒu�ݒ� & �����ݒ�
	CSceneX::Setpos(CDisplay::m_pos);
	CSceneX::Setrot(CDisplay::m_rot);
	CSceneX::Setcol(CSceneX::GetCol());

	if (CDisplay::m_bUse == true)
	{	// �\������̂��؂�ւ�鎞
		CDisplay::m_bUse = false;
		Uninit();
	}
}

//*****************************************************************************
// �`�揈��
//*****************************************************************************
void CDsiEnemy::Draw(void)
{
	CSceneX::Draw();	// �`�揈��
}

//*****************************************************************************
// ���f������
//*****************************************************************************
void CDsiEnemy::EnemyCreate()
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		CEnemy *pEnemy = NULL;		//	CObject��NULL��

		if (pEnemy == NULL)
		{	// NULL�Ȃ�
			pEnemy = CEnemy::Create(CDisplay::m_pos, CDisplay::m_rot, (CObject::OBJTYPE) m_nNum);		// �I�u�W�F�N�g�𐶐�
			int nNum = CEnemy::GetNum();											// �ő�z�u�����擾

			if (nNum == 1)
			{	// �ő�z�u����1�̏ꍇ
				CSceneX *pSceneX = ((CSceneX*)pEnemy);		// CSceneX�փL���X�g(�^�̕ύX)
				D3DXVECTOR3 size = pSceneX->Getsize(0);		// �T�C�Y�̎擾(vtxMax���擾)
				CSelectIcon::Setpos(D3DXVECTOR3(CDisplay::m_pos.x, size.y + CDisplay::m_pos.y + 20.0f, CDisplay::m_pos.z));		// X���f���̈ʒu�擾
			}
		}
	}
}

//*****************************************************************************
// ���f���I��
//*****************************************************************************
void CDsiEnemy::SelectEnemy(void)
{
	int m_MaxModel = CGame::GetNum(2);		// �ǂݍ���ł��郂�f���̍ő吔���擾

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_LEFT) == true)
	{	// ���L�[�����̏ꍇ
		m_nNum = (m_nNum + (m_MaxModel - 1)) % m_MaxModel;		// �I�����f����ς���
		CSceneX::SetInitAll(m_pTexture[m_nNum], m_pMesh[m_nNum], m_pBuffMat[m_nNum], m_nNumMat[m_nNum], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ���f���̏����ݒ�
		CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RIGHT) == true)
	{	// ���L�[�����̏ꍇ
		m_nNum = (m_nNum + 1) % m_MaxModel;		// �I�����f����ς���
		CSceneX::SetInitAll(m_pTexture[m_nNum], m_pMesh[m_nNum], m_pBuffMat[m_nNum], m_nNumMat[m_nNum], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ���f���̏����ݒ�
		CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	}
}

//*****************************************************************************
// �T�C�Y���擾
//*****************************************************************************
D3DXVECTOR3 CDsiEnemy::SetModelSize(int nSize)
{
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���f���̃T�C�Y�擾�p
	CSceneX *pSceneX = NULL;							// CSceneX�̃|�C���^

	if (pSceneX == NULL)
	{	// NULL�Ȃ�
		pSceneX = new CSceneX(PRIORITY_3, CScene::OBJTYPE_ENEMY);	// ���I�m��

		if (pSceneX != NULL)
		{	// NULL����Ȃ��Ȃ�
			pSceneX->SetSize(m_pMesh[m_nNum]);		// �T�C�Y�̐ݒ�
			size = pSceneX->Getsize(nSize);			// ���f���̃T�C�Y�擾(0 : �ő�l / 1 : �ŏ��l)
			pSceneX->Uninit();						// �I������
			pSceneX = NULL;							// NULL��
		}
	}
	return size;
}

