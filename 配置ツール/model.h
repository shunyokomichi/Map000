//=======================================================//
//
// モデル処理 [Model.h]
// Author :
//
//=======================================================//
#ifndef _MODEL_H_
#define _MODEL_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン

//*******************************************************//
// クラス定義
//*******************************************************//
class CModel
{// CModel
public:
	CModel();		// コンストラクタ
	~CModel() {};	// デストラクタ(中身無し)

	HRESULT Init(void);		// 初期化処理
	void Uninit(void) {};	// 終了処理(中身無し)
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	{	// 初期値設定
		m_pTexture = pTexture;	// テクスチャ
		m_pMesh = pMesh;		// メッシュ
		m_pBuffMat = pBuffMat;	// マテリアルバッファ
		m_nNumMat = nNumMat;	// マテリアル数
		m_pos = pos;			// 位置
		m_rot = rot;			// 方向
	}

	D3DXMATRIX GetmtxWorld(void)	{ return m_mtxWorld; }	// ワールドマトリックス取得
	void SetParent(CModel *pModel)	{ m_pParent = pModel; }	// 親モデル設定

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// 位置取得

	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// 方向設定
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// 方向取得

	D3DXVECTOR3 Getsize(int nNum)	{ if (nNum == 0) return m_vtxMax;	else return m_vtxMin; }	// サイズ取得
	void Setcol(D3DXCOLOR col)		{ m_col = col; m_bcolChange = true; }						// カラー設定

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;		// テクスチャ情報へのポインタ
	LPD3DXMESH			m_pMesh;			// メッシュ情報へのポインタ
	LPD3DXBUFFER		m_pBuffMat;			// マテリアル情報へのポインタ
	DWORD				m_nNumMat;			// マテリアル情報数
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス
	CModel				*m_pParent;			// 親子関係

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// 頂点最小値,最大値
	D3DXVECTOR3			m_pos, m_rot;		// 位置,方向
	D3DXCOLOR			m_col;				// カラー
	bool				m_bcolChange;		// カラー変更
};
#endif