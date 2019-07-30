//=======================================================//
//
// フェード処理 [Fade.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "fade.h"		// フェード
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//*******************************************************//
// マクロ定義
//*******************************************************//
#define TEXTURE_NAME	"data\\TEXTURE\\explosion000.png"	// テクスチャ
#define ADDCOLOR_A		(1.0f / 60.0f)						// フェード時のカラー設定

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
LPDIRECT3DTEXTURE9	CFade::m_pTexture	= NULL;				// テクスチャ情報へのポインタ
CFade::FADE			CFade::m_fade		= CFade::FADE_NONE;	// フェード状態

//=======================================================//
// コンストラクタ(CFade)
//=======================================================//
CFade::CFade() : CScene2D(CScene::PRIORITY_FADE, CScene::OBJTYPE_FADE)
{
	// 初期値設定
	m_nCntTimer = 0;	// タイマー
}

//=======================================================//
// 読み込み処理(CFade)
//=======================================================//
HRESULT CFade::Load(void)
{
	if (m_pTexture == NULL)
	{	// NULLの場合

		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// テクスチャ生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=======================================================//
// 開放処理(CFade)
//=======================================================//
void CFade::Unload(void)
{
	if (m_pTexture != NULL)
	{	// テクスチャの破棄
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=======================================================//
// 生成処理(CFade)
//=======================================================//
CFade *CFade::Create(CManager::MODE mode, FADE fade)
{
	// ポインタ生成
	CFade *pFade = NULL;

	// フェードしていない場合
	if (m_fade == FADE_NONE)
	{
		// NULLの場合、フェード生成
		if (pFade == NULL) { pFade = new CFade; }

		if (pFade != NULL)
		{
			pFade->Init(mode, fade);	// 初期化処理
			pFade->BindTexture(NULL);	// テクスチャ割り当て
		}

		return pFade;	// 値を返す
	}
	return NULL;		// 値を返す
}

//=======================================================//
// 初期化処理(CFade)
//=======================================================//
HRESULT CFade::Init(CManager::MODE mode, FADE fade)
{
	if (fade == FADE_OUT)
	{	// フェードアウト時、初期値設定
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else
	{	// それ以外の場合の初期値設定
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	}

	CScene2D::Init();	// 初期化処理

	m_fade = FADE_OUT;	// フェードアウト
	m_modeNext = mode;	// 次モード移行
	m_nCntTimer = 0;	// カウンタ

	return S_OK;
}

//=======================================================//
// 更新処理(CFade)
//=======================================================//
void CFade::Update(void)
{
	// カラー取得
	D3DXCOLOR col = CScene2D::Getcol();
	bool bDelete = false;	// 破棄フラグ

	if (m_fade != FADE_NONE)
	{	// フェード使用時
		if (m_fade == FADE_IN)
		{	// フェードイン
			col.a -= ADDCOLOR_A;	// 透明度上昇

			if (col.a <= 0.0f)
			{	// 透明
				col.a		= 0.0f;			// 透明度調整
				m_fade		= FADE_NONE;	// 未使用状態
				m_nCntTimer = 0;			// カウント初期化
				bDelete		= true;			// フェード破棄
			}
		}
		else if (m_fade == FADE_OUT)
		{	// フェードアウト
			col.a += ADDCOLOR_A;	// 透明度低下

			if (col.a >= 1.0f)
			{	// 不透明
				col.a = 1.0f;	// 透明度調整
				m_nCntTimer++;	// カウンタ加算

				if (m_nCntTimer > 30)
				{	// 一定フレーム到達時
					m_fade = FADE_IN;	// フェードイン
				}
				else if (m_nCntTimer == 15)
				{	// フレーム中間時
					CManager::SetMode(m_modeNext);	// モード移行
					CScene::SetStartUp(0);			// 更新範囲初期化
				}
			}
		}
	}

	CScene2D::Setcol(col);	// カラー設定
	CScene2D::Update();		// 更新処理

	// 破棄フラグ
	if (bDelete == true) { Uninit(); }	// 終了処理
}