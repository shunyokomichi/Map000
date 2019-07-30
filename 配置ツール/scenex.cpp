//=======================================================//
//
// SceneX処理 [SceneX.h]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scenex.h"			// SceneX
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力

//=======================================================//
// コンストラクタ(CSceneX)
//=======================================================//
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture		= NULL;	// テクスチャ情報へのポインタ
	m_pMesh			= NULL;	// メッシュ情報へのポインタ
	m_pBuffMat		= NULL;	// マテリアル情報へのポインタ
	m_nNumMat		= NULL;	// マテリアル情報数

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 方向
	m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
	m_bcolChange =	 false;									// カラー変更
	m_Type			= XTYPE::TYPE_NONE;						// モデルの状態
}

//=======================================================//
// 初期化処理(CSceneX)
//=======================================================//Z
HRESULT CSceneX::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int		nNumVtx;	// 頂点数
	DWORD	sizeFVF;	// 頂点フォーマットのサイズ
	BYTE	*pVtxBuff;	// 頂点バッファのポインタ

	// 頂点数取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{	// カウント
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標代入

		// 最大値
		if (vtx.x > m_vtxMax.x) { m_vtxMax.x = vtx.x; }	// X
		if (vtx.y > m_vtxMax.y) { m_vtxMax.y = vtx.y; }	// Y
		if (vtx.z > m_vtxMax.z) { m_vtxMax.z = vtx.z; }	// Z

		// 最小値
		if (vtx.x < m_vtxMin.x) { m_vtxMin.x = vtx.x; }	// X
		if (vtx.y < m_vtxMin.y) { m_vtxMin.y = vtx.y; }	// Y
		if (vtx.z < m_vtxMin.z) { m_vtxMin.z = vtx.z; }	// Z

		pVtxBuff += sizeFVF;	// サイズ分ポインタ進行
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=======================================================//
// 描画処理(CSceneX)
//=======================================================//
void CSceneX::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9		matDef;				// 現在マテリアル保存用
	D3DXMATERIAL		*pmat;				// マテリアルデータへのポインタ

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在マテリアル取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ取得
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	float fcol_a;

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{	// カウント
		if (m_bcolChange == true)
		{	// カラー変更(透明度)
			fcol_a = pmat[nCntMat].MatD3D.Diffuse.a;
			pmat[nCntMat].MatD3D.Diffuse.a = m_col.a;
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
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	m_bcolChange = false;	// 変更終了
}

//=======================================================//
// サイズ設定(CSceneX)
//=======================================================//
void CSceneX::SetSize(LPD3DXMESH pMesh)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	m_pMesh = pMesh;

	int		nNumVtx;	// 頂点数
	DWORD	sizeFVF;	// 頂点フォーマットのサイズ
	BYTE	*pVtxBuff;	// 頂点バッファのポインタ

	// 頂点数取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{	// カウント
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標代入

													// 最大値
		if (vtx.x > m_vtxMax.x) { m_vtxMax.x = vtx.x; }	// X
		if (vtx.y > m_vtxMax.y) { m_vtxMax.y = vtx.y; }	// Y
		if (vtx.z > m_vtxMax.z) { m_vtxMax.z = vtx.z; }	// Z

														// 最小値
		if (vtx.x < m_vtxMin.x) { m_vtxMin.x = vtx.x; }	// X
		if (vtx.y < m_vtxMin.y) { m_vtxMin.y = vtx.y; }	// Y
		if (vtx.z < m_vtxMin.z) { m_vtxMin.z = vtx.z; }	// Z

		pVtxBuff += sizeFVF;	// サイズ分ポインタ進行
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

}
