//=======================================================//
//
// �e���� [Shadow.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �C���N���[�h�t�@�C��
//*******************************************************//
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

//=======================================================//
// �ݒ菈��(CShadow)
//=======================================================//
void CShadow::SetShadow(D3DXMATRIX &mtxWorld, LPD3DXMESH &pMesh, DWORD &nNumMat, D3DXVECTOR3 pos)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g�擾
	D3DLIGHT9 Light = CManager::GetLight()->GetLightVec();
	D3DXVECTOR4 vLightDirection = D3DXVECTOR4(Light.Direction, 0.0f);

	//�e�p�ˉe�}�g���N�X�쐬
	D3DXMATRIX	mmatrixShadow;
	D3DXVECTOR3 vPoint = pos;
	D3DXVECTOR3 vNormal(0.0f, -1.0f, 0.0f);
	D3DXPLANE	GroundPlane;
	D3DXPlaneFromPointNormal(&GroundPlane, &vPoint, &vNormal);
	D3DXMatrixShadow(&mmatrixShadow, &vLightDirection, &GroundPlane);

	mmatrixShadow = mtxWorld * mmatrixShadow;

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//�e�p�}�e���A���ݒ�
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Emissive.a = 1.0f;
	Material.Emissive.r = 0.1f;
	Material.Emissive.g = 0.1f;
	Material.Emissive.b = 0.1f;

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// �e�Ƃ��ă����_�����O
	pDevice->SetTransform(D3DTS_WORLD, &mmatrixShadow);
	pDevice->SetMaterial(&Material);

	for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
	{
		pMesh->DrawSubset(nCntMat);
	}

	// ���݂̃}�e���A���߂�
	pDevice->SetMaterial(&matDef);

	// ���u�����h���s
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}