//=======================================================//
//
// Scene3D処理 [Scene3D.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scene3d.h"	// Scene3D
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//=======================================================//
// コンストラクタ(CScene3D)
//=======================================================//
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture	= NULL;	// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;	// 頂点バッファ情報へのポインタ
	m_pParent = NULL;									// 親子関係(NULL時は親無し)

	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 方向
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// カラー
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
	m_bLeft = false;
}

//=======================================================//
// 初期化処理(CScene3D)
//=======================================================//
HRESULT CScene3D::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bLeft == false)
	{
		// 頂点情報設定
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);
	}
	else
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_size.y, m_size.z);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
		pVtx[2].pos = D3DXVECTOR3(0.0f, -m_size.y, -m_size.z);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);
	}

	/*D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// 法線計算用（正規化用、ベクトル）

	// 左下
	vec0[0] = pVtx[1].pos - pVtx[0].pos;
	vec0[1] = pVtx[2].pos - pVtx[0].pos;
	// 右上
	vec1[0] = pVtx[2].pos - pVtx[3].pos;
	vec1[1] = pVtx[1].pos - pVtx[3].pos;
	// 内積計算
	D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
	D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
	// 正規化
	D3DXVec3Normalize(&nor[0], &nor[0]);
	D3DXVec3Normalize(&nor[1], &nor[1]);

	// 法線の設定
	pVtx[0].nor = nor[0];
	pVtx[1].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[2].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[3].nor = nor[1];*/

	// 法線設定
	for (int nCntnor = 0; nCntnor < 4; nCntnor++) { pVtx[nCntnor].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f); }
	for (int nCntnor = 0; nCntnor < 4; nCntnor++) { pVtx[nCntnor].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f); }

	// 頂点カラー設定
	for (int nCntCol = 0; nCntCol < 4; nCntCol++) { pVtx[nCntCol].col = m_col; }

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(m_TexUV.x - m_TexUV.x, m_TexUV.y - m_TexUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x,			 m_TexUV.y - m_TexUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexUV.x - m_TexUV.x, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x,			 m_TexUV.y);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=======================================================//
// 終了処理(CScene3D)
//=======================================================//
void CScene3D::Uninit(void)
{
	// 頂点バッファ破棄
	if (m_pVtxBuff != NULL)
	{	// NULL以外
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// Scene開放
	CScene::SetDeath();
}

//=======================================================//
// 更新処理(CScene3D)
//=======================================================//
void CScene3D::Update(void)
{
	// 方向調整
	if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }	// X
	if (D3DX_PI < m_rot.x)	{ m_rot.x -= D3DX_PI * 2.0f; }	// X

	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }	// Y
	if (D3DX_PI < m_rot.y)	{ m_rot.y -= D3DX_PI * 2.0f; }	// Y

	if (m_rot.z < -D3DX_PI) { m_rot.z += D3DX_PI * 2.0f; }	// Z
	if (D3DX_PI < m_rot.z)	{ m_rot.z -= D3DX_PI * 2.0f; }	// Z

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bLeft == false)
	{
		// 頂点情報設定
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);
	}
	else
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, m_size.y, m_size.z);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
		pVtx[2].pos = D3DXVECTOR3(0.0f, -m_size.y, -m_size.z);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);
	}

	// 頂点カラー設定
	for (int nCntCol = 0; nCntCol < 4; nCntCol++) { pVtx[nCntCol].col = m_col; }

	/*
	D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// 法線計算用（正規化用、ベクトル）

	// 左下
	vec0[0] = pVtx[1].pos - pVtx[0].pos;
	vec0[1] = pVtx[2].pos - pVtx[0].pos;
	// 右上
	vec1[0] = pVtx[2].pos - pVtx[3].pos;
	vec1[1] = pVtx[1].pos - pVtx[3].pos;
	// 内積計算
	D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
	D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
	// 正規化
	D3DXVec3Normalize(&nor[0], &nor[0]);
	D3DXVec3Normalize(&nor[1], &nor[1]);

	// 法線の設定
	pVtx[0].nor = nor[0];
	pVtx[1].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[2].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[3].nor = nor[1];
	*/

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=======================================================//
// 描画処理(CScene3D)
//=======================================================//
void CScene3D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxView;	// 計算用マトリックス
	D3DXMATRIX		mtxWorld;			// ワールドマトリックス
	D3DXMATRIX		mtxParent;

	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetmtxWorld();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}


	if (m_scene3dType == SCENE3DTYPE_BILLEFFECTNONE || m_scene3dType == SCENE3DTYPE_BILLEFFECT)
	{//	ビルボードエフェクト
		// Zバッファへの書き込み
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ライト影響受けない
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
	if (m_scene3dType == SCENE3DTYPE_BILLEFFECT)
	{//	ビルボードエフェクト加算合成あり
		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	
	if (m_scene3dType == SCENE3DTYPE_NORMAL)
	{
		// 回転反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	// 左から順に	ビルボード,加算合成あり,加算合成なし
	else if (m_scene3dType == SCENE3DTYPE_BILLBOARD || m_scene3dType == SCENE3DTYPE_BILLEFFECT || SCENE3DTYPE_BILLEFFECTNONE)
	{
		// 逆行列
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;

		
		{	// 親がいない場合
			// 回転反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
		}
	}

	if (m_pParent != NULL)
	{	// 親がいる場合
		// 位置反映
		D3DXMatrixTranslation(&mtxTrans, mtxParent._41, mtxParent._42, mtxParent._43);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	}
	else
	{
		// 位置反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	}

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (m_scene3dType == SCENE3DTYPE_BILLEFFECT)
	{//	ビルボードエフェクト
		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	if (m_scene3dType == SCENE3DTYPE_BILLEFFECT || m_scene3dType == SCENE3DTYPE_BILLEFFECTNONE)
	{//	ビルボードエフェクト
		// Zバッファへの書き込み
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// ライト影響受けない
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	// アルファテスト無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


//=======================================================//
// アニメーション処理(CScene3D)
//=======================================================//
void CScene3D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;

	// UV変更
	m_TexUV = TexUV;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum)				+ TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum)			 + TexMoveUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum) + m_TexUV.x + TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum)			 + TexMoveUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum)				+ TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum) + m_TexUV.y + TexMoveUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum) + m_TexUV.x + TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum) + m_TexUV.y + TexMoveUV.y);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=======================================================//
// 頂点取得処理(CScene3D)
//=======================================================//
D3DXVECTOR3 CScene3D::GetVtx(int nNum)
{
	// 位置
	D3DXVECTOR3 pos;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報設定
	pos = pVtx[nNum].pos;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return pos;
}

//=======================================================//
// 法線取得処理(CScene3D)
//=======================================================//
D3DXVECTOR3 CScene3D::GetNor(int nNum)
{
	// 法線
	D3DXVECTOR3 nor;

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線設定
	nor = pVtx[nNum].nor;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return nor;
}

//=======================================================//
// 高さ取得処理(CScene3D)
//=======================================================//
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 高さ
	float fHeight = 0.0f;

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{	// ポリゴン数カウント
		bool bRange		= false;	// ポリゴン範囲内判定
		int nCntLand	= 0;		// 各ラインチェック

		for (int nCntVtx = 0; nCntVtx < 3; nCntVtx++)
		{	// ポリゴン内の3頂点をカウント
			D3DXVECTOR3 vecA, vecC;			// 計算用ベクトル（頂点同士、頂点とPOS）
			int nChack1 = 0, nChack2 = 0;	// 値調整用（誤差解消用）

			// カウント順の誤差をなくす（1*1 = 013; for = 031 → 013）
			if		(nCntVtx + 1 == 2) { nChack1 = 1; }
			else if (nCntVtx + 1 == 3) { nChack1 = -1; }
			if		(nCntVtx + 2 == 2) { nChack2 = 1; }
			else if (nCntVtx + 2 == 3) { nChack2 = -1; }

			// ベクトル計算
			vecA = pVtx[((nCntVtx + 1) % 3 + nCntPolygon) + (nChack2 * (nCntPolygon % 2))].pos - pVtx[((nCntVtx) % 3 + nCntPolygon) + (nChack1 * (nCntPolygon % 2))].pos;
			vecC = pos - pVtx[((nCntVtx) % 3 + nCntPolygon) + (nChack1 * (nCntPolygon % 2))].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{	// 範囲内（右側）にいることを確認
				nCntLand++;	// チェック数カウント

				if (nCntLand == 3)
				{	// チェック数が3
					bRange = true;		// ポリゴン内にいることを確定

					int nVtxNum = 0;	// 使用頂点番号

					if (nCntPolygon % 2 == 0)	{ nVtxNum = ((0) % 3 + nCntPolygon); }	// 左下
					else						{ nVtxNum = ((2) % 3 + nCntPolygon); }	// 右上

					// ベクトル計算
					D3DXVECTOR3 vecP = pos - pVtx[nVtxNum].pos;
					vecP.y -= pVtx[nVtxNum].pos.y;

					// 高さ計算
					fHeight = (((pVtx[nVtxNum].nor.x * vecP.x) + (pVtx[nVtxNum].nor.z * vecP.z)) / -pVtx[nVtxNum].nor.y) + pVtx[nVtxNum].pos.y;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=======================================================//
// 方向設定処理(ビルボード)(CScene3D)
//=======================================================//
void CScene3D::SetBillRot(float rot)
{
	// 方向調整
	if (rot < -D3DX_PI) { rot += D3DX_PI * 2.0f; }
	if (D3DX_PI < rot)	{ rot -= D3DX_PI * 2.0f; }

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(rot) * m_size.y - cosf(rot) * m_size.x,	 m_pos.y + sinf(rot) * m_size.x + cosf(rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(rot) * m_size.y + cosf(rot) * m_size.x,	 m_pos.y - sinf(rot) * m_size.x + cosf(rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(rot) * m_size.y - cosf(rot) * m_size.x,	 m_pos.y + sinf(rot) * m_size.x - cosf(rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(rot) * m_size.y + cosf(rot) * m_size.x,	 m_pos.y - sinf(rot) * m_size.x - cosf(rot) * m_size.y, 0.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}