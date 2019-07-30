//=======================================================//
//
// Scene2D処理 [Scene2D.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン
#include "scene.h"	// Scene

//*******************************************************//
// クラス定義
//*******************************************************//
class CScene2D : public CScene
{// CScene2D（親：CScene）
public:
	CScene2D(int nPriority, CScene::OBJTYPE objType);	// コンストラクタ
	~CScene2D() {};										// デストラクタ(中身無し)

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// テクスチャ反映

	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
	{	// 初期値設定
		m_pos	= pos;		// 位置
		m_size	= size;		// サイズ
		m_col	= col;		// カラー
		m_TexUV = TexUV;	// UV
	}

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// 位置設定
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// 位置取得

	void Setrot(float rot)			{ m_rot = rot; }	// 方向設定
	float Getrot(void)				{ return m_rot; }	// 方向取得

	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }	// サイズ設定
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// サイズ取得

	void Setcol(D3DXCOLOR col)		{ m_col = col; }	// カラー設定
	D3DXCOLOR Getcol(void)			{ return m_col; }	// カラー取得

	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexMoveUV);	// アニメーション情報設定

private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャ情報へのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ情報へのポインタ

	D3DXVECTOR3 m_pos;		// 位置
	float		m_rot;		// 方向
	D3DXVECTOR3 m_size;		// サイズ
	D3DXCOLOR	m_col;		// カラー
	D3DXVECTOR2 m_TexUV;	// UV
};
#endif