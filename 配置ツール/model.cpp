//=======================================================//
//
// モデル処理 [Model.cpp]
// Author : Shuhn Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "model.h"		// モデル
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//=======================================================//
// コンストラクタ(CModel)
//=======================================================//
CModel::CModel()
{
	// 初期値設定
	m_pParent		= NULL;									// 親子関係(NULL時は親無し)
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 方向
	m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
	m_bcolChange	= false;								// カラー変更
}

//=======================================================//
// 初期化処理(CModel)
//=======================================================//
HRESULT CModel::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nNumVtx;	// 頂点数
	DWORD sizeFVF;	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファのポインタ

	// 頂点数取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// カウント
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 最大値
		if (vtx.x > m_vtxMax.x) { m_vtxMax.x = vtx.x; }	// X
		if (vtx.y > m_vtxMax.y) { m_vtxMax.y = vtx.y; }	// Y
		if (vtx.z > m_vtxMax.z) { m_vtxMax.z = vtx.z; }	// Z

		// 最小値
		if (vtx.x < m_vtxMin.x) { m_vtxMin.x = vtx.x; }	// X
		if (vtx.y < m_vtxMin.y) { m_vtxMin.y = vtx.y; }	// Y
		if (vtx.z < m_vtxMin.z) { m_vtxMin.z = vtx.z; }	// Z

		// サイズ分ポインタ進行
		pVtxBuff += sizeFVF;
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=======================================================//
// 更新処理(CModel)
//=======================================================//
void CModel::Update(void)
{
	// 現在方向調整
	if (m_rot.x > D3DX_PI)	{ m_rot.x -= D3DX_PI * 2.0f; }	// X
	if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }	// X
	if (m_rot.y > D3DX_PI)	{ m_rot.y -= D3DX_PI * 2.0f; }	// Y
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }	// Y
	if (m_rot.z > D3DX_PI)	{ m_rot.z -= D3DX_PI * 2.0f; }	// Z
	if (m_rot.z < -D3DX_PI) { m_rot.z += D3DX_PI * 2.0f; }	// Z
}

//=======================================================//
// 描画処理(CModel)
//=======================================================//
void CModel::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX		mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス
	D3DMATERIAL9	matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL	*pmat;				// マテリアルデータへのポインタ

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pParent != NULL)	{ mtxWorld = m_pParent->GetmtxWorld(); }			// NULL以外
	else					{ pDevice->GetTransform(D3DTS_WORLD, &mtxWorld); }	// NULL時

	// 回転反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックス設定
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在マテリアル取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ取得
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	float fcol_a;
	float fcol_r;
	float fcol_g;
	float fcol_b;


	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{	// カウント
		if (m_bcolChange == true)
		{	// カラー変更
			fcol_a = pmat[nCntMat].MatD3D.Diffuse.a;
			pmat[nCntMat].MatD3D.Diffuse.a = m_col.a;
			fcol_r = pmat[nCntMat].MatD3D.Diffuse.r;
			pmat[nCntMat].MatD3D.Diffuse.r = m_col.r;
			fcol_g = pmat[nCntMat].MatD3D.Diffuse.g;
			pmat[nCntMat].MatD3D.Diffuse.g = m_col.g;
			fcol_b = pmat[nCntMat].MatD3D.Diffuse.b;
			pmat[nCntMat].MatD3D.Diffuse.b = m_col.b;
		}

		// マテリアル設定
		pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

		// テクスチャ設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// オブジェクト(パーツ)描画
		m_pMesh->DrawSubset(nCntMat);

		// カラー変更
		if (m_bcolChange == true) 
		{
			pmat[nCntMat].MatD3D.Diffuse.a = fcol_a; 
			pmat[nCntMat].MatD3D.Diffuse.r = fcol_r;
			pmat[nCntMat].MatD3D.Diffuse.g = fcol_g;
			pmat[nCntMat].MatD3D.Diffuse.b = fcol_b;
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// 変更終了
	m_bcolChange = false;
}