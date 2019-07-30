//=============================================================================
//
// メイン処理 [input.h]
// Author : 荒谷由朗
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	NUM_KEY_MAX			(256)				// キーの最大数
#define	NUM_MOUSE_MAX		(8)					// マウスの最大数
#define MAX_JOYPAD			(3)					// 接続できるパッドの数
#define REPEAT_FRAME		(20)				// リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER		(6)					// リピート情報を起動するフレームの間隔
#define	NUM_BUTTON_MAX		(32)				// キーの最大数

//=====================
//	   基本クラス
//=====================
class CInput
{
public:// 誰でも扱える
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void	Uninit(void);
	virtual void	Update(void) = 0;
protected://個人でのみ使う
	static LPDIRECTINPUT8	m_pInput;		// DirerctInputオブジェクトへのポインタ //全員が一個だけもっていればいいからstatic
	LPDIRECTINPUTDEVICE8	m_pDevice;		// 入力デバイス（キーボード）へのポインタ
};

//=====================
//	   キーボードの派生クラス
//=====================
class CInputKeyboard : public CInput
{
public:// 誰でも扱える
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void	Uninit(void);
	void	Update(void);
	bool	GetKeyboardPress(int nKey);		// キーボードプレス
	bool	GetKeyboardTrigger(int nKey);		// キーボードトリガー
	bool	GetKeyboardAny(int nNumber);	// キーボードトリガー
	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);
private:// 個人でのみ使う
	BYTE m_aKeyState[NUM_KEY_MAX];		// キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードの入力情報 (トリガー情報)
};

//=====================
//	   マウスの派生クラス
//=====================
class CInputMouse : public CInput
{
public:// 誰でも扱える
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void	Uninit(void);
	void	Update(void);
	bool	GetMousePress(int Key);			// マウスプレス
	bool	GetMouseTrigger(int Key);		// マウストリガー
	bool	GetMouseRelease(int Key);		// マウスリリース
	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);
	static D3DXVECTOR3 *GetMousePos(void) { return &m_MousePos; }
	void	MouseLoop(void);

private://個人でのみ使う
	RECT					m_moveRect;				// 画面上で動ける範囲
	int						m_x;					// X座標
	int						m_y;					// Y座標
	bool					m_lButton;				// 左ボタン
	bool					m_rButton;				// 右ボタン
	bool					m_cButton;				// 真ん中ボタン
	int						m_moveAdd;				// 移動量
	RECT					m_imgRect;				// マウス用画像矩形
	int						m_imgWidth;				// マウス画像幅
	int						m_imgHeight;			// マウス画像高さ
	POINT					m_Pos, m_WPos;			// マウスの座標
	static D3DXVECTOR3		m_MousePos;				// マウスの座標
	DIMOUSESTATE2			m_MouseState;			// マウスの状態
	DIMOUSESTATE2			m_MouseStatePress;		// マウスの入力情報（プレス情報）
	DIMOUSESTATE2			m_MouseStateTrigger;	// マウスの入力情報（トリガー情報）
	DIMOUSESTATE2			m_MouseStateRelease;	// マウスの入力情報（リリース情報)
};

//=====================
//	   コントローラーの派生クラス
//=====================
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		BUTTON_A = 0,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_LB,
		BUTTON_RB,
		BUTTON_BACK,
		BUTTON_START,
		BUTTON_LS,
		BUTTON_RS,
		BUTTON_10,
		BUTTON_11,
		BUTTON_12,
		BUTTON_13,
		BUTTON_14,
		BUTTON_15,
		POV_UP_RIGHT,	// 十字キー　右上
		POV_UP_LEFT,	// 十字キー　左上
		POV_DOWN_RIGHT,	// 十字キー　右下
		POV_DOWN_LEFT,	// 十字キー　左下
		POV_UP,			// 十字キー　上
		POV_DOWN,		// 十字キー　下
		POV_RIGHT,		// 十字キー　右
		POV_LEFT,		// 十字キー　左
		STICK_L_UP,		// 左スティック　上
		STICK_L_DOWN,	// 左スティック　下
		STICK_L_RIGHT,	// 左スティック　右
		STICK_L_LEFT,	// 左スティック　左
		STICK_R_UP,		// 右スティック　上
		STICK_R_DOWN,	// 右スティック　下
		STICK_R_RIGHT,	// 右スティック　右
		STICK_R_LEFT,	// 右スティック　左
		MAX_BUTTON,
	}DIJS_BUTTON;
	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void	Uninit(void);
	void	Update(void);
	bool	GetPress(int nKey);
	bool	GetTrigger(int nKey);
	bool	GetRelease(int nKey);
	bool	GetJoyPadAny(int nKey);

	float GetLeftAxiz(void);
	float GetRightAxiz(void);
	float GetRadian(void);

	static CInputJoypad *Create(HINSTANCE hInstance, HWND hWnd);

protected:
	BYTE	m_aKeyState[NUM_BUTTON_MAX];			// コントローラーの入力情報(プレス)
	BYTE	m_aKeyStateTrigger[NUM_BUTTON_MAX];		// コントローラーの入力情報(トリガー)
	BYTE	m_aKeyStateRelease[NUM_BUTTON_MAX];		// コントローラーの入力情報(リリース)
	float	m_LeftAxizX;								// 左スティックのX軸
	float	m_LeftAxizY;								// 左スティックのY軸
	float	m_RightAxizX;								// 右スティックのX軸
	float	m_RightAxizY;								// 右スティックのY軸
	float	m_Radian;									// 角度
};
#endif