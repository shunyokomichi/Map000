//=======================================================//
//
// ���f������ [Model.cpp]
// Author : Shuhn Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "model.h"		// ���f��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����

//=======================================================//
// �R���X�g���N�^(CModel)
//=======================================================//
CModel::CModel()
{
	// �����l�ݒ�
	m_pParent		= NULL;									// �e�q�֌W(NULL���͐e����)
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �J���[
	m_bcolChange	= false;								// �J���[�ύX
}

//=======================================================//
// ����������(CModel)
//=======================================================//
HRESULT CModel::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nNumVtx;	// ���_��
	DWORD sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	// ���_�o�b�t�@�̃|�C���^

	// ���_���擾
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// �J�E���g
		// ���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// �ő�l
		if (vtx.x > m_vtxMax.x) { m_vtxMax.x = vtx.x; }	// X
		if (vtx.y > m_vtxMax.y) { m_vtxMax.y = vtx.y; }	// Y
		if (vtx.z > m_vtxMax.z) { m_vtxMax.z = vtx.z; }	// Z

		// �ŏ��l
		if (vtx.x < m_vtxMin.x) { m_vtxMin.x = vtx.x; }	// X
		if (vtx.y < m_vtxMin.y) { m_vtxMin.y = vtx.y; }	// Y
		if (vtx.z < m_vtxMin.z) { m_vtxMin.z = vtx.z; }	// Z

		// �T�C�Y���|�C���^�i�s
		pVtxBuff += sizeFVF;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=======================================================//
// �X�V����(CModel)
//=======================================================//
void CModel::Update(void)
{
	// ���ݕ�������
	if (m_rot.x > D3DX_PI)	{ m_rot.x -= D3DX_PI * 2.0f; }	// X
	if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }	// X
	if (m_rot.y > D3DX_PI)	{ m_rot.y -= D3DX_PI * 2.0f; }	// Y
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }	// Y
	if (m_rot.z > D3DX_PI)	{ m_rot.z -= D3DX_PI * 2.0f; }	// Z
	if (m_rot.z < -D3DX_PI) { m_rot.z += D3DX_PI * 2.0f; }	// Z
}

//=======================================================//
// �`�揈��(CModel)
//=======================================================//
void CModel::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX		mtxWorld;			// ���[���h�}�g���b�N�X
	D3DMATERIAL9	matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL	*pmat;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pParent != NULL)	{ mtxWorld = m_pParent->GetmtxWorld(); }			// NULL�ȊO
	else					{ pDevice->GetTransform(D3DTS_WORLD, &mtxWorld); }	// NULL��

	// ��]���f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^�擾
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	float fcol_a;
	float fcol_r;
	float fcol_g;
	float fcol_b;


	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{	// �J�E���g
		if (m_bcolChange == true)
		{	// �J���[�ύX
			fcol_a = pmat[nCntMat].MatD3D.Diffuse.a;
			pmat[nCntMat].MatD3D.Diffuse.a = m_col.a;
			fcol_r = pmat[nCntMat].MatD3D.Diffuse.r;
			pmat[nCntMat].MatD3D.Diffuse.r = m_col.r;
			fcol_g = pmat[nCntMat].MatD3D.Diffuse.g;
			pmat[nCntMat].MatD3D.Diffuse.g = m_col.g;
			fcol_b = pmat[nCntMat].MatD3D.Diffuse.b;
			pmat[nCntMat].MatD3D.Diffuse.b = m_col.b;
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
			pmat[nCntMat].MatD3D.Diffuse.r = fcol_r;
			pmat[nCntMat].MatD3D.Diffuse.g = fcol_g;
			pmat[nCntMat].MatD3D.Diffuse.b = fcol_b;
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �ύX�I��
	m_bcolChange = false;
}