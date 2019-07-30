//=======================================================//
//
// セーブアイコン処理 [saveicon.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "saveicon.h"	// セーブアイコン
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力
#include "texture.h"
//=======================================================//
// マクロ定義
//=======================================================//
#define SECOND				(60)								// 秒数
#define COL_A_TIME			(SECOND * 2.5)						// 透明度変化の時間
#define COL_A_CHANGE		(0.02f)								// 透明度変化の量

//=======================================================//
// 静的メンバ変数
//=======================================================//

//=======================================================//
// コンストラクタ(CSaveIco)
//=======================================================//
CSaveIcon::CSaveIcon() : CScene2D(PRIORITY_4, OBJTYPE_UI)
{
	// 値をデフォルトに
	m_nCounter = 0;
}
//=======================================================//
// 確保処理	
//=======================================================//
CSaveIcon *CSaveIcon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CSaveIcon *pSaveIcon = NULL;			// ポインタ

	if (pSaveIcon == NULL)
	{// NULLの場合
		pSaveIcon = new CSaveIcon;			// メモリ確保

		if (pSaveIcon != NULL)
		{// NULL以外の場合
			pSaveIcon->Init(pos, size, col);		// 初期化処理
		}
	}

	return pSaveIcon;			// 値を返す
}


//=======================================================//
// 初期化処理(CScene2D)
//=======================================================//
HRESULT CSaveIcon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// Texureの読み込み
	CScene2D::BindTexture(*CTexture::GetTexture(CTexture::TYPE_SAVE_LOGO));

	// 2Dポリゴンの初期化
	CScene2D::SetInitAll(pos, size, col, D3DXVECTOR2(1.0f, 1.0f));
	CScene2D::Init();

	// 透明度の状態
	m_Type = TYPE_OPAQUE;

	return S_OK;
}

//=======================================================//
// 終了処理(CScene2D)
//=======================================================//
void CSaveIcon::Uninit(void)
{
	CScene2D::Uninit();		// 終了処理
}

//=======================================================//
// 更新処理(CScene2D)
//=======================================================//
void CSaveIcon::Update(void)
{
	D3DXCOLOR col = CScene2D::Getcol();		// 色の取得

	switch (m_Type)
	{
	case TYPE_OPAQUE:
		// 透明から不透明へ
		m_nCounter++;		// カウンター
		col.a += COL_A_CHANGE;		// 変化量

		if (col.a >= 1.0f)
		{	// 透明度が1以上なら
			col.a = 1.0f;
		}
		if (COL_A_TIME < m_nCounter)
		{	// 時間以上なら
			m_nCounter = 0;					// カウンタ初期化
			m_Type = TYPE_TRANSPARENCE;		// タイプを変える
		}
		break;

	case TYPE_TRANSPARENCE:
		// 不透明から透明へ
		m_nCounter++;		// カウンター
		col.a -= COL_A_CHANGE;		// 変化量

		if (col.a <= 0.0f)
		{	// 透明度が1以上なら
			col.a = 0.0f;
		}
		if (COL_A_TIME < m_nCounter)
		{	// 時間以上なら
			Uninit();
			return;
		}
		break;
	}
	
	// 2Dポリゴンの更新 & 色の設定
	CScene2D::Update();
	CScene2D::Setcol(col);
}

//=======================================================//
// 描画処理(CScene2D)
//=======================================================//
void CSaveIcon::Draw(void)
{
	CScene2D::Draw();	// 描画処理

}