//=======================================================//
//
//	�G���� [enemy.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "enemy.h"			// �G
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "game.h"			// �Q�[��
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "meshField.h"		// ���b�V���t�B�[���h
#include "shadow.h"			// �e
#include "input.h"			// ����
#include "object.h"			// �I�u�W�F�N�g

//*******************************************************//
// �}�N����`
//*******************************************************//
#define MODEL_NAME_0	"data\\MODEL\\Enemy\\enemy000.x"	// �G���f��(���傤����̂�����)
#define MODEL_NAME_1	"data\\MODEL\\Enemy\\enemy001.x"	// �G���f��(�P�̂�����)
#define KNOCKBACK		(30.0f)								// �_���[�W���̃m�b�N�o�b�N����
#define AREA			(400.0f)							// �G���A
#define NORMAL_MOVE		(1.0f)								// �ʏ�ړ����x
#define CHASE_MOVE		(1.5f)								// �ǔ��ړ����x
#define MAX_INVINCIBLE_TIME		(30)						// ���G���̎���
#define MAX_DAMAGE_TIME			(30)						// �_���[�W���̎���
#define MOVE					(0.1f)						// �ړ����x

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
LPDIRECT3DTEXTURE9		*CEnemy::m_pTexture[MAX_ENEMY] = {};	// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				CEnemy::m_pMesh[MAX_ENEMY] = {};		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			CEnemy::m_pBuffMat[MAX_ENEMY] = {};		// �}�e���A�����ւ̃|�C���^
DWORD					CEnemy::m_nNumMat[MAX_ENEMY] = {};		// �}�e���A�����̐�
int						CEnemy::m_nNumAll = 0;						// �����z�u���̏��

//=======================================================//
// �R���X�g���N�^
//=======================================================//
CEnemy::CEnemy() : CSceneX(CScene::PRIORITY_3, CScene::OBJTYPE_ENEMY)
{
	//	�f�t�H���g�l��
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu���(�O�̈ʒu)�ۊ�
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ��ʕϐ�
}

//=======================================================//
// �f�X�g���N�^
//=======================================================//
CEnemy::~CEnemy()
{

}

//=======================================================//
// ���[�h����
//=======================================================//
HRESULT CEnemy::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// �e�L�X�g�ɏ����Ă��郂�f���̐������ǂݍ���
		char *cModelName = {};					// ���f�����ۑ��p
		cModelName = CGame::GetName(nCntModel, 2);	// �e�L�X�g����ǂݍ��񂾃��f�����̑��

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

//=======================================================//
// �A�����[�h����
//=======================================================//
void CEnemy::Unload(void)
{
	for (int nCntModel = 0; nCntModel < 2; nCntModel++)
	{
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// �J�E���g
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pTexture[nCntModel][tex]->Release();		// ���
				m_pTexture[nCntModel][tex] = NULL;			// NULL��
			}
		}
		// �}�e���A���̊J��
		if (m_pBuffMat[nCntModel] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pBuffMat[nCntModel]->Release();				// �폜
			m_pBuffMat[nCntModel] = NULL;					// NULL��
		}

		// �}�e���A���̊J��
		if (m_pMesh[nCntModel] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pMesh[nCntModel]->Release();					// ���
			m_pMesh[nCntModel] = NULL;						// NULL��
		}

		m_nNumMat[nCntModel] = 0;				// ������
	}
}

//=======================================================//
// �m�ۏ���
//=======================================================//
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CEnemy *pEnemy = NULL;			// �|�C���^

	if (pEnemy == NULL)
	{// NULL�̏ꍇ
		pEnemy = new CEnemy;			// �������m��

		if (pEnemy != NULL)
		{// NULL�ȊO�̏ꍇ
			pEnemy->Init(pos, rot, nType);		// ����������
		}
	}

	return pEnemy;			// �l��Ԃ�
}

//=======================================================//
// ����������
//=======================================================//
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// CSceneX�̏�������
	CSceneX::SetInitAll(m_pTexture[nType], m_pMesh[nType], m_pBuffMat[nType], m_nNumMat[nType], pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CSceneX::Init();
	
	// CSceneX�Ɉʒu�ƌ�����ݒ�
	CSceneX::Setpos(pos);
	CSceneX::Setrot(rot);

	// ���f���֌W
	m_nTypeNum = nType;		// ���f���̎�ޑ��
	m_nNum = m_nNumAll;			// ���f���̌��ݑ����z�u���̔ԍ�����(�z�u�ԍ��̋L�^)
	m_nNumAll++;				// �����z�u�̉��Z


	return S_OK;
}

//=======================================================//
// �I������
//=======================================================//
void CEnemy::Uninit(void)
{
	m_nNumAll--;				// �����z�u�̉��Z

	CSceneX::Uninit();	// �I��������
}

//=======================================================//
// �X�V����
//=======================================================//
void CEnemy::Update(void)
{

}

//=======================================================//
// �`�揈��
//=======================================================//
void CEnemy::Draw(void)
{
	// X���f���̕`�揈�� & �e�̐ݒ�
	CSceneX::Draw();
}