//=======================================================//
//
// Scene2D処理 [Scene2D.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scene2d.h"	// Scene2D
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//=======================================================//
// コンストラクタ(CScene2D)
//=======================================================//
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture	= NULL;	// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;	// マテリアル情報へのポインタ

	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot		= 0.0f;									// 方向
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// カラー
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
}

//=======================================================//
// 初期化処理(CScene2D)
//=======================================================//
HRESULT CScene2D::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);

	// 1.0f固定
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++) { pVtx[nCntrhw].rhw = 1.0f; }

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
// 終了処理(CScene2D)
//=======================================================//
void CScene2D::Uninit(void)
{
	// 頂点バッファ破棄
	if (m_pVtxBuff != NULL)
	{	// NULL以外
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// シーン開放
	CScene::SetDeath();
}

//=======================================================//
// 更新処理(CScene2D)
//=======================================================//
void CScene2D::Update(void)
{

	// 方向調整
	if (m_rot < -D3DX_PI)	{ m_rot += D3DX_PI * 2.0f; }
	if (D3DX_PI < m_rot)	{ m_rot -= D3DX_PI * 2.0f; }

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);

	// 頂点カラー設定
	for (int nCntCol = 0; nCntCol < 4; nCntCol++) { pVtx[nCntCol].col = m_col; }

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=======================================================//
// 描画処理(CScene2D)
//=======================================================//
void CScene2D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴン描画
 	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// アルファテスト無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=======================================================//
// アニメーション処理(CScene2D)
//=======================================================//
void CScene2D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

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