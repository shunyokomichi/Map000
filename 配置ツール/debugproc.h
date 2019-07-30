//**************************************************************************
//
//				デバック表示処理[debugproc.h]
//				Auther : Shun Yokomichi
//
//**************************************************************************
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//**************************************************************************
//				マクロ定義
//**************************************************************************
#define			MAX_WORD		(16250)

//**************************************************************************
//				クラス定義（デバック表示）
//**************************************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理

	// 静的メンバ関数
	static void Print(char *fmt, ...);		// 文字表示
	static void Draw(void);					// 描画処理

private:
	static LPD3DXFONT m_pFont;
	static char m_aStr[MAX_WORD];
	static bool m_bUse;
};

#endif // !_DEBUGPROC_H_
