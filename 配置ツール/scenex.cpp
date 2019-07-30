//=======================================================//
//
// SceneX���� [SceneX.h]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "scenex.h"			// SceneX
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

//=======================================================//
// �R���X�g���N�^(CSceneX)
//=======================================================//
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture		= NULL;	// �e�N�X�`�����ւ̃|�C���^
	m_pMesh			= NULL;	// ���b�V�����ւ̃|�C���^
	m_pBuffMat		= NULL;	// �}�e���A�����ւ̃|�C���^
	m_nNumMat		= NULL;	// �}�e���A�����

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �J���[
	m_bcolChange =	 false;									// �J���[�ύX
	m_Type			= XTYPE::TYPE_NONE;						// ���f���̏��
}

//=======================================================//
// ����������(CSceneX)
//=======================================================//Z
HRESULT CSceneX::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int		nNumVtx;	// ���_��
	DWORD	sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;	// ���_�o�b�t�@�̃|�C���^

	// ���_���擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{	// �J�E���g
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W���

		// �ő�l
		if (vtx.x > m_vtxMax.x) { m_vtxMax.x = vtx.x; }	// X
		if (vtx.y > m_vtxMax.y) { m_vtxMax.y = vtx.y; }	// Y
		if (vtx.z > m_vtxMax.z) { m_vtxMax.z = vtx.z; }	// Z

		// �ŏ��l
		if (vtx.x < m_vtxMin.x) { m_vtxMin.x = vtx.x; }	// X
		if (vtx.y < m_vtxMin.y) { m_vtxMin.y = vtx.y; }	// Y
		if (vtx.z < m_vtxMin.z) { m_vtxMin.z = vtx.z; }	// Z

		pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^�i�s
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=======================================================//
// �`�揈��(CSceneX)
//=======================================================//
void CSceneX::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9		matDef;				// ���݃}�e���A���ۑ��p
	D3DXMATERIAL		*pmat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]���f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^�擾
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	float fcol_a;

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{	// �J�E���g
		if (m_bcolChange == true)
		{	// �J���[�ύX(�����x)
			fcol_a = pmat[nCntMat].MatD3D.Diffuse.a;
			pmat[nCntMat].MatD3D.Diffuse.a = m_col.a;
		}

		// �}�e���A���ݒ�
		pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

		// �e�N�X�`���ݒ�
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// �I�u�W�F�N�g(�p�[�c)�`��
		m_pMesh->DrawSubset(nCntMat);

		// �J���[�ύX
		if (m_bcolChange == true) 
		{
			pmat[nCntMat].MatD3D.Diffuse.a = fcol_a;
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	m_bcolChange = false;	// �ύX�I��
}

//=======================================================//
// �T�C�Y�ݒ�(CSceneX)
//=======================================================//
void CSceneX::SetSize(LPD3DXMESH pMesh)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	m_pMesh = pMesh;

	int		nNumVtx;	// ���_��
	DWORD	sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;	// ���_�o�b�t�@�̃|�C���^

	// ���_���擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{	// �J�E���g
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W���

													// �ő�l
		if (vtx.x > m_vtxMax.x) { m_vtxMax.x = vtx.x; }	// X
		if (vtx.y > m_vtxMax.y) { m_vtxMax.y = vtx.y; }	// Y
		if (vtx.z > m_vtxMax.z) { m_vtxMax.z = vtx.z; }	// Z

														// �ŏ��l
		if (vtx.x < m_vtxMin.x) { m_vtxMin.x = vtx.x; }	// X
		if (vtx.y < m_vtxMin.y) { m_vtxMin.y = vtx.y; }	// Y
		if (vtx.z < m_vtxMin.z) { m_vtxMin.z = vtx.z; }	// Z

		pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^�i�s
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

}
