//=======================================================//
//
// マネージャー処理 [Manager.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "scene.h"			// Scene
#include "camera.h"			// カメラ
#include "light.h"			// ライト
#include "fade.h"			// フェード
#include "game.h"			// ゲーム
#include "sound.h"			// サウンド
#include "debugproc.h"		// デバック
#include "texture.h"
//*******************************************************//
// マクロ定義
//*******************************************************//
#define MODE_0	(MODE_TITLE)	// タイトル
#define MODE_1	(MODE_TUTORIAL)	// チュートリアル
#define MODE_2	(MODE_GAME)		// ゲーム
#define MODE_3	(MODE_RESULT)	// リザルト
#define MODE_4	(MODE_RANKING)	// ランキング(現在無し)
#define MODE_OPTION	(MODE_2)	// モード設定

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
CManager::MODE	CManager	::m_mode			= CManager::MODE_NONE;	// モード
CRenderer		*CManager	::m_pRenderer		= NULL;	// レンダラー
CInputKeyboard	*CManager	::m_pInputKeyboard	= NULL;	// キーボード
CInputJoypad	*CManager	::m_pInputJoypad	= NULL;	// コントローラー
//CInputDInputPad	*CManager	::m_pInputDIPad		= NULL;	// DInputパッド
//CInputXPad		*CManager	::m_pInputXPad		= NULL;	// Xパッド
CInputMouse		*CManager	::m_pInputMouse		= NULL;	// マウス
CCamera			*CManager	::m_pCamara			= NULL;	// カメラ
CLight			*CManager	::m_pLight			= NULL;	// ライト
CGame			*CManager	::m_pGame			= NULL;	// ゲーム
CPause			*CManager	::m_pPause			= NULL;	// ポーズ
CSound			*CManager	::m_pSound			= NULL;	// サウンド
int				CManager	::m_nPlayNum		= 0;	// プレイ人数
CDebugProc		*CManager	::m_pDebug			= NULL;	// デバック

//=======================================================//
// 初期化処理(CManager)
//=======================================================//
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	//********************************************//
	//
	//	* 各フレームワークのメモリ確保・初期化処理
	//	[NULLの場合、メモリ確保]
	//	[確保完了後、初期化処理]
	//	* ERROR発生時用のメッセージBOXあり
	//
	//********************************************//

	// サウンド
	if (m_pSound == NULL)			{ m_pSound = new CSound; }
	else							{ MessageBox(0, "(manager「Init] : SoundがNULLではない", "ERROR", MB_OK); }
	if (m_pSound != NULL)			{ if (FAILED(m_pSound->Init(hWnd))) { return -1; } }
	else							{ MessageBox(0, "(manager「Init] : SoundがNULL", "ERROR", MB_OK); }

	//キーボード
	if (m_pInputKeyboard == NULL)	{ m_pInputKeyboard = CInputKeyboard::Create(hInstance, hWnd); }
	else							{ MessageBox(0, "(manager「Init] : InputKeyboardがNULLではない", "ERROR", MB_OK); }

	//	コントローラー
	if (m_pInputJoypad == NULL)		{ m_pInputJoypad = CInputJoypad::Create(hInstance, hWnd); }
	else							{ MessageBox(0, "(manager「Init] : InputJoypadがNULLではない", "ERROR", MB_OK); }
	//	マウス
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = CInputMouse::Create(hInstance, hWnd);
	}
	else { MessageBox(0, "(manager「Init] : InputMouseがNULLではない", "ERROR", MB_OK); }

	//// キーボード
	//if (m_pInputKeyboard == NULL)	{ m_pInputKeyboard = new CInputKeyboard; }
	//else							{ MessageBox(0, "(manager「Init] : InputKeyboardがNULLではない", "ERROR", MB_OK); }
	//if (m_pInputKeyboard != NULL)	{ if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd))) { return -1; } }
	//else							{ MessageBox(0, "(manager「Init] : InputKeyboardがNULL", "ERROR", MB_OK); }

	//if (m_pInputDIPad == NULL)		{ m_pInputDIPad = new CInputDInputPad; }
	//else							{ MessageBox(0, "(manager「Init] : InputDIPadがNULLではない", "ERROR", MB_OK); }
	//if (m_pInputDIPad != NULL)		{ if (FAILED(m_pInputDIPad->Init(hInstance, hWnd))) { return -1; } }
	//else							{ MessageBox(0, "(manager「Init] : InputDIPadがNULL", "ERROR", MB_OK); }

	//// Xパッド
	//if (m_pInputXPad == NULL)		{ m_pInputXPad = new CInputXPad; }
	//else							{ MessageBox(0, "(manager「Init] : InputXPadがNULLではない", "警告", MB_OK); }
	//if (m_pInputXPad != NULL)		{ if (FAILED(m_pInputXPad->Init(hInstance, hWnd))) { return -1; } }
	//else							{ MessageBox(0, "(manager「Init] : InputXPadがNULL", "ERROR", MB_OK); }

	// マウス
	//if (m_pInputMouse == NULL) { m_pInputMouse = new CInputMouse; }
	//else { MessageBox(0, "(manager「Init] : InputMouseがNULLではない", "警告", MB_OK); }
	//if (m_pInputMouse != NULL) { if (FAILED(m_pInputMouse->Init(hInstance, hWnd))) { return -1; } }
	//else { MessageBox(0, "(manager「Init] : InputMouseがNULL", "ERROR", MB_OK); }

	if (m_pInputMouse == NULL) { m_pInputMouse = m_pInputMouse->Create(hInstance, hWnd); }


	// レンダラー
	if (m_pRenderer == NULL)		{ m_pRenderer = new CRenderer; }
	else							{ MessageBox(0, "(manager「Init] : RendererがNULLではない", "ERROR", MB_OK); }
	if (m_pRenderer != NULL)		{ if (FAILED(m_pRenderer->Init(hWnd, bWindow))) { return -1; } }
	else							{ MessageBox(0, "(manager「Init] : RendererがNULL", "ERROR", MB_OK); }

	// カメラ
	if (m_pCamara == NULL)			{ m_pCamara = new CCamera; }
	else							{ MessageBox(0, "(manager「Init] : CameraがNULLではない", "ERROR", MB_OK); }
	if (m_pCamara != NULL)			{ if (FAILED(m_pCamara->Init())) { return -1; } }
	else							{ MessageBox(0, "(manager「Init] : CameraがNULL", "ERROR", MB_OK); }

	// ライト
	if (m_pLight == NULL)			{ m_pLight = new CLight; }
	else							{ MessageBox(0, "(manager「Init] : LightがNULLではない", "ERROR", MB_OK); }
	if (m_pLight != NULL)			{ if (FAILED(m_pLight->Init())) { return -1; } }
	else							{ MessageBox(0, "(manager「Init] : LightがNULL", "警告", MB_OK); }

	// デバック
	if (m_pDebug == NULL)			{ m_pDebug = new CDebugProc; }
	else							{ MessageBox(0, "(manager「Init] : DebugがNULLではない", "ERROR", MB_OK); }
	if (m_pDebug != NULL)			{ m_pDebug->Init(); }
	else							{ MessageBox(0, "(manager「Init] : DebugがNULL", "警告", MB_OK); }

	// テクスチャ読み込み
	CTexture::Load();

	SetMode(MODE_OPTION);	// モード設定
	m_bPause = false;		// ポーズ使用状態
	return S_OK;
}

//=======================================================//
// 終了処理(CManager)
//=======================================================//
void CManager::Uninit(void)
{
	//********************************************//
	//
	//	* 各フレームワークのメモリ存在時・終了処理
	//	[NULL以外の時、終了処理]
	//	[処理完了後、メモリ破棄]
	//	[全行程終了後、値をNULLに再設定]
	//
	//********************************************//

	CTexture::Unload();
	if (m_pSound			!= NULL) { m_pSound->Uninit();			delete m_pSound;			m_pSound			= NULL; }	// サウンド
	if (m_pInputKeyboard	!= NULL) { m_pInputKeyboard->Uninit();	delete m_pInputKeyboard;	m_pInputKeyboard	= NULL; }	// キーボード
	if (m_pInputJoypad		!= NULL) { m_pInputJoypad->Uninit();	delete m_pInputJoypad;		m_pInputJoypad		= NULL; }	// キーボード
	//if (m_pInputDIPad		!= NULL) { m_pInputDIPad->Uninit();		delete m_pInputDIPad;		m_pInputDIPad		= NULL; }	// DInputパッド
	//if (m_pInputXPad		!= NULL) { m_pInputXPad->Uninit();		delete m_pInputXPad;		m_pInputXPad		= NULL; }	// Xパッド
	if (m_pInputMouse		!= NULL) { m_pInputMouse->Uninit();		delete m_pInputMouse;		m_pInputMouse = NULL; }	// マウス


	switch (m_mode)
	{	// モード
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame		!= NULL) { m_pGame->Uninit();		delete m_pGame;		m_pGame		= NULL; }
		break;
	}

	if (m_pRenderer != NULL) { m_pRenderer->Uninit();	delete m_pRenderer;	m_pRenderer = NULL; }	// レンダラー
	if (m_pCamara	!= NULL) { m_pCamara->Uninit();		delete m_pCamara;	m_pCamara	= NULL; }	// カメラ
	if (m_pLight	!= NULL) { m_pLight->Uninit();		delete m_pLight;	m_pLight	= NULL; }	// ライト
	if (m_pDebug != NULL)	 { m_pDebug->Uninit();		delete m_pDebug;	m_pDebug = NULL; }		// デバック

}

//=======================================================//
// 更新処理(CManager)
//=======================================================//
void CManager::Update(void)
{
	//********************************************//
	//
	//	* 各フレームワークの更新処理
	//	[NULL以外の時、更新処理]
	//
	//	MODE_GAME時	[ポーズ処理]
	//
	//********************************************//

	if (m_pInputKeyboard	!= NULL) { m_pInputKeyboard->Update(); }	// キーボード
	if (m_pInputJoypad		!= NULL) { m_pInputJoypad->Update(); }		// コントローラー
	//if (m_pInputDIPad		!= NULL) { m_pInputDIPad->Update(); }		// DInputパッド
	//if (m_pInputXPad		!= NULL) { m_pInputXPad->Update(); }		// Xパッド
	if (m_pInputMouse		!= NULL) { m_pInputMouse->Update(); }		// マウス


	switch (m_mode)
	{	// モード
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_P) && CFade::GetFade() == CFade::FADE_NONE || CManager::GetInputJoypad()->GetTrigger(XINPUT_GAMEPAD_START) == true && CFade::GetFade() == CFade::FADE_NONE)
			{
				// ポーズだよ(｀･ω･´)
			}

			m_pGame->Update();
		}
		break;
	}

	if (m_pRenderer != NULL) { m_pRenderer->Update(); }	// レンダラー
	if (m_pCamara	!= NULL) { m_pCamara->Update(); }	// カメラ
	if (m_pLight	!= NULL) { m_pLight->Update(); }	// ライト
}

//=======================================================//
// 描画処理(CManager)
//=======================================================//
void CManager::Draw(void)
{
	//********************************************//
	//
	//	* 各フレームワークの描画処理
	//	レンダラー	[NULL以外の時、描画処理]
	//	カメラ		[NULL以外の時、設定処理]
	//
	//********************************************//

	if (m_pRenderer != NULL) { m_pRenderer->Draw(); }		// レンダラー
	if (m_pCamara	!= NULL) { m_pCamara->SetCamera(); }	// カメラ
}

//=======================================================//
// 設定処理(GameMode)(CManager)
//=======================================================//
void CManager::SetMode(CManager::MODE mode)
{
	//********************************************//
	//
	//	* 各ゲームモードの設定処理
	//	[ゲームの状態を取得]
	//	[NULL以外の時、モード設定と参照]
	//	[モードが違った場合、開放処理へ]
	//	[その後、終了処理 → メモリ破棄 → NULL代入]
	//
	//********************************************//

	// ゲーム状態取得
	CGame::MODEGAME pGame = m_pGame->GetGame();

	switch (m_mode)
	{	// モード（現在）
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{
			// モードが違ったら
			if (m_mode != mode) { m_pGame->Unload(); }

			// 正常な場合
			m_pGame->Uninit(); delete m_pGame; m_pGame = NULL;
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM002);
		break;
	}

	// 文字列確保
	char cName[32];

	switch (mode)
	{	// モード（次回）
	case CManager::MODE_GAME:		wsprintf(cName, "Game");		break;	// ゲーム
	}



	//********************************************//
	//
	//	* 各ゲームモードの設定処理
	//	[NULLの時、メモリ確保]
	//	[NULL以外の時、モード参照]
	//	[正常な場合、初期化処理 → タイトルのみプレイヤー数初期化]
	//	[モードが違った場合、読み込み処理へ]
	//
	//********************************************//

	switch (mode)
	{	// モード（次回）
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame == NULL)
		{	// メモリ確保
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				// モードが違ったら
				if (m_mode != mode) { m_pGame->Load(); }

				// 正常な場合
				m_pGame->Init();
			}
		}
		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM001);
		break;
	}

	m_mode = mode;	// モード変更
}
