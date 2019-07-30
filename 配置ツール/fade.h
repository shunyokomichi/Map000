//=======================================================//
//
// フェード処理 [Fade.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _FADE_H_
#define _FADE_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene2d.h"	// Scene2D

//*******************************************************//
// クラス定義
//*******************************************************//
class CFade : public CScene2D
{// CFade（親：CScene2D）
public:
	typedef enum
	{	// フェード状態
		FADE_NONE = 0,	// 何もしていない
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX		// 最大数
	} FADE;

	CFade();		// コンストラクタ
	~CFade() {};	// デストラクタ(中身無し)

	static HRESULT Load(void);								// テクスチャ読み込み
	static void Unload(void);								// テクスチャ開放
	static CFade *Create(CManager::MODE mode, FADE fade);	// 生成処理

	HRESULT Init(CManager::MODE mode, FADE fade);	// 初期化処理
	void Uninit(void)	{ CScene2D::Uninit(); };	// 終了処理
	void Update(void);								// 更新処理
	void Draw(void)		{ CScene2D::Draw(); };		// 描画処理

	static FADE GetFade(void) { return m_fade; }	// フェード取得

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャ情報へのポインタ
	CManager::MODE				m_modeNext;		// モード移行
	static FADE					m_fade;			// フェード
	int							m_nCntTimer;	// タイムカウンタ
};
#endif