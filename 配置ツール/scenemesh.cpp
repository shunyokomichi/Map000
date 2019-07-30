//=======================================================//
//
//	シーンメッシュ処理 [SceneMesh.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scenemesh.h"	// シーンメッシュ
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//=======================================================//
// コンストラクタ(CSceneMesh)
//=======================================================//
CSceneMesh::CSceneMesh(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture	= NULL;	// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;	// マテリアル情報へのポインタ
	m_pIdxBuff	= NULL;	// インデックス情報へのポインタ

	m_meshType	= MESHTYPE_FIELD;					// メッシュタイプ
	m_num		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 枚数
	//m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	//m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 方向
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);			// UV
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);// 色
}

//=======================================================//
// 終了処理(CSceneMesh)
//=======================================================//
void CSceneMesh::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL) { m_pVtxBuff->Release(); m_pVtxBuff = NULL; }

	// 頂点バッファの破棄
	if (m_pIdxBuff != NULL) { m_pIdxBuff->Release(); m_pIdxBuff = NULL; }

	// 開放処理(死亡フラグ)
	CScene::SetDeath();
}

//=======================================================//
// 描画処理(CSceneMesh)
//=======================================================//
void CSceneMesh::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// フィールド描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nMaxIdx, 0, m_nMaxPolygon);
}

//=======================================================//
// 初期値設定処理(CSceneMesh)
//=======================================================//
void CSceneMesh::SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_meshType	= meshType;	// メッシュタイプ
	m_meshTex	= meshTex;	// テクスチャタイプ
	m_num		= num;		// 枚数
	m_pos		= pos;		// 位置
	m_rot		= rot;		// 方向
	m_size		= size;		// サイズ
	m_TexUV		= TexUV;	// UV
	m_col		= col;			// 色

	D3DXVECTOR2 tex;		// UV計算用
	int nCnt;				// YZ枚数

	switch (meshType)
	{// メッシュタイプ
	case MESHTYPE_FIELD:	// フィールド
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.z + 1);							// 頂点数
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.z + (((int)num.z - 1) * 2);	// インデックス数
		m_nMaxPolygon = ((int)num.x * (int)num.z * 2) + (((int)num.z - 1) * 4);		// ポリゴン数

		nCnt = (int)num.z;	// Z枚数
		break;

	case MESHTYPE_WALL:		// 壁
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// 頂点数
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// インデックス数
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// ポリゴン数

		nCnt = (int)num.y;	// Y枚数
		break;

	case MESHTYPE_CYLINDER:	// シリンダー
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// 頂点数
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// インデックス数
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// ポリゴン数

		if (size.z == 0.0f) size.z = 1.0f;	// 表示面
		nCnt = (int)num.y;					// Y枚数
		break;
	}

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMaxVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// インデックスバッファ生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nMaxIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報設定
	for (int nCnt_yz = 0; nCnt_yz < nCnt + 1; nCnt_yz++)
	{	// YZカウント
		for (int nCnt_x = 0; nCnt_x < num.x + 1; nCnt_x++)
		{	// Xカウント
			switch (meshType)
			{// メッシュタイプ
			case MESHTYPE_FIELD:	// フィールド
				// 頂点情報設定
				pVtx[0].pos = D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCnt_x, 0.0f, size.z - ((size.z * 2) / num.z) * nCnt_yz);
				//pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

				switch (m_meshTex)
				{	// テクスチャタイプ
				case MESHTEX_ALL:		// 全面
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCnt_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCnt_yz));
					break;

				case MESHTEX_NUMSIZE:	// 分割
					tex = D3DXVECTOR2(TexUV.x * (nCnt_x % ((int)num.x + 1)), TexUV.y * (nCnt_yz));
					break;
				}
				// テクスチャ設定
				pVtx[0].tex = tex;

				// 法線設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_WALL:			// 壁
				// 頂点情報設定
				pVtx[0].pos = D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCnt_x, ((size.y * num.y) - (size.y * nCnt_yz)), 0.0f);

				switch (m_meshTex)
				{// テクスチャタイプ
				case MESHTEX_ALL:		// 全面
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCnt_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCnt_yz));
					break;

				case MESHTEX_NUMSIZE:	// 分割
					tex = D3DXVECTOR2(TexUV.x * (nCnt_x % ((int)num.x + 1)), TexUV.y * (nCnt_yz));
					break;
				}

				// テクスチャ設定
				pVtx[0].tex = tex;

				// 法線設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_CYLINDER:		// シリンダー
				// 頂点情報の設定
				pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCnt_x) * size.z) * size.x,	// X
										((size.y) * num.y) - (size.y * (nCnt_yz)),											// YZ
										cosf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCnt_x) * size.z) * size.x);		// X

				switch (m_meshTex)
				{// テクスチャタイプ
				case MESHTEX_ALL:		// 全面
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCnt_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCnt_yz));
					break;

				case MESHTEX_NUMSIZE:	// 分割
					tex = D3DXVECTOR2(TexUV.x * (nCnt_x % ((int)num.x + 1)), TexUV.y * (nCnt_yz));
					break;
				}
				// テクスチャ設定
				pVtx[0].tex = tex;

				// 法線設定
				D3DXVECTOR3 vecNor;
				vecNor.x = pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);	// X
				vecNor.z = pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);	// Z
				D3DXVec3Normalize(&vecNor, &vecNor);				// 正規化
				pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);
				break;
			}

			// カラー設定
			pVtx[0].col = col;

			// 頂点バッファ進行
			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// 法線計算
	SetVecNor();

	WORD *pIdx;	// インデックスデータへのポインタ

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックス設定
	for (int nCnt_yz = 0; nCnt_yz < nCnt; nCnt_yz++)
	{// YZカウント
		for (int nCnt_x = 0; nCnt_x < num.x + 1; nCnt_x++)
		{// Xカウント
		 // インデックス情報設定
			pIdx[0] = (WORD)(((nCnt_yz + 1) * (num.x + 1)) + nCnt_x);
			pIdx[1] = (WORD)(((nCnt_yz) * (num.x + 1)) + nCnt_x);

			pIdx += 2;	// インデックスバッファ進行

			if (nCnt > 1 && nCnt_yz != nCnt - 1 && nCnt_x == num.x)
			{// 末端到達時
				// インデックス情報設定（縮退ポリゴン）
				pIdx[0] = (WORD)(((nCnt_yz) * (num.x + 1)) + nCnt_x);
				pIdx[1] = (WORD)(((nCnt_yz + 2) * (num.x + 1)));

				pIdx += 2;	// インデックスバッファ進行
			}
		}
	}
	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}

//=======================================================//
// UV移動値設定処理(CSceneMesh)
//=======================================================//
void CSceneMesh::SetTex(D3DXVECTOR2 TexMoveUV)
{
	int nCnt;	// YZ枚数

	switch (m_meshType)
	{// メッシュタイプ
	case MESHTYPE_FIELD:		// フィールド
		nCnt = (int)m_num.z;	// Z枚数
		break;

	case MESHTYPE_WALL:			// 壁
		nCnt = (int)m_num.y;	// Y枚数
		break;

	case MESHTYPE_CYLINDER:		// シリンダー
		nCnt = (int)m_num.y;	// Y枚数
		break;
	}

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt_yz = 0; nCnt_yz < nCnt + 1; nCnt_yz++)
	{// YZカウント
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{// Xカウント
			// テクスチャ設定
			switch (m_meshTex)
			{// テクスチャタイプ
			case MESHTEX_ALL:		// 全面
				pVtx[0].tex = D3DXVECTOR2((m_TexUV.x / (m_num.x)) * (nCnt_x % ((int)m_num.x + 1)) + TexMoveUV.x, (m_TexUV.y / (m_num.y)) * (nCnt_yz)+TexMoveUV.y);
				break;

			case MESHTEX_NUMSIZE:	// 分割
				pVtx[0].tex = D3DXVECTOR2(m_TexUV.x * (nCnt_x % ((int)m_num.x + 1)) + TexMoveUV.x, m_TexUV.y * (nCnt_yz)+TexMoveUV.y);
				break;
			}
			pVtx += 1;	// 頂点バッファ進行
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=======================================================//
// 頂点取得処理(CSceneMesh)
//=======================================================//
D3DXVECTOR3 CSceneMesh::GetVtx(int nNum)
{
	D3DXVECTOR3 pos;	// 位置

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
// 法線取得処理(CSceneMesh)
//=======================================================//
D3DXVECTOR3 CSceneMesh::GetNor(int nNum)
{
	D3DXVECTOR3 nor;	// 法線

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
// 判定使用有無の確認処理(CSceneMesh)
//=======================================================//
bool CSceneMesh::ColInMesh(D3DXVECTOR3 pos)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	bool		bLand = false;	// 使用の有無
	D3DXVECTOR3 vecA, vecC;		// 計算用ベクトル（頂点同士、頂点とPOS）
	D3DXVECTOR3 posmtx = pos - m_pos;

	/*
	// ベクトル計算
	vecA = pVtx[(int)(m_num.x)].pos - pVtx[(int)(0)].pos;
	vecC = posmtx - pVtx[(int)(0)].pos;

	if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
	{// 範囲内（右側）にいることを確認
	 // ベクトル計算
		vecA = pVtx[(int)(m_nMaxVtx - 1)].pos - pVtx[(int)(m_num.x)].pos;
		vecC = posmtx - pVtx[(int)(m_num.x)].pos;

		if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
		{// 範囲内（右側）にいることを確認
		 // ベクトル計算
			vecA = pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos - pVtx[(int)(m_nMaxVtx - 1)].pos;
			vecC = posmtx - pVtx[(int)(m_nMaxVtx - 1)].pos;

			if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）にいることを確認
			 // ベクトル計算
				vecA = pVtx[(int)(0)].pos - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;
				vecC = posmtx - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;

				if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
				{// 範囲内（右側）にいることを確認
					bLand = true;
				}
			}
		}
	}*/

	if (m_pos.x - m_size.x <= pos.x && pos.x <= m_pos.x + m_size.x &&
		m_pos.z - m_size.z <= pos.z && pos.z <= m_pos.z + m_size.z)
	{
		bLand = true;
	}

	return bLand;
}

//=======================================================//
// サイン更新処理(CSceneMesh)
//=======================================================//
float CSceneMesh::GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int			nCntVtxNum[4];				// 使用頂点の番号保存
	D3DXVECTOR3 Hitnor[2];					// 使用頂点の法線保存
	bool		bRange_Block	= false;	// ブロック範囲内判定
	float		fUpDown			= 0.0f;		// 傾斜

	// 現在のブロックを確認
	int nHitBlock =(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
	(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	// 各ラインチェック
	int nCntLand_Block = 0;

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{	// ブロック内の4頂点をカウント
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nData = 0, nChack1 = 0, nChack2 = 0;	// 値調整用（次のカウント、誤差解消用）
			int nVtxNum_N = 0, nVtxNum = 0;				// 使用する頂点番号（次の頂点番号、現在の頂点番号）

			nData = (nCntVtx + 1) % 4;					// 次カウント確認

			// カウント順の誤差修正（2*2 = 0134; for = 0143 → 0134）
			if		(nCntVtx == 2)	{ nChack1 = 1; }
			else if (nCntVtx == 3)	{ nChack1 = -1; }
			if		(nData == 2)	{ nChack2 = 1; }
			else if (nData == 3)	{ nChack2 = -1; }

			// 頂点番号計算
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// 頂点番号保存
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// ベクトル計算
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{	// 範囲内（右側）に存在確認
				nCntLand_Block++;	// チェック数カウント

				if (nCntLand_Block == 4)
				{// チェック数が4に到達
					bRange_Block = true;	// ブロック内に存在確定

					D3DXVECTOR3 nor[2], vec0[2], vec1[2];	// 法線計算用（正規化用、ベクトル）

					// 左下
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;

					// 右上
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;

					// 内積計算
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);

					// 正規化
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// 法線設定
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{	// ブロック内に存在時
			D3DXVECTOR3 vecA, vecC;	// 計算用ベクトル（頂点同士、頂点とPOS）
			int nVtxNum = 0;		// 使用する頂点番号

			// ベクトル計算
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))	{ nVtxNum = 2; }	// 範囲内（右側）に存在確認
			else												{ nVtxNum = 1; }	// 範囲内（左側）に存在確認

			// ベクトル計算
			D3DXVECTOR3 vecP = D3DXVECTOR3(pos.x + sinf(rot.y), pos.y, pos.z + cosf(rot.y)) - pos;

			// 傾斜計算
			fUpDown = ((pVtx[nCntVtxNum[nVtxNum]].nor.x * vecP.x) + (pVtx[nCntVtxNum[nVtxNum]].nor.y * vecP.y) + (pVtx[nCntVtxNum[nVtxNum]].nor.z * vecP.z)) /
			(sqrtf((float)(pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.x, 2) + pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.y, 2) +
			pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.z, 2))) *sqrtf((float)(pow((double)vecP.x, 2) + pow((double)vecP.y, 2) + pow(vecP.z, 2))));
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fUpDown;
}

//=======================================================//
// ブロック法線取得処理(CSceneMesh)
//=======================================================//
float CSceneMesh::GetBlockNor(D3DXVECTOR3 pos)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];				// 使用頂点の番号保存
	bool bRange_Block	= false;	// ブロック範囲内判定
	float fNor			= 0.0f;		// 傾斜

	// 現在のブロックを確認
	int nHitBlock =(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
	(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;	// 各ラインチェック

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// ブロック内の4頂点をカウント
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nData = 0, nChack1 = 0, nChack2 = 0;	// 値調整用（次のカウント、誤差解消用）
			int nVtxNum_N = 0, nVtxNum = 0;				// 使用する頂点番号（次の頂点番号、現在の頂点番号）

			nData = (nCntVtx + 1) % 4;	// 次カウント確認

			// カウント順の誤差修正（2*2 = 0134; for = 0143 → 0134）
			if		(nCntVtx == 2)	{ nChack1 = 1; }
			else if (nCntVtx == 3)	{ nChack1 = -1; }
			if		(nData == 2)	{ nChack2 = 1; }
			else if (nData == 3)	{ nChack2 = -1; }

			// 頂点番号計算
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// 頂点番号保存
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// ベクトル計算
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{	// 範囲内（右側）に存在確認
				nCntLand_Block++;	// チェック数カウント

				// チェック数が4に到達
				if (nCntLand_Block == 4) { bRange_Block = true;	break; }	// ブロック内に存在確定
			}
		}

		if (bRange_Block == true)
		{// ブロック内に存在
			D3DXVECTOR3 vecA, vecC;	// 計算用ベクトル（頂点同士、頂点とPOS）
			int nVtxNum = 0;		// 使用する頂点番号

			// ベクトル計算
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))	{ nVtxNum = 2; }	// 範囲内（右側）に存在確認
			else												{ nVtxNum = 1; }	// 範囲内（左側）に存在確認

			fNor = pVtx[nCntVtxNum[nVtxNum]].nor.y;	// 傾斜法線(Y)
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fNor;
}

//=======================================================//
// 高さ設定処理(CSceneMesh)
//=======================================================//
void CSceneMesh::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報設定
	for (int nCnt_yz = 0; nCnt_yz < m_num.z + 1; nCnt_yz++)
	{	// YZカウント
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// Xカウント
			float fLength = sqrtf((float)(pow((double)(pVtx[0].pos.x - pos.x), 2) + pow((double)(pVtx[0].pos.z - pos.z), 2)));

			if (fLength < fRange)
			{
				float fPercent = fLength / fRange;

				pVtx[0].pos.y += cosf(((D3DX_PI * 0.5f) * (fPercent))) * fValue;
			}
			pVtx += 1;	// 頂点バッファ進行
		}
	}
}

//=======================================================//
// 高さ取得処理(CSceneMesh)
//=======================================================//
float CSceneMesh::GetHeight(D3DXVECTOR3 pos)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int			nCntVtxNum[4];					// 使用頂点の番号保存
	D3DXVECTOR3 Hitnor[2];						// 使用頂点の法線保存
	bool		bRange_Block	= false;		// ブロック範囲内判定
	float		fHeight			= (-500.0f);	// 高さ

	D3DXVECTOR3 posmtx = pos - m_pos;

	// 現在のブロックを確認
	int nHitBlock =(int)((posmtx.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
	(int)((int)(((posmtx.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;	// 各ラインチェック

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{	// ブロック内の4頂点をカウント
			D3DXVECTOR3 vecA, vecC;						// 計算用ベクトル（頂点同士、頂点とPOS）
			int nData = 0, nChack1 = 0, nChack2 = 0;	// 値調整用（次のカウント、誤差解消用）
			int nVtxNum_N = 0, nVtxNum = 0;				// 使用する頂点番号（次の頂点番号、現在の頂点番号）

			nData = (nCntVtx + 1) % 4;	// 次カウント確認

			// カウント順の誤差修正（2*2 = 0134; for = 0143 → 0134）
			if		(nCntVtx == 2)	{ nChack1 = 1; }
			else if (nCntVtx == 3)	{ nChack1 = -1; }
			if		(nData == 2)	{ nChack2 = 1; }
			else if (nData == 3)	{ nChack2 = -1; }

			// 頂点番号計算
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// 頂点番号保存
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// ベクトル計算
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = posmtx - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// 範囲内（右側）に存在確認
				nCntLand_Block++;	// チェック数カウント

				if (nCntLand_Block == 4)
				{// チェック数が4に到達
					bRange_Block = true;	// ブロック内に存在確定

					// 法線計算用（正規化用、ベクトル）
					D3DXVECTOR3 nor[2], vec0[2], vec1[2];

					// 左下
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;

					// 右上
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;

					// 内積計算
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);

					// 正規化
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// 法線設定
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{	// ブロック内に存在
			D3DXVECTOR3 vecA, vecC;	// 計算用ベクトル（頂点同士、頂点とPOS）
			int nVtxNum = 0;		// 使用する頂点番号

			// ベクトル計算
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = posmtx - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))	{ nVtxNum = 2; }	// 範囲内（右側）に存在確認
			else												{ nVtxNum = 1; }	// 範囲内（左側）に存在確認

			// ベクトル計算
			D3DXVECTOR3 vecP = posmtx - pVtx[nCntVtxNum[nVtxNum]].pos;
			vecP.y -= pVtx[nCntVtxNum[nVtxNum]].pos.y;

			// 高さ計算
			fHeight = (((Hitnor[nVtxNum / 2 ^ 1].x * vecP.x) + (Hitnor[nVtxNum / 2 ^ 1].z * vecP.z)) / -Hitnor[nVtxNum / 2 ^ 1].y) + pVtx[nCntVtxNum[nVtxNum]].pos.y;
			fHeight += m_pos.y;
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return fHeight;
}

//=======================================================//
// 法線計算処理(CSceneMesh)
//=======================================================//
void CSceneMesh::SetVecNor(void)
{
	/*VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	typedef struct
	{// 各ブロックの法線計算用
		D3DXVECTOR3 nor[4];
	}NORDATA;

	// ブロック数分メモリ確保
	NORDATA *norData = NULL;
	norData = new NORDATA[((m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)];

	if (norData != NULL)
	{// NULL以外だったら
		for (int nCntBlock = 0; nCntBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2; nCntBlock++)
		{// ブロック数カウント
			int nCntVtxNum[4];		// 頂点の番号計算用

			// ブロックの頂点を調べる
			nCntVtxNum[0] = (0 % 2) + (((int)m_num.x + 1) * (0 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[1] = (1 % 2) + (((int)m_num.x + 1) * (1 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[2] = (2 % 2) + (((int)m_num.x + 1) * (2 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[3] = (3 % 2) + (((int)m_num.x + 1) * (3 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));

			D3DXVECTOR3 nor[2], vec0[2], vec1[2];			// 法線計算用（正規化用、ベクトル）

			// 左下
			vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
			vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
			// 右上
			vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
			vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
			// 内積計算
			D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
			D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
			// 正規化
			D3DXVec3Normalize(&nor[0], &nor[0]);
			D3DXVec3Normalize(&nor[1], &nor[1]);

			// 法線の保存
			norData[nCntBlock].nor[0] = (nor[0] + nor[1]) * 0.5f;
			norData[nCntBlock].nor[1] = nor[1];
			norData[nCntBlock].nor[2] = nor[0];
			norData[nCntBlock].nor[3] = (nor[0] + nor[1]) * 0.5f;
		}

		for (int nCntVtx = 0; nCntVtx < m_nMaxVtx; nCntVtx++)
		{// 頂点数カウント
			D3DXVECTOR3 nor;	// 法線計算結果
			int SetBlock[4];	// ブロック番号

			// 頂点に接するブロックを調べる
			SetBlock[0] = (nCntVtx - (int)(m_num.x + 1)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[1] = (nCntVtx - (int)(m_num.x)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[2] = (nCntVtx - 1) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[3] = (nCntVtx)-(nCntVtx / (int)(m_num.x + 1));

			if (nCntVtx != 0 && nCntVtx != 0 + m_num.x && nCntVtx != (m_nMaxVtx - 1) - m_num.x && nCntVtx != (m_nMaxVtx - 1))
			{// 角以外
				if (nCntVtx <= m_num.x)
				{// 上横ライン
					nor = (norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[2] + norData[SetBlock[3]].nor[1]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == 0)
				{// 左縦ライン
					nor = (norData[SetBlock[1]].nor[2] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == (int)(m_num.x))
				{// 右縦ライン
					nor = (norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] + norData[SetBlock[2]].nor[1]) / 3;
				}
				else if ((m_nMaxVtx - 1) - m_num.x <= nCntVtx)
				{// 下横ライン
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2]) / 3;
				}
				else
				{// その他内側
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] +
						norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 6;
				}
			}
			else
			{// 角
				if (nCntVtx == 0)
				{// 左上
					nor = norData[SetBlock[3]].nor[0];
				}
				else if (nCntVtx == 0 + m_num.x)
				{// 右上
					nor = norData[SetBlock[2]].nor[1];
				}
				else if (nCntVtx == (m_nMaxVtx - 1) - m_num.x)
				{// 左下
					nor = norData[SetBlock[1]].nor[2];
				}
				else if (nCntVtx == (m_nMaxVtx - 1))
				{// 右下
					nor = norData[SetBlock[0]].nor[3];
				}
			}

			// 法線の設定
			pVtx[nCntVtx].nor = nor;
		}

		if (norData != NULL)
		{// NULL以外の場合
			delete[] norData;	// 破棄
			norData = NULL;		// NULLへ
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();*/
}

//=======================================================//
// サイン更新処理(線状)(CSceneMesh)
//=======================================================//
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 格納用
	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:		posStart = pos;												break;	// 位置
	case SINSTART_CENTER:	posStart = m_size;											break;	// 中央基軸
	case SINSTART_LU:		posStart = pVtx[0].pos;										break;	// 左上基軸
	case SINSTART_RU:		posStart = pVtx[(int)(m_num.x + 1)].pos;					break;	// 右上基軸
	case SINSTART_LD:		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;	break;	// 左下基軸
	case SINSTART_RD:		posStart = pVtx[(m_nMaxVtx - 1)].pos;						break;	// 右下基軸
	}

	// 計算用
	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2))); break;	// 円状
	case SINTYPE_RIGHT:	fMax = m_size.x * 2.0f; break;														// 右
	case SINTYPE_LEFT:	fMax = m_size.x * 2.0f; break;														// 左
	case SINTYPE_UP:	fMax = m_size.z * 2.0f; break;														// 上
	case SINTYPE_DOWN:	fMax = m_size.z * 2.0f; break;														// 下
	}

	// 頂点情報設定
	for (int nCnt_yz = 0; nCnt_yz < m_num.z + 1; nCnt_yz++)
	{	// YZカウント
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// Xカウント
			float fnow = 0.0f;

			switch (sinType)
			{
			// 円状
			case SINTYPE_CIRCLE:	fnow = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2))); break;
			case SINTYPE_RIGHT:		fnow = pVtx[0].pos.x - posStart.x; frot *= -1;	break;	// X
			case SINTYPE_LEFT:		fnow = pVtx[0].pos.x - posStart.x;				break;	// X
			case SINTYPE_UP:		fnow = pVtx[0].pos.z - posStart.z;				break;	// Z
			case SINTYPE_DOWN:		fnow = pVtx[0].pos.z - posStart.z;				break;	// Z
			}

			float fPP = fnow / fMax;
			pVtx[0].pos.y = sinf(((D3DX_PI) * (fPP * fLine)) + frot) * (fUp/* * (1.2f - fPP)*/);

			pVtx += 1;	// 頂点バッファ進行
		}
	}

	// 法線計算
	SetVecNor();
}

//=======================================================//
// サイン更新処理(範囲)(CSceneMesh)
//=======================================================//
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 格納用
	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:		posStart = pos - m_pos;										break;	// 位置
	case SINSTART_CENTER:	posStart = m_size;											break;	// 中央基軸
	case SINSTART_LU:		posStart = pVtx[0].pos;										break;	// 左上基軸
	case SINSTART_RU:		posStart = pVtx[(int)(m_num.x + 1)].pos;					break;	// 右上基軸
	case SINSTART_LD:		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;	break;	// 左下基軸
	case SINSTART_RD:		posStart = pVtx[(m_nMaxVtx - 1)].pos;						break;	// 右下基軸
	}

	// 範囲サイズ格納用
	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:	fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2))); break;	// 円状
	case SINTYPE_RIGHT:		fMax = m_size.x * 2.0f; break;														// 右
	case SINTYPE_LEFT:		fMax = m_size.x * 2.0f; break;														// 左
	case SINTYPE_UP:		fMax = m_size.z * 2.0f; break;														// 上
	case SINTYPE_DOWN:		fMax = m_size.z * 2.0f; break;														// 下
	}

	// 頂点情報設定
	for (int nCnt_yz = 0; nCnt_yz < m_num.z + 1; nCnt_yz++)
	{	// YZカウント
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// Xカウント
			float fLength = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2)));

			if (fLength < fRange)
			{
				float fPercent	= fLength / fRange;
				float fnow		= 0.0f;

				pVtx[0].pos.y = sinf(((D3DX_PI) * (fPercent * fLine)) + frot) * (fUp * (1.0f - fPercent));
			}

			pVtx += 1;	// 頂点バッファ進行
		}
	}
	// 法線計算
	SetVecNor();
}

//=======================================================//
// サイズの設定
//=======================================================//
void CSceneMesh::SetSize(D3DXVECTOR3 size, D3DXVECTOR3 nNum)
{
	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 tex;		// UV計算用
	int nCnt;				// YZ枚数
	nCnt = (int)m_num.z;	// Z枚数

	// 頂点情報設定
	for (int nCnt_yz = 0; nCnt_yz < nCnt + 1; nCnt_yz++)
	{	// YZカウント
		for (int nCnt_x = 0; nCnt_x < m_num.x + 1; nCnt_x++)
		{	// Xカウント

			// 頂点情報設定
			pVtx[0].pos = D3DXVECTOR3(-size.x + ((size.x * 2) / m_num.x) * nCnt_x, 0.0f, size.z - ((size.z * 2) / m_num.z) * nCnt_yz);
			//pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

			tex = D3DXVECTOR2(m_TexUV.x * (nCnt_x % ((int)m_num.x + 1)), m_TexUV.y * (nCnt_yz));

			// テクスチャ設定
			pVtx[0].tex = tex;

			// 法線設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 法線設定
			D3DXVECTOR3 vecNor;
			vecNor.x = m_pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);	// X
			vecNor.z = m_pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);	// Z
			D3DXVec3Normalize(&vecNor, &vecNor);				// 正規化
			pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);

			// カラー設定
			pVtx[0].col = m_col;

			// 頂点バッファ進行
			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}