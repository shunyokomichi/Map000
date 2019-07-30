//=======================================================//
//
// マネージャー処理 [Manager.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"	// メイン

//*******************************************************//
// マクロ定義
//*******************************************************//
#define MAX_PLAYER	(2)	// プレイヤー最大数

//*******************************************************//
// 前方宣言
//*******************************************************//
class CRenderer;		// レンダラー
class CInputKeyboard;	// キーボード
class CInputJoypad;		// コントローラー
//class CInputDInputPad;// DInoutパッド
//class CInputXPad;		// Xパッド
class CInputMouse;		// マウス
class CCamera;			// カメラ
class CLight;			// ライト
class CGame;			// ゲーム
class CPause;			// ポーズ
class CSound;			// サウンド
class CDebugProc;		// デバック

//*******************************************************//
// クラス定義
//*******************************************************//
class CManager
{// CManager
public:
	typedef enum
	{	// モード種類
		MODE_NONE = 0,	// 何もしていない
		MODE_GAME,		// ゲーム
		MODE_MAX		// 最大数
	}MODE;

	CManager() {};											// コンストラクタ(中身無し)
	virtual ~CManager() {};								// デストラクタ(中身無し)

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		// 初期化処理
	virtual void Uninit(void);							// 終了処理
	virtual void Update(void);							// 更新処理
	virtual void Draw(void);							// 描画処理

	static CRenderer		*GetRenderer(void)		{ return m_pRenderer; }			// レンダラー取得
	static CInputKeyboard	*GetInputKeyboard(void)	{ return m_pInputKeyboard; }	// キーボード取得
	static CInputJoypad		*GetInputJoypad(void)	{ return m_pInputJoypad; }		// コントローラー取得
	//static CInputDInputPad	*GetInputDIPad(void)	{ return m_pInputDIPad; }		// DIパッド取得
	//static CInputXPad		*GetInputXPad(void)		{ return m_pInputXPad; }		// Xパッド取得
	static CInputMouse		*GetInputMouse(void)	{ return m_pInputMouse; }		// マウス取得
	static CCamera			*GetCamera(void)		{ return m_pCamara; }			// カメラ取得
	static CLight			*GetLight(void)			{ return m_pLight; }			// ライト取得
	static CSound			*GetSound(void)			{ return m_pSound; }			// サウンド取得

	static void SetMode(MODE mode);					// モード設定
	static MODE GetMode(void) { return m_mode; }	// モード取得

	// プレイ人数
	static void SetPlayNum(int nPlayerNum)	{ if (nPlayerNum <= MAX_PLAYER)m_nPlayNum = nPlayerNum; else m_nPlayNum = MAX_PLAYER; }
	static int GetPlayNum(void)				{ return m_nPlayNum; }

private:
	static MODE m_mode;							// モード

	static CRenderer		*m_pRenderer;		// レンダラー
	static CInputKeyboard	*m_pInputKeyboard;	// キーボード
	static CInputJoypad		*m_pInputJoypad;	// コントローラー
	//static CInputDInputPad	*m_pInputDIPad;		// DIパッド
	//static CInputXPad		*m_pInputXPad;		// Xパッド
	static CInputMouse		*m_pInputMouse;		// マウス
	static CCamera			*m_pCamara;			// カメラ
	static CLight			*m_pLight;			// ライト
	static CSound			*m_pSound;			// サウンド
	static CGame			*m_pGame;			// ゲーム

	static int m_nPlayNum;						// プレイ人数
	static CPause *m_pPause;					// ポーズ
	static CDebugProc		*m_pDebug;			// デバック
	bool m_bPause;								// ポーズ状態
};
#endif