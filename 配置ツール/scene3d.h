//=======================================================//
//
// Scene3D処理 [Scene3D.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン
#include "scene.h"	// Scene

//*******************************************************//
// クラス定義
//*******************************************************//
class CScene3D : public CScene
{// CScene3D（親：CScene）
public:
	CScene3D(int nPriority, CScene::OBJTYPE objType);	// コンストラクタ
	~CScene3D() {};										// デストラクタ(中身無し)

	typedef enum
	{// Scene3Dタイプ
		SCENE3DTYPE_NORMAL = 0,		// 通常
		SCENE3DTYPE_BILLBOARD,		// ビルボード
		SCENE3DTYPE_BILLEFFECTNONE,	// ビルボードエフェクト用加算合成なし
		SCENE3DTYPE_BILLEFFECT,		// ビルボードエフェクト用加算合成あり
		SCENE3DTYPE_MAX				// 最大数
	}SCENE3DTYPE;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// テクスチャ反映

	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
	{	// 初期値設定
		m_pos			= pos;			// 位置
		m_rot			= rot;			// 方向
		m_size			= size;			// サイズ
		m_col			= col;			// カラー
		m_TexUV			= TexUV;		// テクスチャUV
		m_scene3dType	= scene3dType;	// Scene3Dタイプ
	}

	D3DXMATRIX GetmtxWorld(void) { return m_mtxWorld; }	// ワールドマトリックス取得
	void SetParent(CScene3D *pScene3D) { m_pParent = pScene3D; }	// 親モデル設定

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// 位置取得

	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// 方向設定
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// 方向取得

	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }	// サイズ設定
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// サイズ取得

	void Setcol(D3DXCOLOR col)		{ m_col = col; }	// カラー設定
	D3DXCOLOR Getcol(void)			{ return m_col; }	// カラー取得

	void SetPosleft(bool bLeft) { m_bLeft = bLeft; };	// 中心座標が左の場合
	bool GetPosLeft(void) { return m_bLeft; };			// 中心座標が左の場合取得

	void SetBillRot(float rot);							// ビルボード方向設定

	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);	// アニメーション情報設定

	D3DXVECTOR2 GetTex(void) { return m_TexUV; }	// UV取得
	D3DXVECTOR3 GetVtx(int nNum);					// 頂点
	D3DXVECTOR3 GetNor(int nNum);					// 法線

	float GetHeight(D3DXVECTOR3 pos);				// 高さ取得

private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ情報へのポインタ

	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 方向
	D3DXVECTOR3 m_size;			// サイズ
	D3DXCOLOR	m_col;			// カラー
	D3DXVECTOR2 m_TexUV;		// UV
	SCENE3DTYPE m_scene3dType;	// Scene3Dタイプ
	CScene3D				*m_pParent;			// 親子関係

	bool m_bLeft;
};
#endif