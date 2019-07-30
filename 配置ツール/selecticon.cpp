//=======================================================//
//
// 選択表示処理 [Scene2D.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "selecticon.h"	// 選択アイコン
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力
#include "texture.h"

//*******************************************************//
// マクロ定義
//*******************************************************//
#define POS_Y_MOVE		(1.0f)									// 動く最大量

//*******************************************************//
// 静的メンバ変数宣言
//*******************************************************//
D3DXVECTOR3				CSelectIcon::m_pos		 = {};		// 位置情報

//=======================================================//
// コンストラクタ(CScene2D)
//=======================================================//
CSelectIcon::CSelectIcon() : CScene3D(PRIORITY_4, CScene::OBJTYPE_SELECTICON)
{
	// デフォルト
	m_Type = TYPE_NONE;		// 選択アイコンの状態(透明度)
	m_fPosY = 0.0f;			// Y軸保存用
}

//=======================================================//
// 確保処理	
//=======================================================//
CSelectIcon *CSelectIcon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CSelectIcon *pSelectIcon = NULL;			// ポインタ

	if (pSelectIcon == NULL)
	{// NULLの場合
		pSelectIcon = new CSelectIcon;			// メモリ確保

		if (pSelectIcon != NULL)
		{// NULL以外の場合
			pSelectIcon->Init(pos, size);		// 初期化処理
		}
	}
	return pSelectIcon;			// 値を返す
}

//=======================================================//
// 初期化処理(CScene2D)
//=======================================================//
HRESULT CSelectIcon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// Textureの読み込み
	CScene3D::BindTexture(*CTexture::GetTexture(CTexture::TYPE_SELECT_ICON));

	// 3Dポリゴンの初期化
	CScene3D::SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), SCENE3DTYPE_BILLBOARD);
	CScene3D::Init();

	// 位置の代入
	m_pos = pos;

	m_Type = TYPE_UP;		// 上げる状態へ

	return S_OK;
}

//=======================================================//
// 終了処理(CScene2D)
//=======================================================//
void CSelectIcon::Uninit(void)
{
	CScene3D::Uninit();		//CScene2DのUninitへ
}

//=======================================================//
// 更新処理(CScene2D)
//=======================================================//
void CSelectIcon::Update(void)
{
	switch (m_Type)
	{
	case TYPE_UP:
		// Y軸を上げる状態
		m_fPosY += 0.02f;

		if (m_fPosY <= POS_Y_MOVE)
		{	// 指定した値以下なら
			m_pos.y += m_fPosY / 3;		// 加算
		}
		else
		{
			m_Type = TYPE_DWON;		// タイプの変更(Y軸Dwon状態へ)
		}
		break;

	case TYPE_DWON:
		//　Y軸を下げる状態
		m_fPosY -= 0.02f;

		if (m_fPosY >= 0.0f)
		{	// 指定した値以上なら
			m_pos.y -= m_fPosY / 3;		// 加算
		}
		else
		{
			m_Type = TYPE_UP;		// タイプの変更(Y軸Dwon状態へ)
		}
		break;
	}

	CScene3D::Setpos(m_pos);	// 位置情報更新
	CScene3D::Update();			// 3Dポリゴンの更新
}

//=======================================================//
// 描画処理(CScene2D)
//=======================================================//
void CSelectIcon::Draw(void)
{
	CScene3D::Draw();		// 描画処理
}