//=======================================================//
//
// SceneX処理 [SceneX.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン
#include "scene.h"	// Scene

//*******************************************************//
// クラス定義
//*******************************************************//
class CSceneX : public CScene
{// CSceneX（親：CScene）
public:
	typedef enum 
	{
		TYPE_NONE = 0,		// 何もない状態
		TYPE_NORMAL,		// 通常状態
		TYPE_SAID,			// 表示状態
		TYPE_SELECT,		// 選択状態
		TYPE_MAX			// 総数
	}XTYPE;

	CSceneX(int nPriority, CScene::OBJTYPE objType);	// コンストラクタ
	~CSceneX() {};										// デストラクタ(中身無し)

	HRESULT Init(void);							// 初期化処理
	void Uninit(void) { CScene::SetDeath(); };	// 終了処理
	void Update(void) {};						// 更新処理(中身無し)
	void Draw(void);							// 描画処理

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	{	// 初期値設定
		m_pTexture	= pTexture;	// テクスチャ
		m_pMesh		= pMesh;	// メッシュ
		m_pBuffMat	= pBuffMat;	// マテリアルバッファ
		m_nNumMat	= nNumMat;	// マテリアル数
		m_pos		= pos;		// 位置
		m_rot		= rot;		// 方向
	}

	void SetType(XTYPE xType) { m_Type = xType; }

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// 位置取得

	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }	// 向き設定
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// 向き取得

	D3DXMATRIX GetmtxWorld(void) { return m_mtxWorld; }	// ワールドマトリックス取得

	D3DXVECTOR3 Getsize(int nNum) { if (nNum == 0) return m_vtxMax; else return m_vtxMin;  }	// サイズ取得
	void Setcol(D3DXCOLOR col)		{ m_col = col; m_bcolChange = true; }						// カラー設定
	D3DXCOLOR GetCol(void) { return m_col; }

	void SetSize(LPD3DXMESH pMesh);

private:
	LPDIRECT3DTEXTURE9	*m_pTexture = NULL;	// テクスチャ情報へのポインタ
	LPD3DXMESH			m_pMesh		= NULL;	// メッシュ情報へのポインタ
	LPD3DXBUFFER		m_pBuffMat	= NULL;	// マテリアル情報へのポインタ
	DWORD				m_nNumMat	= NULL;	// マテリアル情報の数
	D3DXMATRIX			m_mtxWorld;			// ワールドマトリックス

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// 頂点最小値,頂点最大値
	D3DXVECTOR3			m_pos,m_rot;		// 位置,方向
	D3DXCOLOR			m_col;				// カラー
	XTYPE				m_Type;				// モデルの状態
	bool				m_bcolChange;		// カラー変更
};
#endif