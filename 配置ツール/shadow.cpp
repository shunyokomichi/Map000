//=======================================================//
//
// 影処理 [Shadow.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// インクルードファイル
//*******************************************************//
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"

//=======================================================//
// 設定処理(CShadow)
//=======================================================//
void CShadow::SetShadow(D3DXMATRIX &mtxWorld, LPD3DXMESH &pMesh, DWORD &nNumMat, D3DXVECTOR3 pos)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライト取得
	D3DLIGHT9 Light = CManager::GetLight()->GetLightVec();
	D3DXVECTOR4 vLightDirection = D3DXVECTOR4(Light.Direction, 0.0f);

	//影用射影マトリクス作成
	D3DXMATRIX	mmatrixShadow;
	D3DXVECTOR3 vPoint = pos;
	D3DXVECTOR3 vNormal(0.0f, -1.0f, 0.0f);
	D3DXPLANE	GroundPlane;
	D3DXPlaneFromPointNormal(&GroundPlane, &vPoint, &vNormal);
	D3DXMatrixShadow(&mmatrixShadow, &vLightDirection, &GroundPlane);

	mmatrixShadow = mtxWorld * mmatrixShadow;

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	// 現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//影用マテリアル設定
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Emissive.a = 1.0f;
	Material.Emissive.r = 0.1f;
	Material.Emissive.g = 0.1f;
	Material.Emissive.b = 0.1f;

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 影としてレンダリング
	pDevice->SetTransform(D3DTS_WORLD, &mmatrixShadow);
	pDevice->SetMaterial(&Material);

	for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
	{
		pMesh->DrawSubset(nCntMat);
	}

	// 現在のマテリアル戻す
	pDevice->SetMaterial(&matDef);

	// αブレンド実行
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}