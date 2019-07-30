//=======================================================//
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "meshfield.h"	// ���b�V���t�B�[���h
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����
#include "camera.h"		// �J����
#include "game.h"		// �Q�[��

//*******************************************************//
// �}�N����`
//*******************************************************//
#define TEXTURE_NAME_1	"data\\TEXTURE\\Object\\yuka.jpg"	// �e�N�X�`��

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
LPDIRECT3DTEXTURE9	CMeshField::m_pTexture[MAX_TEX_FIELD] = {};	// �e�N�X�`�����ւ̃|�C���^
int					CMeshField::m_nNumAll = 0;		// �����z�u���̏��

//=======================================================//
//	�ǂݍ��ݏ���(CMeshField)
//=======================================================//
HRESULT CMeshField::Load(int nMaxTex)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnttex = 0; nCnttex < nMaxTex; nCnttex++)
	{
		char *cModelName = {};					// ���f�����ۑ��p
		cModelName = CGame::GetName(nCnttex, 1);	// �e�L�X�g����ǂݍ��񂾃��f�����̑��

		if (m_pTexture[nCnttex] == NULL)
		{
			// �e�N�X�`������
			D3DXCreateTextureFromFile(pDevice, cModelName, &m_pTexture[nCnttex]);
		}
	}

	return S_OK;
}

//=======================================================//
// �J������(CMeshField)
//=======================================================//
void CMeshField::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCnttex = 0; nCnttex < MAX_TEX_FIELD; nCnttex++)
	{
		if (m_pTexture[nCnttex] != NULL)
		{
			m_pTexture[nCnttex]->Release();
			m_pTexture[nCnttex] = NULL;
		}
	}
}

//=======================================================//
// ��������(CMeshField)
//=======================================================//
CMeshField *CMeshField::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex)
{
	// �|�C���^����
	CMeshField *pMeshField = NULL;

	// NULL�̏ꍇ�A�V�[������
	if (pMeshField == NULL) { pMeshField = new CMeshField; }

	if (pMeshField != NULL)
	{	// NULL�ȊO�̏ꍇ�A�����������i�ʒu�A�傫���A�F�AUV�j
		pMeshField->Init(meshType, num, pos, rot, size, col, TexUV, meshTex);
		pMeshField->BindTexture(m_pTexture[nTex]);	// �e�N�X�`���ݒ�
		pMeshField->m_nTex = nTex;
	}

	return pMeshField;
}

//=======================================================//
// ����������(CMeshField)
//=======================================================//
HRESULT CMeshField::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	CSceneMesh::SetInitAll(meshType, num, pos, rot, size, col, D3DXVECTOR2(TexUV.x, TexUV.y), meshTex);
	CSceneMesh::Init();

	m_nNum = m_nNumAll;			// ���f���̌��ݑ����z�u���̔ԍ�����(�z�u�ԍ��̋L�^)
	m_nNumAll++;				// �����z�u�̉��Z

	return S_OK;
}