//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "object.h"			// �I�u�W�F�N�g
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "game.h"			//�@�Q�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CObject::m_pTexture[MAX_MODEL] = {};		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				CObject::m_pMesh[MAX_MODEL] = {};			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			CObject::m_pBuffMat[MAX_MODEL] = {};		// �}�e���A�����ւ̃|�C���^
DWORD					CObject::m_nNumMat[MAX_MODEL] = {};		// �}�e���A�����̐�
int						CObject::m_nNumAll = 0;						// �����z�u���̏��

//*****************************************************************************
// �R���X�g���N�^									
//*****************************************************************************
CObject::CObject() : CSceneX(PRIORITY_3, CScene::OBJTYPE_BLOCK)
{
	// �f�t�H���g
	m_nNum = 0;		// �����z�u��
}

//*****************************************************************************
// �f�X�g���N�^										(public)	*** CBlock ***
//*****************************************************************************
CObject::~CObject()
{

}

//*****************************************************************************
// ���[�h����							(public)	*** CBullet ***
//*****************************************************************************
HRESULT CObject::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// �e�L�X�g�ɏ����Ă��郂�f���̐������ǂݍ���
		char *cModelName = {};					// ���f�����ۑ��p
		cModelName = CGame::GetName(nCntModel, 0);	// �e�L�X�g����ǂݍ��񂾃��f�����̑��

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(cModelName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntModel], NULL, &m_nNumMat[nCntModel], &m_pMesh[nCntModel]);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
		m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{	// Texture�g���Ă��鐔���񂵂Ă���
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
// �A�����[�h����								(public)	*** CBullet ***
//*****************************************************************************
void CObject::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MAX_MODEL; nCntParnet++)
	{
		// �e�N�X�`���̔j��
		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{	// �g���Ă���Texture�̖�����
			if (m_pTexture[nCntParnet][tex] != NULL)
			{	// NULL�ȊO�̏ꍇ
				m_pTexture[nCntParnet][tex]->Release();		// ���
				m_pTexture[nCntParnet][tex] = NULL;			// NULL��
			}
		}

		delete[] m_pTexture[nCntParnet];					// Texture�̍폜
		m_pTexture[nCntParnet] = NULL;						// NULL��

		// �}�e���A���̊J��
		if (m_pBuffMat[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pBuffMat[nCntParnet]->Release();				// ���
			m_pBuffMat[nCntParnet] = NULL;					// NULL��
		}

		// �}�e���A���̊J��
		if (m_pMesh[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pMesh[nCntParnet]->Release();					// ���
			m_pMesh[nCntParnet] = NULL;						// NULL��
		}

		m_nNumMat[nCntParnet] = 0;							// ������
	}
}

//*****************************************************************************
// �m�ۏ���											(public)	*** CBlock ***
//*****************************************************************************
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	CObject *pObject = NULL;			// �|�C���^

	if (pObject == NULL)
	{	// NULL�̏ꍇ
		pObject = new CObject;			// �������m��

		if (pObject != NULL)
		{// NULL�ȊO�̏ꍇ
			pObject->Init(pos, rot, objType);		// ����������
			pObject->SetType(XTYPE::TYPE_NORMAL);	// �z�u���郂�f���̃^�C�v
		}
	}

	return pObject;			// �l��Ԃ�
}

//*****************************************************************************
// ����������										(public)	*** CBlock ***
//*****************************************************************************
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	// ���f���S�̂��܂Ƃ߂�cpp�̏�����
	CSceneX::SetInitAll(m_pTexture[objType], m_pMesh[objType], m_pBuffMat[objType], m_nNumMat[objType], pos, rot);
	CSceneX::Init();

	// �ʒu�ƌ����̐ݒ�
	CSceneX::Setpos(pos);
	CSceneX::Setrot(rot);

	// ���f���֌W
	m_nTypeNum = objType;		// ���f���̎�ޑ��
	m_nNum = m_nNumAll;			// ���f���̌��ݑ����z�u���̔ԍ�����(�z�u�ԍ��̋L�^)
	m_nNumAll++;				// �����z�u�̉��Z

	return S_OK;
}

//*****************************************************************************
// �I������											(public)	*** CBlock ***
//*****************************************************************************
void CObject::Uninit(void)
{
	// �����z�u�̌��Z
	m_nNumAll--;

	// ���f���̏I������
	CSceneX::Uninit();
}

//*****************************************************************************
// �X�V����											(public)	*** CBlock ***
//*****************************************************************************
void CObject::Update(void)
{
	CSceneX::Update();		// ���f���̍X�V����
}

//*****************************************************************************
// �`�揈��											(public)	*** CBlock ***
//*****************************************************************************
void CObject::Draw(void)
{
	CSceneX::Draw();		// ���f���̕`�揈��
}
